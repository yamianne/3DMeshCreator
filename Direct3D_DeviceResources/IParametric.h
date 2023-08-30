#pragma once
#include <SimpleMath.h>
using namespace DirectX::SimpleMath;

class IParametric
{
public:
	virtual Vector3 GetValue(float u, float v) = 0;
	virtual Vector3 GetDerivativeU(float u, float v) = 0;
	virtual Vector3 GetDerivativeV(float u, float v) = 0;
	Vector3 GetNormal(float u, float v)
	{
		Vector3 uDer = GetDerivativeU(u, v).Normalized();
		Vector3 vDer = GetDerivativeV(u, v).Normalized();
		return vDer.Cross(uDer).Normalized();
	}
};