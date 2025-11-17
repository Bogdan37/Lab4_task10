#pragma once

#include <vector>
#include <string>

// Кількість операцій для генерації у кожному файлі
constexpr long NUM_OPERATIONS = 1000000;

/**
 * @brief Готує тестові файли для 3-х сценаріїв.
 */
void setup_test_files();

/**
 * @brief Запускає тест для набору файлів (кожен у своєму потоці).
 * @param filenames Список файлів, кількість файлів = кількість потоків.
 * @return Час виконання у мілісекундах.
 */
double run_test(const std::vector<std::string>& filenames);
