#pragma once

#include <iostream>
#include <vector>
#include <random>
#include <utility>
#include <assert.h>

//the structure stores the parameters of random variables
struct RandomVariableParametrs {
	double lambda;
	double alpha;
};

//the structure stores information about states (state number, transition probabilities, parameters of random variables)
class State {
public:
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
private:
	int m_id;
	std::vector<std::pair<double, double>> m_transitionProbabilities;
	RandomVariableParametrs m_paramsRV;
};


class Model {
public:
	Model(int n);
	int getNumberStates() const;
	void inputRandomVariableParams();
	void inputTransitionProbability();
	void print() const;
private:
	std::vector<State> m_states;
	int m_number_states = 2;
};