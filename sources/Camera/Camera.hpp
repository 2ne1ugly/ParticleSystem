#pragma once

#include "common.hpp"

class Camera {
public:
	Camera();
	void translate(glm::vec3 delta);
	void rotateYaw(float delta);	//x	
	void rotatePitch(float delta);	//y
	void rotateRoll(float delta);	//z
	void setPosition(glm::vec3 pos);
	void setRotation(glm::vec3 rot);
	glm::mat4 getRotationMatrix();
	void setFov(float fov);
	glm::mat4 calcViewProj(float _width, float _height);
private:
	glm::vec3	_position;
	glm::vec3	_rotation;	//rotation
	float		_fov;
};
