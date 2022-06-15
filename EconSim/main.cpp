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


const float SCREEN_WIDTH = 1920 / 2; //960
const float SCREEN_HEIGHT = 1080 / 2; //540

const int horizontal = 200;
const int vertical = 200;

static float gameZoom = 1;

Terrain t1;

template <typename T> //templates create a unique function definition at compile time depending on parameter type
inline T RandomRange(T min, T max) {
    T scale = rand() / (T)RAND_MAX; 
    return min + scale * (max - min);
}

static void priceGraph(Sim* sim); //prototype thing, change

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

        if (ImGui::BeginMenu("Commodity Prices"))
        {
            priceGraph(sim);
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

static void priceGraph(Sim* sim) {
    ImPlot::CreateContext();
    static double xs1[101], ys1[101], ys2[101], ys3[101];
    srand(0);
    for (int i = 0; i < 101; ++i) {
        xs1[i] = (float)i;
        ys1[i] = RandomRange(400.0, 450.0);
        ys2[i] = RandomRange(275.0, 350.0);
        ys3[i] = RandomRange(150.0, 225.0);
    }
    static bool show_lines = true;
    static bool show_fills = true;
    static float fill_ref = 0;
    static int shade_mode = 0;
    ImGui::Checkbox("Lines", &show_lines); ImGui::SameLine();
    ImGui::Checkbox("Fills", &show_fills);
    if (show_fills) {
        ImGui::SameLine();
        if (ImGui::RadioButton("To -INF", shade_mode == 0))
            shade_mode = 0;
        ImGui::SameLine();
        if (ImGui::RadioButton("To +INF", shade_mode == 1))
            shade_mode = 1;
        ImGui::SameLine();
        if (ImGui::RadioButton("To Ref", shade_mode == 2))
            shade_mode = 2;
        if (shade_mode == 2) {
            ImGui::SameLine();
            ImGui::SetNextItemWidth(100);
            ImGui::DragFloat("##Ref", &fill_ref, 1, -100, 500);
        }
    }

    if (ImPlot::BeginPlot("Stock Prices")) {
        ImPlot::SetupAxes("Days", "Price");
        ImPlot::SetupAxesLimits(0, 100, 0, 500);
        if (show_fills) {
            ImPlot::PushStyleVar(ImPlotStyleVar_FillAlpha, 0.25f);
            ImPlot::PlotShaded("Stock 1", xs1, ys1, 101, shade_mode == 0 ? -INFINITY : shade_mode == 1 ? INFINITY : fill_ref);
            ImPlot::PlotShaded("Stock 2", xs1, ys2, 101, shade_mode == 0 ? -INFINITY : shade_mode == 1 ? INFINITY : fill_ref);
            ImPlot::PlotShaded("Stock 3", xs1, ys3, 101, shade_mode == 0 ? -INFINITY : shade_mode == 1 ? INFINITY : fill_ref);
            ImPlot::PopStyleVar();
        }
        if (show_lines) {
            ImPlot::PlotLine("Stock 1", xs1, ys1, 101);
            ImPlot::PlotLine("Stock 2", xs1, ys2, 101);
            ImPlot::PlotLine("Stock 3", xs1, ys3, 101);
        }
        ImPlot::EndPlot();
    }
    ImPlot::DestroyContext();
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
        topMenuBar(sim);
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

