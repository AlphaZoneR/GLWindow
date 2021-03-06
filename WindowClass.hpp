#pragma once
#define _WIN32_WINNT 0x0500


#include "Utils.hpp"
#include "RenderableVertexArray.hpp"
#include "Texture2D.hpp"



using namespace std;




struct Mouse{
    int x, y;
    POINT p;
    Mouse(){
        this->x = 0;
        this->y = 0;
    }

    void updateX(int m_x){
        this->x = m_x;
    }

    void updateY(int m_y){
        this->y = m_y;
    }

    int getX(){
        return this->p.x;
    }

    int getY(){
        return this->p.y;
    }

};




class WindowClass{
private:
    typedef int (WINAPI * mglChoosePixelFormatProc)(HDC hdc, const int * piAttribIList, const float * pfAttribFList, unsigned nMaxFormats, int * piFormats, unsigned * nNumFormats);
    typedef HGLRC (WINAPI * mglCreateContextAttribsProc)(HDC hdc, HGLRC hglrc, const int * attribList);


    HINSTANCE hInst, hInstance, hInstC;
    HWND hWnd, consoleHWND;
    WNDCLASSEX wcex;
    int width, heigth;
    MSG msg;
    bool running;
    PIXELFORMATDESCRIPTOR pfd;
    mglChoosePixelFormatProc mglChoosePixelFormat;
    mglCreateContextAttribsProc mglCreateContextAttribs;
    HDC hdc;
    HGLRC hrc;
    GLuint vertexbuffer;
    GLuint program;
    RenderableVertexArray test, test1;
    Texture2D d;
    std::clock_t start, now;
    const unsigned WGL_ACCELERATION             = 0x2003;
    const unsigned WGL_COLOR_BITS               = 0x2014;
    const unsigned WGL_CONTEXT_FLAGS            = 0x2094;
    const unsigned WGL_CONTEXT_MAJOR_VERSION    = 0x2091;
    const unsigned WGL_CONTEXT_MINOR_VERSION    = 0x2092;
    const unsigned WGL_DEPTH_BITS               = 0x2022;
    const unsigned WGL_DOUBLE_BUFFER            = 0x2011;
    const unsigned WGL_DRAW_TO_WINDOW           = 0x2001;
    const unsigned WGL_FULL_ACCELERATION        = 0x2027;
    const unsigned WGL_PIXEL_TYPE               = 0x2013;
    const unsigned WGL_SAMPLES                  = 0x2042;
    const unsigned WGL_STENCIL_BITS             = 0x2023;
    const unsigned WGL_SUPPORT_OPENGL           = 0x2010;
    const unsigned WGL_TYPE_RGBA                = 0x202B;
    const unsigned WGL_CONTEXT_PROFILE_MASK     = 0x9126;
    const unsigned WGL_CONTEXT_CORE_PROFILE_BIT = 0x0001;

public:
    std::string windowClass = "win32api";
    std::string windowTitle = "GLWindow";
    Mouse mouse;



    int getHeigth();
    int getWidth();

    void CreateWCEX();
    void CreateHWND(Vec2d);
    void setFullscreen();
    void setWindowed(int, int);
    void PreInit();
    void Init();
    void Render();
    void Update();
    void mainFunc();

    bool UpdateR();
    bool InitContext();
    bool CreateContext();


    float getTime();

    Vec2d getSize();

    HDC getHdc();

    HGLRC CreateModernContext(HDC, int);

    GLuint LoadShader(std::string, std::string);

    glm::vec3 generateRandomColorVector();




    WindowClass(string title, int mwidth, int mheight, bool console, bool fullscreen){
        if(!console){
            this->consoleHWND = GetConsoleWindow();
            ShowWindow(this->consoleHWND, 0);
        }

        this->hInstance = (HINSTANCE)GetWindowLong(this->consoleHWND, GWL_HINSTANCE);
        this->width = mwidth;
        this->heigth = mheight;
        this->windowTitle = title;

        this->CreateWCEX();
        this->CreateHWND(Vec2d(this->width, this->heigth));

        if(this->InitContext()){
            if(!this->CreateContext()){
                PostQuitMessage(0);
            }else{
                if(fullscreen){
                    this->setFullscreen();
                }else{
                    this->setWindowed(this->width, this->heigth);
                }

            }
        }else{
            PostQuitMessage(0);
        }
        running = true;

        ShowWindow(this->hWnd, SW_SHOW);
        SetForegroundWindow(this->hWnd);
        SetActiveWindow(this->hWnd);
        SetFocus(this->hWnd);
        UpdateWindow(this->hWnd);

        this->Init();


        this->mainFunc();
    }





};
