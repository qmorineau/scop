#include "Camera.hpp"

Camera::Camera(int width, int height, vec3 position, vec3 up, float yaw, float pitch) : 
	_front(vec3(0.0f, 0.0f, -1.0f)), 
	_width(width), 
	_height(height)
{
	_aspectRatio = static_cast<float>(_width) / static_cast<float>(_height);
	_position = position;
	_worldUp = up;
	_yaw = yaw;
	_pitch = pitch;
	updateCameraVectors();
}

void Camera::resize(int width, int height)
{
	_width = width;
	_height = height;
	_aspectRatio = static_cast<float>(_width) / static_cast<float>(_height);
}

void Camera::processKeyboard(Camera_Movement direction, float deltaTime)
{
	float velocity = _movementSpeed * deltaTime;
	if (direction == FORWARD)
		_position += _front * velocity;
	if (direction == BACKWARD)
		_position -= _front * velocity;
	if (direction == LEFT)
		_position -= _right * velocity;
	if (direction == RIGHT)
		_position += _right * velocity;
}

void Camera::onMouseMove(double xposIn, double yposIn)
{
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

	if (_firstMouse)
	{
		_lastX = xpos;
		_lastY = ypos;
		_firstMouse = false;
	}

	float xoffset = xpos - _lastX;
	float yoffset = _lastY - ypos; // reversed since y-coordinates go from bottom to top
	_lastX = xpos;
	_lastY = ypos;

	float sensitivity = 0.1f; // change this value to your liking
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	_yaw += xoffset;
	_pitch += yoffset;

	// make sure that when pitch is out of bounds, screen doesn't get flipped
	if (_pitch > 89.0f)
		_pitch = 89.0f;
	if (_pitch < -89.0f)
		_pitch = -89.0f;

	updateCameraVectors();
}

void Camera::onMouseScroll(double xoffset, double yoffset)
{
	(void) xoffset;
	_zoom -= (float)yoffset;
	if (_zoom < 1.0f)
		_zoom = 1.0f;
	if (_zoom > 45.0f)
		_zoom = 45.0f;
}

void Camera::updateCameraVectors()
{
	// calculate the new Front vector
	vec3 front;
	front.x = cos(math::radians(_yaw)) * cos(math::radians(_pitch));
	front.y = sin(math::radians(_pitch));
	front.z = sin(math::radians(_yaw)) * cos(math::radians(_pitch));
	_front = math::normalize(front);
	// also re-calculate the Right and Up vector
	_right = math::normalize(math::cross(_front, _worldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	_up    = math::normalize(math::cross(_right, _front));
}

void Camera::resetPosition()
{
	_position = basePosition;
}