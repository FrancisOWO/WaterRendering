#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <learnopengl/camera.h>

#include "src/timer.h"
#include "src/ocean.h"

#include "tools.h"

//窗口大小
//const int SCR_WIDTH = 640, SCR_HEIGHT = 360;
const int SCR_WIDTH = 800, SCR_HEIGHT = 450;

bool firstMouse = true;
float pitch = 0.0f, yaw = 0.0f;
float fov = 45.0f;
float lastX = SCR_WIDTH / 2.0;
float lastY = SCR_HEIGHT / 2.0;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

//摄像机
const float CAMERA_X = 0.0f, CAMERA_Y = 50.0f, CAMERA_Z = -20.0f;
Camera camera(glm::vec3(CAMERA_X, CAMERA_Y, CAMERA_Z));

int main() {

	//计时器
	cTimer t0;

	//窗口初始化
	GLFWwindow* window = NULL;
	InitWindow(window, SCR_WIDTH, SCR_HEIGHT);

	//水面
	cOcean ocean(64, 0.0005f, vector2(32.0f,32.0f), 64, false);

	//变换矩阵
	glm::mat4 Projection = glm::perspective(45.0f, (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f); 
	glm::mat4 View       = glm::mat4(1.0f);
	glm::mat4 Model      = glm::mat4(1.0f);
	glm::vec3 light_position;

	//VAO
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);

	//渲染循环
	while (!glfwWindowShouldClose(window)) {
		float currentFrame = float(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		//### 水面较大，需提升按键时的移动速度，否则移动不明显！！！
		deltaTime *= 50;

		//处理输入
		processInput(window);

		//清除缓冲
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//变换矩阵
		Projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);
		View = camera.GetViewMatrix();

		//光源位置 ### 光源并未画出！！！
		light_position = glm::vec3(1000.0f, 100.0f, -1000.0f);

		//水面渲染
		glBindVertexArray(VAO);
		ocean.render(float(t0.elapsed(false)), light_position, Projection, View, Model, true);

		//交换缓冲，检查并调用事件
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	ocean.release();

	//释放资源
	glfwTerminate();

	return 0;
}
