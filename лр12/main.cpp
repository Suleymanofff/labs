#include <windows.h>

HWND statusText;

LRESULT CALLBACK Proc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_CREATE: {
        HMENU menu = CreateMenu();
        // Идентификаторы пунктов меню совпадают с идентификаторами кнопок панели.
        AppendMenuW(menu, MF_STRING, 101, L"Создать");
        AppendMenuW(menu, MF_STRING, 102, L"Открыть");
        AppendMenuW(menu, MF_STRING, 103, L"Выход");
        SetMenu(hwnd, menu);
        CreateWindowW(L"BUTTON", L"Создать", WS_CHILD | WS_VISIBLE, 8, 8, 90, 28, hwnd, (HMENU)101, GetModuleHandleW(nullptr), nullptr);
        CreateWindowW(L"BUTTON", L"Открыть", WS_CHILD | WS_VISIBLE, 104, 8, 90, 28, hwnd, (HMENU)102, GetModuleHandleW(nullptr), nullptr);
        CreateWindowW(L"BUTTON", L"Выход", WS_CHILD | WS_VISIBLE, 200, 8, 90, 28, hwnd, (HMENU)103, GetModuleHandleW(nullptr), nullptr);
        statusText = CreateWindowW(L"STATIC", L"Перемещайтесь по меню или нажимайте кнопки панели инструментов", WS_CHILD | WS_VISIBLE | SS_SUNKEN, 0, 0, 500, 24, hwnd, nullptr, GetModuleHandleW(nullptr), nullptr);
        return 0;
    }
    case WM_SIZE:
        MoveWindow(statusText, 0, HIWORD(lParam) - 24, LOWORD(lParam), 24, TRUE); return 0;
    case WM_MENUSELECT:
        // При перемещении по строкам меню выводим подсказку в нижней строке состояния.
        if (LOWORD(wParam) == 101) SetWindowTextW(statusText, L"Строка меню: Создать");
        if (LOWORD(wParam) == 102) SetWindowTextW(statusText, L"Строка меню: Открыть");
        if (LOWORD(wParam) == 103) SetWindowTextW(statusText, L"Строка меню: Выход");
        return 0;
    case WM_COMMAND:
        // Одна обработка команд обслуживает и меню, и кнопки панели инструментов.
        if (LOWORD(wParam) == 103) DestroyWindow(hwnd);
        else SetWindowTextW(statusText, LOWORD(wParam) == 101 ? L"Команда панели/меню: Создать" : L"Команда панели/меню: Открыть");
        return 0;
    case WM_DESTROY: PostQuitMessage(0); return 0;
    }
    return DefWindowProcW(hwnd, msg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE h, HINSTANCE, LPSTR, int n) {
    WNDCLASSW wc{}; wc.lpfnWndProc = Proc; wc.hInstance = h; wc.lpszClassName = L"Lab12"; wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    RegisterClassW(&wc);
    HWND hwnd = CreateWindowW(L"Lab12", L"ЛР 12. Панель инструментов", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 580, 320, nullptr, nullptr, h, nullptr);
    ShowWindow(hwnd, n); UpdateWindow(hwnd);
    MSG msg; while (GetMessageW(&msg, nullptr, 0, 0)) { TranslateMessage(&msg); DispatchMessageW(&msg); }
    return 0;
}
