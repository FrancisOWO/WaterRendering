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

//�ص������������ӿڴ�С
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

//�ص������������������
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

//�ص���������������
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

//����������
void processInput(GLFWwindow* window);

//GLFW��ʼ��
void InitWindow(GLFWwindow*& window, int width, int height);
