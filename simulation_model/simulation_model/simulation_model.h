#pragma once

#include <iostream>
#include <vector>
#include <random>
#include <utility>
#include <assert.h>

//the structure stores information about states (state number, transition probabilities, parameters of random variables)
class State {
public:
	State(int id, int n)
		: m_id(id), m_transitionProbabilities(n), m_randomVariables(n)
	{}
private:
	int m_id;
	std::vector<std::vector<std::pair<double, double>>> m_transitionProbabilities;
	//the structure stores the parameters of random variables
	struct RandomVariableParametrs {
		double lambda;
		double alfa;
	};
	std::vector<RandomVariableParametrs> m_randomVariables;
};


class Model {
public:

private:
	std::vector<State> m_states;
};