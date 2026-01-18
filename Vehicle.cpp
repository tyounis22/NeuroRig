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

// In Vehicle.cpp

void Vehicle::Update(float deltaTime) {

    // --- STEP 1: TIRE GRIP LOGIC ---

    // Calculate the truck's "Right" vector (The direction sticking out the passenger window)
    // In local space, Right is {1, 0, 0}
    Vector3 rightLocal = { 1.0f, 0.0f, 0.0f };
    Vector3 rightWorld = Vector3RotateByQuaternion(rightLocal, this->orientation);

    // Calculate how fast we are sliding sideways (Dot Product)
    // Dot Product asks: "How much of the Velocity is going in the Right direction?"
    float sidewaysSpeed = Vector3DotProduct(this->velocity, rightWorld);

    // Create an impulse to cancel that sliding
    // 0.95f is the "Grip Factor".
    // 1.0 = On Rails (No drift at all)
    // 0.0 = Pure Ice (Full drift)
    // 0.95 = Tarmac (Tiny bit of slip)
    Vector3 gripImpulse = Vector3Scale(rightWorld, -sidewaysSpeed * 0.95f);

    // Apply the grip directly to velocity (Tires react instantly)
    this->velocity = Vector3Add(this->velocity, gripImpulse);

    // --- STEP 2: STANDARD PHYSICS ---
    // Now run the normal physics (Drag, Acceleration, Movement)
    RigidBody::Update(deltaTime);
}


