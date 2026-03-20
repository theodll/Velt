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

Texture2D u_AlbedoTexture : register(t1, space1);


SamplerState u_AlbedoSampler : register(s1, space1);

float4 main(FragInput input) : SV_TARGET
{
    return u_AlbedoTexture.Sample(u_AlbedoSampler, input.v_UV) * u_Material.u_BaseColorFactor;
}