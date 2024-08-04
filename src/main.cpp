#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <algorithm>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <ctime>

#include "bird.h"
#include "walls.h"

sf::Texture Wall::texture;

void spawnWalls(sf::RenderWindow& window, std::vector<Wall>& walls, float& wallX)
{
    if (wallX < window.getSize().x / 2)
    {
        float wallY = (rand() % (600 - 200 + 1) + 200);
        walls.push_back(Wall(window.getSize().x, wallY));
        walls.push_back(Wall(window.getSize().x, wallY - 900));
    }
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 800), "Flappy Bird", sf::Style::Default,
                            sf::ContextSettings(0, 0, 8));

    window.setFramerateLimit(60);
    window.setKeyRepeatEnabled(false);

    srand(static_cast<unsigned>(time(0)));

    sf::Clock timer;

    Bird bird(35, 35);
    std::vector<Wall> walls;

    bool gameOver = false;
    float wallX;

    sf::Font font;
    if (!font.loadFromFile("res/font/Hey Comic.ttf"))
    {
        std::cerr << "Could not load font" << std::endl;
        return 1;
    }

    sf::Text gameOverText;
    gameOverText.setFont(font);
    gameOverText.setString("Game Over!\nPress Enter to Restart");
    gameOverText.setCharacterSize(50);
    gameOverText.setFillColor(sf::Color::White);
    gameOverText.setStyle(sf::Text::Bold);
    gameOverText.setPosition(window.getSize().x / 2 - gameOverText.getLocalBounds().width / 2,
                             window.getSize().y / 2 - gameOverText.getLocalBounds().height / 2);

    while (window.isOpen())
    {
        sf::Time deltaTime = timer.restart();

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
            else if (event.type == sf::Event::KeyPressed)
            {
                if (gameOver && event.key.code == sf::Keyboard::Enter)
                {
                    gameOver = false;
                    bird.reset();
                    walls.clear();
                    walls.push_back(Wall(50, window.getSize().y));

                    // spawnWalls(window, walls, wallX);
                    continue;
                }
                else if (!gameOver)
                {
                    bird.handleEvent(event);
                }
            }
        }

        window.clear();

        if (!gameOver)
        {
            bird.update(deltaTime);
            bird.drawTo(window);

            spawnWalls(window, walls, wallX);

            for (Wall& wall : walls)
            {
                wallX = wall.getPos();
                wall.update(deltaTime);
                wall.drawTo(window);
            }

            walls.erase(std::remove_if(walls.begin(), walls.end(),
                                       [](const Wall& wall) { return wall.getPos() < -60; }),
                        walls.end());

            sf::Vector2f playerPos = bird.getPos();

            if (playerPos.y >= window.getSize().y - 50)
            {
                gameOver = true;
                std::cout << "Game over" << std::endl;
            }

            window.display();
        }
        else
        {
            window.draw(gameOverText);
            window.display();
        }
    }

    return 0;
}
