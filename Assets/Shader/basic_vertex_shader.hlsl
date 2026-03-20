struct VertInput {
    [[vk::location(0)]] float3 a_Position : POSITION; 
    [[vk::location(1)]] float3 a_Normal   : NORMAL;
    [[vk::location(2)]] float3 a_Tangent  : TANGENT;
    [[vk::location(3)]] float3 a_Binormal : BINORMAL;
    [[vk::location(4)]] float2 a_UV       : TEXCOORD0;
};

struct FragInput {
    float4 v_Position : SV_POSITION; 
    [[vk::location(1)]] float2 v_UV : TEXCOORD1; 
};

struct Push {
    float4x4 transform;
};

struct CameraUBO {
    float4x4 viewProj;
};

cbuffer u_ViewProjection : register(b0, space0) {
    CameraUBO u_ViewProjection;
}

[[vk::push_constant]] Push pc_ModelTransform;

FragInput main(VertInput input)
{
    FragInput result;

    float4 worldPos = mul(pc_ModelTransform.transform, float4(input.a_Position, 1.0));
    float4 clipPos  = mul(u_ViewProjection.viewProj, worldPos);

    result.v_Position = clipPos;
    result.v_UV = input.a_UV;

    return result;
}