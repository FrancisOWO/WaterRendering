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

//����������
void processInput(GLFWwindow* window);

//GLFW��ʼ��
void InitWindow(GLFWwindow*& window, int width, int height);

//��������
unsigned int loadTexture(char const* path);
