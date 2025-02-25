#include <cassert>
#include <cmath>
#include <algorithm>
#include <chrono>

#include "simulation_model.h"

#define ASSERT_MSG(condition, message) \
    do { \
        if (!(condition)) { \
            std::cerr << "ERROR: " << message << "\n"; \
            assert(condition); \
        } \
    } while (false)


const double Model::fallibility = 1e-6;

//here, lambdas make sense of parameters for random variables, as well as the state of a random process
Model::Model(int n, std::vector<double> &lambdaI)
{
	ASSERT_MSG(n > 0, "The number of states must be greater than 0!");
	ASSERT_MSG(n == lambdaI.size(), "The number of states does not match the number of values of a random process!");
	if (!std::is_sorted(lambdaI.begin(), lambdaI.end(), std::greater<int>()))
	{
		std::sort(lambdaI.begin(), lambdaI.end(), std::greater<int>());
	}
	m_numberStates = n;
	for (int i = 0; i < n; ++i)
	{
		m_states.emplace_back(i, n);
		m_lambdaI.push_back(lambdaI[i]);
	}
}

void Model::inputRandomVariableParams()
{
	double alpha;
	for (int i = 0; i < m_states.size(); ++i)
	{
		std::cout << "Enter the parameter of the random variable alpha for " << i + 1 << " state: ";
		std::cin >> alpha;
		std::cout << std::endl;

		m_states[i].setRandomVariableParams(m_lambdaI[i], alpha);
	}
}

void Model::inputTransitionProbability()
{
	double prob1, prob2;
	double sum_probabilities_firstRV, sum_probabilities_secondRV;
	for (int i = 0; i < m_states.size(); ++i)
	{
		std::cout << "The transition probabilities for " << i + 1 << " state:\n";
		sum_probabilities_firstRV = 0;
		sum_probabilities_secondRV = 0;
		for (int j = 0; j < m_states.size(); ++j)
		{
			std::cout << "enter the probabilities of transitions to the " << j + 1 << " state by the first random and second random values, respectively: ";
			std::cin >> prob1 >> prob2;
			std::cout << std::endl;
			ASSERT_MSG(prob1 <= 1 && prob2 <= 1, "The probability cannot be greater than 1!");
			ASSERT_MSG(prob1 > 0 && prob2 > 0, "The probability cannot be less than zero!");
			m_states[i].setTransitionProbability(j, prob1, prob2);
			sum_probabilities_firstRV = sum_probabilities_firstRV + prob1;
			sum_probabilities_secondRV = sum_probabilities_secondRV + prob2;
		}
		ASSERT_MSG(sum_probabilities_firstRV == 1 && sum_probabilities_secondRV == 1, "The sum of the probabilities should be 1!");
	}
}

void Model::print() const
{
	for (const auto& state : m_states)
	{
		state.print();
	}
}

int Model::_lotteryInitialState() const
{
	//1 step
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dist(0, 100);
	double random_number = dist(gen) / 100.0;
	//2 step
	random_number = random_number * m_numberStates;
	if (random_number == m_numberStates)
	{
		random_number -= 1;
	}
	return static_cast<int>(random_number);
}

double Model::_timeSpentState(const int state, NumberRV& numberRV) 
{
	//1 step
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> dist(0.0, 1.0);
	double first_random_number = dist(gen);
	double second_random_number = dist(gen);
	//2 step
	double tao1 = -(std::log(1 - first_random_number) / m_states[state].m_paramsRV.lambda);
	double tao2 = -(std::log(1 - second_random_number) / m_states[state].m_paramsRV.alpha);
	//3 step
	double tao = std::min(tao1, tao2);
    numberRV = (tao == tao1) ? NumberRV::First : NumberRV::Second;
    //round to 2 decimal places
    return (std::round(tao * 100.0) / 100.0);
}

int Model::_lotteryState(int fromState, NumberRV& numberRV) const
{
	double sum = 0;
	std::random_device rd;
    std::mt19937 gen(std::chrono::system_clock::now().time_since_epoch().count());
	std::uniform_int_distribution<> dist(0, 100);
	double random_number = dist(gen) / 100.0;
	for (int i = 0; i < m_numberStates; ++i)
	{
		if (numberRV == NumberRV::First)
		{
			sum = sum + m_states[fromState].m_transitionProbabilities[i].first;
		}
		else
		{
			sum = sum + m_states[fromState].m_transitionProbabilities[i].second;
		}
		if (random_number <= sum)
		{
			return i;
		}
	}
	return m_numberStates - 1;
}

std::vector<double> Model::simulation(const double endTime, double beginTime)
{
	std::vector<double> result_vector;
	result_vector.push_back(beginTime);
	int initial_state = _lotteryInitialState();
	//add a counter that counts the number of stays in the state
	m_states[initial_state].m_count++; 
	NumberRV number_RV = NumberRV::First;
	double tao = _timeSpentState(initial_state, number_RV);
	beginTime = beginTime + tao;
	m_states[initial_state].m_timeSpent += tao;
	result_vector.push_back(beginTime);
	int current_state = initial_state;
	while (beginTime <= endTime)
	{
		current_state = _lotteryState(current_state, number_RV);
		assert(current_state != INT_MAX);
		//add a counter that counts the number of stays in the state
		m_states[current_state].m_count++;
		tao = _timeSpentState(current_state, number_RV);
		beginTime = beginTime + tao;
		if (beginTime <= endTime)
		{
			m_states[current_state].m_timeSpent += tao;
			result_vector.push_back(beginTime);
		}
		else
		{

			m_states[current_state].m_timeSpent += endTime - (beginTime - tao);
			result_vector.push_back(endTime);
		}
	}
	return result_vector;
}

int Model::numberStaysInIthState(int state_id) const
{
	ASSERT_MSG(state_id >= 0, "The state number cannot be negative!");
	ASSERT_MSG(state_id < m_states.size(), "The state number is greater than the number of states");
	return m_states[state_id].m_count;
}

void Model::readingTransitionProbabilityFromFiles(const std::string& firstRVProbabilityFile, const std::string& secondRVProbabilityFile)
{
	std::vector<std::vector<double>> matrix1 = readMatrixFromFile(firstRVProbabilityFile);
	std::vector<std::vector<double>> matrix2 = readMatrixFromFile(secondRVProbabilityFile);
	
	double sum_probabilities_firstRV = 0.0, sum_probabilities_secondRV = 0.0;
	double prob1, prob2;
	for (int i = 0; i < m_numberStates; ++i)
	{
		sum_probabilities_firstRV = 0.0;
		sum_probabilities_secondRV = 0.0;
		for (int j = 0; j < m_numberStates; ++j)
		{
			prob1 = matrix1[i][j];
			prob2 = matrix2[i][j];
			//ASSERT_MSG(prob1 <= 1 && prob2 <= 1, "The probability cannot be greater than 1!");
			//ASSERT_MSG(prob1 > 0 && prob2 > 0, "The probability cannot be less than zero!");
			m_states[i].setTransitionProbability(j, prob1, prob2);
			sum_probabilities_firstRV = sum_probabilities_firstRV + prob1;
			sum_probabilities_secondRV = sum_probabilities_secondRV + prob2;
		}
		//ASSERT_MSG(sum_probabilities_firstRV == 1 && sum_probabilities_secondRV == 1, "The sum of the probabilities should be 1!");
		ASSERT_MSG((1 - sum_probabilities_firstRV) <= fallibility && (1 - sum_probabilities_secondRV) <= fallibility, "The sum of the probabilities should be 1!");
	}
}

void Model::setParamsRandomVariable(const std::vector<double>& alphas)
{
    for (int i = 0; i < m_states.size(); ++i)
    {
        m_states[i].setRandomVariableParams(m_lambdaI[i], alphas[i]);
    }
}

void Model::setTransitionProbabilities(const std::vector<std::vector<double>>& firstTransitions, const std::vector<std::vector<double>>& secondTransitions)
{
    double sum_probabilities_firstRV = 0, sum_probabilities_secondRV = 0;
    for (int i = 0; i < m_states.size(); ++i)
    {
        sum_probabilities_firstRV = 0;
        sum_probabilities_secondRV = 0;
        for (int j = 0; j < m_states.size(); ++j)
        {
            ASSERT_MSG(firstTransitions[i][j] <= 1 && secondTransitions[i][j] <= 1, "The probability cannot be greater than 1!");
            ASSERT_MSG(firstTransitions[i][j] > 0 && secondTransitions[i][j] > 0, "The probability cannot be less than zero!");
            m_states[i].setTransitionProbability(j, firstTransitions[i][j], secondTransitions[i][j]);
            sum_probabilities_firstRV = sum_probabilities_firstRV + firstTransitions[i][j];
            sum_probabilities_secondRV = sum_probabilities_secondRV + secondTransitions[i][j];
        }
        ASSERT_MSG(sum_probabilities_firstRV == 1 && sum_probabilities_secondRV == 1, "The sum of the probabilities should be 1!");
    }
}

void Model::clear()
{
	for (int i = 0; i < m_numberStates; ++i)
	{
		m_states[i].m_count = 0;
		m_states[i].m_timeSpent = 0.0;
	}
}

std::vector<double> Model::assessmentLengthStayInStates(const int countIteration, const double endTime)
{
	std::vector<double> result(m_numberStates, 0.0);
	for (int i = 1; i <= countIteration; ++i)
	{
		simulation(endTime);
		for (int j = 0; j < m_numberStates; ++j)
		{
			result[j] += m_states[j].m_timeSpent;
		}
		clear();
	}
	for (double& num : result)
	{
		num /= countIteration;
	}
	return result;
}

int State::getId() const
{
	return m_id;
}

int Model::getNumberStates() const
{
	return m_numberStates;
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
