#include "TestRunner.h"
#include "OptimizedDataStructure.h" // Потрібен для worker_function

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <random>
#include <chrono>       // Для вимірювання часу
#include <thread>       // Для std::thread
#include <functional>   // Для std::ref, std::cref
#include <iomanip>      // Для std::setw, std::setprecision, std::fixed
#include <stdexcept>    // Для std::runtime_error

/**
 * @brief Генерує файл з послідовністю операцій.
 */
void generate_operations_file(const std::string& filename, long num_operations, const std::vector<double>& weights) {
    std::ofstream file(filename);
    if (!file) {
        std::cerr << "Could not open file for writing: " << filename << std::endl;
        return;
    }

    std::mt19937 gen(std::random_device{}());
    std::discrete_distribution<> dist(weights.begin(), weights.end());

    const std::vector<std::string> ops = {
            "read 0", "write 0 1", "read 1", "write 1 1", "read 2", "write 2 1", "string"
    };

    for (long i = 0; i < num_operations; ++i) {
        file << ops[dist(gen)] << "\n";
    }
}

/**
 * @brief Зчитує всі команди з файлу у вектор.
 */
std::vector<std::string> read_commands(const std::string& filename) {
    std::vector<std::string> commands;
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Could not open file for reading: " << filename << std::endl;
        return commands;
    }
    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty()) {
            commands.push_back(line);
        }
    }
    return commands;
}

/**
 * @brief Функція, яку виконує кожен потік. (ОНОВЛЕНА ВЕРСІЯ)
 */
void worker_function(OptimizedDataStructure& data, const std::vector<std::string>& commands) {
    // 'dummy_read' залишаємо volatile, це безпечно
    volatile int dummy_read = 0;

    for (const auto& cmd_line : commands) {
        std::stringstream ss(cmd_line);
        std::string op;
        int field_index;
        int value;

        ss >> op;
        if (op == "write") {
            ss >> field_index >> value;
            data.set(field_index, value);
        } else if (op == "read") {
            ss >> field_index;
            dummy_read = data.get(field_index);
        } else if (op == "string") {
            // --- ОНОВЛЕНИЙ БЛОК ---
            // 1. Створюємо звичайний, не-volatile рядок
            std::string dummy_string = (std::string)data;
            // 2. "Використовуємо" рядок
            dummy_read += (int)dummy_string.length();
            // --- КІНЕЦЬ ОНОВЛЕНОГО БЛОКУ ---
        }
    }

    // Уникаємо попередження "unused variable"
    (void)dummy_read;
} // <--- Це та сама дужка, якої не вистачало

/**
 * @brief Запускає тест для набору файлів (кожен у своєму потоці).
 */
double run_test(const std::vector<std::string>& filenames) {
    OptimizedDataStructure data(M_FIELDS);

    std::vector<std::vector<std::string>> all_commands;
    for (const auto& f : filenames) {
        all_commands.push_back(read_commands(f));
    }

    std::vector<std::thread> threads;
    auto start_time = std::chrono::high_resolution_clock::now();

    for (size_t i = 0; i < filenames.size(); ++i) {
        threads.emplace_back(worker_function, std::ref(data), std::cref(all_commands[i]));
    }

    for (auto& t : threads) {
        t.join();
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration_ms = end_time - start_time;

    return duration_ms.count();
}

/**
 * @brief Готує тестові файли для 3-х сценаріїв.
 */
void setup_test_files() {
    std::cout << "Generating test files (" << NUM_OPERATIONS << " ops each)..." << std::endl;

    // Сценарій A: Варіант 10 (1, 40, 1, 5, 1, 5, 47)
    std::vector<double> weights_a = {1.0, 40.0, 1.0, 5.0, 1.0, 5.0, 47.0};

    // Сценарій B: Рівномірний (всі по 1)
    std::vector<double> weights_b = {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0};

    // Сценарій C: Невідповідний (поганий для нашої оптимізації)
    std::vector<double> weights_c = {1.0, 1.0, 40.0, 5.0, 40.0, 5.0, 8.0};

    for (int i = 1; i <= 3; ++i) {
        generate_operations_file("a_" + std::to_string(i) + ".txt", NUM_OPERATIONS, weights_a);
        generate_operations_file("b_" + std::to_string(i) + ".txt", NUM_OPERATIONS, weights_b);
        generate_operations_file("c_" + std::to_string(i) + ".txt", NUM_OPERATIONS, weights_c);
    }
    std::cout << "Generation complete." << std::endl << std::endl;
}