#pragma once
#include "FastNoiseLite.h"
#include "TileMap.cpp"

class Terrain {
public:
	Terrain();
	TileMap generate();
	int getSize();
private:
	const int mapSize = 500;
	int* level;
};
