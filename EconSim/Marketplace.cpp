#include "Marketplace.h"
#include "Agent.h"
#include "Offer.h"

#include <iostream>
#include <algorithm>
#include <random>
#include <string>
#include <vector>

Marketplace::Marketplace() {
	//initialize all of the market commodities - perhaps use a separate json file or something
	comodityTypes.push_back("food");
	comodityTypes.push_back("wood");
	comodityTypes.push_back("tool");
	for (int i = 0; i < 10; i++) {
		Agent* farmer = new Agent("farmer", 100);
		agents.push_back(*farmer);
	}
	for (int i = 0; i < 10; i++) {
		Agent* logger = new Agent("logger", 100);
		agents.push_back(*logger);
	}
	for (int i = 0; i < 10; i++) {
		Agent* smith = new Agent("smith", 100);
		agents.push_back(*smith);
	}

}

void Marketplace::doRound() {
	for (int i = 0; i < agents.size(); i++) {
		agents.at(i).performProduction();

		//generate bids
		Offer foodBid = agents.at(i).generateBidOffers("food");
		if (foodBid.getCommodity() != "empty") {
			foodBids.push_back(foodBid);
		}
		Offer toolBid = agents.at(i).generateBidOffers("tool");
		if (toolBid.getCommodity() != "empty") {
			toolBids.push_back(toolBid);
		}
		Offer woodBid = agents.at(i).generateBidOffers("wood");
		if (woodBid.getCommodity() != "empty") {
			woodBids.push_back(woodBid);
		}

		//generate asks
		Offer foodAsk = agents.at(i).generateAskOffers("food");
		if (foodAsk.getCommodity() != "empty") {
			foodAsks.push_back(foodAsk);
		}
		Offer toolAsk = agents.at(i).generateAskOffers("tool");
		if (toolAsk.getCommodity() != "empty") {
			toolAsks.push_back(foodAsk);
		}
		Offer woodAsk = agents.at(i).generateAskOffers("wood");
		if (woodAsk.getCommodity() != "empty") {
			woodAsks.push_back(foodAsk);
		}
	}
	std::cout << "All offers added" << std::endl;

	resolveOffers("food");
	//resolveOffers("tool");
	//resolveOffers("wood");

	//for debug
	/*
	std::cout << "food bids size: " << foodBids.size() << std::endl;
	std::cout << "tool bids size: " << toolBids.size() << std::endl;
	std::cout << "wood bids size: " << woodBids.size() << std::endl;

	std::cout << "food asks size: " << foodAsks.size() << std::endl;
	std::cout << "tool asks size: " << toolAsks.size() << std::endl;
	std::cout << "wood asks size: " << woodAsks.size() << std::endl;
	*/

	//**BELOW ARE TEMP, remove to avoid future issues
	//clear bids
	toolBids.clear();
	woodBids.clear();

	//clear asks
	toolAsks.clear();
	woodAsks.clear();
}

void Marketplace::resolveOffers(std::string commodity) {
	auto rng = std::default_random_engine{};

	if (commodity == "food") {
		//shuffle bids/asks books
		std::shuffle(std::begin(foodBids), std::end(foodBids), rng);
		std::shuffle(std::begin(foodAsks), std::end(foodAsks), rng);

		//sort bids high->low
		std::sort(foodBids.rbegin(), foodBids.rend(), [](Offer& one, Offer& two) {return one.getPrice() < two.getPrice(); });
		for (int i = 0; i < foodBids.size(); i++) {
			//std::cout << "Buy " << foodBids.at(i).getCommodity() << " for $" << foodBids.at(i).getPrice() << std::endl;
		}

		//sort asks low->high
		std::sort(foodAsks.begin(), foodAsks.end(), [](Offer& one, Offer& two) {return one.getPrice() < two.getPrice(); });
		for (int i = 0; i < foodAsks.size(); i++) {
			//std::cout << "Sell food for $" << foodAsks.at(i).getPrice() << std::endl;
		}
		//std::cout << "reached this point" << std::endl;
		int sellerPos;
		int buyerPos;
		while (foodBids.size() != 0 && foodAsks.size() != 0) { //while both books are not empty
			int sellerID = foodAsks.at(0).getID();
			int buyerID = foodBids.at(0).getID();
			//std::cout << "food asks size: " << foodAsks.size() << std::endl;
			//std::cout << "food bids size: " << foodBids.size() << std::endl;

			//min of units offered by seller and units desired by buyer
			int quantityTraded = std::min(foodAsks.at(0).getAmount(), foodBids.at(0).getAmount()); 
			//average of two prices
			int clearingPrice = (foodAsks.at(0).getPrice() + foodBids.at(0).getPrice()) / 2;

			//match seller offer id with agent
			if (quantityTraded > 0) {
				for (int i = 0; i < agents.size(); i++) {
					if (agents.at(i).getID() == sellerID) {
						sellerPos = i;
						agents.at(i).sellAmount -= quantityTraded;
						agents.at(i).foodAmount -= quantityTraded; //transfer units of commod
						agents.at(i).wealth += quantityTraded * clearingPrice;
						//update price model
					}
					if (agents.at(i).getID() == buyerID) {
						buyerPos = i;
						agents.at(i).buyAmount -= quantityTraded;
						agents.at(i).foodAmount += quantityTraded; 
						agents.at(i).wealth -= quantityTraded * clearingPrice;
						//update price model
					}
				}
			}
			
			if (agents.at(sellerPos).sellAmount == 0) {
				foodAsks.erase(foodAsks.begin());
			}
			if (agents.at(buyerPos).buyAmount == 0) {
				foodBids.erase(foodBids.begin());
			} 
			std::cout << agents.at(buyerPos).buyAmount << std::endl;
		}
		//**issuing agents update price beliefs
		//reject remaing offers
		foodAsks.clear();
		foodBids.clear();
	}
}

void Marketplace::getAgentPop() {
	for (int i = 0; i < agents.size(); i++) {
		std::cout << agents.at(i).getProfession() << " of id: " << agents.at(i).getID() << std::endl;
	}
}
