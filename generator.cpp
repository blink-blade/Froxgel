#include <inttypes.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include "helpers.h"
#include <vector>
#include <algorithm>
#include <iostream>
#include <glm/common.hpp>
#include <glm/vec2.hpp>
#include <glm/ext/quaternion_geometric.hpp>

struct vec2d {
    float x;
    float y;
};

struct corner {
    float x;
    float y;
    glm::vec2 gradientVec;
};


unsigned int seed = 100;
float **mapNoiseMap;
struct corner **cornerGrid;

struct vec2d possibleGradientVectors[8];

float randd(glm::vec2 co) {
    return glm::fract(sin(glm::dot(co, glm::vec2(12.9898,78.233))) * 43758.5453);
}

glm::vec2 randomGradient(float ix, float iy) {
    glm::vec2 v;
    v.x = sin(ix * randd(glm::vec2(ix, iy)));
    v.y = cos(iy * randd(glm::vec2(ix, iy)));
    return v;
}

 
float interpolate(float a0, float a1, float w) {
    return (a1 - a0) * (3.0 - w * 2.0) * w * w + a0;
}
 
// Sample Perlin noise at coordinates x, y
float noise(float x, float y) {
    // Get the corner positions, x0 is left, x1 is left
    int x0 = (int)x; 
    int y0 = (int)y;
    int x1 = x0 + 1;
    int y1 = y0 + 1;

    // Compute Interpolation weights
    float sx = x - (float)x0;
    float sy = y - (float)y0;

    struct corner tlCorner;
    struct corner blCorner;
    struct corner trCorner;
    struct corner brCorner;
    tlCorner.x = x0, tlCorner.y = y0;
    blCorner.x = x0, blCorner.y = y1;
    trCorner.x = x1, trCorner.y = y0;
    brCorner.x = x1, brCorner.y = y1;
    tlCorner.gradientVec = randomGradient(tlCorner.x, tlCorner.y);
    blCorner.gradientVec = randomGradient(blCorner.x, blCorner.y);
    trCorner.gradientVec = randomGradient(trCorner.x, trCorner.y);
    brCorner.gradientVec = randomGradient(brCorner.x, brCorner.y);

    // Get distance vectors(A vector from the corner which points to the tile) for each corner of the octant.
    struct vec2d tlDistanceVector;
    struct vec2d blDistanceVector;
    struct vec2d trDistanceVector;
    struct vec2d brDistanceVector;
    tlDistanceVector.x = x - tlCorner.x, tlDistanceVector.y = y - tlCorner.y; 
    blDistanceVector.x = x - blCorner.x, blDistanceVector.y = y - blCorner.y; 
    trDistanceVector.x = x - trCorner.x, trDistanceVector.y = y - trCorner.y; 
    brDistanceVector.x = x - brCorner.x, brDistanceVector.y = y - brCorner.y; 

    // Get dot product from the distance vectors and the gradient vectors.
    float tlDotProduct = (tlDistanceVector.x * tlCorner.gradientVec.x) + (tlDistanceVector.y * tlCorner.gradientVec.y);
    float blDotProduct = (blDistanceVector.x * blCorner.gradientVec.x) + (blDistanceVector.y * blCorner.gradientVec.y);
    float trDotProduct = (trDistanceVector.x * trCorner.gradientVec.x) + (trDistanceVector.y * trCorner.gradientVec.y);
    float brDotProduct = (brDistanceVector.x * brCorner.gradientVec.x) + (brDistanceVector.y * brCorner.gradientVec.y);

    // Interpolation.
    float tlTrInterpolation = interpolate(tlDotProduct, trDotProduct, sx);
    float blBrInterpolation = interpolate(blDotProduct, brDotProduct, sx);

    return interpolate(tlTrInterpolation, blBrInterpolation, sy);

}

vector<vector<float>> generateNoiseMap(int noiseWidth, int noiseHeight, int layerAmount, float frequency, float noiseAmplitude, unsigned int newSeed) {
    float val = 0, freq;
    unsigned int * pSeed = &seed;
    *pSeed = newSeed;

    // Allocate memory for the array.
    vector<vector<float>> noiseMap;

    frequency /= 50.0;
    
    for (int y = 0; y < noiseHeight; y++)
    {
        noiseMap.push_back(vector<float>());
        for (int x = 0; x < noiseWidth; x++)
        {
            float amp = 1;
            val = 0;
            freq = frequency;
            for (int i = 0; i < layerAmount; i++) {
                val += noise((float)x * freq, (float)y * freq) * amp;

                freq *= 2;
                amp /= 2;
            }

            noiseMap[y].push_back(val * noiseAmplitude);
        }
    }
    return noiseMap;
}

// This is for getting the noise values with python.
float getNoiseValue(int x, int y) {
    return mapNoiseMap[y][x];
}