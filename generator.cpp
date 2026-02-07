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
#include <glm/vec3.hpp>
#include <glm/ext/quaternion_geometric.hpp>
// This is brought over from an old C project, so it might not be beautiful, but it works.

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

float randd(glm::vec2 co) {
    return glm::fract(sin(glm::dot(co, glm::vec2(12.9898,78.233))) * 43758.5453);
}

glm::vec2 randomGradient(float ix, float iy) {
    glm::vec2 v;
    v.x = sin(ix * randd(glm::vec2(ix, iy)));
    v.y = cos(iy * randd(glm::vec2(ix, iy)));
    return v;
}

float rand3D(glm::vec3 co) {
    return glm::fract(sin(glm::dot(co, glm::vec3(12.9898f, 78.233f, 45.164f))) * 43758.5453f);
}

glm::vec3 randomGradient3D(float ix, float iy, float iz)
{
    float r1 = rand3D(glm::vec3(ix, iy, iz));
    float r2 = rand3D(glm::vec3(ix + 31.7f, iy + 17.3f, iz + 47.9f));

    float theta = r1 * 6.2831853f;   // 2π
    float z = r2 * 2.0f - 1.0f;
    float s = sqrt(1.0f - z * z);

    return glm::vec3(
        s * cos(theta),
        s * sin(theta),
        z
    );
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

    struct corner tlCorner{};
    struct corner blCorner{};
    struct corner trCorner{};
    struct corner brCorner{};
    tlCorner.x = x0, tlCorner.y = y0;
    blCorner.x = x0, blCorner.y = y1;
    trCorner.x = x1, trCorner.y = y0;
    brCorner.x = x1, brCorner.y = y1;
    tlCorner.gradientVec = randomGradient(tlCorner.x, tlCorner.y);
    blCorner.gradientVec = randomGradient(blCorner.x, blCorner.y);
    trCorner.gradientVec = randomGradient(trCorner.x, trCorner.y);
    brCorner.gradientVec = randomGradient(brCorner.x, brCorner.y);

    // Get distance vectors(A vector from the corner which points to the tile) for each corner of the octant.
    struct vec2d tlDistanceVector{};
    struct vec2d blDistanceVector{};
    struct vec2d trDistanceVector{};
    struct vec2d brDistanceVector{};
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

float noise3D(float x, float y, float z)
{
    int x0 = (int)floor(x);
    int y0 = (int)floor(y);
    int z0 = (int)floor(z);

    int x1 = x0 + 1;
    int y1 = y0 + 1;
    int z1 = z0 + 1;

    float sx = x - (float)x0;
    float sy = y - (float)y0;
    float sz = z - (float)z0;

    glm::vec3 p(x, y, z);

    // Gradients at cube corners
    glm::vec3 g000 = randomGradient3D(x0, y0, z0);
    glm::vec3 g100 = randomGradient3D(x1, y0, z0);
    glm::vec3 g010 = randomGradient3D(x0, y1, z0);
    glm::vec3 g110 = randomGradient3D(x1, y1, z0);
    glm::vec3 g001 = randomGradient3D(x0, y0, z1);
    glm::vec3 g101 = randomGradient3D(x1, y0, z1);
    glm::vec3 g011 = randomGradient3D(x0, y1, z1);
    glm::vec3 g111 = randomGradient3D(x1, y1, z1);

    // Distance vectors
    glm::vec3 d000 = p - glm::vec3(x0, y0, z0);
    glm::vec3 d100 = p - glm::vec3(x1, y0, z0);
    glm::vec3 d010 = p - glm::vec3(x0, y1, z0);
    glm::vec3 d110 = p - glm::vec3(x1, y1, z0);
    glm::vec3 d001 = p - glm::vec3(x0, y0, z1);
    glm::vec3 d101 = p - glm::vec3(x1, y0, z1);
    glm::vec3 d011 = p - glm::vec3(x0, y1, z1);
    glm::vec3 d111 = p - glm::vec3(x1, y1, z1);

    // Dot products
    float n000 = glm::dot(g000, d000);
    float n100 = glm::dot(g100, d100);
    float n010 = glm::dot(g010, d010);
    float n110 = glm::dot(g110, d110);
    float n001 = glm::dot(g001, d001);
    float n101 = glm::dot(g101, d101);
    float n011 = glm::dot(g011, d011);
    float n111 = glm::dot(g111, d111);

    // Interpolate along X
    float nx00 = interpolate(n000, n100, sx);
    float nx10 = interpolate(n010, n110, sx);
    float nx01 = interpolate(n001, n101, sx);
    float nx11 = interpolate(n011, n111, sx);

    // Interpolate along Y
    float nxy0 = interpolate(nx00, nx10, sy);
    float nxy1 = interpolate(nx01, nx11, sy);

    // Interpolate along Z
    return interpolate(nxy0, nxy1, sz);
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

float layeredNoise(int x, int y, int layerAmount, float frequency, float noiseAmplitude, unsigned int newSeed) {
    unsigned int * pSeed = &seed;
    *pSeed = newSeed;
    float amp = 1;
    float val = 0;
    for (int i = 0; i < layerAmount; i++) {
        val += noise((float)x * frequency, (float)y * frequency) * amp;

        frequency *= 2;
        amp /= 2;
    }

    return val * noiseAmplitude;
}

float layeredNoise3D(float x, float y, float z,
                     int layerAmount,
                     float frequency,
                     float noiseAmplitude,
                     unsigned int newSeed)
{
    unsigned int* pSeed = &seed;
    *pSeed = newSeed;

    float amp = 1.0f;
    float val = 0.0f;

    for (int i = 0; i < layerAmount; i++)
    {
        val += noise3D(x * frequency, y * frequency, z * frequency) * amp;

        frequency *= 2.0f;
        amp *= 0.5f;
    }

    return val * noiseAmplitude;
}