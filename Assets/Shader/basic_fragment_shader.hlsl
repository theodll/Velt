struct FragInput {
    float4 v_Position : SV_POSITION;
    [[vk::location(1)]] float2 v_UV : TEXCOORD1; 
};

struct MaterialUBO {
    float4 u_BaseColorFactor;
    float u_Metallicness;
    float u_Roughness;
    float u_AbientOcclusion;
    float3 u_EmissiveColor;
};

cbuffer u_Material : register(b0, space1) {
    MaterialUBO u_Material;
};


[[vk::binding(1, 1)]] Texture2D t_AlbedoMap;
[[vk::binding(2, 1)]] Texture2D t_NormalMap;
[[vk::binding(3, 1)]] Texture2D t_MetallicMap;
[[vk::binding(4, 1)]] Texture2D t_RoughnessMap;
// [[vk::binding(5, 1)]] Texture2D t_AmbientOcclusion;
[[vk::binding(6, 1)]] SamplerState s_TextureSampler;

float4 main(FragInput input) : SV_TARGET
{
    return t_AlbedoMap.Sample(s_TextureSampler, input.v_UV) * u_Material.u_BaseColorFactor;
}