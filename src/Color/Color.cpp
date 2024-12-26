#include "Color.h"

ColorBalanceNode::ColorBalanceNode() {
    setTitle("Color Balance");
    setStyle(ImFlow::NodeStyle::brown());

    imageDataPin = addIN<Image>("Image", Image{}, ImFlow::ConnectionFilter::SameType());
    liftDataPin = addIN<Vec3>("Lift", Vec3{}, ImFlow::ConnectionFilter::SameType());
    gainDataPin = addIN<Vec3>("Gain", Vec3{}, ImFlow::ConnectionFilter::SameType());
    gammaDataPin = addIN<Vec3>("Gamma", Vec3{}, ImFlow::ConnectionFilter::SameType());
    factorDataPin = addIN<float>("Factor", float{}, ImFlow::ConnectionFilter::SameType());
    imageData = imageDataPin->val();
    imageDataOld = imageDataPin->val();
    outputImagePin = addOUT<Image>("Image");
    outputImagePin->behaviour([this]() { return imageData; });
}

void ColorBalanceNode::draw() {
    Image imageNew = imageDataPin->val();
    if (imageDataOld.pixels != imageNew.pixels) {
        imageData = imageDataPin->val();
        imageDataOld = imageData;
    }
    if (liftDataPin->isConnected()) {
        lift[0] = liftDataPin->val().r;
        lift[1] = liftDataPin->val().g;
        lift[2] = liftDataPin->val().b;
    } else {
        ImGui::BeginGroup();
        ImGui::Text("Lift");
        ImGui::PushItemWidth(100);
        if (ImGui::ColorPicker3("##Lift", lift)) {
            
        }
        ImGui::PopItemWidth();
        ImGui::EndGroup();
        ImGui::SameLine();
    }
    if (gainDataPin->isConnected()) {
        gain[0] = gainDataPin->val().r;
        gain[1] = gainDataPin->val().g;
        gain[2] = gainDataPin->val().b;
    } else {
        ImGui::BeginGroup();
        ImGui::Text("Gain");
        ImGui::PushItemWidth(100);
        if (ImGui::ColorPicker3("##Gain", gain)) {

        }
        ImGui::PopItemWidth();
        ImGui::EndGroup();
        ImGui::SameLine();
    }
    if (gammaDataPin->isConnected()) {
        gamma[0] = gammaDataPin->val().r;
        gamma[1] = gammaDataPin->val().g;
        gamma[2] = gammaDataPin->val().b;
    } else {
        ImGui::BeginGroup();
        ImGui::Text("Gamma");
        ImGui::PushItemWidth(100);
        if (ImGui::ColorPicker3("##Gamma", gamma)) {

        }
        ImGui::PopItemWidth();
        ImGui::EndGroup();
    }
    if (factorDataPin->isConnected()) {
        factor = factorDataPin->val();
    } else {
        ImGui::Text("Factor");
        ImGui::PushItemWidth(550);
        if (ImGui::SliderFloat("##Factor", &factor, 0.0f, 1.0f)) {
                
        }
        ImGui::PopItemWidth();
    }

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

ColorCorrectionNode::ColorCorrectionNode() {
    setTitle("Color Correction");
    setStyle(ImFlow::NodeStyle::brown());

    imageDataPin = addIN<Image>("Image", Image{}, ImFlow::ConnectionFilter::SameType());
    imageData = imageDataPin->val();
    imageDataOld = imageDataPin->val();
    outputImagePin = addOUT<Image>("Image");
    outputImagePin->behaviour([this]() { return imageData; });

    brightness = 1.0f;
    contrast = 1.0f;
    saturation = 1.0f;
}

void ColorCorrectionNode::draw() {
    Image imageNew = imageDataPin->val();
    if (imageDataOld.pixels != imageNew.pixels) {
        imageData = imageDataPin->val();
        imageDataOld = imageData;
    }
    ImGui::PushItemWidth(200);
    ImGui::SliderFloat("Brightness", &brightness, 0.0f, 2.0f);
    ImGui::SliderFloat("Contrast", &contrast, 0.0f, 2.0f);
    ImGui::SliderFloat("Saturation", &saturation, 0.0f, 2.0f);
    ImGui::PopItemWidth();

    if (ImGui::Button("Exec")) {
        applyColorCorrection();
    }
}

void ColorCorrectionNode::applyColorCorrection() {
    for (size_t i = 0; i < imageData.pixels.size(); i += imageData.channels) {
        float rOld = imageDataOld.pixels[i] / 255.0f;
        float gOld = imageDataOld.pixels[i + 1] / 255.0f;
        float bOld = imageDataOld.pixels[i + 2] / 255.0f;

        float r = imageData.pixels[i] / 255.0f;
        float g = imageData.pixels[i + 1] / 255.0f;
        float b = imageData.pixels[i + 2] / 255.0f;

        r = std::min(std::max(rOld * brightness, 0.0f), 1.0f);
        g = std::min(std::max(gOld * brightness, 0.0f), 1.0f);
        b = std::min(std::max(bOld * brightness, 0.0f), 1.0f);

        r = std::min(std::max(((r - 0.5f) * contrast) + 0.5f, 0.0f), 1.0f);
        g = std::min(std::max(((g - 0.5f) * contrast) + 0.5f, 0.0f), 1.0f);
        b = std::min(std::max(((b - 0.5f) * contrast) + 0.5f, 0.0f), 1.0f);

        float gray = (r + g + b) / 3.0f;
        r = gray + (r - gray) * saturation;
        g = gray + (g - gray) * saturation;
        b = gray + (b - gray) * saturation;

        r = std::min(std::max(r, 0.0f), 1.0f);
        g = std::min(std::max(g, 0.0f), 1.0f);
        b = std::min(std::max(b, 0.0f), 1.0f);

        imageData.pixels[i] = static_cast<unsigned char>(r * 255.0f);
        imageData.pixels[i + 1] = static_cast<unsigned char>(g * 255.0f);
        imageData.pixels[i + 2] = static_cast<unsigned char>(b * 255.0f);

        if (imageData.channels == 4) {
            unsigned char a = imageData.pixels[i + 3];
            imageData.pixels[i + 3] = a;
        }
    }
}