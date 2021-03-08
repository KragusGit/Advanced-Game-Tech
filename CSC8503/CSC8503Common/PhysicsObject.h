#pragma once
#include "../../Common/Vector3.h"
#include "../../Common/Matrix3.h"

#include <vector>
#include"Debug.h"

using namespace NCL::Maths;

namespace NCL {
	class CollisionVolume;

	namespace CSC8503 {
		class Transform;


		class PhysicsObject {
		public:
			PhysicsObject(Transform* parentTransform, const CollisionVolume* parentVolume);
			~PhysicsObject();

			void SetFriction(float val) { friction = val; }
			float GetFriction() const { return friction; }
			void SetElasticity(float val) { elasticity = val; }
			float GetElasticity() const { return elasticity; }

			Vector3 GetLinearVelocity() const {
				return linearVelocity;
			}

			Vector3 GetAngularVelocity() const {
				return angularVelocity;
			}

			Vector3 GetTorque() const {
				return torque;
			}

			Vector3 GetForce() const {
				return force;
			}

			void SetInverseMass(float invMass) {
				inverseMass = invMass;
			}

			float GetInverseMass() const {
				return inverseMass;
			}

			float GetAverageMovement() {
				float avg = 0;
				for (int i = 0; i < pastDisplacementMagnitudes.size(); i++) {
					avg += pastDisplacementMagnitudes[i];
				}
				return (avg > 0.0f) ? (avg / pastDisplacementMagnitudes.size()) : 0;
			}

			void RegisterSquareDisplacemet(float displacement) {
				if (pastDisplacementMagnitudes.size() > 10)
					pastDisplacementMagnitudes.erase(pastDisplacementMagnitudes.begin());
				pastDisplacementMagnitudes.push_back(displacement);
			}

			void UpdateSleep() {
				isSleeping = GetAverageMovement() < 1.0f;
			}

			bool IsSleeping() {
				return  isSleeping;
			}

			float GetTotalFriction() const {
				return totalFriction;
			}



			void UpdateExternalFriction(float fr) {
				externalFriction = fr;
				RecalculateTotalFriction();
			}

			void RecalculateTotalFriction() {
				totalFriction = (externalFriction > 0) ? (externalFriction + friction) : 0;
			}

			void ApplyAngularImpulse(const Vector3& force);
			void ApplyLinearImpulse(const Vector3& force);

			void AddForce(const Vector3& force);

			void AddForceAtPosition(const Vector3& force, const Vector3& position);

			void AddTorque(const Vector3& torque);


			void ClearForces();

			void SetLinearVelocity(const Vector3& v) {
				linearVelocity = v;
			}

			void SetAngularVelocity(const Vector3& v) {
				angularVelocity = v;
			}

			void InitCubeInertia();
			void InitSphereInertia();

			void UpdateInertiaTensor();

			Matrix3 GetInertiaTensor() const {
				return inverseInteriaTensor;
			}

		protected:
			const CollisionVolume* volume;
			Transform* transform;
			float inverseMass;
			float elasticity;
			float friction;
			float totalFriction;
			bool isSleeping;
			std::vector<float> pastDisplacementMagnitudes; //to check for sleep

			float externalFriction;

			//linear stuff
			Vector3 linearVelocity;
			Vector3 force;


			//angular stuff
			Vector3 angularVelocity;
			Vector3 torque;
			Vector3 inverseInertia;
			Matrix3 inverseInteriaTensor;
		};
	}
}

