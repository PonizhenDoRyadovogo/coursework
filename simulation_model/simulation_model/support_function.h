#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <random>
#include <iomanip>

std::vector<std::vector<double>> readMatrixFromFile(const std::string& fileName);
bool generationProbabilitiesFiles(int number_states);