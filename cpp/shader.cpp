#include "shader.h"

void PhongShader::setMatrix(const Mat4f& Model, const Mat4f& View, const Mat4f& Projection) 
{
    model = Model;
    view = View;
    projection = Projection;
    mvp = projection * view * model;
}

void PhongShader::setLightDir(const Vec3f& dir) 
{
    lightDir = normalize(dir);
}

void PhongShader::setLightColor(const Vec3f& color) 
{
    lightColor = color;
}

void PhongShader::setFillLight(const Vec3f& dir, const Vec3f& color) 
{
    fillLightDir = normalize(dir);
    fillLightColor = color;
}

void PhongShader::setViewPosition(const Vec3f& pos) 
{
    viewPosition = pos;
}

void PhongShader::setMaterial(const Vec3f& Ambient, const Vec3f& Diffuse, const Vec3f& Specular, float Shininess)
{
    ambient = Ambient;
    diffuse = Diffuse;
    specular = Specular;
    shininess = Shininess;
}

void PhongShader::setExposure(float Exposure) 
{
    exposure = Exposure;
}

VertexOutput PhongShader::vertex(const VertexInput& in) 
{
    VertexOutput out;
    out.clipPosition = mvp * to_vec4(in.position, 1.f);
    Vec4f world = model * to_vec4(in.position, 1.f);
    out.worldPosition = { world.x, world.y, world.z };
    out.normal = transform_direction(model, in.normal);
    out.reciprocalW = 1.f / out.clipPosition.w;
    return out;
}

Vec3f PhongShader::fragment(const Vec3f& barycentric, const std::array<VertexOutput, 3>& data) const 
{
    float w0 = barycentric.x * data[0].reciprocalW;
    float w1 = barycentric.y * data[1].reciprocalW;
    float w2 = barycentric.z * data[2].reciprocalW;
    float sum = w0 + w1 + w2;
    if (sum == 0.f) 
    {
        return ambient;
    }
    w0 /= sum;
    w1 /= sum;
    w2 /= sum;

    Vec3f position = data[0].worldPosition * w0 + data[1].worldPosition * w1 + data[2].worldPosition * w2;
    Vec3f normal = normalize(data[0].normal * w0 + data[1].normal * w1 + data[2].normal * w2);

    Vec3f LightDir = normalize(-lightDir);
    float diff = std::max(dot(normal, LightDir), 0.f);
    Vec3f Diffuse = diffuse * diff;

    Vec3f ViewDir = normalize(viewPosition - position);
    Vec3f ReflectDir = normalize(2.f * dot(normal, LightDir) * normal - LightDir);
    float spec = std::pow(std::max(dot(ViewDir, ReflectDir), 0.f), shininess);
    Vec3f Specular = specular * spec;

    Vec3f color = (ambient + Diffuse + Specular) * exposure;
    Vec3f keyed = hadamard(color, lightColor);

    Vec3f FillColor = { 0.f, 0.f, 0.f };
    if (length(fillLightColor) > 0.f) {
        float FillDiff = std::max(dot(normal, normalize(-fillLightDir)), 0.f);
        FillColor = hadamard(diffuse * FillDiff, fillLightColor);
    }

    return keyed + FillColor;
}