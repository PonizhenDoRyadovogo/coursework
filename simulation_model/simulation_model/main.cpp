#include <iostream>
#include <iomanip>

#include "simulation_model.h"

int main()
{
	//double t = 0.015657;
	//double t2 = 0.984343;
	//double sum = t + t2;
	//std::cout << sum;
	std::vector<double> vec = { 0.003135, 0.005035, 0.016618, 0.013922, 0.014137, 0.004621, 0.005425, 0.019066, 0.014348, 0.011812, 0.001708, 0.003482, 0.019568,
		0.010003, 0.005196, 0.008141, 0.014311, 0.008675, 0.008226, 0.011692, 0.014726, 0.006399, 0.008848, 0.010466, 0.006428, 0.013085, 0.002616, 0.000503,
		0.011532, 0.012939, 0.009828, 0.007996, 0.012368, 0.011523, 0.009807, 0.019266, 0.009249, 0.008856, 0.019183, 0.010879, 0.01533, 0.003441, 0.00604,
		0.019718, 0.005091, 0.017687, 0.013324, 0.013273, 0.003577, 0.004533, 0.011245, 0.006277, 0.000173, 0.017681, 0.014011, 0.012771, 0.017408, 0.018022,
		0.006922, 0.005479, 0.001945, 0.001821, 6.2e-05,  0.007021, 0.00983, 0.003736, 0.007664,  0.010431, 0.011822, 0.007501, 0.018283, 0.007694, 0.00689, 0.012875,
		0.013753, 0.005899, 0.006023, 0.005391, 0.014836, 0.009705, 0.014678, 0.015921, 0.005086, 0.015365, 0.017383, 0.015426, 0.009785, 0.017841, 0.004202,
		0.015523, 0.004441, 0.009014, 0.002958, 0.003252, 0.011366, 0.002966, 0.018155, 0.006116, 0.010667, 0.019052 };
	double sum = 0.0;
	for (int i = 0; i < 100; ++i)
	{
		sum += vec[i];
	}
	/*if (1.0 <= sum)
	{
		std::cout << sum;
	}*/
	std::cout << sum;
	return 1;						
	int n;
	double end_time;
	std::cout << "Enter number of states: ";
	std::cin >> n;
	std::cout << std::endl;
	std::cout << "Enter end time: ";
	std::cin >> end_time;
	std::cout << std::endl;
	//bool b = generationProbabilitiesFiles(n);
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
		std::cout << "Среднее время пребвания в " << i + 1 << " состоянии: " << result[i] << std::endl;
	}
	return 0;
}