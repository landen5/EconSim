#pragma once
#include "imgui.h"
#include "imgui-SFML.h"
#include "implot.h"
#include "Sim.h"

class GUI {
private:

public:
	static void topMenuBar(Sim* sim);
	static void priceGraph(Sim* sim);
};