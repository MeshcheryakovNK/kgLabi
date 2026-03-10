#pragma once

#include <array>
#include <vector>

#include "image.h"
#include "mesh.h"
#include "shader.h"

class Rasterizer 
{
private:
    struct RasterVertex 
    {
        std::array<float, 2> screenPos;
        float depth;
        VertexOutput payLoad;
    };

    Image colorBuffer;
    std::vector<float> zBuffer;

    static Vec3f barycentric(const std::array<float, 2>& a, const std::array<float, 2>& b, const std::array<float, 2>& c, float px, float py);

public:
    Rasterizer(int width, int height);

    void render(const Mesh& mesh, IShader& shader, uint8_t opacity = 255);
    bool MakePNG(const std::string& path) const;

    const Image& image() const 
    {
        return colorBuffer;
    }
};