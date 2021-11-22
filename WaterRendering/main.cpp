#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <learnopengl/camera.h>

#include "src/timer.h"
#include "src/ocean.h"

#include "tools.h"

//���ڴ�С
//const int SCR_WIDTH = 640, SCR_HEIGHT = 360;
const int SCR_WIDTH = 800, SCR_HEIGHT = 450;

bool firstMouse = true;
float pitch = 0.0f, yaw = 0.0f;
float fov = 45.0f;
float lastX = SCR_WIDTH / 2.0;
float lastY = SCR_HEIGHT / 2.0;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

//�����
const float CAMERA_X = 0.0f, CAMERA_Y = 50.0f, CAMERA_Z = -20.0f;
Camera camera(glm::vec3(CAMERA_X, CAMERA_Y, CAMERA_Z));

int main() {

	//��ʱ��
	cTimer t0;

	//���ڳ�ʼ��
	GLFWwindow* window = NULL;
	InitWindow(window, SCR_WIDTH, SCR_HEIGHT);

	//ˮ��
	cOcean ocean(64, 0.0005f, vector2(32.0f,32.0f), 64, false);

	//�任����
	glm::mat4 Projection = glm::perspective(45.0f, (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f); 
	glm::mat4 View       = glm::mat4(1.0f);
	glm::mat4 Model      = glm::mat4(1.0f);
	glm::vec3 light_position;

	//VAO
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);

	//��Ⱦѭ��
	while (!glfwWindowShouldClose(window)) {
		float currentFrame = float(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		//### ˮ��ϴ�����������ʱ���ƶ��ٶȣ������ƶ������ԣ�����
		deltaTime *= 50;

		//��������
		processInput(window);

		//�������
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//�任����
		Projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);
		View = camera.GetViewMatrix();

		//��Դλ�� ### ��Դ��δ����������
		light_position = glm::vec3(1000.0f, 100.0f, -1000.0f);

		//ˮ����Ⱦ
		glBindVertexArray(VAO);
		ocean.render(float(t0.elapsed(false)), light_position, Projection, View, Model, true);

		//�������壬��鲢�����¼�
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	ocean.release();

	//�ͷ���Դ
	glfwTerminate();

	return 0;
}
