#include <iostream>
#include <memory>
#include "Vehicle.h"
#include "rlgl.h"
#include "raymath.h"


int main() {
    const int screenWidth = 1700;
    const int screenHeight = 1000;

    InitWindow(screenWidth, screenHeight, "First Truck Test");
    SetTargetFPS(60);

    Camera3D camera = { 0 };
    camera.position = (Vector3){0.0f, 50.0f, 50.0f};
    camera.target = (Vector3){0.0f, 0.0f, 0.0f};
    camera.up = (Vector3){0.0f,1.0f,0.0f};
    camera.fovy = 45.0f; //field of view,
    camera.projection = CAMERA_PERSPECTIVE;

    std::unique_ptr<Vehicle> towMater = std::make_unique<Vehicle>();
    towMater->position = (Vector3){0.0f, 1.0f, 0.0f};

    while (!WindowShouldClose()) {

        // --- INPUT SECTION ---
        if (IsKeyDown(KEY_UP))    towMater->Gas(1.0f);
        if (IsKeyDown(KEY_DOWN))  towMater->Gas(-1.0f);
        if (IsKeyDown(KEY_LEFT))  towMater->Steering(-1.0f);
        if (IsKeyDown(KEY_RIGHT)) towMater->Steering(1.0f);

        // --- PHYSICS SECTION ---
        towMater->Update(GetFrameTime());

        // --- DRAWING SECTION ---
        BeginDrawing();
            ClearBackground(RAYWHITE);

            BeginMode3D(camera);
                DrawGrid(50, 1.0f); // Made grid bigger so you have room to drive

                // --- MATRIX TRANSFORMATIONS ---
                rlPushMatrix();
                    // 1. Move to Truck Position
                    rlTranslatef(towMater->position.x, towMater->position.y, towMater->position.z);

                    // 2. Rotate to match Truck Orientation
                    Vector3 axis;
                    float angle;
                    QuaternionToAxisAngle(towMater->orientation, &axis, &angle);
                    rlRotatef(angle * RAD2DEG, axis.x, axis.y, axis.z);

                    // --- TRUCK DRAWING (Local Coordinates) ---
                    // Since we moved the world to the truck, (0,0,0) is the center of the truck.

                    // A. The Chassis (Gray Frame)
                    DrawCube((Vector3){0, 0.5f, 0}, 1.8f, 0.5f, 6.0f, DARKGRAY);

                    // B. The Cabin (Red Head) - Sitting at +Z (Forward)
                    DrawCube((Vector3){0, 1.75f, 2.0f}, 2.0f, 2.5f, 1.5f, RED);
                    // Windshield
                    DrawCube((Vector3){0, 2.2f, 2.76f}, 1.8f, 1.0f, 0.1f, SKYBLUE);

                    // C. The Trailer (White Box) - Sitting at -Z (Back)
                    DrawCube((Vector3){0, 2.0f, -1.5f}, 2.2f, 3.5f, 4.5f, DARKPURPLE);
                    // Rear Door
                    DrawCube((Vector3){0, 2.0f, -3.76f}, 2.0f, 3.0f, 0.1f, GRAY);

                    // D. The Wheels (6 Black Blocks)
                    Vector3 wheelPositions[] = {
                        {-1.1f, 0.5f, 2.0f}, { 1.1f, 0.5f, 2.0f},   // Front Axle
                        {-1.1f, 0.5f, -1.0f}, { 1.1f, 0.5f, -1.0f}, // Rear Axle 1
                        {-1.1f, 0.5f, -2.5f}, { 1.1f, 0.5f, -2.5f}  // Rear Axle 2
                    };

                    for(int i = 0; i < 6; i++) {
                        DrawCube(wheelPositions[i], 0.5f, 1.0f, 1.0f, BLACK);
                        DrawCubeWires(wheelPositions[i], 0.5f, 1.0f, 1.0f, DARKGRAY);
                    }

                rlPopMatrix();
                // --- END MATRIX ---

            EndMode3D();

            // UI
            DrawFPS(10, 10);
            DrawText("NeuroRig: v0.3", 10, 40, 20, DARKGRAY);
            DrawText(TextFormat("Speed: %.1f MPH", Vector3Length(towMater->velocity) * 2.23f), 10, 70, 20, LIME);

        EndDrawing();
    }

CloseWindow();
return 0;
}
