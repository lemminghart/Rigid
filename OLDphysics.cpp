//#include <GL\glew.h>
//#include <glm\gtc\type_ptr.hpp>
//#include <glm\gtc\matrix_transform.hpp>
//#include <cstdio>
//
//#include <imgui\imgui.h>
//#include <imgui\imgui_impl_glfw_gl3.h>
//
//#include "../Particle.h"
//#include "../Solvers.h"
//#include "../Collision.h"
//#include "../Sphere.h"
//#include "../Forces.h"
//
//namespace ClothMesh {
//	extern void setupClothMesh();
//	extern void cleanupClothMesh();
//	extern void updateClothMesh(float* array_data);
//	extern void drawClothMesh();
//	extern const int numCols; //14
//	extern const int numRows; //18
//	extern const int numVerts; // 14 * 18 = 252
//}
//
//namespace Sphere {
//	extern void setupSphere(glm::vec3 pos = glm::vec3(0.f, 1.f, 0.f), float radius = 1.f);
//	extern void cleanupSphere();
//	extern void updateSphere(glm::vec3 pos, float radius = 1.f);
//	extern void drawSphere();
//}
//
////Namespace para manejar variables propias del sistema
//namespace Utils { 
//	//time
//	int time = 0;
//	float percent = 0.f;
//	//solver
//	int solver = EULER; //CAN BE EULER or VERLET
//	//position of first particle
//	glm::vec3 pos{ -3.5f, 9.75f, -4.75f };
//	//separation between particles
//	float part_separation = 0.5f;
//	//
//	
//}
//
////variable global de la esfera
//Esfera *esfera;
//
//
//bool show_test_window = false;
//void GUI() {
//	{	//FrameRate
//		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
//
//		//TODO
//	}
//
//	// ImGui test window. Most of the sample code is in ImGui::ShowTestWindow()
//	if(show_test_window) {
//		ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiSetCond_FirstUseEver);
//		ImGui::ShowTestWindow(&show_test_window);
//	}
//}
//
//void PhysicsInit() {
//	//TODO
//
//	//initialize the sphere
//	esfera = new Esfera;
//
//	//we will set up a first particle and then build the mesh from that particle
//	//set up the first particle
//	Particle temp(true, Utils::pos); //the first particle is fixed
//	temp.index = 0;
//	partArray.push_back(temp);
//
//	//we initialite the following particles in base of the first particle
//	for (int i = 1; i <= ClothMesh::numRows; i++) {
//		for (int j = 1; j <= ClothMesh::numCols; j++) {
//			Particle temp2(&temp, Utils::part_separation * j, Utils::part_separation * i);
//			temp2.index = (j-1) + (ClothMesh::numRows * (i-1)) + 1;
//			printf("%d \n", temp2.index);
//			partArray.push_back(temp2);
//		}
//	}
//
//	//fijamos las particulas en el techo
//	partArray[0].fixed = true;
//	partArray[ClothMesh::numCols - 1].fixed = true;
//
//}
//void PhysicsUpdate(float dt) {
//	//TODO
//
//	//Calculamos primero las fuerzas que afectan las particulas
//	for (int i = 0; i < ClothMesh::numVerts; i++) {
//		Calculate_Forces(partArray[i]);
//	}
//
//	//Actualizamos la posicion de las particulas
//	if (Utils::solver == EULER) {
//		for (int i = 0; i < ClothMesh::numVerts; i++) {
//			if (!partArray[i].fixed) {
//				Euler_Solver(&partArray[i], dt);
//				Collision_Manager(&partArray[i], esfera, Utils::solver);
//			}
//		}
//	}
//
//	//------ UPDATE ZONE -------
//	//Update de las particulas
//	float *partVerts = new float[ClothMesh::numVerts * 3];
//	for (int i = 0; i < ClothMesh::numVerts; ++i) {
//			partVerts[i * 3 + 0] = partArray[i].currentPos.x;
//			partVerts[i * 3 + 1] = partArray[i].currentPos.y;
//			partVerts[i * 3 + 2] = partArray[i].currentPos.z;
//	}
//	ClothMesh::updateClothMesh(partVerts);
//
//	//update de la esfera
//	Sphere::updateSphere(esfera->pos, esfera->radius);
//}
//void PhysicsCleanup() {
//	//delete sphere
//	delete esfera;
//}