// In this engine, triangles are supposed to have their points listed counter-clockwise.
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "window.h"

#include <iostream>

#include "camera.h"
#include "controls.h"
#include "filesystem.h"
#include "marching_cubes.h"
#include "mesh.h"
#include "polygons.h"
#include "skybox.h"

using namespace std;

int main() {
    engineInits();
    MarchingCubes mc = MarchingCubes();
    vector<float> vertices = mc.GenerateVertices();
    Mesh sphere("vec3 vec3 vec3", vertices, "simple_lighting", "simple_lighting");
    vertices = generateIsland(10, 100, 100, 5, 0.992f, 0.282f,  0.203f, 5.0, 3.0, 5.0, 0, -20, 0, 5);
    Mesh ground("vec3 vec3 vec3", vertices, "simple_lighting", "simple_lighting");
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
        glCullFace(GL_BACK);
        glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glClear(GL_DEPTH_BUFFER_BIT);
        ground.switchShader("depth", "depth");
        sphere.switchShader("depth", "depth");
        ground.shaderUniformUpdates();
        sphere.shaderUniformUpdates();
        ground.draw(sunCamera);
        sphere.draw(sunCamera);

        // Normal rendering
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, shadowMap);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, window.width, window.height);
        glCullFace(GL_BACK);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        ground.switchShader("simple_lighting", "simple_lighting");
        sphere.switchShader("simple_lighting", "simple_lighting");
        ground.shaderUniformUpdates();
        skybox.shaderUniformUpdates();
        sphere.shaderUniformUpdates();

        ground.draw(camera);
        skybox.draw(camera);
        sphere.draw(camera);
        screen.draw(camera);
        window.pollEvents();
    }

    window.shutDown();
    return 0;
}
