#pragma once
#include <Windows.h>

class MirrorWindow
{
public:
    MirrorWindow();
    ~MirrorWindow();

public:
    bool Init(int x, int y, int w, int h);

    // show
    void SetShow(bool show);
    bool GetShow() const;

    // handler message
    bool HandlerMessage();

    int GetWidth() const;
    int GetHeight() const;

    HWND GetHwnd() const;

protected:
    bool CreateWindows(int x, int y, int w, int h);

protected:
    HWND      m_Hwnd{nullptr};
    HINSTANCE m_Hinstance{nullptr};
    bool      m_Show{false};
    int       m_Width{0};
    int       m_Height{0};
    int       m_Xpos{0};
    int       m_Ypos{0};
};