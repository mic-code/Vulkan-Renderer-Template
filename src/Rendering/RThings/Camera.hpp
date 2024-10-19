//
// Created by carlo on 2024-10-16.
//

#ifndef CAMERA_HPP
#define CAMERA_HPP

namespace Rendering
{
	class Camera
	{
		struct matrices
		{
			glm::mat4 perspective;
			glm::mat4 view;
			glm::mat4 rotation;
			glm::mat4 translation;
		} matrices;

		enum CameraMode
		{
			E_FIXED,
			E_FREE
		};

	public:
		Camera(glm::vec3 camPos = glm::vec3(0.0f), CameraMode mode = E_FREE, glm::uvec2 aspect = glm::uvec2(800, 600))
		{
			SetPerspective(45.0f, (float)aspect.x/ (float)aspect.y, 0.1f, 512.0f);
			this->position = camPos;
			this->currentMode = mode;
		}
		void Move(float deltaTime)
		{
			RotateCamera();

			glm::vec2 input = glm::vec2(0.0f);
			position += forward * input.y * movementSpeed * deltaTime;
			position += right * input.x * movementSpeed * deltaTime;
			
			matrices.view = glm::lookAt(position, position + forward, up);
		}
		void RotateCamera()
		{
			//todo: add input handler
			glm::vec2 input;
			float currentX = input.x;
			float currentY = input.y;

			if (firstMouse)
			{
				lastX = currentX;
				lastY = currentY;
				firstMouse = false;
			}

			float xOffset =  lastX - currentX;
			float yOffset =  lastY - currentY;

			lastX = currentX;
			lastY = currentY;

			xOffset *= sens;
			yOffset *= sens;
			yaw += xOffset;
			pitch += yOffset;


			glm::vec3 camForward;

			camForward.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
			camForward.y = sin(glm::radians(pitch));
			camForward.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		}

		void SetPerspective(float fov, float aspect, float znear, float zfar)
		{
			matrices.perspective = glm::perspective(glm::radians(fov), aspect, znear, zfar);
		}

		void SetLookAt(glm::vec3 targetPosition)
		{
			this->lookAt = targetPosition;
			matrices.view = glm::lookAt(position,
			                            this->lookAt,
			                            glm::vec3(0.0f, 1.0f, 0.0f));
		}


		glm::vec3 WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
		glm::vec3 forward = glm::vec3(0.0f, 0.0f, 1.0f);
		glm::vec3 right;
		glm::vec3 up;
		glm::vec3 lookAt = glm::vec3(0.0f);

		glm::vec3 position;
		float yaw = -90;
		float pitch = -90;
		float lastX;
		float lastY;
		float sens = .4f;
		bool firstMouse = true;
		bool inverseY = true;
		CameraMode currentMode;
		float movementSpeed = 5.0f;
	};
}
#endif //CAMERA_HPP
