#include "Rectangle2D.h"
#include "Random.h"
#include <raylib.h>
#include <forward_list> // for std::forward_list
#include <iostream>
#include <cassert> // for assert

void initBuildings(std::forward_list<Rectangle2D> &buildings);
void placeGorillaOnBuilding(const std::forward_list<Rectangle2D> &buildings,
                            Rectangle2D &gorilla,
                            float minScreenW, float maxScreenW);

int main()
{
    constexpr int screenW{800};
    constexpr int screenH{450};

    InitWindow(screenW, screenH, "Gorillas");

    SetTargetFPS(60);

    // a list to store all the buildings
    std::forward_list<Rectangle2D> buildings{};

    initBuildings(buildings);

    constexpr float gorillaW{50};
    constexpr float gorillaH{50};

    Rectangle2D lGorilla{gorillaW, gorillaH};
    lGorilla.setTint(BLUE);

    placeGorillaOnBuilding(buildings, lGorilla, screenW - 200, screenW);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        for (const Rectangle2D &building : buildings)
            DrawRectangleRec(building.getRectangle(), building.getTint());

        DrawRectangleRec(lGorilla.getRectangle(), lGorilla.getTint());

        DrawFPS(0, 0);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}

void initBuildings(std::forward_list<Rectangle2D> &buildings)
{

    int remainingScreenW{GetScreenWidth()};

    while (remainingScreenW > 0)
    {
        int randomBuildingW{};

        constexpr int maxBuildingW{80};
        constexpr int minBuildingW{50};

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
        building.setTint(Color{130, 130, 130,
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

void placeGorillaOnBuilding(const std::forward_list<Rectangle2D> &buildings,
                            Rectangle2D &gorilla,
                            float minScreenW, float maxScreenW)
{
    assert((!buildings.empty()) && "There are'nt any buildings.");
    assert(minScreenW < maxScreenW && "Make sure min screen width"
                                      " > max screen width");

    float currentX{minScreenW};

    auto building{buildings.cbegin()};

    while (currentX < maxScreenW)
    {

        if (building == buildings.cend())
            assert(false && "was'nt able to place that gorilla");

        float buildingFullWidth{building->getX() + building->getWidth()};

        // check if the current building's x and width sum falls within
        // the min and max screen width
        if (buildingFullWidth >= minScreenW && buildingFullWidth <= maxScreenW)
        {
            if (building->getWidth() >= gorilla.getWidth())
            {
                // place the given gorilla on this given building
                gorilla.setX(building->getX());
                gorilla.setY(building->getY() - gorilla.getHeight());
                return;
            }
        }

        currentX += building->getX();
        ++building;
    }
}