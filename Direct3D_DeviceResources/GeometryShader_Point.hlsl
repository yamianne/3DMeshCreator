struct GSInput
{
    float4 pos : SV_POSITION;
    //float3 col : COLOR;
};

struct GSOutput
{
	float4 pos : SV_POSITION;
	//float3 col : COLOR;
};

static const float PI = 3.14159265f;

cbuffer cbScreenSize : register(b0)
{
	float4 screenSizes;
}

[maxvertexcount(100)]
void main(
	point GSInput input[1],
	inout TriangleStream< GSOutput > output
)
{
	float4 pointSize = 0.1f * screenSizes;
	uint quadSize = 8;

	GSOutput element;

	float angle = 0.0f;
	element.pos = input[0].pos + pointSize * float4(sin(angle), cos(angle), 0.0f, 0.0f);
    //element.col = input[0].col;
	output.Append(element);

	for (uint i = 1; i < quadSize; i++)
	{
		angle = i * 2 * PI / quadSize;
		element.pos = input[0].pos + pointSize * float4(sin(angle), cos(angle), 0.0f, 0.0f);
        //element.col = input[0].col;
		output.Append(element);

		element = input[0];
		output.Append(element);
	}

	angle = 0.0f;
	element.pos = input[0].pos + pointSize * float4(sin(angle), cos(angle), 0.0f, 0.0f);
    //element.col = input[0].col;
	output.Append(element);
}