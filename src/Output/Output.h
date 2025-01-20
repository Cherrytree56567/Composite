#pragma once
#include <ImNodeFlow.h>
#include <imgui.h>
#include <string>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <imgui_filedialog.h>
#include "../NodeTypes.h"

class OutputNode : public ImFlow::BaseNode {
public:
    OutputNode();
    ~OutputNode();
    void draw() override;
    void execute() override;
    void writeImageData(Image s) { imageData = s; }

private:
    void loadTexture(const Image& image);

    bool imageLoaded = false;
    Image imageData;
    std::shared_ptr<ImFlow::InPin<Image>> imageDataPin;
    ImFileDialogInfo fileDialogInfo;
    bool openFileDialog = false;
    GLuint textureID;
    std::string lastImagePath;
};
