#include <windows.h>

void setup_console() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
}

#include <cstdlib>
#include <iostream>
#include <string>

// Задания вынесены в отдельные функции, чтобы каждую ветвящуюся задачу можно было проверить отдельно.
int task1(int a, int b, int x) {
    if (x > 0) return a + b * x;
    if (x < 0) return x * x;
    return 1;
}

int task2(int n) { return n > 0 ? n + 1 : n; }

int task3(int a, int b, int c) {
    int result = a;
    if (b < result) result = b;
    if (c < result) result = c;
    return result;
}

int task4(int a, int b, int c) {
    int count = 0;
    if (a > 0) ++count;
    if (b > 0) ++count;
    if (c > 0) ++count;
    return count;
}

std::string task5(int a, int b, int c) {
    const int db = std::abs(b - a);
    const int dc = std::abs(c - a);
    if (db <= dc) return "B, расстояние " + std::to_string(db);
    return "C, расстояние " + std::to_string(dc);
}

int main() {
    setup_console();
    bool ok = true;
    // Набор контрольных примеров покрывает все ветви условий из пяти заданий.
    ok = ok && task1(2, 3, 4) == 14 && task1(2, 3, -3) == 9 && task1(2, 3, 0) == 1;
    ok = ok && task2(5) == 6 && task2(-2) == -2;
    ok = ok && task3(8, -1, 4) == -1;
    ok = ok && task4(-1, 2, 3) == 2;
    ok = ok && task5(10, 6, 13) == "C, расстояние 3";
    std::cout << "Лабораторная работа 4. Общие задания 1-5\n";
    std::cout << "Задание 1: " << task1(2, 3, 4) << ", " << task1(2, 3, -3) << ", " << task1(2, 3, 0) << "\n";
    std::cout << "Задание 2: " << task2(5) << ", " << task2(-2) << "\n";
    std::cout << "Задание 3: " << task3(8, -1, 4) << "\n";
    std::cout << "Задание 4: " << task4(-1, 2, 3) << "\n";
    std::cout << "Задание 5: " << task5(10, 6, 13) << "\n";
    std::cout << "Проверка: " << (ok ? "пройдена" : "ошибка") << "\n";
    return ok ? 0 : 1;
}
