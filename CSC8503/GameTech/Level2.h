#pragma once
#include "Level.h"
#include "PlayerObject.h"
#include "AIAgent.h"

namespace NCL {
	namespace CSC8503 {
		class LevelTwo :public Level {
		public:
			LevelTwo(GameTechRenderer* renderer);
			bool CheckForBonus(Vector3 actorPos, Trigger*& bonus);
			~LevelTwo();

			PlayerObject* SpawnPlayer(const Vector3& position);

			AIAgent* SpawnAI(const Vector3& position);

			virtual void UpdateGame(float dt) override;

			void CameraFollow(GameObject* obj, float dt);
			void CreateWorld();

			PlayerObject* playerObj = nullptr;
			AIAgent* AI;
			void LoadNavigationGrid(const std::string& filename);

			vector<GameObject*> AddCubesFromGrid(NavigationGrid* grid, VolumeType volType);

			Vector3 GridposToWorldPos(Vector3 gridPos);

			NavigationGrid* grid;


		protected:
			Vector3 cameraDistanceFromPlayer;
			Vector3 playerStartPosition;
			Vector3 gridStartPosition = Vector3(0, 0, 0);
			float nodeSize = 10;
			Vector2 NodeIndexToGridPos(int nodex, int nodey);
			Vector3 NodeIndexToWorldPos(Vector2 pos);
			Vector3 NodeIndexToWorldPos(int nodex, int nodey);
			float bonusCheckTimer = 0.5f;
			float elapsedBonusTimer = 0;
			float initialCountdown = 5;
			vector<Trigger*> bonuses;
			Vector3 raceStartPos = Vector3(10, 0, 10);
			Vector3 raceEndPos = Vector3(80, 0, 110);
		};
	}
}