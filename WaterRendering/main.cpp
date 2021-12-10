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
const float CAMERA_X = 20.0f, CAMERA_Y = 50.0f, CAMERA_Z = 150.0f;	//找个能看见箱子的位置
Camera camera(glm::vec3(CAMERA_X, CAMERA_Y, CAMERA_Z));

int main()
{
	//计时器
	cTimer t0;

	//窗口初始化
	GLFWwindow* window = NULL;
	InitWindow(window, SCR_WIDTH, SCR_HEIGHT);

    /****************** 立方体 ****************/
    float cubeVertices[] = {
		//位置				 //纹理坐标
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

    //绑定
    unsigned int cubeVAO, cubeVBO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);

    glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

	//顶点属性
	//位置
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//纹理坐标
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

    //解绑
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

	//生成纹理
	unsigned int textureMap = loadTexture("cube/container2.png");
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureMap);

    //着色器
    Shader cubeShader("cube/cube.vert.glsl", "cube/cube.frag.glsl");

    /****************** 海面 ****************/
	//FFT点数N，振幅A，风向[vec2]wind，单块海面边长length，是否显示线框，海面块数(nx,nz)，海面位置偏移[vec2]offset
	//### 单块海面最终的边长是length*scale，渲染时会放大Model(目前scale=5.0f，在ocean.render()中)
	vector2 oceanOffset(-20.0f, 20.0f);
	cOcean ocean(64, 0.0005f, vector2(32.0f,32.0f), 64, false, 8, 6, oceanOffset);

	//变换矩阵
	glm::mat4 projection = glm::perspective(45.0f, (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f); 
	glm::mat4 view       = glm::mat4(1.0f);
	glm::mat4 model		 = glm::mat4(1.0f);
	glm::vec3 light_position;

	//深度测试
	glEnable(GL_DEPTH_TEST);

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
		projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);
		view = camera.GetViewMatrix();

		//光源位置 ### 光源并未画出！！！
		light_position = glm::vec3(1000.0f, 100.0f, -1000.0f);

        /****************** 水面 ****************/
		ocean.render(float(t0.elapsed(false)), light_position, projection, view, model, true);

        /****************** 立方体 ****************/
        if (1) {
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::scale(model, glm::vec3(30.0f));	//放大模型

            cubeShader.use();
			cubeShader.setMat4("projection", projection);
			cubeShader.setMat4("view", view);
			cubeShader.setMat4("model", model);

			cubeShader.setVec3("lightColor", glm::vec3(0.8f));
			cubeShader.setInt("textureMap", 0);

            //绘制
            glBindVertexArray(cubeVAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

		//交换缓冲，检查并调用事件
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	ocean.release();

	//释放资源
	glfwTerminate();

	return 0;
}
