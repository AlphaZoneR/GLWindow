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
            \ngl_Position = Mvp *  vec4(in_verts.xy * vec2(1.0f, 1.77f)  * Scale, in_verts.z * Scale, 1.0);\
            \ngl_PointSize = 3.0f;\
        \n}";

    string FragmentShader1 =
        "#version 330\
        \nout vec4 color;\
        \nvoid main(){\
            \ncolor = vec4(0.3, 0.5, 1.0, 1.0);\
        }";

    float g_vertex_buffer_data[] ={
        0.0f, 0.0f, 0.0f,
        0.75f, 0.0f, 0.0f,
        0.0f, 0.75f, 0.0f,

        0.0f, 0.0f, 0.0f,
        0.75f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.75f,

        0.0f, 0.0f, 0.0f,
        0.0f, 0.75f, 0.0f,
        0.0f, 0.0f, 0.75f,

        0.75f, 0.0f, 0.0f,
        0.0f, 0.75f, 0.0f,
        0.0f, 0.0f, 0.75f,
    };
    this->program = this->LoadShader(VertexShader1, FragmentShader1);
    this->test = RenderableVertexArray(g_vertex_buffer_data, this->program, sizeof(g_vertex_buffer_data)/sizeof(float));

}


void WindowClass::Update(){
}

void WindowClass::Render(){
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    float time = this->getTime()/1000;
    this->test.Render(GL_TRIANGLES);
    this->updateUniformFloat(1.0f, this->program, "Scale");

    glm::mat4x4 proj = Utils::create_perspecitve_projection(45.0f, (float) this->getWidth()/ (float) this->getHeigth(), 0.1f, 1000.0f);

    glm::vec3 eye = glm::vec3(cos(time), sin(time), 0.5);
    glm::vec3 target = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 up = glm::vec3(0.0f, 0.0f, 1.0f);

    glm::mat4x4 lookat = Utils::create_look_at(eye, target, up);

    glm::mat4x4 mvp = proj * lookat;

    this->updateUniformMat4(mvp, this->program, "Mvp");



}

int main()
{
    WindowClass window = WindowClass("GLWindow", 1366, 784, true, false);
}



