#include "OptimizedDataStructure.h"
#include <sstream>
#include <stdexcept>

OptimizedDataStructure::OptimizedDataStructure(int m) : fields(m, 0) {
    if (m != M_FIELDS) {
        throw std::runtime_error("Невірна кількість полів. Очікується " + std::to_string(M_FIELDS));
    }
}


void OptimizedDataStructure::set(int fieldIndex, int value) {
    if (fieldIndex == 0) {
        std::lock_guard<std::mutex> lock(mutex_f0);
        fields[fieldIndex] = value;
    } else if (fieldIndex == 1 || fieldIndex == 2) {
        std::lock_guard<std::mutex> lock(mutex_f1_f2);
        fields[fieldIndex] = value;
    }

}


int OptimizedDataStructure::get(int fieldIndex) const {
    int value = 0;
    if (fieldIndex == 0) {
        std::lock_guard<std::mutex> lock(mutex_f0);
        value = fields[fieldIndex];
    } else if (fieldIndex == 1 || fieldIndex == 2) {
        std::lock_guard<std::mutex> lock(mutex_f1_f2);
        value = fields[fieldIndex];
    }
    return value;
}


OptimizedDataStructure::operator std::string() const {
    std::scoped_lock lock(mutex_f0, mutex_f1_f2);
    std::stringstream ss;
    ss << "[";
    for (size_t i = 0; i < fields.size(); ++i) {
        ss << fields[i] << (i == fields.size() - 1 ? "" : ", ");
    }
    ss << "]";
    return ss.str();
}
