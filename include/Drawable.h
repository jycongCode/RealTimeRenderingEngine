//
// Created by Lenovo on 2024/8/17.
//

#ifndef DRAWABLE_H
#define DRAWABLE_H
#include "Material.h"
class Drawable{
public:
    Drawable(glm::vec3 position,glm::vec3 rotation,glm::vec3 scale):
        Position(position),Rotation(rotation),Scale(scale){};
    Drawable() = default;
    virtual ~Drawable() = default;
    virtual void draw(Shader shader){}
    virtual void destroy(){}
    glm::mat4 GetModelMatrix() const;
public:
    std::string ID;
    Material* Mat;
    glm::vec3 Position = glm::vec3(0.0f);
    glm::vec3 Rotation = glm::vec3(0.0f);
    glm::vec3 Scale = glm::vec3(1.0f);
public:
    static glm::quat EulerToQuat(glm::vec3 euler);
};
#endif //DRAWABLE_H
