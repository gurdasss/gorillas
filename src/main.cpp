#include "Rectangle2D.h"
#include "Circle2D.h"
#include "Line2D.h"
#include "Random.h"
#include <raylib.h>
#include <raymath.h>
#include <forward_list> // for std::forward_list
#include <iostream>
#include <cassert> // for assert
#include <cmath>   // for std::sin & std::cos

void initBuildings(std::forward_list<Rectangle2D> &buildings);
void placeGorillaOnBuilding(const std::forward_list<Rectangle2D> &buildings,
                            Rectangle2D &gorilla,
                            float minScreenW, float maxScreenW);

int main()
{
    constexpr int screenW{800};
    constexpr int screenH{450};

    InitWindow(screenW, screenH, "Gorillas");

    constexpr int targetFPS{60};
    SetTargetFPS(targetFPS);

    // a list to store all the buildings
    std::forward_list<Rectangle2D> buildings{};

    initBuildings(buildings);

    // make sure the gorilla's width should be <= min building width
    constexpr float gorillaW{50};
    constexpr float gorillaH{gorillaW};

    Rectangle2D lGorilla{gorillaW, gorillaH};
    lGorilla.setTint(BLUE);

    // place the left gorilla on left side of the screen
    placeGorillaOnBuilding(buildings, lGorilla, 0, screenW / 4);

    Rectangle2D rGorilla{gorillaW, gorillaH};
    rGorilla.setTint(RED);

    // place the left gorilla on right side of the screen
    placeGorillaOnBuilding(buildings, rGorilla,
                           screenW - screenW / 4,
                           screenW);

    Circle2D banana{};
    banana.setRadius(20);
    banana.setTint(RED);
    banana.setPosition(lGorilla.getPosition());

    Vector2 velocity{};
    Vector2 acceleration{};

    Line2D arrow{};
    arrow.setStartPos(lGorilla.getPosition());
    arrow.setTint(RED);

    bool toggleChance{};

    while (!WindowShouldClose())
    {
        /*
        1. apply acceleration to velocity
        2. apply velocity to object's position
        */

        arrow.setEndPos(GetMousePosition());

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            float speed{Vector2Distance(arrow.getStartPos(),
                                        arrow.getEndPos()) *
                        5};

            float angleInRadian{
                Vector2LineAngle(arrow.getStartPos(),
                                 arrow.getEndPos())};

            // setting the initial velocity
            // dividing the X and Y component of
            // velocity using following formulas:
            // X initial velocity = speed * cos(angleInRadian)
            // because negative Y means upwards direction
            // Y initial velocity = -speed * sin(angleInRadian)
            velocity.x = speed * std::cos(angleInRadian);
            velocity.y = -speed * std::sin(angleInRadian);

            constexpr float gravityPower{100};
            // APPLYING GRAVITY TO THE BANANA
            acceleration.y = 9.8f * gravityPower;
        }

        constexpr float delta{1.0f / targetFPS};

        velocity.x += acceleration.x * delta;
        velocity.y += acceleration.y * delta;

        banana.setX(banana.getX() + velocity.x * delta);
        banana.setY(banana.getY() + velocity.y * delta);

        for (Rectangle2D &building : buildings)
        {
            if (CheckCollisionCircleRec(banana.getPosition(),
                                        banana.getRadius(),
                                        building.getRectangle()))
            {

                if (toggleChance)
                {
                    arrow.setStartPos(lGorilla.getPosition());
                    banana.setPosition(lGorilla.getPosition());
                }
                else
                {
                    arrow.setStartPos(rGorilla.getPosition());
                    banana.setPosition(rGorilla.getPosition());
                }

                toggleChance = !toggleChance;

                velocity = Vector2{};
                acceleration = Vector2{};

                building.setHeight(building.getHeight() - banana.getRadius());
                building.setY(screenH - building.getHeight());
            }
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        for (const Rectangle2D &building : buildings)
            DrawRectangleRec(building.getRectangle(), building.getTint());

        DrawRectangleRec(lGorilla.getRectangle(), lGorilla.getTint());
        DrawRectangleRec(rGorilla.getRectangle(), rGorilla.getTint());

        DrawLineV(arrow.getStartPos(), arrow.getEndPos(), arrow.getTint());
        DrawCircleV(banana.getPosition(), banana.getRadius(), banana.getTint());

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
                                      " < max screen width");

    auto building{buildings.cbegin()};

    // iterate over the whole list of buildings
    while (building != buildings.cend())
    {

        float buildingWidthPosition{building->getX() + building->getWidth()};

        // check if the current building's x + width falls within
        // the min and max screen width
        if (buildingWidthPosition >= minScreenW &&
            buildingWidthPosition <= maxScreenW)
        {
            if (building->getWidth() >= gorilla.getWidth())
            {
                // place the given gorilla on this given building
                gorilla.setX(building->getX());
                gorilla.setY(building->getY() - gorilla.getHeight());
                return;
            }
        }

        ++building;
    }
}
