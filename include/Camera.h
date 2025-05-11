#pragma once
#include <glm/glm.hpp>
#include "Marcos.h"

GLRT_BEGIN
class Camera
{
public:
	enum ENUM_MOVE {
		FORWARD,
		BACKWARD,
		LEFT,
		RIGHT,
		UP,
		DOWN
	};
	enum ENUM_CAMERA_TYPE {
		PERSPECTIVE,
		ORTHOGRAPHIC
	};
	Camera(glm::vec3 position, float yaw, float pitch,
		float rationWH, float nearPlane, float farPlane,
		glm::vec3 up, ENUM_CAMERA_TYPE type);

	void ProcessKeyboard(ENUM_MOVE direction, float deltaTime);
	void ProcessMouseMovement(float xOffset, float yOffset, bool constrainPitch = true);
	void ProcessMouseScroll(float yOffset);

	glm::mat4 GetViewMatrix() const;
	glm::mat4 GetProjectionMatrix() const;
	//透视投影
	void SetPerspective();
	//正交投影
	void SetOrtho();

	glm::vec3& GetPos();
	glm::vec3 GetFront() const { return front; }
private:
	void updateCameraVectors();

	// Camera Attributes
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	const glm::vec3 worldUp;
	float fov;
	float rationWH;
	float nearPlane;
	float farPlane;
	float yaw;
	float pitch;
	ENUM_CAMERA_TYPE projectionMode;

	// Default camera values
	static const float RATIO_WH;
	static const float NEAR_PLANE;
	static const float FAR_PLANE;
	static const float YAW;
	static const float PITCH;
	static const float FOV;
	static const ENUM_CAMERA_TYPE PROJECTION_MODE;
};

GLRT_END

