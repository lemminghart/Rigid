#include "../Cubo.h"

Cubo::Cubo() {
	/*glm::mat4 Ibody;
	glm::mat4 InertiaInverse;*/
	
	//setup the initial parameters (position)
	currentPos.x = 0.f;
	currentPos.y = 3.f;
	currentPos.z = 0.f;

	//setup the initial speed
	currentV.x = 0.f;
	currentV.y = 0.f;
	currentV.z = 0.f;

	//setup the initial forces
	forces.x = 0.f;
	forces.y = 0.f;
	forces.z = 0.f;

	//setup the initial Angular Velocity
	angularV.x = 0.f;
	angularV.y = 0.f;
	angularV.z = 0.f;

	//setup the Angular Momentum
	angularMom.x = 0.f;
	angularMom.y = 0.f;
	angularMom.z = 0.f;

	//setup the linear Velocity
	linearV.x = 0.f;
	linearV.y = 0.f;
	linearV.z = 0.f;

	//setup the linear Momentum
	linearMom.x = 0.f;
	linearMom.y = 0.f;
	linearMom.z = 0.f;

	//TORQUE
	//Se calcula en el physics init

	//ROTATION
	//Se calcula en el physics init

	//INERTIAINVERSE
	//Se calcula en el solver
	InertiaInverse = glm::mat3(1.f);

	////setup the Ibody
	Ibody = glm::mat3((1.f/6.f));

	//setup the mass of the cube
	M = 1;

	loop = false;

	lastMatrix = glm::mat4(1.f);
}

Cubo::~Cubo() {

}