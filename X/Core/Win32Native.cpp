#include <X/Core/Win32Native.h>

#include <string>

#pragma comment(lib, "Winmm.lib")

namespace X {

    LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
        switch (message) {
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        }
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    Win32Native::Win32Native()
        : m_hInstance(nullptr), m_windowTitleA(nullptr), m_windowTitleW(nullptr), m_width(800), m_height(600) {}

    Win32Native::~Win32Native() {
        if (m_hWnd) DestroyWindow(m_hWnd);

        if(m_windowTitleA) UnregisterClassA(m_windowTitleA, m_hInstance);

        if(m_windowTitleW) UnregisterClassW(m_windowTitleW, m_hInstance);
    }

    bool Win32Native::Initialize(HINSTANCE hInstance, const char* windowTitle, int width, int height, int nCmdShow) {
        m_width = width;
        m_height = height;
        m_windowTitleA = windowTitle;

        WNDCLASSEXA wc = { sizeof(WNDCLASSEXA), CS_CLASSDC, WindowProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, m_windowTitleA, NULL };
        RegisterClassExA(&wc);

        m_hWnd = CreateWindowExA(0, wc.lpszClassName, m_windowTitleA, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, m_width, m_height, NULL, NULL, wc.hInstance, NULL);

        if (!m_hWnd) {
            return false;
        }

        ShowWindow(m_hWnd, nCmdShow);
        UpdateWindow(m_hWnd);

        return true;
    }

    bool Win32Native::Initialize(HINSTANCE hInstance, const wchar_t* windowTitle, int width, int height, int nCmdShow) {
        m_width = width;
        m_height = height;
        m_windowTitleW = windowTitle;

        WNDCLASSEXW wc = { sizeof(WNDCLASSEXW), CS_CLASSDC, WindowProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, m_windowTitleW, NULL };
        RegisterClassExW(&wc);

        m_hWnd = CreateWindowExW(0, wc.lpszClassName, m_windowTitleW, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, m_width, m_height, NULL, NULL, wc.hInstance, NULL);

        if (!m_hWnd) {
            return false;
        }

        ShowWindow(m_hWnd, nCmdShow);
        UpdateWindow(m_hWnd);

        return true;
    }

    bool Win32Native::ProcessMessages(MSG* msg) {

        if (PeekMessage(msg, nullptr, 0, 0, PM_REMOVE)) {
            TranslateMessage(msg);
            DispatchMessage(msg);
            return true;
        }

        return false;
    }


    void Win32Native::SetTitle(const char* title)
    {
        SetWindowTextA(m_hWnd, title);
    }

    void Win32Native::SetTitle(const wchar_t* title)
    {
        SetWindowTextW(m_hWnd, title);
    }

}