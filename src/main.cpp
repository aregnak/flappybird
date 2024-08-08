#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Audio.hpp>
#include <algorithm>
#include <string>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>

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

void saveHighScore(int highscore)
{
    std::ofstream outFile("save/highscore.txt");
    if (outFile.is_open())
    {
        outFile << highscore;
        outFile.close();
    }
}

int loadHighScore()
{
    std::ifstream inFile("save/highscore.txt"); //
    int highscore = 0;
    if (inFile.is_open())
    {
        inFile >> highscore;
        inFile.close();
    }

    return highscore;
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

    Bird bird(45, 45);
    std::vector<Wall> walls;

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
    gameOverText.setString("Game Over!\nPress Space to Restart");
    gameOverText.setCharacterSize(50);
    gameOverText.setFillColor(sf::Color::White);
    gameOverText.setStyle(sf::Text::Bold);
    gameOverText.setPosition(window.getSize().x / 2.f - gameOverText.getLocalBounds().width / 2,
                             window.getSize().y / 2.f - gameOverText.getLocalBounds().height / 2);
    gameOverText.setOutlineColor(sf::Color::Black);
    gameOverText.setOutlineThickness(5.f);

    sf::Text mainMenuText;
    mainMenuText.setFont(font);
    mainMenuText.setString("Flappy Bird\nPress Space to start");
    mainMenuText.setCharacterSize(50);
    mainMenuText.setFillColor(sf::Color::White);
    mainMenuText.setStyle(sf::Text::Bold);
    mainMenuText.setPosition(window.getSize().x / 2.f - mainMenuText.getLocalBounds().width / 2,
                             window.getSize().y / 2.f - mainMenuText.getLocalBounds().height / 2);
    mainMenuText.setOutlineColor(sf::Color::Black);
    mainMenuText.setOutlineThickness(5.f);

    sf::Text pausedText;
    pausedText.setFont(font);
    pausedText.setString("Game Paused\nPress Escape or P");
    pausedText.setCharacterSize(50);
    pausedText.setFillColor(sf::Color::White);
    pausedText.setStyle(sf::Text::Bold);
    pausedText.setPosition(window.getSize().x / 2.f - mainMenuText.getLocalBounds().width / 2,
                           window.getSize().y / 2.f - pausedText.getLocalBounds().height / 2);
    pausedText.setOutlineColor(sf::Color::Black);
    pausedText.setOutlineThickness(5.f);

    sf::Text scoreText;
    scoreText.setFont(font);
    scoreText.setCharacterSize(30);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(10, 10);
    scoreText.setOutlineColor(sf::Color::Black);
    scoreText.setOutlineThickness(5.f);

    sf::Text highscoreText;
    highscoreText.setFont(font);
    highscoreText.setCharacterSize(30);
    highscoreText.setFillColor(sf::Color::White);
    highscoreText.setPosition(10, 50);
    highscoreText.setOutlineColor(sf::Color::Black);
    highscoreText.setOutlineThickness(5.f);

    sf::SoundBuffer pointBuffer;
    pointBuffer.loadFromFile("res/sound/sfx_point.wav");
    sf::Sound pointSound(pointBuffer);
    pointSound.setVolume(40.f);

    sf::SoundBuffer passBuffer;
    passBuffer.loadFromFile("res/sound/sfx_swooshing.wav");
    sf::Sound passSound(passBuffer);
    passSound.setVolume(100.f);

    sf::SoundBuffer hitBuffer;
    hitBuffer.loadFromFile("res/sound/sfx_hit.wav");
    sf::Sound hitSound(hitBuffer);
    hitSound.setVolume(60.f);

    sf::SoundBuffer dieBuffer;
    dieBuffer.loadFromFile("res/sound/sfx_die.wav");
    sf::Sound dieSound(dieBuffer);
    dieSound.setVolume(40.f);

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
                if (gameOver && event.key.code == sf::Keyboard::Space)
                {
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

                if (mainMenu && event.key.code == sf::Keyboard::Space)
                {
                    mainMenu = false;
                    continue;
                }

                if (!gamePaused && !mainMenu && !gameOver &&
                        event.key.code == sf::Keyboard::Escape ||
                    !gamePaused && !mainMenu && !gameOver && event.key.code == sf::Keyboard::P)
                {
                    gamePaused = true;
                    continue;
                }
                else if (gamePaused && !mainMenu && !gameOver &&
                             event.key.code == sf::Keyboard::Escape ||
                         gamePaused && !mainMenu && !gameOver && event.key.code == sf::Keyboard::P)
                {
                    gamePaused = false;
                    continue;
                }

                else if (!gameOver && !mainMenu && !gamePaused && !hitWall)
                {
                    bird.handleEvent(event);
                }
            }
        }

        window.clear();

        if (!gameOver && !mainMenu && !gamePaused)
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
                if (hitWall)
                {
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