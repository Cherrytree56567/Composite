#pragma once

struct Image {
    std::vector<unsigned char> pixels;
    int width = 0;
    int height = 0;
    int channels = 0;
    std::string path;
};

struct Vec3 {
    float r;
    float g;
    float b;
};