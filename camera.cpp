#include "camera.h"

// Default values
float cameraSpeed = 6.0f;
static constexpr float DEFAULT_SENSITIVITY = 0.1f;
static constexpr float DEFAULT_FOV = 90.0f;
float nearPlane = 0.1f;
float farPlane = 1000000.0f;

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch, bool isPlayer)
    : Front(glm::vec3(0.0f, 0.0f, -1.0f)),
      MovementSpeed(cameraSpeed),
      MouseSensitivity(DEFAULT_SENSITIVITY),
      Fov(DEFAULT_FOV),
      IsPlayer(false)
{
    Position = position;
    WorldUp = up;
    Yaw = yaw;
    Pitch = pitch;
    Pitch = isPlayer;

    UpdateCameraVectors();
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

void Camera::ProcessMouseMovement(float xOffset, float yOffset, bool constrainPitch)
{
    if (IsPlayer) {
        xOffset *= MouseSensitivity;
        yOffset *= MouseSensitivity;

        Yaw   += xOffset;
        Pitch += yOffset;

        if (constrainPitch)
        {
            if (Pitch > 89.0f)
                Pitch = 89.0f;
            if (Pitch < -89.0f)
                Pitch = -89.0f;
        }
    }

    UpdateCameraVectors();
}

void Camera::ProcessMouseScroll(float yOffset)
{
    Fov -= yOffset;
    if (Fov < 30.0f)
        Fov = 30.0f;
    if (Fov > 90.0f)
        Fov = 90.0f;
}

void Camera::LookAt(const glm::vec3& target, bool constrainPitch)
{
    glm::vec3 direction = glm::normalize(target - Position);

    // Calculate yaw and pitch from direction vector
    float pitch = glm::degrees(asin(direction.y));
    float yaw   = glm::degrees(atan2(direction.z, direction.x));

    // OpenGL forward is -Z, so offset yaw
    yaw -= 90.0f;

    Pitch = pitch;
    Yaw   = yaw;

    if (constrainPitch)
    {
        if (Pitch > 89.0f)  Pitch = 89.0f;
        if (Pitch < -89.0f) Pitch = -89.0f;
    }

    UpdateCameraVectors();
}

void Camera::UpdateCameraVectors()
{
    glm::vec3 front;
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Front = glm::normalize(front);
    Right = glm::normalize(glm::cross(Front, WorldUp));
    Up    = glm::normalize(glm::cross(Right, Front));
}

Camera camera = Camera();
glm::mat4 projection = glm::perspective(glm::radians(camera.Fov), 800.0f / 600.0f, nearPlane, farPlane);
Camera sunCamera = Camera();
glm::mat4 sunProjection = glm::perspective(glm::radians(sunCamera.Fov), 800.0f / 600.0f, nearPlane, farPlane);
