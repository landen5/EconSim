#include "Offer.h"
#include <string>

Offer::Offer(std::string commodity, int type, int amount, int price, int agentID) {
	this->commodity = commodity;
	this->type = type;
	this->amount = amount;
	this->price = price;
	this->agentID = agentID;
}

std::string Offer::getCommodity() {
	return commodity;
}

int Offer::getType() {
	return type;
}

int Offer::getAmount() {
	return amount;
}

int Offer::getPrice() {
	return price;
}

int Offer::getID() {
	return agentID;
}