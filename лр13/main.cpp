#include <algorithm>
#include <array>
#include <string>
#include <windows.h>
#include <commdlg.h>

std::array<COLORREF, 10> ringColors = {
    RGB(220,60,60), RGB(60,160,70), RGB(70,120,220), RGB(230,180,40), RGB(150,80,190),
    RGB(30,180,180), RGB(240,120,50), RGB(120,120,120), RGB(40,200,120), RGB(250,250,250)
};
HWND targetWindow;

void draw_target(HWND hwnd, HDC hdc) {
    RECT rc; GetClientRect(hwnd, &rc);
    // Мишень вписывается во временное окно и масштабируется по его меньшей стороне.
    const int size = std::max<LONG>(40, std::min(rc.right, rc.bottom) * 9 / 10);
    const int cx = rc.right / 2, cy = rc.bottom / 2;
    SetBkMode(hdc, TRANSPARENT);
    for (int i = 0; i < 10; ++i) {
        const int r = size / 2 - i * size / 20;
        HBRUSH br = CreateSolidBrush(ringColors[i]); HGDIOBJ old = SelectObject(hdc, br);
        Ellipse(hdc, cx - r, cy - r, cx + r, cy + r);
        SelectObject(hdc, old); DeleteObject(br);
    }
    for (int i = 0; i < 10; ++i) {
        const int r = size / 2 - i * size / 20 - size / 40;
        const std::wstring s = std::to_wstring(i + 1);
        TextOutW(hdc, cx - 6, cy - r, s.c_str(), (int)s.size());
    }
}

LRESULT CALLBACK TargetProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (msg == WM_PAINT) {
        PAINTSTRUCT ps; HDC hdc = BeginPaint(hwnd, &ps);
        draw_target(hwnd, hdc);
        EndPaint(hwnd, &ps); return 0;
    }
    return DefWindowProcW(hwnd, msg, wParam, lParam);
}

void choose_colors(HWND hwnd) {
    static COLORREF custom[16]{};
    for (int i = 0; i < 10; ++i) {
        // Для каждого поля мишени последовательно вызывается стандартный диалог выбора цвета.
        CHOOSECOLORW cc{};
        cc.lStructSize = sizeof(CHOOSECOLORW);
        cc.hwndOwner = hwnd;
        cc.lpCustColors = custom;
        cc.rgbResult = ringColors[i];
        cc.Flags = CC_FULLOPEN | CC_RGBINIT;
        if (!ChooseColorW(&cc)) break;
        ringColors[i] = cc.rgbResult;
    }
    // После выбора цветов временное окно перерисовывается с новой палитрой.
    InvalidateRect(targetWindow, nullptr, TRUE);
}

LRESULT CALLBACK Proc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_CREATE:
        // Временное окно с мишенью размещается на основной диалоговой панели.
        targetWindow = CreateWindowExW(WS_EX_CLIENTEDGE, L"Lab13Target", L"", WS_CHILD | WS_VISIBLE, 20, 20, 360, 360, hwnd, nullptr, GetModuleHandleW(nullptr), nullptr);
        CreateWindowW(L"BUTTON", L"Выбрать цвета", WS_CHILD | WS_VISIBLE, 400, 40, 140, 32, hwnd, (HMENU)101, GetModuleHandleW(nullptr), nullptr);
        return 0;
    case WM_COMMAND:
        if (LOWORD(wParam) == 101) choose_colors(hwnd);
        return 0;
    case WM_DESTROY: PostQuitMessage(0); return 0;
    }
    return DefWindowProcW(hwnd, msg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE h, HINSTANCE, LPSTR, int n) {
    WNDCLASSW wc{}; wc.lpfnWndProc = Proc; wc.hInstance = h; wc.lpszClassName = L"Lab13"; wc.hbrBackground = (HBRUSH)(COLOR_3DFACE + 1);
    RegisterClassW(&wc);
    WNDCLASSW tc{}; tc.lpfnWndProc = TargetProc; tc.hInstance = h; tc.lpszClassName = L"Lab13Target"; tc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    RegisterClassW(&tc);
    HWND hwnd = CreateWindowW(L"Lab13", L"ЛР 13. Диалоговая панель", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 580, 450, nullptr, nullptr, h, nullptr);
    ShowWindow(hwnd, n); UpdateWindow(hwnd);
    MSG msg; while (GetMessageW(&msg, nullptr, 0, 0)) { TranslateMessage(&msg); DispatchMessageW(&msg); }
    return 0;
}
