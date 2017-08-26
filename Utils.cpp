#include "Utils.hpp"
#define M_PI 3.14159265358979323846
#include <math.h>
glm::mat4x4 Utils::create_perspecitve_projection(float fovy, float aspect, float near, float far){
    float ymax = near * (float) tan(fovy * M_PI / 360.0f);
    float xmax = ymax * aspect;
    return create_perspective_projection_from_bounds(-xmax, xmax, -ymax, ymax, near, far);
}


glm::mat4x4 Utils::create_perspective_projection_from_bounds(float left, float right, float bottom, float top, float near, float far){
    float A, B, C, D, E, F;
    A = (right + left) / (right - left);
    B = (top + bottom) / (top - bottom);
    C = -(far + near) / (far - near);
    D = -2. * far * near / (far - near);
    E = 2. * near / (right - left);
    F = 2. * near / (top - bottom);

    glm::mat4x4 matrix;

    matrix[0][0] = E;
    matrix[0][1] = 0.0f;
    matrix[0][2] = 0.0f;
    matrix[0][3] = 0.0f;

    matrix[1][0] = 0.0f;
    matrix[1][1] = F;
    matrix[1][2] = 0.0f;
    matrix[1][3] = 0.0f;

    matrix[2][0] = A;
    matrix[2][1] = B;
    matrix[2][2] = C;
    matrix[2][3] = -1.0f;

    matrix[3][0] = 0.0f;
    matrix[3][1] = 0.0f;
    matrix[3][2] = D;
    matrix[3][3] = 0.0f;

    return matrix;
}


glm::mat4x4 Utils::create_look_at(glm::vec3 eye, glm::vec3 target, glm::vec3 up){
    glm::vec3 forward = glm::normalize(target - eye);
    glm::vec3 side = glm::normalize(glm::cross(forward, up));
    glm::vec3 up1 = glm::normalize(glm::cross(side, forward));
    glm::mat4x4 toReturn;

    toReturn[0][0] = side[0];
    toReturn[0][1] = up[0];
    toReturn[0][2] = -forward[0];
    toReturn[0][3] = 0.0f;

    toReturn[1][0] = side[1];
    toReturn[1][1] = up[1];
    toReturn[1][2] = -forward[1];
    toReturn[1][3] = 0.0f;

    toReturn[2][0] = side[2];
    toReturn[2][1] = up[2];
    toReturn[2][2] = -forward[2];
    toReturn[2][3] = 0.0f;


    toReturn[3][0] = -glm::dot(side, eye);
    toReturn[3][1] = -glm::dot(up, eye);
    toReturn[3][2] = glm::dot(forward, eye);
    toReturn[3][3] = 1.0f;

    return toReturn;



}


