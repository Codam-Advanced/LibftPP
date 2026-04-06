#pragma once

#include <algorithm>
#include <functional>
#include <map>
#include <new>
#include <set>
#include <stdexcept>
#include <vector>
template<typename TState> class StateMachine
{

	using Callback = std::function<void()>;

	public:

		StateMachine() = delete;
		StateMachine(const TState& startState): _currentState(startState) { addState(startState); }
		
	 	/**
	 	 * @brief 
	 	 * 
	 	 * @param state 
	 	 */
		void addState(const TState& state)
		{
			auto it = _states.find(state);

			if (it != _states.end())
				return;

			_states.emplace(state, t_stateActions());
		}

		/**
		 * @brief Specifies what to execute upon a specific transition.
		 */
		void addTransition(const TState& startState, const TState& finalState, const Callback& lambda)
		{
			t_stateActions& action = getStateAction(startState);
			std::map<TState, std::vector<Callback>>& transition = action.transitionsActions;
			auto it = transition.find(finalState);
			std::vector<Callback>* vec;

			if (it == transition.end())
			{
				auto newObject = transition.emplace(finalState, std::move(std::vector<Callback>()));

				if (!newObject.second)
					throw std::bad_alloc();

				vec = &newObject.first->second;
			}
			else 
			{
				vec = &it->second;
			}

			// store the lambda function
			vec->push_back(lambda);
		}

		/**
		 * @brief  Specifies what to execute when the StateMachine is in a specific state.
		 */
		void addAction(const TState& state, const Callback& lambda) 
		{
			t_stateActions& action = getStateAction(state);

			// add an action to the state
			action.actions.push_back(lambda);
		}
		
		/**
		 * @brief Executes the transition to a specific state.
		 */
		void transitionTo(const TState& state)
		{
			t_stateActions& action = getStateAction(_currentState);
			std::map<TState, std::vector<Callback>>& transition = action.transitionsActions;
			auto it = transition.find(state);

			if (it == transition.end())
				throw std::runtime_error("Transition not registered for current state: [" + std::to_string(static_cast<int>(_currentState)) + "] to state: [" + std::to_string(static_cast<int>(state)) + "]");

			for (auto& callback : it->second)
			{
				// execute the callback
				callback();
			}

			// transition to the new state
			_currentState = state;
		}

		/**
		 * @brief Executes the action registered for the current state.
		 */
		void update()
		{
			t_stateActions& action = getStateAction(_currentState);

			for (auto& callback : action.actions)
			{
				// execute the callback
				callback();
			}
		}

	private:
	
		typedef struct s_stateActions
		{
			std::map<TState, std::vector<Callback>> transitionsActions;
			std::vector<Callback> actions;
		} t_stateActions;

		t_stateActions& getStateAction(const TState& state)
		{
			auto it = _states.find(state);

			if (it == _states.end())
				throw std::runtime_error("State not registered! state [" + std::to_string(static_cast<int>(state)) + "]");
			
			// return the state actions object
			return it->second;
		}

		
		std::map<TState, t_stateActions> _states;
		TState _currentState;
};