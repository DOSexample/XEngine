#include "XWin32.h"

static WNDPROC s_WndProc = nullptr;

LRESULT CALLBACK XWin32Proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}

XWin32::XWin32()
    : m_hInstance(nullptr), m_width(800), m_height(600)
{
    ZeroMemory(&m_wCls, sizeof(m_wCls));
}

XWin32::~XWin32() {
    if (m_hWnd) DestroyWindow(m_hWnd);

    if (m_wCls.cbSize) UnregisterClass(m_wCls.lpszClassName, m_hInstance);
}

bool XWin32::Initialize(HINSTANCE hInstance, LPCSTR windowTitle, int width, int height, WNDPROC proc) {
    m_width = width;
    m_height = height;

    if (proc == nullptr)
        s_WndProc = XWin32Proc;
    else
        s_WndProc = proc;

    m_wCls = { sizeof(WNDCLASSEXA), 0, s_WndProc, 0, 0, hInstance, NULL, NULL, NULL, NULL, windowTitle, NULL };
    RegisterClassEx(&m_wCls);

    m_hWnd = CreateWindowEx(0, windowTitle, windowTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, width, height, NULL, NULL, hInstance, NULL);

    if (!m_hWnd) {
        return false;
    }

    return true;
}

bool XWin32::ProcessMessages(MSG* msg) {

    if (PeekMessage(msg, nullptr, 0, 0, PM_REMOVE)) {
        TranslateMessage(msg);
        DispatchMessage(msg);
        return true;
    }

    return false;
}

void XWin32::Show(int nCmdShow)
{
    ::ShowWindow(m_hWnd, nCmdShow);
    ::UpdateWindow(m_hWnd);
}

void XWin32::SetTitle(LPCSTR title)
{
    SetWindowText(m_hWnd, title);
}

int XWin32::MessageBox(LPCSTR lpText, MessageBoxType uType)
{
    return ::MessageBox(m_hWnd, lpText, m_wCls.lpszClassName, uType);
}
