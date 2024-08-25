//
// Created by Lenovo on 2024/8/17.
//

#ifndef QUAD_H
#define QUAD_H
#include <glad/glad.h>
#include "Drawable.h"
class Quad :public Drawable{
public:
    Quad();
    Quad(glm::vec3 position,glm::vec3 rotation,glm::vec3 scale);
    void draw(Shader *shader) override;
    void destroy() override;
private:
    GLuint VAO = 0,VBO = 0;

};



#endif //QUAD_H
