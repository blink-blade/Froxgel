#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "camera.h"
#include "window.h"
int lastX = 0, lastY = 0;

void mouseCallback(GLFWwindow* window, double xpos, double ypos)
{
    static bool firstMouse = true;


    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xOffset = xpos - lastX;
    float yOffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xOffset, yOffset);
}

void processInput() {
    if(glfwGetKey(window.GLFWWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window.GLFWWindow, true);
    }
    if (glfwGetKey(window.GLFWWindow, GLFW_KEY_W) == GLFW_PRESS) {
        camera.ProcessKeyboard(FORWARD, true);
    }
    if (glfwGetKey(window.GLFWWindow, GLFW_KEY_S) == GLFW_PRESS) {
        camera.ProcessKeyboard(BACKWARD, true);
    }
    if (glfwGetKey(window.GLFWWindow, GLFW_KEY_A) == GLFW_PRESS) {
        camera.ProcessKeyboard(LEFT, true);
    }
    if (glfwGetKey(window.GLFWWindow, GLFW_KEY_D) == GLFW_PRESS) {
        camera.ProcessKeyboard(RIGHT, true);
    }
}