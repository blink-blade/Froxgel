#ifndef WINDOW_H
#define WINDOW_H
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

using namespace std;

class Window {
public:
    GLFWwindow* GLFWWindow;
    int Width, Height;
    Window() {
        if (!glfwInit()) {
            throw std::invalid_argument("Failed to initialize GLFW");
        }

        GLFWWindow = glfwCreateWindow(800, 600, "Froxgel", nullptr, nullptr);
        if (!GLFWWindow) {
            glfwTerminate();
            throw std::invalid_argument("GLFWWindow could not be created");
        }

        glfwMakeContextCurrent(GLFWWindow);
        glfwGetWindowSize(GLFWWindow, &Width, &Height);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            throw std::invalid_argument("Failed to initialize GLAD");
        }

        cout << "OpenGL version: " << glGetString(GL_VERSION) << "\n";
    }

    bool ShouldClose() const {
        return glfwWindowShouldClose(GLFWWindow);
    }

    void PollEvents() const {
        glfwSwapBuffers(GLFWWindow);
        glfwPollEvents();
    }

    static void ShutDown() {
        glfwTerminate();
    }

private:
    // utility function for checking shader compilation/linking errors.
    // ------------------------------------------------------------------------
};

extern Window window;

#endif