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
    vector<float> vertices = generateSphere(5, 100, 100);
    Mesh sphere("vec3 vec3 vec3", vertices, "simple_lighting", "simple_lighting");
    vertices = generateGrid(100, 100, 10, 9, -500, -10, -500, true, 10);
    Mesh ground("vec3 vec3 vec3", vertices, "simple_lighting", "simple_lighting");
    vertices = generateSphere(0.33, 100, 100);
    Mesh sunSphere("vec3 vec3 vec3", vertices, "simple_lighting", "simple_lighting");
    vertices = {
         0.0f,  0.0f, 0.0f,  1.0f, 1.0f,
         0.0f, -1.0f, 0.0f,  1.0f, 0.0f,
        -1.0f, -1.0f, 0.0f,  0.0f, 0.0f,

        -1.0f,  0.0f, 0.0f,  0.0f, 1.0f,
        0.0f,  0.0f, 0.0f,  1.0f, 1.0f,
        -1.0f, -1.0f, 0.0f,  0.0f, 0.0f

    };
    Mesh screen("vec3 vec2", vertices, "screen", "screen");
    Skybox skybox("vec3 vec3 vec2", skyboxVertices, "skybox", "skybox");
    screen.shader.use();
    screen.shader.setInt("shadowMap", 0);
    while (!window.shouldClose()) {
        engineUpdates();
        lightUpdates();

        // Shadow map rendering
        glCullFace(GL_FRONT);
        glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glClear(GL_DEPTH_BUFFER_BIT);
        sphere.switchShader("depth", "depth");
        ground.switchShader("depth", "depth");
        sphere.shaderUniformUpdates();
        ground.shaderUniformUpdates();
        sphere.draw(sunCamera);
        ground.draw(sunCamera);

        // Normal rendering
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, shadowMap);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, window.width, window.height);
        glCullFace(GL_BACK);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        sphere.switchShader("simple_lighting", "simple_lighting");
        ground.switchShader("simple_lighting", "simple_lighting");
        sunSphere.switchShader("simple_lighting", "simple_lighting");
        sphere.shaderUniformUpdates();
        ground.shaderUniformUpdates();
        sunSphere.shaderUniformUpdates(glm::vec3(sunCamera.Position.x / 1, sunCamera.Position.y / 1, sunCamera.Position.z / 1));
        skybox.shaderUniformUpdates();

        sphere.draw(camera);
        ground.draw(camera);
        sunSphere.draw(camera);
        skybox.draw(camera);
        screen.draw(camera);
        window.pollEvents();
    }

    window.shutDown();
    return 0;
}
