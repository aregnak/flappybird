#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Shader.hpp>
#include <SFML/Graphics/Shape.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Texture.hpp>

#include <iostream>

/*-------------------------
This class creates the walls, adds the texture, and moves the walls horizontally.

Position is decided when creating the object, size and speed decided in here.


---------------------------*/
class Wall
{
public:
    Wall(float x, float y);

    // check for collision with an item, can be anything but is used with bird hitbox
    const bool collision(const sf::Shape& item);

    // leaving these here as they probably benefit from being inline
    // move walls towards left
    void update(sf::Time& deltaTime)
    {
        wall.move({ _moveSpeed * deltaTime.asSeconds(), 0.f });
        //
    }

    // get position of walls, since y doesnt matter only x is returned
    float getX() const
    {
        return wall.getPosition().x; //
    }

    // render to the screen
    void drawTo(sf::RenderWindow& _window) const { _window.draw(wall); }

private:
    sf::RectangleShape wall;
    inline static sf::Texture texture;

    float _moveSpeed;
};