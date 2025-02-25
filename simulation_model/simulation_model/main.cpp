#include <iostream>
#include <iomanip>

#include "simulation_model.h"

int main()
{		
	int countState;
	std::cout << "Enter a number of state: " << std::endl;
	std::cin >> countState;
	double endTime;
	std::cout << "Enter end time: " << std::endl;
	std::cin >> endTime;
	//experiment_1
	std::string fileLambds, fileAlphas;
	std::cout<<"Enter the name of the file containing the values of the FIRST random variable(for example lambds.txt)" << std::endl;
	std::cin >> fileLambds;
	std::cout << "Enter the name of the file containing the values of the SECOND random variable(for example alphas.txt)" << std::endl;
	std::cin >> fileAlphas;
	std::vector<double> lambdsExperiment1 = readRVFromFile(fileLambds);
	std::vector<double> alphasExperiment1 = readRVFromFile(fileAlphas);
	Model experiment1(countState, lambdsExperiment1);
	experiment1.setParamsRandomVariable(alphasExperiment1);
	std::string fileFirst, fileSecond;
	std::cout << "Enter the name of the file containing the transition probabilities for the FIRST random variable(for example first.txt)" << std::endl;
	std::cin >> fileFirst;
	std::cout << "Enter the name of the file containing the transition probabilities for the SECOND random variable(for example second.txt)" << std::endl;
	std::cin >> fileSecond;
	experiment1.readingTransitionProbabilityFromFiles(fileFirst, fileSecond);
	for (int i = 50; i <= 1000; i += 50) 
	{
		std::vector<double> resultFirstExperiment = experiment1.assessmentLengthStayInStates(i, endTime);
		std::string fileName = "C:/coursework/resultExperiments/experiment1/firstExperimentWithCountIteration" + std::to_string(i) + ".txt";
		writeVectorToFile(resultFirstExperiment, fileName);
	}
	return 0;
}