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
float deltaTime;
float previousFrameTime;

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

void framebufferSizeCallback(GLFWwindow* wind, int width, int height) {
    glViewport(0, 0, width, height);
    float aspect = (float)width / (float)height;
    projection = glm::perspective(glm::radians(camera.Fov), aspect, nearPlane, farPlane);
    sunProjection = glm::ortho(-(float)width / 10, (float)width / 10, -(float)height / 10, (float)height / 10, nearPlane, farPlane / 10);
    // sunProjection = glm::perspective(glm::radians(camera.Fov), aspect, nearPlane, farPlane);
    window.Width = width;
    window.Height = height;
    CHECK_FRAMEBUFFER_STATUS();
}

unsigned int depthMapFBO;
unsigned int shadowMap;
const unsigned int SHADOW_WIDTH = 4096, SHADOW_HEIGHT = 4096;
void engineInits() {
    camera.IsPlayer = true;
    glfwSetFramebufferSizeCallback(window.GLFWWindow, framebufferSizeCallback);
    // glfwSetScrollCallback(window, scrollCallback);
    glfwSetInputMode(window.GLFWWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window.GLFWWindow, mouseCallback);

    glGenFramebuffers(1, &depthMapFBO);
    glGenTextures(1, &shadowMap);
    glBindTexture(GL_TEXTURE_2D, shadowMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    // attach depth texture as FBO's depth buffer
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);


    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glDepthMask(GL_TRUE);
    stbi_set_flip_vertically_on_load(true);
}

void engineUpdates() {
    timeValue = glfwGetTime();
    deltaTime = timeValue - previousFrameTime;
    previousFrameTime = timeValue;
    processInput();
    glFrontFace(GL_CW);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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

int xyToI(int x, int y, int width) {
    return x + width*y;
}