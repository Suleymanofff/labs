#include <algorithm>
#include <array>
#include <random>
#include <string>
#include <windows.h>

std::array<COLORREF, 10> colors;

void init_colors() {
    // Палитра содержит разные цвета; порядок перемешивается при запуске программы.
    std::array<COLORREF, 10> palette = {
        RGB(220,60,60), RGB(60,160,70), RGB(70,120,220), RGB(230,180,40), RGB(150,80,190),
        RGB(30,180,180), RGB(240,120,50), RGB(120,120,120), RGB(40,200,120), RGB(250,250,250)
    };
    std::mt19937 gen(GetTickCount());
    std::shuffle(palette.begin(), palette.end(), gen);
    colors = palette;
}

void draw_target(HWND hwnd, HDC hdc) {
    RECT rc; GetClientRect(hwnd, &rc);
    // Размер мишени зависит от меньшей стороны окна, поэтому она всегда полностью видима.
    const int size = std::max<LONG>(40, std::min(rc.right - rc.left, rc.bottom - rc.top) * 9 / 10);
    const int cx = rc.right / 2, cy = rc.bottom / 2;
    SetBkMode(hdc, TRANSPARENT);
    for (int i = 0; i < 10; ++i) {
        // Круги рисуются от внешнего поля к центру.
        const int r = size / 2 - i * size / 20;
        HBRUSH br = CreateSolidBrush(colors[i]); HGDIOBJ old = SelectObject(hdc, br);
        Ellipse(hdc, cx - r, cy - r, cx + r, cy + r);
        SelectObject(hdc, old); DeleteObject(br);
    }
    for (int i = 0; i < 10; ++i) {
        const int r = size / 2 - i * size / 20 - size / 40;
        const std::wstring text = std::to_wstring(i + 1);
        TextOutW(hdc, cx - 6, cy - r, text.c_str(), (int)text.size());
    }
}

LRESULT CALLBACK Proc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_CREATE: init_colors(); return 0;
    case WM_SIZE: InvalidateRect(hwnd, nullptr, TRUE); return 0;
    case WM_PAINT: {
        PAINTSTRUCT ps; HDC hdc = BeginPaint(hwnd, &ps);
        draw_target(hwnd, hdc);
        EndPaint(hwnd, &ps); return 0;
    }
    case WM_DESTROY: PostQuitMessage(0); return 0;
    }
    return DefWindowProcW(hwnd, msg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE h, HINSTANCE, LPSTR, int n) {
    WNDCLASSW wc{}; wc.lpfnWndProc = Proc; wc.hInstance = h; wc.lpszClassName = L"Lab10"; wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    RegisterClassW(&wc);
    HWND hwnd = CreateWindowW(L"Lab10", L"ЛР 10. Мишень", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 720, 560, nullptr, nullptr, h, nullptr);
    ShowWindow(hwnd, n); UpdateWindow(hwnd);
    MSG msg; while (GetMessageW(&msg, nullptr, 0, 0)) { TranslateMessage(&msg); DispatchMessageW(&msg); }
    return 0;
}
