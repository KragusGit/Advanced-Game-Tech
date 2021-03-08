#pragma once
#include "Level.h"
#include "PlayerObject.h"

namespace NCL {
	namespace CSC8503 {
		class LevelOne :public Level {
		public:
			LevelOne(GameTechRenderer* renderer);
			~LevelOne();

			PlayerObject* SpawnPlayer(const Vector3& position);

			virtual void UpdateGame(float dt) override;

			void CameraFollow(GameObject* obj, float dt);
			void CreateWorld();
			void AddFinalObstacle();
			void AddStairs(Button* toggle);
			PlayerObject* playerObj = nullptr;
			OscillatingObject* lift = nullptr;
			vector<OscillatingObject*> stairs;
			vector<OscillatingObject*> finalObstacles;

		protected:
			bool playerOnTrigger1 = false;
			Vector3 cameraDistanceFromPlayer;
			GameObject* pendulum1;
			GameObject* pendulum2;
			GameObject* pendulum3;

		};
	}
}