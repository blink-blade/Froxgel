#ifndef MESH_H
#define MESH_H
#include <iostream>
#include <utility>
#include <vector>
#include "helpers.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "camera.h"
#include "lighting.h"
#include "shader.h"

using namespace std;

class Mesh {
public:
    string vertexFormatRaw;
    vector<string> vertexFormatTerms;
    vector<int> vertexFormatSizes;
    vector<int> vertexFormatGroups;
    int vertexStride;
    int numbersPerVertex;
    vector<float> vertexData;
    int vertexCount;
    unsigned int VBO, VAO;
    Shader shader = Shader();

    Mesh(string vertexFormatRawArg, vector<float> vertexDataArg, string vertex, string frag) {
        SwitchShader(vertex, frag);
        vertexData = std::move(vertexDataArg);
        vertexFormatRaw = std::move(vertexFormatRawArg);
        vertexFormatTerms = splitString(vertexFormatRaw, ' ');
        SetupVertexFormatSizes();
        vertexStride = std::accumulate(vertexFormatSizes.begin(), vertexFormatSizes.end(), 0);
        numbersPerVertex = std::accumulate(vertexFormatGroups.begin(), vertexFormatGroups.end(), 0);
        vertexCount = vertexData.size() / numbersPerVertex;
        InitBuffers();
    }

    void SwitchShader(string vertex, string frag) {
        shader = Shader();
        shader.init(vertex.c_str(), frag.c_str());
    }

    void ShaderUniformUpdates(glm::vec3 offset = glm::vec3(0.0f)) const {
        shader.use();
        shader.setVec3("offset", offset);
        shader.setFloat("time", timeValue);
        shader.setMat4("sunSpaceMatrix", sunProjection * sunCamera.GetViewMatrix());
        shader.setVec3("dirLight.ambient", 0.1f * 0.2, 0.1f * 0.2, 0.1f * 0.2f);
        shader.setVec3("dirLight.diffuse", 0.4f * 2, 0.4f * 2, 0.4f * 2);
        shader.setVec3("dirLight.specular", 0.5f * 2, 0.5f * 2, 0.5f * 2);
        shader.setVec3("dirLight.direction", sunDir.x, sunDir.y, sunDir.z);
    }

    void Draw(Camera cam) const {
        shader.use();
        shader.setVec3("cameraPos", cam.Position);
        shader.setMat4("spaceMatrix", projection * cam.GetViewMatrix());
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, vertexCount);
    }

private:
    void InitBuffers() {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertexCount * vertexStride, vertexData.data(), GL_STATIC_DRAW);

        for (int i = 0; i < vertexFormatGroups.size(); ++i) {
            int size = vertexFormatGroups[i];
            int offset = 0;
            if (i > 0) {
                offset = std::accumulate(vertexFormatGroups.begin(), vertexFormatGroups.begin() + i, 0) * sizeof(float);
            }
            glVertexAttribPointer(i, size, GL_FLOAT, GL_FALSE, vertexStride, reinterpret_cast<void *>(offset));
            glEnableVertexAttribArray(i);
        }

    }
    // Supported types: vec4, vec3, vec2, float
    void SetupVertexFormatSizes() {
        for (string term : vertexFormatTerms) {
            if (term == "vec4") {
                vertexFormatSizes.push_back(sizeof(float) * 4.0f);
                vertexFormatGroups.push_back(4);
            }
            else if (term == "vec3") {
                vertexFormatSizes.push_back(sizeof(float) * 3.0f);
                vertexFormatGroups.push_back(3);
            }
            else if (term == "vec2") {
                vertexFormatSizes.push_back(sizeof(float) * 2.0f);
                vertexFormatGroups.push_back(2);
            }
            else if (term == "float") {
                vertexFormatSizes.push_back(sizeof(float) * 1.0f);
                vertexFormatGroups.push_back(1);
            }
            else {
                throw runtime_error("Unknown vertex format term");
            }
        }
    }

};

class GPUMesh {
public:
    string vertexFormatRaw;
    vector<string> vertexFormatTerms;
    vector<int> vertexFormatSizes;
    vector<int> vertexFormatGroups;
    int vertexStride;
    int numbersPerVertex;
    unsigned int SSBO, VAO;
    Shader shader = Shader();

    GPUMesh(string vertexFormatRawArg, string vertex, string frag, unsigned int SSBOArg, unsigned int baseOffset = 0) {
        SwitchShader(vertex, frag);
        vertexFormatRaw = std::move(vertexFormatRawArg);
        vertexFormatTerms = splitString(vertexFormatRaw, ' ');
        SetupVertexFormatSizes();
        vertexStride = std::accumulate(vertexFormatSizes.begin(), vertexFormatSizes.end(), 0);
        numbersPerVertex = std::accumulate(vertexFormatGroups.begin(), vertexFormatGroups.end(), 0);
        SSBO = SSBOArg;
        InitBuffers(baseOffset);
    }

    void ShaderUniformUpdates(glm::vec3 offset = glm::vec3(0.0f)) const {
        shader.use();
        shader.setVec3("offset", offset);
        shader.setFloat("time", timeValue);
        shader.setMat4("sunSpaceMatrix", sunProjection * sunCamera.GetViewMatrix());
        shader.setVec3("dirLight.ambient", 0.1f * 0.2, 0.1f * 0.2, 0.1f * 0.2f);
        shader.setVec3("dirLight.diffuse", 0.4f * 2, 0.4f * 2, 0.4f * 2);
        shader.setVec3("dirLight.specular", 0.5f * 2, 0.5f * 2, 0.5f * 2);
        shader.setVec3("dirLight.direction", sunDir.x, sunDir.y, sunDir.z);
    }

    void Draw(Camera cam, int vertexCount) const {
        shader.use();
        shader.setVec3("cameraPos", cam.Position);
        shader.setMat4("spaceMatrix", projection * cam.GetViewMatrix());
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, vertexCount);
    }

    void SwitchShader(string vertex, string frag) {
        shader = Shader();
        shader.init(vertex.c_str(), frag.c_str());
    }

private:
    void InitBuffers(unsigned int baseOffset) {
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, SSBO);
        for (int i = 0; i < vertexFormatGroups.size(); ++i) {
            int size = vertexFormatGroups[i];
            int offset = 0;
            if (i > 0) {
                offset = std::accumulate(vertexFormatGroups.begin(), vertexFormatGroups.begin() + i, 0) * sizeof(float);
            }
            glVertexAttribPointer(i, size, GL_FLOAT, GL_FALSE, vertexStride, reinterpret_cast<void *>(baseOffset + offset));
            glEnableVertexAttribArray(i);
        }

    }
    // Supported types: vec4, vec3, vec2, float
    void SetupVertexFormatSizes() {
        for (string term : vertexFormatTerms) {
            if (term == "vec4") {
                vertexFormatSizes.push_back(sizeof(float) * 4.0f);
                vertexFormatGroups.push_back(4);
            }
            else if (term == "vec3") {
                vertexFormatSizes.push_back(sizeof(float) * 3.0f);
                vertexFormatGroups.push_back(3);
            }
            else if (term == "vec2") {
                vertexFormatSizes.push_back(sizeof(float) * 2.0f);
                vertexFormatGroups.push_back(2);
            }
            else if (term == "float") {
                vertexFormatSizes.push_back(sizeof(float) * 1.0f);
                vertexFormatGroups.push_back(1);
            }
            else {
                throw runtime_error("Unknown vertex format term");
            }
        }
    }
};

extern void addMesh(Mesh mesh);
extern void switchAllMeshShaders(string vert, string frag);
#endif
