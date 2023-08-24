cbuffer cbColor : register(b0)
{
    float4 objectColor;
};

struct PSInput
{
	float4 pos : SV_POSITION;
	//float3 col : COLOR;
};

float4 main(PSInput i) : SV_TARGET
{
    //return float4(i.col, 1.0f);
    return objectColor;
}