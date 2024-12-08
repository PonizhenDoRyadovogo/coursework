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
bool generationProbabilitiesFiles(int number_states)
{
    // ������� ��������� ��������� �����
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dist(0.0, 1.0); // ��������� ����� �� 0 �� 1
    
    // ������� ��� ����� ��� ������
    std::ofstream firstFile("first.txt");
    std::ofstream secondFile("second.txt");
    
    if (!firstFile.is_open() || !secondFile.is_open()) {
        return false;  // ������ ��� �������� ������
    }

    // ���������� ����� � ����� � �������
    int numRows = number_states;
    int numCols = number_states;
    
    for (int fileIndex = 0; fileIndex < 2; ++fileIndex) {
        // ��� ������� ����� ���������� ������
        for (int i = 0; i < numRows; ++i) {
            std::vector<double> row(numCols);
    
            // ��������� ��������� ����� � ������
            double sum = 0.0;
            for (int j = 0; j < numCols; ++j) {
                row[j] = dist(gen);
                sum += row[j];
            }
    
            // ������������ �����, ����� �� ����� ���� ����� 1
            for (int j = 0; j < numCols; ++j) {
                row[j] /= sum;  // ����� ������ ����� �� ����� ������
            }

            // ��������� ��� ����� �� 6 ������ ����� �������
            for (int j = 0; j < numCols; ++j) {
                row[j] = std::trunc(row[j] * 1e6) / 1e6;  // ��������� �� 6 
            }

            // ��������, ����� ����� ����� � ������ ���� ����� 1
            double new_sum = 0.0;
            for (int j = 0; j < numCols; ++j) {
                new_sum += row[j];
            }

            //new_sum = std::round(new_sum * 1e6) / 1e6;
            
            // ���� ����� �� �����-�� �������� �� ����� 1, ������������
            if (new_sum < 1.0) {
                row[numCols - 1] += (1.0 - new_sum);  // ������������ ��������� �������
            }
            else if (new_sum > 1.0)
            {
                row[numCols - 1] -= (new_sum - 1.0);
            }
    
            new_sum = 0.0;
            for (int j = 0; j < numCols; ++j) {
                new_sum += row[j];
            }

            // ���������� ������ � ����
            if (fileIndex == 0) {
                // ���������� � ������ ����
                for (int j = 0; j < numCols; ++j) {
                    firstFile << std::fixed << std::setprecision(6) << row[j] << " ";
                }
                firstFile << "\n";
            }
            else {
                // ���������� �� ������ ����
                for (int j = 0; j < numCols; ++j) {
                    secondFile << std::fixed << std::setprecision(6) << row[j] << " ";
                }
                secondFile << "\n";
            }
        }
    }
    
    // ��������� �����
    firstFile.close();
    secondFile.close();
    
    return true;
}
