#pragma once
#define M_PI 3.14159265358979323846


#include <windows.h>
#include <stdlib.h>
#include <tchar.h>

#include <math.h>
#include <string>
#include <iostream>
#include <vector>
#include <ctime>
#include <random>

// Include glew
#define GLEW_STATIC
#include <GL/glew.h>
#include <GL/wglew.h>


//Include OpenGL
#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/glu.h>
#include <GL/wglew.h>

// Include GLFW
#include <GLFW/glfw3.h>

// Include GLM

#include <glm/glm.hpp>

#include <glm/ext.hpp>

#include "FreeImage.h"

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
    int LoadShader(std::string, std::string);
    bool updateUniformFloat(float, GLint, std::string);
    bool updateUniformMat4(glm::mat4x4, GLint, std::string);
    bool updateUniformVec3(glm::vec3, GLint, std::string);
}
