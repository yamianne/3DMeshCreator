cbuffer cbWorld : register(b0)
{
	matrix worldMatrix;
};

cbuffer cbView : register(b1)
{
	matrix viewMatrix;
};

cbuffer cbProj : register(b2)
{
	matrix projMatrix;
};

struct VSInput
{
	float3 pos : POSITION;
};

struct PSInput
{
	float4 pos : SV_POSITION;
};

PSInput main(VSInput i)
{
	PSInput o;
    o.pos = mul(worldMatrix, float4(i.pos, 1.0f));
    o.pos = mul(viewMatrix, o.pos);
    o.pos = mul(projMatrix, o.pos);
	return o;
}