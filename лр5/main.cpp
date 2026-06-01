#include <windows.h>

void setup_console() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
}

#include <iostream>

int asm_like_result(int x) {
    // Вариант 10 задает кусочную функцию: формула применяется только при x >= 8.
    if (x >= 8) return -x * x + x - 9;
    return 0;
}

int cpp_result(int x) { return x >= 8 ? -x * x + x - 9 : 0; }

int main() {
    setup_console();
    // Два значения проверяют обе ветви условия: основную формулу и нулевой результат.
    const int x1 = 8, x2 = 5;
    const int y1 = asm_like_result(x1), y2 = asm_like_result(x2);
    const bool ok = y1 == cpp_result(x1) && y2 == cpp_result(x2);
    std::cout << "Лабораторная работа 5. Вариант 10\n";
    std::cout << "Функция: f(x) = -x^2 + x - 9 при x >= 8, иначе 0\n";
    std::cout << "Результат для x=8: " << y1 << "\n";
    std::cout << "Результат для x=5: " << y2 << "\n";
    std::cout << "Проверка: " << (ok ? "пройдена" : "ошибка") << "\n";
    return ok ? 0 : 1;
}
