#include <windows.h>

void setup_console() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
}

#include <iostream>

// Последовательность действий повторяет вычисление через рабочий регистр:
// берем c, затем последовательно применяем вычитание и сложение.
int asm_like_result(int a, int b, int c, int d) {
    int y = c;
    y -= a;
    y += b;
    y += d;
    return y;
}

int cpp_result(int a, int b, int c, int d) {
    return c - a + b + d;
}

int main() {
    setup_console();
    const int a = 7, b = 4, c = 20, d = 3;
    // Сравниваем алгоритмический расчет с прямой формулой C++.
    const int y_asm = asm_like_result(a, b, c, d);
    const int y_cpp = cpp_result(a, b, c, d);
    std::cout << "Лабораторная работа 1. Вариант 10\n";
    std::cout << "Формула: Y = c - a + b + d\n";
    std::cout << "Исходные данные: a=" << a << ", b=" << b << ", c=" << c << ", d=" << d << "\n";
    std::cout << "Результат алгоритма: " << y_asm << "\n";
    std::cout << "Проверочный расчет C++: " << y_cpp << "\n";
    std::cout << "Проверка: " << (y_asm == y_cpp ? "пройдена" : "ошибка") << "\n";
    return y_asm == y_cpp ? 0 : 1;
}
