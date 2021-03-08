#include "AIAgent.h"
#include <math.h>
#include "Trigger.h"

using namespace NCL;
using namespace CSC8503;

NCL::CSC8503::AIAgent::~AIAgent() {
	delete bonusBehaviour;
	delete mainBehaviour;
}

void AIAgent::Update(float dt) {

	if (move)
		mainBehaviour->Execute(dt);
	/*if (pathNodes.size() > 0 && move)
		FollowPath(dt);*/
}

void AIAgent::SetPathNodes(vector<Vector3> path) {
	pathNodes = path;
	currentPathIndex = 0;

}

void AIAgent::Goto(Vector3 point, float dt) {
	point.y = transform.GetPosition().y;
	LookAt(point, dt);
	Move((point - transform.GetPosition()).Normalised(), dt);
}

void AIAgent::FollowPath(float dt) {

	if (currentPathIndex == pathNodes.size()) {	//Reached 
		return;
	}

	Vector3 currentNode = pathNodes[currentPathIndex];
	currentNode.y = transform.GetPosition().y;

	if ((transform.GetPosition() - currentNode).LengthSquared() < 0.2) {
		transform.SetPosition(currentNode);
		currentPathIndex++;
		if (currentPathIndex == pathNodes.size()) {	//Reached 
			return;
		}
		currentNode = pathNodes[currentPathIndex];
		currentNode.y = transform.GetPosition().y;
	}
	//Debug::DrawLine(transform.GetPosition(), (transform.GetOrientation() * Vector3(0, 0, 1)) * 2);
	LookAt(currentNode, dt);
	Move((currentNode - transform.GetPosition()).Normalised(), dt);
	Debug::FlushRenderables(dt);
}

void AIAgent::LookAt(Vector3 node, float dt) {
	Vector3 dir = (node - transform.GetPosition()).Normalised();
	dir.y = 0;

	//Raycast and check for obstacles here and set steer angle....
	//TODO
	//

	if (steerOffsetAngle != 0) {
		dir.x = dir.x * std::cos(steerOffsetAngle) + dir.z * std::sin(steerOffsetAngle);
		dir.z = -dir.x * std::sin(steerOffsetAngle) + dir.z * std::cos(steerOffsetAngle);
	}
	Vector3 rotAxis = Vector3::Cross(Vector3(0, 0, 1), dir);
	float dot = Vector3::Dot(Vector3(0, 0, 1), dir);

	Quaternion lookat;
	lookat.x = rotAxis.x;
	lookat.y = rotAxis.y;
	lookat.z = rotAxis.z;
	lookat.w = dot + 1;
	lookat.Normalise();

	transform.SetOrientation(Quaternion::Slerp(transform.GetOrientation(), lookat, 5));

}

void AIAgent::Move(Vector3 dir, float dt) {
	//transform.SetPosition(transform.GetPosition() + (dir * dt * 10));
	GetPhysicsObject()->AddForce(transform.GetOrientation() * (Vector3(0, 0, 1)) * (speed * dt));
}

