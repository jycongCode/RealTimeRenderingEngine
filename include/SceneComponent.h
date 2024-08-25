//
// Created by Lenovo on 2024/8/14.
//

#ifndef SCENE_COMPONENT
#define SCENE_COMPONENT

#include "Component.h"
#include "Scene.h"
class SceneComponent:public Component{
public:
    Scene* Current;
    std::string CurrentID;
    std::string CurrentStoragePath;
    void SetUp(RTREngine* engine) override;
    void Update(float deltaTime) override;
    void Destroy() override;
};
#endif
