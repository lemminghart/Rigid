#pragma once
#include <vector>
#include <GL\glew.h>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_transform.hpp>


class Particle {
public:
	glm::vec3 currentPos, lastPos; //Stores Current and Last Position

	Particle();
	Particle(glm::vec3 pos, glm::vec3 lastp);
	~Particle();
};

static std::vector <Particle> partArray; //vector de particulas