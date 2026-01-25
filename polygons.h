#ifndef POLYGON_H
#define POLYGON_H
#include <bits/stdc++.h>
#include <vector>

using namespace std;

extern std::vector<float> generateSphere(float radius, unsigned int sectorCount, unsigned int stackCount);
extern std::vector<float> generateSphereVerticesFlat(float radius, unsigned int sectorCount, unsigned int stackCount);
extern vector<float> generateGrid(int width, int height, int scale, int numsPerVertex, int offsetX, int offsetY, int offsetZ, bool doNoise = false, float noiseScale = 1.0);

#endif