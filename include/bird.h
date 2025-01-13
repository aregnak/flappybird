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
    Bird(float x, float y)
        : _gravity(1800.f)
        , _jump(-650.f)
        , _velocity(0.f)
        , _terminalVelocity(600.f)
        , _jumpCooldown(sf::seconds(0.1f))
        , _canJump(true)
        , _rotate(0.f)
        , _birdtexrectTop(0)
    {
        // bird player init
        bird.setSize(sf::Vector2f(x, y));
        bird.setOrigin({x / 2, y / 2});
        bird.setPosition({200, 300});
        bird.setRotation(sf::degrees(90));

        // hitbox init
        hitbox.setRadius((x / 3));
        hitbox.setOrigin({hitbox.getRadius(), hitbox.getRadius()});
        hitbox.setPosition(bird.getPosition());
        hitbox.setRotation(sf::degrees(bird.getRotation().asDegrees()));

        // bird texture with animation, basically the spritesheet.
        if (!texture.loadFromFile("res/sprite/Player/StyleBird1/AllBird1.png"))
        {
            std::cout << "failed to load bird texture" << std::endl;
            system("pause");
        }
        birdTextRect = sf::IntRect({0, 0}, {16, 16});
        bird.setTextureRect(birdTextRect);
        bird.setTexture(&texture);

        if (!jumpBuffer.loadFromFile("res/sound/sfx_wing.wav"))
        {
            std::cerr << "didnt load jump sound" << std::endl;
        }
        jumpSound.setBuffer(jumpBuffer);  // Set the buffer to jumpSound
    }

    void handleEvent(const auto* keyDown)
    {
        // because of sf::Event changes, the getIf needs to be true, then check for space key
        //if (const auto* keyDown = event.getIf<sf::Event::KeyPressed>())
        {    
            if (keyDown->scancode == sf::Keyboard::Scancode::Space &&
            _canJump)
            {
                jumpSound.play();
                _velocity = _jump;
                _canJump = false;
                _jumpClock.restart();
            }
        if (keyDown->scancode == sf::Keyboard::Scancode::G)
            {
                if (_birdtexrectTop <= 80)
                {
                    _birdtexrectTop += 16;
                }
                else
                {
                    _birdtexrectTop = 0;
                }
            }
        }

    }

    void update(sf::Time deltaTime)
    {
        if (deltaTime == sf::Time::Zero)
        {
            return;
        }

        if (!_canJump)
        {
            if (_jumpClock.getElapsedTime() > _jumpCooldown)
            {
                _canJump = true;
            }

            if (clock.getElapsedTime().asSeconds() > 0.025f)
            {
                if (birdTextRect.position == sf::Vector2i(48, 0))
                {
                    birdTextRect.position = sf::Vector2i(0, 0);
                }
                else
                {
                    birdTextRect.position += sf::Vector2i(16, 0);
                }
                clock.restart();
            }
            bird.setTextureRect(birdTextRect);
        }
        else
        {
            birdTextRect = sf::IntRect({0, _birdtexrectTop}, {16, 16});
            bird.setTextureRect(birdTextRect);
        }

        if (bird.getPosition().y < 10)
        {
            bird.setPosition({bird.getPosition().x, 10});
            hitbox.setPosition(bird.getPosition());
        }

        _rotate = _velocity / 2.5f;

        _velocity += _gravity * deltaTime.asSeconds();

        if (_velocity > _terminalVelocity)
        {
            _velocity = _terminalVelocity;
        }

        if (bird.getRotation() < sf::degrees(320) && bird.getRotation() > sf::degrees(290))
        {
            bird.setRotation(sf::degrees(320));
        }

        if (bird.getRotation() > sf::degrees(40) && bird.getRotation() < sf::degrees(90))
        {
            bird.setRotation(sf::degrees(40));
        }

        hitbox.move({0.f, _velocity * deltaTime.asSeconds()});

        bird.move({0.f, _velocity * deltaTime.asSeconds()});
        bird.rotate(sf::degrees(_rotate * deltaTime.asSeconds()));
    }

    void reset()
    {
        _velocity = 0.f;
        _rotate = 0.f;
        _canJump = true;
        bird.setPosition(sf::Vector2f(200, 300));
        bird.setRotation(sf::degrees(0));
        hitbox.setPosition(bird.getPosition());
        hitbox.setRotation(sf::degrees(bird.getRotation().asDegrees()));
    }

    void drawTo(sf::RenderWindow& window)
    {
        window.draw(bird);
        //window.draw(hitbox);
    }

    sf::Vector2f getPos()
    {
        return bird.getPosition(); //
    }

    sf::CircleShape getShape() const
    {
        return hitbox; //
    }

    void deathAnimation()
    {
        bird.rotate(sf::degrees(-15));

        constexpr float gravityEffect = 0.001;
        for (int i = 0; i < 5; ++i)
        {
            sf::Vector2f movement(i, gravityEffect * i * i);
            if (bird.getPosition().x - movement.x >= 0 && bird.getPosition().y + movement.y >= 0)
            {
                bird.move({-movement.x, movement.y});
                hitbox.move({-movement.x, movement.y});
            }
        }
    }

private:
    sf::RectangleShape bird;
    sf::CircleShape hitbox;
    sf::Texture texture;
    sf::IntRect birdTextRect;

    sf::SoundBuffer jumpBuffer;
    sf::Sound jumpSound;
    
    float _gravity;
    float _jump;
    float _velocity;
    float _terminalVelocity;
    float _rotate;
    int _birdtexrectTop;

    sf::Clock clock;
    sf::Clock _jumpClock;
    sf::Time _jumpCooldown;
    bool _canJump;
};