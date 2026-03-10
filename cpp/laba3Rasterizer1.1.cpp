#include <iostream>

#include "camera.h"
#include "mesh.h"
#include "rasterizer.h"
#include "shader.h"

int main() 
{
    const int width = 1920;
    const int height = 1080;

    Mesh model("meshes/african_head.obj");
    Mesh cube("cube-tex.obj");

    Vec3f CamPos = { -2.f, 1.f,3.f };
    Vec3f Target = { 0.f, 0.f, 0.f };
    Vec3f UpDir = { 0.f, 1.f, 0.f };
    float FOV = 90.f;
    float Aspect = static_cast<float>(width) / static_cast<float>(height);
    float NearPlane = 0.1f;
    float FarPlane = 100.f;

    Camera camera(CamPos, Target, UpDir, FOV, Aspect, NearPlane, FarPlane);

    Mat4f ModelMatrix = Mat4f::translation({ 0.f, -0.05f, 0.f }) * Mat4f::scale({ 1.4f, 1.4f, 1.4f });
    Mat4f ModelMatrix1 = Mat4f::translation({ -1.f, -1.5f, -1.5f }) * Mat4f::scale({ 2.f, 3.f, 2.f });

    Vec3f LightDir = { 0.4f, 0.2f, 0.1f };
    Vec3f LightColor = { 1.f, 0.4f, 0.7f };
    Vec3f FillLightDir = { -0.1f, 0.4f, -0.5f };
    Vec3f FillLightColor = { 0.45f, 0.3f, 1.f };

    Vec3f Ambient = { 0.1f, 0.1f, 0.1f };
    Vec3f Diffuse = { 0.3f, 0.2f, 0.5f };
    Vec3f Specular = { 0.2f, 0.3f, 0.1f };
    float Shininess = 15.f;
    float Exposure = 1.f;

    PhongShader shader;
    shader.setMatrix(ModelMatrix, camera.getViewMatrix(), camera.getProjectionMatrix());
    shader.setLightDir(normalize(LightDir));
    shader.setLightColor(LightColor);
    shader.setFillLight(normalize(FillLightDir), FillLightColor);
    shader.setViewPosition(camera.getPosition());
    shader.setMaterial(Ambient, Diffuse, Specular, Shininess);
    shader.setExposure(Exposure);

    Rasterizer raster(width, height);
    raster.render(model, shader);
    shader.setMatrix(ModelMatrix1, camera.getViewMatrix(), camera.getProjectionMatrix());
    raster.render(cube, shader, 200);



    raster.MakePNG("output.png");
}
