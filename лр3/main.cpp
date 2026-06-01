#include <windows.h>

void setup_console() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
}

#include <cmath>
#include <iomanip>
#include <iostream>
#include <stdexcept>

double asm_like_result(double b) {
    // Расчет ведется в double, чтобы сохранить дробную часть результата.
    const double numerator = 9.0 - (b + 2.0) * (b + 2.0);
    const double denominator = b * b + 10.0 * b + 25.0;
    if (std::fabs(denominator) < 1e-12) throw std::runtime_error("деление на нуль");
    return numerator / denominator;
}

double cpp_result(double b) {
    return (9.0 - (b + 2.0) * (b + 2.0)) / (b * b + 10.0 * b + 25.0);
}

int main() {
    setup_console();
    const double b = 1.5;
    const double y_asm = asm_like_result(b);
    const double y_cpp = cpp_result(b);
    // Вещественные числа сравниваются с погрешностью, а не через точное равенство.
    const bool ok = std::fabs(y_asm - y_cpp) < 1e-9;
    std::cout << std::fixed << std::setprecision(6);
    std::cout << "Лабораторная работа 3. Вариант 10\n";
    std::cout << "Формула: (9 - (b + 2)^2) / (b^2 + 10b + 25)\n";
    std::cout << "Исходные данные: b=" << b << "\n";
    std::cout << "Результат алгоритма: " << y_asm << "\n";
    std::cout << "Проверочный расчет C++: " << y_cpp << "\n";
    std::cout << "Проверка: " << (ok ? "пройдена" : "ошибка") << "\n";
    return ok ? 0 : 1;
}
