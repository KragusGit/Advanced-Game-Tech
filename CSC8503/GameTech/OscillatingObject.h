#pragma once
#include "../CSC8503Common/PhysicsSystem.h"
#include "../CSC8503Common/StateMachine.h"
#include "../CSC8503Common/State.h"
#include "../CSC8503Common/StateTransition.h"


namespace NCL {
	namespace CSC8503 {

		class OscillatingObject :public GameObject {
		public:
			StateMachine* stateMachine;
			OscillatingObject(vector<Vector3>positions, bool startOscillatingOnSpawn, float timeToTravelOneLeg = 1, float waitAfterRest = 2);
			~OscillatingObject();
			void Update(float dt);
			void GoToNextState();
			void ToggleAutoOscillate();

		protected:
			void Move(float dt);
			bool autoOscillate;
			bool moving = false;
			float timeToTravelOneLeg;
			float elapsedTime;
			float restingTimer;
			float secondsToWaitBeforeMoving;
			int currentPositionIndex = 0;

			vector<	Vector3> positions;
		};
	}
}