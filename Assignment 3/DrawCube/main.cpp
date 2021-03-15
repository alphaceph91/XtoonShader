#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "soil.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <learnopengl/shader_m.h>
#include <learnopengl/camera.h>
#include <learnopengl/model.h>

using namespace glm;

void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;

//Camera attributes
vec3 cameraPos = vec3(0.0f, 0.0f, 0.75f);
vec3 cameraFront = vec3(0.0f, 0.0f, -1.0f);
vec3 cameraUp = vec3(0.0f, 1.0f, 0.0f);

//Timing
float deltaTime = 0.0f;	
float lastFrame = 0.0f;

//Light attributes
float lightX = 16.0f;
float lightY = 1.0f;
float lightZ = 1.0f;

vec3 lightPos = vec3(lightX, lightY, lightZ);
int shaderType = 1;
float weightTexture = 0.6;
bool mPressed = false;
unsigned int texture;
int textureType = 1;

int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// GLFW Window
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Xtoon Shader", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

	// Initialize GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}


	Shader myShader("modelShader.vert", "modelShader.frag");
	//Model myModel("objects/happy-buddha/happy-buddha.obj");
	Model myModel("objects/buddha2/buddhaNoMaterial.obj");
	
	myShader.use();
	
	glEnable(GL_DEPTH_TEST);
	
	// Main Loop
	while (!glfwWindowShouldClose(window)) {
		//Textures from XToon paper
		//if (textureType == 1) {
		//	texture = TextureFromFile("toonTexture.png", "textures");
		//}
		//if (textureType == 2)
		//{
		//	texture = TextureFromFile("greyGradient.png", "textures");
		//}
		//if (textureType == 3)
		//{
		//	texture = TextureFromFile("greenGradient.png", "textures");
		//}
		//if (textureType == 4)
		//{
		//	texture = TextureFromFile("blackWhite.png", "textures");
		//}

		//New textures
		if (textureType == 1) {
			texture = TextureFromFile("linearGrad.png", "textures");
		}
		if (textureType == 2)
		{
			texture = TextureFromFile("radialGrad.png", "textures");
		}
		if (textureType == 3)
		{
			texture = TextureFromFile("angularGrad.png", "textures");
		}
		if (textureType == 4)
		{
			texture = TextureFromFile("linearGradBW.png", "textures");
		}

		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);

		mat4 projection = perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.f);
		mat4 view = lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		mat4 model = mat4(1.0f);
		//model = rotate(model, (float)glfwGetTime(), vec3(0.0f, 1.0f, 0.0f));
		model = translate(model, vec3(0.0f, -0.25f, 0.0f));
		model = scale(model, vec3(0.2f, 0.2f, 0.2f));

		myShader.setMat4("projection", projection);
		myShader.setMat4("view", view);
		myShader.setMat4("model", model);

		myShader.setVec3("cameraPos", cameraPos);
		myShader.setVec3("lightPos", lightPos);
		myShader.setInt("shaderType", shaderType);
		myShader.setFloat("weightTexture", weightTexture);

		myModel.Draw(myShader);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	float cameraSpeed = 2.5 * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPos += cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos -= cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
		lightX += 1.0;
	if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
		lightY += 1.0;
	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
		lightZ += 1.0;
	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
		lightX -= 1.0;
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
		lightY -= 1.0;
	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
		lightZ -= 1.0;
	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
		shaderType = 1;
	if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
		shaderType = 0;

	//switch between textures
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		textureType = 1;
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
		textureType = 2;
	if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
		textureType = 3;
	if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
		textureType = 4;

	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) {
		if (mPressed) {
			mPressed = false;
			weightTexture += 0.1;
			if (weightTexture >= 1.0)
				weightTexture = 0.2;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_RELEASE)
			mPressed = true;



	//std::cout << lightX << " " << lightY << " " << lightZ << std::endl; 
	//std::cout << cameraPos.x << " " << cameraPos.y << " " << cameraPos.z << std::endl;

	lightPos = vec3(lightX, lightY, lightZ);
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}