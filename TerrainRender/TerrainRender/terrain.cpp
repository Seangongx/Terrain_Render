#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include "data.h"
#include "shader.h"
//#include "camera.h"
#include "lod.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>//引入图形计算库
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
//using namespace std;

#define FILE_VERTICES "Data\\new_vertices.txt"
#define FILE_INDICES "Data\\new_indices.txt"
#define SETTINGS

#ifdef SETTINGS//程序配置项设置

#define TESTVROWS 16384
#define TESTIROWS 32258

//settings
#define DEF_WINDOW_WIDTH 800
#define DEF_WINDOW_HEIGHT 600
// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

// Camera
glm::vec3 cameraPositionv = glm::vec3(0.0f, 0.0f, 10.0f);
glm::vec3 worldUpv = glm::vec3(0.0f, 1.0f, 0.0f);
CCamera ourCamera(cameraPositionv, worldUpv);
//float lastX = 0, lastY = 0;
float lastX = DEF_WINDOW_WIDTH / 2.0f;
float lastY = DEF_WINDOW_HEIGHT / 2.0f;
bool firstMouse = true;

#endif


//callback function definition
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

int main()
{
	CTerrain data(FILE_VERTICES, FILE_INDICES);

	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(DEF_WINDOW_WIDTH, DEF_WINDOW_HEIGHT, "TerrainDemo", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	
	//回调函数注册处
	glfwMakeContextCurrent(window);//将我们窗口的上下文设置为当前线程的主上下文了。	
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);//窗口大小改变
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	//设置全局OpenGL状态，增加深度信息，必须在glad找到所有函数指针之后才能调用
	glEnable(GL_DEPTH_TEST);
	//绘制属性初始化
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);//捕捉鼠标光标，让其在窗口内消失

	Shader ourShader("Shaders\\terrain_shader.vs", "Shaders\\terrain_shader.fs");

	data.Create(129);

	while (!glfwWindowShouldClose(window))
	{
		//计算移动速度的技巧
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		// -----
		processInput(window);

		// render
		// ------
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ourShader.use();

		ourCamera.SetProj((float)DEF_WINDOW_WIDTH, (float)DEF_WINDOW_HEIGHT, 0.1f, 100.0f);
		ourShader.setMat4("projection", ourCamera.projection);
		ourCamera.SetView();
		ourShader.setMat4("view", ourCamera.view);
		ourCamera.SetModel_Edit();
		ourShader.setMat4("model", ourCamera.model);

		data.Render();

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	data.Delete();

	glfwTerminate();

	return 0;
}


void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS)
	{
		GLint polyState = 0;
		glGetIntegerv(GL_POLYGON_MODE, &polyState);
		if (polyState == GL_FILL)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else if(polyState == GL_LINE)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
	}

	if (glfwGetKey(window, GLFW_KEY_CAPS_LOCK) == GLFW_PRESS)
	{
		GLint cursorState = glfwGetInputMode(window, GLFW_CURSOR);
		if (cursorState == GLFW_CURSOR_DISABLED)
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
		else if (cursorState == GLFW_CURSOR_NORMAL)
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		ourCamera.ProcessKeyboard(FORWARD, deltaTime);
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		ourCamera.ProcessKeyboard(BACKWARD, deltaTime);
	else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		ourCamera.ProcessKeyboard(LEFT, deltaTime);
	else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		ourCamera.ProcessKeyboard(RIGHT, deltaTime);
	else if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		ourCamera.ProcessKeyboard(START, deltaTime);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
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

	ourCamera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	ourCamera.ProcessMouseScroll(yoffset);
}
