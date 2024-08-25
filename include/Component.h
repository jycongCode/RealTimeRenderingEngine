//
// Created by Lenovo on 2024/8/24.
//

#ifndef COMPONENT_H
#define COMPONENT_H
class RTREngine;
class Component {
protected:
    RTREngine *engine = nullptr;
public:
    virtual ~Component() = default;
    virtual void SetUp(RTREngine*engine) {
        this->engine = engine;
    };
    virtual void Update(float deltaTime){};
    virtual void Destroy(){};
};
#endif //COMPONENT_H


/*
 * RTREngine -->
 */