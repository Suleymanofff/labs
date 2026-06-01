#include <algorithm>
#include <cstdlib>
#include <windows.h>

LRESULT CALLBACK Proc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    static bool sizing = false;
    switch (msg) {
    case WM_LBUTTONDOWN: {
        const int x = LOWORD(lParam), y = HIWORD(lParam);
        // Секретная точка находится около координат 40x40 рабочей области окна.
        if (std::abs(x - 40) < 18 && std::abs(y - 40) < 18) {
            sizing = true;
            SetCapture(hwnd);
        }
        return 0;
    }
    case WM_LBUTTONUP:
        sizing = false; ReleaseCapture(); return 0;
    case WM_MOUSEMOVE:
        if (sizing) {
            // Меняем только размеры окна: левая и верхняя границы остаются на месте.
            POINT pt{LOWORD(lParam), HIWORD(lParam)}; ClientToScreen(hwnd, &pt);
            RECT wr; GetWindowRect(hwnd, &wr);
            SetWindowPos(hwnd, nullptr, 0, 0, std::max(180L, pt.x - wr.left), std::max(130L, pt.y - wr.top), SWP_NOMOVE | SWP_NOZORDER);
        }
        return 0;
    case WM_PAINT: {
        PAINTSTRUCT ps; HDC hdc = BeginPaint(hwnd, &ps);
        RECT rc; GetClientRect(hwnd, &rc);
        DrawTextW(hdc, L"Окно без рамок с фоном трехмерных элементов\nРазмер меняется перетаскиванием из секретной точки", -1, &rc, DT_CENTER | DT_VCENTER | DT_WORDBREAK);
        EndPaint(hwnd, &ps); return 0;
    }
    case WM_RBUTTONDOWN: DestroyWindow(hwnd); return 0;
    case WM_DESTROY: PostQuitMessage(0); return 0;
    }
    return DefWindowProcW(hwnd, msg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE h, HINSTANCE, LPSTR, int n) {
    WNDCLASSW wc{}; wc.lpfnWndProc = Proc; wc.hInstance = h; wc.lpszClassName = L"Lab9"; wc.hbrBackground = (HBRUSH)(COLOR_3DFACE + 1); wc.hCursor = LoadCursor(nullptr, IDC_SIZEALL);
    RegisterClassW(&wc);
    HWND hwnd = CreateWindowExW(WS_EX_APPWINDOW, L"Lab9", L"", WS_POPUP | WS_VISIBLE, 300, 180, 480, 320, nullptr, nullptr, h, nullptr);
    ShowWindow(hwnd, n);
    MSG msg; while (GetMessageW(&msg, nullptr, 0, 0)) { TranslateMessage(&msg); DispatchMessageW(&msg); }
    return 0;
}
