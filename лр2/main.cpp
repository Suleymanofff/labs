#include <windows.h>

void setup_console() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
}

#include <iostream>
#include <stdexcept>

int asm_like_result(int b) {
    // Числитель и знаменатель считаются отдельно, как в пошаговом арифметическом алгоритме.
    const int numerator = 9 - (b + 2) * (b + 2);
    const int denominator = b * b + 10 * b + 25;
    if (denominator == 0) throw std::runtime_error("деление на нуль");
    return numerator / denominator;
}

int cpp_result(int b) {
    return (9 - (b + 2) * (b + 2)) / (b * b + 10 * b + 25);
}

int main() {
    setup_console();
    const int b = -4;
    // Контрольная формула нужна, чтобы сразу увидеть ошибку в основной реализации.
    const int y_asm = asm_like_result(b);
    const int y_cpp = cpp_result(b);
    std::cout << "Лабораторная работа 2. Вариант 10\n";
    std::cout << "Формула: (9 - (b + 2)^2) / (b^2 + 10b + 25)\n";
    std::cout << "Исходные данные: b=" << b << "\n";
    std::cout << "Результат алгоритма: " << y_asm << "\n";
    std::cout << "Проверочный расчет C++: " << y_cpp << "\n";
    std::cout << "Проверка: " << (y_asm == y_cpp ? "пройдена" : "ошибка") << "\n";
    return y_asm == y_cpp ? 0 : 1;
}
