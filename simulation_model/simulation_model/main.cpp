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
	std::string fileLambds, fileAlphas;
	std::cout<<"Enter the name of the file containing the values of the FIRST random variable(for example lambds.txt)" << std::endl;
	std::cin >> fileLambds;
	std::cout << "Enter the name of the file containing the values of the SECOND random variable(for example alphas.txt)" << std::endl;
	std::cin >> fileAlphas;
	std::vector<double> lambdsExperiment1 = readRVFromFile(fileLambds);
	std::vector<double> alphasExperiment1 = readRVFromFile(fileAlphas);
	std::string fileFirst, fileSecond;
	std::cout << "Enter the name of the file containing the transition probabilities for the FIRST random variable(for example first.txt)" << std::endl;
	std::cin >> fileFirst;
	std::cout << "Enter the name of the file containing the transition probabilities for the SECOND random variable(for example second.txt)" << std::endl;
	std::cin >> fileSecond;
	
	//experiment_1
	/*Model experiment1(countState, lambdsExperiment1);
	experiment1.setParamsRandomVariable(alphasExperiment1);
	experiment1.readingTransitionProbabilityFromFiles(fileFirst, fileSecond);
	for (int i = 50; i <= 1000; i += 50) 
	{
		std::vector<double> resultFirstExperiment = experiment1.assessmentLengthStayInStates(i, endTime);
		std::string fileName = "C:/coursework/resultExperiments/experiment1/firstExperimentWithCountIteration" + std::to_string(i) + ".txt";
		writeVectorToFile(resultFirstExperiment, fileName);
	}*/

	//experiment_2
	Model experiment2(countState, lambdsExperiment1);
	experiment2.setParamsRandomVariable(alphasExperiment1);
	experiment2.readingTransitionProbabilityFromFiles(fileFirst, fileSecond);
	std::vector<double> resultSecondExperiment;
	for (int i = 50; i <= 500; i += 50)
	{
		resultSecondExperiment.push_back(experiment2.assessmentIntervalDuration(1000, i));
	}
	std::string fileName = "C:/coursework/resultExperiments/experiment2/resultSecondExperiment1.txt";
	writeVectorToFile(resultSecondExperiment, fileName);
	return 0;
}