// Sources and Inspiration:
// TheCherno's Sparky Engine (https://github.com/thecherno/sparky) - AdvancedLighting.hlsl
// TheCherno & StudioChernos Hazel "Dev" Engine (https://github.com/studiocherno/hazel)
// LearnOpenGL (https://learnopengl.com/)
// Physically Based Rendering 2 & 4
// Real Time Rendering

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

[[vk::binding(6, 2)]]
cbuffer u_CameraUBO
{
    CAMERA_UBO u_CameraUBO;
};

[[vk::binding(0, 2)]] Texture2D t_RenderTargetAlbedoAO;
[[vk::binding(1, 2)]] Texture2D t_RenderTargetNormalRough;
[[vk::binding(2, 2)]] Texture2D t_RenderTargetMetallicEmit;
[[vk::binding(3, 2)]] Texture2D t_RenderTargetDepth;
[[vk::binding(5, 2)]] SamplerState s_RenderTargetSampler;

// GBUFFER LAYOUT
// | Texture          | R (8Bit)      | G (8Bit)   | B (8Bit)   | A (8Bit)          |
// |------------------|---------------|------------|------------|-------------------|
// | Albedo AO        | Albedo R      | Albedo G   | Albedo B   | Ambient Occlusion |
// | Normal Roughness | Normal X      | Normal Y   | Normal Z   | Roughness         |
// | Metal Emission   | Metallicness  | Emission R | Emission G | Emission B        |
// | Depth            | Depth (32Bit) |            |            |                   |

#define PI 3.1415926535897932384626433832795f
#define GAMMA 2.2f

float3 ReconstructWorldPosition(float2 uv, float depth, float4x4 invViewProj)
{
    float4 ndc;
    ndc.x = uv.x * 2.0f - 1.0f;
    ndc.y = (1.0f - uv.y) * 2.0f - 1.0f; // Vulkan: flip Y (UV origin top-left, NDC Y points up)
    ndc.z = depth;
    ndc.w = 1.0f;

    float4 worldPos = mul(invViewProj, ndc);
    worldPos.xyz /= worldPos.w;

    return worldPos.xyz;
}

// Fresnel-Schlick approximation
float3 FresnelSchlick(float cosTheta, float3 F0)
{
    return F0 + (1.0f - F0) * pow(clamp(1.0f - cosTheta, 0.0f, 1.0f), 5.0f);
}

// GGX/Towbridge-Reitz normal distribution function
float DistributionGGX(float3 N, float3 H, float roughness)
{
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0f);
    float NdotH2 = NdotH * NdotH;

    float nom = a2;
    float denom = (NdotH2 * (a2 - 1.0f) + 1.0f);
    denom = PI * denom * denom;

    return nom / denom;
}

// Schlick-GGX geometry function
float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0f);
    float k = (r * r) / 8.0f;

    float nom = NdotV;
    float denom = NdotV * (1.0f - k) + k;

    return nom / denom;
}

// Smith's method (combines view and light geometry)
float GeometrySmith(float3 N, float3 V, float3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0f);
    float NdotL = max(dot(N, L), 0.0f);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}

// Main PBR lighting calculation
float3 CalculatePBRLight(
    float3 fragPos,
    float3 viewPos,
    float3 normal,
    float3 albedo,
    float metallic,
    float roughness,
    float3 lightPos,
    float3 lightColor,
    float lightIntensity
)
{
    float3 V = normalize(viewPos - fragPos);
    float3 L = normalize(lightPos - fragPos);
    float3 H = normalize(V + L);

    float distance = length(lightPos - fragPos);
    float attenuation = 1.0f / (distance * distance);
    float3 radiance = lightColor * lightIntensity * attenuation;

    // Calculate F0 (base reflectivity): 0.04 for dielectrics, albedo for metals
    float3 F0 = float3(0.04f, 0.04f, 0.04f);
    F0 = lerp(F0, albedo, metallic);

    // Cook-Torrance BRDF
    float3 F = FresnelSchlick(max(dot(H, V), 0.0f), F0);
    float D = DistributionGGX(normal, H, roughness);
    float G = GeometrySmith(normal, V, L, roughness);

    float3 kS = F;
    float3 kD = float3(1.0f, 1.0f, 1.0f) - kS;
    kD *= (1.0f - metallic); // Metals have no diffuse

    float3 numerator = D * F * G;
    float denominator = 4.0f * max(dot(normal, V), 0.0f) * max(dot(normal, L), 0.0f) + 0.0001f;
    float3 specular = numerator / denominator;

    float NdotL = max(dot(normal, L), 0.0f);
    float3 Lo = (kD * albedo / PI + specular) * radiance * NdotL;

    return Lo;
}

float4 main(PS_INPUT input) : SV_TARGET
{
    // --- Sample GBuffer ---
    float depth = t_RenderTargetDepth.Sample(s_RenderTargetSampler, input.v_UV).r;
    float4 albedoAOSample = t_RenderTargetAlbedoAO.Sample(s_RenderTargetSampler, input.v_UV);
    float4 normalRoughSample = t_RenderTargetNormalRough.Sample(s_RenderTargetSampler, input.v_UV);
    float4 metallicEmitSample = t_RenderTargetMetallicEmit.Sample(s_RenderTargetSampler, input.v_UV); // FIX: was never sampled

    // --- Unpack GBuffer ---
    // Albedo is stored in sRGB, convert to linear for lighting calculations
    float3 albedo = pow(albedoAOSample.rgb, float3(GAMMA, GAMMA, GAMMA));
    float ambientOcclusion = albedoAOSample.a;

    // Normals are stored in [0,1], unpack to [-1,1] and renormalize
    float3 worldNormal = normalize(normalRoughSample.rgb * 2.0f - 1.0f);
    float roughness = normalRoughSample.a;

    // FIX: Read metallic and emission from the MetallicEmit GBuffer texture
    float metallicness = metallicEmitSample.r;
    float3 emission = metallicEmitSample.gba; // G=EmitR, B=EmitG, A=EmitB

    // --- Reconstruct world position from depth ---
    float3 worldPos = ReconstructWorldPosition(input.v_UV, depth, u_CameraUBO.invViewProj);
    float3 viewPos = u_CameraUBO.cameraPos.xyz;

    // Hardcoded point light in the center (0, 0, 0)
    float3 lightPos = u_CameraUBO.cameraPos;
    float3 lightColor = float3(1.0f, 0.796f, 0.098f);
    float lightIntensity = 100.0f;

    // --- Calculate PBR lighting ---
    float3 Lo = CalculatePBRLight(
        worldPos,
        viewPos,
        worldNormal,
        albedo,
        metallicness,
        roughness,
        lightPos,
        lightColor,
        lightIntensity
    );

    // Ambient lighting (simple approximation, modulated by AO)
    float3 ambient = albedo * 0.03f * ambientOcclusion;

    // FIX: Emission must be added in linear space, BEFORE tone mapping
    // (adding it after tone mapping crushes bright emissive values to near-white)
    float3 color = ambient + Lo + emission;

    // Tone mapping (Reinhard operator)
    color = color / (color + float3(1.0f, 1.0f, 1.0f));

    // Gamma correction: convert linear -> sRGB for display
    color = pow(color, float3(1.0f / GAMMA, 1.0f / GAMMA, 1.0f / GAMMA));

    return float4(color, 1.0f);
}