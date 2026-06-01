#include <windows.h>

HMENU popupMenu;

LRESULT CALLBACK Proc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_CREATE: {
        HMENU mainMenu = CreateMenu();
        const wchar_t* names[] = {L"Файл", L"Правка", L"Вид", L"Фигуры", L"Цвета", L"Сервис", L"Справка", L"Окно", L"Параметры"};
        // Большое количество пунктов заставляет главное меню переноситься при сужении окна.
        for (int i = 0; i < 9; ++i) AppendMenuW(mainMenu, MF_STRING, 100 + i, names[i]);
        SetMenu(hwnd, mainMenu);
        popupMenu = CreatePopupMenu();
        AppendMenuW(popupMenu, MF_STRING, 201, L"Первый");
        // MF_MENUBARBREAK размещает следующие пункты плавающего меню в одной горизонтальной линии.
        AppendMenuW(popupMenu, MF_MENUBARBREAK | MF_STRING, 202, L"Второй");
        AppendMenuW(popupMenu, MF_MENUBARBREAK | MF_STRING, 203, L"Третий");
        AppendMenuW(popupMenu, MF_MENUBARBREAK | MF_STRING, 204, L"Четвертый");
        return 0;
    }
    case WM_RBUTTONUP: {
        // Плавающее меню открывается в точке щелчка правой кнопкой мыши.
        POINT pt{LOWORD(lParam), HIWORD(lParam)}; ClientToScreen(hwnd, &pt);
        TrackPopupMenu(popupMenu, TPM_LEFTALIGN | TPM_TOPALIGN, pt.x, pt.y, 0, hwnd, nullptr); return 0;
    }
    case WM_COMMAND: MessageBoxW(hwnd, L"Выбрана команда меню", L"ЛР 11", MB_OK); return 0;
    case WM_PAINT: {
        PAINTSTRUCT ps; HDC hdc = BeginPaint(hwnd, &ps);
        RECT rc; GetClientRect(hwnd, &rc);
        FillRect(hdc, &rc, (HBRUSH)(COLOR_WINDOW + 1));
        SetBkMode(hdc, TRANSPARENT);
        rc.top = rc.bottom - 42;
        DrawTextW(hdc, L"Правая кнопка мыши открывает плавающее меню в одну линию.", -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
        EndPaint(hwnd, &ps); return 0;
    }
    case WM_DESTROY: PostQuitMessage(0); return 0;
    }
    return DefWindowProcW(hwnd, msg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE h, HINSTANCE, LPSTR, int n) {
    WNDCLASSW wc{}; wc.lpfnWndProc = Proc; wc.hInstance = h; wc.lpszClassName = L"Lab11"; wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    RegisterClassW(&wc);
    HWND hwnd = CreateWindowW(L"Lab11", L"ЛР 11. Меню", WS_OVERLAPPEDWINDOW, 100, 100, 420, 300, nullptr, nullptr, h, nullptr);
    ShowWindow(hwnd, n); UpdateWindow(hwnd);
    MSG msg; while (GetMessageW(&msg, nullptr, 0, 0)) { TranslateMessage(&msg); DispatchMessageW(&msg); }
    return 0;
}
