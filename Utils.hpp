#pragma once

struct Vec2d{
    float x, y;

    Vec2d(){
        this->x = 0.0f;
        this->y = 0.0f;
    }

    Vec2d(float iX, float iY){
        this->x = iX;
        this->y = iY;
    }
};

struct Vec3d{
    float x, y, z;
    Vec3d(float iX, float iY){
        this->x = iX;
        this->y = iY;
    }
};

