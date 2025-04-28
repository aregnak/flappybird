#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Audio.hpp>

#include <algorithm>
#include <string>
#include <iostream>
#include <cstdlib>
#include <ctime>

#include "bird.h"
#include "walls.h"
#include "loadSounds.h"
#include "loadHS.h"

#define screen_width 800
#define screen_height 800

sf::Texture Wall::texture;

void spawnWalls(sf::RenderWindow& window, std::vector<Wall>& walls, float& wallX)
{
    if (wallX < window.getSize().x / 2.f)
    {
        float wallY =
            (rand() % (600 - 200 + 1) + 200); // random position between 600 and 200 pixels
        walls.push_back(Wall(window.getSize().x, wallY));
        walls.push_back(Wall(window.getSize().x, wallY - 880)); // second wall
    }
}

int main()
{
    srand(static_cast<unsigned>(time(0)));

    sf::RenderWindow window(sf::VideoMode({screen_width, screen_height}), "Flappy Bird");
                            //sf::Style::Default, sf::ContextSettings({0}, {0}, {8}));

    // window.setFramerateLimit(75);
    window.setVerticalSyncEnabled(true);
    window.setKeyRepeatEnabled(false);

    sf::Clock timer;
    sf::Clock gameTimer;

    // creating bird object
    Bird bird(45, 45);
    bird.reset();

    // creating walls vector, all the walls are managed here
    std::vector<Wall> walls;
    walls.reserve(6);


    // game variable inits
    bool gameOver = false;
    bool mainMenu = true;
    bool gamePaused = false;

    float wallX = 0;
    int score = 0;
    int highscore = loadHighScore();

    bool isPassed = false;
    bool isDead = false;
    bool hitWall = false;
    bool stupid = false;
    float currentTime = 0;

    sf::Font font;
    if (!font.openFromFile("res/font/Neon.ttf"))
    {
        std::cerr << "Could not load font" << std::endl;
    }

    // initialize game texts and sounds
    sf::Text gameOverText = createGameOverText(font, window);
    sf::Text mainMenuText = createMainMenuText(font, window);
    sf::Text pausedText = createPausedText(font, window);
    sf::Text scoreText = createScoreText(font, window);
    sf::Text highscoreText = createHighscoreText(font, window);

    sf::Sound pointSound = createPointSound();
    sf::Sound passSound = createPassSound();
    sf::Sound hitSound = createHitSound();
    sf::Sound dieSound = createDieSound();
    //sf::Sound jumpSound = createJumpSound();

    sf::Texture bgTexture;
    if (!bgTexture.loadFromFile("res/sprite/Background/Background7.png"))
    {
        std::cout << "back ground texture failed" << std::endl;
        system("pause");
    }
    bgTexture.setRepeated(true);

    sf::Sprite bg(bgTexture);
    bg.setTextureRect(sf::IntRect({0, 0}, {(int)window.getSize().x - ((int)window.getSize().x / 3),
                                  (int)window.getSize().y - ((int)window.getSize().y / 2)}));
    bg.setScale({window.getSize().x / 256.f, window.getSize().y / 256.f});

    const float bgScrollSpeed = 100.f;

    while (window.isOpen())
    {
        sf::Time deltaTime = timer.restart();

        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
            else if (const sf::Event::KeyPressed* keyDown = event->getIf<sf::Event::KeyPressed>())
            {
                if (gameOver && keyDown->scancode == sf::Keyboard::Scancode::Space)
                {
                    // reset game after death
                    gameOver = false;
                    isDead = false;
                    hitWall = false;
                    stupid = false;
                    score = 0;
                    bird.reset();
                    walls.clear();
                    walls.push_back(Wall(60, window.getSize().y));
                    continue;
                }

                // menus
                if (mainMenu && keyDown->scancode == sf::Keyboard::Scancode::Space)
                {
                    mainMenu = false;
                    continue;
                }

                // unpause the game with either esc or P
                if (!gamePaused && !mainMenu && !gameOver &&
                        keyDown->scancode == sf::Keyboard::Scancode::Escape ||
                    !gamePaused && !mainMenu && !gameOver && keyDown->scancode == sf::Keyboard::Scancode::P)
                {
                    gamePaused = true;
                    continue;
                }
                // unpause the game with either esc or P
                else if (gamePaused && !mainMenu && !gameOver &&
                             keyDown->scancode == sf::Keyboard::Scancode::Escape ||
                         gamePaused && !mainMenu && !gameOver && keyDown->scancode == sf::Keyboard::Scancode::P)
                {
                    gamePaused = false;
                    continue;
                }

                // handleEvent directly receives the key instead of checking again with event in in bird.h
                else if (!gameOver && !mainMenu && !gamePaused && !hitWall)
                {
                    bird.handleEvent(keyDown);
                }
            }
        }

        window.clear();

        if (!gameOver && !mainMenu && !gamePaused)
        {
            // background scrolling effect
            float moveDistance = bgScrollSpeed * deltaTime.asSeconds();
            bg.move({-moveDistance, 0});
            if (bg.getPosition().x <= -screen_width)
            {
                bg.setPosition({0, 0});
            }

            window.draw(bg);

            // create walls
            for (Wall& wall : walls)
            {
                wallX = wall.getX();
                wall.update(deltaTime);
                wall.drawTo(window);

                if (wall.collision(bird.getShape()))
                {
                    bird.deathAnimation();
                    hitWall = true;

                    if (!isDead)
                    {
                        isDead = true;
                        hitSound.play();
                        currentTime = gameTimer.getElapsedTime().asSeconds();
                    }
                }
                if (isDead)
                {
                    if (!stupid)
                    {
                        if (gameTimer.getElapsedTime().asSeconds() - currentTime > 0.25)
                        {
                            dieSound.play();
                            stupid = true;
                        }
                    }

                    if (gameTimer.getElapsedTime().asSeconds() - currentTime > 1)
                    {
                        gameOver = true;
                    }
                }

                if (!isPassed && !hitWall && wall.getX() < bird.getPos().x &&
                    wall.getX() > bird.getPos().x - 20)
                {
                    passSound.play();
                    isPassed = true;
                    score++;

                    if (score % 10 == 0)
                    {
                        pointSound.play();
                    }
                }

                if (wall.getX() < -80)
                {
                    isPassed = false;
                }
            }
            spawnWalls(window, walls, wallX);
            // std::cout << walls.size() << std::endl;

            bird.update(deltaTime);
            bird.drawTo(window);

            walls.erase(std::remove_if(walls.begin(), walls.end(),
                                       [](const Wall& wall) { return wall.getX() < -80; }),
                        walls.end());

            sf::Vector2f playerPos = bird.getPos();

            if (playerPos.y >= window.getSize().y - 50)
            {
                bird.deathAnimation();
                if (!isDead)
                {
                    isDead = true;
                    hitSound.play();

                    currentTime = gameTimer.getElapsedTime().asSeconds();
                }
            }

            scoreText.setString("Score  " + std::to_string(score));
            window.draw(scoreText);

            highscoreText.setString("High Score  " + std::to_string(highscore));

            window.draw(highscoreText);

            window.display();
        }
        else if (gameOver && !mainMenu)
        {
            window.draw(bg);
            window.draw(gameOverText);

            if (score > highscore)
            {
                highscore = score;
                saveHighScore(highscore);
            }

            window.display();
        }
        else if (mainMenu && !gamePaused)
        {
            window.draw(bg);
            window.draw(mainMenuText);

            window.display();
        }
        else if (gamePaused)
        {
            window.draw(bg);
            window.draw(pausedText);

            window.display();
        }
    }

    return 0;
}
