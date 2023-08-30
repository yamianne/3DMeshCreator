#pragma once
#include <SimpleMath.h>
using namespace DirectX::SimpleMath;

class MatrixTransformations
{
public:
	static Matrix CreateViewMatrix(const Vector3& eye, const Vector3& eyeDirection, const Vector3& up);
	static Matrix CreateProjectionMatrix(float fov, float aspectRatio, float nearPlane, float farPlane);
};

