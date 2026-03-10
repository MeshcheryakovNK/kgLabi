#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "math.h"

class Image 
{
private:
    int width;
    int height;
    std::vector<uint8_t> pixels;

public:
    Image();
    Image(int width, int height);

    void clear(const Vec3f& color);
    void setPixelColor(int x, int y, const Vec3f& color, uint8_t opacity);
    bool makePNG(const std::string& path) const;

    int getWidth() const 
    { 
        return width;
    }

    int getHeight() const 
    { 
        return height;
    }
};