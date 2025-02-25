#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <random>
#include <iomanip>
#include <assert.h>

std::vector<std::vector<double>> readMatrixFromFile(const std::string& fileName);
bool generationProbabilitiesFiles(int number_states);
void writeVectorToFile(const std::vector<double>& vec, const std::string& fileName);
std::vector<double> readRVFromFile(const std::string& fileName);