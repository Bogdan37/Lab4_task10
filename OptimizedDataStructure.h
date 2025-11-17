#pragma once

#include <vector>
#include <string>
#include <mutex>

// Кількість полів (m=3 для варіанту 10)
constexpr int M_FIELDS = 3;

class OptimizedDataStructure {
private:
    std::vector<int> fields;


    mutable std::mutex mutex_f0;
    mutable std::mutex mutex_f1_f2;

public:

    OptimizedDataStructure(int m = M_FIELDS);


    void set(int fieldIndex, int value);


    int get(int fieldIndex) const;


    operator std::string() const;
};
