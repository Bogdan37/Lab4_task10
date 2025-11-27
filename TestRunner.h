#pragma once

#include <vector>
#include <string>


constexpr long NUM_OPERATIONS = 1000000;


void setup_test_files();


double run_test(const std::vector<std::string>& filenames);
