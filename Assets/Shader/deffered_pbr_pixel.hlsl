#define MAX_LIGHTS 32
#define PI 3.14159265359

struct PS_INPUT
{
    float4 v_Position : SV_Position;
    float2 v_UV : TEXCOORD0;
};

struct CAMERA_UBO
{
    float4x4 viewProj;
    float4x4 invViewProj;
    float3 cameraPos;
};

struct LIGHT
{
    float4 Color;
    float3 Position;
    float Intensity;
};

struct LIGHT_UBO
{
    LIGHT Lights[MAX_LIGHTS];
    float3 _pad01;
    int Count;
};

// GBUFFER LAYOUT
// | Texture          | R (8Bit)      | G (8Bit)   | B (8Bit)   | A (8Bit)          |
// |------------------|---------------|------------|------------|-------------------|
// | Albedo AO        | Albedo R      | Albedo G   | Albedo B   | Ambient Occlusion |
// | Normal Roughness | Normal X      | Normal Y   | Normal Z   | Roughness         |
// | Metal Emission   | Metallicness  | Emission R | Emission G | Emission B        |
// | Depth            | Depth (32Bit) |            |            |                   |

[[vk::binding(0, 2)]] Texture2D t_RenderTargetAlbedoAO;
[[vk::binding(1, 2)]] Texture2D t_RenderTargetNormalRough;
[[vk::binding(2, 2)]] Texture2D t_RenderTargetMetallicEmit;
[[vk::binding(3, 2)]] Texture2D t_RenderTargetDepth;
[[vk::binding(4, 2)]] SamplerState s_RenderTargetSampler;

[[vk::binding(5, 2)]] ConstantBuffer<CAMERA_UBO> u_CameraUBO;
[[vk::binding(6, 2)]] ConstantBuffer<LIGHT_UBO> u_LightUBO;

float3 ReconstructWorldPosition(float2 uv, float depth, float4x4 invViewProj);
float NormalDistributionFunction(float alpha, float3 N, float3 H);
float GeometryShadowingFunction(float alpha, float3 N, float3 X);
float SmithModel(float alpha, float3 N, float3 V, float3 L);
float3 FresnelSchlickFunction(float3 F0, float3 V, float3 H);

float4 main(PS_INPUT input) : SV_TARGET
{
    float2 uv = input.v_UV;
    
    float depth = t_RenderTargetDepth.Sample(s_RenderTargetSampler, uv).r;
    float4 albedoAO = t_RenderTargetAlbedoAO.Sample(s_RenderTargetSampler, uv);
    float4 normalRough = t_RenderTargetNormalRough.Sample(s_RenderTargetSampler, uv);
    float4 metallicEmit = t_RenderTargetMetallicEmit.Sample(s_RenderTargetSampler, uv);

    float3 albedo = albedoAO.rgb;
    float3 N = normalize(normalRough.rgb * 2.0f - 1.0f);
    float roughness = max(normalRough.a, 0.05);
    float metallicness = metallicEmit.r;
    float3 emission = metallicEmit.gba;
    
    float alphaGGX = roughness * roughness;

    float3 pixelPos = ReconstructWorldPosition(uv, depth, u_CameraUBO.invViewProj);
    float3 V = normalize(u_CameraUBO.cameraPos - pixelPos);
    
    float3 F0 = lerp(float3(0.04, 0.04, 0.04), albedo, metallicness);
    
    float3 totalLighting = float3(0.0, 0.0, 0.0);

    int lightCount = min(u_LightUBO.Count, MAX_LIGHTS);
    
    for (int i = 0; i < lightCount; i++)
    {
        LIGHT light = u_LightUBO.Lights[i];
        
        float3 unnormalizedL = light.Position - pixelPos;
        float distance = length(unnormalizedL);
        float3 L = unnormalizedL / distance;
        float3 H = normalize(V + L);

        float attenuation = 1.0 / (distance * distance + 0.0001);
        float3 radiance = light.Color.rgb * light.Intensity * attenuation;

        float3 F = FresnelSchlickFunction(F0, V, H);
        float3 Ks = F;
        float3 Kd = (float3(1.0, 1.0, 1.0) - Ks) * (1.0 - metallicness);

        float D = NormalDistributionFunction(alphaGGX, N, H);
        float G = SmithModel(alphaGGX, N, V, L);
        
        float NdotL = max(dot(N, L), 0.0);
        float NdotV = max(dot(N, V), 0.0);
        
        float denom = 4.0 * NdotV * NdotL + 0.0001;
        float3 specular = (D * G * F) / denom;
        
        float3 diffuse = Kd * albedo / PI;

        totalLighting += (diffuse + specular) * radiance * NdotL;
    }

    totalLighting += (emission * 2.0);

    totalLighting = totalLighting / (totalLighting + float3(1.0, 1.0, 1.0));

    return float4(totalLighting, 1.0);
}

float3 ReconstructWorldPosition(float2 uv, float depth, float4x4 invViewProj)
{
    float4 ndc;
    ndc.xy = uv * 2.0f - 1.0f;
    ndc.z = depth;
    ndc.w = 1.0f;
    
    float4 worldPos = mul(invViewProj, ndc);
    worldPos.xyz /= worldPos.w;
    
    return worldPos.xyz;
}

float NormalDistributionFunction(float alpha, float3 N, float3 H)
{
    float numerator = pow(alpha, 2.0f);
    
    float NdotH = max(dot(N, H), 0.0f);
    float denominator = PI * pow(pow(NdotH, 2.0f) * (pow(alpha, 2.0f) - 1.0f) + 1.0f, 2.0f);
    denominator = max(denominator, 0.0000000001);
    
    return numerator / denominator;
}

float GeometryShadowingFunction(float alpha, float3 N, float3 X)
{
    float numerator = max(dot(N, X), 0.0f);
    
    float k = alpha / 2.0f;
    float denominator = max(dot(N, X), 0.0f) * (1.0f - k) + k;
    denominator = max(denominator, 0.000000001);
    
    return numerator / denominator;
}

float SmithModel(float alpha, float3 N, float3 V, float3 L)
{
    return GeometryShadowingFunction(alpha, N, V) * GeometryShadowingFunction(alpha, N, L);
}

float3 FresnelSchlickFunction(float3 F0, float3 V, float3 H)
{
    return F0 + (float3(1.0f, 1.0f, 1.0f) - F0) * pow(1.0f - max(dot(V, H), 0.0f), 5.0f);
}