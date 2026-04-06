#include "Camera.hpp"

Camera::Camera(int width, int height, vec3 position, vec3 up, float yaw, float pitch) : 
	_front(vec3(0.0f, 0.0f, -1.0f)), 
	_width(width), 
	_height(height),
	_target(0, 0, 0),
	_baseYaw(yaw),
	_basePitch(pitch),
	_baseFront(_front),
	_baseUp(up),
	_basePosition(position)
{
	_mode = CameraMode::FREE;
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

void Camera::moveSphereMode(CameraMovement dir, float deltaTime)
{
	float angle = _movementSpeed * deltaTime;

    if (dir == LEFT)      _yaw   -= angle;
    if (dir == RIGHT)     _yaw   += angle;
    if (dir == FORWARD)   _pitch += angle;
    if (dir == BACKWARD)  _pitch -= angle;

    // clamp pitch
	float clamp = math::radians(89.f);
    _pitch = _pitch < -clamp ? -clamp : _pitch > clamp ? clamp : _pitch;

    _position.x = _target.x + 2 * cos(_pitch) * cos(_yaw);
    _position.y = _target.y + 2 * sin(_pitch);
    _position.z = _target.z + 2 * cos(_pitch) * sin(_yaw);

    // always look at target
    _front = math::normalize(_target - _position);
    _right = math::normalize(math::cross(_front, _worldUp));
    _up    = math::normalize(math::cross(_right, _front));
}

void Camera::moveFreeMode(CameraMovement direction, float deltaTime)
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

void Camera::processKeyboard(CameraMovement direction, float deltaTime)
{
	switch (_mode)
	{
		case CameraMode::FREE:
			moveFreeMode(direction, deltaTime);
			break;
		case CameraMode::SPHERE:
			moveSphereMode(direction, deltaTime);
			break;
	}

}

void Camera::onMouseMove(double xposIn, double yposIn)
{
	if (_mode != Camera::SPHERE)
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
	_position = _basePosition;
	_front = _baseFront;

	_yaw = _baseYaw;
	_pitch = _basePitch;

	_lastX = 0;
	_lastY = 0;
	_firstMouse = true;
}

void Camera::changeMode()
{
	resetPosition();
	switch (_mode)
	{
		case CameraMode::SPHERE:
			_mode = CameraMode::FREE;
			break;	
		case CameraMode::FREE:
			_mode = CameraMode::SPHERE;
			break;
	}
	updateCameraVectors();
}