//
// Created by Lenovo on 2024/8/25.
//

#ifndef MATHUTILS_H
#define MATHUTILS_H

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
namespace MATHUTILS{
    inline glm::quat EulerToQuat(glm::vec3 euler) {
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

    inline glm::vec3 EulerToVec3(glm::vec3 euler) {
        glm::quat q = EulerToQuat(euler);
        glm::mat3 R = glm::mat3(glm::mat4_cast(q));
        return glm::normalize(R * glm::vec3(0.01,0.01,1.0));
    }
}

#endif //MATHUTILS_H
