#include "helpers.h"
#include <bits/stdc++.h>
#include <vector>

#include "camera.h"
#include "window.h"
#include "controls.h"
#define STB_IMAGE_IMPLEMENTATION

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "stb_image.h"

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

    // glFrontFace(GL_CCW);
    glEnable(GL_DEPTH_TEST);
    stbi_set_flip_vertically_on_load(true);
}

void engineUpdates() {
    timeValue = glfwGetTime();
    processInput();
}

unsigned int loadTexture(char const * path, bool linearFiltering = false) {
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;


        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
        if (linearFiltering) {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        }
        else {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        }

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}