#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "window.h"

#include <iostream>

#include "mesh.h"

using namespace std;

int main() {
    Window window;
    vector<float> vertices {
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
         0.5f, -0.5f, 0.0f, 0.5f, 1.0f,
         0.0f,  0.5f, 0.0f, 1.0f, 0.0f
    };
    Mesh mesh("vec3 vec2", vertices, "base_tex", "base_tex");

    while (!window.shouldClose()) {
        glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        for (int term : mesh.vertexFormatGroups) {
            // cout << mesh.numbersPerVertex << endl;
        }
        mesh.draw();
        window.pollEvents();
    }

    window.shutDown();
    return 0;
}
