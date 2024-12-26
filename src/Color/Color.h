#pragma once
#include <ImNodeFlow.h>
#include <imgui.h>
#include <string>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include "../NodeTypes.h"

class ColorBalanceNode : public ImFlow::BaseNode {
public:
    ColorBalanceNode();
    void draw() override;

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

class ColorCorrectionNode : public ImFlow::BaseNode {
public:
    ColorCorrectionNode();
    void draw() override;

private:
    void applyColorCorrection();

    float brightness;
    float contrast;
    float saturation;

    Image imageData;
    Image imageDataOld;
    std::shared_ptr<ImFlow::InPin<Image>> imageDataPin;
    std::shared_ptr<ImFlow::OutPin<Image>> outputImagePin;
};
