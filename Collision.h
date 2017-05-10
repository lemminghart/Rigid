#pragma once


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

//Calcula la colisión de una particula con el plano
static void Calculate_Plane_Collision(glm::vec3 n, Particle *part, float d, int solver) {
	//Coeficiente de elasticidad
	float E = 0.7f;

	//Coeficiente de friccion
	float F = 0.3f;

	//Calculate new position of the particle
	part->currentPos.x = part->currentPos.x - ((1 + E) * (((n.x * part->currentPos.x) + (n.y * part->currentPos.y) + (n.z * part->currentPos.z)) + d) * n.x);
	part->currentPos.y = part->currentPos.y - ((1 + E) * (((n.x * part->currentPos.x) + (n.y * part->currentPos.y) + (n.z * part->currentPos.z)) + d) * n.y);
	part->currentPos.z = part->currentPos.z - ((1 + E) * (((n.x * part->currentPos.x) + (n.y * part->currentPos.y) + (n.z * part->currentPos.z)) + d) * n.z);


	if (solver == EULER) {
		//Calculate new velocity of the particle
		part->currentV.x = part->currentV.x - ((1 + E) * ((n.x * part->currentV.x) + (n.y * part->currentV.y) + (n.z * part->currentV.z)) * n.x);
		part->currentV.y = part->currentV.y - ((1 + E) * ((n.x * part->currentV.x) + (n.y * part->currentV.y) + (n.z * part->currentV.z)) * n.y);
		part->currentV.z = part->currentV.z - ((1 + E) * ((n.x * part->currentV.x) + (n.y * part->currentV.y) + (n.z * part->currentV.z)) * n.z);

		//apply friction on EULER
		part->currentV.x = part->currentV.x - F * Calculate_V_Tangential(n, part->currentV).x;
		part->currentV.y = part->currentV.y - F * Calculate_V_Tangential(n, part->currentV).y;
		part->currentV.z = part->currentV.z - F * Calculate_V_Tangential(n, part->currentV).z;
	}
	if (solver == VERLET) {
		//Mirroring the last pos, so we can calculate new direction properly
		// P = P' - 2(n·P' + d) · n
		part->lastPos.x = part->lastPos.x - ((1 + E) * ((n.x * part->lastPos.x) + (n.y * part->lastPos.y) + (n.z * part->lastPos.z) + d) * n.x);
		part->lastPos.y = part->lastPos.y - ((1 + E) * ((n.x * part->lastPos.x) + (n.y * part->lastPos.y) + (n.z * part->lastPos.z) + d) * n.y);
		part->lastPos.z = part->lastPos.z - ((1 + E)* ((n.x * part->lastPos.x) + (n.y * part->lastPos.y) + (n.z * part->lastPos.z) + d) * n.z);

		//apply Friction and elasticity
		float alpha = 1 - E; //elasticity
							 //friction uses the raw variable F of friction

							 //previoulsy we calculate the vector between lastPos & currentPos to calculate Normal & Tangential vectors
		glm::vec3 vector;
		vector.x = part->currentPos.x - part->lastPos.x;
		vector.y = part->currentPos.y - part->lastPos.y;
		vector.z = part->currentPos.z - part->lastPos.z;

		// P = P + alpha * Vn + F * Vt
		part->lastPos.x = part->lastPos.x + (alpha * Calculate_V_Normal(n, vector).x) + (F * Calculate_V_Tangential(n, vector).x);
		part->lastPos.y = part->lastPos.y + (alpha * Calculate_V_Normal(n, vector).y) + (F * Calculate_V_Tangential(n, vector).y);
		part->lastPos.z = part->lastPos.z + (alpha * Calculate_V_Normal(n, vector).z) + (F * Calculate_V_Tangential(n, vector).z);
	}
}

//Calcula la colisión de una particula con la esfera
static void Calculate_Sphere_Collision(Particle *part, Esfera *esfera, int solver) {
	//resolvemos el sistema de ecuaciones entre la recta generada por la posición anterior y la posicion actual con la ecuacion de la esfera
	//esto nos da una ecuacion de segundo grado, que resolvemos a continuación
	
	float alpha1, alpha2; //incógnita del sistema de escuaciones
	glm::vec3 r1, r2; //puntos de la superficie de la esfera cortados por la recta
	
	glm::vec3 vector = Calculate_Vector(part->currentPos, part->lastPos);

	/*float a = (pow(esfera->pos.x, 2) + pow(vector.x,2) +
			   pow(esfera->pos.y, 2) + pow(vector.y,2) +
			   pow(esfera->pos.z, 2) + pow(vector.z,2));

	float b = (-2*esfera->pos.x * vector.x -
			   2*esfera->pos.y * vector.y -
			   2* esfera->pos.z * vector.z);

	float c = pow(esfera->radius, 2);*/

	float a = pow(vector.x, 2) + pow(vector.y, 2) + pow(vector.z, 2);

	float b = 2 * part->lastPos.x*vector.x - 2 * esfera->pos.x*vector.x +
		   	  2 * part->lastPos.y*vector.y - 2 * esfera->pos.y*vector.y +
			  2 * part->lastPos.z*vector.z - 2 * esfera->pos.z*vector.z;

	float c = pow(part->lastPos.x, 2) + pow(esfera->pos.x, 2) - 2 * esfera->pos.x * part->lastPos.x +
			  pow(part->lastPos.y, 2) + pow(esfera->pos.y, 2) - 2 * esfera->pos.y * part->lastPos.y +
			  pow(part->lastPos.z, 2) + pow(esfera->pos.z, 2) - 2 * esfera->pos.z * part->lastPos.z - pow(esfera->radius, 2);

	//Aplicamos la ecuacion de 2ndo grado para resolverlo
	//como tenemos 2 soluciones posibles obtendremos 2 alphas
	alpha1 = (-b + sqrt(pow(b, 2) - 4 * a*c)) / (2 * a);
	alpha2 = (-b - sqrt(pow(b, 2) - 4 * a*c)) / (2 * a);

	//aplicamos las alphas a los posibles puntos
	//r1
	r1 = part->lastPos + Calculate_Vector(part->currentPos, part->lastPos) * alpha1;

	//r2
	r2 = part->lastPos + Calculate_Vector(part->currentPos, part->lastPos) * alpha2;

	//ahora diferenciamos los puntos buscando cuál está mas cerca de la posición anterior de la particula
	
	if (Calculate_Distance(r1, part->lastPos) < Calculate_Distance(r2, part->lastPos)) {
		//aplicar colision particula plano en el plano tangente a la esfera en punto r1
		glm::vec3 n = glm::normalize(Calculate_Vector(esfera->pos, r1));
		Calculate_Plane_Collision(n, part, Calculate_d(n, r1), solver);
	}
	else {
		//aplicar colision particula plano en el plano tangente a la esfera en punto r2
		glm::vec3 n = glm::normalize(Calculate_Vector(esfera->pos, r2));
		Calculate_Plane_Collision(n, part, Calculate_d(n, r2), solver);
	}

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

//bool que calcula si hay colision entre una particula y una esfera
static bool Check_Sphere_Collision(Particle *part, Esfera *esfera) {

	//collision = true if if(d(part,center) <  radius)
	float check = Calculate_Distance(part->currentPos, esfera->pos);

	if (check <= esfera->radius) {
		return true;
	}
	else {
		return false;
	}
}

//Calcula la colision con la caja contenedora
static void Box_Collision(Particle *part, int solver) {
	glm::vec3 n;
	glm::vec3 p;

	//He cogido los 2 extremos del cubo:
	// {-5,0,-5} : p_bottom, p_left & p_back
	// {5,10,5} : p_front, p_right & p_top
	//las normales apuntan al centro del cubo

	//Collision with plane_bottom
	n = { 0.f,1.f,0.f };
	p = { -5.f,0.f,-5.f };
	if (Check_Plane_Collision(n, part, Calculate_d(n, p))) {
		Calculate_Plane_Collision(n, part, Calculate_d(n, p), solver);
	}

	//Collision with plane_left
	n = { 1.f,0.f,0.f };
	p = { -5.f,0.f,-5.f };
	if (Check_Plane_Collision(n, part, Calculate_d(n, p))) {
		Calculate_Plane_Collision(n, part, Calculate_d(n, p), solver);
	}

	//Collision with plane_back
	n = { 0.f,0.f,1.f };
	p = { -5.f,0.f,-5.f };
	if (Check_Plane_Collision(n, part, Calculate_d(n, p))) {
		Calculate_Plane_Collision(n, part, Calculate_d(n, p), solver);
	}

	//Collision with plane_front
	n = { 0.f,0.f,-1.f };
	p = { 5.f,10.f,5.f };
	if (Check_Plane_Collision(n, part, Calculate_d(n, p))) {
		Calculate_Plane_Collision(n, part, Calculate_d(n, p), solver);
	}

	//Collision with plane_right
	n = { -1.f,0.f,0.f };
	p = { 5.f,10.f,5.f };
	if (Check_Plane_Collision(n, part, Calculate_d(n, p))) {
		Calculate_Plane_Collision(n, part, Calculate_d(n, p), solver);
	}

	//Collision with plane_top
	n = { 0.f,-1.f,0.f };
	p = { 5.f,10.f,5.f };
	if (Check_Plane_Collision(n, part, Calculate_d(n, p))) {
		Calculate_Plane_Collision(n, part, Calculate_d(n, p), solver);
	}
}

static void Sphere_Collision(Particle *part, Esfera *esfera, int solver) {
	
	//primero hay que checkear si hay colision
	if (Check_Sphere_Collision(part, esfera)) {
		Calculate_Sphere_Collision(part, esfera, solver);
	}
}

//Calcula TODAS las colisiones del programa
static void Collision_Manager(Particle *part, Esfera *esfera, int solver) {

	//Check collision with BOX
	Box_Collision(part, solver);

	//Check collision with the sphere
	Sphere_Collision(part, esfera, solver);
}
