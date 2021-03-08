#pragma once
#include <vector>
#include "../CSC8503Common/GameObject.h"
#include "GameTechRenderer.h"
#include "../CSC8503Common/PhysicsSystem.h"
#include "OscillatingObject.h"
#include "Button.h"
#include "Trigger.h"
#include "../CSC8503Common/NavigationGrid.h"
namespace NCL {
	namespace CSC8503 {
		class Level {
		public:
			Level(GameTechRenderer* renderer);
			~Level();
			virtual void UpdateGame(float dt) = 0; //Update Renderer and physics when overriding, also flush debug if using debug print etc.
			bool GameWon() { return gameWon; };
			bool GameLost() { return gameLost; };
			GameTechRenderer* renderer;
			PhysicsSystem* physics;
			GameWorld* world;
		protected:

			GameObject* AddOBBFloorToWorld(const Vector3& position, const Vector3& rotation, const Vector3& size, float friction);

			GameObject* AddFloorToWorld(const Vector3& position, const Vector3& rotation, const Vector3& size, float friction);
			GameObject* AddSphereToWorld(const Vector3& position, float radius, float inverseMass = 10.0f);
			Button* AddButtonToWorld(const Vector3& position, const Vector3& rotation, Vector3 dimensions, float inverseMass = 0);
			Trigger* AddTriggerToWorld(const Vector3& position, const Vector3& rotation, Vector3 dimensions, float inverseMass = 0, bool drawMesh = false);
			GameObject* AddOBBCubeToWorld(const Vector3& position, const Vector3& rotation, Vector3 dimensions, float inverseMass);
			GameObject* AddAABBCubeToWorld(const Vector3& position, const Vector3& rotation, Vector3 dimensions, float inverseMass = 10.0f);
			GameObject* AddCapsuleToWorld(const Vector3& position, float halfHeight, float radius, float inverseMass = 10.0f);
			vector<GameObject*> AddCubesFromFile(const std::string& filename, VolumeType volType = VolumeType::AABB);
		
			OscillatingObject* SpawnOscillatingObject(const Vector3& position, const Vector3& size, vector<Vector3>positions, bool startOnSpawn, float timeToTravelOneLeg, float timeToWait, OGLMesh* mesh, float inverseMass = 0);

			void InitialiseAssets();
			void InitBasicSystems();
			void ReadObjCoordinates(std::ifstream& file, vector<Vector3>& element);
			void ToggleGravity();
			void InverseGravity();
			void SetGlobalGravity(Vector3 gravity);

			OGLMesh* capsuleMesh = nullptr;
			OGLMesh* cubeMesh = nullptr;
			OGLMesh* sphereMesh = nullptr;
			OGLTexture* basicTex = nullptr;
			OGLShader* basicShader = nullptr;;

			Vector3 globalGravity;
			void SubscribeOnCompleted(std::function < void()> func) { OnCompleted.push_back(func); };
			void SubscribeOnFailed(std::function < void()> func) { OnFailed.push_back(func); };
	
			
			bool gameWon = false;
			bool gameLost = false;

			virtual	void OnGameFailed() {
				gameLost = true;
				for (int i = 0; i < OnFailed.size(); i++) {
					if (OnFailed[i] != nullptr)
						OnFailed[i]();
				};
			};

			virtual void OnGameCompleted() {
				gameWon = true;
				for (int i = 0; i < OnCompleted.size(); i++) {
					if (OnCompleted[i] != nullptr)
						OnCompleted[i]();
				};
			}

			
		private:
			bool useGravity = true;
			vector < std::function < void() >> OnCompleted;
			vector < std::function < void() >> OnFailed;

		};
	}
}