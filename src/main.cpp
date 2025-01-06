#include <iostream>
#include <ImNodeFlow.h>
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>
#include <thread>
#include "Input/Input.h"
#include "Output/Output.h"
#include "Color/Color.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#pragma comment(lib, "opengl32.lib")

int main() {
    if (!glfwInit()) {
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(1280, 720, "Composite", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\segoeui.ttf", 18);
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");
    ImFlow::ImNodeFlow graph;
    auto nodea = graph.addNode<OutputNode>(ImVec2(100, 100));
    auto nodeb = graph.addNode<ColorBalanceNode>(ImVec2(100, 100));

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        int windowWidth, windowHeight;
        glfwGetFramebufferSize(window, &windowWidth, &windowHeight);

        ImGui::SetNextWindowSize(ImVec2((float)windowWidth, (float)windowHeight), ImGuiCond_Always);

        ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f), ImGuiCond_Always);

        ImGui::Begin("Node Graph", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_MenuBar);
        if (ImGui::BeginMenuBar()) {
            if (ImGui::BeginMenu("File")) {
                if (ImGui::MenuItem("New")) {

                }
                if (ImGui::MenuItem("Open")) {

                }
                if (ImGui::MenuItem("Save")) {
                    
                }
                if (ImGui::MenuItem("Save As")) {

                }
                ImGui::EndMenu();
            }
            if (false) { //ImGui::BeginMenu("Edit")) {
                if (ImGui::MenuItem("Preferences")) {

                }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("View")) {
                if (ImGui::MenuItem("Zoom In")) {
                    graph.getGrid().m_scale -= 0.5;
                }
                if (ImGui::MenuItem("Zoom Out")) {
                    graph.getGrid().m_scale += 0.5;
                }
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Add")) {
                if (ImGui::BeginMenu("Input")) {
                    if (ImGui::MenuItem("Image")) {
                        auto node = graph.addNode<ImageNode>(ImVec2(100, 100));
                    }
                    ImGui::EndMenu();
                }
                if (ImGui::BeginMenu("Output")) {
                    if (ImGui::MenuItem("Composite")) {
                        auto node = graph.addNode<OutputNode>(ImVec2(100, 100));
                    }
                    ImGui::EndMenu();
                }
                if (ImGui::BeginMenu("Color")) {
                    if (ImGui::MenuItem("Color Balance")) {
                        auto node = graph.addNode<ColorBalanceNode>(ImVec2(100, 100));
                    }
                    if (ImGui::MenuItem("Color Correction")) {
                        auto node = graph.addNode<ColorCorrectionNode>(ImVec2(100, 100));
                    }
                    if (ImGui::MenuItem("Alpha Over")) {
                        auto node = graph.addNode<AlphaOverNode>(ImVec2(100, 100));
                    }
                    if (ImGui::MenuItem("Exposure")) {
                        auto node = graph.addNode<ExposureNode>(ImVec2(100, 100));
                    }
                    if (ImGui::MenuItem("Gamma")) {
                        auto node = graph.addNode<GammaNode>(ImVec2(100, 100));
                    }
                    ImGui::EndMenu();
                }
                
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Render")) {
                if (ImGui::MenuItem("Render Image")) {
                    //std::thread t(graph.executeAll);
                    graph.executeAll();
                    graph.exportToXML("test.txt");
                }
                
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Help")) {
                if (ImGui::MenuItem("About")) {

                }
                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }
        graph.update();
        ImGui::End();

        if (ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
            ImGui::OpenPopup("RightClickToolbar");
        }

        if (ImGui::BeginPopup("RightClickToolbar")) {
            if (ImGui::BeginMenu("Input")) {
                if (ImGui::MenuItem("Image")) {
                    auto node = graph.addNode<ImageNode>(ImVec2(100, 100));
                }
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Output")) {
                if (ImGui::MenuItem("Output")) {
                    auto node = graph.addNode<OutputNode>(ImVec2(100, 100));
                }
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Color")) {
                if (ImGui::MenuItem("Color Balance")) {
                    auto node = graph.addNode<ColorBalanceNode>(ImVec2(100, 100));
                }
                if (ImGui::MenuItem("Color Correction")) {
                    auto node = graph.addNode<ColorCorrectionNode>(ImVec2(100, 100));
                }
                if (ImGui::MenuItem("Alpha Over")) {
                    auto node = graph.addNode<AlphaOverNode>(ImVec2(100, 100));
                }
                if (ImGui::MenuItem("Exposure")) {
                    auto node = graph.addNode<ExposureNode>(ImVec2(100, 100));
                }
                if (ImGui::MenuItem("Gamma")) {
                    auto node = graph.addNode<GammaNode>(ImVec2(100, 100));
                }
                ImGui::EndMenu();
            }

            ImGui::EndPopup();
        }

        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}