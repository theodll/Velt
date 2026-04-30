#define PI 3.14159265359
#define MAX_LIGHTS 1024

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

[[vk::binding(0, 2)]] Texture2D t_RenderTargetAlbedoAO;
[[vk::binding(1, 2)]] Texture2D t_RenderTargetNormalRough;
[[vk::binding(2, 2)]] Texture2D t_RenderTargetMetallicEmit;
[[vk::binding(3, 2)]] Texture2D t_RenderTargetDepth;
[[vk::binding(4, 2)]] SamplerState s_RenderTargetSampler;

[[vk::binding(5, 2)]] ConstantBuffer<CAMERA_UBO> u_CameraUBO;
[[vk::binding(6, 2)]] ConstantBuffer<LIGHT_UBO> u_LightUBO;

float3 ReconstructWorldPosition(float2 uv, float depth, float4x4 invViewProj)
{
    float4 ndc;
    ndc.x = uv.x * 2.0f - 1.0f;
    ndc.y = (1.0f - uv.y) * 2.0f - 1.0f;
    ndc.z = depth;
    ndc.w = 1.0f;
    
    float4 worldPos = mul(invViewProj, ndc);
    worldPos.xyz /= worldPos.w;
    return worldPos.xyz;
}

float NormalDistributionFunction(float alpha, float3 N, float3 H)
{
    float a2 = alpha * alpha;
    float NdotH = max(dot(N, H), 0.0f);
    float denom = (NdotH * NdotH * (a2 - 1.0f) + 1.0f);
    return a2 / (PI * denom * denom + 0.0000001f);
}

float GeometrySchlickGGX(float NdotX, float k)
{
    return NdotX / (NdotX * (1.0f - k) + k);
}

float SmithModel(float alpha, float3 N, float3 V, float3 L)
{
    float k = (alpha + 1.0f) * (alpha + 1.0f) / 8.0f;
    return GeometrySchlickGGX(max(dot(N, V), 0.0f), k) * GeometrySchlickGGX(max(dot(N, L), 0.0f), k);
}

float3 FresnelSchlickFunction(float3 F0, float3 V, float3 H)
{
    return F0 + (1.0f - F0) * pow(clamp(1.0f - max(dot(V, H), 0.0f), 0.0f, 1.0f), 5.0f);
}

float4 main(PS_INPUT input) : SV_TARGET
{
    float2 uv = input.v_UV;
    float depth = t_RenderTargetDepth.Sample(s_RenderTargetSampler, uv).r;
    
    if (depth >= 1.0f)
        discard;

    float4 albedoAO = t_RenderTargetAlbedoAO.Sample(s_RenderTargetSampler, uv);
    float4 normalRough = t_RenderTargetNormalRough.Sample(s_RenderTargetSampler, uv);
    float4 metallicEmit = t_RenderTargetMetallicEmit.Sample(s_RenderTargetSampler, uv);

    float3 albedo = albedoAO.rgb;
    float3 N = normalize(normalRough.rgb * 2.0f - 1.0f);
    float roughness = max(normalRough.a, 0.05f);
    float metallicness = metallicEmit.r;
    float3 emission = metallicEmit.gba;
    float ao = albedoAO.a;
    
    float alphaGGX = roughness * roughness;
    float3 pixelPos = ReconstructWorldPosition(uv, depth, u_CameraUBO.invViewProj);
    float3 V = normalize(u_CameraUBO.cameraPos - pixelPos);

    float3 F0 = lerp(float3(0.04, 0.04, 0.04), albedo, metallicness);
    float3 totalLighting = float3(0.0, 0.0, 0.0);

    for (int i = 0; i < u_LightUBO.Count; i++)
    {
        float3 L_vector = u_LightUBO.Lights[i].Position - pixelPos;
        float distSq = dot(L_vector, L_vector);
        float3 L = normalize(L_vector);
        float3 H = normalize(V + L);

        float attenuation = 1.0f / (distSq + 1.0f);
        float3 radiance = u_LightUBO.Lights[i].Color.rgb * u_LightUBO.Lights[i].Intensity * attenuation;

        float D = NormalDistributionFunction(alphaGGX, N, H);
        float G = SmithModel(roughness, N, V, L);
        float3 F = FresnelSchlickFunction(F0, V, H);
        
        float3 ks = F;
        float3 kd = (1.0f - ks) * (1.0f - metallicness);

        float NdotL = max(dot(N, L), 0.0f);
        float NdotV = max(dot(N, V), 0.0f);

        float3 numerator = D * G * F;
        float denominator = 4.0f * NdotV * NdotL + 0.0001f;
        float3 specular = numerator / denominator;

        totalLighting += (kd * albedo / PI + specular) * radiance * NdotL;
    }

    float3 ambient = float3(0.03, 0.03, 0.03) * albedo * ao;
    float3 finalColor = ambient + totalLighting + emission;

    finalColor = finalColor / (finalColor + 1.0f);
    finalColor = pow(finalColor, 1.0f / 2.2f);

    return float4(finalColor, 1.0f);
}