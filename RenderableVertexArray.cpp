#include "RenderableVertexArray.hpp"


using namespace std;

void RenderableVertexArray::Render(GLuint MODERN_GL_TESTER){
    glUseProgram(this->programID);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, this->vertexbuffer);
    glVertexAttribPointer(
       0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
       3,                  // size
       GL_FLOAT,           // type
       GL_FALSE,           // normalized?
       0,                  // stride
       (void*)0            // array buffer offset
    );
    glDrawArrays(MODERN_GL_TESTER, 0, this->data.size()/3);
    glDisableVertexAttribArray(0);
}

void RenderableVertexArray::getInfo(){
    cout << "VertexArrayID: " << this->VertexArrayID << endl;
    cout << "VertexBuffer: " << this->vertexbuffer << endl;
}


void RenderableVertexArray::setData(GLfloat data[]){
}

