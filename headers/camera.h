#pragma once

#include "math.h"

class Camera 
{
private:
    Vec3f position;
    Vec3f target;
    Vec3f upDir;
    float fov;
    float aspect;
    float near;
    float far;

public:
    Camera(Vec3f Position, Vec3f Target, Vec3f UpDir, float FovDegrees, float AspectRatio, float NearPlane, float FarPlane) 
    {
        position = Position;
        target = Target;
        upDir = normalize(UpDir);
        fov = FovDegrees;
        aspect = AspectRatio;
        near = NearPlane;
        far = FarPlane;
    }

    Mat4f getViewMatrix() const 
    {
        return Mat4f::look_at(position, target, upDir);
    }

    Mat4f getProjectionMatrix() const 
    {
        return Mat4f::perspective(radians(fov), aspect, near, far);
    }

    const Vec3f& getPosition() const 
    {
        return position;
    }
};