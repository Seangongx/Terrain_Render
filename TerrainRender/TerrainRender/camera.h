#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>//����ͼ�μ����
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

// ����������ķ���
enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	START
};

// Ĭ�ϳ�ʼֵ
const float YAW = -90.0f;//һ��ʼ����cameraFront�ķ���Z�Ḻ����
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVTY = 0.1f;
const float ZOOM = 45.0f;

// �����������
class CCamera
{
public:
	//-------------------------------------------�����޸ĵķֽ���---------------------------------------------//
	//Camera Matrix
	glm::mat4 projection;
	glm::mat4 view;
	glm::mat4 model;
	void SetProj(float _w, float _h, float _n, float _f)
	{
		projection = glm::perspective(glm::radians(Zoom), _w / _h, _n, _f); //glm::radians(fov)
	}
	void SetView()
	{
		view = GetViewMatrix();
	}
	void SetModel_Edit()
	{
		model = glm::mat4();//ÿ�εû�ԭ����Ȼ��Խ��ԽС
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));//���ע�͵�����90�ȵ�ƫת
		model = glm::scale(model, glm::vec3(0.05f));
	}
	//-------------------------------------------�����޸ĵķֽ���---------------------------------------------//

	// Camera Attributes
	glm::vec3 Position;
	glm::vec3 Front;//Direaction vector
	glm::vec3 Up;//Cameraup vector
	glm::vec3 Right;
	glm::vec3 WorldUp;
	// Eular Angles
	float Yaw;
	float Pitch;
	float InitYaw;//�����ʼ��Ϣ
	float InitPitch;
	// Camera options
	float MovementSpeed;
	float MouseSensitivity;
	float Zoom;

	// Constructor with vectors
	CCamera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH)
		: Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVTY), Zoom(ZOOM)
	{
		Position = position;
		WorldUp = up;
		Yaw = yaw; InitYaw = yaw;
		Pitch = pitch; InitPitch = pitch;
		updateCameraVectors();
	}
	// Constructor with scalar values
	CCamera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch)
		: Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVTY), Zoom(ZOOM)
	{
		Position = glm::vec3(posX, posY, posZ);
		WorldUp = glm::vec3(upX, upY, upZ);
		Yaw = yaw; InitYaw = yaw;
		Pitch = pitch; InitPitch = pitch;
		updateCameraVectors();
	}

	// Returns the view matrix calculated using Eular Angles and the LookAt Matrix
	glm::mat4 GetViewMatrix()
	{
		return glm::lookAt(Position, Position + Front, Up);
	}

	// Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
	void ProcessKeyboard(Camera_Movement direction, float deltaTime)
	{
		float velocity = MovementSpeed * deltaTime;
		if (direction == FORWARD)
			Position += Front * velocity;
		if (direction == BACKWARD)
			Position -= Front * velocity;
		if (direction == LEFT)
			Position -= Right * velocity;
		if (direction == RIGHT)
			Position += Right * velocity;
		if (direction == START)
		{
			Position = glm::vec3(0.0f, 0.0f, 10.0f);
			Yaw = InitYaw;
			Pitch = InitPitch;
			updateCameraVectors();
		}
	}

	// Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
	void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
	{
		xoffset *= MouseSensitivity;
		yoffset *= MouseSensitivity;

		Yaw += xoffset;
		Pitch += yoffset;

		// Make sure that when pitch is out of bounds, screen doesn't get flipped
		if (constrainPitch)
		{
			if (Pitch > 89.0f)
				Pitch = 89.0f;
			if (Pitch < -89.0f)
				Pitch = -89.0f;
		}

		// Update Front, Right and Up Vectors using the updated Eular angles
		updateCameraVectors();
	}

	// Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
	void ProcessMouseScroll(float yoffset)
	{
		if (Zoom >= 1.0f && Zoom <= 45.0f)
			Zoom -= yoffset;
		if (Zoom <= 1.0f)
			Zoom = 1.0f;
		if (Zoom >= 45.0f)
			Zoom = 45.0f;
	}

private:
	// Calculates the front vector from the Camera's (updated) Eular Angles
	void updateCameraVectors()
	{
		// Calculate the new Front vector
		glm::vec3 front;
		front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		front.y = sin(glm::radians(Pitch));
		front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		Front = glm::normalize(front);
		// Also re-calculate the Right and Up vector
		// Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
		Right = glm::normalize(glm::cross(Front, WorldUp)); //Ĭ�ϼ��㷽��Ϊ����ϵ
		Up = glm::normalize(glm::cross(Right, Front));
	}
};


#endif