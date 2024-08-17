//
// Created by Lenovo on 2024/8/12.
//

#include <glad/glad.h>
#include "Texture.h"
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
void Texture::CreateFromColor(glm::vec3 color,int size) {
    // Create id for texture
    unsigned int tex;
    // generate and bind texture
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    // set texture wrap parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filter parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // set image data
    unsigned char* data = new unsigned char[3 * size * size * sizeof(unsigned char)];
    for (unsigned int i = 0; i < size  * size; i++)
    {
        data[i * 3] = (unsigned char)(color.x * 255.0f);
        data[i * 3 + 1] = (unsigned char)(color.y * 255.0f);
        data[i * 3 + 2] = (unsigned char)(color.z * 255.0f);
    }
    // set texture data and generate mipmaps
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, size, size, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    // free image memory
    delete[] data;
    id = tex;
}