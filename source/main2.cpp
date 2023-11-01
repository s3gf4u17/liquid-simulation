#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <math.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
float particlesize = 3.0;
float gravity = 0.0;
float hitenergyloss = 0.5;

float smoothradius = 0.1;
#include "render.cpp"
#include "glsetup.cpp"
#include "physics.cpp"

int main() {
    GLFWwindow* window = glSetup("shvertex.glsl","shfragment.glsl");

    Liquid liquid(10,200);
    unsigned int VBO = liquid.generateVBO();

    while (!glfwWindowShouldClose(window)) {
        processInput(window);
        glClearColor(0.0f,0.0f,0.0f,1.0f);
        glPointSize(particlesize);
        glClear(GL_COLOR_BUFFER_BIT);
        liquid.updateVBO(ImGui::GetIO().DeltaTime);
        glDrawArrays(GL_POINTS,0,liquid.count);

        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        {
            ImGui::SetNextWindowPos(ImVec2(0,0));
            ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
            ImGui::Begin("control",(bool*)true,window_flags);
            ImGui::Text("FPS: %.0f",1.0/ImGui::GetIO().DeltaTime);
            ImGui::SliderFloat(":Gravity",&gravity,-3.0,3.0,"%.2f");
            ImGui::SliderFloat(":ParticleSize",&particlesize,1.0,30.0,"%.2f");
            ImGui::SliderFloat(":HitEnergyLoss",&hitenergyloss,0.0,1.0,"%.2f");
            ImGui::SliderFloat(":SmoothRadius",&smoothradius,0.0,1.0,"%.2f");
            ImGui::Text("Density: %.4f",liquid.calculateDensity());
            ImGui::End();
        }
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }
    glfwTerminate();
    return 0;
}