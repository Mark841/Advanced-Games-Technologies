#include "StateGameObject.h"
#include "../CSC8503Common/StateTransition.h"
#include "../CSC8503Common/StateMachine.h"
#include "../CSC8503Common/State.h"
#include "../CSC8503Common/Debug.cpp"

using namespace NCL;
using namespace CSC8503;

StateGameObject::StateGameObject(ObjectMovement movement, int layer, string name) : GameObject(layer, name)
{
	counter = 0.0f;
	forceMultiplier = 1.0f; 
	wanderDestination = Vector3(-1, -1, -1);
	switch (movement)
	{
	case(ObjectMovement::MOVING):
		InitMoving();
		break;
	case(ObjectMovement::ROTATING):
		InitRotating();
		break;
	case(ObjectMovement::SPIN):
		InitSpinning();
		break;
	case(ObjectMovement::AI):
		InitAI();
		break;
	}
}

StateGameObject::~StateGameObject()
{
	delete stateMachine;
}

void StateGameObject::Update(float dt)
{
	stateMachine->Update(dt);
}
void StateGameObject::OnCollisionBegin(GameObject* otherObject)
{
	if (otherObject->GetName() == "PLAYER BALL")
	{
		collisionWithPlayerBall = otherObject;
	}
}
void StateGameObject::OnCollisionEnd(GameObject* otherObject)
{
	if (otherObject->GetName() == "PLAYER BALL")
	{
		collisionWithPlayerBall = nullptr;
	}
}
int StateGameObject::DistanceToObject(GameObject* otherObject)
{
	Vector3 pos = this->GetTransform().GetPosition();
	Vector3 otherObjectPos = otherObject->GetTransform().GetPosition();
	Pathfind(pos, otherObjectPos);
	return nodes.size();
}

void StateGameObject::InitMoving()
{
	counter = 0.5f;
	stateMachine = new StateMachine();

	State* goLeft = new State([&](float dt)->void
		{
			this->MoveLeft(dt);
		});
	State* goRight = new State([&](float dt)->void
		{
			this->MoveRight(dt);
		});

	stateMachine->AddState(goLeft);
	stateMachine->AddState(goRight);

	stateMachine->AddTransition(new StateTransition(goLeft, goRight, [&](void)->bool
		{
			return this->counter > 1.0f;
		}));
	stateMachine->AddTransition(new StateTransition(goRight, goLeft, [&](void)->bool
		{
			return this->counter < 0.0f;
		}));
}
void StateGameObject::InitRotating()
{
	stateMachine = new StateMachine();

	State* antiClockwise = new State([&](float dt)->void
		{
			this->RotateAnticlockwise(dt);
		});
	State* clockwise = new State([&](float dt)->void
		{
			this->RotateClockwise(dt);
		});

	stateMachine->AddState(antiClockwise);
	stateMachine->AddState(clockwise);

	stateMachine->AddTransition(new StateTransition(antiClockwise, clockwise, [&](void)->bool
		{
			return this->counter > 5.0f;
		}));
	stateMachine->AddTransition(new StateTransition(clockwise, antiClockwise, [&](void)->bool
		{
			return this->counter < 0.0f;
		}));
}
void StateGameObject::InitSpinning()
{
	stateMachine = new StateMachine();

	State* antiClockwise = new State([&](float dt)->void
		{
			this->SpinAnticlockwise(dt);
		});
	State* clockwise = new State([&](float dt)->void
		{
			this->SpinClockwise(dt);
		});
	State* collected = new State([&](float dt)->void
		{
			if (collisionWithPlayerBall == nullptr)
			{
				return;
			}
			this->SetActive(false);
		});
	State* inactive = new State([&](float dt)->void
		{
			this->Inactive();
			this->SetActive(false);
		});

	stateMachine->AddState(antiClockwise);
	stateMachine->AddState(clockwise);
	stateMachine->AddState(collected);
	stateMachine->AddState(inactive);

	stateMachine->AddTransition(new StateTransition(antiClockwise, clockwise, [&](void)->bool
		{
			return this->counter > 10.0f;
		}));
	stateMachine->AddTransition(new StateTransition(clockwise, antiClockwise, [&](void)->bool
		{
			return this->counter < 0.0f;
		}));
	stateMachine->AddTransition(new StateTransition(antiClockwise, collected, [&](void)->bool
		{
			return !(collisionWithPlayerBall == nullptr);
		}));
	stateMachine->AddTransition(new StateTransition(clockwise, collected, [&](void)->bool
		{
			return !(collisionWithPlayerBall == nullptr);
		}));
	stateMachine->AddTransition(new StateTransition(collected, inactive, [&](void)->bool
		{
			return (collisionWithPlayerBall == nullptr);
		}));
}
void StateGameObject::InitAI()
{
	stateMachine = new StateMachine();

	State* huntPlayer = new State([&](float dt)->void
		{
			this->SetState(States::HUNTING_PLAYER);
			this->Hunt(dt, 1);
		});
	State* wander = new State([&](float dt)->void
		{
			this->SetState(States::WANDERING);
			if (wanderDestination == Vector3(-1,-1,-1))
			{
				GenerateNewDestination();
			}
			this->Wander(dt);
		});
	State* goForPowerUp = new State([&](float dt)->void
		{
			this->SetState(States::HUNTING_POWER_UP);
			this->Hunt(dt, 2);
		});

	stateMachine->AddState(huntPlayer);
	stateMachine->AddState(wander);
	stateMachine->AddState(goForPowerUp);

	stateMachine->AddTransition(new StateTransition(huntPlayer, goForPowerUp, [&](void)->bool
		{
			for (PowerUpObject* p : powerUps)
			{
				if (p->GetAbility() != PowerUp::SPEED_UP)
				{
					continue;
				}
				int distToPlayer = DistanceToObject(playerBall);
				if (DistanceToObject(p) < distToPlayer || distToPlayer == 0)
				{
					return true;
				}
			}
			return false;
		}));
	stateMachine->AddTransition(new StateTransition(goForPowerUp, huntPlayer, [&](void)->bool
		{
			for (PowerUpObject* p : powerUps)
			{
				if (p->GetAbility() != PowerUp::SPEED_UP)
				{
					continue;
				}
				int distToPlayer = DistanceToObject(playerBall);
				if (DistanceToObject(p) < distToPlayer && distToPlayer != 0)
				{
					return false;
				}
			}
			return true;
		}));
	stateMachine->AddTransition(new StateTransition(huntPlayer, wander, [&](void)->bool
		{
			int distToPlayer = DistanceToObject(playerBall);
			if (distToPlayer > 20 || distToPlayer == 0)
			{
				wanderDestination = Vector3(-1, -1, -1);
				return true;
			}
			return false;
		}));
	stateMachine->AddTransition(new StateTransition(wander, huntPlayer, [&](void)->bool
		{
			int distToPlayer = DistanceToObject(playerBall);
			if (distToPlayer < 20 && distToPlayer > 0)
			{
				wanderDestination = Vector3(-1, -1, -1);
				return true;
			}
			return false;
		}));
	stateMachine->AddTransition(new StateTransition(goForPowerUp, wander, [&](void)->bool
		{
			for (PowerUpObject* p : powerUps)
			{
				if (p->GetAbility() != PowerUp::SPEED_UP)
				{
					continue;
				}
				if (DistanceToObject(p) > 30)
				{
					return true;
				}
			}
			return false;
		}));
	stateMachine->AddTransition(new StateTransition(wander, goForPowerUp, [&](void)->bool
		{
			for (PowerUpObject* p : powerUps)
			{
				if (p->GetAbility() != PowerUp::SPEED_UP)
				{
					continue;
				}
				if (DistanceToObject(p) < 30)
				{
					wanderDestination = Vector3(-1, -1, -1);
					return true;
				}
			}
			return false;
		}));
}

void StateGameObject::MoveLeft(float dt)
{
	GetPhysicsObject()->SetLinearVelocity(GetPhysicsObject()->GetLinearVelocity() + Vector3(-1, 0, 0));
	this->SetState(States::MOVING_LEFT);
	counter += dt;
}
void StateGameObject::MoveRight(float dt)
{
	GetPhysicsObject()->SetLinearVelocity(GetPhysicsObject()->GetLinearVelocity() + Vector3(1, 0, 0));
	this->SetState(States::MOVING_RIGHT);
	counter -= dt;
}
void StateGameObject::RotateAnticlockwise(float dt)
{
	GetPhysicsObject()->SetAngularVelocity(GetPhysicsObject()->GetAngularVelocity() + Vector3(0, 0.05f, 0));
	this->SetState(States::ROTATING_ANTICLOCKWISE);
	counter += dt;
}
void StateGameObject::RotateClockwise(float dt)
{
	GetPhysicsObject()->SetAngularVelocity(GetPhysicsObject()->GetAngularVelocity() + Vector3(0, -0.05f, 0));
	this->SetState(States::ROTATING_CLOCKWISE);
	counter -= dt;
}
void StateGameObject::SpinAnticlockwise(float dt)
{
	GetPhysicsObject()->SetAngularVelocity({ 0,1,0 });
	this->SetState(States::SPINNING_ANTICLOCKWISE);
	counter += dt;
}
void StateGameObject::SpinClockwise(float dt)
{
	GetPhysicsObject()->SetAngularVelocity({ 0,-1,0 });
	this->SetState(States::SPINNING_CLOCKWISE);
	counter -= dt;
}
void StateGameObject::Inactive()
{
	this->SetState(States::INACTIVE);
}

void StateGameObject::Hunt(float dt, int huntingTarget)
{
	Vector3 startPos = this->GetTransform().GetPosition();
	Vector3 endPos;
	if (huntingTarget == 1)
	{ // Player
		endPos = playerBall->GetTransform().GetPosition();
	}
	else if (huntingTarget == 2)
	{ // PowerUp
		float distance = FLT_MAX;
		for (PowerUpObject* p : powerUps)
		{
			if (p->GetAbility() == PowerUp::SPEED_UP)
			{
				Vector3 powerUpPos = p->GetTransform().GetPosition();
				if (abs((this->GetTransform().GetPosition() - powerUpPos).Length()) < distance)
				{// if this power up distance is less than the last
					endPos = powerUpPos;
				}
			}
		}
	}

	endPos.y = 5;

	Vector3 direction;
	bool path = Pathfind(startPos, endPos);
	if (path && nodes.size() > 2)
	{
		direction = (abs((nodes[1] - startPos).Length() > 5.0f)) ? nodes[1] - startPos : nodes[2] - startPos;

		////////////////////////////////////////////////////////////////////////
		Debug::DrawLine(startPos, nodes[1], Vector4(0, 1, 0, 1));
		for (int i = 2; i < nodes.size(); ++i)
		{
			Vector3 a = nodes[i - 1];
			Vector3 b = nodes[i];

			Debug::DrawLine(a, b, Vector4(0, 1, 0, 1));
		}
		////////////////////////////////////////////////////////////////////////
	}
	else if (path)
	{
		direction = (nodes.size() > 1) ? nodes[1] - startPos : nodes[0] - startPos;
	}
	else
	{
		direction = (endPos - startPos) * 0.1f;
	}
	direction *= forceMultiplier;
	GetPhysicsObject()->ApplyLinearImpulse(direction * 0.001f);
}
void StateGameObject::Wander(float dt) 
{
	Vector3 startPos = this->GetTransform().GetPosition();
	bool path = Pathfind(startPos, wanderDestination);
	if (!path)
	{
		GenerateNewDestination();
	}
	if (path && nodes.size() > 2)
	{
		Vector3 direction = (abs((nodes[1] - startPos).Length() > 5.0f)) ? nodes[1] - startPos : nodes[2] - startPos;
		direction *= forceMultiplier;
		GetPhysicsObject()->ApplyLinearImpulse(direction * 0.001f);

		////////////////////////////////////////////////////////////////////////
		Debug::DrawLine(startPos, nodes[1], Vector4(0, 1, 0, 1));
		for (int i = 2; i < nodes.size(); ++i)
		{
			Vector3 a = nodes[i - 1];
			Vector3 b = nodes[i];

			Debug::DrawLine(a, b, Vector4(0, 1, 0, 1));
		}
		////////////////////////////////////////////////////////////////////////
	}
	if (path && nodes.size() < 2)
	{ // Get a new path
		wanderDestination = Vector3(-1, -1, -1);
	}
}

bool StateGameObject::Pathfind(const Vector3& startPos, const Vector3& endPos)
{
	nodes.clear();
	NavigationPath outPath;

	NavigationGrid grid("LevelTwoMaze.txt");
	bool found = grid.FindPath(startPos, endPos, outPath);

	Vector3 pos;
	while (outPath.PopWaypoint(pos))
	{
		nodes.push_back(pos);
	}
	return found;
}
void StateGameObject::GenerateNewDestination()
{
	int x = ((rand() % 15) + 1) * 25;
	int z = ((rand() % 15) + 1) * 25;
	wanderDestination = Vector3(x, 5, z);
}