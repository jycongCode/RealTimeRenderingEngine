//
// Created by Lenovo on 2024/8/12.
//

#include "Texture.h"
#include <glad/glad.h>
#include <iostream>
#include <stb_image.h>
void Texture::LoadFromFile(const char *filePath, bool sRGB) {
    unsigned int id;
    int width,height,nChannels;
    glGenTextures(1,&id);
    glBindTexture(GL_TEXTURE_2D,id);
    unsigned char* data = stbi_load(filePath,&width,&height,&nChannels,0);
    if(data){
        GLenum dataFormat;
        GLenum internalFormat;
        switch (nChannels) {
            case(1):
                internalFormat = GL_RED;
                dataFormat = GL_RED;
            break;
            case(3):
                internalFormat = sRGB ? GL_SRGB : GL_RGB;
                dataFormat = GL_RGB;
            break;
            case(4):
                internalFormat = sRGB ? GL_SRGB_ALPHA : GL_RGBA;
                dataFormat = GL_RGBA;
            break;
        }
        glBindTexture(GL_TEXTURE_2D,id);
        glTexImage2D(GL_TEXTURE_2D,0,internalFormat,width,height,0,dataFormat,GL_UNSIGNED_BYTE,data);
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        stbi_image_free(data);
    }else{
        std::cout << "ERROR::ASSIMP::LOAD_TEXTURE_FAILURE" << std::endl;
        stbi_image_free(data);
    }
    this->id = id;
}
