// GLProject.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>
#include "libs/glad_loader_docs.h"
#include <GLFW/glfw3.h>
#include <learnopengl/shader_m.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include "Polygon.cpp"
#include "Table.h"
#include <learnopengl/camera.h>
#include "Room.h"
#include "stb_image.h"

using namespace glm;

int SCR_WIDTH = 1280;
int SCR_HEIGHT = 720;

Camera camera(glm::vec3(0.0f, 2.0f, 5.0f)); // camera postion
float lastX = 400.0f;						// camera postion
float lastY = 300.0f;						// camera postion
float deltaTime = 0.0f;						// for camera movement
float lastFrame = 0.0f;						// for camera movement
bool firstMouse = true;

int cameraMode = 3; // 1 is up
					// 2 is
					// 3 is free camera -this inital-

// method for camera movement
void mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
	if (cameraMode != 3) // exti if mode 1 or 2
		return;

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(
		xoffset,
		yoffset);
}
void setCameraMode(int mode)
{
	cameraMode = mode;

	if (mode == 1)
	{
		// Top View
		camera.Position = glm::vec3(0.0f, 2.5f, 0.0f);
		camera.WorldUp = glm::vec3(0.0f, 0.0f, -1.0f);
		camera.Yaw = -90.0f;
		camera.Pitch = -89.0f;
	}
	else if (mode == 2)
	{
		// Sitting View
		camera.Position = glm::vec3(0.0f, 0.8f, 2.5f);
		camera.WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
		camera.Yaw = -90.0f;
		camera.Pitch = -2.0f;
	}
	else if (mode == 3)
	{
		// Free Camera
		camera.Position = glm::vec3(0.0f, 2.0f, 5.0f);
		camera.WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
		camera.Yaw = -90.0f;
		camera.Pitch = 0.0f;
	}

	camera.ProcessMouseMovement(0.0f, 0.0f);
	firstMouse = true;
}
void processInput(GLFWwindow *window) // check input
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		setCameraMode(1);
	else if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
		setCameraMode(2);
	else if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
		setCameraMode(3);

	if (cameraMode == 3)
	{
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			camera.ProcessKeyboard(FORWARD, deltaTime);

		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			camera.ProcessKeyboard(BACKWARD, deltaTime);

		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			camera.ProcessKeyboard(LEFT, deltaTime);

		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			camera.ProcessKeyboard(RIGHT, deltaTime);
	}
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) // set mode from input
{
	if (action == GLFW_PRESS)
	{
		if (key == GLFW_KEY_1)
			setCameraMode(1);

		else if (key == GLFW_KEY_2)
			setCameraMode(2);

		else if (key == GLFW_KEY_3)
			setCameraMode(3);
	}
}

unsigned int loadTexture(const char *path) // method for Texture
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrChannels;

	// stbi_set_flip_vertically_on_load(true);

	unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 0);

	if (data)
	{
		glBindTexture(GL_TEXTURE_2D, textureID);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Failed to load texture: " << path << std::endl;
	}

	return textureID;
}

int main()
{
	//  screen Preparation
	glfwInit();
	GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "my project 3", NULL, NULL);
	glfwMakeContextCurrent(window);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	glEnable(GL_DEPTH_TEST);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetKeyCallback(window, key_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	Shader ourShader("./shaders/vs/L3.vs", "./shaders/fs/L3.fs");

	Table table;
	Room room;

	unsigned int woodTexture = loadTexture("./textures/crate.jpg"); // image for texture

	table.top.setTexture(woodTexture); // set image in texture

	ourShader.use();
	ourShader.setInt("texture1", 0);

	// setings for projection
	mat4 projection = mat4(1.0f);
	projection = perspective(radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	ourShader.setMat4("projection", projection);
	//-------

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// for camera movement
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// for draw
		glm::mat4 view = camera.GetViewMatrix();
		// view = lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		ourShader.setMat4("view", view);

		room.draw(ourShader);
		table.draw(ourShader);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
