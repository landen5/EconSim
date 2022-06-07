#include "Sim.h"
#include <iostream>
#include "FastNoiseLite.h"
#include <vector>

#include <Kairos/Timestep.hpp>

kairos::Timestep timestep; //1


Sim::Sim() {
	simSpeed = 1.0;
	timestep.setStep(1.0 / simSpeed); // fraction of a second
	//potential memory leak
	Settlement* london = new Settlement("London", 500, sf::Vector2f(2000.0f, 3000.0f));
	Settlement* paris = new Settlement("Paris", 800, sf::Vector2f(3000.0f, 500.0f));
	settlements.push_back(*london);
	settlements.push_back(*paris);
}

void Sim::Update(float elapsed) {
	timestep.addFrame(); // (2)
	while (timestep.isUpdateRequired()) {// (3) 
		std::cout << getSimSpeed() << std::endl;
		settlements.at(0).rotateSprite(25.0f);
	}
	
}

void Sim::increaseSpeed() {
	if (simSpeed > 0) {
		simSpeed += 0.1;
		timestep.setStep(1.0 / simSpeed); // fraction of a second
	}
}

void Sim::decreaseSpeed() {
	simSpeed -= 0.1;
	timestep.setStep(1.0 / simSpeed); // fraction of a second
}

float Sim::getSimSpeed() {
	return simSpeed;
}
