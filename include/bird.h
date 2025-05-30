#pragma once

#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>

#include <iostream>

class Bird
{
public:
    Bird(float x, float y);

    void handleEvent(const sf::Event::KeyPressed* keyDown);
    void update(sf::Time deltaTime);
    void reset();
    void deathAnimation();

    // inline functions
    void drawTo(sf::RenderWindow& _window)
    {
        _window.draw(_bird);
        //_window.draw(hitbox);
    }

    sf::Vector2f getPos()
    {
        return _bird.getPosition(); //
    }

    const sf::CircleShape getShape()
    {
        return _hitbox; //
    }

private:
    // bird shape and hitbox
    sf::RectangleShape _bird;
    sf::CircleShape _hitbox;

    // textures
    sf::Texture _texture;
    sf::IntRect _birdTextRect;

    // sounds
    sf::SoundBuffer _jumpBuffer;
    sf::Sound _jumpSound;

    // delta time dependencies
    sf::Clock _clock;
    sf::Clock _jumpClock;
    sf::Time _jumpCooldown;

    // bird's parameters
    float _gravity;
    float _jump;
    float _velocity;
    float _terminalVelocity;
    float _rotate;
    int _birdtexrectTop;
    bool _canJump;
};
