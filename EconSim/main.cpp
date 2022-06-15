#include <SFML/Graphics.hpp>
#include "imgui.h"
#include "imgui-SFML.h"

#include <iostream>
#include <cmath>
#include "Sim.h"
#include "FastNoiseLite.h"
#include "Terrain.h"
#include "Settlement.h"

#include "implot.h"
#include "GUI.h"


const float SCREEN_WIDTH = 1920 / 2; //960
const float SCREEN_HEIGHT = 1080 / 2; //540

const int horizontal = 200;
const int vertical = 200;

static float gameZoom = 1;

Terrain t1;

int main()
{
    sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Econ Sim");
    ImGui::SFML::Init(window);

    sf::Clock clock;
    sf::View view1;
    //utimately should make this a camera class

    Sim *sim = new Sim();
    TileMap map = t1.generate();

    view1.setCenter(sf::Vector2f((t1.getSize() * 8)/2, (t1.getSize() * 8)/2)); //*8 bc each tile is 8 pixels
    view1.setSize(sf::Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT));

    sf::Clock deltaClock; //for imgui internal
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(window, event);

            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseWheelMoved)
            {
                //std::cout << "wheel movement: " << event.mouseWheel.delta << std::endl;
                //view1.zoom(0.01f * event.mouseWheel.delta);
                gameZoom -= 0.1f * event.mouseWheel.delta;
                //std::cout << gameZoom << std::endl;
            }

        }
        //keyboard input
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        {
            view1.move(0.f, -15.f);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        {
            view1.move(0.f, 15.f);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        {
            view1.move(-15.f, 0.f);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            view1.move(15.f, 0.f);
        }

        ImGui::SFML::Update(window, deltaClock.restart()); //imgui uses delta internally
        
                                                           //gui rendering
        GUI::topMenuBar(sim);
        //priceGraph(sim);
        //ImGui::ShowDemoWindow();


        //uses the time elapsed at each iteration of the game loop to update the game logic
        sf::Time elapsed = clock.restart();
        window.clear();
        window.setView(view1);
        view1.zoom(gameZoom);
        window.draw(map);
        window.draw((*sim).settlements.at(0).getSprite());
        window.draw((*sim).settlements.at(1).getSprite());
        (*sim).Update(elapsed.asSeconds());

        ImGui::SFML::Render(window);

        window.display();

        gameZoom = 1; //find a better way to do this
    }

    ImGui::SFML::Shutdown();
    return 0;
}

