#pragma once
#include <glm/glm.hpp>
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


namespace Utils{
    glm::mat4x4 create_perspecitve_projection(float, float, float , float);
    glm::mat4x4 create_perspective_projection_from_bounds(float, float, float, float, float, float);
    glm::mat4x4 create_look_at(glm::vec3, glm::vec3, glm::vec3);
}
