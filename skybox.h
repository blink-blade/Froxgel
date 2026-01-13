#ifndef FROXGEL_SKYBOX_H
#define FROXGEL_SKYBOX_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

#include "mesh.h"
#include "lighting.h"

extern std::vector<float> skyboxVertices;
class Skybox : public Mesh {
    public:
    using Mesh::Mesh;

    void draw() const {
        glm::mat4 rotationMat(1);
        rotationMat = glm::rotate(rotationMat, 0.001f, glm::vec3(0.0, 0.0, 1.0));
        sunDir = glm::vec3(rotationMat * glm::vec4(sunDir, 1.0));;
        Mesh::draw();

    }
};


#endif //FROXGEL_SKYBOX_H