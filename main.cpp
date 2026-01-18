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
    camera.position = (Vector3){0.0f, 10.0f, 10.0f};
    camera.target = (Vector3){0.0f, 0.0f, 0.0f};
    camera.up = (Vector3){0.0f,1.0f,0.0f};
    camera.fovy = 45.0f; //field of view,
    camera.projection = CAMERA_PERSPECTIVE;

    std::unique_ptr<Vehicle> towMater = std::make_unique<Vehicle>();
    towMater->position = (Vector3){0.0f, 1.0f, 0.0f};

    while (!WindowShouldClose()) {

        // --- INPUT SECTION ---
        // We now call the high-level Vehicle functions
        if (IsKeyDown(KEY_UP)) {
            towMater->Gas(-1.0f);
        }
        if (IsKeyDown(KEY_DOWN)) {
            towMater->Gas(1.0f); // Reverse
        }
        if (IsKeyDown(KEY_LEFT)) {
            towMater->Steering(-1.0f); // Turn Left
        }
        if (IsKeyDown(KEY_RIGHT)) {
            towMater->Steering(1.0f); // Turn Right
        }

        // --- PHYSICS SECTION ---
        towMater->Update(GetFrameTime());

        // --- DRAWING SECTION ---
        BeginDrawing();
            ClearBackground(RAYWHITE);

            BeginMode3D(camera);
                DrawGrid(20, 1.0f);

                // --- MATRIX MAGIC START ---
                // We need to tell Raylib: "Apply this rotation to everything I draw next"
                rlPushMatrix();

                    // 1. Move the "pen" to the truck's position
                    rlTranslatef(towMater->position.x, towMater->position.y, towMater->position.z);

                    // 2. Rotate the "pen" to match the truck's orientation
                    // We need to convert Quaternion to Axis-Angle for rlRotatef
                    Vector3 axis;
                    float angle;
                    QuaternionToAxisAngle(towMater->orientation, &axis, &angle);

                    // Raylib uses Degrees, Quaternion gives Radians. Convert it!
                    rlRotatef(angle * RAD2DEG, axis.x, axis.y, axis.z);

                    // 3. Draw the Truck
                    // (We draw at 0,0,0 relative to the 'pen', because we already moved the pen to the right spot)

                    // Body (Red)
                    DrawCube((Vector3){0, 0, 0}, 2.0f, 2.0f, 4.0f, RED);

                    // Nose/Cabin (Blue) - This helps you see which way is FORWARD
                    DrawCube((Vector3){0, 0.5f, 1.5f}, 1.5f, 1.5f, 1.0f, BLUE);

                    // Wires for style
                    DrawCubeWires((Vector3){0, 0, 0}, 2.0f, 2.0f, 4.0f, MAROON);

                rlPopMatrix();
                // --- MATRIX MAGIC END ---
                // Reset the "pen" so we don't accidentally draw the grid rotated next frame

            EndMode3D();

            DrawFPS(10, 10);
            DrawText("Use Arrows to Drive!", 10, 40, 20, DARKGRAY);

            // Debug: Print Speed so you can see physics working
            DrawText(TextFormat("Speed: %.2f", Vector3Length(towMater->velocity)), 10, 70, 20, DARKGRAY);

        EndDrawing();
    }


CloseWindow();
return 0;
}
