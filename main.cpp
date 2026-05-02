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

    int dispatchSizeX = 200; int dispatchSizeY = 200; int dispatchSizeZ = 200;
    int localSize = 4;

    ComputeShader mcComp;
    mcComp.init("marching_cubes");
    mcComp.setDispatchSize(dispatchSizeX, dispatchSizeY, dispatchSizeZ);
    size_t bufferSize = 2048000000;
    unsigned int vertexSSBO = mcComp.CreateSSBO(bufferSize, 0, GL_DYNAMIC_DRAW);

    mcComp.use();
    mcComp.setInt("localSize", localSize);
    mcComp.setInt("gridSizeX", localSize * dispatchSizeX);
    mcComp.setInt("gridSizeY", localSize * dispatchSizeY);
    mcComp.setInt("gridSizeZ", localSize * dispatchSizeZ);

    MarchingCubes mc = MarchingCubes(0.2, 5);
    vector<float> vertices = mc.GenerateVertices();
    GPUMesh mcm("vec3 vec3 vec3", "marching_cubes", "simple_lighting", vertexSSBO, sizeof(unsigned int));
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
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
    mcComp.ResetCounter(vertexSSBO);
    mcComp.use();
    mcComp.setInt("iterationCount", 3);
    mcComp.setFloat("surfaceLevel", 0.5);
    mcComp.setFloat("time", timeValue / 7);
    mcComp.dispatch();
    while (!window.ShouldClose()) {


        engineUpdates();
        lightUpdates();

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
        mcm.SwitchShader("marching_cubes", "simple_lighting");
        ground.ShaderUniformUpdates();
        skybox.ShaderUniformUpdates();
        mcm.ShaderUniformUpdates();

        ground.Draw(camera);
        skybox.Draw(camera);

        glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT |
                        GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT);
        mcm.Draw(camera, mcComp.ReadCounter(vertexSSBO));
        cout << mcComp.ReadCounter(vertexSSBO) / 3 << endl;
        
        // screen.Draw(camera);
        window.PollEvents();
    }

    window.ShutDown();
    return 0;
}
