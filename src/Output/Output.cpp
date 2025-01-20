#include "Output.h"
#include "stb_image.h"  // Include this if you're using stb_image for loading images

OutputNode::OutputNode() {
    setTitle("Composite");
    setStyle(ImFlow::NodeStyle::cyan());
    imageDataPin = addIN<Image>("Image", imageData, ImFlow::ConnectionFilter::SameType());
    imageData = imageDataPin->val();

    // Initial texture loading
    if (!imageData.pixels.empty() && textureID == 0) {
        loadTexture(imageData);
    }
}

OutputNode::~OutputNode() {
    if (textureID != 0) {
        glDeleteTextures(1, &textureID);
    }
}

void OutputNode::execute() {
    m_insa.clear();
    if (imageDataPin->isConnected()) {
        auto parent = imageDataPin->getLink().lock()->left()->getParent();
        m_insa.push_back({parent->getName(), std::to_string(parent->getUID())});
    }
}

void OutputNode::draw() {

    glDeleteTextures(1, &textureID);  // Delete old texture
        
    loadTexture(imageData);  // Load new texture
    lastImagePath = imageData.path;  // Update the last image path

    if (textureID == 0) {
        if (!imageLoaded) {
            ImGui::Text("No Image");
        }
    }

    if (textureID != 0) {
        float aspectRatio = static_cast<float>(imageData.width) / static_cast<float>(imageData.height);
        float displayWidth = 200.0f;
        float displayHeight = displayWidth / aspectRatio;

        ImGui::Image(textureID, ImVec2(displayWidth, displayHeight));
    }
}

// This function will load image data into a texture if necessary
void OutputNode::loadTexture(const Image& image) {
    if (image.pixels.empty() && !image.path.empty()) {
        int width, height, channels;
        unsigned char* data = stbi_load(image.path.c_str(), &width, &height, &channels, 0);
        if (data) {
            imageData.width = width;
            imageData.height = height;
            imageData.channels = channels;
            imageData.pixels.resize(width * height * channels);
            memcpy(imageData.pixels.data(), data, width * height * channels);
            stbi_image_free(data);
        }
    }

    // If the image was successfully loaded
    if (!imageData.pixels.empty()) {
        glGenTextures(1, &textureID);  // Create texture and store it in the class
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, (imageData.channels == 4 ? GL_RGBA : GL_RGB), imageData.width, imageData.height, 0, 
                     (imageData.channels == 4 ? GL_RGBA : GL_RGB), GL_UNSIGNED_BYTE, imageData.pixels.data());
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}
