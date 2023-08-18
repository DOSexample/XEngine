#pragma once

#include <Windows.h>

namespace X {



    class Win32Native {
    public:
        Win32Native();
        ~Win32Native()
            ;
        bool Initialize(HINSTANCE hInstance, const char* windowTitle, int width, int height, int nCmdShow);
        bool Initialize(HINSTANCE hInstance, const wchar_t* windowTitle, int width, int height, int nCmdShow);

        HWND GetHWND() const { return m_hWnd; }
        int GetWidth() const { return m_width; }
        int GetHeight() const { return m_height; }
        const char* GetTitleA() const { return m_windowTitleA; }
        const wchar_t* GetTitleW() const { return m_windowTitleW; }

        bool ProcessMessages(MSG* msg);

        void SetTitle( const char* title );
        void SetTitle( const wchar_t* title );

    private:
        HINSTANCE m_hInstance;
        const char* m_windowTitleA;
        const wchar_t* m_windowTitleW;
        int m_width = 800;
        int m_height = 600;
        HWND m_hWnd = nullptr;
    };

}