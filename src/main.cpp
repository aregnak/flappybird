#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/ContextSettings.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Window/WindowStyle.hpp>
#include <algorithm>
#include <cinttypes>
#include <vector>
#include <iostream>

#include "bird.h"
#include "walls.h"

void spawnWalls(sf::RenderWindow& window, std::vector<Wall>& walls, float wallX)
{
    if (wallX < (float)window.getSize().x / 2)
    {
        float wallY = (rand() % (600 - 200 + 1) + 200);
        walls.push_back(Wall(window.getSize().x, wallY));
        walls.push_back(Wall(window.getSize().x, wallY - 875));
    }
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 800), "Flappy Bird", sf::Style::Default,
                            sf::ContextSettings(0, 0, 8));

    window.setFramerateLimit(60);
    window.setKeyRepeatEnabled(false);

    sf::Clock timer;

    Bird bird(30, 30);
    std::vector<Wall> walls;

    float wallX;
    bool gameOver;

    //sf::RectangleShape rect;
    //rect.setSize(sf::Vector2f(100, 100));
    //rect.setPosition(sf::Vector2f(400, 400));

    while (window.isOpen())
    {
        window.setSize(sf::Vector2u(800, 800));

        sf::Time deltaTime = timer.restart();

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
            else if (event.type == event.KeyPressed)
            {
                if (gameOver && event.key.code == sf::Keyboard::Enter)
                {
                    gameOver = false;
                    bird.reset();
                    walls.clear();
                }

                else if (!gameOver)
                    // float wallX = walls.getPos();
                    bird.handleEvent(event);
            }
        }

        window.clear();

        if (!gameOver)
        {
            spawnWalls(window, walls, wallX);

            for (Wall& wall : walls)
            {
                wallX = wall.getPos();
                wall.update(deltaTime);
                wall.drawTo(window);
            }

            walls.erase(std::remove_if(walls.begin(), walls.end(),
                                       [&window](const Wall& wall) { return wall.getPos() < -60; }),
                        walls.end());

            sf::Vector2f playerPos = bird.getPos();

            if (playerPos.y >= window.getSize().y - 50)
            {
                //window.close();
                gameOver = true;
            }

            bird.update(deltaTime);

            bird.drawTo(window);
            window.display();
        }
    }
}