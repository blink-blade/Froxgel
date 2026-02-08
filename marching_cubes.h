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
    //     y
    //     |
    //     4------5
    //    /|     /|
    //   7------6 |
    //   | |    | |
    //   | 0----|-1 ---- x
    //   |/     |/
    //   3------2
    //  /
    // z
    // The parent instance of MarchingCubes.
    MarchingCubes* Marcher;
    // East, west, north, south, top, bottom. East is positive x, top is positive y, and north is positive z.
    int East, West, North, South, Top, Bottom;
    // The eight corners' noise values, (west bottom south), (east bottom north), ect.
    float WBSLevel, EBSLevel, EBNLevel, WBNLevel, WTSLevel, ETSLevel, ETNLevel, WTNLevel;
    // The eight corners' state, either ground or air AKA on or off.
    bool WBSGround, EBSGround, EBNGround, WBNGround, WTSGround, ETSGround, ETNGround, WTNGround;

    int CubeIndex = 0;

    void Init(int i, int y, int z);

    Cube(MarchingCubes* marchingCubes);

    float GetNoiseValue(int x, int y, int z) const;

    static glm::vec3 InterpolateVerts(glm::vec3 p1, glm::vec3 p2);

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