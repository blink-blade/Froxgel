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
    cameraSpeed = 6.0f;
    if(glfwGetKey(window.GLFWWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window.GLFWWindow, true);
    }
    if (glfwGetKey(window.GLFWWindow, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
        cameraSpeed = 90.0f;
    }
    if (glfwGetKey(window.GLFWWindow, GLFW_KEY_LEFT_ALT) == GLFW_PRESS) {
        cameraSpeed *= 10.0f;
    }
    camera.MovementSpeed = cameraSpeed;
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
    if (glfwGetKey(window.GLFWWindow, GLFW_KEY_SPACE) == GLFW_PRESS) {
        camera.ProcessKeyboard(UP, true);
    }
}