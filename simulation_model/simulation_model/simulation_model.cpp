#include "simulation_model.h"

Model::Model(int n)
{
	for (auto& i : m_states)
	{
		m_states.emplace_back(i, n);
	}
}
