#include "pch.h"
#include "Camera.h"


Camera::Camera(Vector3 cameraPosition, float pitch, float yaw) :
    m_cameraPosition(cameraPosition), m_pitch(pitch), m_yaw(yaw)
{
    RecalculateCameraFrame();
}

Camera::Camera(Vector3 cameraPosition, Vector3 targetPosition) : 
	m_cameraPosition(cameraPosition)
{
	Vector3 forward = targetPosition - cameraPosition;
	forward.Normalize();
	Vector3 right = forward.Cross(Vector3::UnitY);
	right.Normalize();
	Vector3 up = right.Cross(forward);
	up.Normalize();

	m_cameraRight = right;
	m_cameraForward = forward;
	m_cameraUp = up;
	m_pitch = asinf(forward.y);
	auto cos_pitch = cosf(m_pitch);
	m_yaw = acosf(forward.z / cos_pitch);// +XM_PI / 2; // ????
}

void Camera::Rotate(float deltaPitch, float deltaYaw)
{
	m_pitch -= deltaPitch;
	m_yaw -= deltaYaw;

	// limit pitch to straight up or straight down
	// with a little fudge-factor to avoid gimbal lock
	float limit = XM_PI / 2.0f - 0.01f;
	m_pitch = std::max(-limit, m_pitch);
	m_pitch = std::min(+limit, m_pitch);

	if (m_yaw > XM_PI)
	{
		m_yaw -= XM_PI * 2.0f;
	}
	else if (m_yaw < -XM_PI)
	{
		m_yaw += XM_PI * 2.0f;
	}

	RecalculateCameraFrame();
}

void Camera::Move(Vector3 offset)
{
	m_cameraPosition += offset;

	m_updateViewMatrix = true;
}

void Camera::RecalculateCameraFrame()
{
	float y = sinf(m_pitch);
	float r = cosf(m_pitch);
	float z = r * cosf(m_yaw);
	float x = r * sinf(m_yaw);
	Vector3 forward = Vector3(x, y, z);
	forward.Normalize();
	Vector3 right = forward.Cross(Vector3::UnitY);
	right.Normalize();
	Vector3 up = right.Cross(forward);
	up.Normalize();

	m_cameraRight = right;
	m_cameraForward = forward;
	m_cameraUp = up;

	m_updateViewMatrix = true;
}