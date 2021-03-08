#include <map>
#include <vector>
#include <functional>
namespace NCL {
	namespace CSC8503 {
		class State; // Predeclare the classes we need
		class StateTransition;
		// Typedefs to make life easier !


		typedef std::multimap <State*, StateTransition*> TransitionContainer;
		typedef TransitionContainer::iterator TransitionIterator;

		class StateMachine {
		public:
			StateMachine();
			~StateMachine();

			void AddState(State* s);
			void AddTransition(StateTransition* t);

			void Update(float dt);
			std::function <void(State*, State*)>  onStateChange;

		protected:
			State* activeState;
			std::vector <State*> allStates;
			TransitionContainer allTransitions;

		};
	}
}