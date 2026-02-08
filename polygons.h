#ifndef POLYGON_H
#define POLYGON_H
#include <bits/stdc++.h>
#include <vector>

using namespace std;

extern void computeNormal(float ax, float ay, float az,
                    float bx, float by, float bz,
                    float cx, float cy, float cz,
                    float& nx, float& ny, float& nz);
extern std::vector<float> GenerateSphere(
    float radius,
    unsigned int sectorCount,  // longitude
    unsigned int stackCount,    // latitude
    float r = -1,
    float g = -1,
    float b = -1);
extern std::vector<float> GenerateSphereVerticesFlat(float radius, unsigned int sectorCount, unsigned int stackCount);
extern vector<float> GenerateGrid(
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
);
extern vector<float> GenerateIsland(
    float radius,
    unsigned int sectorCount,  // longitude
    unsigned int stackCount,    // latitude
    float flatHeight,
    float r = 0.992156862745f,
    float g = 0.282352941176,
    float b = 0.203921568627,
    float scaleX = 1.0f,
    float scaleY = 1.0f,
    float scaleZ = 1.0f,
    int offsetX = 0,
    int offsetY = 0,
    int offsetZ = 0,
    float noiseAmplitude = 1.0f
);
#endif