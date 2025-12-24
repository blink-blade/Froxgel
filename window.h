#ifndef WINDOW_H
#define WINDOW_H
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

using namespace std;

class Window {
public:
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

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            throw std::invalid_argument("Failed to initialize GLAD");
        }

        cout << "OpenGL version: " << glGetString(GL_VERSION) << "\n";
    }

    bool shouldClose() const {
        return glfwWindowShouldClose(GLFWWindow);
    }

    void pollEvents() const {
        glfwSwapBuffers(GLFWWindow);
        glfwPollEvents();
    }

    static void shutDown() {
        glfwTerminate();
    }

private:
    // utility function for checking shader compilation/linking errors.
    // ------------------------------------------------------------------------
    GLFWwindow* GLFWWindow;
};

#endif