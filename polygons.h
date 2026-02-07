#ifndef POLYGON_H
#define POLYGON_H
#include <bits/stdc++.h>
#include <vector>

using namespace std;

extern std::vector<float> generateSphere(
    float radius,
    unsigned int sectorCount,  // longitude
    unsigned int stackCount,    // latitude
    float r = -1,
    float g = -1,
    float b = -1);
extern std::vector<float> generateSphereVerticesFlat(float radius, unsigned int sectorCount, unsigned int stackCount);
extern vector<float> generateGrid(
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
extern vector<float> generateIsland(
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