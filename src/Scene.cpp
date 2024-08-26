//
// Created by Lenovo on 2024/8/23.
//

#include "Scene.h"
#include "Model.h"
#include "Quad.h"
#include <fstream>
#include "FileUtils.h"

glm::mat4 Scene::GetViewMatrix() {
    return MainCamera.GetViewMatrix();
}

glm::mat4 Scene::GetProjectionMatrix() const {
    return glm::perspective(MainCamera.Zoom,MainCamera.aspect,MainCamera.nearPlane,MainCamera.farPlane);
}

void Scene::Destroy() {
    for(Drawable* drawable : DrawableList) {
        drawable->destroy();
    }
    DrawableList.clear();
}

void Scene::Load(const char *path) {
    StoragePath = path;
    json data = FileUtils::GetJsonFromFile(path);
    // load scene id
    ID = data["id"];
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
            this->DrawableList.push_back(model);
        }else if(drawable["type"] == "Quad") {
            auto* quad = new Quad();
            quad->ID = drawable["id"];
            quad->Position = FileUtils::GetVec3FromJson(drawable,"position");
            quad->Rotation = FileUtils::GetVec3FromJson(drawable,"rotation");
            quad->Scale = FileUtils::GetVec3FromJson(drawable,"scale");
            quad->Mat = FileUtils::GetMaterialFromJson(drawable["material"]);
            this->DrawableList.push_back(quad);
        }
    }

    // Load camera
    auto cameraData = data["Camera"];
    MainCamera.Position = FileUtils::GetVec3FromJson(cameraData,"position");
    MainCamera.Pitch = cameraData["pitch"];
    MainCamera.Yaw = cameraData["yaw"];
    MainCamera.Zoom = cameraData["zoom"];
    MainCamera.MouseSensitivity = cameraData["sensitivity"];
    MainCamera.MovementSpeed = cameraData["speed"];
    MainCamera.ID = cameraData["id"];

    // Load Lights
    auto lightData = data["Light"];
    // load sunlight
    Sunlight.direction = FileUtils::GetVec3FromJson(lightData["SunLight"],"direction");
    Sunlight.lightColor = FileUtils::GetVec3FromJson(lightData["SunLight"],"color");
    Sunlight.intensity = lightData["SunLight"]["intensity"];
    Sunlight.right = lightData["SunLight"]["right"];
    Sunlight.top = lightData["SunLight"]["top"];
    Sunlight.nearp = lightData["SunLight"]["nearp"];
    Sunlight.farp = lightData["SunLight"]["farp"];
    Sunlight.dis = lightData["SunLight"]["dis"];

    std::cout << "Scene [" << ID << "] loaded from path : "  << StoragePath << std::endl;
}

void Scene::Save() {
    Save(StoragePath.c_str());
}

void Scene::Save(const char *path) {
    json data;

    // save scene id
    data["id"] = ID;
    auto drawableData = json::array();

    // save drawables
    for(const auto drawable : DrawableList) {
        if(const auto* ptr = dynamic_cast<Model*>(drawable)) {
            json model = {
                {"id",ptr->ID.c_str()},
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
                {"id",ptr->ID.c_str()},
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
        {"id",MainCamera.ID},
        {"position",json::array({MainCamera.Position[0],MainCamera.Position[1],MainCamera.Position[2]})},
        {"pitch",MainCamera.Pitch},
          {"yaw",MainCamera.Yaw},
        {"zoom",MainCamera.Zoom},
        {"sensitivity",MainCamera.MouseSensitivity},
        {"speed",MainCamera.MovementSpeed}
    };
    data["Camera"] = camData;

    // save light
    json lightData;
    lightData["SunLight"] = {
        {"type","directional"},
        {"direction",FileUtils::CreateJsonArrayFromVec3(Sunlight.direction)},
        {"color",FileUtils::CreateJsonArrayFromVec3(Sunlight.lightColor)},
        {"intensity",Sunlight.intensity},
        {"right",Sunlight.right},
        {"top",Sunlight.top},
        {"nearp",Sunlight.nearp},
        {"farp",Sunlight.farp},
        {"dis",Sunlight.dis}
    };
    data["Light"] = lightData;
    FileUtils::DumpJsonFile(data,path);
    std::cout << "Scene [" << ID << "] saved in path : "  << path << std::endl;
}
