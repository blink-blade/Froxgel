#include "marching_cubes.h"

#include <glm/ext/quaternion_trigonometric.hpp>
#include <glm/gtc/quaternion.hpp>

#include "polygons.h"

MarchingCubes::MarchingCubes()
{

}

vector<float> MarchingCubes::GenerateVertices()
{

    return generateSphere(5, 100, 100);;
}