#include <iostream>
#include "WindowClass.hpp"


using namespace std;
using namespace Utils;


void WindowClass::PreInit(){
    glewInit();


}


void WindowClass::Init(){
    glClearColor(0.9f, 0.9f, 0.9f, 1.0f);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glAlphaFunc(GL_GREATER, 0.0f);
    glDepthFunc(GL_LEQUAL);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_BLEND);

    string VertexShader1 =
        "#version 330\
        \nin vec3 in_verts;\
        \nuniform float Scale;\
        \nuniform mat4 Mvp;\
        \nvoid main(){\
            \ngl_Position = Mvp * vec4(in_verts.xy * Scale, in_verts.z * Scale, 1.0);\
            \ngl_PointSize = 3.0f;\
        \n}";

    string FragmentShader1 =
        "#version 330\
        \nout vec4 color;\
        \nvoid main(){\
            \ncolor = vec4(0.3, 0.5, 1.0, .5f);\
        }";

    d.LoadTexture("test.png");
    d.setScale(0.5);

    float g_vertex_buffer_data[] ={
        -1.0f, 1.0f, 0.0f,
        -1.0f, -1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,

        1.0f, -1.0f, 0.0f,
        -1.0f, -1.0f, 0.0f,
        1.0f, 1.0f, 0.0f
    };
    this->program = Utils::LoadShader(VertexShader1, FragmentShader1);
    this->test = RenderableVertexArray(g_vertex_buffer_data, this->program, sizeof(g_vertex_buffer_data)/sizeof(float));



}


void WindowClass::Update(){
    float time = this->getTime()/1000;
    Utils::updateUniformFloat(0.5f, this->program, "Scale");
    glm::mat4x4 proj = glm::perspective(45.0f, (float) this->getWidth()/ (float) this->getHeigth(), 0.1f, 100.0f);
    glm::vec3 eye = glm::vec3(cos(time) , sin(time), 1);
    glm::vec3 target = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

    glm::mat4x4 lookat = glm::lookAt(eye, target, up);
    glm::mat4x4 mvp = proj * lookat;

    Utils::updateUniformMat4(mvp, this->program, "Mvp");
}

void WindowClass::Render(){
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    //this->test.Render(GL_TRIANGLES);
    this->d.Render();
}

int main()
{
    WindowClass window = WindowClass("GLWindow", 1366, 784, true, false);
}



