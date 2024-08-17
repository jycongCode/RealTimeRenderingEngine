//
// Created by Lenovo on 2024/8/17.
//
#include "Drawable.h"

glm::mat4 Drawable::GetModelMatrix() const {
    glm::mat4 T(1.0f);
    T = glm::translate(T,Position);

    glm::mat4 S(1.0f);
    S = glm::scale(S,Scale);

    glm::mat4 R = glm::mat4_cast(EulerToQuat(Rotation));

    return T * S * R;
}

glm::quat Drawable::EulerToQuat(glm::vec3 euler) {
    euler = glm::radians(euler);
    float sx = glm::sin(euler.x/2);
    float sy = glm::sin(euler.y/2);
    float sz = glm::sin(euler.z/2);
    float cx = glm::cos(euler.x/2);
    float cy = glm::cos(euler.y/2);
    float cz = glm::cos(euler.z/2);

    return{ cx*cy*cz - sx*sy*sz,
   sx*cy*cz + cx*sy*sz,
   cx*sy*cz - sx*cy*sz,
   cx*cy*sz + sx*sy*cz };
}
