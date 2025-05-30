#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include "bird.h"
#include "walls.h"

class Game
{
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
    void moveBackground(sf::Time deltaTime);
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
    sf::Font _font;
    sf::Texture _backgroundTexture;

    // Window and view
    sf::RenderWindow _window;
    sf::View _gameView;

    // Game objects
    Bird _bird;
    std::vector<Wall> walls;
    sf::Sprite _background;

    // UI elements
    sf::Text _gameOverText;
    sf::Text _mainMenuText;
    sf::Text _pausedText;
    sf::Text _scoreText;
    sf::Text _highscoreText;

    // Sounds
    sf::Sound _pointSound;
    sf::Sound _passSound;
    sf::Sound _hitSound;
    sf::Sound _dieSound;

    // Timers
    sf::Clock _gameTimer;
    sf::Clock _deltaTimer;

    // Game state
    bool _gameOver;
    bool _mainMenu;
    bool _gamePaused;
    bool _isPassed;
    bool _isDead;
    bool _hitWall;
    bool _deathSoundPlayed;
    int _score;
    int _highscore;
    float _wallX;
    float _currentTime;
};