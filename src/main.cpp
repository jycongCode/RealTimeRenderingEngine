//
// Created by Lenovo on 2024/8/12.
//

#include "RTREngine.h"
int main(void)
{
    RTREngine engine;
    engine.setup();
    engine.sceneComponent.addModel("Marry","../resources/models/Marry/Marry.obj");
    engine.renderComponent.addShader("BlinnPhong","../resources/shaders/BlinnPhong.vert","../resources/shaders/BlinnPhong.frag");
    engine.run();
    return 0;
}