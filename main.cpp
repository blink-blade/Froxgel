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
    Mesh sphere("vec3 vec3 vec3", vertices, "simple_lighting", "simple_lighting");
    vertices = generateGrid(1000, 1000, 10, 9, -5000, -100, -5000);
    Mesh ground("vec3 vec3 vec3", vertices, "simple_lighting", "simple_lighting");
    vertices = generateSphere(100, 100, 100);
    Mesh sunSphere("vec3 vec3 vec3", vertices, "simple_lighting", "simple_lighting");
    Skybox skybox("vec3 vec3 vec2", skyboxVertices, "skybox", "skybox");

    while (!window.shouldClose()) {
        engineUpdates();
        lightUpdates();
        sphere.shaderUniformUpdates();
        sphere.draw(sunCamera);
        ground.shaderUniformUpdates();
        ground.draw(sunCamera);
        // sunSphere.shaderUniformUpdates(sunCamera.Position);
        // sunSphere.draw();
        skybox.shaderUniformUpdates();
        skybox.draw(sunCamera);
        window.pollEvents();
    }

    window.shutDown();
    return 0;
}
