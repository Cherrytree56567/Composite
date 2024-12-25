#pragma once
#include <ImNodeFlow.h>
#include <imgui.h>
#include <string>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include "../NodeTypes.h"

class ColorBalanceNode : public ImFlow::BaseNode {
public:
    ColorBalanceNode() {
        setTitle("Color Balance");
        setStyle(ImFlow::NodeStyle::brown());

        // Input and output pins
        imageDataPin = addIN<Image>("Image", Image{}, ImFlow::ConnectionFilter::SameType());
        imageData = imageDataPin->val();
        imageDataOld = imageDataPin->val();
        outputImagePin = addOUT<Image>("Image");
        outputImagePin->behaviour([this]() { return imageData; });
    }

    void draw() override {
        Image imageNew = imageDataPin->val();
        if (imageDataOld.pixels != imageNew.pixels) {
            imageData = imageDataPin->val();
            imageDataOld = imageData;  // Update old image to current one
        }

        ImGui::Text("Lift");
        ImGui::PushItemWidth(200);
        if (ImGui::ColorPicker3("##Lift", lift)) {
            for (size_t i = 0; i < imageData.pixels.size(); i += imageData.channels) {
                imageData.pixels[i] = std::clamp(static_cast<int>(imageData.pixels[i] + lift[0] * 255), 0, 255);
                if (imageData.channels > 1) {
                    imageData.pixels[i + 1] = std::clamp(static_cast<int>(imageData.pixels[i + 1] + lift[1] * 255), 0, 255);
                }
                if (imageData.channels > 2) {
                    imageData.pixels[i + 2] = std::clamp(static_cast<int>(imageData.pixels[i + 2] + lift[2] * 255), 0, 255);
                }
            }
        }
        ImGui::PopItemWidth();

        ImGui::Text("Gain");
        ImGui::SameLine();
        ImGui::PushItemWidth(200);
        if (ImGui::ColorPicker3("##Gain", gain)) {
            // Apply Gain modifications here...
        }
        ImGui::PopItemWidth();

        ImGui::Text("Gamma");
        ImGui::PushItemWidth(200);
        if (ImGui::ColorPicker3("##Gamma", gamma)) {
            // Apply Gamma modifications here...
        }
        ImGui::PopItemWidth();

        ImGui::Text("Factor");
        if (ImGui::SliderFloat("##Factor", &factor, 0.0f, 1.0f)) {
            // Apply Factor modifications here...
        }
    }

private:
    Image imageData;
    Image imageDataOld;
    GLuint textureID;
    std::shared_ptr<ImFlow::InPin<Image>> imageDataPin;
    std::shared_ptr<ImFlow::OutPin<Image>> outputImagePin;

    float lift[3] = {1.0f, 1.0f, 1.0f};
    float gain[3] = {1.0f, 1.0f, 1.0f};
    float gamma[3] = {1.0f, 1.0f, 1.0f};
    float factor = 1.0f;
};
