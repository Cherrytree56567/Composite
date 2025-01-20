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
}

void ColorBalanceNode::execute() {
    m_insa.clear();
    m_params.clear();
    if (imageDataPin->isConnected()) {
        auto parent = imageDataPin->getLink().lock()->left()->getParent();
        m_insa.push_back({parent->getName(), std::to_string(parent->getUID())});
    }
    if (liftDataPin->isConnected()) {
        auto parent = liftDataPin->getLink().lock()->left()->getParent();
        m_insa.push_back({parent->getName(), std::to_string(parent->getUID())});
    } else {
        m_params.push_back({"lift", std::to_string(lift[0]) + ", " + std::to_string(lift[1]) + ", " + std::to_string(lift[2])});
    }
    if (gainDataPin->isConnected()) {
        auto parent = gainDataPin->getLink().lock()->left()->getParent();
        m_insa.push_back({parent->getName(), std::to_string(parent->getUID())});
    } else {
        m_params.push_back({"gain", std::to_string(gain[0]) + ", " + std::to_string(gain[1]) + ", " + std::to_string(gain[2])});
    }
    if (gammaDataPin->isConnected()) {
        auto parent = gammaDataPin->getLink().lock()->left()->getParent();
        m_insa.push_back({parent->getName(), std::to_string(parent->getUID())});
    } else {
        m_params.push_back({"gamma", std::to_string(gamma[0]) + ", " + std::to_string(gamma[1]) + ", " + std::to_string(gamma[2])});
    }
    if (factorDataPin->isConnected()) {
        auto parent = factorDataPin->getLink().lock()->left()->getParent();
        m_insa.push_back({parent->getName(), std::to_string(parent->getUID())});
    } else {
        m_params.push_back({"factor", std::to_string(factor)});
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
}

void ColorCorrectionNode::execute() {
    m_insa.clear();
    m_params.clear();
    if (imageDataPin->isConnected()) {
        auto parent = imageDataPin->getLink().lock()->left()->getParent();
        m_insa.push_back({parent->getName(), std::to_string(parent->getUID())});
    }
    m_params.push_back({"brightness", std::to_string(brightness)});
    m_params.push_back({"contrast", std::to_string(contrast)});
    m_params.push_back({"saturation", std::to_string(saturation)});
}

AlphaOverNode::AlphaOverNode() {
    setTitle("Alpha Over");
    setStyle(ImFlow::NodeStyle::brown());

    topImagePin = addIN<Image>("Image", Image{}, ImFlow::ConnectionFilter::SameType());
    bottomImagePin = addIN<Image>("Image", Image{}, ImFlow::ConnectionFilter::SameType());
    factorPin = addIN<float>("Factor", float{}, ImFlow::ConnectionFilter::SameType());

    outputImagePin = addOUT<Image>("Image");
    outputImagePin->behaviour([this]() { return outputImage; });

    topFallbackColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
    bottomFallbackColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
}

void AlphaOverNode::draw() {
    bool topConnected = topImagePin->isConnected();
    bool bottomConnected = bottomImagePin->isConnected();

    if (!topConnected) {
        ImGui::Dummy(ImVec2(0.0f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_Button, topFallbackColor);
        if (ImGui::Button("##TopFallback", ImVec2(30, 15))) {
            showTopColorPicker = true;
            pickerPosition = ImGui::GetMousePos();
        }
        ImGui::PopStyleColor();
    }

    if (!bottomConnected) {
        ImGui::Dummy(ImVec2(0.0f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_Button, bottomFallbackColor);
        if (ImGui::Button("##BottomFallback", ImVec2(30, 15))) {
            showBottomColorPicker = true;
            pickerPosition = ImGui::GetMousePos();
        }
        ImGui::PopStyleColor();
    }

    if (!factorPin->isConnected()) {
        ImGui::Dummy(ImVec2(0.0f, 1.0f));
        ImGui::PushItemWidth(200);
        ImGui::SliderFloat("##AlphaFactor", &alphaFactor, 0.0f, 1.0f, "%.2f");
        ImGui::PopItemWidth();
    } else {
        alphaFactor = factorPin->val();
    }

    if (showTopColorPicker) {
        ImGui::SetNextWindowPos(pickerPosition);
        if (ImGui::Begin("Color Picker", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove)) {
            ImGui::ColorPicker4("##picker", (float*)&topFallbackColor);
            if (ImGui::Button("Done")) {
                showTopColorPicker = false;
            }
        }
        ImGui::End();
    }

    if (showBottomColorPicker) {
        ImGui::SetNextWindowPos(pickerPosition);
        if (ImGui::Begin("Color Picker", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove)) {
            ImGui::ColorPicker4("##picker", (float*)&bottomFallbackColor);
            if (ImGui::Button("Done")) {
                showBottomColorPicker = false;
            }
        }
        ImGui::End();
    }
}

void AlphaOverNode::showColorPicker(const char* title, ImVec4& color) {
    ImGui::Begin(title, nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
    ImGui::ColorPicker4("##picker", (float*)&color);
    ImGui::End();
}

void AlphaOverNode::execute() {
    m_insa.clear();
    m_params.clear();
    if (topImagePin->isConnected()) {
        auto parent = topImagePin->getLink().lock()->left()->getParent();
        m_insa.push_back({parent->getName(), std::to_string(parent->getUID())});
    } else {
        m_params.push_back({"color", std::to_string(topFallbackColor.x) + ", " + std::to_string(topFallbackColor.y) + ", " + std::to_string(topFallbackColor.z) + ", " + std::to_string(topFallbackColor.w)});
    }
    if (bottomImagePin->isConnected()) {
        auto parent = bottomImagePin->getLink().lock()->left()->getParent();
        m_insa.push_back({parent->getName(), std::to_string(parent->getUID())});
    } else {
        m_params.push_back({"color", std::to_string(bottomFallbackColor.x) + ", " + std::to_string(bottomFallbackColor.y) + ", " + std::to_string(bottomFallbackColor.z) + ", " + std::to_string(bottomFallbackColor.w)});
    }
    if (factorPin->isConnected()) {
        auto parent = factorPin->getLink().lock()->left()->getParent();
        m_insa.push_back({parent->getName(), std::to_string(parent->getUID())});
    } else {
        m_params.push_back({"factor", std::to_string(alphaFactor)});
    }
}

ExposureNode::ExposureNode() {
    setTitle("Exposure");
    setStyle(ImFlow::NodeStyle::brown());

    topImagePin = addIN<Image>("Image", Image{}, ImFlow::ConnectionFilter::SameType());
    exposurePin = addIN<float>("Exposure", float{}, ImFlow::ConnectionFilter::SameType());

    outputImagePin = addOUT<Image>("Image");
    outputImagePin->behaviour([this]() { return outputImage; });

    topFallbackColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
}

void ExposureNode::draw() {
    bool topConnected = topImagePin->isConnected();

    if (!topConnected) {
        ImGui::Dummy(ImVec2(0.0f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_Button, topFallbackColor);
        if (ImGui::Button("##TopFallback", ImVec2(30, 15))) {
            showTopColorPicker = true;
            pickerPosition = ImGui::GetMousePos();
        }
        ImGui::PopStyleColor();
    }

    if (!exposurePin->isConnected()) {
        ImGui::Dummy(ImVec2(0.0f, 1.0f));
        ImGui::PushItemWidth(200);
        ImGui::SliderFloat("##AlphaFactor", &exposure, 0.0f, 10.0f, "%.2f");
        ImGui::PopItemWidth();
    } else {
        exposure = exposurePin->val();
    }

    if (showTopColorPicker) {
        ImGui::SetNextWindowPos(pickerPosition);
        if (ImGui::Begin("Color Picker", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove)) {
            ImGui::ColorPicker4("##picker", (float*)&topFallbackColor);
            if (ImGui::Button("Done")) {
                showTopColorPicker = false;
            }
        }
        ImGui::End();
    }
}

void ExposureNode::execute() {
    m_insa.clear();
    m_params.clear();
    if (topImagePin->isConnected()) {
        auto parent = topImagePin->getLink().lock()->left()->getParent();
        m_insa.push_back({parent->getName(), std::to_string(parent->getUID())});
    } else {
        m_params.push_back({"color", std::to_string(topFallbackColor.x) + ", " + std::to_string(topFallbackColor.y) + ", " + std::to_string(topFallbackColor.z) + ", " + std::to_string(topFallbackColor.w)});
    }
    if (exposurePin->isConnected()) {
        auto parent = exposurePin->getLink().lock()->left()->getParent();
        m_insa.push_back({parent->getName(), std::to_string(parent->getUID())});
    } else {
        m_params.push_back({"exposure", std::to_string(exposure)});
    }
}

GammaNode::GammaNode() {
    setTitle("Gamma");
    setStyle(ImFlow::NodeStyle::brown());

    topImagePin = addIN<Image>("Image", Image{}, ImFlow::ConnectionFilter::SameType());
    gammaPin = addIN<float>("Gamma", float{}, ImFlow::ConnectionFilter::SameType());

    outputImagePin = addOUT<Image>("Image");
    outputImagePin->behaviour([this]() { return outputImage; });

    topFallbackColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
}

void GammaNode::draw() {
    bool topConnected = topImagePin->isConnected();

    if (!topConnected) {
        ImGui::Dummy(ImVec2(0.0f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_Button, topFallbackColor);
        if (ImGui::Button("##TopFallback", ImVec2(30, 15))) {
            showTopColorPicker = true;
            pickerPosition = ImGui::GetMousePos();
        }
        ImGui::PopStyleColor();
    }

    if (!gammaPin->isConnected()) {
        ImGui::Dummy(ImVec2(0.0f, 1.0f));
        ImGui::PushItemWidth(200);
        ImGui::SliderFloat("Gamma", &gamma, 0.0f, 10.0f, "%.2f");
        ImGui::PopItemWidth();
    } else {
        gamma = gammaPin->val();
    }

    if (showTopColorPicker) {
        ImGui::SetNextWindowPos(pickerPosition);
        if (ImGui::Begin("Color Picker", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove)) {
            ImGui::ColorPicker4("##picker", (float*)&topFallbackColor);
            if (ImGui::Button("Done")) {
                showTopColorPicker = false;
            }
        }
        ImGui::End();
    }
}

void GammaNode::execute() {
    m_insa.clear();
    m_params.clear();
    if (topImagePin->isConnected()) {
        auto parent = topImagePin->getLink().lock()->left()->getParent();
        m_insa.push_back({parent->getName(), std::to_string(parent->getUID())});
    } else {
        m_params.push_back({"color", std::to_string(topFallbackColor.x) + ", " + std::to_string(topFallbackColor.y) + ", " + std::to_string(topFallbackColor.z) + ", " + std::to_string(topFallbackColor.w)});
    }
    if (gammaPin->isConnected()) {
        auto parent = gammaPin->getLink().lock()->left()->getParent();
        m_insa.push_back({parent->getName(), std::to_string(parent->getUID())});
    } else {
        m_params.push_back({"gamma", std::to_string(gamma)});
    }
}