struct PS_INPUT
{
    float4 v_Position : SV_Position;
    float2 v_UV : TEXCOORD0;
};

float4 main(PS_INPUT input) : SV_TARGET
{
    return float4(1.0, 1.0, 0.0, 1.0);
}