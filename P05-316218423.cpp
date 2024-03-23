/*
Práctica 5: Optimización y Carga de Modelos
*/
//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
//para probar el importer
//#include<assimp/Importer.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader_m.h"
#include "Camera.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"

const float toRadians = 3.14159265f / 180.0f;
float anguloPata1 = 0.0f;
float anguloPata2 = 0.0f;
float anguloPata3 = 0.0f;
float anguloPata4 = 0.0f;
float anguloLlanta = 0.0f;
float anguloCofre = 0.0f;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;
Model Goddard_M;
Model Goddard_Cuerpo;
Model Goddard_Cabeza;
Model Goddard_Mandibula;
Model Goddard_Pata_Derecha_Delantera;
Model Goddard_Pata_Izquierda_Delantera;
Model Goddard_Pata_Derecha_Tracera;
Model Goddard_Pata_Izquierda_Tracera;

Model Porsche;
Model Llanta;
Model Cofre;

Skybox skybox;

//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;


// Vertex Shader
static const char* vShader = "shaders/shader_m.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_m.frag";





void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		//	x      y      z			u	  v			nx	  ny    nz
			-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};


	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh* obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh* obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);


}


void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}



int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.5f, 7.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 1.0f);

	Goddard_M = Model();
	Goddard_M.LoadModel("Models/goddard_base.obj");

	Goddard_Cuerpo = Model();
	Goddard_Cuerpo.LoadModel("Models/Cuerpo.obj");

	Goddard_Cabeza = Model();
	Goddard_Cabeza.LoadModel("Models/Cabeza_Goddard_obj.obj");

	Goddard_Mandibula = Model();
	Goddard_Mandibula.LoadModel("Models/Mandibula_Goddard_obj.obj");

	Goddard_Pata_Derecha_Delantera = Model();
	Goddard_Pata_Derecha_Delantera.LoadModel("Models/Pata_Derecha_Delantera.obj");

	Goddard_Pata_Izquierda_Delantera = Model();
	Goddard_Pata_Izquierda_Delantera.LoadModel("Models/Pata_Izquierda_Delantera.obj");

	Goddard_Pata_Derecha_Tracera = Model();
	Goddard_Pata_Derecha_Tracera.LoadModel("Models/Pata_Derecha_Tracera.obj");

	Goddard_Pata_Izquierda_Tracera = Model();
	Goddard_Pata_Izquierda_Tracera.LoadModel("Models/Pata_Izquierda_Tracera.obj");

	Porsche = Model();
	Porsche.LoadModel("Models/porsche.obj");

	Llanta = Model();
	Llanta.LoadModel("Models/llanta.obj");

	Cofre = Model();
	Cofre.LoadModel("Models/cofre.obj");


	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	skybox = Skybox(skyboxFaces);

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);


	glm::mat4 model(1.0);
	glm::mat4 modelaux(1.0);
	glm::mat4 modelaux2(1.0);
	glm::mat4 auxCarro(1.0);
	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);

	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		//Recibir eventos del usuario
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//Se dibuja el Skybox
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);

		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformColor = shaderList[0].getColorLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		// INICIA DIBUJO DEL PISO
		color = glm::vec3(0.5f, 0.5f, 0.5f); //piso de color gris
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[2]->RenderMesh();

		//------------*INICIA DIBUJO DE NUESTROS DEMÁS OBJETOS-------------------*
		// 
		//Carro 
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-10.0f, 2.0f, -1.5f+mainWindow.getavanza()));
		auxCarro = model;
		color = glm::vec3(1.0f, 1.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Porsche.RenderModel();

		//Cofre
		model = auxCarro;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 7.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getcofre()), glm::vec3(1.0f, 0.0f, 0.0f));
		color = glm::vec3(1.0f, 0.5f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Cofre.RenderModel();

		//LLantas
		model = auxCarro;
		model = glm::translate(model, glm::vec3(3.0f, -2.0f, 6.5f));
		model = glm::rotate(model, glm::radians(mainWindow.getllantaAvanza() + mainWindow.getllantaRetrocede()), glm::vec3(1.0f, 0.0f, 0.0f));
		color = glm::vec3(0.0f, 0.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta.RenderModel();

		model = auxCarro;
		model = glm::translate(model, glm::vec3(3.0f, -2.0f, -4.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getllantaAvanza()+ mainWindow.getllantaRetrocede()), glm::vec3(1.0f, 0.0f, 0.0f));
		color = glm::vec3(0.0f, 0.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta.RenderModel();

		model = auxCarro;
		model = glm::translate(model, glm::vec3(-3.5f, -2.0f, 6.5f));
		model = glm::rotate(model, glm::radians(mainWindow.getllantaAvanza() + mainWindow.getllantaRetrocede()), glm::vec3(1.0f, 0.0f, 0.0f));
		color = glm::vec3(0.0f, 0.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta.RenderModel();

		model = auxCarro;
		model = glm::translate(model, glm::vec3(-3.5f, -2.0f, -4.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getllantaAvanza() + mainWindow.getllantaRetrocede()), glm::vec3(1.0f, 0.0f, 0.0f));
		color = glm::vec3(0.0f, 0.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta.RenderModel();

		//Goddard
		//Cuerpo
		color = glm::vec3(0.0f, 0.0f, 1.0f); //modelo de goddard de color negro

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 2.0f, -1.5f));
		modelaux = model;
		modelaux2 = model;
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Goddard_Cuerpo.RenderModel();//modificar por el modelo sin las 4 patas y sin cola

		//Patas
		//delanteras
		model = modelaux;
		model = glm::translate(model, glm::vec3(1.0f, -0.4f, -0.8f));
		model = glm::rotate(model, glm::radians(mainWindow.getpata1()), glm::vec3(0.0f, 0.0f, 1.0f));
		color = glm::vec3(0.0f, 1.0f, .0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Goddard_Pata_Derecha_Delantera.RenderModel();


		model = modelaux;
		model = glm::translate(model, glm::vec3(1.0f, -0.4f, 0.5f));
		model = glm::rotate(model, glm::radians(mainWindow.getpata2()), glm::vec3(0.0f, 0.0f, 1.0f));
		color = glm::vec3(0.0f, 1.0f, .0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Goddard_Pata_Izquierda_Delantera.RenderModel();
		color = glm::vec3(0.0f, 0.0f, 1.0f);

		//Traceras
		model = modelaux;
		model = glm::translate(model, glm::vec3(-0.7f, -1.1f, -0.8f));
		model = glm::rotate(model, glm::radians(mainWindow.getpata3()), glm::vec3(0.0f, 0.0f, 1.0f));
		color = glm::vec3(0.0f, 1.0f, .0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Goddard_Pata_Derecha_Tracera.RenderModel();


		model = modelaux;
		model = glm::translate(model, glm::vec3(-0.7f, -1.2f, 0.5f));
		model = glm::rotate(model, glm::radians(mainWindow.getpata4()), glm::vec3(0.0f, 0.0f, 1.0f));
		color = glm::vec3(0.0f, 1.0f, .0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Goddard_Pata_Izquierda_Tracera.RenderModel();


		//Cabeza
		model = modelaux2;
		model = glm::translate(model, glm::vec3(1.2f, 0.8f, 0.0f));
		modelaux2 = model;
		color = glm::vec3(0.0f, 0.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Goddard_Cabeza.RenderModel();

		//mandibula
		model = modelaux2;
		model = glm::translate(model, glm::vec3(1.5f, 0.2f, 0.0f));
		color = glm::vec3(0.0f, 0.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Goddard_Mandibula.RenderModel();


		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
