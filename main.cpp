#include <iostream>
#include <memory>
#include "RigidBody.h"


int main() {
    const int screenWidth = 800;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "First Truck Test");
    SetTargetFPS(60);

    Camera3D camera = { 0 };
    camera.position = (Vector3){0.0f, 10.0f, 10.0f};
    camera.target = (Vector3){0.0f, 0.0f, 0.0f};
    camera.up = (Vector3){0.0f,1.0f,0.0f};
    camera.fovy = 45.0f; //field of view,
    camera.projection = CAMERA_PERSPECTIVE;

    std::unique_ptr<RigidBody> towMater = std::make_unique<RigidBody>();
    towMater->position = (Vector3){0.0f, 1.0f, 0.0f};

    while (!WindowShouldClose()) {
        if (IsKeyDown(KEY_UP)) {
            towMater->ApplyForce({0.0f, 0.0f, -20.0f});
        }
        if (IsKeyDown(KEY_DOWN)) {
            towMater->ApplyForce({0.0f, 0.0f, 20.0f});
        }
        towMater->Update(GetFrameTime());
    BeginDrawing();
        ClearBackground(RAYWHITE);

    BeginMode3D(camera);
        DrawGrid(20,1.0f);
        DrawCube(towMater->position, 2.0f,2.0f,4.0f,RED);
        DrawCubeWires(towMater->position, 2.0f, 2.0f, 4.0f, MAROON);
    EndMode3D();

    DrawFPS(10,10);
    DrawText("Move with up/down arrows", 10,40,20, BLACK);
    EndDrawing();
}

CloseWindow();
return 0;
}
