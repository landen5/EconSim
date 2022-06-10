#pragma once

#include "Offer.h"
#include <string>

class Agent {
private:
	std::string profession;

	void* id;
public:
	int buyAmount;
	int sellAmount;
	int foodAmount;
	int woodAmount;
	int toolAmount;
	int wealth;

	Agent(std::string profession, int wealth);
	void performProduction();
	Offer generateBidOffers(std::string commodity);
	Offer generateAskOffers(std::string commodity);
	std::string getProfession();
	int getID();
};