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

//回调函数：调整视口大小
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

//回调函数：处理鼠标输入
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

//回调函数：滚轮缩放
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

//处理按键输入
void processInput(GLFWwindow* window);

//GLFW初始化
void InitWindow(GLFWwindow*& window, int width, int height);
