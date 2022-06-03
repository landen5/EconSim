#pragma once

#include "FastNoiseLite.h"


class Sim {
private:
	int worldSize = 0;
public:
	Sim();
	static void Update(float elapsed);

};
