//
// Created by Lenovo on 2024/8/14.
//

#ifndef _SCENE_COMPONENT
#define _SCENE_COMPONENT
#include "Camera.h"
class SceneComponent {
public:
    Camera camera;

public:
    void setup();
    void update(float deltaTime);
    void destroy();
};
#endif
