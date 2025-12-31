// In this engine, triangles are supposed to have their points listed counter-clockwise.
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "window.h"

#include <iostream>

#include "camera.h"
#include "controls.h"
#include "mesh.h"
#include "polygons.h"

using namespace std;

int main() {
    engineInits();
    vector<float> vertices = generateGrid(100, 100, 10, 5);
    

    Mesh mesh("vec3 vec2", vertices, "base_tex", "base_tex");

    while (!window.shouldClose()) {
        processInput();
        timeValue = glfwGetTime();

        glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        cout << camera.Position.x << " " << camera.Position.y << " " << camera.Position.z << endl;
        mesh.shaderUniformUpdates();
        mesh.draw();
        window.pollEvents();
    }

    window.shutDown();
    return 0;
}
