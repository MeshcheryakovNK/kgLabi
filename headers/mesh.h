#pragma once

#include <array>
#include <string>
#include <vector>

#include "math.h"

class Mesh 
{
private:
    struct Face 
    {
        std::array<int, 3> vertexIds;
        std::array<int, 3> normalIds;
    };

    std::vector<Vec3f> vertices;
    std::vector<Vec3f> normals;
    std::vector<Face> faces;

public:
    Mesh(const std::string& path);

    std::array<int, 3> getVertexIndecesByFaceID(int faceID) const;
    std::array<int, 3> getNormalIndecesByFaceID(int faceID) const;
    Vec3f getVertexByIndex(int index) const;
    Vec3f getNormalByIndex(int index) const;
    int getFacesCount() const;
};