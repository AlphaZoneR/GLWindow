#include "WindowClass.hpp"
#include <GL/gl.h>
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam){
    HDC hdc;
    switch (message){
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
            break;
    }

    return 0;
}


void WindowClass::CreateHWND(Vec2d mSize){
    this->hWnd = CreateWindow(_T(this->windowClass.c_str()), _T(this->windowTitle.c_str()), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, mSize.x, mSize.y, NULL, NULL, this->hInstance, NULL);
    if (!this->hWnd){
        MessageBox(NULL, _T("Call to CreateWindow failed!"), _T(this->windowTitle.c_str()), NULL);
        cout << GetLastError() << endl;
    }
}


void WindowClass::CreateWCEX(){
    this->wcex.cbSize         = sizeof(WNDCLASSEX);
    this->wcex.style          = CS_HREDRAW | CS_VREDRAW;
    this->wcex.lpfnWndProc    = (WNDPROC) &WndProc;
    this->wcex.cbClsExtra     = 0;
    this->wcex.cbWndExtra     = 0;
    this->wcex.hInstance      = this->hInstance;
    this->wcex.hIcon          = LoadIcon(NULL, MAKEINTRESOURCE(32512));
    this->wcex.hCursor        = LoadCursor(NULL, IDC_ARROW);
    this->wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    this->wcex.lpszMenuName   = NULL;
    this->wcex.lpszClassName  = _T(windowClass.c_str());
    this->wcex.hIconSm        = LoadIcon(NULL, MAKEINTRESOURCE(32512));

    if (!RegisterClassEx(&wcex)){
        MessageBox(NULL, _T("Call to RegisterClassEx failed!"), _T(this->windowTitle.c_str()), NULL);
    }

}

bool WindowClass::InitContext(){
    this->pfd = {
        sizeof(PIXELFORMATDESCRIPTOR),	// nSize
        1,								// nVersion
        PFD_DRAW_TO_WINDOW |
        PFD_SUPPORT_OPENGL |
        PFD_GENERIC_ACCELERATED |
        PFD_DOUBLEBUFFER,				// dwFlags
        0,								// iPixelType
        32,								// cColorBits
        0,								// cRedBits
        0,								// cRedShift
        0,								// cGreenBits
        0,								// cGreenShift
        0,								// cBlueBits
        0,								// cBlueShift
        0,								// cAlphaBits
        0,								// cAlphaShift
        0,								// cAccumBits
        0,								// cAccumRedBits
        0,								// cAccumGreenBits
        0,								// cAccumBlueBits
        0,								// cAccumAlphaBits
        24,								// cDepthBits
        8,								// cStencilBits
        0,								// cAuxBuffers
        PFD_MAIN_PLANE,					// iLayerType
        0,								// bReserved
        0,								// dwLayerMask
        0,								// dwVisibleMask
        0,								// dwDamageMask
    };

    HMODULE hinst = GetModuleHandle(0);

    if(!hinst){
        return 0;
    }

    WNDCLASS extClass = {
        CS_OWNDC,
        DefWindowProc,
        0,
        0,
        hinst,
        0,
        0,
        0,
        0,
        "CTXLoader"
    };

    if(!RegisterClass(&extClass)){
        return 0;
    }

    HWND loaderHwnd = CreateWindow(
        "CTXLoader",
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        hinst,
        0
    );

    if(!loaderHwnd){
        return 0;
    }

    HDC loaderHDC = GetDC(loaderHwnd);

    if(!loaderHDC){
        return 0;
    }

    int loaderPixelformat = ChoosePixelFormat(loaderHDC, &this->pfd);

    if(!loaderPixelformat){
        return 0;
    }

    if(!SetPixelFormat(loaderHDC, loaderPixelformat, &this->pfd)){
        return 0;
    }

    HGLRC loaderHglrc = wglCreateContext(loaderHDC);

    if(!loaderHglrc){
        return 0;
    }

    if(!wglMakeCurrent(loaderHDC, loaderHglrc)){
        return 0;
    }

    this->mglCreateContextAttribs = (mglCreateContextAttribsProc)wglGetProcAddress("wglCreateContextAttribsARB");
    if(!this->mglCreateContextAttribs){
        return 0;
    }

    this->mglChoosePixelFormat = (mglChoosePixelFormatProc)wglGetProcAddress("wglChoosePixelFormatARB");
    if(!this->mglChoosePixelFormat){
        return 0;
    }

    if(!wglMakeCurrent(0, 0)){
        return 0;
    }

    if(!wglDeleteContext(loaderHglrc))
        return 0;

    if(!ReleaseDC(loaderHwnd, loaderHDC))
        return 0;

    if(!DestroyWindow(loaderHwnd))
        return 0;

    if(!UnregisterClass("CTXLoader", hinst))
        return 0;

    return true;
}


HGLRC WindowClass::CreateModernContext(HDC hdc, int samples){
    HGLRC context = 0;

	if (mglChoosePixelFormat && mglCreateContextAttribs) {
		while (true) {
			int attributes[] = {
				WGL_SUPPORT_OPENGL, 1,
				WGL_DRAW_TO_WINDOW, 1,
				WGL_DOUBLE_BUFFER, 1,
				WGL_ACCELERATION, WGL_FULL_ACCELERATION,
				WGL_PIXEL_TYPE, WGL_TYPE_RGBA,
				WGL_COLOR_BITS, 24,
				WGL_DEPTH_BITS, 24,
				WGL_SAMPLES, samples,
				0, 0,
			};

			int pixelformat = 0;
			unsigned numFormats = 0;

			if (!mglChoosePixelFormat(hdc, attributes, 0, 1, &pixelformat, &numFormats)) {
				continue;
			}

			if (numFormats) {
				if (!SetPixelFormat(hdc, pixelformat, &pfd)) {
					continue;
				}

				break;
			}

			if (!samples) {
				break;
			}

			samples /= 2;
		}

		int major[] = {4, 4, 4, 4, 4, 4, 3, 3, 3, 3, 0};
		int minor[] = {5, 4, 3, 2, 1, 0, 3, 2, 1, 0, 0};

		for (int i = 0; i < 10; ++i) {
			int attriblist[] = {
				WGL_CONTEXT_PROFILE_MASK, WGL_CONTEXT_CORE_PROFILE_BIT,
				WGL_CONTEXT_MAJOR_VERSION, major[i],
				WGL_CONTEXT_MINOR_VERSION, minor[i],
				0, 0,
			};

			context = mglCreateContextAttribs(hdc, 0, attriblist);

			if (context) {
				break;
			}
		}
	}

	if (!context) {
		int pixelformat = ChoosePixelFormat(hdc, &pfd);

		if (!pixelformat) {
			return 0;
		}

		if (!SetPixelFormat(hdc, pixelformat, &pfd)) {
			return 0;
		}

		context = wglCreateContext(hdc);
	}

	return context;
}

bool WindowClass::UpdateR(){
    SwapBuffers(this->hdc);
    if(GetMessage(&this->msg, NULL, 0, 0)){
        TranslateMessage(&this->msg);
        DispatchMessage(&this->msg);
        return this->running;
    }

}


bool WindowClass::CreateContext(){
    this->hdc = GetDC(this->hWnd);
    if(!this->hdc){
        cout << "HWND is null";
        return 0;
    }
    this->hrc = this->CreateModernContext(this->hdc, 16);
    if(!wglMakeCurrent(this->hdc, this->hrc)){
        cout << "Cannot wglMakeCurrent" << endl;
        return 0;
    }
    this->PreInit();


    return 1;

}

void WindowClass::setFullscreen(){

    this->width = GetSystemMetrics(0);
    this->heigth = GetSystemMetrics(1);

    SetWindowLong(this->hWnd, GWL_EXSTYLE, 0);
    SetWindowLong(this->hWnd, GWL_STYLE, WS_POPUP);
    SetWindowPos(this->hWnd, HWND_TOP, 0, 0, this->width, this->heigth, 0);

    glViewport(0, 0, this->width, this->heigth);
}

void WindowClass::setWindowed(int w, int h){
    RECT rect = {};
    rect.right = w;
    rect.bottom = h;

    int style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU;

    AdjustWindowRect(&rect, style, 0);

    int aWidth = rect.right - rect.left;
    int aHeigth = rect.bottom - rect.top;

    this->width = aWidth;
    this->heigth = aHeigth;

    int sw = GetSystemMetrics(0);
    int sy = GetSystemMetrics(1);

    int x = (sw - w) / 2;
    int y = (sy - h) / 2;

    if (y < 0){
        y = 0;
    }

    SetWindowLong(this->hWnd, GWL_EXSTYLE, WS_EX_DLGMODALFRAME);
    SetWindowLong(this->hWnd, GWL_STYLE, style);
    SetWindowPos(this->hWnd, HWND_TOP, x, y, this->width, this->heigth, 0);

    glViewport(0, 0, w, h);
}

HDC WindowClass::getHdc(){
    return this->hdc;
}

Vec2d WindowClass::getSize(){
    return Vec2d(this->width, this->heigth);
}


int WindowClass::getWidth(){
    return this->width;
}


int WindowClass::getHeigth(){
    return this->heigth;
}


GLuint WindowClass::LoadShader(std::string VertexShader, std::string FragmentShader){
    GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
    GLint Result = GL_FALSE;
    int infolen;

    char const * VertexSourcePointer = VertexShader.c_str();
    glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
    glCompileShader(VertexShaderID);

    glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &infolen);
    if(infolen){
        std::vector<char> VertexShaderErrorMessage(infolen+1);
		glGetShaderInfoLog(VertexShaderID, infolen, NULL, &VertexShaderErrorMessage[0]);
		cout << &VertexShaderErrorMessage[0] << endl;
    }

    char const * FragmentSourcePointer = FragmentShader.c_str();
    glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
    glCompileShader(FragmentShaderID);

    glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &infolen);
    if(infolen){
        std::vector<char> FragmenShaderErrorMessage(infolen+1);
		glGetShaderInfoLog(VertexShaderID, infolen, NULL, &FragmenShaderErrorMessage[0]);
		cout << &FragmenShaderErrorMessage[0] << endl;
    }

    GLuint ProgramId = glCreateProgram();
    glAttachShader(ProgramId, VertexShaderID);
    glAttachShader(ProgramId, FragmentShaderID);
    glLinkProgram(ProgramId);

    glGetProgramiv(ProgramId, GL_LINK_STATUS, &Result);
    glGetProgramiv(ProgramId, GL_INFO_LOG_LENGTH, &infolen);

    if(infolen){
        std::vector<char> ProgramErrorMessage(infolen+1);
        glGetProgramInfoLog(ProgramId, infolen, NULL, &ProgramErrorMessage[0]);
        cout << &ProgramErrorMessage[0] << endl;
    }

    glDetachShader(ProgramId, VertexShaderID);
    glDetachShader(ProgramId, FragmentShaderID);

    glDeleteShader(VertexShaderID);
    glDeleteShader(FragmentShaderID);

    return ProgramId;

}



float WindowClass::getTime(){
    return (this->now - this->start);
}


bool WindowClass::updateUniformFloat(float value, GLint programID, std::string uniformName){
    GLint loc = glGetUniformLocation(programID, uniformName.c_str());
    if(loc >= 0){
        glUniform1f(loc, value);
        return 1;
    }
    return 0;
}

bool WindowClass::updateUniformMat4(glm::mat4x4 lookAtMatrix, GLint programID, std::string uniformName){
    GLint loc = glGetUniformLocation(programID, uniformName.c_str());
    if(loc >= 0){
        glUniformMatrix4fv(loc, 1, 0, &lookAtMatrix[0][0]);
        return 1;
    }
    return 0;
}



