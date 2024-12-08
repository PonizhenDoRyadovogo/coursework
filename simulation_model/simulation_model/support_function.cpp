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
    // Создаем генератор случайных чисел
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dist(0.0, 1.0); // Генерация чисел от 0 до 1
    
    // Создаем два файла для записи
    std::ofstream firstFile("first.txt");
    std::ofstream secondFile("second.txt");
    
    if (!firstFile.is_open() || !secondFile.is_open()) {
        return false;  // Ошибка при открытии файлов
    }

    // Количество строк и чисел в строках
    int numRows = number_states;
    int numCols = number_states;
    
    for (int fileIndex = 0; fileIndex < 2; ++fileIndex) {
        // Для каждого файла генерируем строки
        for (int i = 0; i < numRows; ++i) {
            std::vector<double> row(numCols);
    
            // Генерация случайных чисел в строке
            double sum = 0.0;
            for (int j = 0; j < numCols; ++j) {
                row[j] = dist(gen);
                sum += row[j];
            }
    
            // Нормализация чисел, чтобы их сумма была равна 1
            for (int j = 0; j < numCols; ++j) {
                row[j] /= sum;  // Делим каждое число на сумму строки
            }

            // Округляем все числа до 6 знаков после запятой
            for (int j = 0; j < numCols; ++j) {
                row[j] = std::trunc(row[j] * 1e6) / 1e6;  // Округляем до 6 
            }

            // Проверка, чтобы сумма чисел в строке была точно 1
            double new_sum = 0.0;
            for (int j = 0; j < numCols; ++j) {
                new_sum += row[j];
            }

            //new_sum = std::round(new_sum * 1e6) / 1e6;
            
            // Если сумма по каким-то причинам не равна 1, корректируем
            if (new_sum < 1.0) {
                row[numCols - 1] += (1.0 - new_sum);  // Корректируем последний элемент
            }
            else if (new_sum > 1.0)
            {
                row[numCols - 1] -= (new_sum - 1.0);
            }
    
            new_sum = 0.0;
            for (int j = 0; j < numCols; ++j) {
                new_sum += row[j];
            }

            // Записываем данные в файл
            if (fileIndex == 0) {
                // Записываем в первый файл
                for (int j = 0; j < numCols; ++j) {
                    firstFile << std::fixed << std::setprecision(6) << row[j] << " ";
                }
                firstFile << "\n";
            }
            else {
                // Записываем во второй файл
                for (int j = 0; j < numCols; ++j) {
                    secondFile << std::fixed << std::setprecision(6) << row[j] << " ";
                }
                secondFile << "\n";
            }
        }
    }
    
    // Закрываем файлы
    firstFile.close();
    secondFile.close();
    
    return true;
}
