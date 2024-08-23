//
// Created by Lenovo on 2024/8/23.
//

#include "Scene.h"
#include "Model.h"
#include "Quad.h"
#include <fstream>
#include "FileUtils.h"

glm::mat4 Scene::GetViewMatrix() {
    return camera.GetViewMatrix();
}

glm::mat4 Scene::GetProjectionMatrix() const {
    return glm::perspective(camera.Zoom,camera.aspect,camera.nearPlane,camera.farPlane);
}

void Scene::destroy() {
    for(Drawable* drawable : drawableList) {
        drawable->destroy();
    }
}

void Scene::Load(const char *path) {
    filePath = path;
    json data = FileUtils::GetJsonFromFile(path);
    // load scene id
    id = data["id"];
    // Load model
    auto drawableList = data["Models"];
    for (auto drawable : drawableList) {
        if(drawable["type"] == "model") {
            std::string file = drawable["path"];
            auto* model = new Model(file.c_str());
            model->ID = drawable["id"];
            model->filePath = file;
            model->Position = FileUtils::GetVec3FromJson(drawable,"position");
            model->Rotation = FileUtils::GetVec3FromJson(drawable,"rotation");
            model->Scale = FileUtils::GetVec3FromJson(drawable,"scale");
            model->Mat = FileUtils::GetMaterialFromJson(drawable["material"]);
            this->drawableList.push_back(model);
        }else if(drawable["type"] == "Quad") {
            std::string ID = drawable["id"];
            auto* quad = new Quad();
            quad->Position = FileUtils::GetVec3FromJson(drawable,"position");
            quad->Rotation = FileUtils::GetVec3FromJson(drawable,"rotation");
            quad->Scale = FileUtils::GetVec3FromJson(drawable,"scale");
            quad->Mat = FileUtils::GetMaterialFromJson(drawable["material"]);
            this->drawableList.push_back(quad);
        }
    }

    // Load camera
    auto cameraData = data["Camera"];
    camera.Position = FileUtils::GetVec3FromJson(cameraData,"position");
    camera.Pitch = cameraData["pitch"];
    camera.Yaw = cameraData["yaw"];
    camera.Zoom = cameraData["zoom"];
    camera.MouseSensitivity = cameraData["sensitivity"];
    camera.MovementSpeed = cameraData["speed"];
    camera.ID = cameraData["id"];

    // Load Lights
    auto lightData = data["Light"];
    // load sunlight
    sunLight.direction = FileUtils::GetVec3FromJson(lightData["SunLight"],"direction");
    sunLight.lightColor = FileUtils::GetVec3FromJson(lightData["SunLight"],"color");
    sunLight.intensity = lightData["SunLight"]["intensity"];

    std::cout << "Scene [" << data["ID"] << "] loaded from path : "  << filePath << std::endl;
}
void Scene::Save() {
    json data;

    // save scene id
    data["ID"] = id;
    auto drawableData = json::array();

    // save drawables
    for(const auto drawable : drawableList) {
        if(const auto* ptr = dynamic_cast<Model*>(drawable)) {
            json model = {
                {"ID",ptr->ID.c_str()},
                {"path",ptr->filePath.c_str()},
                {"type","model"},
                {"material",FileUtils::CreateJsonFromMaterial(ptr->Mat)},
                {"position",FileUtils::CreateJsonArrayFromVec3(ptr->Position)},
                {"rotation",FileUtils::CreateJsonArrayFromVec3(ptr->Rotation)},
                {"scale",FileUtils::CreateJsonArrayFromVec3(ptr->Scale)}
                };
            drawableData.push_back(model);
        }else if(const auto* ptr = dynamic_cast<Quad*>(drawable)) {
            json quad = {
                {"ID",ptr->ID.c_str()},
                {"type","Quad"},
                  {"material",FileUtils::CreateJsonFromMaterial(ptr->Mat)},
                {"position",FileUtils::CreateJsonArrayFromVec3(ptr->Position)},
                {"rotation",FileUtils::CreateJsonArrayFromVec3(ptr->Rotation)},
                {"scale",FileUtils::CreateJsonArrayFromVec3(ptr->Scale)}
            };
            drawableData.push_back(quad);
        }
    }
    data["Models"] = drawableData;

    // save camera
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

    // save light
    json lightData;
    lightData["SunLight"] = {
        {"ID",sunLight.ID.c_str()},
        {"type","directional"},
        {"direction",FileUtils::CreateJsonArrayFromVec3(sunLight.direction)},
        {"color",FileUtils::CreateJsonArrayFromVec3(sunLight.lightColor)},
        {"intensity",sunLight.intensity}
    };
    data["Light"] = lightData;
    FileUtils::DumpJsonFile(data,filePath.c_str());
    std::cout << "Scene [" << data["ID"] << "] saved in path : "  << filePath << std::endl;
}
