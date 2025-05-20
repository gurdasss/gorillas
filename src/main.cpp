#include "Rectangle2D.h"
#include "Random.h"
#include <raylib.h>
#include <forward_list>
#include <iostream>

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

    constexpr int maxBuildingW{80};
    constexpr int minBuildingW{50};

    int remainingScreenW{GetScreenWidth()};

    while (remainingScreenW > 0)
    {
        int randomBuildingW{};

        if (remainingScreenW >= maxBuildingW)
        {
            // consider full predefined random range
            randomBuildingW = Random::get(minBuildingW, maxBuildingW);
            remainingScreenW -= randomBuildingW;
        }
        // fit the building to the left edge of the screen
        else if ((remainingScreenW < maxBuildingW &&
                  remainingScreenW > minBuildingW) ||
                 remainingScreenW <= minBuildingW)
        {
            randomBuildingW = remainingScreenW;

            // we've finally covered the whole screen width
            remainingScreenW = 0;
        }

        std::cout << "Remaining screen width: " << remainingScreenW << '\n';

        const int maxScreenH{GetScreenHeight() / 2};
        const int minScreenH{maxScreenH / 4};

        Rectangle2D building{static_cast<float>(randomBuildingW),
                             static_cast<float>(
                                 Random::get(minScreenH, maxScreenH))};

        constexpr unsigned int minAlpha{125u};
        constexpr unsigned int maxAlpha{255u};

        // randomly set the alpha value of base building color
        building.setTint(Color{230, 41, 55,
                               static_cast<unsigned char>(
                                   Random::get(minAlpha, maxAlpha))});

        building.setPosition(Vector2{
            // place the newly created building to
            // the leftmost x-axis of the screen
            static_cast<float>(remainingScreenW),
            static_cast<float>(GetScreenHeight()) - building.getHeight(),
        });

        buildings.push_front(building);
    }
}