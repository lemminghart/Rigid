#include "../Particle.h"
#include <glm\gtc\matrix_transform.hpp>

Particle::Particle() {
	//setup the initial position
	currentPos.x = 0.f;
	currentPos.y = 0.f;
	currentPos.z = 0.f;

	lastPos = glm::vec3(0.f);

}

Particle::Particle(glm::vec3 pos, glm::vec3 lastp) {
	//setup the initial position
	currentPos = pos;
	lastPos = lastp;
}


Particle::~Particle() {

}
