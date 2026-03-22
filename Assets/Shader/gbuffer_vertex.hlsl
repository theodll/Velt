struct VS_INPUT
{
    [[vk::location(0)]] float3 a_Position : POSITION;
    [[vk::location(1)]] float3 a_Normal : NORMAL;
    [[vk::location(2)]] float3 a_Tangent : TANGENT;
    [[vk::location(3)]] float3 a_Binormal : BINORMAL;
    [[vk::location(4)]] float2 a_UV : TEXCOORD0;
};

struct VS_OUTPUT
{
    float4 v_Position : SV_POSITION;
    float3 v_Normal : NORMAL;
    float3 v_Tangent : TANGENT;
    float3 v_Binormal : BINORMAL;
    float2 v_UV : TEXCOORD0;
};

struct TRANSFORM_PC
{
    float4x4 transform;
};

struct CAMERA_UBO
{
    float4x4 viewProj;
};

cbuffer u_ViewProjection : register(b0, space0)
{
    CAMERA_UBO u_ViewProjection;
};

[[vk::push_constant]] TRANSFORM_PC pc_ModelTransform;

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;
    
    float4 worldPos = mul(pc_ModelTransform.transform, float4(input.a_Position, 1.0));
    output.v_Position = mul(u_ViewProjection.viewProj, worldPos);
     
    float3x3 normalMatrix = (float3x3) pc_ModelTransform.transform;
    output.v_Normal = mul(normalMatrix, input.a_Normal);
    output.v_Tangent = mul(normalMatrix, input.a_Tangent);
    output.v_Binormal = mul(normalMatrix, input.a_Binormal);
    
    output.v_UV = input.a_UV;
    
    return output;
}