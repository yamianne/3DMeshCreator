#pragma once
#include <SimpleMath.h>

using namespace DirectX::SimpleMath;

struct Lighting
{
    Vector4 ambientColor;
    Vector4 surfaceCoef;
    Vector4 lightingPosition;
    Vector4 lightingColor;
};