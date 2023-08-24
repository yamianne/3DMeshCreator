#pragma once
//#include <DirectXMath.h>

using namespace DirectX;

class Camera
{
public:
    Camera(Vector3 cameraPosition = Vector3(0.0f, 1.0f, -5.0f), float pitch = 0, float yaw = 0);
    Camera(Vector3 cameraPosition, Vector3 targetPosition);

    void            Rotate(float deltaPitch, float deltaYaw);
    void            Move(Vector3 offset);
    Vector3         GetPosition()           { return m_cameraPosition; }
    Vector3         GetForward()            { return m_cameraForward; }
    Vector3         GetRight()              { return m_cameraRight; }
    Vector3         GetUp()                 { return m_cameraUp; }

    void            RecalculateCameraFrame();
    bool            ViewMatrixNeedsUpdate() { return m_updateViewMatrix; }
    void            ViewMatrixUpdated()     { m_updateViewMatrix = false; }
private:
    Vector3         m_cameraPosition;
    float           m_pitch;
    float           m_yaw;

    // Keep camera frame for optimalization
    Vector3         m_cameraRight;
    Vector3         m_cameraForward;
    Vector3         m_cameraUp;

    bool            m_updateViewMatrix = true;
};

