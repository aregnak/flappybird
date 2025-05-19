#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Audio.hpp>

#include <algorithm>
#include <string>
#include <iostream>
#include <cstdlib>
#include <ctime>

#include "bird.h"
#include "walls.h"
#include "loadSounds.h"
#include "loadHS.h"
#include "game.h"

#define INITIAL_WIDTH 800
#define INITIAL_HEIGHT 800
#define ASPECT_RATIO (float)INITIAL_WIDTH / INITIAL_HEIGHT

#define WALL_GAP 400

sf::Texture Wall::texture;

// Function to maintain aspect ratio when window is resized
void updateView(sf::RenderWindow& window, sf::View& view)
{
    float windowWidth = window.getSize().x;
    float windowHeight = window.getSize().y;
    float viewWidth;
    float viewHeight;

    // Calculate view dimensions to maintain aspect ratio
    if (windowWidth / windowHeight > ASPECT_RATIO)
    {
        viewHeight = INITIAL_HEIGHT;
        viewWidth = viewHeight * (windowWidth / windowHeight);
    }
    else
    {
        viewWidth = INITIAL_WIDTH;
        viewHeight = viewWidth / (windowWidth / windowHeight);
    }

    view.setSize(sf::Vector2f(viewWidth, viewHeight));
    view.setCenter(sf::Vector2f(INITIAL_WIDTH / 2.f, INITIAL_HEIGHT / 2.f));
    window.setView(view);
}

void spawnWalls(sf::RenderWindow& window, std::vector<Wall>& walls, float& wallX)
{
    // Check if we need to spawn new walls based on the last wall's position
    if (window.getSize().x - wallX >= WALL_GAP)
    {
        float wallY =
            (rand() % (600 - 200 + 1) + 200); // random position between 600 and 200 pixels
        walls.push_back(Wall(window.getSize().x, wallY));
        walls.push_back(Wall(window.getSize().x, wallY - 880)); // second wall
        //wallX = window.getSize().x; // Update wallX to the position of the new wall
    }
}

int main()
{
    Game game;
    game.run();
    return 0;
}
