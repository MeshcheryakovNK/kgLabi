#include "image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

Image::Image() {};

Image::Image(int Width, int Height) 
{
    width = Width; 
    height = Height;
    pixels = std::vector<uint8_t>(static_cast<size_t>(width) * height * 4, 0);
}


void Image::clear(const Vec3f& color) 
{
    const uint8_t r = static_cast<uint8_t>(clamp(color.x, 0.f, 1.f) * 255.f);
    const uint8_t g = static_cast<uint8_t>(clamp(color.y, 0.f, 1.f) * 255.f);
    const uint8_t b = static_cast<uint8_t>(clamp(color.z, 0.f, 1.f) * 255.f);
    for (size_t i = 0; i < pixels.size(); i += 4) 
    {
        pixels[i] = r;
        pixels[i + 1] = g;
        pixels[i + 2] = b;
        pixels[i + 3] = 255;
    }
}

void Image::setPixelColor(int x, int y, const Vec3f& color, uint8_t opacity) 
{
    if (x < 0 || x >= width || y < 0 || y >= height) 
    {
        return;
    }

    size_t index = (static_cast<size_t>(y) * width + x) * 4;
    if (opacity == 255) 
    {
        pixels[index] = static_cast<uint8_t>(clamp(color.x, 0.f, 1.f) * 255.f);
        pixels[index + 1] = static_cast<uint8_t>(clamp(color.y, 0.f, 1.f) * 255.f);
        pixels[index + 2] = static_cast<uint8_t>(clamp(color.z, 0.f, 1.f) * 255.f);
        pixels[index + 3] = opacity;
    }
    else 
    {
        pixels[index] = (pixels[index] + static_cast<uint8_t>(clamp(color.x, 0.f, 1.f) * 255.f)) / 2;
        pixels[index + 1] = (pixels[index + 1] + static_cast<uint8_t>(clamp(color.y, 0.f, 1.f) * 255.f)) / 2;
        pixels[index + 2] = (pixels[index + 2] + static_cast<uint8_t>(clamp(color.z, 0.f, 1.f) * 255.f)) / 2;
        pixels[index + 3] = opacity;
    }
}

bool Image::makePNG(const std::string& path) const 
{
    int stride = width * 4;
    return stbi_write_png(path.c_str(), width, height, 4, pixels.data(), stride) != 0;
}