#ifndef POLYGON_H
#define POLYGON_H
#include <bits/stdc++.h>
#include <vector>

using namespace std;

extern std::vector<float> generateSphere(float radius, unsigned int sectorCount, unsigned int stackCount);
extern vector<float> generateGrid(int width, int height, int scale, int numsPerVertex);

#endif