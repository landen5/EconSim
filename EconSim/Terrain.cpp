#include "Terrain.h"
#include <iostream>


Terrain::Terrain() {
    // Create and configure FastNoise object
    FastNoiseLite noise;
    noise.SetFrequency(.001);
    //noise.SetFractalOctaves(1);
    noise.SetSeed(831);
    noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);

    level = new int[mapSize * mapSize];

    // Gather noise data
    float *noiseData = new float[mapSize * mapSize];
    //float noiseData[mapSize * mapSize] = { 0 }; //this uses a ton of stack memory, consider using heap (ie. new ...)
    int index = 0;

    for (int y = 0; y < mapSize; y++)
    {
        for (int x = 0; x < mapSize; x++)
        {
            noiseData[index++] = std::abs(noise.GetNoise((float)x, (float)y));
        }
    }
    //same idea, use heap as this is incredibly inefficient
    //int level[mapSize * mapSize] = { 0 };

    // Do something with this data...
    for (int i = 0; i < (mapSize * mapSize); i++) {
        if (noiseData[i] <= 0.2) {
            level[i] = 0; //water
        }
        if (noiseData[i] > 0.2 && noiseData[i] <= 0.25) {
            level[i] = 1; //sand
        }
        if (noiseData[i] > 0.25 && noiseData[i] <= 0.7) {
            level[i] = 2; //grass
        }
        if (noiseData[i] > 0.7 && noiseData[i] <= 0.92) {
            level[i] = 3; //hill
        }
        if (noiseData[i] > 0.92) {
            level[i] = 4; //rock
        }
        //std::cout << level[i] << std::endl;
    }

    //clear up
    delete[] noiseData;

}


TileMap Terrain::generate() {
    TileMap map;
    if (!map.load("Assets/tileset.png", sf::Vector2u(8, 8), level, mapSize, mapSize)) {
        std::cout << "Error loading tileset image" << std::endl;
        //return -1;
    }
    delete[] level;
    return map;
}

int Terrain::getSize() {
    return mapSize;
}


