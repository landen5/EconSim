#include <SFML/Graphics.hpp>
#include "imgui.h"
#include "imgui-SFML.h"

#include <iostream>
#include <cmath>
#include "Sim.h"
#include "FastNoiseLite.h"
#include "Terrain.h"
#include "Settlement.h"

const float SCREEN_WIDTH = 1920 / 2; //960
const float SCREEN_HEIGHT = 1080 / 2; //540

const int horizontal = 200;
const int vertical = 200;

static float gameZoom = 1;

Terrain t1;

//temp, put this in other class
static void topMenuBar(Sim* sim) {
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::Button("+")) {
            //std::cout << "Increased speed" << std::endl;
            sim->increaseSpeed();
        }
        ImGui::Text("speed: ");
        if (ImGui::Button("-")) {
            //std::cout << "Decreased speed" << std::endl;
            sim->decreaseSpeed();
        }

        if (ImGui::BeginMenu("File"))
        {
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit"))
        {
            if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
            if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Econ Sim");
    ImGui::SFML::Init(window);

    sf::Clock clock;
    sf::View view1;
    //utimately should make this a camera class


    Sim *sim = new Sim();
    TileMap map = t1.generate();

    //temp
    Settlement *london = new Settlement("London", 500, sf::Vector2f(2000.0f, 3000.0f));
    Settlement* paris = new Settlement("Paris", 800, sf::Vector2f(3000.0f, 500.0f));

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
        ImGui::SFML::Update(window, deltaClock.restart()); //imgui uses delta internally

        topMenuBar(sim);
        //ImGui::ShowDemoWindow();

        //uses the time elapsed at each iteration of the game loop to update the game logic
        sf::Time elapsed = clock.restart();
        window.clear();
        window.setView(view1);
        view1.zoom(gameZoom);
        window.draw(map);
        window.draw(london->getSprite());
        window.draw(paris->getSprite());
        (*sim).Update(elapsed.asSeconds());

        ImGui::SFML::Render(window);

        window.display();

        gameZoom = 1; //find a better way to do this
    }

    ImGui::SFML::Shutdown();
    return 0;
}

