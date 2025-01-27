#include <iostream>
#include <iomanip>

#include "simulation_model.h"

int main()
{			
	int n;
	double end_time;
	std::cout << "Enter number of states: ";
	std::cin >> n;
	std::cout << std::endl;
	std::cout << "Enter end time: ";
	std::cin >> end_time;
	std::cout << std::endl;
	bool b = generationProbabilitiesFiles(n);
	std::vector <double> lambds;
	std::vector <double> alphas;
	std::vector<std::vector<double>> data;
	for (int i = 0; i < n; ++i)
	{
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<> dist(0.0, 100.0);
		double random_number = dist(gen);
		lambds.push_back(random_number);
	}
	for (int i = 0; i < n; ++i)
	{
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<> dist(0.0, 100.0);
		double random_number = dist(gen);
		alphas.push_back(random_number);
	}
	std::vector<double> iteration;
	for (int i = 0; i < 100; ++i)
	{
		iteration.clear();
		Model* model = new Model(n, lambds);
		model->readingTransitionProbabilityFromFiles("first.txt", "second.txt");
		model->setParamsRandomVariable(alphas);
		model->simulation(end_time);
		double total_number_iterations = 0.0;
		for (int i = 0; i < n; ++i)
		{
			total_number_iterations += static_cast<double>(model->numberStaysInIthState(i));
			iteration.push_back(static_cast<double>(model->numberStaysInIthState(i)));
		}
		for (int i = 0; i < n; ++i)
		{
			iteration[i] /= total_number_iterations;
		}
		data.push_back(iteration);
	}
	std::vector<double> result;
	for (int i = 0; i < n; ++i)
	{
		double sum = 0.0;
		for (int j = 0; j < n; ++j)
		{
			sum += data[j][i];
		}
		sum /= static_cast<double>(n);
		result.push_back(sum);
	}
	for (int i = 0; i < n; ++i)
	{
		std::cout << "Average time spent in " << i + 1 << " state: " << result[i] << std::endl;
	}
	return 0;
}