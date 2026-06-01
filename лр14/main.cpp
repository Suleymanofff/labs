#include <windows.h>

#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

void setup_console() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
}

struct Result {
    std::string name;
    int total_time;
    int cpu_busy;
    int switches;
    // Две строки циклограммы показывают занятость процессора и устройства ввода-вывода по квантам.
    std::vector<std::string> cpu_line;
    std::vector<std::string> io_line;
};

double cpu_load(const Result& result) {
    return 100.0 * result.cpu_busy / result.total_time;
}

void print_timeline(const Result& result) {
    std::cout << "\n" << result.name << "\n";
    std::cout << "Время:       ";
    for (int i = 1; i <= result.total_time; ++i) {
        std::cout << std::setw(8) << i;
    }
    std::cout << "\nПроцессор:   ";
    for (const auto& cell : result.cpu_line) {
        std::cout << std::setw(8) << cell;
    }
    std::cout << "\nВвод-вывод:  ";
    for (const auto& cell : result.io_line) {
        std::cout << std::setw(8) << cell;
    }
    std::cout << "\nОбщее время выполнения: " << result.total_time << " квантов\n";
    std::cout << "Занятость процессора: " << result.cpu_busy << "/" << result.total_time
              << " = " << std::fixed << std::setprecision(1) << cpu_load(result) << "%\n";
    std::cout << "Количество переключений: " << result.switches << "\n";
}

Result single_task_mode() {
    Result result{"Однозадачный режим", 24, 12, 2, {}, {}};
    // В однозадачном режиме каждый процесс полностью проходит CPU и ввод-вывод до запуска следующего.
    for (int p = 1; p <= 3; ++p) {
        for (int i = 0; i < 4; ++i) result.cpu_line.push_back("Пр" + std::to_string(p));
        for (int i = 0; i < 4; ++i) result.io_line.push_back("-");
        for (int i = 0; i < 4; ++i) result.cpu_line.push_back("-");
        for (int i = 0; i < 4; ++i) result.io_line.push_back("Вв" + std::to_string(p));
    }
    return result;
}

Result non_preemptive_mode() {
    Result result{"Невытесняющая многозадачность", 16, 12, 2, {}, {}};
    // Процесс сам уступает процессор после своих CPU-этапов, пока его ввод-вывод выполняется отдельно.
    result.cpu_line = {
        "Пр1", "Пр1", "Пр1", "Пр1",
        "Пр2", "Пр2", "Пр2", "Пр2",
        "Пр3", "Пр3", "Пр3", "Пр3",
        "-", "-", "-", "-"
    };
    result.io_line = {
        "-", "-", "-", "-",
        "Вв1", "Вв1", "Вв1", "Вв1",
        "Вв2", "Вв2", "Вв2", "Вв2",
        "Вв3", "Вв3", "Вв3", "Вв3"
    };
    return result;
}

Result preemptive_mode() {
    Result result{"Вытесняющая многозадачность", 22, 12, 11, {}, {}};
    // Круговое планирование выдает каждому процессу по одному процессорному кванту за раз.
    result.cpu_line = {
        "Пр1", "Пр2", "Пр3", "Пр1", "Пр2", "Пр3",
        "Пр1", "Пр2", "Пр3", "Пр1", "Пр2", "Пр3",
        "-", "-", "-", "-", "-", "-", "-", "-", "-", "-"
    };
    result.io_line = {
        "-", "-", "-", "-", "-", "-", "-", "-", "-", "-",
        "Вв1", "Вв1", "Вв1", "Вв1",
        "Вв2", "Вв2", "Вв2", "Вв2",
        "Вв3", "Вв3", "Вв3", "Вв3"
    };
    return result;
}

int main() {
    setup_console();
    std::cout << "Лабораторная работа 14. Управление заданиями\n";
    std::cout << "Вариант 10\n";
    std::cout << "N=3, E=4, T=2\n";
    std::cout << "Процессорные этапы: P1={1,2}, P2={1,2}, P3={1,2}\n";
    std::cout << "Этапы ввода-вывода для всех процессов: 3,4\n";

    print_timeline(single_task_mode());
    print_timeline(non_preemptive_mode());
    print_timeline(preemptive_mode());

    std::cout << "\nВывод: минимальное общее время для варианта 10 дает невытесняющая "
                 "многозадачность, так как работа процессора перекрывается с вводом-выводом.\n";
    return 0;
}
