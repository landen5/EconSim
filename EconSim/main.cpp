#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include "Sim.h"
//#include "TileMap.cpp" //for testing***********
#include "FastNoiseLite.h"
#include "Terrain.h"

const float SCREEN_WIDTH = 1920 / 2; //960
const float SCREEN_HEIGHT = 1080 / 2; //540

const int horizontal = 200;
const int vertical = 200;

static float gameZoom = 1;

int main()
{
    sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Econ Sim");
    sf::Clock clock;
    sf::View view1;
    //utimately should make this a camera class


    Sim sim;
    Terrain t1;
    TileMap map = t1.generate();

    view1.setCenter(sf::Vector2f((t1.getSize() * 8)/2, (t1.getSize() * 8)/2)); //*8 bc each tile is 8 pixels
    view1.setSize(sf::Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT));

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseWheelMoved)
            {
                //std::cout << "wheel movement: " << event.mouseWheel.delta << std::endl;
                //view1.zoom(0.01f * event.mouseWheel.delta);
                gameZoom -= 0.1f * event.mouseWheel.delta;
                std::cout << gameZoom << std::endl;
            }

            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::W)
                {
                    view1.move(0.f, -25.f);
                    
                }
                if (event.key.code == sf::Keyboard::S)
                {
                    view1.move(0.f, 25.f);

                }
                if (event.key.code == sf::Keyboard::A)
                {
                    view1.move(-25.f, 0.f);

                }
                if (event.key.code == sf::Keyboard::D)
                {
                    view1.move(25.f, 0.f);

                }
            }
        }

        //uses the time elapsed at each iteration of the game loop to update the game logic
        sf::Time elapsed = clock.restart();
        window.clear();
        window.setView(view1);
        view1.zoom(gameZoom);
        window.draw(map);
        sim.Update(elapsed.asSeconds());
        window.display();

        gameZoom = 1; //find a better way to do this
    }

    return 0;
}