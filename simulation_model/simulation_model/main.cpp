#include <iostream>

#include "simulation_model.h"

int main()
{
	int n;
	std::cout << "Enter number of states: ";
	std::cin >> n;
	std::cout << std::endl;
	std::vector<int> vec = { 1, 2 };
	Model model(n, vec);
	model.inputTransitionProbability();
	model.inputRandomVariableParams();
	model.simulation(5);
	model.print();
	return 0;
}