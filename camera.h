#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <glm/detail/type_quat.hpp>

using namespace std;
enum CameraMovement
{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

class Camera
{
public:
    glm::vec3 Position;

    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;

    // Euler angles (still used for mouse input)
    float Yaw;
    float Pitch;

    // NEW: orientation
    glm::quat Orientation;

    float MovementSpeed;
    float MouseSensitivity;
    float Fov;
    float IsPlayer;

    Camera(
        glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f),
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
        float yaw = -90.0f,
        float pitch = 0.0f,
        bool isPlayer = false
    );

    glm::mat4 GetViewMatrix() const;

    void ProcessKeyboard(CameraMovement direction, float deltaTime);
    void ProcessMouseMovement(float xOffset, float yOffset);

    void LookAt(const glm::vec3& target);

private:
    void UpdateVectorsFromQuaternion();
    void UpdateQuaternionFromEuler();
};

extern float cameraSpeed;
extern Camera camera;
extern glm::mat4 projection;
extern Camera sunCamera;
extern glm::mat4 sunProjection;
extern float nearPlane;
extern float farPlane;