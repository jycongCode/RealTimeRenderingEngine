//
// Created by Lenovo on 2024/8/12.
//

#include "Quad.h"
#include "RTREngine.h"
int main(void)
{
    RTREngine engine;
    engine.setup();
    engine.sceneComponent.LoadScene("../scene/Marry.json");
    engine.renderComponent.addShader("BlinnPhong","../resources/shaders/BlinnPhong.vert","../resources/shaders/BlinnPhong.frag");
    engine.run();
    return 0;
}
