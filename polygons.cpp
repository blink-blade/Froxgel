#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <cmath>
#include <glm/vec3.hpp>
#include <glm/ext/quaternion_geometric.hpp>

#include "generator.h"
#include "shader.h"

using namespace std;

glm::vec3 calculateSurfaceNormal(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3) {
    // Need to fix this function, it will only work for half of the triangles. Depending on the orientation of the triangles, the subtractions need done differently.
    glm::vec3 U = p3 - p1;
    glm::vec3 V = p2 - p1;

    glm::vec3 normal((U.y * V.z) - (U.z * V.y), (U.z * V.x) - (U.x * V.z), (U.x * V.y) - (U.y * V.x));

    return glm::normalize(normal);
}

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

vector<float> generateGrid(
    int width,
    int height,
    float scale,
    int numsPerVertex,
    float r = 0.992156862745f,
    float g = 0.282352941176,
    float b = 0.203921568627,
    int offsetX = 0,
    int offsetY = 0,
    int offsetZ = 0,
    float noiseAmplitude = 1.0f
) {
    const float frameDepth = 10.0f;
    const float bottomY = offsetY - frameDepth;

    auto emit = [&](vector<float>& v, int& i,
                    float x, float y, float z,
                    const glm::vec3& n) {
        v[i++] = x; v[i++] = y; v[i++] = z;
        v[i++] = n.x; v[i++] = n.y; v[i++] = n.z;
        v[i++] = r; v[i++] = g; v[i++] = b;
        for (int k = 9; k < numsPerVertex; k++)
            v[i++] = 1.0f;
    };

    // Generate noise
    ComputeShader gen;
    gen.init("perlin", width + 1, height + 1, 1);
    gen.use();
    gen.setInt("noiseHeight", height + 1);
    gen.setFloat("noiseAmplitude", noiseAmplitude);
    gen.dispatch();

    vector<glm::vec4> img(gen.elementCount);
    glGetBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, gen.size, img.data());

    auto idx = [&](int x, int y) {
        return xyToI(x, y, gen.workGroupAmount.x);
    };

    // Height & normals
    vector<vector<float>> h(height + 1, vector<float>(width + 1));
    vector<vector<glm::vec3>> n(height + 1, vector<glm::vec3>(width + 1));

    for (int y = 0; y <= height; y++)
        for (int x = 0; x <= width; x++) {
            auto d = img[idx(x, y)];
            h[y][x] = d.x;
            n[y][x] = glm::vec3(d.y, d.z, d.w);
        }

    // Allocate vertices
    int terrainVerts = width * height * 6;
    int frameVerts   = (width * 2 + height * 2) * 6;
    int bottomVerts  = 6;

    vector<float> vertices(
        (terrainVerts + frameVerts + bottomVerts) * numsPerVertex
    );

    int i = 0;

    // Terrain
    for (int x = 0; x < width; x++)
        for (int y = 0; y < height; y++) {
            emit(vertices, i,
                 x * scale + offsetX,
                 h[y][x] + offsetY,
                 y * scale + offsetZ,
                 n[y][x]);

            emit(vertices, i,
                 (x + 1) * scale + offsetX,
                 h[y][x + 1] + offsetY,
                 y * scale + offsetZ,
                 n[y][x + 1]);

            emit(vertices, i,
                 x * scale + offsetX,
                 h[y + 1][x] + offsetY,
                 (y + 1) * scale + offsetZ,
                 n[y + 1][x]);

            emit(vertices, i,
                 (x + 1) * scale + offsetX,
                 h[y][x + 1] + offsetY,
                 y * scale + offsetZ,
                 n[y][x + 1]);

            emit(vertices, i,
                 (x + 1) * scale + offsetX,
                 h[y + 1][x + 1] + offsetY,
                 (y + 1) * scale + offsetZ,
                 n[y + 1][x + 1]);

            emit(vertices, i,
                 x * scale + offsetX,
                 h[y + 1][x] + offsetY,
                 (y + 1) * scale + offsetZ,
                 n[y + 1][x]);
        }


    // Frame
    for (int x = 0; x < width; x++) {
        float x0 = x * scale + offsetX;
        float x1 = (x + 1) * scale + offsetX;
        float zF = offsetZ;
        float zB = height * scale + offsetZ;

        emit(vertices, i, x0, h[0][x] + offsetY, zF, {0, 0, -1});
        emit(vertices, i, x0, bottomY, zF, {0, 0, -1});
        emit(vertices, i, x1, h[0][x + 1] + offsetY, zF, {0, 0, -1});
        emit(vertices, i, x1, h[0][x + 1] + offsetY, zF, {0, 0, -1});
        emit(vertices, i, x0, bottomY, zF, {0, 0, -1});
        emit(vertices, i, x1, bottomY, zF, {0, 0, -1});

        emit(vertices, i, x0, h[height][x] + offsetY, zB, {0, 0, 1});
        emit(vertices, i, x1, h[height][x + 1] + offsetY, zB, {0, 0, 1});
        emit(vertices, i, x0, bottomY, zB, {0, 0, 1});
        emit(vertices, i, x1, h[height][x + 1] + offsetY, zB, {0, 0, 1});
        emit(vertices, i, x1, bottomY, zB, {0, 0, 1});
        emit(vertices, i, x0, bottomY, zB, {0, 0, 1});
    }

    for (int y = 0; y < height; y++) {
        float z0 = y * scale + offsetZ;
        float z1 = (y + 1) * scale + offsetZ;
        float xL = offsetX;
        float xR = width * scale + offsetX;

        emit(vertices, i, xL, h[y + 1][0] + offsetY, z1, {-1, 0, 0});
        emit(vertices, i, xL, bottomY, z0, {-1, 0, 0});
        emit(vertices, i, xL, h[y][0] + offsetY, z0, {-1, 0, 0});
        emit(vertices, i, xL, bottomY, z1, {-1, 0, 0});
        emit(vertices, i, xL, bottomY, z0, {-1, 0, 0});
        emit(vertices, i, xL, h[y + 1][0] + offsetY, z1, {-1, 0, 0});

        emit(vertices, i, xR, bottomY, z0, {1, 0, 0});
        emit(vertices, i, xR, h[y + 1][width] + offsetY, z1, {1, 0, 0});
        emit(vertices, i, xR, h[y][width] + offsetY, z0, {1, 0, 0});
        emit(vertices, i, xR, bottomY, z0, {1, 0, 0});
        emit(vertices, i, xR, bottomY, z1, {1, 0, 0});
        emit(vertices, i, xR, h[y + 1][width] + offsetY, z1, {1, 0, 0});
    }


    // Bottom plane
    glm::vec3 up(0, 1, 0);
    float x0 = offsetX;
    float x1 = width * scale + offsetX;
    float z0 = offsetZ;
    float z1 = height * scale + offsetZ;

    emit(vertices, i, x0, bottomY, z0, up);
    emit(vertices, i, x1, bottomY, z0, up);
    emit(vertices, i, x0, bottomY, z1, up);
    emit(vertices, i, x1, bottomY, z0, up);
    emit(vertices, i, x1, bottomY, z1, up);
    emit(vertices, i, x0, bottomY, z1, up);

    return vertices;
}

vector<float> generateIsland(
    float radius,
    unsigned int sectorCount,  // longitude
    unsigned int stackCount,    // latitude
    float flatHeight,
    int offsetX = 0,
    int offsetY = 0,
    int offsetZ = 0,
    float noiseAmplitude = 1.0f
) {
    vector<float> sphereVertices = generateSphere(radius, sectorCount, stackCount);
    for (int i = 0; i < sphereVertices.size(); i += 9) {
        float x = sphereVertices[i];
        float y = sphereVertices[i + 1];
        float z = sphereVertices[i + 2];
        glm::vec3 normal = glm::normalize(glm::vec3(x, y, z));
        float noiseVal = layeredNoise3D(normal.x, normal.y, normal.z, 5, 1, noiseAmplitude, 100);
        glm::vec3 noiseOffset = normal * noiseVal;
        if (y >= flatHeight) {
            y /= 10;
        }
        x += noiseOffset.x;
        y += noiseOffset.y;
        z += noiseOffset.z;
        sphereVertices[i] = x + offsetX;
        sphereVertices[i + 1] = y + offsetY;
        sphereVertices[i + 2] = z + offsetZ;
    }
    return sphereVertices;
}