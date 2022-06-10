#pragma once

#include "Agent.h"
#include "Offer.h"

#include <iostream>
#include <string>
#include <vector>

class Marketplace {
private:
	std::vector<Agent> agents;
	std::vector<std::string> comodityTypes;

	//offers
	std::vector<Offer> foodBids;
	std::vector<Offer> foodAsks;

	std::vector<Offer> toolBids;
	std::vector<Offer> toolAsks;

	std::vector<Offer> woodBids;
	std::vector<Offer> woodAsks;

public:
	Marketplace();
	void getAgentPop();
	void doRound();
	void resolveOffers(std::string commodity);
};
