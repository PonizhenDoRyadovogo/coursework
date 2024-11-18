#include <iostream>

#include "simulation_model.h"

int main()
{
	int n;
	std::cout << "Enter number of states: ";
	std::cin >> n;
	std::cout << std::endl;
	std::vector<int> lambds = { 3, 2, 8, 4};
	Model model(n, lambds);
	model.inputTransitionProbability();
	model.inputRandomVariableParams();
	std::vector<double> result = model.simulation(5);
	for (const auto& i : result)
	{
		std::cout << i << " ";
	}
	std::cout << std::endl;
	for (int i = 0; i < n; ++i)
	{
		std::cout << "Number of stays in " << i + 1 << " state: " << model.numberStaysInIthState(i);
	}
	return 0;
}