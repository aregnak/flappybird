#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/ContextSettings.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Window/WindowStyle.hpp>

#include "bird.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 800), "Flappy Bird", sf::Style::Default,
                            sf::ContextSettings(0, 0, 8));

    window.setFramerateLimit(60);
    window.setKeyRepeatEnabled(false);

    sf::Clock timer;

    Bird bird(30, 30);

    //sf::RectangleShape rect;
    //rect.setSize(sf::Vector2f(100, 100));
    //rect.setPosition(sf::Vector2f(400, 400));

    while (window.isOpen())
    {
        window.setSize(sf::Vector2u(800, 800));

        sf::Time deltaTime = timer.restart();

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }

            else if (event.type == event.KeyPressed)
            {
            }
            bird.handleEvent(event);
        }

        sf::Vector2f playerPos = bird.getPos();

        if (playerPos.y >= 750)
        {
            window.close();
        }

        bird.update(deltaTime);

        window.clear();
        bird.drawTo(window);
        window.display();
    }
}