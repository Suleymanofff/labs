#include <algorithm>
#include <windows.h>

LRESULT CALLBACK ChildProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    // Разные кнопки мыши выдают разные системные звуковые сигналы.
    case WM_LBUTTONDOWN: MessageBeep(MB_ICONASTERISK); return 0;
    case WM_RBUTTONDOWN: MessageBeep(MB_ICONEXCLAMATION); return 0;
    case WM_MBUTTONDOWN: MessageBeep(MB_ICONHAND); return 0;
    case WM_PAINT: {
        PAINTSTRUCT ps; HDC hdc = BeginPaint(hwnd, &ps);
        RECT rc; GetClientRect(hwnd, &rc);
        DrawTextW(hdc, L"Окно без заголовка с полосами просмотра\nЛевая, правая и средняя кнопки мыши дают разные сигналы", -1, &rc, DT_CENTER | DT_VCENTER | DT_WORDBREAK);
        EndPaint(hwnd, &ps); return 0;
    }}
    return DefWindowProcW(hwnd, msg, wParam, lParam);
}

void place_child(HWND parent, HWND child) {
    RECT rc; GetClientRect(parent, &rc);
    // Половина ширины и высоты дает площадь дочернего окна в одну четверть рабочей области.
    const int w = std::max<LONG>(120, (rc.right - rc.left) / 2);
    const int h = std::max<LONG>(90, (rc.bottom - rc.top) / 2);
    SetWindowPos(child, nullptr, (rc.right - w) / 2, (rc.bottom - h) / 2, w, h, SWP_NOZORDER | SWP_SHOWWINDOW);
}

LRESULT CALLBACK MainProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    static HWND child = nullptr;
    switch (msg) {
    case WM_CREATE:
        // Временное окно без заголовка создается как дочернее окно с двумя полосами прокрутки.
        child = CreateWindowExW(WS_EX_CLIENTEDGE, L"Lab8Child", L"", WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL, 0, 0, 100, 100, hwnd, nullptr, GetModuleHandleW(nullptr), nullptr);
        place_child(hwnd, child); return 0;
    case WM_SIZE: if (child) place_child(hwnd, child); return 0;
    case WM_DESTROY: PostQuitMessage(0); return 0;
    }
    return DefWindowProcW(hwnd, msg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE h, HINSTANCE, LPSTR, int n) {
    WNDCLASSW wc{}; wc.lpfnWndProc = MainProc; wc.hInstance = h; wc.lpszClassName = L"Lab8Main"; wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    RegisterClassW(&wc);
    WNDCLASSW cc{}; cc.lpfnWndProc = ChildProc; cc.hInstance = h; cc.lpszClassName = L"Lab8Child"; cc.hbrBackground = (HBRUSH)(COLOR_3DFACE + 1);
    RegisterClassW(&cc);
    HWND hwnd = CreateWindowW(L"Lab8Main", L"ЛР 8. Вариант 10", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 720, 480, nullptr, nullptr, h, nullptr);
    ShowWindow(hwnd, n); UpdateWindow(hwnd);
    MSG msg; while (GetMessageW(&msg, nullptr, 0, 0)) { TranslateMessage(&msg); DispatchMessageW(&msg); }
    return 0;
}
