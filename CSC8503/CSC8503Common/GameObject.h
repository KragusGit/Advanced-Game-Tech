#pragma once
#include "Transform.h"
#include "CollisionVolume.h"

#include "PhysicsObject.h"
#include "RenderObject.h"

#include <vector>

using std::vector;

namespace NCL {
	enum Tag {
		None,
		Floor,
		_Button,
		_Trigger
	};
	namespace CSC8503 {

		class GameObject {
		public:
			GameObject(string name = "");
			~GameObject();

			void SetBoundingVolume(CollisionVolume* vol) {
				boundingVolume = vol;
			}

			const CollisionVolume* GetBoundingVolume() const {
				return boundingVolume;
			}

			bool IsActive() const {
				return isActive;
			}

			Transform& GetTransform() {
				return transform;
			}

			RenderObject* GetRenderObject() const {
				return renderObject;
			}

			PhysicsObject* GetPhysicsObject() const {
				return physicsObject;
			}

			void SetRenderObject(RenderObject* newObject) {
				renderObject = newObject;
			}

			void SetPhysicsObject(PhysicsObject* newObject) {
				physicsObject = newObject;
			}

			const string& GetName() const {
				return name;
			}

			void SetTag(Tag val) { tag = val; }

			const  Tag& GetTag() const {
				return tag;
			}

			void ToggleUseOfCustomGravity() {
				useCustomGravity = !useCustomGravity;
			}
			bool CanUseOfCustomGravity() { return useCustomGravity; };

			Vector3 GetCustomGravity() { return customGravity; };
			void SetCustomGravity(Vector3 gravity) {
				customGravity = gravity;
			}
			virtual void OnCollisionBegin(GameObject* otherObject) {
				//std::cout << "OnCollisionBegin event occured!\n";void PlayerObject::OnCollisionBegin(GameObject* colObj) {
				if (otherObject->GetTag() == Tag::Floor) {
					GetPhysicsObject()->UpdateExternalFriction(otherObject->GetPhysicsObject()->GetFriction());
				}
			}

			virtual void OnCollisionEnd(GameObject* otherObject) {
				//std::cout << "OnCollisionEnd event occured!\n";
			}

			static Vector3 Lerp(Vector3 startPos, Vector3 endPos, float normalisedPercentage);

			bool GetBroadphaseAABB(Vector3& outsize) const;

			void UpdateBroadphaseAABB();

			void SetWorldID(int newID) {
				worldID = newID;
			}

			int		GetWorldID() const {
				return worldID;
			}

			virtual void OnClick() {// When clicked 
			};

		protected:
			Transform			transform;

			CollisionVolume* boundingVolume;
			PhysicsObject* physicsObject;
			RenderObject* renderObject;

			bool	isActive;
			int		worldID;
			string	name;
			Tag	tag;
			bool useCustomGravity = false;


			Vector3 broadphaseAABB;
			Vector3 customGravity;
		};
	}
}

