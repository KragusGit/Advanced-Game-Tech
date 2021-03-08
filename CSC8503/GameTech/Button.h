#pragma once
#include "../CSC8503Common/PhysicsSystem.h"
#include<functional>
#include<vector>
namespace NCL {
	namespace CSC8503 {

		class Button : public GameObject {

		public:
			Button() {};
			~Button() { OnPress.clear(); };

			void ExecuteOnPress();
			void SubscribeOnPress(std::function < void()> func) { OnPress.push_back(func); };
			void OnClick() override { ExecuteOnPress(); };
		protected:
			vector < std::function < void() >> OnPress;

		};

	}
}