#include <windows.h>

#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

void setup_console() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
}

struct TransitionCheck {
    int number;
    std::string transition;
};

std::set<std::string> allowed_transitions() {
    // Базовая модель состояний процесса: допустимость проверяется по этому набору, а не задается вручную в варианте.
    return {
        "Н-Г",      // процесс принят системой и помещен в очередь готовых
        "Г-В",      // диспетчер выбрал готовый процесс для выполнения
        "В-Г",      // вытеснение процессора по таймеру
        "В-Б",      // ожидание события или ввода-вывода
        "В-З",      // завершение выполняющегося процесса
        "Г-Г/П",    // свопинг готового процесса
        "Б-Б/П",    // свопинг блокированного процесса
        "Б/П-Г/П",  // событие произошло, но процесс остается вне памяти
        "Г/П-Г",    // возврат готового приостановленного процесса в память
        "Б/П-Б"     // возврат блокированного приостановленного процесса в память
    };
}

std::map<std::string, std::string> explanations() {
    return {
        {"В-З", "Выполняющийся процесс может завершиться"},
        {"В-Н", "После выполнения процесс не возвращается в новое состояние"},
        {"Г-Б", "Готовый процесс не может заблокироваться, пока он не выполняется"},
        {"Г-Б/П", "Из готового состояния нет прямого перехода в блокированный/приостановленный"},
        {"Г-Г/П", "Готовый процесс может быть выгружен из памяти и стать готовым/приостановленным"},
        {"Г-З", "Готовый процесс не завершается напрямую, сначала он должен выполняться"},
        {"Г-Н", "Переход назад в новое состояние невозможен"},
        {"Г-В", "Диспетчер выбирает готовый процесс для выполнения"},
        {"Б-Б/П", "Блокированный процесс может быть приостановлен при свопинге"},
        {"Б-Г/П", "Прямой переход Б-Г/П не является базовым; для приостановленного процесса используется Б/П-Г/П"}
    };
}

bool is_valid_transition(const std::string& transition) {
    const std::set<std::string> allowed = allowed_transitions();
    return allowed.find(transition) != allowed.end();
}

std::string explain_transition(const std::string& transition) {
    const std::map<std::string, std::string> notes = explanations();
    const auto found = notes.find(transition);
    if (found != notes.end()) return found->second;
    return "переход не входит в базовую модель состояний процесса";
}

std::vector<int> valid_numbers(const std::vector<TransitionCheck>& variant) {
    std::vector<int> result;
    for (const auto& item : variant) {
        // Итоговый список номеров строится алгоритмически по правилам модели.
        if (is_valid_transition(item.transition)) {
            result.push_back(item.number);
        }
    }
    return result;
}

void print_numbers(const std::vector<int>& numbers) {
    for (std::size_t i = 0; i < numbers.size(); ++i) {
        if (i > 0) std::cout << ", ";
        std::cout << numbers[i];
    }
}

int main() {
    setup_console();

    // Вариант 10 содержит только проверяемые переходы; их оценка вычисляется ниже.
    const std::vector<TransitionCheck> variant10 = {
        {1, "В-З"},
        {2, "В-Н"},
        {3, "Г-Б"},
        {4, "Г-Б/П"},
        {5, "Г-Г/П"},
        {6, "Г-З"},
        {7, "Г-Н"},
        {8, "Г-В"},
        {9, "Б-Б/П"},
        {10, "Б-Г/П"}
    };

    std::cout << "Лабораторная работа 15-16. Управление процессами\n";
    std::cout << "Вариант 10\n";
    std::cout << "Задание: определить допустимость переходов между состояниями процесса.\n\n";
    std::cout << "Обозначения состояний:\n";
    std::cout << "Н - новый; В - выполняющийся; Г - готовый; Б - блокированный;\n";
    std::cout << "Б/П - блокированный/приостановленный; Г/П - готовый/приостановленный; З - завершающийся.\n";

    std::cout << "\nПереходы варианта 10\n";
    for (const auto& item : variant10) {
        const bool valid = is_valid_transition(item.transition);
        std::cout << item.number << ". " << item.transition << " - "
                  << (valid ? "допустим" : "недопустим")
                  << ". " << explain_transition(item.transition) << ".\n";
    }

    const std::vector<int> answer = valid_numbers(variant10);
    std::cout << "\nДопустимые переходы: ";
    print_numbers(answer);
    std::cout << ".\n";
    std::cout << "Ответ: допустимыми являются переходы ";
    print_numbers(answer);
    std::cout << ".\n";
    return 0;
}
