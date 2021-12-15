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
	grid = NavigationGrid("LevelTwoMaze.txt");
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
float StateGameObject::DistanceFromObject(GameObject* otherObject)
{
	// May wanna change this function to return path length not just direct line length

	Vector3 pos = this->GetTransform().GetPosition();
	Vector3 otherObjectPos = otherObject->GetTransform().GetPosition();
	return abs((otherObjectPos - pos).Length());
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
			this->Hunt(dt, 1);
		});
	State* wander = new State([&](float dt)->void
		{
			this->Wander(dt);
		});
	State* goForPowerUp = new State([&](float dt)->void
		{
			this->Hunt(dt, 2);
		});

	stateMachine->AddState(huntPlayer);
	stateMachine->AddState(wander);
	stateMachine->AddState(goForPowerUp);

	stateMachine->AddTransition(new StateTransition(huntPlayer, goForPowerUp, [&](void)->bool
		{
			for (PowerUpObject* p : powerUps)
			{
				if (p->GetAbility() == PowerUp::SPEED_UP && DistanceFromObject(playerBall) > DistanceFromObject(p))
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
				if (p->GetAbility() == PowerUp::SPEED_UP && DistanceFromObject(playerBall) > DistanceFromObject(p))
				{
					return false;
				}
			}
			return true;
		}));
	stateMachine->AddTransition(new StateTransition(huntPlayer, wander, [&](void)->bool
		{
			return DistanceFromObject(playerBall) > 150.0f;
		}));
	stateMachine->AddTransition(new StateTransition(wander, huntPlayer, [&](void)->bool
		{
			return DistanceFromObject(playerBall) < 150.0f;
		}));
	stateMachine->AddTransition(new StateTransition(goForPowerUp, wander, [&](void)->bool
		{
			for (PowerUpObject* p : powerUps)
			{
				if (DistanceFromObject(p) > 150.0f)
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
				if (DistanceFromObject(p) > 150.0f)
				{
					return false;
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
	Vector3 startPos;
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
			Vector3 powerUpPos = p->GetTransform().GetPosition();
			if (abs((this->GetTransform().GetPosition() - powerUpPos).Length()) < distance)
			{// if this power up distance is less than the last
				endPos = powerUpPos;
			}
		}
	}

	startPos.y = 0;
	endPos.y = 0;

	NavigationPath outPath;
	vector<Vector3> nodes;

	bool found = grid.FindPath(startPos, endPos, outPath);

	Vector3 pos;
	while (outPath.PopWaypoint(pos))
	{
		nodes.push_back(pos);
	}

	Vector3 a = nodes[0];
	Vector3 b = nodes[1];

	Debug::DrawLine(a, b, Vector4(0, 1, 0, 1));
}
void StateGameObject::Wander(float dt) {

}