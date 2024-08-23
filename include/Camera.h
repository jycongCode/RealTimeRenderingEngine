//
// Created by Lenovo on 2024/8/13.
//

#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    LIFT,
    DROP
};
const float INITIAL_YAW         = -118.0f;
const float INITIAL_PITCH       =  -22.0f;
const float INITIAL_SPEED       =  2.5f;
const float INITIAL_SENSITIVITY =  0.1f;
const float INITIAL_ZOOM        =  45.0f;
class Camera {
public:
    std::string ID;
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
    float aspect;
public:
    Camera(glm::vec3 position = glm::vec3(1.9f, 3.3f, 2.5f),
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
    glm::mat4 GetProjectionMatrix();
    void ProcessKeyboard(Camera_Movement direction, float deltaTime);
    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);
    void ProcessMouseScroll(float yoffset);
private:
    void updateCameraVectors();
};
