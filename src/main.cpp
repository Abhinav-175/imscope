#include <iostream>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <implot.h>

#include <GLFW/glfw3.h>

static void glfw_error_callback(int error, const char* description) {
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

struct ScrollingBuffer {
    int MaxSize;
    int Offset;
    ImVector<ImVec2> Data;
    ScrollingBuffer() {
        Offset  = 0;
        Data.reserve(MaxSize);
    }
    void AddPoint(float x, float y) {
        if (Data.size() < MaxSize)
            Data.push_back(ImVec2(x,y));
        else {
            Data[Offset] = ImVec2(x,y);
            Offset =  (Offset + 1) % MaxSize;
        }
    }
    void Erase() {
        if (Data.size() > 0) {
            Data.shrink(0);
            Offset  = 0;
        }
    }
};

void Demo_RealtimePlots() {
    static ScrollingBuffer sdata10k; sdata10k.MaxSize = 10000;
    static float t = 0;
    double ticks[] = {t-9, t-8, t-7, t-6, t-5, t-4, t-3, t-2, t-1, t};
    const char* tick_labels[] = {"9", "-8", "-7", "-6", "-5", "-4", "-3", "-2", "-1", "0"};
    float width = ImGui::GetContentRegionAvail().x;
    // float height = ImGui::GetContentRegionAvail().y;

    float x_val;
    if (std::cin>> x_val) {
        sdata10k.AddPoint(t, x_val);
        t += ImGui::GetIO().DeltaTime; 
    }

    if (ImPlot::BeginPlot("##Scrolling")) {
        ImPlot::SetupAxisLimits(ImAxis_X1, t - 10.0f, t, ImGuiCond_Always);
        ImPlot::SetupAxisLimits(ImAxis_Y1, -1, 1);
        ImPlot::SetupAxisTicks(ImAxis_X1, ticks, 10, tick_labels);
        ImPlot::PlotLine("uniform random", &sdata10k.Data[0].x, &sdata10k.Data[0].y, sdata10k.Data.size(), 0, sdata10k.Offset, 2 * sizeof(float));
        ImPlot::EndPlot();
    }

    if (ImPlot::BeginPlot("##histogram", ImVec2(width*0.5, 350))) {
        ImPlot::SetupAxisLimits(ImAxis_X1, -1, 1);
        ImPlot::SetupAxisLimits(ImAxis_Y1, 0, 150);
        ImPlot::SetNextFillStyle(IMPLOT_AUTO_COL,0.5f);
        // ImPlot::PlotHistogram("uniform random", &sdata500.Data[0].y, sdata500.Data.size(), 150);
        ImPlot::PlotHistogram("uniform random", &sdata10k.Data[0].y, sdata10k.Data.size(), 150, 1.0, ImPlotRange(-1, 1));

        ImPlot::EndPlot();
    }

    ImGui::SameLine();
    if (ImPlot::BeginPlot("##test", ImVec2(width*0.5, 350))) {
        float data[] = { 0.1f, 0.2f, 0.3f, 0.4f, 0.5f };
        ImPlot::PlotLine("My Data", data, 5);
        ImPlot::EndPlot();
    }
}

int main(int, char**) {
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

    const char* glsl_version = "#version 300 es";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);

    float main_scale = ImGui_ImplGlfw_GetContentScaleForMonitor(glfwGetPrimaryMonitor());
    GLFWwindow* window = glfwCreateWindow((int)(540 * main_scale), (int)(700 * main_scale), "ImScope Scrolling plot and histogram", nullptr, nullptr);
    if (window == nullptr)
        return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImPlot::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    ImGui::StyleColorsDark();

    ImGuiStyle& style = ImGui::GetStyle();
    style.ScaleAllSizes(main_scale);
    style.FontScaleDpi = main_scale;

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        if (glfwGetWindowAttrib(window, GLFW_ICONIFIED) != 0) {
            ImGui_ImplGlfw_Sleep(10);
            continue;
        }

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);

        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImVec2(display_w, display_h));

        {
            ImGui::Begin("test", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
            Demo_RealtimePlots();
            ImGui::End();
        }

        ImGui::Render();
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImPlot::DestroyContext();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}