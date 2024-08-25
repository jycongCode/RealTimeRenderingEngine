//
// Created by Lenovo on 2024/8/23.
//

#ifndef FILEUTILS_H
#define FILEUTILS_H
#include <fstream>
#include <iostream>
#include <glm/glm.hpp>
#include <nlohmann/json.hpp>
#include "Material.h"
using json = nlohmann::json;
namespace FileUtils {
    inline json GetJsonFromFile(const char* path) {
        std::ifstream f(path);
        json data;
        if(!f.is_open()) {
            std::cout << "File Error: Cannot open " << path << std::endl;
        }else {
            data = json::parse(f);
        }
        f.close();
        return data;
    }

   inline void DumpJsonFile(json data,const char* path) {
        std::ofstream out(path);
        if(!out.is_open()) {
            std::cout << "File Error: Cannot open " << path << std::endl;
        }else {
            out << data.dump();
        }
        out.close();
    }

   inline glm::vec3 GetVec3FromJson(json data,const char* prefix) {
        return {data[prefix][0],data[prefix][1],data[prefix][2]};
    }

    inline json::array_t CreateJsonArrayFromVec3(glm::vec3 val) {
        return json::array({val.x,val.y,val.z});
    }

    inline Material* GetMaterialFromJson(json mat) {
        if(mat["type"] == "BlinnPhong") {
            return new MBlinnPhong(mat["ambient"]);
        }else if(mat["type"] == "BlinnPhong_Pure") {
            auto color = GetVec3FromJson(mat,"color");
            return new MBlinnPhong_Pure(color,mat["ambient"]);
        }else {
            return nullptr;
        }
    }

    inline json CreateJsonFromMaterial(Material* mat) {
        json data;
        if(MBlinnPhong* ptr = dynamic_cast<MBlinnPhong*>(mat)) {
            data = {
               { "type",ptr->GetType().c_str()},
               {"ambient",ptr->ambient}
            };
        }else if(MBlinnPhong_Pure* ptr = dynamic_cast<MBlinnPhong_Pure*>(mat)) {
            data = {
                { "type",ptr->GetType().c_str()},
                {"ambient",ptr->ambient},
                {"color",CreateJsonArrayFromVec3(ptr->color)}
            };
        }
        return data;
    }


}
#endif //FILEUTILS_H
