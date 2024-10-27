#include "simulation_model.h"

Model::Model(int n)
{
	m_number_states = n;
	for (int i = 0; i < n; ++i)
	{
		m_states.emplace_back(i, n);
	}
}

void Model::inputRandomVariableParams()
{
	double lambda, alpha;
	for (int i = 0; i < m_states.size(); ++i)
	{
		std::cout << "Enter the parameters of the random variables lambda and alpha, respectively, for " << i + 1 << " state: ";
		std::cin >> lambda >> alpha;
		std::cout << std::endl;
		m_states[i].setRandomVariableParams(lambda, alpha);
	}
}

void Model::inputTransitionProbability()
{
	double prob1, prob2;
	for (int i = 0; i < m_states.size(); ++i)
	{
		std::cout << "The transition probabilities for " << i + 1 << " state:\n";
		for (int j = 0; j < m_states.size(); ++j)
		{
			std::cout << "enter the probabilities of transitions to the " << j + 1 << " state by the first random and second random values, respectively: ";
			std::cin >> prob1 >> prob2;
			std::cout << std::endl;
			m_states[i].setTransitionProbability(j, prob1, prob2);
		}
	}
}

void Model::print() const
{
	for (const auto& state : m_states)
	{
		state.print();
	}
}

int State::getId() const
{
	return m_id;
}

int Model::getNumberStates() const
{
	return m_number_states;
}

void State::setTransitionProbability(int toState, double prob1, double prob2)
{
	if (toState >= 0 && toState < m_transitionProbabilities.size()) {
		m_transitionProbabilities[toState] = {prob1, prob2};
	}
}		

void State::setRandomVariableParams(double lambda, double alpha)
{
	m_paramsRV = { lambda, alpha };
}

void State::print() const
{
	std::cout << "State: " << m_id + 1 << ":\n";
	std::cout << "random variable parameters: lambda = " << m_paramsRV.lambda << ", alpha = " << m_paramsRV.alpha << "\n";
	for (int j = 0; j < m_transitionProbabilities.size(); ++j)
	{
		std::cout << "->" << j + 1 << ": " << m_transitionProbabilities[j].first << ", " << m_transitionProbabilities[j].second << "\n";
	}
}