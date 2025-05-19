#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include "bird.h"
#include "walls.h"

class Game {
public:
    Game();
    ~Game() = default;

    void run();

private:
    void processEvents();
    void update(sf::Time deltaTime);
    void render();
    void handlePlayerInput(const sf::Event::KeyPressed* keyEvent);
    void updateView();
    void spawnWalls();
    void resetGame();
    void loadResources();
    void initializeGameObjects();

    // Constants
    static constexpr int INITIAL_WIDTH = 800;
    static constexpr int INITIAL_HEIGHT = 800;
    static constexpr float ASPECT_RATIO = float(INITIAL_WIDTH) / INITIAL_HEIGHT;
    static constexpr float WALL_GAP = 400.f;
    static constexpr float BG_SCROLL_SPEED = 100.f;

    // Resources (must be declared before objects that use them)
    sf::Font font;
    sf::Texture backgroundTexture;

    // Window and view
    sf::RenderWindow window;
    sf::View gameView;

    // Game objects
    Bird bird;
    std::vector<Wall> walls;
    sf::Sprite background;

    // UI elements
    sf::Text gameOverText;
    sf::Text mainMenuText;
    sf::Text pausedText;
    sf::Text scoreText;
    sf::Text highscoreText;

    // Sounds
    sf::Sound pointSound;
    sf::Sound passSound;
    sf::Sound hitSound;
    sf::Sound dieSound;

    // Timers
    sf::Clock gameTimer;
    sf::Clock deltaTimer;

    // Game state
    bool gameOver;
    bool mainMenu;
    bool gamePaused;
    bool isPassed;
    bool isDead;
    bool hitWall;
    bool deathSoundPlayed;
    int score;
    int highscore;
    float wallX;
    float currentTime;
}; 