//
// Created by Lenovo on 2024/8/12.
//

#include "Shader.h"
#include <string>
#include <fstream>
#include <sstream>
#include <glad/glad.h>
#include <iostream>

Shader::Shader(const char *vertexPath, const char *fragmentPath) {
    std::string vertexCode;
    std::string fragmentCode;

    std::ifstream vertexFile;
    std::ifstream fragmentFile;

    vertexFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fragmentFile.exceptions(std::ifstream ::failbit|std::ifstream ::badbit);

    try{
        vertexFile.open(vertexPath);
        std::stringstream vertexStream;
        vertexStream << vertexFile.rdbuf();
        vertexFile.close();
        vertexCode = vertexStream.str();
    }catch(std::ifstream::failure e){
        std::cout << "ERROR::SHADER[" << vertexPath <<"]::FILE NOT SUCCESSFULLY READ" << std::endl;
    }

    try {
        fragmentFile.open(fragmentPath);
        std::stringstream fragmentStream;
        fragmentStream << fragmentFile.rdbuf();
        fragmentFile.close();
        fragmentCode = fragmentStream.str();
    }catch(std::ifstream::failure e){
        std::cout << "ERROR::SHADER[" << fragmentPath <<"]::FILE NOT SUCCESSFULLY READ" << std::endl;
    }

    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    unsigned int vertexShader;
    unsigned int fragmentShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader,1,&vShaderCode,NULL);
    glCompileShader(vertexShader);
    checkShaderCompileStatus(vertexPath,vertexShader);

    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader,1,&fShaderCode,NULL);
    glCompileShader(fragmentShader);
    checkShaderCompileStatus(fragmentPath,fragmentShader);

    this->ID = glCreateProgram();
    glAttachShader(this->ID,vertexShader);
    glAttachShader(this->ID,fragmentShader);

    glLinkProgram(this->ID);
    std::string programName(vertexPath);
    int pos = programName.find_last_of('/')+1;
    int n = programName.find_last_of('.') - pos;
    programName = programName.substr(pos,n);
    checkProgramLinkStatus(programName.c_str(),this->ID);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}
