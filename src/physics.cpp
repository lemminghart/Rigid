#include <imgui\imgui.h>
#include <imgui\imgui_impl_glfw_gl3.h>
#include <GL\glew.h>
#include <glm/gtc/type_ptr.hpp>

#include "../Cubo.h"
#include "../Solvers.h"

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

	//setup the initial torque
	cubo->torque = cubo->forces;

	Cube::updateCube(initPos);
}
void PhysicsUpdate(float dt) {

	cubo->torque = glm::vec3(0);

	Euler_Solver(cubo, dt);

	//translacion  * rotacion
	glm::mat4 translation = glm::mat4(1, 0, 0, 0,
									  0, 1, 0, 0,
									  0, 0, 1, 0,
		cubo->currentPos.x, cubo->currentPos.y, cubo->currentPos.z, 1);

	glm::mat4 rotation = glm::mat4_cast(cubo->rotation);


	Cube::updateCube((translation*rotation));

}
void PhysicsCleanup() {
	delete cubo;
}