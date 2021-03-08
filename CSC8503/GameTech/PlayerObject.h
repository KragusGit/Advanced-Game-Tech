#pragma once
#include "..\CSC8503Common\GameObject.h"
namespace NCL {
	namespace CSC8503 {

		class PlayerObject : public GameObject {
		public:
			bool moveFwd;
			bool moveBack;
			bool turnLeft;
			bool turnRight;
			
			const float MAX_VELOCITY = 20;
			

			void OnCollisionBegin(GameObject* obj) override;
	//		void OnCollisionEnd(GameObject* obj) override;
			virtual void Update(float dt);


			float speed=5000;
		protected:
			void Jump(float dt);
			void TurnLeft(float dt);
			void TurnRight(float dt);
			void MoveFwd(float dt);
			void MoveBack(float dt);
			bool inAir = false;
		};
	}
}