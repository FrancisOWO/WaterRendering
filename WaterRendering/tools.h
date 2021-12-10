#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <learnopengl/camera.h>

extern float deltaTime, lastFrame;

extern bool firstMouse;
extern float lastX, lastY;
extern float pitch, yaw;
extern float fov;

extern Camera camera;

//处理按键输入
void processInput(GLFWwindow* window);

//GLFW初始化
void InitWindow(GLFWwindow*& window, int width, int height);

//加载纹理
unsigned int loadTexture(char const* path);
