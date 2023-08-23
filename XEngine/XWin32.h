#pragma once

#undef _UNICODE
#undef UNICODE

#include <Windows.h>

#pragma comment(lib, "Winmm.lib")

enum MessageBoxType : UINT
{
    ICON_ERROR = MB_ICONERROR,
    BUTTON_OK = MB_OK,
    BUTTON_OKCANCEL = MB_OKCANCEL,

    BUTTON_OK_AND_ICON_ERROR = MB_OK | ICON_ERROR,
};

class XWin32
{
public:

    XWin32();
    ~XWin32();

    HWND GetHWND() const { return m_hWnd; }
    int GetWidth() const { return m_width; }
    int GetHeight() const { return m_height; }
    const char* GetTitle() const { return m_wCls.lpszClassName; }

    bool Initialize(HINSTANCE hInstance, LPCSTR windowTitle, int width, int height, WNDPROC proc = nullptr);
    bool ProcessMessages(MSG* msg);

    void Show(int nShowCmd);

    void SetTitle(LPCSTR title);

    int MessageBox(LPCSTR lpText, MessageBoxType uType);


    HINSTANCE m_hInstance;
    int m_width = 800;
    int m_height = 600;
    HWND m_hWnd = nullptr;
    WNDCLASSEX m_wCls;
};


