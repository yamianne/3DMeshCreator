#pragma once
#include <SimpleMath.h>
using namespace DirectX::SimpleMath;

class MatrixTransformations
{
public:
	static Matrix CreateViewMatrix(const Vector3& eye, const Vector3& eyeDirection, const Vector3& up);
	static Matrix CreateProjectionMatrix(float fov, float aspectRatio, float nearPlane, float farPlane);

	//// Computes Moore�Penrose Pseudo-inverse matrix using SVD (Singular value decomposition)
	//// of matrix 3x4
	//// Input - Matrix 4x4, assuming last row is empty 
	//static Matrix Pseudoinverse(Matrix& matrix);
};

