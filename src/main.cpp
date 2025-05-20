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

    constexpr int maxBuildingW{50};
    [[maybe_unused]] constexpr int minBuildingW{20};

    for (auto i{0}; i < GetScreenWidth() / maxBuildingW; ++i)
    {
        const int maxScreenH{GetScreenHeight() / 2};
        const int minScreenH{maxScreenH / 2};

        Rectangle2D building{maxBuildingW,
                             static_cast<float>(
                                 Random::get(minScreenH, maxScreenH))};

        constexpr unsigned int minAlpha{70u};
        constexpr unsigned int maxAlpha{255u};

        building.setTint(Color{230, 41, 55,
                               static_cast<unsigned char>(
                                   Random::get(minAlpha, maxAlpha))});

        building.setPosition(Vector2{
            static_cast<float>(maxBuildingW * i),
            static_cast<float>(GetScreenHeight()) - building.getHeight(),
        });

        buildings.push_front(building);
    }
}