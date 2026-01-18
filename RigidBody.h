//
// Created by Tareq Younis on 03/01/2026.
//
#pragma once
#include "raylib.h"
#include "Entity.h"

class RigidBody : public Entity {
public:
    RigidBody();
    float mass = 1.0f;
    Vector3 velocity = {0.0f, 0.0f, 0.0f};
    Vector3 force = {0.0f, 0.0f, 0.0f};

    void ApplyForce(Vector3 force);

    virtual void Update(float deltaTime); //adding virtual here will allow the Vehicle class to change the way movement works.

    virtual ~RigidBody();

};


