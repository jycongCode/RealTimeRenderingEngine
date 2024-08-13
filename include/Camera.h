//
// Created by Lenovo on 2024/8/13.
//

#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    LIFT,
    DROP
};
const float INITIAL_YAW         = -90.0f;
const float INITIAL_PITCH       =  0.0f;
const float INITIAL_SPEED       =  2.5f;
const float INITIAL_SENSITIVITY =  0.1f;
const float INITIAL_ZOOM        =  45.0f;
class Camera {
public:
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    // euler Angles
    float Yaw;
    float Pitch;
    // camera options
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;
    float nearPlane = 0.1f;
    float farPlane = 100.0f;

public:
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
            glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f),
            float yaw = INITIAL_YAW, float pitch = INITIAL_PITCH) :
            MovementSpeed(INITIAL_SPEED), MouseSensitivity(INITIAL_SENSITIVITY), Zoom(INITIAL_ZOOM)
    {
        Position = position;
        WorldUp = up;
        Front = front;
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }

public:
    glm::mat4 GetViewMatrix();
    void ProcessKeyboard(Camera_Movement direction, float deltaTime);
    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);
    void ProcessMouseScroll(float yoffset);
private:
    void updateCameraVectors();
};
