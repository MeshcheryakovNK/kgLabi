#include "rasterizer.h"

#include <algorithm>
#include <cmath>
#include <limits>

Rasterizer::Rasterizer(int width, int height) 
{
    colorBuffer = Image(width, height);
    colorBuffer.clear({ 0.f, 0.f, 0.f });
    zBuffer = std::vector<float>(width * height, std::numeric_limits<float>::infinity());
}
    
Vec3f Rasterizer::barycentric(const std::array<float, 2>& a, const std::array<float, 2>& b, const std::array<float, 2>& c, float px, float py) 
{
    Vec3f u{ b[0] - a[0], c[0] - a[0], a[0] - px };
    Vec3f v{ b[1] - a[1], c[1] - a[1], a[1] - py };
    Vec3f cross_prod = cross(u, v);
    if (std::abs(cross_prod.z) < 1e-8f) {
        return { -1.f, 1.f, 1.f };
    }

    float inv = 1.f / cross_prod.z;
    float u_coord = cross_prod.x * inv;
    float v_coord = cross_prod.y * inv;
    return { 1.f - u_coord - v_coord, u_coord, v_coord };
}

void Rasterizer::render(const Mesh& model, IShader& shader) 
{
    colorBuffer.clear({ 0.f, 0.f, 0.f });
    std::fill(zBuffer.begin(), zBuffer.end(), std::numeric_limits<float>::infinity());

    int width = colorBuffer.getWidth();
    int height = colorBuffer.getHeight();

    std::array<RasterVertex, 3> verts;

    for (int face = 0; face < model.getFacesCount(); ++face) 
    {
        auto VertexIds = model.getVertexIndecesByFaceID(face);
        auto NormalIds = model.getNormalIndecesByFaceID(face);

        for (int i = 0; i < 3; ++i) 
        {
            VertexInput input{ model.getVertexByIndex(VertexIds[i]), model.getNormalByIndex(NormalIds[i]) };
            VertexOutput output = shader.vertex(input);
            float InvW = output.reciprocalW;
            Vec3f ndc{ output.clipPosition.x * InvW, output.clipPosition.y * InvW, output.clipPosition.z * InvW };

            verts[i].screenPos = {(ndc.x + 1.f) * 0.5f * static_cast<float>(width - 1), (1.f - (ndc.y + 1.f) * 0.5f) * static_cast<float>(height - 1)};
            verts[i].depth = (ndc.z + 1.f) * 0.5f;
            verts[i].payLoad = output;
        }

        float minX = std::min({ verts[0].screenPos[0], verts[1].screenPos[0], verts[2].screenPos[0] });
        float maxX = std::max({ verts[0].screenPos[0], verts[1].screenPos[0], verts[2].screenPos[0] });
        float minY = std::min({ verts[0].screenPos[1], verts[1].screenPos[1], verts[2].screenPos[1] });
        float maxY = std::max({ verts[0].screenPos[1], verts[1].screenPos[1], verts[2].screenPos[1] });

        int x0 = static_cast<int>(std::floor(std::max(0.f, minX)));
        int x1 = static_cast<int>(std::ceil(std::min(static_cast<float>(width - 1), maxX)));
        int y0 = static_cast<int>(std::floor(std::max(0.f, minY)));
        int y1 = static_cast<int>(std::ceil(std::min(static_cast<float>(height - 1), maxY)));

        for (int y = y0; y <= y1; ++y) 
        {
            for (int x = x0; x <= x1; ++x) 
            {
                float px = static_cast<float>(x) + 0.5f;
                float py = static_cast<float>(y) + 0.5f;
                Vec3f bary = barycentric(verts[0].screenPos, verts[1].screenPos, verts[2].screenPos, px, py);
                if (bary.x < 0.f || bary.y < 0.f || bary.z < 0.f) 
                {
                    continue;
                }
                float depth = bary.x * verts[0].depth + bary.y * verts[1].depth + bary.z * verts[2].depth;
                size_t index = static_cast<size_t>(y) * width + x;
                if (depth < zBuffer[index]) 
                {
                    Vec3f color = shader.fragment(bary, { verts[0].payLoad, verts[1].payLoad, verts[2].payLoad });
                    colorBuffer.setPixelColor(x, y, color);
                    zBuffer[index] = depth;
                }
            }
        }
    }
}

bool Rasterizer::MakePNG(const std::string& path) const 
{
    return colorBuffer.makePNG(path);
}