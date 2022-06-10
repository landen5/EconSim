#pragma once
#include <string>

class Offer {
private:
	std::string commodity;
	int type; //0 = buy, 1 = sell
	int amount;
	int price;
	int agentID;
public: 
	Offer(std::string commodity, int type, int amount, int price, int agentID);
	std::string getCommodity();
	int getType();
	int getAmount();
	int getPrice();
	int getID();
};