#include "Marketplace.h"
#include "Agent.h"
#include "Offer.h"

#include <iostream>
#include <algorithm>
#include <random>
#include <string>
#include <vector>

Marketplace::Marketplace() {
	historicalFoodPrices.push_back(0);
	historicalToolPrices.push_back(0);
	historicalWoodPrices.push_back(0);
	//initialize all of the market commodities - perhaps use a separate json file or something
	comodityTypes.push_back("food");
	comodityTypes.push_back("wood");
	comodityTypes.push_back("tool");
	for (int i = 0; i < 100; i++) {
		Agent* farmer = new Agent("farmer", 10);
		agents.push_back(*farmer);
	}
	for (int i = 0; i < 100; i++) {
		Agent* logger = new Agent("logger", 10);
		agents.push_back(*logger);
	}
	for (int i = 0; i < 100; i++) {
		Agent* smith = new Agent("smith", 10);
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
	resolveOffers("wood");
	resolveOffers("tool");

	if (historicalFoodPrices.size() > 10) {
		historicalFoodPrices.erase(historicalFoodPrices.begin());
	}
	if (historicalToolPrices.size() > 10) {
		historicalToolPrices.erase(historicalToolPrices.begin());
	}
	if (historicalWoodPrices.size() > 10) {
		historicalWoodPrices.erase(historicalWoodPrices.begin());
	}
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

		int foodSum = 0;
		int roundCount = 0;
		while (foodBids.size() != 0 && foodAsks.size() != 0) { //while both books are not empty

			int sellerPos;
			int buyerPos;

			int sellerID = foodAsks.at(0).getID();
			int buyerID = foodBids.at(0).getID();
			for (int i = 0; i < agents.size(); i++) {
				if (agents.at(i).getID() == sellerID) {
					sellerPos = i;
				}
				if (agents.at(i).getID() == buyerID) {
					buyerPos = i;
				}
			}

			int quantityTraded = std::min(agents.at(sellerPos).foodToSell, agents.at(buyerPos).foodToBuy);
			int clearingPrice = (foodAsks.at(0).getPrice() + foodBids.at(0).getPrice()) / 2;
			
			if (quantityTraded > 0) { // i think this condition is not necesary
				/*
				std::cout << agents.at(sellerPos).getProfession() << agents.at(sellerPos).getID() << " (" <<
					agents.at(sellerPos).foodToSell << " food) matched with " <<
					agents.at(buyerPos).getProfession() << agents.at(buyerPos).getID() << " (" <<
					agents.at(buyerPos).foodToBuy << " food) Cleared at " << clearingPrice << std::endl; */
				agents.at(sellerPos).foodToSell -= quantityTraded;
				agents.at(buyerPos).foodToBuy -= quantityTraded;
				agents.at(sellerPos).foodAmount -= quantityTraded;
				agents.at(buyerPos).foodAmount += quantityTraded;
				agents.at(sellerPos).wealth += quantityTraded * clearingPrice;
				agents.at(buyerPos).wealth -= quantityTraded * clearingPrice;
				//both update price models

			}
			if (agents.at(sellerPos).foodToSell == 0) {
				foodAsks.erase(foodAsks.begin());
				//std::cout << "removed seller" << std::endl;
			}
			if (agents.at(buyerPos).foodToBuy == 0) {
				foodBids.erase(foodBids.begin());
				//std::cout << "removed buyer" << std::endl;
			}

			foodSum += clearingPrice;
			roundCount++;
		}
		if (roundCount > 0) {
			historicalFoodPrices.push_back(foodSum / roundCount);
		}
		//instead of setting price to zero, it just maintains old price if no trades occured
		else if (roundCount == 0 && historicalFoodPrices.size() >= 1) {
			int prev = historicalFoodPrices.at(historicalFoodPrices.size() - 2);
			historicalFoodPrices.push_back(prev);
		}
		else
		{
			historicalFoodPrices.push_back(0);
		}
		//**issuing agents update price beliefs
		//reject remaing offers
		foodAsks.clear();
		foodBids.clear();
	}

	//------------------------------------WOOD-------------------------------------------------------
	if (commodity == "wood") {

		//shuffle bids/asks books
		std::shuffle(std::begin(woodBids), std::end(woodBids), rng);
		std::shuffle(std::begin(woodAsks), std::end(woodAsks), rng);

		//sort bids high->low
		std::sort(woodBids.rbegin(), woodBids.rend(), [](Offer& one, Offer& two) {return one.getPrice() < two.getPrice(); });
		for (int i = 0; i < woodBids.size(); i++) {
			//std::cout << "Buy " << foodBids.at(i).getCommodity() << " for $" << foodBids.at(i).getPrice() << std::endl;
		}

		//sort asks low->high
		std::sort(woodAsks.begin(), woodAsks.end(), [](Offer& one, Offer& two) {return one.getPrice() < two.getPrice(); });
		for (int i = 0; i < woodAsks.size(); i++) {
			//std::cout << "Sell food for $" << foodAsks.at(i).getPrice() << std::endl;
		}

		int woodSum = 0;
		int roundCount = 0;
		while (woodBids.size() != 0 && woodAsks.size() != 0) { //while both books are not empty
			int sellerPos;
			int buyerPos;

			int sellerID = woodAsks.at(0).getID();
			int buyerID = woodBids.at(0).getID();
			for (int i = 0; i < agents.size(); i++) {
				if (agents.at(i).getID() == sellerID) {
					sellerPos = i;
				}
				if (agents.at(i).getID() == buyerID) {
					buyerPos = i;
				}
			}

			int quantityTraded = std::min(agents.at(sellerPos).woodToSell, agents.at(buyerPos).woodToBuy);
			int clearingPrice = (woodAsks.at(0).getPrice() + woodBids.at(0).getPrice()) / 2;

			if (quantityTraded > 0) { // i think this condition is not necesary
				/*
				std::cout << agents.at(sellerPos).getProfession() << agents.at(sellerPos).getID() << " (" <<
					agents.at(sellerPos).foodToSell << " food) matched with " <<
					agents.at(buyerPos).getProfession() << agents.at(buyerPos).getID() << " (" <<
					agents.at(buyerPos).foodToBuy << " food) Cleared at " << clearingPrice << std::endl; */
				agents.at(sellerPos).woodToSell -= quantityTraded;
				agents.at(buyerPos).woodToBuy -= quantityTraded;
				agents.at(sellerPos).woodAmount -= quantityTraded;
				agents.at(buyerPos).woodAmount += quantityTraded;
				agents.at(sellerPos).wealth += quantityTraded * clearingPrice;
				agents.at(buyerPos).wealth -= quantityTraded * clearingPrice;
				//both update price models

			}
			if (agents.at(sellerPos).woodToSell == 0) {
				woodAsks.erase(woodAsks.begin());
				//std::cout << "removed seller" << std::endl;
			}
			if (agents.at(buyerPos).woodToBuy == 0) {
				woodBids.erase(woodBids.begin());
				//std::cout << "removed buyer" << std::endl;
			}

			woodSum += clearingPrice;
			roundCount++;
		}
		if (roundCount > 0) {
			historicalWoodPrices.push_back(woodSum / roundCount);
		}
		else if (roundCount == 0 && historicalWoodPrices.size() >= 1) {
			int prev = historicalWoodPrices.at(historicalWoodPrices.size() - 2);
			historicalWoodPrices.push_back(prev);
		}
		else
		{
			historicalWoodPrices.push_back(0);
		}
		//**issuing agents update price beliefs
		//reject remaing offers
		woodAsks.clear();
		woodBids.clear();
	}

	//------------------------------------TOOL-------------------------------------------------------
	if (commodity == "tool") {

		//shuffle bids/asks books
		std::shuffle(std::begin(toolBids), std::end(toolBids), rng);
		std::shuffle(std::begin(toolAsks), std::end(toolAsks), rng);

		//sort bids high->low
		std::sort(toolBids.rbegin(), toolBids.rend(), [](Offer& one, Offer& two) {return one.getPrice() < two.getPrice(); });
		for (int i = 0; i < toolBids.size(); i++) {
			//std::cout << "Buy " << foodBids.at(i).getCommodity() << " for $" << foodBids.at(i).getPrice() << std::endl;
		}

		//sort asks low->high
		std::sort(toolAsks.begin(), toolAsks.end(), [](Offer& one, Offer& two) {return one.getPrice() < two.getPrice(); });
		for (int i = 0; i < toolAsks.size(); i++) {
			//std::cout << "Sell food for $" << foodAsks.at(i).getPrice() << std::endl;
		}

		int toolSum = 0;
		int roundCount = 0;
		while (toolBids.size() != 0 && toolAsks.size() != 0) { //while both books are not empty
			int sellerPos;
			int buyerPos;

			int sellerID = toolAsks.at(0).getID();
			int buyerID = toolBids.at(0).getID();
			for (int i = 0; i < agents.size(); i++) {
				if (agents.at(i).getID() == sellerID) {
					sellerPos = i;
				}
				if (agents.at(i).getID() == buyerID) {
					buyerPos = i;
				}
			}

			int quantityTraded = std::min(agents.at(sellerPos).toolToSell, agents.at(buyerPos).toolToBuy);
			int clearingPrice = (toolAsks.at(0).getPrice() + toolBids.at(0).getPrice()) / 2;

			if (quantityTraded > 0) { // i think this condition is not necesary
				/*
				std::cout << agents.at(sellerPos).getProfession() << agents.at(sellerPos).getID() << " (" <<
					agents.at(sellerPos).foodToSell << " food) matched with " <<
					agents.at(buyerPos).getProfession() << agents.at(buyerPos).getID() << " (" <<
					agents.at(buyerPos).foodToBuy << " food) Cleared at " << clearingPrice << std::endl; */
				agents.at(sellerPos).toolToSell -= quantityTraded;
				agents.at(buyerPos).toolToBuy -= quantityTraded;
				agents.at(sellerPos).toolAmount -= quantityTraded;
				agents.at(buyerPos).toolAmount += quantityTraded;
				agents.at(sellerPos).wealth += quantityTraded * clearingPrice;
				agents.at(buyerPos).wealth -= quantityTraded * clearingPrice;
				//both update price models

			}
			if (agents.at(sellerPos).toolToSell == 0) {
				toolAsks.erase(toolAsks.begin());
				//std::cout << "removed seller" << std::endl;
			}
			if (agents.at(buyerPos).toolToBuy == 0) {
				toolBids.erase(toolBids.begin());
				//std::cout << "removed buyer" << std::endl;
			}

			toolSum += clearingPrice;
			roundCount++;
		}
		if (roundCount > 0) {
			historicalToolPrices.push_back(toolSum / roundCount);
		}
		else if (roundCount == 0 && historicalToolPrices.size() >= 1) {
			int prev = historicalToolPrices.at(historicalToolPrices.size() - 2);
			historicalToolPrices.push_back(prev);
		}
		else
		{
			historicalToolPrices.push_back(0);
		}
		//**issuing agents update price beliefs
		//reject remaing offers
		toolAsks.clear();
		toolBids.clear();
	}
}

void Marketplace::getAgentPop() {
	for (int i = 0; i < agents.size(); i++) {
		std::cout << agents.at(i).getProfession() << " of id: " << agents.at(i).getID() << std::endl;
	}
}

int Marketplace::getFoodPrice() {
	return historicalFoodPrices.at(historicalFoodPrices.size()-1);
}

int Marketplace::getToolPrice() {
	return historicalToolPrices.at(historicalToolPrices.size() - 1);
}

int Marketplace::getWoodPrice() {
	return historicalWoodPrices.at(historicalWoodPrices.size() - 1);
}
