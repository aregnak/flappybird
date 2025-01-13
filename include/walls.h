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
    Wall(float x, float y)
        : _moveSpeed(-350.f)
    {
        wall.setPosition({x, y});
        wall.setSize(sf::Vector2f(80, 700));

        if (!texture.loadFromFile("res/sprite/pipetest.png"))
        {
            std::cout << "pipe texture failed" << std::endl;
            system("pause");
        }

        wall.setTexture(&texture);
    }

    // move walls towards left
    void update(sf::Time& deltaTime)
    {
        wall.move({_moveSpeed * deltaTime.asSeconds(), 0.f});
        //
    }

    // get position of walls, since y doesnt matter only x is returned
    float getX() const
    {
        return wall.getPosition().x; //
    }
   
    // check for collision with an item, can be anything but is used with bird hitbox
    bool collision(const sf::Shape& item) const
    {
        // new way of doing it with std::optional in SFML 3.0
        std::optional<sf::FloatRect> checkCollision = wall.getGlobalBounds().findIntersection(item.getGlobalBounds());
        
        if (!checkCollision)
        {
            return false;
        }
        return true;
    }

    void drawTo(sf::RenderWindow& window)
    {
        window.draw(wall); //
    }

private:
    sf::RectangleShape wall;
    static sf::Texture texture;

    float _moveSpeed;
};