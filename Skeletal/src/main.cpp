#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <cstdlib> // for std::abort

#include "Shader.h"
#include "Texture2D.h"


void FrameBufferResizeCallback(GLFWwindow*, int, int);
void ProcessInput(GLFWwindow*);
static void APIENTRY OpenGLErrorCallback(GLenum, GLenum, unsigned int, GLenum, int, const char*, const void*);

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE); // For error notifications


	GLFWwindow* window = glfwCreateWindow(800, 600, "Skeletal Animation", nullptr, nullptr);

	if (window == nullptr)
	{
		std::cout << "Failed to initalize GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Window callbacks
	glfwSetFramebufferSizeCallback(window, FrameBufferResizeCallback);

	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// OpenGL error handling
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(OpenGLErrorCallback, nullptr);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, true);

	glViewport(0, 0, 800, 600);

	Shader shader("res/shaders/basic.vs", "res/shaders/basic.frag");
	shader.Bind();

	Texture2D texture("res/textures/tiles.png", true);
	Texture2D texture1("res/textures/brick.jpg", false);

	float index = 2.0f;

	float vertices[] = {
		// position			  // Colors             // Tex coordinates
		0.5f, 0.5f, 0.0f,     1.0f, 0.0f, 0.0f,     2.0f, 2.0f, // top right 0
		0.5f, 0.0f, 0.0f,     0.0f, 1.0f, 0.0f,     2.0f, 0.0f, // bottom right 1
		0.0f, 0.0f, 0.0f,     0.0f, 0.0f, 1.0f,     0.0f, 0.0f, // bottom left 2 
		0.0f, 0.5f, 0.0f,	  1.0f, 1.0f, 0.0f,     0.0f, 2.0f // top left  3
	};

	unsigned int indices[] = {
		0, 1, 2,
		2, 0, 3
	};
	
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 4 * 8, vertices, GL_STATIC_DRAW);

	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(float) * 8, static_cast<void*>(0));
	glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(float) * 8, reinterpret_cast<void*>(sizeof(float) * 3));
	glVertexAttribPointer(2, 2, GL_FLOAT, false, sizeof(float) * 8, reinterpret_cast<void*>(sizeof(float)*6));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	while (!glfwWindowShouldClose(window))
	{
		ProcessInput(window);
		// Render
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		

		shader.Bind();
		texture.Bind(0);
		shader.SetInt("u_texture", 0);
		texture1.Bind(1);
		shader.SetInt("u_texture1", 1);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::rotate(model, static_cast<float>(glfwGetTime()), glm::vec3(0.0f, 0.0f, 1.0f));
		shader.SetMat4("u_transform", model);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;

}


void FrameBufferResizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void ProcessInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}

static void APIENTRY OpenGLErrorCallback(
	GLenum src, GLenum type, 
	unsigned int id, GLenum severity, 
	int length, const char* msg, 
	const void* userParam)
{
	std::cerr << msg << std::endl;
	if (severity == GL_DEBUG_SEVERITY_HIGH) {
		std::cerr << "High severity..." << std::endl;
		std::terminate();
	}
}
