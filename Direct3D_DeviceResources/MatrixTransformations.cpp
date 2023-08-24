#include "pch.h"
#include "MatrixTransformations.h"

Matrix MatrixTransformations::CreateViewMatrix(const Vector3& eye, const Vector3& eyeDirection, const Vector3& up)
{
    //Vector3 zaxis = eye - target;
    Vector3 zaxis = -eyeDirection;
    zaxis.Normalize();
    Vector3 xaxis = up.Cross(zaxis);
    xaxis.Normalize();
    Vector3 yaxis = zaxis.Cross(xaxis);

    Matrix viewMatrix = Matrix(
        Vector4(xaxis.x, yaxis.x, zaxis.x, 0),
        Vector4(xaxis.y, yaxis.y, zaxis.y, 0),
        Vector4(xaxis.z, yaxis.z, zaxis.z, 0),
        Vector4(-xaxis.Dot(eye), -yaxis.Dot(eye), -zaxis.Dot(eye), 1)
    );
    return viewMatrix;
}

Matrix MatrixTransformations::CreateProjectionMatrix(float fov, float aspectRatio, float nearPlane, float farPlane)
{
    float e = 1 / tan(fov / 2);
    Matrix projMatrix = Matrix(
        Vector4(e / aspectRatio, 0, 0, 0),
        Vector4(0, e, 0, 0),
        Vector4(0, 0, -(nearPlane + farPlane) / (farPlane - nearPlane), -1),
        Vector4(0, 0, -2 * nearPlane * farPlane / (farPlane - nearPlane), 0)
    );
    return projMatrix;
}
