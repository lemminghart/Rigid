#pragma once
#include <GL\glew.h>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_transform.hpp>


class Cubo {
public:
	glm::vec3 currentPos; //Stores Current and Last Position
	glm::vec3 currentV; //Stores Current and Last Velocity
	glm::vec3 forces;
	glm::vec3 angularV; //Stores Angular velocity
	glm::vec3 angularMom;
	glm::vec3 linearV; //Stores the Linear Velocity
	glm::vec3 linearMom; //Stores the Linear Momentum
	glm::vec3 torque;
	glm::quat rotation;
	glm::mat3 Ibody;
	glm::mat3 InertiaInverse;

	glm::mat4 lastMatrix;
	float M; //mass = 1

	bool loop; //for controlling the initialisation

	Cubo();
	~Cubo();
};