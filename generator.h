#include <inttypes.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include "helpers.h"
#include <vector>

extern float noise(float x, float y);
extern vector<vector<float>> generateNoiseMap(int noiseWidth, int noiseHeight, int layerAmount, float frequency, float noiseAmplitude, unsigned int newSeed);