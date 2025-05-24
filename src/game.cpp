#include "game.h"
#include "loadHS.h"
#include "loadSounds.h"
#include <cstdlib>
#include <ctime>
#include <algorithm>

Game::Game()
    : bird(45, 45)
    , background(backgroundTexture)
    , gameView(sf::FloatRect(
          { 0.f, 0.f }, { static_cast<float>(INITIAL_WIDTH), static_cast<float>(INITIAL_HEIGHT) }))
    , gameOverText(font, "")
    , mainMenuText(font, "")
    , pausedText(font, "")
    , scoreText(font, "")
    , highscoreText(font, "")
    , pointSound(createPointSound())
    , passSound(createPassSound())
    , hitSound(createHitSound())
    , dieSound(createDieSound())
    , gameOver(false)
    , mainMenu(true)
    , gamePaused(false)
    , isPassed(false)
    , isDead(false)
    , hitWall(false)
    , deathSoundPlayed(false)
    , score(0)
    , highscore(0)
    , wallX(0)
    , currentTime(0)
{
    // Initialize random seed
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    // Create the window
    window.create(sf::VideoMode({ INITIAL_WIDTH, INITIAL_HEIGHT }), "Flappy Bird",
                  sf::Style::Default);
    window.setVerticalSyncEnabled(true);
    window.setKeyRepeatEnabled(false);

    // Load resources first
    loadResources();

    // Now initialize view and game objects
    updateView();
    initializeGameObjects();
}

void Game::run()
{
    while (window.isOpen())
    {
        sf::Time deltaTime = deltaTimer.restart();
        processEvents();
        update(deltaTime);
        render();
    }
}

void Game::processEvents()
{
    while (const std::optional event = window.pollEvent())
    {
        if (event->is<sf::Event::Closed>())
        {
            window.close();
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
    if (gameOver && keyEvent->scancode == sf::Keyboard::Scancode::Space)
    {
        resetGame();
    }
    else if (mainMenu && keyEvent->scancode == sf::Keyboard::Scancode::Space)
    {
        mainMenu = false;
    }
    else if (!gamePaused && !mainMenu && !gameOver &&
             (keyEvent->scancode == sf::Keyboard::Scancode::Escape ||
              keyEvent->scancode == sf::Keyboard::Scancode::P))
    {
        gamePaused = true;
    }
    else if (gamePaused && !mainMenu && !gameOver &&
             (keyEvent->scancode == sf::Keyboard::Scancode::Escape ||
              keyEvent->scancode == sf::Keyboard::Scancode::P))
    {
        gamePaused = false;
    }
    else if (!gameOver && !mainMenu && !gamePaused && !hitWall)
    {
        bird.handleEvent(keyEvent);
    }
}

void Game::update(sf::Time deltaTime)
{
    if (!gameOver && !mainMenu && !gamePaused)
    {
        moveBackground(deltaTime);
        // Update walls
        for (Wall& wall : walls)
        {
            wallX = wall.getX();
            wall.update(deltaTime);

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

        // Spawn new walls
        spawnWalls();

        // Update bird
        bird.update(deltaTime);

        // Clean up off-screen walls
        walls.erase(std::remove_if(walls.begin(), walls.end(),
                                   [](const Wall& wall) { return wall.getX() < -80; }),
                    walls.end());

        // Check for ground collision
        if (bird.getPos().y >= INITIAL_HEIGHT - 50)
        {
            bird.deathAnimation();
            if (!isDead)
            {
                isDead = true;
                hitSound.play();
                currentTime = gameTimer.getElapsedTime().asSeconds();
            }
        }

        // Handle death sequence
        if (isDead)
        {
            if (!deathSoundPlayed)
            {
                if (gameTimer.getElapsedTime().asSeconds() - currentTime > 0.25)
                {
                    dieSound.play();
                    deathSoundPlayed = true;
                }
            }

            if (gameTimer.getElapsedTime().asSeconds() - currentTime > 1)
            {
                gameOver = true;
            }
        }

        // Update score text
        scoreText.setString("Score  " + std::to_string(score));
        highscoreText.setString("High Score  " + std::to_string(highscore));
    }
    else if (gameOver && !mainMenu)
    {
        if (score > highscore)
        {
            highscore = score;
            saveHighScore(highscore);
        }
    }
}

void Game::render()
{
    window.clear();
    window.setView(gameView);

    window.draw(background);

    if (!gameOver && !mainMenu && !gamePaused)
    {
        for (const auto& wall : walls)
        {
            wall.drawTo(window);
        }
        bird.drawTo(window);
        window.draw(scoreText);
        window.draw(highscoreText);
    }
    else if (gameOver && !mainMenu)
    {
        window.draw(gameOverText);
    }
    else if (mainMenu && !gamePaused)
    {
        window.draw(mainMenuText);
    }
    else if (gamePaused)
    {
        window.draw(pausedText);
    }

    window.display();
}

void Game::updateView()
{
    float windowWidth = window.getSize().x;
    float windowHeight = window.getSize().y;
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

    gameView.setSize(sf::Vector2f(viewWidth, viewHeight));
    gameView.setCenter(sf::Vector2f(INITIAL_WIDTH / 2.f, INITIAL_HEIGHT / 2.f));
    window.setView(gameView);
}

void Game::spawnWalls()
{
    if (walls.empty() || window.getSize().x - wallX >= WALL_GAP)
    {
        float wallY =
            (rand() % (600 - 200 + 1) + 200); // random position between 600 and 200 pixels
        walls.push_back(Wall(window.getSize().x, wallY));
        walls.push_back(Wall(window.getSize().x, wallY - 880)); // second wall
        wallX = window.getSize().x;
    }
}

void Game::moveBackground(sf::Time deltaTime)
{
    float moveDistance = BG_SCROLL_SPEED * deltaTime.asSeconds();
    background.move({ -moveDistance, 0 });
    if (background.getPosition().x <= -INITIAL_WIDTH)
    {
        background.setPosition({ 0, 0 });
    }
}

void Game::resetGame()
{
    gameOver = false;
    isDead = false;
    hitWall = false;
    deathSoundPlayed = false;
    score = 0;
    bird.reset();
    walls.clear();
    walls.push_back(Wall(60, window.getSize().y));
}

void Game::initializeGameObjects()
{
    bird.reset();
    walls.reserve(6);
}

void Game::loadResources()
{
    // Load font
    if (!font.openFromFile("res/font/Neon.ttf"))
    {
        std::cerr << "Failed to load font";
    }

    // Load background texture
    if (!backgroundTexture.loadFromFile("res/sprite/Background/Background7.png"))
    {
        std::cerr << "Failed to load background texture";
    }
    backgroundTexture.setRepeated(true);

    background.setTextureRect(
        sf::IntRect({ 0, 0 }, { (int)window.getSize().x - ((int)window.getSize().x / 3),
                                (int)window.getSize().y - ((int)window.getSize().y / 2) }));
    background.setScale({ window.getSize().x / 256.f, window.getSize().y / 256.f });

    // Setup text elements
    gameOverText.setString("Game Over!\nPress Space to Restart");
    gameOverText.setCharacterSize(50);
    gameOverText.setFillColor(sf::Color::White);
    gameOverText.setOutlineColor(sf::Color::Black);
    gameOverText.setOutlineThickness(3);
    gameOverText.setOrigin(sf::Vector2f(gameOverText.getLocalBounds().size.x / 2,
                                        gameOverText.getLocalBounds().size.y / 2));
    gameOverText.setPosition(sf::Vector2f(INITIAL_WIDTH / 2.f, INITIAL_HEIGHT / 2.f));

    mainMenuText.setString("Press Space to Start");
    mainMenuText.setCharacterSize(50);
    mainMenuText.setFillColor(sf::Color::White);
    mainMenuText.setOutlineColor(sf::Color::Black);
    mainMenuText.setOutlineThickness(3);
    mainMenuText.setOrigin(sf::Vector2f(mainMenuText.getLocalBounds().size.x / 2,
                                        mainMenuText.getLocalBounds().size.y / 2));
    mainMenuText.setPosition(sf::Vector2f(INITIAL_WIDTH / 2.f, INITIAL_HEIGHT / 2.f));

    pausedText.setString("Paused\nPress P or Esc to Resume");
    pausedText.setCharacterSize(50);
    pausedText.setFillColor(sf::Color::White);
    pausedText.setOutlineColor(sf::Color::Black);
    pausedText.setOutlineThickness(3);
    pausedText.setOrigin(sf::Vector2f(pausedText.getLocalBounds().size.x / 2,
                                      pausedText.getLocalBounds().size.y / 2));
    pausedText.setPosition(sf::Vector2f(INITIAL_WIDTH / 2.f, INITIAL_HEIGHT / 2.f));

    scoreText.setCharacterSize(40);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setOutlineColor(sf::Color::Black);
    scoreText.setOutlineThickness(3);
    scoreText.setPosition(sf::Vector2f(20, 20));

    highscoreText.setCharacterSize(40);
    highscoreText.setFillColor(sf::Color::White);
    highscoreText.setOutlineColor(sf::Color::Black);
    highscoreText.setOutlineThickness(3);
    highscoreText.setPosition(sf::Vector2f(20, 70));
}