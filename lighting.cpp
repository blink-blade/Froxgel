#include "lighting.h"

#include <iostream>
#include <glm/vec3.hpp>
#include <glm/ext/quaternion_geometric.hpp>

#include "camera.h"
#include "helpers.h"

glm::vec3 sunDir = glm::normalize(glm::vec3(0.0f, -1.0f, 0.0f));
void lightUpdates() {
    // std::cout << sunCamera.Position.x << ", " << sunCamera.Position.y << ", " << sunCamera.Position.z << std::endl;
    // cout << sunCamera.Front.x << " " << sunCamera.Front.y << " " << sunCamera.Front.z << endl;
    sunCamera.Position = -glm::vec3(sunDir.x * 10000, sunDir.y * 10000, sunDir.z * 10000);
    sunCamera.LookAt(glm::vec3(0.1f));
}
