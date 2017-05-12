#pragma once
#include <imgui\imgui.h>
#include <imgui\imgui_impl_glfw_gl3.h>
#include <GL\glew.h>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "../Cubo.h"
#include "../Particle.h"


//calcula la d del plano
static float Calculate_d(glm::vec3 n, glm::vec3 plane_point) {
	//d = -n·P donde P es un punto del plano
	return ((-n.x * plane_point.x) + (-n.y * plane_point.y) + (-n.z * plane_point.z));
}

//calcula la distancia entre la primera componente y la segunda
static float Calculate_Distance(glm::vec3 p, glm::vec3 q) {
	//d(p,q) = 
	return sqrt(pow((q.x - p.x), 2) + pow((q.y - p.y), 2) + pow((q.z - p.z), 2));
}

//calcula el vector entre la primera componente y la segunda
static glm::vec3 Calculate_Vector(glm::vec3 p, glm::vec3 q) {
	//v = (p-q)
	return glm::vec3 { (q.x-p.x), (q.y - p.y), (q.z - p.z) };
}

//calcula la Velocidad normal en un rebote (para aplicar la friccion)
static glm::vec3 Calculate_V_Normal(glm::vec3 n, glm::vec3 v) {
	glm::vec3 Vn; //normal vector

			  //v may be the Particle velocity (EULER) or the Lastpos->CurrentPos vector in VERLET

			  //Vn = (V·n) * n
	Vn.x = ((n.x * v.x) + (n.y * v.y) + (n.z * v.z)) * n.x;
	Vn.y = ((n.x * v.x) + (n.y * v.y) + (n.z * v.z)) * n.y;
	Vn.z = ((n.x * v.x) + (n.y * v.y) + (n.z * v.z)) * n.z;

	return Vn;
}

//calcula la Velocidad tangencial en un rebote (para aplicar la friccion)
static glm::vec3 Calculate_V_Tangential(glm::vec3 n, glm::vec3 v) {
	glm::vec3 Vt; //tangential vector

			  //v may be the Particle velocity (EULER) or the Lastpos->CurrentPos vector in VERLET

			  //Vt = V - Vn
	Vt.x = v.x - Calculate_V_Normal(n, v).x;
	Vt.y = v.y - Calculate_V_Normal(n, v).y;
	Vt.z = v.z - Calculate_V_Normal(n, v).z;

	return Vt;
}

//bool que calcula si hay colision con un plano donde n = normal del plano, part = Particula y d = un punto del plano
static bool Check_Plane_Collision(glm::vec3 n, Particle *part, float d) {

	//collision = true if ((n·p_t + d)(n·p'_t + d) <= 0)
	float check = (((n.x * part->lastPos.x) + (n.y * part->lastPos.y) + (n.z * part->lastPos.z)) + d) *
		(((n.x * part->currentPos.x) + (n.y * part->currentPos.y) + (n.z * part->currentPos.z)) + d);

	if (check <= 0) {
		return true;
	}
	else {
		return false;
	}
}

static glm::vec3 Calculate_position(glm::vec4 pos4, glm::mat4 matrix) {
	
	pos4 = matrix * pos4;
	
	return glm::vec3(pos4.x, pos4.y, pos4.z);
}

//calcula la el rebote con los cada plano de la caja contenedora
static void Calculate_Plane_Collision(Cubo *cubo, glm::vec3 normal, glm::vec3 vert) {

	float E = 1.f;
	//glm::vec3 Vrel = normal * cubo->currentV;
	float Vrel = glm::dot(normal, cubo->currentV);

	//glm::vec3 j = (-(1 + E)*Vrel) / (1.f + normal * glm::cross((cubo->Ibody * glm::cross(vert, normal)), vert));
	float j  = (-(1 + E)*Vrel) / (glm::dot((cubo->M + normal), glm::cross((cubo->Ibody * glm::cross(vert, normal)), vert)));


	glm::vec3 BigJ = j * normal;

	cubo->torque = glm::cross(vert, BigJ);
	cubo->linearMom = cubo->linearMom + BigJ;
	cubo->angularMom = cubo->angularMom + cubo->torque;
}


//Calcula la colision con la caja contenedora
static bool Box_Collision(Cubo *cubo, glm::mat4 matrix, glm::vec3 vert) {
	glm::vec3 n;
	glm::vec3 p;

	//He cogido los 2 extremos del box:
	// {-5,0,-5} : p_bottom, p_left & p_back
	// {5,10,5} : p_front, p_right & p_top
	//las normales apuntan al centro del box


	//generar la particula
	//inicializar la particula
	//checkear colision
	//destruir particula
	
	glm::vec4 pos4 = glm::vec4(vert, 1);
	glm::vec3 pos = Calculate_position(pos4, matrix);
	glm::vec3 lastPos = Calculate_position(pos4, cubo->lastMatrix);

		Particle *part = new Particle(pos, lastPos);

			//Collision with plane_bottom
			n = { 0.f,1.f,0.f };
			p = { -5.f,0.f,-5.f };
			if (Check_Plane_Collision(n, part, Calculate_d(n, p))) {
				std::cout << "COLLISION: BOTTOM" <<std::endl;				
				Calculate_Plane_Collision(cubo, n, pos);
				delete part;
				return true;
			}

			//Collision with plane_left
			n = { 1.f,0.f,0.f };
			p = { -5.f,0.f,-5.f };
			if (Check_Plane_Collision(n, part, Calculate_d(n, p))) {
				std::cout << "COLLISION: LEFT" << std::endl;
				Calculate_Plane_Collision(cubo, n, pos);
				delete part;
				return true;
			}

			//Collision with plane_back
			n = { 0.f,0.f,1.f };
			p = { -5.f,0.f,-5.f };
			if (Check_Plane_Collision(n, part, Calculate_d(n, p))) {
				std::cout << "COLLISION: BACK" << std::endl;
				Calculate_Plane_Collision(cubo, n, pos);
				delete part;
				return true;
			}

			//Collision with plane_front
			n = { 0.f,0.f,-1.f };
			p = { 5.f,10.f,5.f };
			if (Check_Plane_Collision(n, part, Calculate_d(n, p))) {
				std::cout << "COLLISION: FRONT" << std::endl;
				Calculate_Plane_Collision(cubo, n, pos);
				delete part;
				return true;
			}

			//Collision with plane_right
			n = { -1.f,0.f,0.f };
			p = { 5.f,10.f,5.f };
			if (Check_Plane_Collision(n, part, Calculate_d(n, p))) {
				std::cout << "COLLISION: RIGHT" << std::endl;
				Calculate_Plane_Collision(cubo, n, pos);
				delete part;
				return true;
			}

			//Collision with plane_top
			n = { 0.f,-1.f,0.f };
			p = { 5.f,10.f,5.f };
			if (Check_Plane_Collision(n, part, Calculate_d(n, p))) {
				std::cout << "COLLISION: TOP" << std::endl;
				Calculate_Plane_Collision(cubo, n, pos);
				delete part;
				return true;
			}
		
			delete part;
			return false;
}

//Calcula TODAS las colisiones del programa
static bool Collision_Manager(Cubo *cubo, glm::mat4 matrix, glm::vec3 vert) {

	//Check collision with BOX
	bool result = Box_Collision(cubo, matrix, vert);
	if (result) {
		return true;
	}
	else { 
		return false; 
	}
}
