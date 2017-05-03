#include "../Cubo.h"

Cubo::Cubo() {
	//setup the initial parameters (position)
	currentPos.x = 0;
	currentPos.y = 5;
	currentPos.z = 0;

	//setup the initial speed
	currentV.x = 0.f;
	currentV.y = 0.f;
	currentV.z = 0.f;

	//setupo the initial forces

}

Cubo::~Cubo() {

}