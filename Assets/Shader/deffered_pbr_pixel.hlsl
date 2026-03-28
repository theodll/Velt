struct PS_INPUT
{
    float4 v_Position : SV_Position;
    float2 v_UV : TEXCOORD0;
};

struct CAMERA_UBO
{
    float4x4 viewProj;
    float4x4 invViewProj;
};

[[vk::binding(0, 0)]] cbuffer u_CameraUBO
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
// |                  |               |            |            |                   |


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

float4 main(PS_INPUT input) : SV_TARGET
{
    float depth = t_RenderTargetDepth.Sample(s_RenderTargetSampler, input.v_UV);
    float4 albedoAOmap = t_RenderTargetAlbedoAO.Sample(s_RenderTargetSampler, input.v_UV);
    float4 normalRoughMap = t_RenderTargetNormalRough.Sample(s_RenderTargetSampler, input.v_UV);
    float4 metalEmitMap = t_RenderTargetMetallicEmit.Sample(s_RenderTargetSampler, input.v_UV);
    
    
    float4 albedoColor = float4(albedoAOmap.rgb, 1.0f);
    float3 worldNormal = normalRoughMap.rgb * 2.0f - 1.0f;
    float roughness = normalRoughMap.a;
    float metallicness = metalEmitMap.r;
    
    
    float3 worldPos = ReconstructWorldPosition(input.v_UV, depth, u_CameraUBO.invViewProj);  
    
    return float4(worldNormal, 1.0f);
    //return albedoColor;
}