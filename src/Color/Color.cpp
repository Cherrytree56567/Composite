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
    if (imageDataPin->isConnected()) {
        m_insa.push_back({"Image", std::to_string(imageDataPin->getLink().lock()->right()->getParent()->getUID())});
    } else {
        m_insa.push_back({"Image", "NONE"});
    }
    if (!liftDataPin->isConnected()) {
        m_params.push_back({"Lift", std::to_string(lift[0]) + ", " + std::to_string(lift[1]) + ", " + std::to_string(lift[2])});
    } else {
        m_insa.push_back({"Lift", std::to_string(liftDataPin->getLink().lock()->right()->getParent()->getUID())});
    }
    if (!gainDataPin->isConnected()) {
        m_params.push_back({"Gain", std::to_string(gain[0]) + ", " + std::to_string(gain[1]) + ", " + std::to_string(gain[2])});
    } else {
        m_insa.push_back({"Gain", std::to_string(gainDataPin->getLink().lock()->right()->getParent()->getUID())});
    }
    if (!gammaDataPin->isConnected()) {
        m_params.push_back({"Gamma", std::to_string(gamma[0]) + ", " + std::to_string(gamma[1]) + ", " + std::to_string(gamma[2])});
    } else {
        m_insa.push_back({"Gamma", std::to_string(gammaDataPin->getLink().lock()->right()->getParent()->getUID())});
    }
    if (!factorDataPin->isConnected()) {
        m_params.push_back({"Factor", std::to_string(factor)});
    } else {
        m_insa.push_back({"Factor", std::to_string(factorDataPin->getLink().lock()->right()->getParent()->getUID())});
    }
    if (outputImagePin->isConnected()) {
        m_outsa.push_back({"Image", std::to_string(outputImagePin->getLink().lock()->right()->getParent()->getUID())});
    } else {
        m_insa.push_back({"Image", "NONE"});
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
    m_params.push_back({"Brightness", std::to_string(brightness)});
    m_params.push_back({"Contrast", std::to_string(contrast)});
    m_params.push_back({"Saturation", std::to_string(saturation)});
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

void AlphaOverNode::scaleFallbackImage(Image& fallbackImage, const Image& targetImage, const ImVec4& fallbackColor) {
    fallbackImage.width = targetImage.width;
    fallbackImage.height = targetImage.height;
    fallbackImage.channels = targetImage.channels;
    fallbackImage.pixels.resize(targetImage.width * targetImage.height * targetImage.channels);

    for (size_t i = 0; i < fallbackImage.pixels.size(); i += fallbackImage.channels) {
        fallbackImage.pixels[i] = static_cast<unsigned char>(fallbackColor.x * 255); // R
        fallbackImage.pixels[i + 1] = static_cast<unsigned char>(fallbackColor.y * 255); // G
        fallbackImage.pixels[i + 2] = static_cast<unsigned char>(fallbackColor.z * 255); // B

        if (fallbackImage.channels == 4) {
            fallbackImage.pixels[i + 3] = static_cast<unsigned char>(fallbackColor.w * 255); // A
        }
    }
}

void AlphaOverNode::applyAlphaOver(float alphaFactor) {
    if (!topImagePin->isConnected() && !bottomImagePin->isConnected()) {
        topImage = {512, 512, 4};
        bottomImage = {512, 512, 4};
        scaleFallbackImage(topImage, bottomImage, topFallbackColor);
        scaleFallbackImage(bottomImage, topImage, bottomFallbackColor);
    } else if (topImagePin->isConnected() && !bottomImagePin->isConnected()) {
        topImage = topImagePin->val();
        scaleFallbackImage(bottomImage, topImage, bottomFallbackColor);
    } else if (!topImagePin->isConnected() && bottomImagePin->isConnected()) {
        bottomImage = bottomImagePin->val();
        scaleFallbackImage(topImage, bottomImage, topFallbackColor);
    } else {
        topImage = topImagePin->val();
        bottomImage = bottomImagePin->val();
    }

    if (topImage.width != bottomImage.width || topImage.height != bottomImage.height || topImage.channels != bottomImage.channels) {
        return;
    }

    outputImage.width = topImage.width;
    outputImage.height = topImage.height;
    outputImage.channels = topImage.channels;
    outputImage.pixels.resize(topImage.pixels.size());

    for (size_t i = 0; i < topImage.pixels.size(); i += topImage.channels) {
        float topAlpha = (topImage.channels == 4) ? topImage.pixels[i + 3] / 255.0f : 1.0f;
        float bottomAlpha = (bottomImage.channels == 4) ? bottomImage.pixels[i + 3] / 255.0f : 1.0f;

        float outAlpha = topAlpha * alphaFactor + bottomAlpha * (1.0f - topAlpha * alphaFactor);

        for (size_t j = 0; j < 3; ++j) {
            float topVal = topImage.pixels[i + j] / 255.0f;
            float bottomVal = bottomImage.pixels[i + j] / 255.0f;

            float blendedVal = (topVal * topAlpha * alphaFactor + bottomVal * bottomAlpha * (1.0f - topAlpha * alphaFactor)) / outAlpha;

            outputImage.pixels[i + j] = static_cast<unsigned char>(blendedVal * 255);
        }

        if (topImage.channels == 4) {
            outputImage.pixels[i + 3] = static_cast<unsigned char>(outAlpha * 255);
        }
    }
}

void AlphaOverNode::execute() {
    std::cout << "Executing Node: Alpha Over Node" << "\n";
    applyAlphaOver(alphaFactor);
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

void ExposureNode::applyExposure(float ExposureFactor) {
    if (topImagePin && topImagePin->isConnected()) {
        topImage = topImagePin->val();
    } else {
        topImage = Image(512, 512, 4);
        topImage.pixels.resize(topImage.width * topImage.height * topImage.channels);

        unsigned char r = static_cast<unsigned char>(topFallbackColor.x * 255);
        unsigned char g = static_cast<unsigned char>(topFallbackColor.y * 255);
        unsigned char b = static_cast<unsigned char>(topFallbackColor.z * 255);
        unsigned char a = static_cast<unsigned char>(topFallbackColor.w * 255);

        for (size_t i = 0; i < topImage.pixels.size(); i += 4) {
            topImage.pixels[i] = r;
            topImage.pixels[i + 1] = g;
            topImage.pixels[i + 2] = b;
            topImage.pixels[i + 3] = a;
        }
    }

    auto applyExposure = [](float value, float exposure) -> float {
        return 1.0f - std::exp(-value * exposure);
    };

    outputImage.width = topImage.width;
    outputImage.height = topImage.height;
    outputImage.channels = topImage.channels;
    outputImage.pixels.resize(topImage.pixels.size());

    for (size_t i = 0; i < topImage.pixels.size(); i += topImage.channels) {
        for (size_t j = 0; j < 3; ++j) {
            float topVal = topImage.pixels[i + j] / 255.0f;
            float exposedVal = applyExposure(topVal, exposure);
            outputImage.pixels[i + j] = static_cast<unsigned char>(exposedVal * 255);
        }

        if (topImage.channels == 4) {
            outputImage.pixels[i + 3] = topImage.pixels[i + 3];
        }
    }
}

void ExposureNode::execute() {
    std::cout << "Executing Node: Exposure Node" << "\n";
    applyExposure(exposure);
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

void GammaNode::applyGamma(float Gamma) {
    if (topImagePin && topImagePin->isConnected()) {
        topImage = topImagePin->val();
    } else {
        topImage = Image(512, 512, 4);
        topImage.pixels.resize(topImage.width * topImage.height * topImage.channels);

        unsigned char r = static_cast<unsigned char>(topFallbackColor.x * 255);
        unsigned char g = static_cast<unsigned char>(topFallbackColor.y * 255);
        unsigned char b = static_cast<unsigned char>(topFallbackColor.z * 255);
        unsigned char a = static_cast<unsigned char>(topFallbackColor.w * 255);

        for (size_t i = 0; i < topImage.pixels.size(); i += 4) {
            topImage.pixels[i] = r;
            topImage.pixels[i + 1] = g;
            topImage.pixels[i + 2] = b;
            topImage.pixels[i + 3] = a;
        }
    }

    auto applyGamma = [](float value, float gamma) -> float {
        return std::pow(value, gamma);
    };

    outputImage.width = topImage.width;
    outputImage.height = topImage.height;
    outputImage.channels = topImage.channels;
    outputImage.pixels.resize(topImage.pixels.size());

    for (size_t i = 0; i < topImage.pixels.size(); i += topImage.channels) {
        for (size_t j = 0; j < 3; ++j) {
            float topVal = topImage.pixels[i + j] / 255.0f;
            float gammaCorrectedVal = applyGamma(topVal, gamma);
            outputImage.pixels[i + j] = static_cast<unsigned char>(gammaCorrectedVal * 255);
        }

        if (topImage.channels == 4) {
            outputImage.pixels[i + 3] = topImage.pixels[i + 3];
        }
    }
}

void GammaNode::execute() {
    std::cout << "Executing Node: Gamma Node" << "\n";
    applyGamma(gamma);
}