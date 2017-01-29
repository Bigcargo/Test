#include "MirrorWindow.h"
#include <iostream>
#include <vector>

// const bool GlobalRunning = false;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC         hdc;

    switch (message) {
        case WM_PAINT:
            hdc = BeginPaint(hWnd, &ps);
            EndPaint(hWnd, &ps);
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
    }
    std::cout << "windProc" << std::endl;
    return DefWindowProc(hWnd, message, wParam, lParam);
}

std::vector<MONITORINFO> GlobalMonitorInfoVec;
BOOL CALLBACK MonitorEnumProc(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData)
{
    //Save display info
    MONITORINFO monitorinfo;
    monitorinfo.cbSize = sizeof(MONITORINFO);

    //monitorinfo
    auto result = GetMonitorInfo(hMonitor, &monitorinfo);

    if (result == 1) {
        GlobalMonitorInfoVec.push_back(monitorinfo);
        return TRUE;
    }
    return FALSE;
}

void GetMirrorInfo()
{
    EnumDisplayMonitors(NULL, NULL, MonitorEnumProc, 0);

    // int dspNum = ::GetSystemMetrics(SM_CMONITORS);
    // std::cout << "sdpNUm: " << dspNum << std::endl;

    // DISPLAY_DEVICE DisplayDevice1;  
    // ZeroMemory(&DisplayDevice1, sizeof(DisplayDevice1));
    // EnumDisplayDevices(NULL, dspNum, &DisplayDevice1, 0);
    // std::cout << DisplayDevice1.cb << std::endl;
    // std::cout << DisplayDevice1.DeviceID << std::endl;
    // std::cout << DisplayDevice1.DeviceKey << std::endl;
    // std::cout << DisplayDevice1.DeviceName << std::endl;
    // std::cout << DisplayDevice1.DeviceString << std::endl;
    // std::cout << DisplayDevice1.StateFlags << std::endl;

    // DISPLAY_DEVICE DisplayDevice;  
    // DWORD          i = 0;
    // ZeroMemory(&DisplayDevice, sizeof(DisplayDevice));  
    // DisplayDevice.cb = sizeof(DisplayDevice);  

    // for (i = 0; EnumDisplayDevices(NULL, i, &DisplayDevice, 0); i++) {
    //    std::cout << DisplayDevice.cb << std::endl;
    //    std::cout << DisplayDevice.DeviceID << std::endl;
    //    std::cout << DisplayDevice.DeviceKey << std::endl;
    //    std::cout << DisplayDevice.DeviceName << std::endl;
    //    std::cout << DisplayDevice.DeviceString << std::endl;
    //    std::cout << DisplayDevice.StateFlags << std::endl;
    //}
}

MirrorWindow::MirrorWindow()
{
}

MirrorWindow::~MirrorWindow()
{
    DestroyWindow(m_Hwnd);
}

bool MirrorWindow::Init(int x, int y, int w, int h)
{
    GetMirrorInfo();
    auto nx   = x;
    auto ny   = y;
    auto nw   = w;
    auto nh   = h;
    auto size = GlobalMonitorInfoVec.size();
    if (size) {
        auto monitorInfo = GlobalMonitorInfoVec[0];
        if (size > 1) {
            monitorInfo = GlobalMonitorInfoVec[size - 1];
        }
        std::cout << "123" << std::endl;
        nx = monitorInfo.rcMonitor.left;
        ny = monitorInfo.rcMonitor.top;

        nw = monitorInfo.rcMonitor.right - nx;
        nh = monitorInfo.rcMonitor.bottom - ny;
    }
    std::cout << "size = " << size << " x = " << nx << "y = " << ny << "w = " << nw << "h = " << nh
              << std::endl;
    return CreateWindows(nx, ny, nw, nh);
}

void MirrorWindow::SetShow(bool show)
{
    if (!m_Hwnd) {
        return;
    }

    m_Show = show;
    ShowWindow(m_Hwnd, m_Show);
}

bool MirrorWindow::GetShow() const
{
    return m_Show;
}

bool MirrorWindow::HandlerMessage()
{
    MSG msg;
    while (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return true;
}

int MirrorWindow::GetWidth() const
{
    return m_Width;
}

int MirrorWindow::GetHeight() const
{
    return m_Height;
}

HWND MirrorWindow::GetHwnd() const
{
    return m_Hwnd;
}

bool MirrorWindow::CreateWindows(int x, int y, int w, int h)
{
    // Register class
    WNDCLASSEX wcex;
    wcex.cbSize        = sizeof(WNDCLASSEX);
    wcex.style         = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc   = WndProc;
    wcex.cbClsExtra    = 0;
    wcex.cbWndExtra    = 0;
    wcex.hInstance     = m_Hinstance;
    wcex.hIcon         = LoadIcon(m_Hinstance, nullptr);
    wcex.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName  = NULL;
    wcex.lpszClassName = "TutorialWindowClass";
    wcex.hIconSm       = LoadIcon(wcex.hInstance, nullptr);
    if (!RegisterClassEx(&wcex))
        return false;

    // Create window
    // RECT rc = {x, y, int(w), int(h)};
    // AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
    m_Hwnd = CreateWindow("TutorialWindowClass", "Direct3D 11 Tutorial 7", WS_OVERLAPPEDWINDOW, x,
                          y, w, h, NULL, NULL, m_Hinstance, NULL);
    if (!m_Hwnd)
        return false;

    m_Width  = w;
    m_Height = h;
    m_Xpos   = x;
    m_Ypos   = y;

    // default no display
    ShowWindow(m_Hwnd, m_Show);

    return true;
}
