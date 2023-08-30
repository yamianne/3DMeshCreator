cbuffer cbColor : register(b0)
{
    float4 surfaceColor;
};

cbuffer cbLighting : register(b1)
{
    float4 ambientColor;
    float4 surfaceCoef;
    float4 lightingPosition;
    float4 lightingColor;
}

#define MAX_LIGHTS 1

struct PSInput
{
    float4 pos : SV_POSITION;
    float3 worldPos : POSITION;
    float3 normal : NORMAL0;
    float3 view : NORMAL1;
};

float4 main(PSInput input) : SV_TARGET
{
    float3 N = normalize(input.normal);
    float4 finalColor = float4(0.0f, 0.0f, 0.0f, 0.0f);

    for (int i = 0; i < MAX_LIGHTS; i++)
    {
        float3 L = normalize(lightingPosition.xyz - input.worldPos);
        float3 V = normalize(-input.view);
        float3 R = normalize(-reflect(L, N));
        
        float4 Iamb = ambientColor * surfaceCoef.x;

        float4 Idiff = surfaceColor * surfaceCoef.y * max(dot(N, L), 0.0);
        Idiff = clamp(Idiff, 0.0, 1.0);

        float4 Ispec = lightingColor * surfaceCoef.z * pow(max(dot(R, V), 0.0), 0.3 * surfaceCoef.w);

        Ispec = clamp(Ispec, 0.0, 1.0);

        finalColor += Iamb + Idiff + Ispec;
    }

    return finalColor;
    //return float4(normalize(input.normal), 1.0f);
}
    