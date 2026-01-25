#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <cmath>

using namespace std;

// A function that I stole from chatGPT like an absolute genius would. ():
std::vector<float> generateSphereVerticesFlat(
    float radius,
    unsigned int sectorCount,
    unsigned int stackCount
) {
    std::vector<float> data;
    const float PI = 3.14159265359f;

    auto pushVertex = [&](float x, float y, float z,
                          float nx, float ny, float nz,
                          float u, float v) {
        data.push_back(x);
        data.push_back(y);
        data.push_back(z);
        data.push_back(nx);
        data.push_back(ny);
        data.push_back(nz);
        data.push_back(u);
        data.push_back(v);
    };

    auto computeNormal = [](float ax, float ay, float az,
                            float bx, float by, float bz,
                            float cx, float cy, float cz,
                            float& nx, float& ny, float& nz) {
        float ux = bx - ax;
        float uy = by - ay;
        float uz = bz - az;

        float vx = cx - ax;
        float vy = cy - ay;
        float vz = cz - az;

        nx = uy * vz - uz * vy;
        ny = uz * vx - ux * vz;
        nz = ux * vy - uy * vx;

        float len = std::sqrt(nx * nx + ny * ny + nz * nz);
        nx /= len;
        ny /= len;
        nz /= len;
    };

    for (unsigned int i = 0; i < stackCount; ++i) {
        float stackAngle1 = PI / 2 - i * PI / stackCount;
        float stackAngle2 = PI / 2 - (i + 1) * PI / stackCount;

        float xy1 = radius * std::cos(stackAngle1);
        float z1  = radius * std::sin(stackAngle1);
        float xy2 = radius * std::cos(stackAngle2);
        float z2  = radius * std::sin(stackAngle2);

        for (unsigned int j = 0; j < sectorCount; ++j) {
            float sectorAngle1 = j * 2 * PI / sectorCount;
            float sectorAngle2 = (j + 1) * 2 * PI / sectorCount;

            // Quad corners
            float x1 = xy1 * std::cos(sectorAngle1);
            float y1 = xy1 * std::sin(sectorAngle1);

            float x2 = xy2 * std::cos(sectorAngle1);
            float y2 = xy2 * std::sin(sectorAngle1);

            float x3 = xy2 * std::cos(sectorAngle2);
            float y3 = xy2 * std::sin(sectorAngle2);

            float x4 = xy1 * std::cos(sectorAngle2);
            float y4 = xy1 * std::sin(sectorAngle2);

            float u1 = (float)j / sectorCount;
            float u2 = (float)(j + 1) / sectorCount;
            float v1 = (float)i / stackCount;
            float v2 = (float)(i + 1) / stackCount;

            // -------- Triangle 1 --------
            float nx, ny, nz;
            computeNormal(
                x1, y1, z1,
                x2, y2, z2,
                x3, y3, z2,
                nx, ny, nz
            );

            pushVertex(x1, y1, z1, nx, ny, nz, u1, v1);
            pushVertex(x2, y2, z2, nx, ny, nz, u1, v2);
            pushVertex(x3, y3, z2, nx, ny, nz, u2, v2);

            // -------- Triangle 2 --------
            computeNormal(
                x1, y1, z1,
                x3, y3, z2,
                x4, y4, z1,
                nx, ny, nz
            );

            pushVertex(x1, y1, z1, nx, ny, nz, u1, v1);
            pushVertex(x3, y3, z2, nx, ny, nz, u2, v2);
            pushVertex(x4, y4, z1, nx, ny, nz, u2, v1);
        }
    }

    return data;
}

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
                data.push_back(x * invLen);
                data.push_back(y * invLen);
                data.push_back(z * invLen);
            };

            // Triangle 1
            pushVertex(x3, y3, z2, u2, v2);
            pushVertex(x2, y2, z2, u1, v2);
            pushVertex(x1, y1, z1, u1, v1);

            // Triangle 2
            pushVertex(x4, y4, z1, u2, v1);
            pushVertex(x3, y3, z2, u2, v2);
            pushVertex(x1, y1, z1, u1, v1);
        }
    }

    return data;
}

vector<float> generateGrid(int width, int height, int scale, int numsPerVertex, int offsetX, int offsetY, int offsetZ) {
    vector<float> vertices(width * height * numsPerVertex * 6);
    int i = 0;
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            vertices[i++] = x * scale + offsetX;
            vertices[i++] = 0.0f + offsetY;
            vertices[i++] = y * scale + offsetZ;
            vertices[i++] = 0.0f;
            vertices[i++] = 1.0f;
            vertices[i++] = 0.0f;
            // Fill in the blank spaces that aren't position.
            for (int num = 6; num < numsPerVertex; num++) {
                vertices[i++] = 1.0f;
            }

            vertices[i++] = x * scale + scale + offsetX;
            vertices[i++] = 0.0f + offsetY;
            vertices[i++] = y * scale + offsetZ;
            vertices[i++] = 0.0f;
            vertices[i++] = 1.0f;
            vertices[i++] = 0.0f;
            for (int num = 6; num < numsPerVertex; num++) {
                vertices[i++] = 1.0f;
            }

            vertices[i++] = x * scale + offsetX;
            vertices[i++] = 0.0f + offsetY;
            vertices[i++] = y * scale + scale + offsetZ;
            vertices[i++] = 0.0f;
            vertices[i++] = 1.0f;
            vertices[i++] = 0.0f;
            for (int num = 6; num < numsPerVertex; num++) {
                vertices[i++] = 1.0f;
            }

            vertices[i++] = x * scale + scale + offsetX;
            vertices[i++] = 0.0f + offsetY;
            vertices[i++] = y * scale + offsetZ;
            vertices[i++] = 0.0f;
            vertices[i++] = 1.0f;
            vertices[i++] = 0.0f;
            for (int num = 6; num < numsPerVertex; num++) {
                vertices[i++] = 1.0f;
            }

            vertices[i++] = x * scale + scale + offsetX;
            vertices[i++] = 0.0f + offsetY;
            vertices[i++] = y * scale + scale + offsetZ;
            vertices[i++] = 0.0f;
            vertices[i++] = 1.0f;
            vertices[i++] = 0.0f;
            for (int num = 6; num < numsPerVertex; num++) {
                vertices[i++] = 1.0f;
            }

            vertices[i++] = x * scale + offsetX;
            vertices[i++] = 0.0f + offsetY;
            vertices[i++] = y * scale + scale + offsetZ;
            vertices[i++] = 0.0f;
            vertices[i++] = 1.0f;
            vertices[i++] = 0.0f;
            for (int num = 6; num < numsPerVertex; num++) {
                vertices[i++] = 1.0f;
            }
        }
    }
    cout << vertices.size() << endl;
    return vertices;
}
