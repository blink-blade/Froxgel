#include "helpers.h"
#include <bits/stdc++.h>
#include <vector>

#include "camera.h"
#include "window.h"
#include "controls.h"

using namespace std;
float timeValue;

vector<string> splitString(string s, char delim) {
    stringstream ss(s);
    vector<string> output;

    while (getline(ss, s, delim)) {

        // store token string in the vector
        output.push_back(s);
    }
    return output;
}

void CHECK_FRAMEBUFFER_STATUS() {
    GLenum status;
    status = glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER);
    switch(status) {
        case GL_FRAMEBUFFER_COMPLETE:
            break;

        case GL_FRAMEBUFFER_UNSUPPORTED:
            /* choose different formats */
            break;

        default:
            /* programming error; will fail on all hardware */
            fputs("Framebuffer Error\n", stderr);
            exit(-1);
    }
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    projection = glm::perspective(glm::radians(camera.Fov), 800.0f / 600.0f, nearPlane, farPlane);
    CHECK_FRAMEBUFFER_STATUS();
}

void engineInits() {
    glfwSetFramebufferSizeCallback(window.GLFWWindow, framebufferSizeCallback);
    // glfwSetScrollCallback(window, scrollCallback);
    glfwSetInputMode(window.GLFWWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window.GLFWWindow, mouseCallback);

    glEnable(GL_DEPTH_TEST);
    // stbi_set_flip_vertically_on_load(true);
}