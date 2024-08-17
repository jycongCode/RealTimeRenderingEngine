//
// Created by Lenovo on 2024/8/12.
//

#include "Quad.h"
#include "RTREngine.h"
int main(void)
{
    RTREngine engine;
    engine.setup();
    Quad* q = new Quad();
    q->Rotation.x = 90.0f;
    Texture whiteTex;
    whiteTex.CreateFromColor({0.8f,0.7f,0.8f},32.0f);
    q->texture = whiteTex.id;
    engine.sceneComponent.addDrawable("Floor",q);
    engine.sceneComponent.addModel("Marry","../resources/models/Marry/Marry.obj");
    engine.renderComponent.addShader("BlinnPhong","../resources/shaders/BlinnPhong.vert","../resources/shaders/BlinnPhong.frag");
    engine.run();
    return 0;
}
