#include "bird.h"

Bird::Bird(float x, float y)
    : _gravity(1800.f)
    , _jump(-650.f)
    , _velocity(0.f)
    , _terminalVelocity(600.f)
    , _jumpCooldown(sf::seconds(0.1f))
    , _canJump(true)
    , _rotate(0.f)
    , _birdtexrectTop(0)
    , _jumpBuffer("res/sound/sfx_wing.wav")
    , _jumpSound(_jumpBuffer)
{
    // bird player init
    _bird.setSize(sf::Vector2f(x, y));
    _bird.setOrigin({ x / 2, y / 2 });
    _bird.setPosition({ 200, 300 });
    _bird.setRotation(sf::degrees(90));

    // hitbox init
    _hitbox.setRadius((x / 3));
    _hitbox.setOrigin({ _hitbox.getRadius(), _hitbox.getRadius() });
    _hitbox.setPosition(_bird.getPosition());
    _hitbox.setRotation(sf::degrees(_bird.getRotation().asDegrees()));

    // bird texture with animation, basically the spritesheet.
    if (!_texture.loadFromFile("res/sprite/Player/StyleBird1/AllBird1.png"))
    {
        std::cout << "failed to load bird texture" << std::endl;
        system("pause");
    }
    _birdTextRect = sf::IntRect({ 0, 0 }, { 16, 16 });
    _bird.setTextureRect(_birdTextRect);
    _bird.setTexture(&_texture);
}

void Bird::handleEvent(const sf::Event::KeyPressed* keyDown)
{
    // jumping when space pressed
    if (keyDown->scancode == sf::Keyboard::Scancode::Space && _canJump)
    {
        _jumpSound.play();
        _velocity = _jump;
        _canJump = false;
        _jumpClock.restart();
    }

    // changing bird color by pressing G
    // no idea why G
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

void Bird::update(sf::Time deltaTime)
{
    if (deltaTime == sf::Time::Zero)
    {
        return;
    }

    if (!_canJump)
    {
        // check jump cooldown
        if (_jumpClock.getElapsedTime() > _jumpCooldown)
        {
            _canJump = true;
        }

        // jump animation
        if (_clock.getElapsedTime().asSeconds() > 0.325f)
        {
            if (_birdTextRect.position == sf::Vector2i(48, 0))
            {
                _birdTextRect.position = sf::Vector2i(0, 0);
            }
            else
            {
                _birdTextRect.position += sf::Vector2i(16, 0);
            }
            _clock.restart();
        }
        _bird.setTextureRect(_birdTextRect);
    }
    else
    {
        // idle bird sprite
        _birdTextRect = sf::IntRect({ 0, _birdtexrectTop }, { 16, 16 });
        _bird.setTextureRect(_birdTextRect);
    }

    // honestly i forgot what this is for whoever wrote this code shouldve commented
    if (_bird.getPosition().y < 10)
    {
        _bird.setPosition({ _bird.getPosition().x, 10 });
        _hitbox.setPosition(_bird.getPosition());
    }

    // rotation and falling speed calculations
    _rotate = _velocity / 2.5f;

    _velocity += _gravity * deltaTime.asSeconds();

    if (_velocity > _terminalVelocity)
    {
        _velocity = _terminalVelocity;
    }

    if (_bird.getRotation() < sf::degrees(320) &&
        _bird.getRotation() > sf::degrees(290)) // room for error
    {
        _bird.setRotation(sf::degrees(320)); // max angle looking down
    }

    if (_bird.getRotation() > sf::degrees(40) &&
        _bird.getRotation() < sf::degrees(90)) // room for error
    {
        _bird.setRotation(sf::degrees(40)); // max angle looking up
    }

    _hitbox.move({ 0.f, _velocity * deltaTime.asSeconds() });

    _bird.move({ 0.f, _velocity * deltaTime.asSeconds() });
    _bird.rotate(sf::degrees(_rotate * deltaTime.asSeconds()));
}

void Bird::reset()
{
    _velocity = 0.f;
    _rotate = 0.f;
    _canJump = true;
    _bird.setPosition(sf::Vector2f(200, 300));
    _bird.setRotation(sf::degrees(0));
    _hitbox.setPosition(_bird.getPosition());
    _hitbox.setRotation(sf::degrees(_bird.getRotation().asDegrees()));
}

void Bird::deathAnimation()
{
    _bird.rotate(sf::degrees(-15));

    constexpr float gravityEffect = 0.001;
    for (int i = 0; i < 5; ++i)
    {
        sf::Vector2f movement(i, gravityEffect * i * i);
        if (_bird.getPosition().x - movement.x >= 0 && _bird.getPosition().y + movement.y >= 0)
        {
            _bird.move({ -movement.x, movement.y });
            _hitbox.move({ -movement.x, movement.y });
        }
    }
}
