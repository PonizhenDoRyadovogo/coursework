#include "support_function.h"

std::vector<std::vector<double>> readMatrixFromFile(const std::string& fileName)
{
	std::vector<std::vector<double>> matrix;
	std::ifstream file(fileName);

	if (!file.is_open())
	{
		std::cerr << "Can't open file!\n";
		return matrix;
	}

	std::string line;
	while (std::getline(file, line))
	{
		std::vector<double> row;
		double number;
		std::stringstream ss(line);

		while (ss >> number)
		{
			row.push_back(number);
		}
		
		matrix.push_back(row);
	}

	file.close();
	return matrix;
}