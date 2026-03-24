struct VS_OUTPUT
{
    float4 v_Position : SV_Position;
    float2 v_UV : TEXCOORD0;
};

VS_OUTPUT main(uint vertexID : SV_VertexID)
{
    VS_OUTPUT output;
    
    output.v_UV = float2((vertexID << 1) & 2, vertexID & 2);
    output.v_Position = float4(output.v_UV * float2(2.0, -2.0) + float2(-1.0, 1.0), 0.0, 1.0);
    
    return output;
}