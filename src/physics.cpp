#include <imgui\imgui.h>
#include <imgui\imgui_impl_glfw_gl3.h>
#include <GL\glew.h>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "../Cubo.h"
#include "../Solvers.h"
#include "../Collision.h"

extern GLuint compileShader(const char* shaderStr, GLenum shaderType, const char* name = "");
extern void linkProgram(GLuint program);

namespace RenderVars {
	extern glm::mat4 _modelView;
	extern glm::mat4 _MVP;
}

namespace Cube {
	extern void setupCube();
	extern void cleanupCube();
	extern void updateCube(const glm::mat4& transform);
	extern void drawCube();
}

namespace Utils { //Namespace para manejar variables propias del sistema
	 //time
	int time = 0;
	extern float percent = 0.f;
	extern float percent_2 = 0.f;
	float halfW = 0.5f;
	glm::vec3 verts[] = {
		glm::vec3(-halfW, -halfW, -halfW),
		glm::vec3(-halfW, -halfW,  halfW),
		glm::vec3(halfW, -halfW,  halfW),
		glm::vec3(halfW, -halfW, -halfW),
		glm::vec3(-halfW,  halfW, -halfW),
		glm::vec3(-halfW,  halfW,  halfW),
		glm::vec3(halfW,  halfW,  halfW),
		glm::vec3(halfW,  halfW, -halfW)
	};
}


bool show_test_window = false;
void GUI() {
	{	//FrameRate
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

		//TODO
	}

	// ImGui test window. Most of the sample code is in ImGui::ShowTestWindow()
	if(show_test_window) {
		ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiSetCond_FirstUseEver);
		ImGui::ShowTestWindow(&show_test_window);
	}
}

Cubo *cubo;

void PhysicsInit() {
	cubo = new Cubo;
	//matriz para inicializar el cubo en el centro del box
	glm::mat4 initPos = glm::mat4(1, 0, 0, 0,
							      0, 1, 0, 0,
		                          0, 0, 1, 0,
		cubo->currentPos.x, cubo->currentPos.y, cubo->currentPos.z, 1);

	cubo->forces.y = 189.f * 2.f;
	cubo->forces.x = 0.f;
	cubo->forces.z = 0;

	glm::vec3 randomPoint = glm::vec3(((float)rand()/RAND_MAX - 0.5f), ((float)rand() / RAND_MAX - 0.5f), ((float)rand() / RAND_MAX - 0.5f));

	randomPoint = randomPoint + cubo->currentPos;

	//setup the initial torque
	cubo->torque = glm::cross((randomPoint - cubo->currentPos), cubo->forces);

	Cube::updateCube(initPos);
}
void PhysicsUpdate(float dt) {

	if (cubo->loop == true) {
		cubo->torque = glm::vec3(0);
		cubo->forces = glm::vec3(0);		
	}
	cubo->loop = true;
	Euler_Solver(cubo, dt);

	//translacion  * rotacion
	glm::mat4 translation = glm::mat4(1, 0, 0, 0,
									  0, 1, 0, 0,
									  0, 0, 1, 0,
		cubo->currentPos.x, cubo->currentPos.y, cubo->currentPos.z, 1);

	glm::mat4 rotation = glm::mat4_cast(cubo->rotation);

	glm::mat4 result = translation*rotation;

	
	//update del cubo
	Cube::updateCube((result));
	for (int i = 0; i < 8; i++) {
		//comprobamos colisiones tras el update
		/*std::cout << i << std::endl;*/
		Collision_Manager(cubo, result, Utils::verts[i]);
	}
	//actualizamos la matriz de transformacion
	cubo->lastMatrix = result;

	//aqui entra cada 1 segundos
	if (Utils::percent > 0.33f) {
		Utils::time++;
		std::cout << "TIME:" << Utils::time << std::endl;
		//aqui entra cada 5 segundos
		if (Utils::percent_2 > 0.33f * 20.f) {
			std::cout << "RESTART SIM" << std::endl;
			delete cubo;
			PhysicsInit();

			//bajamos el contador a 0 para que a los 5 segundos vuelva a entrar aqui
			Utils::percent_2 = 0;
		}

		//bajamos el contador a 0 para que al segundo vuelva a entrar aqui
		Utils::percent = 0;
		
	}

	Utils::percent += dt; //contador incremental
	Utils::percent_2 += dt; //contador incremental

}
void PhysicsCleanup() {
	delete cubo;
}