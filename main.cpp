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
    unsigned int skyboxTexture = loadTexture(FileSystem::getPath("images/highresskybox.png").c_str(), true);
    Mesh skybox("vec3 vec3 vec2", skyboxVertices, "skybox", "skybox");

    while (!window.shouldClose()) {
        glm::mat4 rotationMat(1);
        rotationMat = glm::rotate(rotationMat, 0.001f, glm::vec3(0.0, 0.5, 1.0));
        sunDir = glm::vec3(rotationMat * glm::vec4(sunDir, 1.0));;
        // glEnable(GL_CULL_FACE);
        // glCullFace(GL_FRONT);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        processInput();
        timeValue = glfwGetTime();

        glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, skyboxTexture);
        // cout << camera.Position.x << " " << camera.Position.y << " " << camera.Position.z << endl;
        mesh.shaderUniformUpdates();
        mesh.draw();
        skybox.shaderUniformUpdates();
        skybox.draw();
        window.pollEvents();
    }

    window.shutDown();
    return 0;
}
