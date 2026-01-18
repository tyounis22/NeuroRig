//
// Created by Tareq Younis on 03/01/2026.
//

#include "RigidBody.h"
#include "raymath.h"

RigidBody::RigidBody() {
}

RigidBody::~RigidBody() {
}

void RigidBody::ApplyForce(Vector3 forceIn) {
    this-> force = Vector3Add(this->force, forceIn);
}

void RigidBody::Update(float deltaTime) {
    //Now add drag logic (air resistance) which acts opposite to velocity
    // -2.0f is the drag coeffecient, how thick that air is.

    Vector3 dragForce = Vector3Scale(this->velocity, -2.0f);
    //now add the drag to the other forces
    this->force = Vector3Add(this->force, dragForce);
    Vector3 acceleration = Vector3Scale(this->force, 1.0f / this->mass);
    this->velocity = Vector3Add(this->velocity, Vector3Scale(acceleration, deltaTime));
    this->position = Vector3Add(this->position, Vector3Scale(this->velocity, deltaTime));
    this->force = {0.0f,0.0f,0.0f};


}