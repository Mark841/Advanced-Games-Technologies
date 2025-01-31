#pragma once
#include <functional>

namespace NCL {
	namespace CSC8503 {
		typedef std::function<void(float)> StateUpdateFunction;

		class State {
		public:
			State() {}
			State(StateUpdateFunction someFunc) {
				func = someFunc;
			}
			virtual ~State() {}

			void Update(float dt)
			{
				if (func != nullptr)
				{
					func(dt);
				}
			}
		protected:
			StateUpdateFunction func;
		};

		typedef void(*StateFunc)(void*);

		class GenericState : public State {
		public:
			GenericState(StateFunc someFunc, void* someData) {
				func = someFunc;
				funcData = someData;
			}
			virtual void Update() {
				if (funcData != nullptr) {
					func(funcData);
				}
			}
		protected:
			StateFunc func;
			void* funcData;
		};
	}
}