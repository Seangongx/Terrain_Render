#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include"data.h"
#include"shader.h"
#include"camera.h"
#include"file.h"
using namespace std;

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>//����ͼ�μ����
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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
Camera ourCamera(cameraPositionv, worldUpv);
//float lastX = 0, lastY = 0;
float lastX = DEF_WINDOW_WIDTH / 2.0f;
float lastY = DEF_WINDOW_HEIGHT / 2.0f;
bool firstMouse = true;

//callback function definition
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

int main()
{
	int vrows = f_get_rows("vertices.txt");
	int vcols = f_get_columns("vertices.txt");
	int irows = f_get_rows("indices.txt");
	int icols = f_get_columns("indices.txt");

	// load data
	Terrain_Data data(vrows, vcols, irows, icols);
	double** Vertices = data.Load_Vertices("vertices.txt");
	unsigned int** Indices = data.Load_Indices("indices.txt");
	//d_show2vd(Vertices, vrows, vcols - 1);
	//d_show2vui(Indices, irows, icols - 1);

	//pick_verticesΪ����һά���飬 pick_indicesΪ����һά����
	double *pick_vertices = new double[TESTVROWS * (MAXCOLUMNS - 1)];
	unsigned int *pick_indices = new unsigned int[TESTIROWS * (MAXCOLUMNS - 1)];
	if (!d_pick_vertices(Vertices, pick_vertices, vrows, vcols - 1))
	{
		cout << "data load error!" << endl;
	}
	//d_show1vd(pick_vertices, vrows, vcols - 1);
	if (!d_pick_indices(Indices, pick_indices, irows, icols - 1))
	{
		cout << "data load error!" << endl;
	}
	//d_show1vui(pick_indices, irows, icols - 1);


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

	//�ص�����ע�ᴦ
	glfwMakeContextCurrent(window);//�����Ǵ��ڵ�����������Ϊ��ǰ�̵߳����������ˡ�	
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);//���ڴ�С�ı�
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);//��׽����꣬�����ڴ�������ʧ

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	//����ȫ��OpenGL״̬�����������Ϣ��������glad�ҵ����к���ָ��֮����ܵ���
	glEnable(GL_DEPTH_TEST);

	Shader ourShader("terrain_shader.vs", "terrain_shader.fs");

	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	int sizeof_vertices = TESTVROWS * 3 * sizeof(double);
	int sizeof_indices = TESTIROWS * 3 * sizeof(unsigned int);
	//ԭ��д��
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof_vertices, pick_vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof_indices, pick_indices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_DOUBLE, GL_TRUE, 3 * sizeof(double), (void*)0);

	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	while (!glfwWindowShouldClose(window))
	{
		//�����ƶ��ٶȵļ���
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

		// draw our first triangle
		ourShader.use();

		glm::mat4 projection;
		projection = glm::perspective(ourCamera.Zoom, (float)DEF_WINDOW_WIDTH / (float)DEF_WINDOW_HEIGHT, 0.1f, 500.0f); //glm::radians(fov)
		ourShader.setMat4("projection", projection);

		glm::mat4 view = ourCamera.GetViewMatrix();
		ourShader.setMat4("view", view);

		glm::mat4 model;
		model = glm::rotate(model, 90.0f, glm::vec3(1.0, 0.0, 0.0));//���ע�͵�����90�ȵ�ƫת
		model = glm::scale(model, glm::vec3(0.03, 0.03, 0.03));
		ourShader.setMat4("model", model);

		glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
		glDrawElements(GL_TRIANGLES, 3 * irows, GL_UNSIGNED_INT, 0);//�����������θ�������������������Ҫ���ƵĶ���������������θ���*3

		// glBindVertexArray(0); // no need to unbind it every time 

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

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