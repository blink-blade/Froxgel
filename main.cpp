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
    vector<float> vertices = generateSphere(10, 100, 100);
    Mesh sphere("vec3 vec3 vec3", vertices, "simple_lighting", "simple_lighting");
    vertices = generateGrid(100, 100, 1, 9, -50, -10, -50);
    Mesh ground("vec3 vec3 vec3", vertices, "simple_lighting", "simple_lighting");
    vertices = generateSphere(10, 100, 100);
    Mesh sunSphere("vec3 vec3 vec3", vertices, "simple_lighting", "simple_lighting");
    vertices = {
        -1.0f, -1.0f, 0.0f,  0.0f, 0.0f,
         0.0f, -1.0f, 0.0f,  1.0f, 0.0f,
         0.0f,  0.0f, 0.0f,  1.0f, 1.0f,

        -1.0f, -1.0f, 0.0f,  0.0f, 0.0f,
         0.0f,  0.0f, 0.0f,  1.0f, 1.0f,
        -1.0f,  0.0f, 0.0f,  0.0f, 1.0f
    };
    Mesh screen("vec3 vec2", vertices, "screen", "screen");
    Skybox skybox("vec3 vec3 vec2", skyboxVertices, "skybox", "skybox");
    screen.shader.use();
    screen.shader.setInt("depthMap", 0);
    while (!window.shouldClose()) {
        engineUpdates();
        lightUpdates();

        glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glClear(GL_DEPTH_BUFFER_BIT);
        sphere.switchShader("depth", "depth");
        ground.switchShader("depth", "depth");
        sphere.shaderUniformUpdates();
        ground.shaderUniformUpdates();
        sphere.draw(sunCamera, sunProjection);
        ground.draw(sunCamera, sunProjection);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, depthMap);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, window.width, window.height);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        sphere.switchShader("simple_lighting", "simple_lighting");
        ground.switchShader("simple_lighting", "simple_lighting");
        sunSphere.switchShader("simple_lighting", "simple_lighting");
        sphere.shaderUniformUpdates();
        ground.shaderUniformUpdates();
        sunSphere.shaderUniformUpdates(sunCamera.Position);
        skybox.shaderUniformUpdates();

        sphere.draw(sunCamera, sunProjection);
        ground.draw(sunCamera, sunProjection);
        sunSphere.draw(sunCamera, sunProjection);
        skybox.draw(sunCamera, sunProjection);
        screen.draw(sunCamera, sunProjection);
        window.pollEvents();
    }

    window.shutDown();
    return 0;
}
