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
    vector<float> vertices = generateSphere(1000, 100, 100);
    

    Mesh mesh("vec3 vec3 vec2", vertices, "simple_lighting", "simple_lighting");
    Skybox skybox("vec3 vec3 vec2", skyboxVertices, "skybox", "skybox");

    while (!window.shouldClose()) {
        engineUpdates();
        mesh.shaderUniformUpdates();
        mesh.draw();
        skybox.shaderUniformUpdates();
        skybox.draw();
        window.pollEvents();
    }

    window.shutDown();
    return 0;
}
