#pragma once
#include "Level.h"
#include "PlayerObject.h"


namespace NCL {
	namespace CSC8503 {
		class LevelFive :public Level {
		public:
			LevelFive(GameTechRenderer* renderer) ;
			~LevelFive();

			PlayerObject* SpawnPlayer(const Vector3& position);
			
			virtual void UpdateGame(float dt) override;

			void CameraFollow(GameObject* obj, float dt);
			void CreateWorld();

	
			
			PlayerObject* playerObj = nullptr;


		protected:

			Vector3 cameraDistanceFromPlayer;
		};
	}
}