#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glad/glad.h>

#include "Matrix4.hpp"
#include "Math.hpp"
#include "Types.hpp"

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods

const vec3 defaultPosition(0.0f, 0.0f, 3.0f);
const float defaultZoom = 45.0f;

// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera
{
	public:
		enum CameraMovement
		{
			FORWARD,
			BACKWARD,
			LEFT,
			RIGHT
		};
		enum CameraMode
		{
			FREE,
			ORBITAL
		};

		Camera(int width = 800, 
			int height = 600, 
			vec3 position = defaultPosition,
			vec3 up = vec3(0.0f, 1.0f, 0.0f), 
			float yaw = -90.0f, 
			float pitch = 0.0f);

		// returns the view matrix calculated using Euler Angles and the LookAt Matrix
		mat4 getViewMatrix() {return mat4::lookAt(_position, _position + _front, _up);}
		void resize(int width, int height);
		// processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
		void	processKeyboard(CameraMovement direction, float deltaTime);
		void	onMouseMove(double xposIn, double yposIn);
		void	onMouseScroll(double xoffset, double yoffset);
		void	resetPosition();
		void	updateOrientation();
		void	changeMode();
		void	changePosition(const vec3&);
		// getter
		float	getZoom() {return _zoom;};
		float	getAspectRatio() {return _aspectRatio;};
		vec3 	getPosition() {return _position;};
	private:
		// camera Attributes
		vec3 		_position;
		vec3 		_front;
		vec3 		_up;
		vec3 		_right;
		vec3 		_worldUp;
		// euler Angles
		float 		_yaw;
		float 		_pitch;
		// camera options
		const float _movementSpeed = 2.5f;
		const float _mouseSensitivity = 0.1f;
		float 		_zoom = defaultZoom;
		float 		_aspectRatio;

		int 		_width;
		int 		_height;
		bool 		_firstMouse = true;
		float 		_lastX = 0.0f;
		float 		_lastY = 0.0f;
		CameraMode	_mode;

		const vec3	_target;
		const vec3	_baseFront;
		const vec3 	_baseUp;
		const vec3	_basePosition;
		// calculates the front vector from the Camera's (updated) Euler Angles
		void updateCameraVectors();
		void moveFreeMode(CameraMovement dir, float deltaTime);
		void moveOrbitalMode(CameraMovement dir, float deltaTime);
};

#endif
