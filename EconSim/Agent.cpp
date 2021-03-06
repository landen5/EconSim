#include "Agent.h"
#include <string>

Agent::Agent(std::string profession, int wealth) {
	this->profession = profession;
	this->wealth = wealth;

	int startingRange = 8 - 1 + 1; //max - min +1
	foodAmount = rand() % startingRange + 1;
	toolAmount = rand() % startingRange + 1;
	woodAmount = rand() % startingRange + 1;
	id = &*this;
}

void Agent::performProduction() {
	foodAmount -= 1; //consume food, *need to impelment death
	if (profession == "farmer") {
		//produce/use commodity
		if (toolAmount >= 2) {
			foodAmount += 4;
			toolAmount -= 1; //add probablity to determine chance of breaking tool
		}
	}
	if (profession == "logger") {
		//produce/use
		if (toolAmount >= 2) {
			woodAmount += 4;
			toolAmount -= 1;
		}
	}
	if (profession == "smith") {
		//produce/use
		if (woodAmount >= 2) {
			toolAmount += 4;
			woodAmount -= 1;
		}
	}
}

int priceRange = 10 - 1 + 1; //max - min +1
Offer Agent::generateBidOffers(std::string commodity) {
	if (commodity == "food") {
		//determine amount of food to buy - eventually make a separate abstract function
		//also make the buy amount dynamic and dpened on favorability factors
		foodToBuy = 3;

		int tempPrice = rand() % priceRange + 1;
		if (foodAmount < 2) { //all agents consume same amount of food
			Offer* buyFood = new Offer("food", 0, foodToBuy, tempPrice, getID());
			return *buyFood;
		}
		else {
			Offer* buyEmpty = new Offer("empty", 0, 0, 0, getID());
			return *buyEmpty;
		}
	}
	if (commodity == "tool") {
		toolToBuy = 3;

		int tempPrice = rand() % priceRange + 1;
		if (profession == "farmer" && toolAmount < 2) {
			Offer* buyTool = new Offer("tool", 0, toolToBuy, tempPrice, getID()); //good, type, amount, price, and id - **change amount and price to be dyanmic
			return *buyTool;
		}
		if (profession == "logger" && toolAmount < 2) {
			Offer* buyTool = new Offer("tool", 0, toolToBuy, tempPrice, getID());
			return *buyTool;
		}
		else {
			Offer* buyEmpty = new Offer("empty", 0, 0, 0, getID());
			return *buyEmpty;
		}
	}
	if (commodity == "wood") {
		woodToBuy = 3;

		int tempPrice = rand() % priceRange + 1;
		if (profession == "smith" && woodAmount < 2) {
			Offer* buyWood = new Offer("wood", 0, woodToBuy, tempPrice, getID());
			return *buyWood;
		}
		else {
			Offer* buyEmpty = new Offer("empty", 0, 0, 0, getID());
			return *buyEmpty;
		}
	}
	else {
		Offer* buyEmpty = new Offer("empty", 0, 0, 0, getID());
		return *buyEmpty;
	}
}

Offer Agent::generateAskOffers(std::string commodity) {
	if (commodity == "food") {
		//determine dynamic selling price
		foodToSell = 4;

		int tempPrice = rand() % priceRange + 1;
		if (profession == "farmer" && foodAmount > 8) {
			Offer* sellFood = new Offer("food", 1, foodToSell, tempPrice, getID());
			return *sellFood;
		}
		else {
			Offer* sellEmpty = new Offer("empty", 0, 0, 0, getID());
			return *sellEmpty;
		}
	}
	if (commodity == "tool") {
		toolToSell = 3;

		int tempPrice = rand() % priceRange + 1;
		if (profession == "smith" && toolAmount > 6) {
			Offer* sellTool = new Offer("tool", 1, toolToSell, tempPrice, getID());
			return *sellTool;
		}
		else {
			Offer* sellEmpty = new Offer("empty", 0, 0, 0, getID());
			return *sellEmpty;
		}
	}
	if (commodity == "wood") {
		woodToSell = 5;

		int tempPrice = rand() % priceRange + 1;
		if (profession == "logger" && woodAmount > 10) {
			Offer* sellWood = new Offer("wood", 1, woodToSell, tempPrice, getID());
			return *sellWood;
		}
		else {
			Offer* sellEmpty = new Offer("empty", 0, 0, 0, getID());
			return *sellEmpty;
		}
	}
	Offer* sellEmpty = new Offer("empty", 0, 0, 0, getID());
	return *sellEmpty;
}

std::string Agent::getProfession() {
	return profession;
}


int Agent::getID() {
	return (int)id;
}