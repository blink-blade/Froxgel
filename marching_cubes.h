#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <glm/detail/type_quat.hpp>

using namespace std;

class MarchingCubes;

class Cube
{
public:
    // The parent instance of MarchingCubes.
    MarchingCubes* Marcher;
    // East, west, north, south, top, bottom. East is positive x, top is positive y, and north is positive z.
    int East, West, North, South, Top, Bottom;
    // The eight corners' noise values, (west bottom south), (west bottom north), ect.
    float WBSLevel, WBNLevel, EBSLevel, EBNLevel, WTSLevel, WTNLevel, ETSLevel, ETNLevel;
    // The eight corners' state, either ground or air AKA on or off.
    bool WBSGround, WBNGround, EBSGround, EBNGround, WTSGround, WTNGround, ETSGround, ETNGround;

    int CubeIndex = 0;

    void Init(int i, int y, int z);

    Cube(MarchingCubes* marchingCubes);

    float GetNoiseValue(int x, int y, int z) const;

    void CalculateCubeConfigIndex();

    bool IsGround(float value) const;

private:

};

class MarchingCubes
{
public:
    // Anything below the surface level will be considered solid, anything above it will be considered air.
    float SurfaceLevel = 0.5;
    int GridSize = 25;
    vector<vector<vector<float>>> NoiseGrid;
    vector<vector<vector<Cube>>> Cubes;
    vector<float> Stewart;
    MarchingCubes(float groundLevel, int gridSize);

    void GenerateNoise();

    vector<float> GenerateVertices();

private:
};