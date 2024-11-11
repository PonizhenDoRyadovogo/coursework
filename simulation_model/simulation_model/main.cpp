#include <iostream>

#include "simulation_model.h"

int main()
{
	int n;
	std::cout << "Enter number of states: ";
	std::cin >> n;
	std::cout << std::endl;
	std::vector<int> vec = { 3, 1};
	Model model(n, vec);
	model.inputTransitionProbability();
	model.inputRandomVariableParams();
	std::vector<double> result = model.simulation(5);
	for (const auto& i : result)
	{
		std::cout << i << " ";
	}
	return 0;
}