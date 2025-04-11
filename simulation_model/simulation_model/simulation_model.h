#ifndef SIMULATION_MODEL
#define SIMULATION_MODEL

#include <iostream>
#include <vector>
#include <random>
#include <utility>

#include "support_function.h"


//the structure stores the parameters of random variables
struct RandomVariableParametrs {
	double lambda;
	double alpha;
};

enum class NumberRV
{
	First,
	Second
};

//the structure stores information about states (state number, transition probabilities, parameters of random variables)
class State {
public:
	friend class Model;
    friend class MainWindow;
public:
	//ToDo: State();
	State(int id, int n)
		: m_id(id), m_transitionProbabilities(n)
	{
		m_paramsRV.lambda = 0;
		m_paramsRV.alpha = 0;
	}
	int getId() const;
	void setTransitionProbability(int toState, double prob1, double prob2);
	void setRandomVariableParams(double lambda, double alpha);

	void print() const;
protected:
	int m_id;
	std::vector<std::pair<double, double>> m_transitionProbabilities;
	RandomVariableParametrs m_paramsRV;
	int m_count = 0;
	double m_timeSpent = 0;
};

class Model {
public:
    friend class MainWindow;
public:
    Model() = default;
    Model(int n, std::vector<double>& lambdaI);
	int getNumberStates() const;
	void inputRandomVariableParams();
	void inputTransitionProbability();
	void print() const;
	std::vector<double> simulation(const double endTime, double beginTime = 0.0);
	int numberStaysInIthState(int state_id) const;
	void readingTransitionProbabilityFromFiles(const std::string& firstRVProbabilityFile, const std::string& secondRVProbabilityFile);
    void setParamsRandomVariable(const std::vector<double>& alphas);
    void setTransitionProbabilities(const std::vector<std::vector<double>>& firstTransitions, const std::vector<std::vector<double>>& secondTransitions);
	void clear();
public://experiments
	std::vector<double> assessmentLengthStayInStates(const int countIteration, const double endTime);
	double assessmentIntervalDuration(const int countIteration, const double endTime);
public: 
	static const double fallibility;
private:
    int _lotteryInitialState() const;
	double _timeSpentState(const int state, NumberRV& numberRV);
	int _lotteryState(int fromState, NumberRV& numberRV) const;
private:
    std::vector<double> m_lambdaI;
	std::vector<State> m_states;
	int m_numberStates = 2;
};

#endif