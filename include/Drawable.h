//
// Created by Lenovo on 2024/8/17.
//

#ifndef DRAWABLE_H
#define DRAWABLE_H
#include "Shader.h"

class Drawable{
public:
    virtual ~Drawable() = default;
    virtual void draw(Shader shader){}
    virtual void destroy(){}
    glm::vec3 Position = glm::vec3(0.0f);
    glm::vec3 Rotation = glm::vec3(0.0f);
    glm::vec3 Scale = glm::vec3(1.0f);
    glm::mat4 GetModelMatrix() const;
public:
    static glm::quat EulerToQuat(glm::vec3 euler);
};
#endif //DRAWABLE_H
