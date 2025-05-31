#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>

GLRT_BEGIN
const float Camera::RATIO_WH = 1.0f;
const float Camera::NEAR_PLANE = 0.01f;
const float Camera::FAR_PLANE = 100.0f;
const float Camera::YAW = -90.0f;
const float Camera::PITCH = 0.0f;
const float Camera::FOV = 45.0f;
const Camera::ENUM_CAMERA_TYPE Camera::PROJECTION_MODE = Camera::PERSPECTIVE;
Camera::Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 4.0f), float yaw = YAW, float pitch = PITCH,
	float rationWH = RATIO_WH, float nearPlane = NEAR_PLANE, float farPlane = FAR_PLANE,
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), ENUM_CAMERA_TYPE type = Camera::PERSPECTIVE)
	: 
	position(position), 
	yaw(yaw), 
	pitch(pitch),
	rationWH(rationWH),
	nearPlane(nearPlane),
	farPlane(farPlane),
	worldUp(up), 
	projectionMode(type)
{
	updateCameraVectors();
}

void Camera::ProcessKeyboard(ENUM_MOVE direction, float deltaTime)
{
	float velocity = 5 * deltaTime;
	if (direction == FORWARD)
		position += front * velocity;
	if (direction == BACKWARD)
		position -= front * velocity;
	if (direction == LEFT)
		position -= right * velocity;
	if (direction == RIGHT)
		position += right * velocity;
	if (direction == UP)
		position += up * velocity;
	if (direction == DOWN)
		position -= up * velocity;
}

void Camera::ProcessMouseMovement(float xOffset, float yOffset, bool constrainPitch)
{
	xOffset *= 0.02;
	yOffset *= 0.02;

	yaw += xOffset;
	pitch += yOffset;

	// Make sure that when pitch is out of bounds, screen doesn't get flipped
	if (constrainPitch)
	{
		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;
	}

	// Update Front, Right and Up Vectors using the updated Euler angles
	updateCameraVectors();
}

void Camera::ProcessMouseScroll(float yOffset)
{
	if (fov >= 1.0f && fov <= 45.0f)
		fov -= yOffset;
	else if (fov <= 1.0f)
		fov = 1.0f;
	else if (fov >= 45.0f)
		fov = 45.0f;
}

glm::mat4 Camera::GetViewMatrix() const
{
	return glm::lookAt(position, position + front, up);
}

glm::mat4 Camera::GetProjectionMatrix() const
{
	switch (projectionMode)
	{
	case Camera::PERSPECTIVE:
		return glm::perspective(glm::radians(fov), rationWH, nearPlane, farPlane);
		break;
	case Camera::ORTHOGRAPHIC:
		return glm::ortho(-fov / 4.0f, fov / 4.0f, -fov / 4.0f / rationWH, fov / 4.0f / rationWH, nearPlane, farPlane);
		break;
	default:
		return glm::mat4(1.0f);
		break;
	}
}

void Camera::SetPerspective()
{
	projectionMode = ENUM_CAMERA_TYPE::PERSPECTIVE;
}

void Camera::SetOrtho()
{
	projectionMode = ENUM_CAMERA_TYPE::ORTHOGRAPHIC;
}

glm::vec3& Camera::GetPos()
{
	return position;
}

void Camera::updateCameraVectors()
{
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(front);
	// Also re-calculate the Right and Up vector
	right = glm::normalize(glm::cross(front, worldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	up = glm::normalize(glm::cross(right, front));
}





GLRT_END