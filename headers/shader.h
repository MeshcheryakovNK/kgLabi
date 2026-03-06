#pragma once

#include <array>

#include "math.h"

struct VertexInput 
{
    Vec3f position;
    Vec3f normal;
};

struct VertexOutput 
{
    Vec4f clipPosition;
    Vec3f worldPosition;
    Vec3f normal;
    float reciprocalW = 1.f;
};

class IShader 
{
public:
    virtual ~IShader() = default;
    virtual VertexOutput vertex(const VertexInput& in) = 0;
    virtual Vec3f fragment(const Vec3f& barycentric, const std::array<VertexOutput, 3>& data) const = 0;
};

class PhongShader : public IShader 
{
private:
    Mat4f model;
    Mat4f view;
    Mat4f projection;

    Mat4f mvp;

    Vec3f lightDir;
    Vec3f lightColor;
    Vec3f fillLightDir;
    Vec3f fillLightColor;

    Vec3f viewPosition;

    Vec3f diffuse;
    Vec3f ambient;
    Vec3f specular;

    float shininess;
    float exposure;

public:
    void setMatrix(const Mat4f& model, const Mat4f& view, const Mat4f& projection);
    void setLightDir(const Vec3f& dir);
    void setLightColor(const Vec3f& color);
    void setFillLight(const Vec3f& dir, const Vec3f& color);
    void setViewPosition(const Vec3f& pos);
    void setMaterial(const Vec3f& ambient, const Vec3f& diffuse, const Vec3f& specular, float shininess);
    void setExposure(float exposure);

    VertexOutput vertex(const VertexInput& in) override;
    Vec3f fragment(const Vec3f& barycentric, const std::array<VertexOutput, 3>& data) const override;
};
