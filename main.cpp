// In this engine, triangles are supposed to have their points listed counter-clockwise.
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "window.h"

#include <iostream>

#include "camera.h"
#include "controls.h"
#include "filesystem.h"
#include "mesh.h"
#include "polygons.h"
#include "skybox.h"

using namespace std;

int main() {
    engineInits();
    vector<float> vertices = generateSphere(100, 100, 100);
    
    Mesh mesh("vec3 vec3 vec3", vertices, "simple_lighting", "simple_lighting");
    vertices = generateGrid(1000, 1000, 100, 9, -50000, -100, -50000);
    Mesh mesh2("vec3 vec3 vec3", vertices, "simple_lighting", "simple_lighting");
    Skybox skybox("vec3 vec3 vec2", skyboxVertices, "skybox", "skybox");

    while (!window.shouldClose()) {
        engineUpdates();
        mesh.shaderUniformUpdates();
        mesh.draw();
        mesh2.shaderUniformUpdates();
        mesh2.draw();
        skybox.shaderUniformUpdates();
        skybox.draw();
        window.pollEvents();
    }

    window.shutDown();
    return 0;
}
