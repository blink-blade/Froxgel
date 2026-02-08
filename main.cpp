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
    MarchingCubes mc = MarchingCubes(0.2, 100);
    vector<float> vertices = mc.GenerateVertices();
    Mesh sphere("vec3 vec3 vec3", vertices, "simple_lighting", "simple_lighting");
    vertices = GenerateIsland(10, 50, 50, 5, 0.992f, 0.282f,  0.203f, 5.0, 3.0, 5.0, 0, -20, 0, 5);
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
    while (!window.ShouldClose()) {
        engineUpdates();
        lightUpdates();

        // Shadow map rendering
        glCullFace(GL_BACK);
        glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glClear(GL_DEPTH_BUFFER_BIT);
        ground.SwitchShader("depth", "depth");
        sphere.SwitchShader("depth", "depth");
        ground.ShaderUniformUpdates();
        sphere.ShaderUniformUpdates();
        ground.Draw(sunCamera);
        sphere.Draw(sunCamera);

        // Normal rendering
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, shadowMap);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, window.Width, window.Height);
        glCullFace(GL_BACK);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        ground.SwitchShader("simple_lighting", "simple_lighting");
        sphere.SwitchShader("simple_lighting", "simple_lighting");
        ground.ShaderUniformUpdates();
        skybox.ShaderUniformUpdates();
        sphere.ShaderUniformUpdates();

        ground.Draw(camera);
        skybox.Draw(camera);
        sphere.Draw(camera);
        // screen.Draw(camera);
        window.PollEvents();
    }

    window.ShutDown();
    return 0;
}
