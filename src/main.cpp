#include "Rectangle2D.h"
#include "Random.h"
#include <raylib.h>
#include <forward_list>

void initBuildings(std::forward_list<Rectangle2D> &buildings);

int main()
{
    constexpr int screenW{800};
    constexpr int screenH{450};

    InitWindow(screenW, screenH, "Gorillas");

    SetTargetFPS(60);

    // a list to store all the buildings
    std::forward_list<Rectangle2D> buildings{};

    initBuildings(buildings);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        for (const Rectangle2D &building : buildings)
            DrawRectangleRec(building.getRectangle(), building.getTint());

        DrawFPS(0, 0);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}

void initBuildings(std::forward_list<Rectangle2D> &buildings)
{
    const float maxScreenH{static_cast<float>(GetScreenHeight()) / 2.0f};
    const float minScreenH{maxScreenH / 2.0f};

    constexpr float maxBuildingW{50.0f};
    constexpr float minBuildingW{20.0f};
}