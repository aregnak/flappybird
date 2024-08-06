#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Clock.hpp>
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
        , _jumpCooldown(sf::seconds(0.15f))
        , _canJump(true)
        , _rotate(0.f)
    {
        bird.setSize(sf::Vector2f(x, y));
        bird.setOrigin(x / 2, y / 2);
        bird.setPosition(100, 300);
        bird.setRotation(0);

        if (!texture.loadFromFile("res/sprite/Player/StyleBird1/Bird1-6.png"))
        {
            std::cout << "failed to load bird texture" << std::endl;
            system("pause");
        }
        birdTextRect = sf::IntRect(0, 0, 16, 16);
        bird.setTextureRect(birdTextRect);
        bird.setTexture(&texture);
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
        if (deltaTime == sf::Time::Zero)
        {
            return;
        }

        if (!_canJump && _jumpClock.getElapsedTime() > _jumpCooldown)
        {
            _canJump = true;
        }

        if (!_canJump)
        {
            if (clock.getElapsedTime().asSeconds() > 0.0375f)
            {
                if (birdTextRect.left == 48)
                {
                    birdTextRect.left = 0;
                }
                else
                {
                    birdTextRect.left += 16;
                }
                clock.restart();
            }
            bird.setTextureRect(birdTextRect);
        }
        else
        {
            birdTextRect = sf::IntRect(0, 0, 16, 16);
            bird.setTextureRect(birdTextRect);
        }

        _rotate = _velocity / 600.f;

        _velocity += _gravity * deltaTime.asSeconds();

        if (_velocity > _terminalVelocity)
        {
            _velocity = _terminalVelocity;
        }

        bird.move(0, _velocity * deltaTime.asSeconds());
        bird.rotate(_rotate);
    }

    void reset()
    {
        _velocity = 0.f;
        _rotate = 0.f;
        _canJump = true;
        bird.setPosition(sf::Vector2f(100, 300));
        bird.setRotation(0);
    }

    void drawTo(sf::RenderWindow& window)
    {
        window.draw(bird); //
    }

    sf::Vector2f getPos()
    {
        return bird.getPosition(); //
    }

    sf::RectangleShape getShape() const
    {
        return bird; //
    }

    void deathAnimation()
    {
        bird.rotate(-15);

        constexpr float gravityEffect = 0.001;
        for (int i = 0; i < 5; ++i)
        {
            sf::Vector2f movement(i, gravityEffect * i * i);
            // Add boundary checks here to ensure bird stays within game area
            if (bird.getPosition().x - movement.x >= 0 && bird.getPosition().y + movement.y >= 0)
            {
                bird.move(-movement.x, movement.y);
            }
        }
    }

private:
    sf::RectangleShape bird;
    sf::Texture texture;
    sf::IntRect birdTextRect;

    float _gravity;
    float _jump;
    float _velocity;
    float _terminalVelocity;
    float _rotate;

    sf::Clock clock;
    sf::Clock _jumpClock;
    sf::Time _jumpCooldown;
    bool _canJump;
};
