#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Window/Event.hpp>
#include <algorithm>
#include <exception>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <ctime>

#include "bird.h"
#include "walls.h"

sf::Texture Wall::texture;

void spawnWalls(sf::RenderWindow& window, std::vector<Wall>& walls, float& wallX)
{
    if (wallX < window.getSize().x / 2.f)
    {
        float wallY = (rand() % (600 - 200 + 1) + 200);
        walls.push_back(Wall(window.getSize().x, wallY));
        walls.push_back(Wall(window.getSize().x, wallY - 880));
    }
}

int main()
{
    srand(static_cast<unsigned>(time(0)));

    sf::RenderWindow window(sf::VideoMode(800, 800), "Flappy Bird", sf::Style::Default,
                            sf::ContextSettings(0, 0, 8));

    window.setFramerateLimit(60);
    window.setKeyRepeatEnabled(false);

    sf::Clock timer;
    sf::Clock gameTimer;

    Bird bird(35, 35);
    std::vector<Wall> walls;

    bool gameOver = false;
    float wallX = 0;
    int score = 0;
    bool isPassed = false;
    bool isHit = false;
    float currentTime = 0;

    sf::Font font;
    if (!font.loadFromFile("res/font/Neon.ttf"))
    {
        std::cerr << "Could not load font" << std::endl;
        return 1;
    }
    font.setSmooth(true);

    sf::Texture bgTexture;
    if (!bgTexture.loadFromFile("res/sprite/Background/Background7.png"))
    {
        std::cout << "back ground texture failed" << std::endl;
        system("pause");
    }
    bgTexture.setRepeated(true);

    sf::Sprite bg(bgTexture);
    bg.setTextureRect(sf::IntRect(0, 0, window.getSize().x - (window.getSize().x / 3.f),
                                  window.getSize().y - (window.getSize().y / 2.f)));
    bg.setScale(window.getSize().x / 256.f, window.getSize().y / 256.f);

    const float bgScrollSpeed = 100.f;

    sf::Text gameOverText;
    gameOverText.setFont(font);
    gameOverText.setString("Game Over!\nPress Enter to Restart");
    gameOverText.setCharacterSize(50);
    gameOverText.setFillColor(sf::Color::White);
    gameOverText.setStyle(sf::Text::Bold);
    gameOverText.setPosition(window.getSize().x / 2.f - gameOverText.getLocalBounds().width / 2,
                             window.getSize().y / 2.f - gameOverText.getLocalBounds().height / 2);
    gameOverText.setOutlineColor(sf::Color::Black);
    gameOverText.setOutlineThickness(5.f);

    sf::Text scoreText;
    scoreText.setFont(font);
    scoreText.setCharacterSize(30);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(10, 10);
    scoreText.setOutlineColor(sf::Color::Black);
    scoreText.setOutlineThickness(5.f);

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
                    isHit = false;
                    score = 0;
                    bird.reset();
                    walls.clear();
                    walls.push_back(Wall(60, window.getSize().y));

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
            float moveDistance = bgScrollSpeed * deltaTime.asSeconds();
            bg.move(-moveDistance, 0);
            if (bg.getPosition().x <= -800)
            {
                bg.setPosition(0, 0);
            }

            window.draw(bg);

            for (Wall& wall : walls)
            {
                wallX = wall.getX();
                wall.update(deltaTime);
                wall.drawTo(window);

                if (wall.collision(bird.getShape()))
                {
                    //bird.deathAnimation();

                    if (!isHit)
                    {
                        isHit = true;

                        currentTime = gameTimer.getElapsedTime().asSeconds();
                    }
                }
                if (isHit)
                {
                    if (gameTimer.getElapsedTime().asSeconds() - currentTime > 0.3)
                    {
                        //gameOver = true;
                    }
                }

                if (!isPassed && wall.getX() < bird.getPos().x &&
                    wall.getX() > bird.getPos().x - 20)
                {
                    isPassed = true;
                    score++;
                }

                if (wall.getX() < -80)
                {
                    isPassed = false;
                }
            }
            spawnWalls(window, walls, wallX);

            bird.update(deltaTime);
            bird.drawTo(window);

            walls.erase(std::remove_if(walls.begin(), walls.end(),
                                       [](const Wall& wall) { return wall.getX() < -80; }),
                        walls.end());

            sf::Vector2f playerPos = bird.getPos();

            if (playerPos.y >= window.getSize().y - 50)
            {
                bird.deathAnimation();
                if (!isHit)
                {
                    isHit = true;

                    currentTime = gameTimer.getElapsedTime().asSeconds();
                }
            }

            scoreText.setString("Score  " + std::to_string(score));
            window.draw(scoreText);

            window.display();
        }
        else
        {
            window.draw(bg);
            window.draw(gameOverText);

            window.display();
        }
    }

    return 0;
}