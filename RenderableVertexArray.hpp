#pragma once
#define GLEW_STATIC


#include <GL/glew.h>
#include <GL/wglew.h>
#include <iostream>
#include <GL/gl.h>

#include <vector>

using namespace std;

class RenderableVertexArray{
private:
    GLuint VertexArrayID, vertexbuffer, programID;
    float * vertexBufferData;
    vector<float> data;
public:
    RenderableVertexArray(float bufferData[], GLuint prID, int mSize){
        this->programID = prID;

        glGenVertexArrays(1, &this->VertexArrayID);
        glBindVertexArray(this->VertexArrayID);

        for(int i = 0 ; i < mSize ; i++){
            this->data.push_back(bufferData[i]);
        }

        glGenBuffers(1, &this->vertexbuffer);
        glBindBuffer(GL_ARRAY_BUFFER, this->vertexbuffer);
        glBufferData(GL_ARRAY_BUFFER, this->data.size() * sizeof(float), &this->data.front(), GL_DYNAMIC_DRAW);


    }
    RenderableVertexArray(){
        this->VertexArrayID = 0;
        this->vertexbuffer = 0;
    }


    void Render(GLuint);

    void getInfo();

    void setData(GLfloat[]);
};
