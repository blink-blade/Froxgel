#include "lighting.h"

#include <iostream>
#include <glm/vec3.hpp>
#include <glm/ext/quaternion_geometric.hpp>

#include "camera.h"
#include "helpers.h"

glm::vec3 sunDir = glm::normalize(glm::vec3(0.0, -1.0, 0));
void lightUpdates() {
    // std::cout << sunCamera.Position.x << ", " << sunCamera.Position.y << ", " << sunCamera.Position.z << std::endl;
    // cout << sunCamera.Front.x << " " << sunCamera.Front.y << " " << sunCamera.Front.z << endl;
    glm::mat4 rotationMat(1);
    rotationMat = glm::rotate(rotationMat, 0.001f, glm::vec3(0.0, 0.0, 1.0));
    sunDir = glm::vec3(rotationMat * glm::vec4(sunDir, 1.0));
    // cout << sunDir.x << " " << sunDir.y << " " << sunDir.z << endl;
    sunCamera.Position = -glm::vec3(sunDir.x * 100, sunDir.y * 100, sunDir.z * 100);
    sunCamera.LookAt(glm::vec3(0.1f));
}
