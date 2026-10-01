#pragma once


template <typename TState>
typename StateMachine<TState>::StateInfo_s&
StateMachine<TState>::_getState(const TState& state)
{
	typename std::map<TState, std::unique_ptr<StateInfo_s> >::iterator it = _states.find(state);
	
	if (it == _states.end())
		throw std::invalid_argument("State not found");
	return *(it->second);
}

template <typename TState>
StateMachine<TState>::StateMachine() : _curr(nullptr)
{}

template <typename TState>
StateMachine<TState>::~StateMachine()
{}

template <typename TState>
void	StateMachine<TState>::addState(const TState &state)
{
	_states[state] = std::make_unique<StateInfo_s>();
	if (!_curr)
		_curr = _states[state].get();
}

template <typename TState>
void	StateMachine<TState>::addTransition(const TState &startState, const TState &endState, const std::function<void()> &lambda)
{
	StateInfo_s& curr = _getState(startState);
	curr.transitions[endState] = lambda;
}

template <typename TState>
void	StateMachine<TState>::addAction(const TState &state, const std::function<void()> &lambda)
{
	StateInfo_s& curr = _getState(state);
	curr.action = lambda;
}

template <typename TState>
void	StateMachine<TState>::transitionTo(const TState &state)
{
	if (!_curr)
		throw std::runtime_error("No current state");

	StateInfo_s& targetState = _getState(state);

	typename std::map<TState, std::function<void()> >::iterator it = _curr->transitions.find(state);
	if (it == _curr->transitions.end())
		throw std::invalid_argument("State not found");
	it->second();

	_curr = &targetState;
}

template <typename TState>
void	StateMachine<TState>::update()
{
	if (!_curr)
		throw std::runtime_error("No current state");

	if (!_curr->action)
		throw std::invalid_argument("State not found");

	_curr->action();
}
