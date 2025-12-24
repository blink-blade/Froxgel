#ifndef MESH_H
#define MESH_H
#include <iostream>
#include <utility>
#include <vector>
#include "helpers.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

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
        shader.init(vertex.c_str(), frag.c_str());
        vertexData = std::move(vertexDataArg);
        vertexFormatRaw = std::move(vertexFormatRawArg);
        vertexFormatTerms = splitString(vertexFormatRaw, ' ');
        setupVertexFormatSizes();
        vertexStride = std::accumulate(vertexFormatSizes.begin(), vertexFormatSizes.end(), 0);
        numbersPerVertex = std::accumulate(vertexFormatGroups.begin(), vertexFormatGroups.end(), 0);
        vertexCount = vertexData.size() / numbersPerVertex;
        initBuffers();
    }

    void draw() {
        shader.use();
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, vertexCount);
    }

private:
    void initBuffers() {
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
            glVertexAttribPointer(i, size, GL_FLOAT, GL_FALSE, vertexStride, (void*)offset);
            glEnableVertexAttribArray(i);
        }

    }

    // Supported types: vec4, vec3, vec2, float
    void setupVertexFormatSizes() {
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

#endif