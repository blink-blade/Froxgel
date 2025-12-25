#ifndef CONTROLS_H
#define CONTROLS_H
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "window.h"

void mouseCallback(GLFWwindow* window, double xpos, double ypos);
void processInput();
#endif