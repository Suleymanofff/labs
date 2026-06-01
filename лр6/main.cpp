#include <windows.h>

void setup_console() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
}

#include <iostream>
#include <vector>

// Каждая функция реализует одно из пяти заданий по циклам и массивам.
int sum_range(int n, int k) { int s = 0; for (int i = n; i <= k; ++i) s += i; return s; }
int product_range(int n, int k) { int p = 1; for (int i = n; i <= k; ++i) p *= i; return p; }
int sum_squares(const std::vector<int>& values) { int s = 0; for (int v : values) s += v * v; return s; }
int count_negative(const std::vector<int>& values) { int c = 0; for (int v : values) if (v < 0) ++c; return c; }
int max_value(const std::vector<int>& values) { int m = values.front(); for (int v : values) if (v > m) m = v; return m; }

int main() {
    setup_console();
    bool ok = true;
    // Контрольные значения фиксируют ожидаемые ответы для всех пяти задач.
    ok = ok && sum_range(3, 7) == 25;
    ok = ok && product_range(2, 5) == 120;
    ok = ok && sum_squares({2, -3, 4}) == 29;
    ok = ok && count_negative({4, -1, 0, -7, 3}) == 2;
    ok = ok && max_value({4, 9, 1, 6}) == 9;
    std::cout << "Лабораторная работа 6. Массивы и циклы\n";
    std::cout << "Задание 1, сумма от 3 до 7: " << sum_range(3, 7) << "\n";
    std::cout << "Задание 2, произведение от 2 до 5: " << product_range(2, 5) << "\n";
    std::cout << "Задание 3, сумма квадратов: " << sum_squares({2, -3, 4}) << "\n";
    std::cout << "Задание 4, количество отрицательных: " << count_negative({4, -1, 0, -7, 3}) << "\n";
    std::cout << "Задание 5, максимальный элемент: " << max_value({4, 9, 1, 6}) << "\n";
    std::cout << "Проверка: " << (ok ? "пройдена" : "ошибка") << "\n";
    return ok ? 0 : 1;
}
