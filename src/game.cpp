#include "game.h"
#include "loadHS.h"
#include "loadSounds.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <algorithm>

Game::Game()
    : _bird(45, 45)
    , _background(_backgroundTexture)
    , _gameView(sf::FloatRect(
          { 0.f, 0.f }, { static_cast<float>(INITIAL_WIDTH), static_cast<float>(INITIAL_HEIGHT) }))
    , _gameOverText(_font, "")
    , _mainMenuText(_font, "")
    , _pausedText(_font, "")
    , _scoreText(_font, "")
    , _highscoreText(_font, "")
    , _pointSound(createPointSound())
    , _passSound(createPassSound())
    , _hitSound(createHitSound())
    , _dieSound(createDieSound())
    , _gameOver(false)
    , _mainMenu(true)
    , _gamePaused(false)
    , _isPassed(false)
    , _isDead(false)
    , _hitWall(false)
    , _deathSoundPlayed(false)
    , _score(0)
    , _highscore(loadHighScore())
    , _wallX(0)
    , _currentTime(0)
{
    // Initialize random seed
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    // Create the window
    _window.create(sf::VideoMode({ INITIAL_WIDTH, INITIAL_HEIGHT }), "Flappy Bird",
                   sf::Style::Default);
    _window.setVerticalSyncEnabled(true);
    _window.setKeyRepeatEnabled(false);

    // Load resources first
    loadResources();

    // Now initialize view and game objects
    updateView();
    initializeGameObjects();
}

void Game::run()
{
    while (_window.isOpen())
    {
        sf::Time deltaTime = _deltaTimer.restart();
        processEvents();
        update(deltaTime);
        render();
    }
}

void Game::processEvents()
{
    while (const std::optional event = _window.pollEvent())
    {
        if (event->is<sf::Event::Closed>())
        {
            _window.close();
        }
        else if (const sf::Event::Resized* resizeEvent = event->getIf<sf::Event::Resized>())
        {
            updateView();
        }
        else if (const sf::Event::KeyPressed* keyEvent = event->getIf<sf::Event::KeyPressed>())
        {
            handlePlayerInput(keyEvent);
        }
    }
}

void Game::handlePlayerInput(const sf::Event::KeyPressed* keyEvent)
{
    if (_gameOver && keyEvent->scancode == sf::Keyboard::Scancode::Space)
    {
        resetGame();
    }
    else if (_mainMenu && keyEvent->scancode == sf::Keyboard::Scancode::Space)
    {
        _mainMenu = false;
    }
    else if (!_gamePaused && !_mainMenu && !_gameOver &&
             (keyEvent->scancode == sf::Keyboard::Scancode::Escape ||
              keyEvent->scancode == sf::Keyboard::Scancode::P))
    {
        _gamePaused = true;
    }
    else if (_gamePaused && !_mainMenu && !_gameOver &&
             (keyEvent->scancode == sf::Keyboard::Scancode::Escape ||
              keyEvent->scancode == sf::Keyboard::Scancode::P))
    {
        _gamePaused = false;
    }
    else if (!_gameOver && !_mainMenu && !_gamePaused && !_hitWall)
    {
        _bird.handleEvent(keyEvent);
    }
}

void Game::update(sf::Time deltaTime)
{
    if (!_gameOver && !_mainMenu && !_gamePaused)
    {
        moveBackground(deltaTime);
        // Update walls
        for (Wall& wall : walls)
        {
            _wallX = wall.getX();
            wall.update(deltaTime);

            if (wall.collision(_bird.getShape()))
            {
                _bird.deathAnimation();
                _hitWall = true;

                if (!_isDead)
                {
                    _isDead = true;
                    _hitSound.play();
                    _currentTime = _gameTimer.getElapsedTime().asSeconds();
                }
            }

            if (!_isPassed && !_hitWall && wall.getX() < _bird.getPos().x &&
                wall.getX() > _bird.getPos().x - 20)
            {
                _passSound.play();
                _isPassed = true;
                _score++;

                if (_score % 10 == 0)
                {
                    _pointSound.play();
                }
            }

            if (wall.getX() < -80)
            {
                _isPassed = false;
            }
        }

        // Spawn new walls
        spawnWalls();

        // Update bird
        _bird.update(deltaTime);

        // Clean up off-screen walls
        walls.erase(std::remove_if(walls.begin(), walls.end(),
                                   [](const Wall& wall) { return wall.getX() < -80; }),
                    walls.end());

        // Check for ground collision
        if (_bird.getPos().y >= INITIAL_HEIGHT - 50)
        {
            _bird.deathAnimation();
            if (!_isDead)
            {
                _isDead = true;
                _hitSound.play();
                _currentTime = _gameTimer.getElapsedTime().asSeconds();
            }
        }

        // Handle death sequence
        if (_isDead)
        {
            if (!_deathSoundPlayed)
            {
                if (_gameTimer.getElapsedTime().asSeconds() - _currentTime > 0.25)
                {
                    _dieSound.play();
                    _deathSoundPlayed = true;
                }
            }

            if (_gameTimer.getElapsedTime().asSeconds() - _currentTime > 1)
            {
                _gameOver = true;
            }
        }

        // Update score text
        _scoreText.setString("Score  " + std::to_string(_score));
        _highscoreText.setString("High Score  " + std::to_string(_highscore));
    }
    else if (_gameOver && !_mainMenu)
    {
        if (_score > _highscore)
        {
            _highscore = _score;
            saveHighScore(_highscore);
        }
    }
}

void Game::render()
{
    _window.clear();
    _window.setView(_gameView);

    _window.draw(_background);

    if (!_gameOver && !_mainMenu && !_gamePaused)
    {
        for (const auto& wall : walls)
        {
            wall.drawTo(_window);
        }
        _bird.drawTo(_window);
        _window.draw(_scoreText);
        _window.draw(_highscoreText);
    }
    else if (_gameOver && !_mainMenu)
    {
        _window.draw(_gameOverText);
    }
    else if (_mainMenu && !_gamePaused)
    {
        _window.draw(_mainMenuText);
    }
    else if (_gamePaused)
    {
        _window.draw(_pausedText);
    }

    _window.display();
}

void Game::updateView()
{
    float windowWidth = _window.getSize().x;
    float windowHeight = _window.getSize().y;
    float viewWidth;
    float viewHeight;

    if (windowWidth / windowHeight > ASPECT_RATIO)
    {
        viewHeight = INITIAL_HEIGHT;
        viewWidth = viewHeight * (windowWidth / windowHeight);
    }
    else
    {
        viewWidth = INITIAL_WIDTH;
        viewHeight = viewWidth / (windowWidth / windowHeight);
    }

    _gameView.setSize(sf::Vector2f(viewWidth, viewHeight));
    _gameView.setCenter(sf::Vector2f(INITIAL_WIDTH / 2.f, INITIAL_HEIGHT / 2.f));
    _window.setView(_gameView);
}

void Game::spawnWalls()
{
    if (walls.empty() || _window.getSize().x - _wallX >= WALL_GAP)
    {
        float wallY =
            (rand() % (600 - 200 + 1) + 200); // random position between 600 and 200 pixels
        walls.push_back(Wall(_window.getSize().x, wallY));
        walls.push_back(Wall(_window.getSize().x, wallY - 880)); // second wall
        _wallX = _window.getSize().x;
    }
}

void Game::moveBackground(sf::Time deltaTime)
{
    float moveDistance = BG_SCROLL_SPEED * deltaTime.asSeconds();
    _background.move({ -moveDistance, 0 });
    if (_background.getPosition().x <= -INITIAL_WIDTH)
    {
        _background.setPosition({ 0, 0 });
    }
}

void Game::resetGame()
{
    _gameOver = false;
    _isDead = false;
    _hitWall = false;
    _deathSoundPlayed = false;
    _score = 0;
    _bird.reset();
    walls.clear();
    if (_window.getSize().x > 1000)
    {
        float wallY =
            (rand() % (600 - 200 + 1) + 200); // random position between 600 and 200 pixels
        walls.push_back(Wall(static_cast<float>(_window.getSize().x) - WALL_GAP - 10.f, wallY));
        walls.push_back(Wall(static_cast<float>(_window.getSize().x) - WALL_GAP - 10.f,
                             wallY - 880)); // second wall
    }
    spawnWalls();
}

void Game::initializeGameObjects()
{
    _bird.reset();
    walls.reserve(6);
}

void Game::loadResources()
{
    // Load _font
    if (!_font.openFromFile("res/font/Neon.ttf"))
    {
        std::cerr << "Failed to load font";
    }

    // Load background texture
    if (!_backgroundTexture.loadFromFile("res/sprite/Background/Background7.png"))
    {
        std::cerr << "Failed to load background texture";
    }
    _backgroundTexture.setRepeated(true);

    _background.setTextureRect(
        sf::IntRect({ 0, 0 }, { (int)_window.getSize().x - ((int)_window.getSize().x / 3),
                                (int)_window.getSize().y - ((int)_window.getSize().y / 2) }));
    _background.setScale({ _window.getSize().x / 256.f, _window.getSize().y / 256.f });

    // Setup text elements
    _gameOverText.setString("Game Over!\nPress Space to Restart");
    _gameOverText.setCharacterSize(50);
    _gameOverText.setFillColor(sf::Color::White);
    _gameOverText.setOutlineColor(sf::Color::Black);
    _gameOverText.setOutlineThickness(3);
    _gameOverText.setOrigin(sf::Vector2f(_gameOverText.getLocalBounds().size.x / 2,
                                         _gameOverText.getLocalBounds().size.y / 2));
    _gameOverText.setPosition(sf::Vector2f(INITIAL_WIDTH / 2.f, INITIAL_HEIGHT / 2.f));

    _mainMenuText.setString("Press Space to Start");
    _mainMenuText.setCharacterSize(50);
    _mainMenuText.setFillColor(sf::Color::White);
    _mainMenuText.setOutlineColor(sf::Color::Black);
    _mainMenuText.setOutlineThickness(3);
    _mainMenuText.setOrigin(sf::Vector2f(_mainMenuText.getLocalBounds().size.x / 2,
                                         _mainMenuText.getLocalBounds().size.y / 2));
    _mainMenuText.setPosition(sf::Vector2f(INITIAL_WIDTH / 2.f, INITIAL_HEIGHT / 2.f));

    _pausedText.setString("Paused\nPress P or Esc to Resume");
    _pausedText.setCharacterSize(50);
    _pausedText.setFillColor(sf::Color::White);
    _pausedText.setOutlineColor(sf::Color::Black);
    _pausedText.setOutlineThickness(3);
    _pausedText.setOrigin(sf::Vector2f(_pausedText.getLocalBounds().size.x / 2,
                                       _pausedText.getLocalBounds().size.y / 2));
    _pausedText.setPosition(sf::Vector2f(INITIAL_WIDTH / 2.f, INITIAL_HEIGHT / 2.f));

    _scoreText.setCharacterSize(40);
    _scoreText.setFillColor(sf::Color::White);
    _scoreText.setOutlineColor(sf::Color::Black);
    _scoreText.setOutlineThickness(3);
    _scoreText.setPosition(sf::Vector2f(20, 20));

    _highscoreText.setCharacterSize(40);
    _highscoreText.setFillColor(sf::Color::White);
    _highscoreText.setOutlineColor(sf::Color::Black);
    _highscoreText.setOutlineThickness(3);
    _highscoreText.setPosition(sf::Vector2f(20, 70));
}