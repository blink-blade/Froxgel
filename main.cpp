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

    int dispatchSize = 5;
    int localSize = 8;

    ComputeShader densityComp;
    densityComp.init("density");
    densityComp.setDispatchSize(dispatchSize, dispatchSize, dispatchSize);
    size_t densityBufferSize = dispatchSize * dispatchSize * dispatchSize * localSize * localSize * localSize * sizeof(float);
    unsigned int densitySSBO = densityComp.CreateSSBO(densityBufferSize, 0, GL_STATIC_DRAW);

    ComputeShader mcComp;
    mcComp.init("marching_cubes");
    mcComp.setDispatchSize(dispatchSize, dispatchSize, dispatchSize);
    size_t bufferSize = sizeof(glm::vec4) + dispatchSize * dispatchSize * dispatchSize * localSize * localSize * localSize * 6 * sizeof(glm::vec4);
    unsigned int vertexSSBO = mcComp.CreateSSBO(bufferSize, 1, GL_DYNAMIC_DRAW);

    mcComp.use();
    mcComp.setInt("gridSize", localSize * dispatchSize);
    densityComp.use();
    densityComp.setInt("gridSize", localSize * dispatchSize);

    MarchingCubes mc = MarchingCubes(0.2, 5);
    vector<float> vertices = mc.GenerateVertices();
    GPUMesh mcm("vec4", "marching_cubes", "marching_cubes", vertexSSBO, sizeof(glm::vec4));
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
    densityComp.use();
    densityComp.setFloat("time", timeValue);
    densityComp.dispatch();
    while (!window.ShouldClose()) {
        engineUpdates();
        lightUpdates();


        glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
        mcComp.ResetCounter(vertexSSBO);
        mcComp.use();
        mcComp.setFloat("time", timeValue);
        mcComp.dispatch();

        // Shadow map rendering
        glCullFace(GL_BACK);
        glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glClear(GL_DEPTH_BUFFER_BIT);
        ground.SwitchShader("depth", "depth");
        mcm.SwitchShader("depth", "depth");
        ground.ShaderUniformUpdates();
        mcm.ShaderUniformUpdates();
        ground.Draw(sunCamera);
        mcm.Draw(sunCamera, mcComp.ReadCounter(vertexSSBO));

        // Normal rendering
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, shadowMap);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, window.Width, window.Height);
        glCullFace(GL_BACK);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        ground.SwitchShader("simple_lighting", "simple_lighting");
        mcm.SwitchShader("marching_cubes", "marching_cubes");
        ground.ShaderUniformUpdates();
        skybox.ShaderUniformUpdates();
        mcm.ShaderUniformUpdates();

        ground.Draw(camera);
        skybox.Draw(camera);


        mcm.Draw(camera, mcComp.ReadCounter(vertexSSBO));
        
        // screen.Draw(camera);
        window.PollEvents();
    }

    window.ShutDown();
    return 0;
}
