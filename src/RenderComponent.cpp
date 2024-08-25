//
// Created by Lenovo on 2024/8/14.
//
#include <glad/glad.h>
#include "RenderComponent.h"
#include "Shader.h"
#include <dirent.h>
#include "DisplayComponent.h"
#include "Material.h"

const char* shaderDir = "../resources/shaders";
const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

void RenderComponent::SetUp(RTREngine* engine) {
     Component::SetUp(engine);
     glGenBuffers(1,&MatricesUBO);
     glBindBuffer(GL_UNIFORM_BUFFER,MatricesUBO);
     glBufferData(GL_UNIFORM_BUFFER,128,nullptr,GL_STATIC_DRAW);
     glBindBufferBase(GL_UNIFORM_BUFFER,0,MatricesUBO);

     glGenBuffers(1,&LightUBO);
     glBindBuffer(GL_UNIFORM_BUFFER,LightUBO);
     glBufferData(GL_UNIFORM_BUFFER,2*4*4,nullptr,GL_STATIC_DRAW);
     glBindBufferBase(GL_UNIFORM_BUFFER,2,LightUBO);

     glGenBuffers(1,&CameraUBO);
     glBindBuffer(GL_UNIFORM_BUFFER,CameraUBO);
     glBufferData(GL_UNIFORM_BUFFER,1*(4*sizeof(float)),nullptr,GL_STATIC_DRAW);
     glBindBufferBase(GL_UNIFORM_BUFFER,1,CameraUBO);

     glGenFramebuffers(1,&ShadowMapFBO);

     glGenTextures(1, &ShadowMap);
     glBindTexture(GL_TEXTURE_2D, ShadowMap);
     glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
                  SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

     glBindFramebuffer(GL_FRAMEBUFFER, ShadowMapFBO);
     glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, ShadowMap, 0);
     glDrawBuffer(GL_NONE);
     glReadBuffer(GL_NONE);
     glBindFramebuffer(GL_FRAMEBUFFER, 0);
     LoadAndCompileShaders(shaderDir);
}

void RenderComponent::SetUpUBOData() {
     const auto* sceneComponent = engine->sceneComponent;
     glBindBuffer(GL_UNIFORM_BUFFER,MatricesUBO);
     glm::mat4 projection = sceneComponent->Current->GetProjectionMatrix();
     glBufferSubData(GL_UNIFORM_BUFFER,0,sizeof(glm::mat4),glm::value_ptr(projection));
     glm::mat4 view = sceneComponent->Current->GetViewMatrix();
     glBufferSubData(GL_UNIFORM_BUFFER,sizeof(glm::mat4),sizeof(glm::mat4),glm::value_ptr(view));
     glBindBuffer(GL_UNIFORM_BUFFER,0);

     glBindBuffer(GL_UNIFORM_BUFFER,LightUBO);
     glBufferSubData(GL_UNIFORM_BUFFER,0,sizeof(glm::vec3),glm::value_ptr(sceneComponent->Current->Sunlight.GetDirVec3()));
     glBufferSubData(GL_UNIFORM_BUFFER,16,sizeof(glm::vec3),glm::value_ptr(sceneComponent->Current->Sunlight.lightColor));
     glBufferSubData(GL_UNIFORM_BUFFER,28,sizeof(float),&(sceneComponent->Current->Sunlight.intensity));
     glBindBuffer(GL_UNIFORM_BUFFER,0);

     glBindBuffer(GL_UNIFORM_BUFFER,CameraUBO);
     glBufferSubData(GL_UNIFORM_BUFFER,0,sizeof(glm::vec3),glm::value_ptr(sceneComponent->Current->MainCamera.Position));
     glBindBuffer(GL_UNIFORM_BUFFER,0);
}

void RenderComponent::GenerateShadowMap() {
     glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
     glBindFramebuffer(GL_FRAMEBUFFER,ShadowMapFBO);
     glClear(GL_DEPTH_BUFFER_BIT);
     Shader* shader = shaderLib["ShadowMap"];
     RenderScene();
     glBindFramebuffer(GL_FRAMEBUFFER, 0);
     // 2. then render scene as normal with shadow mapping (using depth map)
     glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
     ConfigureShaderAndMatrices();
     glBindTexture(GL_TEXTURE_2D, depthMap);
     RenderScene();
}

void RenderComponent::Update(float deltaTime) {
     // UBO set up, projection+view matrices, lights, camera
     SetUpUBOData();
     glViewport(0,0,engine->displayComponent->ScrWidth,engine->displayComponent->ScrHeight);
     //TODO: draw skybox
     glEnable(GL_DEPTH_TEST);
     //draw loop
     for(const auto drawable : sceneComponent->Current->DrawableList) {
          std::string shaderName = drawable->Mat->shaderName;
          Shader* shader = shaderLib[shaderName];
          shader->use();
          shader->setMat4("model",drawable->GetModelMatrix());
          drawable->Mat->Render(shader);
          drawable->draw(shader);
     }
}

void RenderComponent::Destroy() {
     glDeleteBuffers(1,&MatricesUBO);
     glDeleteBuffers(1,&LightUBO);
     glDeleteBuffers(1,&CameraUBO);
     MatricesUBO = 0,LightUBO = 0,CameraUBO = 0;
}

void RenderComponent::LoadAndCompileShaders(const char *directoryPath) {
     std::string prefix = directoryPath;
     DIR* dir;
     struct dirent* entry;
     dir = opendir(directoryPath);
     if (dir == nullptr) {
          std::cout << "Cannot open directory [" << directoryPath << "]" << std::endl;
          return;
     }
     while ((entry = readdir(dir)) != nullptr) {
          if (std::string(entry->d_name) == "." || std::string(entry->d_name) == "..") {
               continue;
          }
          std::string fileName = entry->d_name;
          std::string shaderName = fileName.substr(0,fileName.find_last_of('.'));
          if(shaderLib.count(shaderName)==0) {
               auto vertex = prefix+"/"+shaderName+".vert";
               auto frag = prefix+"/"+shaderName+".frag";
               shaderLib[shaderName] = new Shader(vertex.c_str(),frag.c_str());
               std::cout << shaderName << " Compiled" << std::endl;
          }
     }
}
