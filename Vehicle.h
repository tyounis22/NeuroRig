//
// Created by Tareq Younis on 12/01/2026.
//

#pragma once
#include "RigidBody.h"


class Vehicle : public RigidBody {
public:
    Vehicle();

    float enginePower = 50.0f;
    float steeringAngle = 0.0f;

    void Gas(float amount); //1.0 for full gas, -1.0 for full reverse
    void Steering(float amount); //1.0 for full right, -1.0 for full left

    void Update(float deltaTime) override;

    virtual ~Vehicle();



};
