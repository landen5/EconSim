#include "GUI.h"

template <typename T> //templates create a unique function definition at compile time depending on parameter type
inline T RandomRange(T min, T max) {
    T scale = rand() / (T)RAND_MAX;
    return min + scale * (max - min);
}

void GUI::topMenuBar(Sim* sim) {
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::Button("+")) {
            //std::cout << "Increased speed" << std::endl;
            sim->increaseSpeed();
        }
        ImGui::Text("Speed: ");
        if (ImGui::Button("-")) {
            //std::cout << "Decreased speed" << std::endl;
            sim->decreaseSpeed();
        }

        if (ImGui::BeginMenu("Edit"))
        {
            if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
            if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();

        priceGraph(sim);
    }
}
// utility structure for realtime plot
struct ScrollingBuffer {
    int MaxSize;
    int Offset;
    ImVector<ImVec2> Data;
    ScrollingBuffer(int max_size = 2000) {
        MaxSize = max_size;
        Offset = 0;
        Data.reserve(MaxSize);
    }
    void AddPoint(float x, float y) {
        if (Data.size() < MaxSize)
            Data.push_back(ImVec2(x, y));
        else {
            Data[Offset] = ImVec2(x, y);
            Offset = (Offset + 1) % MaxSize;
        }
    }
    void Erase() {
        if (Data.size() > 0) {
            Data.shrink(0);
            Offset = 0;
        }
    }
};

static float t = 0;
void GUI::priceGraph(Sim* sim) {
    ImPlot::CreateContext();

    ImGui::BulletText("Dynamic price chart");
    static ScrollingBuffer sdata1, sdata2, sdata3;
    ImVec2 mouse = ImGui::GetMousePos();
    //static float t = 0;
    //t += ImGui::GetIO().DeltaTime;
    sdata1.AddPoint(t, sim->settlements.at(0).getMarket().getFoodPrice());
    sdata2.AddPoint(t, sim->settlements.at(0).getMarket().getWoodPrice());
    sdata3.AddPoint(t, sim->settlements.at(0).getMarket().getToolPrice());

    static float history = 10.0f;
    ImGui::SliderFloat("Zoom", &history, 1, 30, "%.1f s"); //check demo for original implementation

    static ImPlotAxisFlags flags = ImPlotAxisFlags_NoTickLabels;

    if (ImPlot::BeginPlot("##Scrolling", ImVec2(-1, 150))) {
        ImPlot::SetupAxes("Rounds", "Price", flags, flags);
        ImPlot::SetupAxisLimits(ImAxis_X1, t - history, t, ImGuiCond_Always);
        ImPlot::SetupAxisLimits(ImAxis_Y1, 0, 10); //y axis limits
        ImPlot::PlotLine("Food Price", &sdata1.Data[0].x, &sdata1.Data[0].y, sdata1.Data.size(), sdata1.Offset, 2 * sizeof(float));
        ImPlot::PlotLine("Wood Price", &sdata2.Data[0].x, &sdata2.Data[0].y, sdata2.Data.size(), sdata2.Offset, 2 * sizeof(float));
        ImPlot::PlotLine("Tool Price", &sdata3.Data[0].x, &sdata3.Data[0].y, sdata3.Data.size(), sdata3.Offset, 2 * sizeof(float));

        ImPlot::EndPlot();
    }
    if (sim->didRound == true) {
        t += 0.1;
    }

    /*
    static double xs1[101], ys1[101], ys2[101], ys3[101];
    srand(0);
    for (int i = 0; i < 101; ++i) {
        xs1[i] = (float)i;
        ys1[i] = RandomRange(400.0, 450.0);
        ys2[i] = RandomRange(275.0, 350.0);
        ys3[i] = RandomRange(150.0, 225.0);
    }
    static bool show_lines = true;
    static bool show_fills = true;
    static float fill_ref = 0;
    static int shade_mode = 0;
    ImGui::Checkbox("Lines", &show_lines); ImGui::SameLine();
    ImGui::Checkbox("Fills", &show_fills);
    if (show_fills) {
        ImGui::SameLine();
        if (ImGui::RadioButton("To -INF", shade_mode == 0))
            shade_mode = 0;
        ImGui::SameLine();
        if (ImGui::RadioButton("To +INF", shade_mode == 1))
            shade_mode = 1;
        ImGui::SameLine();
        if (ImGui::RadioButton("To Ref", shade_mode == 2))
            shade_mode = 2;
        if (shade_mode == 2) {
            ImGui::SameLine();
            ImGui::SetNextItemWidth(100);
            ImGui::DragFloat("##Ref", &fill_ref, 1, -100, 500);
        }
    }

    if (ImPlot::BeginPlot("Stock Prices")) {
        ImPlot::SetupAxes("Days", "Price");
        ImPlot::SetupAxesLimits(0, 100, 0, 500);
        if (show_fills) {
            ImPlot::PushStyleVar(ImPlotStyleVar_FillAlpha, 0.25f);
            ImPlot::PlotShaded("Stock 1", xs1, ys1, 101, shade_mode == 0 ? -INFINITY : shade_mode == 1 ? INFINITY : fill_ref);
            ImPlot::PlotShaded("Stock 2", xs1, ys2, 101, shade_mode == 0 ? -INFINITY : shade_mode == 1 ? INFINITY : fill_ref);
            ImPlot::PlotShaded("Stock 3", xs1, ys3, 101, shade_mode == 0 ? -INFINITY : shade_mode == 1 ? INFINITY : fill_ref);
            ImPlot::PopStyleVar();
        }
        if (show_lines) {
            ImPlot::PlotLine("Stock 1", xs1, ys1, 101);
            ImPlot::PlotLine("Stock 2", xs1, ys2, 101);
            ImPlot::PlotLine("Stock 3", xs1, ys3, 101);
        }
        ImPlot::EndPlot();
    } */
    ImPlot::DestroyContext();
}