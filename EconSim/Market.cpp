#include "Marketplace.h"

#include <iostream>
#include <string>
#include <vector>

Marketplace::Marketplace() {
	//initialize all of the market commodities - perhaps use a separate json file or something
	comodityTypes.push_back("food");
	comodityTypes.push_back("wood");
	comodityTypes.push_back("tool");

}