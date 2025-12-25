#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

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
    // Camera attributes
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;

    // Euler angles
    float Yaw;
    float Pitch;

    // Options
    float MovementSpeed;
    float MouseSensitivity;
    float Fov;

    // Constructor
    Camera(
        glm::vec3 position = glm::vec3(0.0f, 0.1f, 3.0f),
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
        float yaw = -90.0f,
        float pitch = 0.0f
    );

    // Returns the view matrix
    glm::mat4 GetViewMatrix() const;

    // Input handling
    void ProcessKeyboard(CameraMovement direction, float deltaTime);
    void ProcessMouseMovement(float xOffset, float yOffset, bool constrainPitch = true);
    void ProcessMouseScroll(float yOffset);

private:
    void UpdateCameraVectors();
};

extern Camera camera;
extern glm::mat4 projection;
extern float nearPlane;
extern float farPlane;