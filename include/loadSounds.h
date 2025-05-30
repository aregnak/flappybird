#pragma once

#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Graphics/Texture.hpp>

// game texts
inline sf::Text createGameOverText(sf::Font& _font, const sf::RenderWindow& window)
{
    sf::Text gameOverText(_font, "Game Over!\nPress Space to Restart");
    gameOverText.setCharacterSize(40);
    gameOverText.setFillColor(sf::Color::White);
    gameOverText.setStyle(sf::Text::Bold);
    gameOverText.setPosition(
        { window.getSize().x / 2.f - gameOverText.getLocalBounds().size.x / 2,
          window.getSize().y / 2.f - gameOverText.getLocalBounds().size.y / 2 });
    gameOverText.setOutlineColor(sf::Color::Black);
    gameOverText.setOutlineThickness(5.f);
    return gameOverText;
}

inline sf::Text createMainMenuText(sf::Font& _font, const sf::RenderWindow& window)
{
    sf::Text mainMenuText(_font, "Flappy Bird\nPress Space to start");
    mainMenuText.setCharacterSize(40);
    mainMenuText.setFillColor(sf::Color::White);
    mainMenuText.setStyle(sf::Text::Bold);
    mainMenuText.setPosition(
        { window.getSize().x / 2.f - mainMenuText.getLocalBounds().size.x / 2,
          window.getSize().y / 2.f - mainMenuText.getLocalBounds().size.y / 2 });
    mainMenuText.setOutlineColor(sf::Color::Black);
    mainMenuText.setOutlineThickness(5.f);
    return mainMenuText;
}

inline sf::Text createPausedText(sf::Font& _font, const sf::RenderWindow& window)
{
    sf::Text pausedText(_font, "Game Paused\nPress Escape or P");
    pausedText.setCharacterSize(40);
    pausedText.setFillColor(sf::Color::White);
    pausedText.setStyle(sf::Text::Bold);
    pausedText.setPosition({ window.getSize().x / 2.f - pausedText.getLocalBounds().size.x / 2,
                             window.getSize().y / 2.f - pausedText.getLocalBounds().size.y / 2 });
    pausedText.setOutlineColor(sf::Color::Black);
    pausedText.setOutlineThickness(5.f);
    return pausedText;
}

inline sf::Text createScoreText(sf::Font& _font, const sf::RenderWindow& window)
{
    sf::Text scoreText(_font, "");
    scoreText.setCharacterSize(30);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition({ 10, 10 });
    scoreText.setOutlineThickness(5.f);
    return scoreText;
}

inline sf::Text createHighscoreText(sf::Font& _font, const sf::RenderWindow& window)
{
    sf::Text highscoreText(_font, "");
    highscoreText.setCharacterSize(30);
    highscoreText.setFillColor(sf::Color::White);
    highscoreText.setPosition({ 10, 50 });
    highscoreText.setOutlineColor(sf::Color::Black);
    highscoreText.setOutlineThickness(5.f);
    return highscoreText;
}

// game sounds
// buffer is created here to make main.cpp cleaner
inline sf::SoundBuffer pointBuffer("res/sound/sfx_point.wav");
inline sf::Sound createPointSound()
{
    sf::Sound pointSound(pointBuffer);
    pointSound.setVolume(40.f);
    return pointSound;
}

inline sf::SoundBuffer passBuffer("res/sound/sfx_swooshing.wav");
inline sf::Sound createPassSound()
{
    sf::Sound passSound(passBuffer);
    passSound.setVolume(100.f);
    return passSound;
}

inline sf::SoundBuffer hitBuffer("res/sound/sfx_hit.wav");
inline sf::Sound createHitSound()
{
    sf::Sound hitSound(hitBuffer);
    hitSound.setVolume(60.f);
    return hitSound;
}

inline sf::SoundBuffer dieBuffer("res/sound/sfx_die.wav");
inline sf::Sound createDieSound()
{
    sf::Sound dieSound(dieBuffer);
    dieSound.setVolume(40.f);
    return dieSound;
}