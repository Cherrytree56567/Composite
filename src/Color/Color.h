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
            imageDataOld = imageData;
        }
        ImGui::BeginGroup();
        ImGui::Text("Lift");
        ImGui::PushItemWidth(100);
        if (ImGui::ColorPicker3("##Lift", lift)) {
            
        }
        ImGui::PopItemWidth();
        ImGui::EndGroup();
        ImGui::SameLine();

        ImGui::BeginGroup();
        ImGui::Text("Gain");
        ImGui::PushItemWidth(100);
        if (ImGui::ColorPicker3("##Gain", gain)) {

        }
        ImGui::PopItemWidth();
        ImGui::EndGroup();
        ImGui::SameLine();

        ImGui::BeginGroup();
        ImGui::Text("Gamma");
        ImGui::PushItemWidth(100);
        if (ImGui::ColorPicker3("##Gamma", gamma)) {

        }
        ImGui::PopItemWidth();
        ImGui::EndGroup();

        ImGui::Text("Factor");
        ImGui::PushItemWidth(550);
        if (ImGui::SliderFloat("##Factor", &factor, 0.0f, 1.0f)) {
            
        }
        ImGui::PopItemWidth();

        if (ImGui::Button("Exec")) {
            for (size_t i = 0; i < imageData.pixels.size(); i += imageData.channels) {
                float originalRed = imageDataOld.pixels[i] / 255.0f;
                float adjustedRed = std::pow((originalRed + lift[0]) * gain[0], gamma[0]);
                imageData.pixels[i] = static_cast<uint8_t>(
                    std::clamp(
                        static_cast<int>(
                            (originalRed * (1.0f - factor) + adjustedRed * factor) * 255.0f
                        ), 
                        0, 255
                    )
                );

                if (imageData.channels > 1) {
                    float originalGreen = imageDataOld.pixels[i + 1] / 255.0f;
                    float adjustedGreen = std::pow((originalGreen + lift[1]) * gain[1], gamma[1]);
                    imageData.pixels[i + 1] = static_cast<uint8_t>(
                        std::clamp(
                            static_cast<int>(
                                (originalGreen * (1.0f - factor) + adjustedGreen * factor) * 255.0f
                            ), 
                            0, 255
                        )
                    );
                }

                if (imageData.channels > 2) {
                    float originalBlue = imageDataOld.pixels[i + 2] / 255.0f;
                    float adjustedBlue = std::pow((originalBlue + lift[2]) * gain[2], gamma[2]);
                    imageData.pixels[i + 2] = static_cast<uint8_t>(
                        std::clamp(
                            static_cast<int>(
                                (originalBlue * (1.0f - factor) + adjustedBlue * factor) * 255.0f
                            ), 
                            0, 255
                        )
                    );
                }
            }
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
