struct VS_INPUT 
{
    [[vk::location(0)]] float3 a_Position : POSITION; 
    [[vk::location(1)]] float3 a_Normal   : NORMAL;
    [[vk::location(2)]] float3 a_Tangent  : TANGENT;
    [[vk::location(3)]] float3 a_Binormal : BINORMAL;
    [[vk::location(4)]] float2 a_UV       : TEXCOORD0;
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
    
    float4 worldPos = mul(input.a_Position, pc_ModelTransform.transform);
    output.v_Position = mul(worldPos, u_ViewProjection.viewProj);
     
    output.v_Normal = mul(input.a_Normal, pc_ModelTransform.transform);
    
    output.v_Tangent = mul(input.a_Tangent, pc_ModelTransform.transform);
    
    output.v_Binormal = mul(input.a_Binormal, pc_ModelTransform.transform);
    
    output.v_UV = input.a_UV;
    
    return output;
};

