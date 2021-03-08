#pragma once
#include "..\CSC8503Common\GameObject.h"
#include <functional>
namespace NCL {
	namespace CSC8503 {

		class Trigger : public GameObject {
		public:
			Trigger() { SetTag(Tag::_Trigger); }
			void SetActive(bool active) { isActive = active; };
			void ExecuteOnEnter(GameObject* object);
			void ExecuteOnExit(GameObject* object);
			void SubscribeOnEnter(std::function < void(GameObject* obj)> func) { OnEnter.push_back(func); };
			void SubscribeOnExit(std::function < void(GameObject* obj)> func) { OnExit.push_back(func); };
			void OnCollisionBegin(GameObject* otherObject) override { ExecuteOnEnter(otherObject); };
			void OnCollisionEnd(GameObject* otherObject) override { ExecuteOnExit(otherObject); };
		protected:
			vector < std::function < void(GameObject* obj) >> OnEnter;
			vector < std::function < void(GameObject* obj) >> OnExit;

		};
	}
}