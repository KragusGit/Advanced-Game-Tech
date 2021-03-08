#include "Trigger.h"

void NCL::CSC8503::Trigger::ExecuteOnEnter(GameObject* object) {
	for (int i = 0; i < OnEnter.size(); i++) {
		if (OnEnter[i] != nullptr)
			OnEnter[i](object);
	}
}

void NCL::CSC8503::Trigger::ExecuteOnExit(GameObject* object) {
	for (int i = 0; i < OnExit.size(); i++) {
		if (OnExit[i] != nullptr)
			OnExit[i](object);
	}
}
