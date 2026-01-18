#include <iostream>
#include <memory>
#include <vector>       // Required for managing multiple cars, vectors of vehicle objects
#include "Vehicle.h"
#include "rlgl.h"
#include "raymath.h"

//needs a helper function to draw the environment,
    void DrawHighway() {
    // asphalt (Dark Gray)
    // -20 to +20 width (4 lanes approx), 2000 units long
    DrawCube((Vector3){0, -0.05f, 500.0f}, 40.0f, 0.1f, 2000.0f, DARKGRAY);

        //the double yellow center divider,
        DrawCube((Vector3){-0.5f, 0.0f, 500.0f}, 0.5f, 0.11f, 2000.0f, GOLD);
    DrawCube((Vector3){ 0.5f, 0.0f, 500.0f}, 0.5f, 0.11f, 2000.0f, GOLD);

    // dash lane markers
    // loop to draw dashed lines every 20 units
    for (int z = -500; z < 1500; z += 20) {

        DrawCube((Vector3){10.0f, 0.0f, (float)z}, 0.5f, 0.11f, 10.0f, WHITE);
        DrawCube((Vector3){-10.0f, 0.0f, (float)z}, 0.5f, 0.11f, 10.0f, WHITE);
    }
}

//function to draw a vehicle
// we extracted this so we can draw the Player AND the Traffic easily

void DrawVehicle(Vehicle& v, Color cabinColor, Color trailerColor) {
    rlPushMatrix();
        //move to vehicle position
        rlTranslatef(v.position.x, v.position.y, v.position.z);

        //Rotate
        Vector3 axis; float angle;
        QuaternionToAxisAngle(v.orientation, &axis, &angle);
        rlRotatef(angle * RAD2DEG, axis.x, axis.y, axis.z);

        // art part
        // chassis
        DrawCube((Vector3){0, 0.5f, 0}, 1.8f, 0.5f, 6.0f, DARKGRAY);
        // cabin
        DrawCube((Vector3){0, 1.75f, 2.0f}, 2.0f, 2.5f, 1.5f, cabinColor);
        DrawCube((Vector3){0, 2.2f, 2.76f}, 1.8f, 1.0f, 0.1f, SKYBLUE); // Windshield
        // trailer
        DrawCube((Vector3){0, 2.0f, -1.5f}, 2.2f, 3.5f, 4.5f, trailerColor);
        // wheels
        Vector3 wheelPositions[] = { {-1.1f,0.5f,2.0f}, {1.1f,0.5f,2.0f}, {-1.1f,0.5f,-1.0f}, {1.1f,0.5f,-1.0f}, {-1.1f,0.5f,-2.5f}, {1.1f,0.5f,-2.5f} };
        for(auto& pos : wheelPositions) {
            DrawCube(pos, 0.5f, 1.0f, 1.0f, BLACK);
            DrawCubeWires(pos, 0.5f, 1.0f, 1.0f, DARKGRAY);
        }
    rlPopMatrix();
}

int main() {
    const int screenWidth = 1700;
    const int screenHeight = 1000;
    InitWindow(screenWidth, screenHeight, "NeuroRig: Highway Sim");
    SetTargetFPS(60);

    //setup main player (will be autonomous later)
    std::unique_ptr<Vehicle> player = std::make_unique<Vehicle>();
    player->position = (Vector3){10.0f, 1.0f, 0.0f}; // Start in Right Lane

    //setup the traffic and spawned random cars
        std::vector<std::unique_ptr<Vehicle>> traffic;

    // helper to spawn a car
    auto SpawnCar = [&](float x, float z) {
        auto car = std::make_unique<Vehicle>();
        car->position = {x, 1.0f, z};
        traffic.push_back(std::move(car));
    };
        auto SpawnCarReverse = [&](float x, float z) {
            auto car = std::make_unique<Vehicle>();
            car->position = {x, 1.0f, z};
            traffic.push_back(std::move(car));
        };


    // spawn 3 cars in different lanes
    SpawnCar(-5.0f, 50.0f); // Left lane, ahead
    SpawnCar(5.0f, 100.0f); // Right lane, further ahead
    SpawnCarReverse(-5.0f, -40.0f); // Left lane, behind

    //setuyp the camera
    Camera3D camera = { 0 };
    camera.up = (Vector3){0.0f, 1.0f, 0.0f};
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    // "target" camera position (where we WANT to be)
    Vector3 cameraOffset = { 0.0f, 15.0f, -30.0f }; // Behind and Up
    Vector3 currentCamPos = { 0.0f, 20.0f, -40.0f }; // Actual position (starts far back)

    while (!WindowShouldClose()) {

        // 1. UPDATE PLAYER
        if (IsKeyDown(KEY_UP))    player->Gas(1.0f);
        if (IsKeyDown(KEY_DOWN))  player->Gas(-1.0f);
        if (IsKeyDown(KEY_LEFT))  player->Steering(-1.0f);
        if (IsKeyDown(KEY_RIGHT)) player->Steering(1.0f);
        player->Update(GetFrameTime());

        // UPDATE TRAFFIC
        for (auto& car : traffic) {
            // AI Logic: Just hold the gas pedal down slightly (0.4 power)
            // This makes them cruise at a steady speed
            car->Gas(0.4f);

            // AI Physics
            car->Update(GetFrameTime());
        }

        // UPDATE CAMERA (Smoothing)
        // Calculate where the camera SHOULD be
        Vector3 targetPos = Vector3Add(player->position, Vector3RotateByQuaternion({0, 20.0f, -30.0f}, player->orientation));

        // Use Lerp to smoothly slide 'currentCamPos' towards 'targetPos'
        // 0.05f is the "Smooth Speed" (Lower is slower/heavier)
        currentCamPos.x = Lerp(currentCamPos.x, targetPos.x, 0.05f);
        currentCamPos.y = Lerp(currentCamPos.y, targetPos.y, 0.05f);
        currentCamPos.z = Lerp(currentCamPos.z, targetPos.z, 0.05f);

        camera.position = currentCamPos;
        camera.target = player->position;


        // DRAW
        BeginDrawing();
            ClearBackground(BROWN); // Day sky

            BeginMode3D(camera);

                DrawHighway();

                // Draw Player (Red Cabin, White Trailer)
                DrawVehicle(*player, RED, RAYWHITE);

                // Draw Traffic (Blue Cabins, Orange Trailers)
                for(auto& car : traffic) {
                    DrawVehicle(*car, BLUE, ORANGE);
                }

            EndMode3D();

            DrawFPS(10, 10);
            DrawText("Traffic Simulation Active", 10, 40, 20, DARKGRAY);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}