#pragma once
#include <vector>
#include "Settlement.h"

class Sim {
private:
	float simSpeed;

public:
	Sim();
	bool didRound;
	void Update(float elapsed);
	void increaseSpeed();
	void decreaseSpeed();
	float getSimSpeed();
	std::vector<Settlement> settlements;
};
