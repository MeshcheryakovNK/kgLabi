#include "mesh.h"

#include <fstream>
#include <sstream>

std::vector<std::string> tokenizeFace(const std::string& token)
{
    std::vector<std::string> parts;
    std::string current;
    for (char ch : token) 
    {
        if (ch == '/') 
        {
            parts.push_back(current);
            current.clear();
        }
        else 
        {
            current.push_back(ch);
        }
    }
    parts.push_back(current);
    return parts;
}

Mesh::Mesh(const std::string& path) 
{
    std::ifstream file(path);

    std::string line;
    while (std::getline(file, line)) 
    {
        if (line.empty()) 
        {
            continue;
        }

        std::istringstream stream(line);
        std::string prefix;

        stream >> prefix;
        if (prefix == "v")
        {
            float x, y, z;
            stream >> x >> y >> z;
            vertices.push_back({ x, y, z });
        }
        else if (prefix == "vn") 
        {
            float x, y, z;
            stream >> x >> y >> z;
            normals.push_back(normalize({ x, y, z }));
        }
        else if (prefix == "f") 
        {
            Face face;
            for (int i = 0; i < 3; ++i) 
            {
                std::string token;
                stream >> token;
                if (token.empty()) 
                {
                    continue;
                }
                std::vector<std::string> parts = tokenizeFace(token);
                int vertexID = parts.size() > 0 && !parts[0].empty() ? std::stoi(parts[0]) : 0;
                int normalID = parts.size() > 2 && !parts[2].empty() ? std::stoi(parts[2]) : 0;
                face.vertexIds[i] = vertexID - 1;
                face.normalIds[i] = normalID > 0 ? normalID - 1 : -1;
            }
            faces.push_back(face);
        }
    }

    if (normals.empty()) 
    {
        normals.resize(vertices.size(), Vec3f{});
        std::vector<int> counts(vertices.size(), 0);
        for (auto& face : faces) 
        {
            Vec3f v0 = vertices[face.vertexIds[0]];
            Vec3f v1 = vertices[face.vertexIds[1]];
            Vec3f v2 = vertices[face.vertexIds[2]];
            Vec3f n = normalize(cross(v1 - v0, v2 - v0));
            for (int i = 0; i < 3; ++i) 
            {
                int idx = face.vertexIds[i];
                normals[idx] += n;
                counts[idx] += 1;
                face.normalIds[i] = idx;
            }
        }
        for (int i = 0; i < normals.size(); ++i) 
        {
            if (counts[i] > 0) 
            {
                normals[i] = normalize(normals[i]);
            }
        }
    }
    else 
    {
        for (auto& face : faces) 
        {
            for (int i = 0; i < 3; ++i) 
            {
                if (face.normalIds[i] < 0) 
                {
                    face.normalIds[i] = face.vertexIds[i];
                }
            }
        }
    }
}

std::array<int, 3> Mesh::getVertexIndecesByFaceID(int face_id) const
{
    return faces[face_id].vertexIds;
}

std::array<int, 3> Mesh::getNormalIndecesByFaceID(int face_id) const
{
    return faces[face_id].normalIds;
}

Vec3f Mesh::getVertexByIndex(int index) const 
{
    return vertices[index];
}

Vec3f Mesh::getNormalByIndex(int index) const 
{
    return normals[index];
}

int Mesh::getFacesCount() const 
{
    return faces.size(); 
}