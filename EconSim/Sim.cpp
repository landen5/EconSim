#include "Sim.h"
#include <iostream>
#include "FastNoiseLite.h"

#include <Kairos/Timestep.hpp>

kairos::Timestep timestep; //1


Sim::Sim() {
	simSpeed = 1.0;
	timestep.setStep(1.0 / simSpeed); // fraction of a second
}

void Sim::Update(float elapsed) {
	timestep.addFrame(); // (2)
	while (timestep.isUpdateRequired()) {// (3) 
		std::cout << getSimSpeed() << std::endl;
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
