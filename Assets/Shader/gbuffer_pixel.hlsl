struct PS_INPUT
{
    float4 v_Position : SV_POSITION;
    float3 v_Normal : NORMAL;
    float3 v_Tangent : TANGENT;
    float3 v_Binormal : BINORMAL;
    float2 v_UV : TEXCOORD0;
};

struct GBUFFER_OUT
{
    [[vk::location(0)]] float4 g_AlbedoAO : SV_Target0;
    [[vk::location(1)]] float4 g_NormalRough : SV_Target1;
    [[vk::location(2)]] float4 g_MetalEmit : SV_Target2;
    [[vk::location(3)]] uint g_EntityID : SV_Target3;
};

struct MATERIAL_UBO
{
    float4 BaseColorFactor;
    float Metallicness;
    float Roughness;
    float AbientOcclusion;
    float _pad0; 
    float3 EmissiveColor;
    float _pad1; 
};

cbuffer u_Material : register(b0, space1)
{
    MATERIAL_UBO u_Material;
};

[[vk::binding(1, 1)]] Texture2D t_AlbedoMap;
[[vk::binding(2, 1)]] Texture2D t_NormalMap;
[[vk::binding(3, 1)]] Texture2D t_MetallicMap;
[[vk::binding(4, 1)]] Texture2D t_RoughnessMap;
[[vk::binding(5, 1)]] SamplerState s_TextureSampler;

GBUFFER_OUT main(PS_INPUT input)
{
    GBUFFER_OUT output;
    
    float3 albedo = t_AlbedoMap.Sample(s_TextureSampler, input.v_UV).rgb;
    float3 normSample = t_NormalMap.Sample(s_TextureSampler, input.v_UV).rgb;
    float metallic = t_MetallicMap.Sample(s_TextureSampler, input.v_UV).r;
    float roughness = t_RoughnessMap.Sample(s_TextureSampler, input.v_UV).r;
    
    float3 tangentNormal = normSample * 2.0 - 1.0;

        float3 N = normalize(input.v_Normal);

        // float3 T = input.v_Tangent;
        // float3 B = input.v_Binormal;

        // if (length(T) < 0.0001)
        // {
        //     T = cross(N, float3(0.0, 1.0, 0.0));
        //     if (length(T) < 0.0001)
        //         T = cross(N, float3(1.0, 0.0, 0.0));
        // }

        // T = normalize(T - dot(T, N) * N);

        // if (length(B) < 0.0001)
        // {
        //     B = cross(N, T);
        // }
        // else
        // {
        //     B = normalize(B - dot(B, N) * N);
        // }

        // float3x3 TBN = float3x3(T, B, N);

    // float3 worldNormal = normalize(mul(tangentNormal, TBN));
    float3 worldNormal = N;

    output.g_AlbedoAO = float4(albedo * u_Material.BaseColorFactor.rgb, u_Material.AbientOcclusion);
    output.g_NormalRough = float4(worldNormal * 0.5 + 0.5, roughness * u_Material.Roughness);
    output.g_MetalEmit = float4(metallic * u_Material.Metallicness, u_Material.EmissiveColor);
    
    return output;
}