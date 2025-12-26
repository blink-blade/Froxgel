#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "window.h"

#include <iostream>

#include "camera.h"
#include "controls.h"
#include "mesh.h"

using namespace std;

int main() {
    engineInits();
    vector<float> vertices {
        // -500.5f, -500.5f, 0.0f, 0.0f, 0.0f,
        //  500.5f, -500.5f, 500.0f, 0.5f, 1.0f,
        //  500.0f,  500.5f, 0.0f, 1.0f, 0.0f
    };
    for (int x = 0; x < 6000; x += 10) {
        for (int y = 0; y < 6000; y += 10) {
            vertices.push_back(x * 10);
            vertices.push_back(y * 10);
            vertices.push_back(0.0f);

            vertices.push_back(0.0f);
            vertices.push_back(0.0f);


            vertices.push_back(x * 10 + 5);
            vertices.push_back(y * 10);
            vertices.push_back((x + y) / 10);

            vertices.push_back(0.5f);
            vertices.push_back(1.0f);


            vertices.push_back(x * 10 + 10);
            vertices.push_back(y * 10);
            vertices.push_back(0.0f);

            vertices.push_back(1.0f);
            vertices.push_back(0.0f);
        }
    }
    Mesh mesh("vec3 vec2", vertices, "base_tex", "base_tex");

    while (!window.shouldClose()) {
        processInput();
        timeValue = glfwGetTime();

        glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        cout << camera.Position.x << " " << camera.Position.y << " " << camera.Position.z << endl;
        mesh.shaderUniformUpdates();
        mesh.draw();
        window.pollEvents();
    }

    window.shutDown();
    return 0;
}
