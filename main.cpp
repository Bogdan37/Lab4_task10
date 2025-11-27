#include "TestRunner.h"
#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <vector>
#include <string>


int main() {
    try {

        setup_test_files();


        std::cout << std::fixed << std::setprecision(2);
        std::cout << "--- Starting Tests ---" << std::endl;
        std::cout << std::setw(25) << "" << std::setw(15) << "1 Thread (ms)" << std::setw(15) << "2 Threads (ms)" << std::setw(15) << "3 Threads (ms)" << std::endl;
        std::cout << "----------------------------------------------------------------------" << std::endl;


        double a1 = run_test({"a_1.txt"});
        double a2 = run_test({"a_1.txt", "a_2.txt"});
        double a3 = run_test({"a_1.txt", "a_2.txt", "a_3.txt"});
        std::cout << std::setw(25) << "Scenario A (Variant 10):"
                  << std::setw(15) << a1
                  << std::setw(15) << a2
                  << std::setw(15) << a3 << std::endl;


        double b1 = run_test({"b_1.txt"});
        double b2 = run_test({"b_1.txt", "b_2.txt"});
        double b3 = run_test({"b_1.txt", "b_2.txt", "b_3.txt"});
        std::cout << std::setw(25) << "Scenario B (Uniform):"
                  << std::setw(15) << b1
                  << std::setw(15) << b2
                  << std::setw(15) << b3 << std::endl;


        double c1 = run_test({"c_1.txt"});
        double c2 = run_test({"c_1.txt", "c_2.txt"});
        double c3 = run_test({"c_1.txt", "c_2.txt", "c_3.txt"});
        std::cout << std::setw(25) << "Scenario C (Worst Case):"
                  << std::setw(15) << c1
                  << std::setw(15) << c2
                  << std::setw(15) << c3 << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}