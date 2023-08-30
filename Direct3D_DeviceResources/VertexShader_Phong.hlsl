cbuffer cbWorld : register(b0)
{
    matrix worldMatrix;
};

cbuffer cbView : register(b1)
{
    matrix viewMatrix;
    matrix invViewMatrix;
};

cbuffer cbProj : register(b2)
{
    matrix projMatrix;
};

struct VSInput
{
    float3 pos : POSITION;
    float3 normal : NORMAL;
};

struct PSInput
{
    float4 pos : SV_POSITION;
    float3 worldPos : POSITION;
    float3 normal : NORMAL0;
    float3 view : NORMAL1;
};

PSInput main(VSInput i)
{
    PSInput o;
    float4 pos = mul(worldMatrix, float4(i.pos, 1.0f));
    o.worldPos = pos.xyz;
    pos = mul(viewMatrix, pos);
    o.pos = mul(projMatrix, pos);
    
    float4 normal = float4(i.normal, 0);
    o.normal = mul(normal, worldMatrix).xyz;
    // Apply view transform to normal
    float4 view = float4(0, 0, -1, 0);
    o.view = mul(view, invViewMatrix).xyz - o.worldPos;
    return o;
}