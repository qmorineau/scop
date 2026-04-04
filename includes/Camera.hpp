#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glad/glad.h>

#include "Matrix4.hpp"
#include "Math.hpp"
#include "Types.hpp"

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

// Default camera values
const float YAW         = -90.0f;
const float PITCH       =  0.0f;
const float SPEED       =  2.5f;
const float SENSITIVITY =  0.1f;
const float ZOOM        =  45.0f;


// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera
{
	public:
		// camera Attributes
		vec3 Position;
		vec3 Front;
		vec3 Up;
		vec3 Right;
		vec3 WorldUp;
		// euler Angles
		float Yaw;
		float Pitch;
		// camera options
		float MovementSpeed;
		float MouseSensitivity;
		float Zoom;
		float aspectRatio;

		int _width;
		int _height;
		bool firstMouse = true;
		float lastX = 0.0f;
		float lastY = 0.0f;

		// constructor with vectors
		Camera(int width = 800, int height = 600, vec3 position = vec3(0.0f, 0.0f, 2.0f), vec3 up = vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) : Front(vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM), _width(width), _height(height)
		{
			aspectRatio = static_cast<float>(_width) / static_cast<float>(_height);
			Position = position;
			WorldUp = up;
			Yaw = yaw;
			Pitch = pitch;
			updateCameraVectors();
		}
		// constructor with scalar values
		Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
		{
			Position = vec3(posX, posY, posZ);
			WorldUp = vec3(upX, upY, upZ);
			Yaw = yaw;
			Pitch = pitch;
			updateCameraVectors();
		}

		// returns the view matrix calculated using Euler Angles and the LookAt Matrix
		mat4 GetViewMatrix()
		{
			return mat4::lookAt(Position, Position + Front, Up);
		}

		void resize(int width, int height)
		{
			_width = width;
			_height = height;
			aspectRatio = static_cast<float>(_width) / static_cast<float>(_height);
		}

		// processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
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
		}

		void onMouseMove(double xposIn, double yposIn)
		{
			float xpos = static_cast<float>(xposIn);
			float ypos = static_cast<float>(yposIn);

			if (firstMouse)
			{
				lastX = xpos;
				lastY = ypos;
				firstMouse = false;
			}

			float xoffset = xpos - lastX;
			float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
			lastX = xpos;
			lastY = ypos;

			float sensitivity = 0.1f; // change this value to your liking
			xoffset *= sensitivity;
			yoffset *= sensitivity;

			Yaw += xoffset;
			Pitch += yoffset;

			// make sure that when pitch is out of bounds, screen doesn't get flipped
			if (Pitch > 89.0f)
				Pitch = 89.0f;
			if (Pitch < -89.0f)
				Pitch = -89.0f;

			updateCameraVectors();
		}

		void onMouseScroll(double xoffset, double yoffset)
		{
			(void) xoffset;
			Zoom -= (float)yoffset;
			if (Zoom < 1.0f)
				Zoom = 1.0f;
			if (Zoom > 45.0f)
				Zoom = 45.0f;
		}

	private:
		// calculates the front vector from the Camera's (updated) Euler Angles
		void updateCameraVectors()
		{
			// calculate the new Front vector
			vec3 front;
			front.x = cos(math::radians(Yaw)) * cos(math::radians(Pitch));
			front.y = sin(math::radians(Pitch));
			front.z = sin(math::radians(Yaw)) * cos(math::radians(Pitch));
			Front = math::normalize(front);
			// also re-calculate the Right and Up vector
			Right = math::normalize(math::cross(Front, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
			Up    = math::normalize(math::cross(Right, Front));
		}
};

#endif
