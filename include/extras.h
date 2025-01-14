#pragma once 

#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Graphics/Texture.hpp>

  
// game texts
sf::Text createGameOverText(sf::Font& font, const sf::RenderWindow& window)
{
    sf::Text gameOverText(font, "Game Over!\nPress Space to Restart");
    gameOverText.setCharacterSize(40);
    gameOverText.setFillColor(sf::Color::White);
    gameOverText.setStyle(sf::Text::Bold);
    gameOverText.setPosition({window.getSize().x / 2.f - gameOverText.getLocalBounds().size.x / 2,
                                window.getSize().y / 2.f - gameOverText.getLocalBounds().size.y / 2});
    gameOverText.setOutlineColor(sf::Color::Black);
    gameOverText.setOutlineThickness(5.f);
    return gameOverText;
}

sf::Text createMainMenuText(sf::Font& font, const sf::RenderWindow& window)
{
    sf::Text mainMenuText(font, "Flappy Bird\nPress Space to start");
    mainMenuText.setCharacterSize(40);
    mainMenuText.setFillColor(sf::Color::White);
    mainMenuText.setStyle(sf::Text::Bold);
    mainMenuText.setPosition({window.getSize().x / 2.f - mainMenuText.getLocalBounds().size.x / 2,
                             window.getSize().y / 2.f - mainMenuText.getLocalBounds().size.y / 2});
    mainMenuText.setOutlineColor(sf::Color::Black);
    mainMenuText.setOutlineThickness(5.f);
    return mainMenuText;
}

sf::Text createPausedText(sf::Font& font, const sf::RenderWindow& window)
{
    sf::Text pausedText(font, "Game Paused\nPress Escape or P");
    pausedText.setCharacterSize(40);
    pausedText.setFillColor(sf::Color::White);
    pausedText.setStyle(sf::Text::Bold);
    pausedText.setPosition({window.getSize().x / 2.f - pausedText.getLocalBounds().size.x / 2,
                           window.getSize().y / 2.f - pausedText.getLocalBounds().size.y/ 2});
    pausedText.setOutlineColor(sf::Color::Black);
    pausedText.setOutlineThickness(5.f);
    return pausedText;
}

sf::Text createScoreText(sf::Font& font, const sf::RenderWindow& window)
{
    sf::Text scoreText(font, "");
    scoreText.setCharacterSize(30);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition({10, 10});
    scoreText.setOutlineThickness(5.f);
    return scoreText;
}

sf::Text createHighscoreText(sf::Font& font, const sf::RenderWindow& window)
{
    sf::Text highscoreText(font, "");
    highscoreText.setCharacterSize(30);
    highscoreText.setFillColor(sf::Color::White);
    highscoreText.setPosition({10, 50});
    highscoreText.setOutlineColor(sf::Color::Black);
    highscoreText.setOutlineThickness(5.f);
    return highscoreText;
}

// game sounds
// buffer is created here to make main.cpp cleaner
sf::SoundBuffer pointBuffer("res/sound/sfx_point.wav");
sf::Sound createPointSound()
{
    sf::Sound pointSound(pointBuffer);
    pointSound.setVolume(40.f);
    return pointSound;
}    

sf::SoundBuffer passBuffer("res/sound/sfx_swooshing.wav");
sf::Sound createPassSound()
{
    sf::Sound passSound(passBuffer);
    passSound.setVolume(100.f);
    return passSound;
}

sf::SoundBuffer hitBuffer("res/sound/sfx_hit.wav");
sf::Sound createHitSound()
{
    sf::Sound hitSound(hitBuffer);
    hitSound.setVolume(60.f);
    return hitSound;
}

sf::SoundBuffer dieBuffer("res/sound/sfx_die.wav");
sf::Sound createDieSound()
{
    sf::Sound dieSound(dieBuffer);
    dieSound.setVolume(40.f);
    return dieSound;
}   

// sf::SoundBuffer jumpBuffer("res/sound/sfx_wing.wav");
// sf::Sound createJumpSound()
// {
//     sf::Sound jumpSound(jumpBuffer);
//     jumpSound.setVolume(60.f);
//     return jumpSound;
// }