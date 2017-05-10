#pragma once
#include "../Cubo.h"
#include <cmath>
#include <glm/gtx/quaternion.hpp>

#define GRAVITY -9.81f
#define SLOW_GRAVITY -9.81f / 2.f

static void Euler_Solver(Cubo *cubo, float dt) {

	//Calculate the Linear Momentum
	// P = P. + dt * F.
	cubo->linearMom.x = cubo->linearMom.x + (dt * cubo->forces.x);
	cubo->linearMom.y = cubo->linearMom.y + (dt * (cubo->forces.y + GRAVITY));
	cubo->linearMom.z = cubo->linearMom.z + (dt * cubo->forces.z);

	//Calculate the Angular Momentum
	// L = L + dt * T(torque) [torque = Inertia Tensor * W (angular velocity)
	cubo->angularMom.x = cubo->torque.x;
	cubo->angularMom.y = cubo->torque.y;
	cubo->angularMom.z = cubo->torque.z; //en el momento inicial creo una fuerza de la cual calculo el torque (diapo del gato). Luego no se usa mas

	//Calculate the new velocity
	// v = P / M
	cubo->currentV.x = cubo->linearMom.x / cubo->M;
	cubo->currentV.y = cubo->linearMom.y / cubo->M;
	cubo->currentV.z = cubo->linearMom.z / cubo->M;

	//Calculate the new Position
	// x = x + dt * v

	cubo->currentPos.x = cubo->currentPos.x + (dt * cubo->currentV.x);
	cubo->currentPos.y = cubo->currentPos.y + (dt * cubo->currentV.y);
	cubo->currentPos.z = cubo->currentPos.z + (dt * cubo->currentV.z);

	//Calculate the inverse of Inertia Tensor
	glm::mat3 R = glm::mat3_cast(cubo->rotation);
	cubo->InertiaInverse = R * glm::inverse(cubo->Ibody) * glm::transpose(R);
	
	//Calculate angular velocity
	// w = I^-1 * L
	cubo->angularV = glm::inverse(cubo->InertiaInverse) * glm::vec4(cubo->angularMom,0); //si da problemas pasar InertiaInverse a mat3

	//Calculate rotation
	cubo->rotation = cubo->rotation + dt * 0.5f * glm::quat(0,cubo->angularV)*cubo->rotation;
	cubo->rotation = glm::normalize(cubo->rotation);

	//-----------------------------------------------------------------------------------------

	////Calculate the new velocity
	//// V = V. + dt * (F/m)[acceleration]
	////We only need to recalculate the V on Y, it's the only one affected by external force
	//cubo->currentV.x = cubo->currentV.x;
	//cubo->currentV.y = cubo->currentV.y + dt * GRAVITY;
	//cubo->currentV.z = cubo->currentV.z;
}
