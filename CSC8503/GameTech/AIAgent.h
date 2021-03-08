#pragma once
#include "../CSC8503Common/BehaviourAction.h"
#include "../CSC8503Common/BehaviourSequence.h"
#include "../CSC8503Common/BehaviourSelector.h"
#include "../CSC8503Common/NavigationGrid.h"
#include "Trigger.h"
namespace NCL {
	namespace CSC8503 {

		class AIAgent :public GameObject {
		public:
			~AIAgent();
			void Update(float dt);
			void SetPathNodes(vector<Vector3>path);
			void Goto(Vector3 point, float dt);
			void FollowPath(float dt);
			void LookAt(Vector3 node, float dt);
			void Move(Vector3 dir, float dt);

			bool move;
			bool HasReachedDestination() { return currentPathIndex == pathNodes.size(); };
			void SetCurrentTarget(Trigger* target) { currentTarget = target; };
			Trigger* GetCurrentTarget() { return currentTarget; };
			void SetSpeed(float speed) { this->speed = speed; };
			BehaviourNode* mainBehaviour;
			BehaviourNode* bonusBehaviour;
		protected:
			vector <Vector3> pathNodes;
			int currentPathIndex;
			float steerOffsetAngle = 0;
			Trigger* currentTarget;
			float speed = 5000;

		};
	}
}