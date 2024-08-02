#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include <iostream>

class Bird
{
public:
    Bird(float x, float y)
        : _gravity(1800.f)
        , _jump(-600.f)
        , _velocity(0.f)
        , _terminalVelocity(600.f)
        , _jumpCooldown(sf::seconds(0.1f))
        , _canJump(true)
    {
        bird.setSize(sf::Vector2f(x, y));
        bird.setOrigin(x / 2, y / 2);
        bird.setPosition(sf::Vector2f(100, 300));
    }

    void handleEvent(const sf::Event& event)
    {
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space &&
            _canJump)
        {
            _velocity = _jump;
            _canJump = false;
            _jumpClock.restart();
        }
    }

    void update(sf::Time deltaTime)
    {
        if (!_canJump && _jumpClock.getElapsedTime() > _jumpCooldown)
        {
            _canJump = true;
        }

        _velocity += _gravity * deltaTime.asSeconds();

        if (_velocity > _terminalVelocity)
        {
            _velocity = _terminalVelocity;
        }

        if (bird.getPosition().y >= 750)
        {
            //_velocity = 0;
        }

        bird.move(0, _velocity * deltaTime.asSeconds());

        // std::cout << "Velocity: " << bird.getPosition().y << std::endl;
    }

    void reset()
    {
        _velocity = 0.f;
        _canJump = true;
        bird.setPosition(sf::Vector2f(100, 300));
    }

    void drawTo(sf::RenderWindow& window) { window.draw(bird); }

    sf::Vector2f getPos() { return bird.getPosition(); }

private:
    sf::RectangleShape bird;
    float _gravity;
    float _jump;
    float _velocity;
    float _terminalVelocity;

    sf::Clock _jumpClock;
    sf::Time _jumpCooldown;
    bool _canJump;
};
