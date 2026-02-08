#include "marching_cubes.h"

#include <glm/ext/quaternion_trigonometric.hpp>
#include <glm/gtc/quaternion.hpp>

#include "generator.h"
#include "marching_cubes_tables.h"
#include "polygons.h"

MarchingCubes::MarchingCubes(float groundLevel, int gridSize) {
    SurfaceLevel = groundLevel;
    GridSize = gridSize;
}

Cube::Cube(MarchingCubes* marchingCubes) {
    Marcher = marchingCubes;
}

float Cube::GetNoiseValue(const int x, const int y, const int z) const {
    return Marcher->NoiseGrid[x][y][z];
}

void Cube::CalculateCubeConfigIndex() {
    bool values[8] = {WBSGround, EBSGround, EBNGround, WBNGround, WTSGround, ETSGround, ETNGround, WTNGround};
    for (int i = 0; i < 8; i++) {
        if (values[i]) {
            CubeIndex |= 1 << i;
        }
    }
}

// If the value is below ground level, return true.
bool Cube::IsGround(float value) const {
    return value < Marcher->SurfaceLevel;
}

glm::vec3 Cube::InterpolateVerts(glm::vec3 p1, glm::vec3 p2, float valp1, float valp2)
{
    float iso = Marcher->SurfaceLevel;

    float t = (iso - valp1) / (valp2 - valp1);
    return p1 + t * (p2 - p1);
}
// Initialize all variables, get noise values, decide what is air and what is not.
void Cube::Init(int x, int y, int z) {
    // x y z should be the west bottom south corner.
    West = x; East = x + 1;
    Bottom = y; Top = y + 1;
    South = z; North = z + 1;
    WBSLevel = GetNoiseValue(West, Bottom, South); WBSGround = IsGround(WBSLevel);
    WBNLevel = GetNoiseValue(West, Bottom, North); WBNGround = IsGround(WBNLevel);
    EBSLevel = GetNoiseValue(East, Bottom, South); EBSGround = IsGround(EBSLevel);
    EBNLevel = GetNoiseValue(East, Bottom, North); EBNGround = IsGround(EBNLevel);
    WTSLevel = GetNoiseValue(West, Top, South); WTSGround = IsGround(WTSLevel);
    WTNLevel = GetNoiseValue(West, Top, North); WTNGround = IsGround(WTNLevel);
    ETSLevel = GetNoiseValue(East, Top, South); ETSGround = IsGround(ETSLevel);
    ETNLevel = GetNoiseValue(East, Top, North); ETNGround = IsGround(ETNLevel);
    CalculateCubeConfigIndex();
    vector<glm::vec3> corners = {
        glm::vec3(West, Bottom, South),  // 0
        glm::vec3(East, Bottom, South),  // 1
        glm::vec3(East, Bottom, North),  // 2
        glm::vec3(West, Bottom, North),  // 3
        glm::vec3(West, Top, South),     // 4
        glm::vec3(East, Top, South),     // 5
        glm::vec3(East, Top, North),     // 6
        glm::vec3(West, Top, North)      // 7
    };
    vector levels = {WBSLevel, EBSLevel, EBNLevel, WBNLevel, WTSLevel, ETSLevel, ETNLevel, WTNLevel};
    const int *triangulation = triTable[CubeIndex];
    for (int i = 0; triangulation[i] != -1; i +=3) {
        int edgeIndex = triangulation[i];
        if (edgeIndex == -1) {
            continue;
        }

        // Get indices of corner points A and B for each of the three edges
        // of the cube that need to be joined to form the triangle.
        int a0 = cornerIndexAFromEdge[triangulation[i]];
        int b0 = cornerIndexBFromEdge[triangulation[i]];

        int a1 = cornerIndexAFromEdge[triangulation[i+1]];
        int b1 = cornerIndexBFromEdge[triangulation[i+1]];

        int a2 = cornerIndexAFromEdge[triangulation[i+2]];
        int b2 = cornerIndexBFromEdge[triangulation[i+2]];

        glm::vec3 vertexA = InterpolateVerts(corners[a0], corners[b0], levels[a0], levels[b0]);
        glm::vec3 vertexB = InterpolateVerts(corners[a1], corners[b1], levels[a1], levels[b1]);
        glm::vec3 vertexC = InterpolateVerts(corners[a2], corners[b2], levels[a2], levels[b2]);

        Marcher->Stewart.push_back(vertexA.x);
        Marcher->Stewart.push_back(vertexA.y);
        Marcher->Stewart.push_back(vertexA.z);
        Marcher->Stewart.insert(Marcher->Stewart.end(), {1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f});
        Marcher->Stewart.push_back(vertexB.x);
        Marcher->Stewart.push_back(vertexB.y);
        Marcher->Stewart.push_back(vertexB.z);
        Marcher->Stewart.insert(Marcher->Stewart.end(), {1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f});
        Marcher->Stewart.push_back(vertexC.x);
        Marcher->Stewart.push_back(vertexC.y);
        Marcher->Stewart.push_back(vertexC.z);
        Marcher->Stewart.insert(Marcher->Stewart.end(), {1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f});
    }
}

void MarchingCubes::GenerateNoise()
{
    // Clear all the cubes from memory.
    NoiseGrid.clear();
    // Allocate and initialize all the cubes, generating their noise values, deciding what is air and what is not, ect.
    int noiseGridSize = GridSize + 1;
    NoiseGrid.resize(noiseGridSize);
    for (int x = 0; x < noiseGridSize; x++) {
        NoiseGrid[x].resize(noiseGridSize);
        for (int y = 0; y < noiseGridSize; y++) {
            NoiseGrid[x][y].resize(noiseGridSize);
            for (int z = 0; z < noiseGridSize; z++) {
                NoiseGrid[x][y][z] = layeredNoise3D(x, y, z, 5, 0.07, 2.0f, 100);
            }
        }
    }
}

vector<float> MarchingCubes::GenerateVertices()
{
    // Ready the vertex buffer.
    Stewart.clear();
    Stewart = {};
    // Make sure to generate the noise before we have the cubes try to access the noise values.
    GenerateNoise();
    // Clear all the cubes from memory.
    Cubes.clear();
    // Allocate and initialize all the cubes, generating their noise values, deciding what is air and what is not, ect.
    Cubes.resize(GridSize);
    for (int x = 0; x < GridSize; x++) {
        Cubes[x].resize(GridSize);
        for (int y = 0; y < GridSize; y++) {
            Cubes[x][y].resize(GridSize, this);
            for (int z = 0; z < GridSize; z++) {
                Cubes[x][y][z].Init(x, y, z);
            }
        }
    }

    for (int i = 0; i < Stewart.size() - 18; i += 27) {
        float x1 = Stewart[i]; float y1 = Stewart[i + 1]; float z1 = Stewart[i + 2];
        float x2 = Stewart[i + 9]; float y2 = Stewart[i + 10]; float z2 = Stewart[i + 11];
        float x3 = Stewart[i + 18]; float y3 = Stewart[i + 19]; float z3 = Stewart[i + 20];
        float nx, ny, nz;
        computeNormal(
            x3, y3, z3,
            x2, y2, z2,
            x1, y1, z1,
            nx, ny, nz
        );

        Stewart[i + 3] = nx; Stewart[i + 4] = ny; Stewart[i + 5] = nz;
        Stewart[i + 12] = nx; Stewart[i + 13] = ny; Stewart[i + 14] = nz;
        Stewart[i + 21] = nx; Stewart[i + 22] = ny; Stewart[i + 23] = nz;
    }
    return Stewart;
}