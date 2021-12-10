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
const float CAMERA_X = 20.0f, CAMERA_Y = 50.0f, CAMERA_Z = 150.0f;	//�Ҹ��ܿ������ӵ�λ��
Camera camera(glm::vec3(CAMERA_X, CAMERA_Y, CAMERA_Z));

int main()
{
	//��ʱ��
	cTimer t0;

	//���ڳ�ʼ��
	GLFWwindow* window = NULL;
	InitWindow(window, SCR_WIDTH, SCR_HEIGHT);

    /****************** ������ ****************/
    float cubeVertices[] = {
		//λ��				 //��������
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    //��
    unsigned int cubeVAO, cubeVBO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);

    glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

	//��������
	//λ��
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//��������
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

    //���
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

	//��������
	unsigned int textureMap = loadTexture("cube/container2.png");
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureMap);

    //��ɫ��
    Shader cubeShader("cube/cube.vert.glsl", "cube/cube.frag.glsl");

    /****************** ���� ****************/
	//FFT����N�����A������[vec2]wind�����麣��߳�length���Ƿ���ʾ�߿򣬺������(nx,nz)������λ��ƫ��[vec2]offset
	//### ���麣�����յı߳���length*scale����Ⱦʱ��Ŵ�Model(Ŀǰscale=5.0f����ocean.render()��)
	vector2 oceanOffset(-20.0f, 20.0f);
	cOcean ocean(64, 0.0005f, vector2(32.0f,32.0f), 64, false, 8, 6, oceanOffset);

	//�任����
	glm::mat4 projection = glm::perspective(45.0f, (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f); 
	glm::mat4 view       = glm::mat4(1.0f);
	glm::mat4 model		 = glm::mat4(1.0f);
	glm::vec3 light_position;

	//��Ȳ���
	glEnable(GL_DEPTH_TEST);

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
		projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);
		view = camera.GetViewMatrix();

		//��Դλ�� ### ��Դ��δ����������
		light_position = glm::vec3(1000.0f, 100.0f, -1000.0f);

        /****************** ˮ�� ****************/
		ocean.render(float(t0.elapsed(false)), light_position, projection, view, model, true);

        /****************** ������ ****************/
        if (1) {
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::scale(model, glm::vec3(30.0f));	//�Ŵ�ģ��

            cubeShader.use();
			cubeShader.setMat4("projection", projection);
			cubeShader.setMat4("view", view);
			cubeShader.setMat4("model", model);

			cubeShader.setVec3("lightColor", glm::vec3(0.8f));
			cubeShader.setInt("textureMap", 0);

            //����
            glBindVertexArray(cubeVAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

		//�������壬��鲢�����¼�
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	ocean.release();

	//�ͷ���Դ
	glfwTerminate();

	return 0;
}
