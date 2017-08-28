#pragma once

#include "Utils.hpp"


class Texture2D{
private:
    FIBITMAP * image;
    BYTE * bits;
    FREE_IMAGE_FORMAT fif;
    int width, height;
    float scale;
    GLuint textureID;
    std::string vertexShader, fragmentShader;
    GLuint programID, VertexArrayID, vertexbuffer;
    std::vector<float> data;
public:

    Texture2D(){
        this->fif = FIF_UNKNOWN;
        this->width = 0;
        this->height = 0;
        this->bits = BYTE(0);
        this->scale = 1;
    }

    bool LoadTexture(std::string);
    void Render();
    void setScale(float);

};
