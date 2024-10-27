#pragma once

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
};

class Model {
public:
	//ToDo: Model();
	Model(int n, std::vector<int>& lambdaI);
	int getNumberStates() const;
	void inputRandomVariableParams();
	void inputTransitionProbability();
	void print() const;
	std::vector<double> simulation(const double end_time, double begin_time = 0.0);
private:
	int _lotteryInitialState() const;
	double _timeSpentState(const int state, NumberRV& number_RV);
	int _lotteryState(int from_state, NumberRV& number_rv) const;
private:
	std::vector<int> m_lambdaI;
	std::vector<State> m_states;
	int m_number_states = 2;
};