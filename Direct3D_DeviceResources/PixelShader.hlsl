cbuffer cbColor : register(b0)
{
    float4 objectColor;
};

struct PSInput
{
	float4 pos : SV_POSITION;
};

float4 main(PSInput i) : SV_TARGET
{
	return objectColor;
}