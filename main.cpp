#include <iostream>
#include "WindowClass.hpp"

using namespace std;

void WindowClass::Init(){
    glClearColor(0.75f, 0.75f, 0.75f, 1.0f);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glAlphaFunc(GL_GREATER, 0.0f);
    glDepthFunc(GL_LEQUAL);

    glDisable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_BLEND);
}


void WindowClass::Update(){
}

void WindowClass::Render(){
    int aspect = 1.29f;
    int height = this->getHeigth();
    int width = this->getWidth();


    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(70, aspect, 0.1, 1000);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(140, 140, 120, 0, 0, 0, 0, 0, 1);

    glColor3ub(64, 64, 64);
    glBegin(GL_LINES);
        for (float c = -70; c <= 70; c += 10) {
            glVertex3f(-70, c, 0);
            glVertex3f(70, c, 0);
            glVertex3f(c, -70, 0);
            glVertex3f(c, 70, 0);
        }
    glEnd();
}

int main()
{
    WindowClass window = WindowClass("GLWindow", 1280, 720, true, false);
}
