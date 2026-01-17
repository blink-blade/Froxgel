#include "camera.h"

#include <iostream>
#include <glm/ext/quaternion_trigonometric.hpp>
#include <glm/gtc/quaternion.hpp>

// Default values
float cameraSpeed = 6.0f;
static constexpr float DEFAULT_SENSITIVITY = 0.1f;
static constexpr float DEFAULT_FOV = 90.0f;
float nearPlane = 0.1f;
float farPlane = 1000000.0f;

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch, bool isPlayer)
    : Position(position),
      WorldUp(up),
      Yaw(yaw),
      Pitch(pitch),
      MovementSpeed(6.0f),
      MouseSensitivity(0.07f),
      Fov(70.0f),
      IsPlayer(false)
{
    IsPlayer = isPlayer;
    UpdateQuaternionFromEuler();
    UpdateVectorsFromQuaternion();
}
glm::mat4 Camera::GetViewMatrix() const
{
    return glm::lookAt(Position, Position + Front, Up);
}

void Camera::ProcessKeyboard(CameraMovement direction, float deltaTime)
{
    if (!IsPlayer) {return;}
    float velocity = MovementSpeed * deltaTime;

    if (direction == FORWARD)
        Position += Front * velocity;
    if (direction == BACKWARD)
        Position -= Front * velocity;
    if (direction == LEFT)
        Position -= Right * velocity;
    if (direction == RIGHT)
        Position += Right * velocity;
    if (direction == UP)
        Position += WorldUp * velocity;
    if (direction == DOWN)
        Position -= WorldUp * velocity;
}

void Camera::ProcessMouseMovement(float xOffset, float yOffset)
{
    xOffset *= MouseSensitivity;
    yOffset *= MouseSensitivity;

    Yaw   -= xOffset;
    Pitch += yOffset;
    Pitch = glm::clamp(Pitch, -89.0f, 89.0f);

    UpdateQuaternionFromEuler();
    UpdateVectorsFromQuaternion();
}

void Camera::LookAt(const glm::vec3& target)
{
    glm::vec3 forward = glm::normalize(target - Position);

    // Handle degenerate case (camera exactly at target)
    if (glm::dot(forward, forward) < 1e-6f)
        return;

    // Build quaternion from direction
    Orientation = glm::quatLookAtRH(forward, WorldUp);

    // Update direction vectors
    UpdateVectorsFromQuaternion();

    // OPTIONAL: extract yaw/pitch back out (for mouse continuity)
    Pitch = glm::degrees(asin(Front.y));
    Yaw   = glm::degrees(atan2(Front.x, -Front.z));
}

void Camera::UpdateQuaternionFromEuler()
{
    glm::quat yawQ   = glm::angleAxis(glm::radians(Yaw),   WorldUp);
    glm::quat pitchQ = glm::angleAxis(glm::radians(Pitch), glm::vec3(1, 0, 0));

    Orientation = yawQ * pitchQ;
    Orientation = glm::normalize(Orientation);
}

void Camera::UpdateVectorsFromQuaternion()
{
    Front = Orientation * glm::vec3(0, 0, -1);
    Right = glm::normalize(glm::cross(Front, WorldUp));
    Up    = glm::normalize(glm::cross(Right, Front));
}

Camera camera = Camera();
glm::mat4 projection = glm::perspective(glm::radians(camera.Fov), 800.0f / 600.0f, nearPlane, farPlane);
Camera sunCamera = Camera();
glm::mat4 sunProjection = glm::perspective(glm::radians(sunCamera.Fov), 800.0f / 600.0f, nearPlane, farPlane);
