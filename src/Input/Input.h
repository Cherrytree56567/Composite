#pragma once
#include <ImNodeFlow.h>
#include <imgui.h>
#include <string>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <imgui_filedialog.h>
#include "../NodeTypes.h"

class ImageNode : public ImFlow::BaseNode {
public:
    ImageNode();
    ~ImageNode();
    void draw() override;
    void execute() override;

private:
    bool loadImage(const std::string& filePath);

    bool imageLoaded = false;
    Image imageData;
    ImFileDialogInfo fileDialogInfo;
    bool openFileDialog = false;
    GLuint textureID;
};
