//
// Created by Tareq Younis on 12/01/2026.
//

#include "Vehicle.h"
#include "raymath.h"
#include "external/vox_loader.h"

Vehicle::Vehicle() {
}

Vehicle::~Vehicle() {
}

void Vehicle::Gas(float amount) {

    //it's all relative so first define forward as z axis in trucks local space
    Vector3 localForward  { 0.0f, 0.0f, 1.0f};

    //rotate that vector to match the truck's current orientation
    Vector3 worldForward = Vector3RotateByQuaternion(localForward, this->orientation);

    //scale it by throttle amount and engine power
    Vector3 forceToApply = Vector3Scale(worldForward, amount * this->enginePower);

    //push the truck forward
    this->ApplyForce(forceToApply);

}

void Vehicle::Steering(float amount) {

    //define rotation sensitivity
    Quaternion turnRotation = QuaternionFromAxisAngle({0.0f, 1.0f, 0.0f}, amount * -0.05f);

    //lock in changes to accumulate rotation. Do this by multiplying current rotation by the new turn
    this->orientation = QuaternionMultiply(this->orientation, turnRotation);
    }

void Vehicle::Update(float deltaTime) {

    //must call parent class's update function, to ensure the physics math still happens
    RigidBody::Update(deltaTime);
}


