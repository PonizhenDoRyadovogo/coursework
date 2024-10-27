#include <iostream>

#include "simulation_model.h"

int main()
{
	int n;
	std::cout << "Enter number of states: ";
	std::cin >> n;
	std::cout << std::endl;
	Model model(n);
	model.inputTransitionProbability();
	model.inputRandomVariableParams();
	model.print();
	return 0;
}