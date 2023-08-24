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
	//float3 col : COLOR;
};

struct VSOutput
{
    float4 pos : SV_POSITION;
    //float3 col : COLOR;
};


VSOutput main(VSInput i)
{
    VSOutput o;
	o.pos = mul(worldMatrix, float4(i.pos, 1.0f));
	o.pos = mul(viewMatrix, o.pos);
	o.pos = mul(projMatrix, o.pos);
    //o.col = i.col;
	return o;
}