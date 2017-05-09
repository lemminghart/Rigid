#include "../Cubo.h"

Cubo::Cubo() {
	/*glm::mat4 Ibody;
	glm::mat4 InertiaInverse;*/
	
	//setup the initial parameters (position)
	currentPos.x = 0;
	currentPos.y = 5;
	currentPos.z = 0;

	//setup the initial speed
	currentV.x = 0.f;
	currentV.y = 0.f;
	currentV.z = 0.f;

	//setup the initial forces
	forces.x = 0;
	forces.y = 0;
	forces.z = 0;

	//setup the initial Angular Velocity
	angularV.x = 0;
	angularV.y = 0;
	angularV.z = 0;

	//setup the Angular Momentum
	angularMom.x = 0;
	angularMom.y = 0;
	angularMom.z = 0;

	//setup the linear Velocity
	linearV.x = 0;
	linearV.y = 0;
	linearV.z = 0;

	//setup the linear Momentum
	linearMom.x = 0;
	linearMom.y = 0;
	linearMom.z = 0;

	//TORQUE
	//Se calcula en el physics init

	//ROTATION
	//Se calcula en el physics init

	//INERTIAINVERSE
	//Se calcula en el solver
	InertiaInverse = glm::mat4(0);

	////setup the Ibody
	//Ibody = glm::mat4(0);

	//setup the mass of the cube
	M = 1;
}

Cubo::~Cubo() {

}