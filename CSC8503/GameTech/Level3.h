#pragma once
#include "Level.h"
#include "PlayerObject.h"


namespace NCL {
	namespace CSC8503 {
		class LevelThree :public Level {
		public:
			LevelThree(GameTechRenderer* renderer);
			~LevelThree();

			PlayerObject* SpawnPlayer(const Vector3& position);
			Trigger* fusRoDah; //Acts more like whirlwind sprint but who cares, FusRoDah is cooler!
			virtual void UpdateGame(float dt) override;

			void CameraFollow(GameObject* obj, float dt);
			void CreateWorld();

		void 	CollectBonus(Trigger* bonus);
			
			PlayerObject* playerObj = nullptr;


		protected:

			Vector3 cameraDistanceFromPlayer;
		};
	}
}