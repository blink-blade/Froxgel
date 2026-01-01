#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <cmath>

using namespace std;

// A function that I stole from chatGPT like an absolute genius would. ():
std::vector<float> generateSphere(
    float radius,
    unsigned int sectorCount,  // longitude
    unsigned int stackCount    // latitude
) {
    std::vector<float> data;
    const float PI = 3.14159265359f;

    for (unsigned int i = 0; i < stackCount; ++i) {
        float stackAngle1 = PI / 2 - i * PI / stackCount;
        float stackAngle2 = PI / 2 - (i + 1) * PI / stackCount;

        float xy1 = radius * cosf(stackAngle1);
        float z1  = radius * sinf(stackAngle1);
        float xy2 = radius * cosf(stackAngle2);
        float z2  = radius * sinf(stackAngle2);

        for (unsigned int j = 0; j < sectorCount; ++j) {
            float sectorAngle1 = j * 2 * PI / sectorCount;
            float sectorAngle2 = (j + 1) * 2 * PI / sectorCount;

            // 4 points of the quad
            float x1 = xy1 * cosf(sectorAngle1);
            float y1 = xy1 * sinf(sectorAngle1);

            float x2 = xy2 * cosf(sectorAngle1);
            float y2 = xy2 * sinf(sectorAngle1);

            float x3 = xy2 * cosf(sectorAngle2);
            float y3 = xy2 * sinf(sectorAngle2);

            float x4 = xy1 * cosf(sectorAngle2);
            float y4 = xy1 * sinf(sectorAngle2);

            // UVs
            float u1 = (float)j / sectorCount;
            float u2 = (float)(j + 1) / sectorCount;
            float v1 = (float)i / stackCount;
            float v2 = (float)(i + 1) / stackCount;

            // Helper lambda to push one vertex
            auto pushVertex = [&](float x, float y, float z, float u, float v) {
                // Position
                data.push_back(x);
                data.push_back(y);
                data.push_back(z);

                // Normal (unit sphere)
                float invLen = 1.0f / radius;
                data.push_back(x * invLen);
                data.push_back(y * invLen);
                data.push_back(z * invLen);

                // UV
                data.push_back(u);
                data.push_back(v);
            };

            // Triangle 1
            pushVertex(x1, y1, z1, u1, v1);
            pushVertex(x2, y2, z2, u1, v2);
            pushVertex(x3, y3, z2, u2, v2);

            // Triangle 2
            pushVertex(x1, y1, z1, u1, v1);
            pushVertex(x3, y3, z2, u2, v2);
            pushVertex(x4, y4, z1, u2, v1);
        }
    }

    return data;
}
vector<float> generateGrid(int width, int height, int scale, int numsPerVertex) {
    vector<float> vertices(width * height * numsPerVertex * 6);
    int i = 0;
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            vertices[i++] = x * scale;
            vertices[i++] = 0.0f;
            vertices[i++] = y * scale;
            // Fill in the blank spaces that aren't position.
            for (int num = 3; num < numsPerVertex; num++) {
                vertices[i++] = 0.0f;
            }

            vertices[i++] = x * scale + scale;
            vertices[i++] = 0.0f;
            vertices[i++] = y * scale;
            for (int num = 3; num < numsPerVertex; num++) {
                vertices[i++] = 0.0f;
            }

            vertices[i++] = x * scale;
            vertices[i++] = 0.0f;
            vertices[i++] = y * scale + scale;
            for (int num = 3; num < numsPerVertex; num++) {
                vertices[i++] = 0.0f;
            }

            vertices[i++] = x * scale + scale;
            vertices[i++] = 0.0f;
            vertices[i++] = y * scale;
            for (int num = 3; num < numsPerVertex; num++) {
                vertices[i++] = 0.0f;
            }

            vertices[i++] = x * scale + scale;
            vertices[i++] = 0.0f;
            vertices[i++] = y * scale + scale;
            for (int num = 3; num < numsPerVertex; num++) {
                vertices[i++] = 0.0f;
            }

            vertices[i++] = x * scale;
            vertices[i++] = 0.0f;
            vertices[i++] = y * scale + scale;
            for (int num = 3; num < numsPerVertex; num++) {
                vertices[i++] = 0.0f;
            }
        }
    }
    cout << vertices.size() << endl;
    return vertices;
}
