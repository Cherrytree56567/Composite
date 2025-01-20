#include <iostream>
#include <ImNodeFlow.h>
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>
#include <thread>
#include <windows.h>
#include "Input/Input.h"
#include "Output/Output.h"
#include "Color/Color.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#pragma comment(lib, "opengl32.lib")

std::string OutPath = "output.png";

bool IsProcessRunning(HANDLE hProcess) {
    DWORD exitCode;
    if (GetExitCodeProcess(hProcess, &exitCode)) {
        return (exitCode == STILL_ACTIVE);
    }
    return false;
}

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
    ImFileDialogInfo fileDialogInfo;
    bool openFileDialog = false;

    PROCESS_INFORMATION pi = {0};
    bool sss = false;

    fileDialogInfo.title = "Open File";
    fileDialogInfo.directoryPath = std::filesystem::current_path();
    fileDialogInfo.type = ImGuiFileDialogType_SaveFile;
    fileDialogInfo.fileFilterFunc = std::function<bool(std::string)>([](std::string filename) {
        return filename.ends_with(".png") || filename.ends_with(".jpg");
    });

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
                    graph.executeAll();
                    graph.exportToXML("output.xml");
                    openFileDialog = true;
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

        if (ImGui::FileDialog(&openFileDialog, &fileDialogInfo)) {
            if (fileDialogInfo.resultPath.c_str() != nullptr) {
                OutPath = fileDialogInfo.resultPath.string();
                
                std::string ccommand = "CompositeRenderer.exe output.xml " + OutPath;
                LPSTR command = const_cast<LPSTR>(ccommand.c_str());
                std::cout << command;

                STARTUPINFO si = {0};
                si.cb = sizeof(STARTUPINFO);
                si.dwFlags = STARTF_USESHOWWINDOW;
                si.wShowWindow = SW_HIDE;

                if (CreateProcess(NULL, command, NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi)) {

                } else {
                    std::cout << "[Composite::GUI] Error: Failed to start process : " << GetLastError() << ".\n";
                }
                sss = true;
                fileDialogInfo.resultPath.clear();
            }
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

        if (!IsProcessRunning(pi.hProcess) && sss == true) {
            sss = false;
            for (const auto& [nodeUID, nodePtr] : graph.m_nodes) {
                if (!nodePtr) continue;

                if (nodePtr->m_title == "Composite") {
                    int width, height, channels;
                    unsigned char* image_data = stbi_load(OutPath.c_str(), &width, &height, &channels, 0);
                    Image Tep(width, height, channels);
                    Tep.pixels.resize(width * height * channels);
                    std::memcpy(Tep.pixels.data(), image_data, width * height * channels);
                    std::dynamic_pointer_cast<OutputNode>(nodePtr)->writeImageData(Tep);
                    break;
                }
            }
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