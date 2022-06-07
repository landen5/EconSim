#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

class Settlement {
private:
	//float development;
	std::string name;
	int population;
	sf::Vector2f pos;
	sf::Sprite sprite;
	sf::Texture texture;
public:
	Settlement(std::string name, int population, sf::Vector2f pos);
	std::string getName();
	int getPopulation();
	void initSprite(sf::Vector2f pos);
	void changePopulation(int amount);
	sf::Sprite getSprite();
	void rotateSprite(float angle);
};