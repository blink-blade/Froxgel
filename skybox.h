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

    void draw(Camera cam, glm::mat4 proj) const {
        Mesh::draw(cam, proj);
    }
};


#endif //FROXGEL_SKYBOX_H