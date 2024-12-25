#include "Input.h"
#include <stb_image.h>
#include <GLFW/glfw3.h>
#include <imgui.h>

ImageNode::ImageNode() {
    setTitle("Image");
    setStyle(ImFlow::NodeStyle::cyan());
    addOUT<Image>("Image")->behaviour([this]() { return imageData; });

    fileDialogInfo.title = "Open File";
    fileDialogInfo.directoryPath = std::filesystem::current_path();
    fileDialogInfo.type = ImGuiFileDialogType_OpenFile;
    fileDialogInfo.fileFilterFunc = std::function<bool(std::string)>([](std::string filename) {
        return filename.ends_with(".png") || filename.ends_with(".jpg");
    });
}

ImageNode::~ImageNode() {
    // Delete the OpenGL texture if it exists
    if (textureID != 0) {
        glDeleteTextures(1, &textureID);
    }
}

void ImageNode::draw() {
    if (!imageLoaded) {
        if (ImGui::Button("Choose Image")) {
            openFileDialog = true;
        }

        if (ImGui::FileDialog(&openFileDialog, &fileDialogInfo)) {
            if (fileDialogInfo.resultPath.c_str() != nullptr && loadImage((std::string)fileDialogInfo.resultPath.string())) {
                imageLoaded = true;
            }
        }
    } else {
        // Use a basic ImGui Image that can show the image data as a texture.
        float aspectRatio = static_cast<float>(imageData.width) / static_cast<float>(imageData.height);
        float displayWidth = 200.0f;
        float displayHeight = displayWidth / aspectRatio;

        // ImGui needs a texture ID, so use the OpenGL texture generated from the pixel data
        ImGui::Image(textureID, ImVec2(displayWidth, displayHeight));

        if (ImGui::Button("Choose Image")) {
            openFileDialog = true;
        }

        if (ImGui::FileDialog(&openFileDialog, &fileDialogInfo)) {
            if (fileDialogInfo.resultPath.c_str() != nullptr && loadImage((std::string)fileDialogInfo.resultPath.string())) {
                imageLoaded = true;
            }
        }
    }
}

bool ImageNode::loadImage(const std::string& filePath) {
    int width, height, channels;
    unsigned char* data = stbi_load(filePath.c_str(), &width, &height, &channels, 0);
    if (data) {
        // Store pixel data in the Image struct
        imageData.pixels.resize(width * height * channels);
        std::memcpy(imageData.pixels.data(), data, width * height * channels);

        // Generate an OpenGL texture from the pixel data
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, (channels == 4 ? GL_RGBA : GL_RGB), width, height, 0, (channels == 4 ? GL_RGBA : GL_RGB), GL_UNSIGNED_BYTE, data);

        // Set texture parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, 0);

        // Save image data in the struct for later use
        imageData.width = width;
        imageData.height = height;
        imageData.channels = channels;
        imageData.path = filePath;

        // Free the loaded image data
        stbi_image_free(data);

        return true;
    }
    return false;
}
