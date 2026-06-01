#include <windows.h>

void setup_console() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
}

#include <cctype>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

int read_operand(const std::string& token, int a) {
    if (token == "A") return a;
    if (token.size() == 1 && std::isdigit(static_cast<unsigned char>(token[0]))) return token[0] - '0';
    throw std::runtime_error("недопустимый операнд: " + token);
}

struct Command { std::string mnemonic; std::string left; std::string right; };

std::vector<std::string> read_program_lines() {
    std::vector<std::string> lines;
    // Сначала ищем program.zpp в текущей папке запуска.
    std::ifstream file("program.zpp");
    if (file) {
        std::string line;
        while (std::getline(file, line)) lines.push_back(line);
        return lines;
    }

    // Если VS Code запустил программу из другой папки, ищем файл рядом с main.exe.
    wchar_t exe_path[MAX_PATH]{};
    if (GetModuleFileNameW(nullptr, exe_path, MAX_PATH) == 0) return lines;
    wchar_t* slash = wcsrchr(exe_path, L'\\');
    if (!slash) return lines;
    *slash = L'\0';
    if (SetCurrentDirectoryW(exe_path)) {
        std::ifstream file_near_exe("program.zpp");
        std::string line;
        while (std::getline(file_near_exe, line)) lines.push_back(line);
    }
    return lines;
}

Command parse_line(const std::string& line) {
    std::istringstream input(line);
    std::string mnemonic, operands;
    // Строка должна содержать только мнемонику ADD/MUL и пару операндов через запятую.
    if (!(input >> mnemonic >> operands) || (mnemonic != "ADD" && mnemonic != "MUL")) {
        throw std::runtime_error("недопустимая команда: " + line);
    }
    if (input >> std::ws && !input.eof()) {
        throw std::runtime_error("лишний текст в строке: " + line);
    }
    const std::size_t comma = operands.find(',');
    if (comma == std::string::npos || operands.find(',', comma + 1) != std::string::npos) {
        throw std::runtime_error("ошибка разделителя операндов: " + line);
    }
    return {mnemonic, operands.substr(0, comma), operands.substr(comma + 1)};
}

int main() {
    setup_console();
    const std::vector<std::string> program_lines = read_program_lines();
    if (program_lines.empty()) {
        std::cerr << "Не удалось открыть файл program.zpp. Он должен лежать рядом с main.exe или в текущей папке запуска.\n";
        return 1;
    }
    int a = 0;
    std::string line;
    int line_no = 0;
    std::cout << "Лабораторная работа 7. Вариант 10\n";
    try {
        for (const std::string& line : program_lines) {
            if (line.find_first_not_of(" \t\r\n") == std::string::npos) continue;
            ++line_no;
            const Command command = parse_line(line);
            const int x = read_operand(command.left, a);
            const int y = read_operand(command.right, a);
            // Результат каждой команды языка Z++ всегда записывается обратно в регистр A.
            a = command.mnemonic == "ADD" ? x + y : x * y;
            std::cout << "Строка " << line_no << ": " << line << " -> A=" << a << "\n";
        }
    } catch (const std::exception& error) {
        std::cerr << "Ошибка синтаксиса в строке " << line_no << ": " << error.what() << "\n";
        return 2;
    }
    std::cout << "Итоговое значение A: " << a << "\n";
    return a == 57 ? 0 : 1;
}
