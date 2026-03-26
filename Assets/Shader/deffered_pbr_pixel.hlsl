struct PS_INPUT
{
    float4 v_Position : SV_Position;
    float2 v_UV : TEXCOORD0;
};

[[vk::binding(0, 2)]] Texture2D t_RenderTargetAlbedoAO;
[[vk::binding(1, 2)]] Texture2D t_RenderTargetNormalRough;
[[vk::binding(2, 2)]] Texture2D t_RenderTargetMetallicEmit;
[[vk::binding(3, 2)]] Texture2D t_RenderTargetDepth;
[[vk::binding(5, 2)]] SamplerState s_RenderTargetSampler;


float4 main(PS_INPUT input) : SV_TARGET
{
    
    return float4(1.0, 1.0, 0.0, 1.0);
}