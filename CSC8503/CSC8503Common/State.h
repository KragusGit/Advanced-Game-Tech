# pragma once
#include <functional>

namespace NCL {
	namespace CSC8503 {
		typedef std::function < void(float)> StateUpdateFunction;

		class State {
		public:
			State() { uint32_t id = reinterpret_cast<uint32_t>(this); }

			State(StateUpdateFunction someFunc) {
				func = someFunc;
				uint32_t id = reinterpret_cast<uint32_t>(this);
			}
			void Update(float dt) {
				if (func != nullptr) {
					func(dt);
				}
			}

		protected:
			StateUpdateFunction func;
		};
	}
}
