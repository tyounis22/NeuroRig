//#pragma once

#include <iostream>
#include "raylib.h"

class Entity {
public:
    Entity();

    size_t id;
    Vector3 position = {0.0f, 0.0f, 0.0f};
    Quaternion orientation = {0.0f, 0.0f, 0.0f, 1.0f};

    //math for rotation involves cos, cos(0) = 1, therefore w component must hold that amount of rotation.
    virtual ~Entity();
};

