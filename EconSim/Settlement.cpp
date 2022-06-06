#include "Settlement.h"
#include <SFML/Graphics.hpp>
#include <iostream>

Settlement::Settlement(std::string name, int population, sf::Vector2f pos) {
	this->name = name;
	this->population = population;
	initSprite(pos);
}

std::string Settlement::getName() {
	return name;
}

int Settlement::getPopulation() {
	return population;
}

void Settlement::initSprite(sf::Vector2f pos) {
	if (!texture.loadFromFile("Assets/village.png"))
	{
		std::cout << "Unable to load settlement image" << std::endl;
	}
	sprite.setTexture(texture);
	sprite.setPosition(sf::Vector2f(pos));
}

sf::Sprite Settlement::getSprite() {
	return sprite;
}



void Settlement::changePopulation(int amount) {
	population += amount;
}