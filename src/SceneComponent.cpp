//
// Created by Lenovo on 2024/8/14.
//

#include "SceneComponent.h"
#include <fstream>
#include <string>
#include "InputComponent.h"
#include "Quad.h"
using json = nlohmann::json;
using namespace nlohmann::literals;
glm::vec3 EulerToVector(glm::vec3 rotation) {
    glm::quat quat = Drawable::EulerToQuat(rotation);
    glm::mat4 rotMat = glm::mat4_cast(quat);
    return glm::vec3(rotMat * glm::vec4(0.0,0.0,1.0,0.0));
}

void SceneComponent::draw(const char *modelName, Shader shader) {
    if(modelMap.count(modelName) == 0) {
        std::cout << "SCENENCOMPONENT::ERROR[" << modelName << "]::Not in Scene" << std::endl;
        return;
    }
    Drawable* model = modelMap[modelName];
    shader.use();
    glm::mat4 view = camera.GetViewMatrix();
    glm::mat4 projection = glm::perspective(camera.Zoom,aspect,camera.nearPlane,camera.farPlane);
    glm::mat4 MVP = projection * view * model->GetModelMatrix();
    shader.setMat4("MVP",MVP);
    shader.setMat4("M",model->GetModelMatrix());
    shader.setVec3("cameraPos_wS",camera.Position);
    shader.setFloat("envLight",envLight);
    unsigned int dirId = 0;
    unsigned int pointId = 0;
    for(auto light:lights) {
        if(light->type == LightType::Directional) {
            auto* dirLight = (DirLight*)light;
            shader.setVec3("dirLights[" + std::to_string(dirId) + "].color",dirLight->lightColor);
            shader.setFloat("dirLights[" + std::to_string(dirId) + "].intensity",dirLight->intensity);
            shader.setVec3("dirLight_wS[" + std::to_string(dirId) + "]",EulerToVector(dirLight->direction));
            dirId++;
        }else if(light->type == LightType::Point) {
            auto* pointLight = (PointLight*)light;
            shader.setVec3("pointLights[" + std::to_string(pointId) + "].color",pointLight->lightColor);
            shader.setFloat("pointLights[" + std::to_string(pointId) + "].intensity",pointLight->intensity);
            shader.setVec3("pointLight_wS[" + std::to_string(pointId) + "]",pointLight->position);
            shader.setFloat("pointLights[" + std::to_string(pointId) + "].radius",pointLight->radius);
            pointId++;
        }
    }
    shader.setInt("dirNum",dirId);
    shader.setInt("pointNum",pointId);
    model->draw(shader);
}

void SceneComponent::addModel(const char *modelName, const char *modelPath,glm::vec3 position,glm::vec3 rotation,glm::vec3 scale) {
    auto * model = new Model(modelPath);
    if(modelMap.count(modelName) > 0) {
        std::cout << "SCENENCOMPONENT::ERROR[" << modelName << "] already exists" << std::endl;
        return;
    }
    if(!model->meshes.empty()) {
        model->Position = position;
        model->Rotation = rotation;
        model->Scale = scale;
        modelMap.insert({modelName,model});
    }
}

void SceneComponent::addDrawable(const char *name, Drawable *drawable) {
    if(modelMap.count(name) > 0) {
        std::cout << "SCENENCOMPONENT::ERROR[" << name << "] already exists" << std::endl;
        return;
    }
    modelMap.insert({name,drawable});
}

void SceneComponent::LoadScene(const char *filePath) {
    std::ifstream f(filePath);
    json data = json::parse(f);
    sceneID = data["ID"];
    sceneFile = filePath;
    std::string ID,path,type;
    glm::vec3 position,rotation,scale;
    // Load model
    auto modelList = data["Models"];
    for (auto model : modelList) {
        if(model["type"] == "model") {
            ID = model["ID"];
            path = model["path"];
            position = {model["position"][0],model["position"][1],model["position"][2]};
            rotation = {model["rotation"][0],model["rotation"][1],model["rotation"][2]};
            scale = {model["scale"][0],model["scale"][1],model["scale"][2]};
            addModel(ID.c_str(),path.c_str(),position,rotation,scale);
        }else if(model["type"] == "Quad") {
            ID = model["ID"];
            position = {model["position"][0],model["position"][1],model["position"][2]};
            rotation = {model["rotation"][0],model["rotation"][1],model["rotation"][2]};
            scale = {model["scale"][0],model["scale"][1],model["scale"][2]};
            Quad* quad = new Quad(position,rotation,scale);
            glm::vec3 color = {1.0f,1.0f,1.0f};
            Texture tex;
            tex.CreateFromColor(color,32.0f);
            quad->texture = tex.id;
            addDrawable(ID.c_str(),quad);
        }
    }
    // Load camera
    auto cameraData = data["Camera"];
    position = {cameraData["position"][0],cameraData["position"][1],cameraData["position"][2]};
    camera.Position = position;
    camera.Pitch = cameraData["pitch"];
    camera.Yaw = cameraData["yaw"];
    camera.Zoom = cameraData["zoom"];
    camera.MouseSensitivity = cameraData["sensitivity"];
    camera.MovementSpeed = cameraData["speed"];
    camera.ID = cameraData["ID"];

    // Load Lights
    auto lightList = data["Light"];
    for(auto light:lightList) {
        if(light["type"] == "directional") {
            std::string id = light["ID"];
            glm::vec3 dir = {light["direction"][0],light["direction"][1],light["direction"][2]};
            glm::vec3 color = {light["color"][0],light["color"][1],light["color"][2]};
            float intensity = light["intensity"];
            auto* light = new DirLight(id,dir,color,intensity);
            lights.push_back(light);
        }else if(light["type"] == "point") {
            std::string id = light["ID"];
            glm::vec3 pos = {light["position"][0],light["position"][1],light["position"][2]};
            glm::vec3 color =  {light["color"][0],light["color"][1],light["color"][2]};
            float intensity = light["intensity"];
            float radius = light["radius"];
            auto* light = new PointLight(id,pos,radius,color,intensity);
            lights.push_back(light);
        }
    }

    envLight = data["Environment"]["Irradiance"];
    std::cout << "Scene Loaded:" << data["ID"] << std::endl;
    std::cout << "From path:" << filePath << std::endl;
}

void SceneComponent::SaveScene(const char *filePath) {
    json data;
    data["ID"] = sceneID;
    auto modelData = json::array();
    for(auto [key,drawable] : modelMap) {
        if(Model* ptr = dynamic_cast<Model*>(drawable)) {
            json model = {
                {"ID",key.c_str()},
                {"path",ptr->filePath.c_str()},
                {"type","model"},
                {"position",json::array({ptr->Position[0],ptr->Position[1],ptr->Position[2]})},
                {"rotation",json::array({ptr->Rotation[0],ptr->Rotation[1],ptr->Rotation[2]})},
                  {"scale",json::array({ptr->Scale[0],ptr->Scale[1],ptr->Scale[2]})}            };
            modelData.push_back(model);
        }else if(Quad* ptr = dynamic_cast<Quad*>(drawable)) {
            json quad = {
                {"ID",key.c_str()},
                {"type","Quad"},
                {"position",json::array({ptr->Position[0],ptr->Position[1],ptr->Position[2]})},
                {"rotation",json::array({ptr->Rotation[0],ptr->Rotation[1],ptr->Rotation[2]})},
                  {"scale",json::array({ptr->Scale[0],ptr->Scale[1],ptr->Scale[2]})},
            };
            modelData.push_back(quad);
        }
    }
    data["Models"] = modelData;
    json camData = {
        {"ID",camera.ID},
        {"position",json::array({camera.Position[0],camera.Position[1],camera.Position[2]})},
        {"pitch",camera.Pitch},
          {"yaw",camera.Yaw},
        {"zoom",camera.Zoom},
        {"sensitivity",camera.MouseSensitivity},
        {"speed",camera.MovementSpeed}
    };
    data["Camera"] = camData;
    json lightData = json::array();
    for(auto light : lights) {
        if(DirLight* ptr = dynamic_cast<DirLight*>(light)) {
            json ld = {
                {"ID",ptr->ID.c_str()},
                {"type","directional"},
                {"direction",json::array({ptr->direction[0],ptr->direction[1],ptr->direction[2]})},
                {"color",json::array({ptr->lightColor[0],ptr->lightColor[1],ptr->lightColor[2]})},
                {"intensity",ptr->intensity}
            };
            lightData.push_back(ld);
        }
    }
    data["Light"] = lightData;
    data["Environment"]["Irradiance"] = envLight;
    std::ofstream outFile(filePath);
    if (!outFile.is_open()) {
        std::cerr << "file open failed" << std::endl;
    }else {
        outFile << data.dump();
    }
    outFile.close();
    std::cout << "Write into file \""<< filePath << "\""<< std::endl;
}


void SceneComponent::setup(DisplayComponent &display) {
    aspect = static_cast<float>(display.ScrWidth) / static_cast<float>(display.ScrHeight);
}

void SceneComponent::update(float deltaTime) const {
}

void SceneComponent::destroy() const {
    for (auto [fst, snd] : modelMap) {
        if(snd != nullptr) {
            snd->destroy();
            delete(snd);
        }
    }
}

