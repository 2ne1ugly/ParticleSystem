#include "Camera.hpp"

Camera::Camera() :
	_position(glm::vec3(0.f)),
	_rotation(glm::vec3(0.f)),
	_fov(glm::pi<float>() / 2) {
	
}

void Camera::translate(glm::vec3 delta) {
	_position += delta;
}

void Camera::rotateYaw(float delta) {
	_rotation.x += delta;
}

void Camera::rotatePitch(float delta) {
	_rotation.y += delta;
}

void Camera::rotateRoll(float delta) {
	_rotation.z += delta;
}

void Camera::setPosition(glm::vec3 pos) {
	_position = pos;
}

void Camera::setRotation(glm::vec3 rot) {
	_rotation = rot;
}

void Camera::setFov(float fov) {
	_fov = fov;
}

glm::mat4 Camera::calcViewProj(float _width, float _height) {
	glm::mat4 rotate = getRotationMatrix();
	glm::vec3 up = glm::vec3(rotate *glm::vec4(0.f, 1.f, 0.f, 1.f));
	glm::vec3 front = glm::vec3(rotate * glm::vec4(0.f, 0.f, 1.f, 1.f));
	glm::mat4 view = glm::lookAt(_position, _position + front, up);
	glm::mat4 proj = glm::perspectiveFov(_fov, _width, _height, 0.01f, 1000.f);
	return proj * view;
}

glm::mat4 Camera::getRotationMatrix() {
	return glm::yawPitchRoll(_rotation.x, _rotation.y, _rotation.z);
};
