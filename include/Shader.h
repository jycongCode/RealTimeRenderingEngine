//
// Created by Lenovo on 2024/8/12.
//

#pragma once
#include <glad/glad.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
class Shader {
public:
    unsigned int ID;
    Shader(const char *vertexPath, const char *fragmentPath);

    void Destroy(){
        glDeleteProgram(ID);
    }
    void use() {
        glUseProgram(this->ID);
    }
    void setBool(const std::string &name, bool value) const {
        int uniformLocation = glad_glGetUniformLocation(this->ID,name.c_str());
        glUniform1i(uniformLocation,(int)value);
    }

    void setFloat(const std::string &name, float value) const {
        int uniformLocation = glad_glGetUniformLocation(this->ID,name.c_str());
        glUniform1f(uniformLocation,value);
    }

    void setInt(const std::string &name, int value) const {
        int uniformLocation = glad_glGetUniformLocation(this->ID,name.c_str());
        glUniform1i(uniformLocation,value);
    }

    void setMat4(const std::string &name, glm::mat4 value) const {
        int uniformLocation = glad_glGetUniformLocation(this->ID,name.c_str());
        glUniformMatrix4fv(uniformLocation,1,GL_FALSE,glm::value_ptr(value));
    }

    void setVec3(const std::string &name, float x,float y,float z) const {
        int uniformLocation = glad_glGetUniformLocation(this->ID,name.c_str());
        glUniform3f(uniformLocation,x,y,z);
    }

    void setVec3(const std::string &name,glm::vec3 value) const{
        int uniformLocation = glad_glGetUniformLocation(this->ID,name.c_str());
        glUniform3fv(uniformLocation,1,glm::value_ptr(value));
    }

    void setVec2(const std::string &name,glm::vec2 value) const{
        int uniformLocation = glad_glGetUniformLocation(this->ID,name.c_str());
        glUniform2fv(uniformLocation,1,glm::value_ptr(value));
    }
private:

    void checkProgramLinkStatus(const char* programName,GLuint program){
        int success;
        char infoLog[512];
        glGetProgramiv(program,GL_LINK_STATUS,&success);

        if(!success){
            glGetProgramInfoLog(program,512,NULL,infoLog);
            std::cout << "ERROR::PROGRAM[" << programName << "]::LINK_FAILED\n" << infoLog << std::endl;
        }
    }
    void checkShaderCompileStatus(const char* shaderName,GLuint shader){
        int success;
        char infoLog[512];
        glGetShaderiv(shader,GL_COMPILE_STATUS,&success);
        if(!success){
            glGetShaderInfoLog(shader,512,NULL,infoLog);
            std::cout << "ERROR::SHADER["<< shaderName <<"]::COMPILATION_FAILED\n" << infoLog << std::endl;
        }
    }
};
