#pragma once
#include "../CSC8503Common/GameWorld.h"
#include <set>

namespace NCL {


	namespace CSC8503 {
		class PhysicsSystem	{
		public:
			PhysicsSystem(GameWorld& g);
			~PhysicsSystem();

			void Clear();

			void Update(float dt);

			void UseGravity(bool state) {
				applyGravity = state;
			}

			void SetGlobalDamping(float d) {
				globalDamping = d;
			}
			void SetLocalDamping(float d) {
				localDamping = d;
			}


			Vector3 GetGravity();

			void SetGravity(const Vector3& g);
			bool usePenaltyResolution = false;
		protected:
			void BasicCollisionDetection();
			void BroadPhase();
			void NarrowPhase();

			void ClearForces();

			void ResolveSpringCollision(GameObject& a, GameObject& b, CollisionDetection::ContactPoint& p) const;

			void IntegrateAccel(float dt);
			void IntegrateVelocity(float dt);

			void UpdateConstraints(float dt);

			void UpdateCollisionList();
			void UpdateObjectAABBs();

			void ImpulseResolveCollision(GameObject& a , GameObject&b, CollisionDetection::ContactPoint& p) const;

			GameWorld& gameWorld;

			bool	applyGravity;
			Vector3 gravity;
			float	dTOffset;
			float	globalDamping;
			float localDamping = 0.4f;

			std::set<CollisionDetection::CollisionInfo> allCollisions;
			std::set < CollisionDetection::CollisionInfo > broadphaseCollisions;

			bool useBroadPhase		= true;
			int numCollisionFrames	= 5;
			int checkForSleepInFrames = 10;
			int elapsedFramesForSleepCheck = 0;
		
		};
	}
}

