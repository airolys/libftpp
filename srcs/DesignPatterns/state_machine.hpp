#include <iostream>
#include <functional>
#include <memory>
#include <map>

template <typename TState>
class StateMachine
{
	struct StateInfo_s
	{
		std::function<void()>					action;
		std::map<TState, std::function<void()>>	transitions;
	};

	public:

		StateMachine();
		~StateMachine();

		void	update();
		void	transitionTo(const TState& state);

		void	addState(const TState& state);
		void	addAction(const TState& state, const std::function<void()>& lambda);
		void	addTransition(const TState& startState, const TState& endState, const std::function<void()>& lambda);

	private:

		StateInfo_s*									_curr;
		std::map<TState, std::unique_ptr<StateInfo_s>>	_states;
	
		StateInfo_s&	_getState(const TState& state);
};

#include "state_machine.tpp"
