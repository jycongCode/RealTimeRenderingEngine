//
// Created by Lenovo on 2024/8/17.
//
#include "Drawable.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include "MathUtils.h"
glm::mat4 Drawable::GetModelMatrix() const {
    glm::mat4 T(1.0f);
    T = glm::translate(T,Position);

    glm::mat4 S(1.0f);
    S = glm::scale(S,Scale);

    glm::mat4 R = glm::mat4_cast(MATHUTILS::EulerToQuat(Rotation));

    return T * S * R;
}

