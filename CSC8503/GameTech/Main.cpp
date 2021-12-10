#include "../../Common/Window.h"

#include "../CSC8503Common/StateMachine.h"
#include "../CSC8503Common/StateTransition.h"
#include "../CSC8503Common/State.h"

#include "../CSC8503Common/NavigationGrid.h"

#include "TutorialGame.h"
#include "../CSC8503Common/BehaviourAction.h"
#include "../CSC8503Common/BehaviourSequence.h"
#include "../CSC8503Common/BehaviourSelector.h"
#include "../CSC8503Common/PushdownState.h"
#include "../CSC8503Common/PushdownMachine.h"

using namespace NCL;
using namespace CSC8503;

void TestStateMachine()
{
	StateMachine* testMachine = new StateMachine();
	int data = 0;

	State* A = new State([&](float dt)->void
		{
			std::cout << "I'm in state A!" << std::endl;
			data++;
		});
	State* B = new State([&](float dt)->void
		{
			std::cout << "I'm in state B!" << std::endl;
			data--;
		});

	StateTransition* stateAB = new StateTransition(A, B, [&](void)->bool
		{
			return data > 10;
		});
	StateTransition* stateBA = new StateTransition(B, A, [&](void)->bool
		{
			return data < 0;
		});

	testMachine->AddState(A);
	testMachine->AddState(B);
	testMachine->AddTransition(stateAB);
	testMachine->AddTransition(stateBA);

	for (int i = 0; i < 100; ++i)
	{
		testMachine->Update(1.0f);
	}
}

vector<Vector3> testNodes;
void TestPathfinding()
{
	NavigationGrid grid("TestGrid1.txt");

	NavigationPath outPath;

	Vector3 startPos(80, 0, 10);
	Vector3 endPos(80, 0, 80);

	bool found = grid.FindPath(startPos, endPos, outPath);

	Vector3 pos;
	while (outPath.PopWaypoint(pos))
	{
		testNodes.push_back(pos);
	}
}
void DisplayPathfinding()
{
	for (int i = 1; i < testNodes.size(); ++i)
	{
		Vector3 a = testNodes[i - 1];
		Vector3 b = testNodes[i];

		Debug::DrawLine(a, b, Vector4(0, 1, 0, 1));
	}
}

void TestBehaviourTree()
{
	float behaviourTimer;
	float distanceToTarget;
	BehaviourAction* findKey = new BehaviourAction("Find Key", [&](float dt, BehaviourState state)->BehaviourState {
		if (state == Initialise)
		{
			std::cout << "Looking for a key!" << std::endl;
			behaviourTimer = rand() % 100;
			state = Ongoing;
		}
		else if (state == Ongoing)
		{
			behaviourTimer -= dt;
			if (behaviourTimer <= 0.0f)
			{
				std::cout << "Found a key!" << std::endl;
				return Success;
			}
		}
		return state; // will be ongoing until success;
		});
	BehaviourAction* goToRoom = new BehaviourAction("Go To Room", [&](float dt, BehaviourState state)->BehaviourState {
		if (state == Initialise)
		{
			std::cout << "Going to the loot room!" << std::endl;
			state = Ongoing;
		}
		else if (state == Ongoing)
		{
			distanceToTarget -= dt;
			if (distanceToTarget <= 0.0f)
			{
				std::cout << "Reached room!" << std::endl;
				return Success;
			}
		}
		return state; // will be ongoing until success;
		});
	BehaviourAction* openDoor = new BehaviourAction("Open Door", [&](float dt, BehaviourState state)->BehaviourState {
		if (state == Initialise)
		{
			std::cout << "Opening Door!" << std::endl;
			return Success;
		}
		return state; // will be ongoing until success;
		});

	BehaviourAction* lookForTreasure = new BehaviourAction("Look For Treasure", [&](float dt, BehaviourState state)->BehaviourState {
		if (state == Initialise)
		{
			std::cout << "Looking for treasure!" << std::endl;
			return Ongoing;
		}
		else if (state == Ongoing)
		{
			bool found = rand() % 2;
			if (found)
			{
				std::cout << "I found some treasure!" << std::endl;
				return Success;
			}
			std::cout << "No treasure in here..." << std::endl;
			return Failure;
		}
		return state; // will be ongoing until success;
		});
	BehaviourAction* lookForItems = new BehaviourAction("Look For Items", [&](float dt, BehaviourState state)->BehaviourState {
		if (state == Initialise)
		{
			std::cout << "Looking for items!" << std::endl;
			state = Ongoing;
		}
		else if (state == Ongoing)
		{
			bool found = rand() % 2;
			if (found)
			{
				std::cout << "I found some items!" << std::endl;
				return Success;
			}
			std::cout << "No items in here..." << std::endl;
			return Failure;
		}
		return state; // will be ongoing until success;
		});

	BehaviourSequence* sequence = new BehaviourSequence("Room Sequence");
	sequence->AddChild(findKey);
	sequence->AddChild(goToRoom);
	sequence->AddChild(openDoor);

	BehaviourSelector* selection = new BehaviourSelector("LootSelector");
	selection->AddChild(lookForTreasure);
	selection->AddChild(lookForItems);

	BehaviourSequence* rootSequence = new BehaviourSequence("Root Sequence");
	rootSequence->AddChild(sequence);
	rootSequence->AddChild(selection);

	for (int i = 0; i < 5; ++i)
	{
		rootSequence->Reset();
		behaviourTimer = 0.0f;
		distanceToTarget = rand() % 250;
		BehaviourState state = Ongoing;
		std::cout << "We're going on an adventure!" << std::endl;
		while (state == Ongoing)
		{
			state = rootSequence->Execute(1.0f); // fake dt
		}
		if (state == Success)
		{
			std::cout << "What a successful adventure!" << std::endl;
		}
		else if (state == Failure)
		{
			std::cout << "What a waste of time!" << std::endl;
		}
	}
	std::cout << "All done!" << std::endl;
}

class PauseScreen : public PushdownState
{
	PushdownResult OnUpdate(float dt, PushdownState** newState) override
	{
		if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::U))
		{
			return PushdownResult::Pop;
		}
		return PushdownResult::NoChange;
	}
};
class Game1 : public PushdownState
{
	PushdownResult OnUpdate(float dt, PushdownState** newState) override
	{
		if (Window::GetKeyboard()->KeyDown(KeyboardKeys::P))
		{
			g->SetPaused(true);
			g->UpdateGame(dt);
			*newState = new PauseScreen();
			return PushdownResult::Push;
		}
		Debug::Print("PRESS ESCAPE TO RETURN TO MENU", Vector2(5, 80));
		if (Window::GetKeyboard()->KeyDown(KeyboardKeys::ESCAPE))
		{
			return PushdownResult::Pop;
		}
		g->UpdateGame(dt);
		return PushdownResult::NoChange;
	}
	void OnAwake() override
	{
		g->SetPaused(false);
	}
protected:
	TutorialGame* g = new TutorialGame(1);
};
class Game2 : public PushdownState
{
	PushdownResult OnUpdate(float dt, PushdownState** newState) override
	{
		if (Window::GetKeyboard()->KeyDown(KeyboardKeys::P))
		{
			g->SetPaused(true);
			g->UpdateGame(dt);
			*newState = new PauseScreen();
			return PushdownResult::Push;
		}
		Debug::Print("PRESS ESCAPE TO RETURN TO MENU", Vector2(5, 80));
		if (Window::GetKeyboard()->KeyDown(KeyboardKeys::ESCAPE))
		{
			return PushdownResult::Pop;
		}
		g->UpdateGame(dt);
		return PushdownResult::NoChange;
	}
	void OnAwake() override
	{
		g->SetPaused(false);
	}
protected:
	TutorialGame* g = new TutorialGame(2);
};
class IntroScreen : public PushdownState
{
	PushdownResult OnUpdate(float dt, PushdownState** newState) override
	{
		Debug::Print("CSC8503 Advanced Game Tech", Vector2(25, 40));
		Debug::Print("LAUNCH LEVEL 1: (PRESS 1)", Vector2(25, 50));
		if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::NUM1))
		{
			*newState = new Game1();
			return PushdownResult::Push;
		}
		Debug::Print("LAUNCH LEVEL 2: (PRESS 2)", Vector2(25, 60));
		if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::NUM2))
		{
			*newState = new Game2();
			return PushdownResult::Push;
		}
		Debug::Print("PRESS ESCAPE TO EXIT", Vector2(30, 70));
		if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::ESCAPE))
		{
			return PushdownResult::Pop;
		}
		g->UpdateGame(dt);
		return PushdownResult::NoChange;
	}
protected:
	TutorialGame* g = new TutorialGame(0);
};

/*

The main function should look pretty familar to you!
We make a window, and then go into a while loop that repeatedly
runs our 'game' until we press escape. Instead of making a 'renderer'
and updating it, we instead make a whole game, and repeatedly update that,
instead. 

This time, we've added some extra functionality to the window class - we can
hide or show the 

*/
int main() {
	Window*w = Window::CreateGameWindow("CSC8503 Game technology!", 1600, 900);
	
	if (!w->HasInitialised()) {
		return -1;
	}	
	srand(time(0));
	w->ShowOSPointer(false);
	w->LockMouseToWindow(true);

	w->GetTimer()->GetTimeDeltaSeconds(); //Clear the timer so we don't get a larget first dt!

	//TestStateMachine();
	//TestBehaviourTree();

	//TestPathfinding();
	PushdownMachine machine(new IntroScreen());

	while (w->UpdateWindow()) 
	{		
		//DisplayPathfinding();

		float dt = w->GetTimer()->GetTimeDeltaSeconds();
		
		if (dt > 0.1f) {
			std::cout << "Skipping large time delta" << std::endl;
			continue; //must have hit a breakpoint or something to have a 1 second frame time!
		}
		if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::PRIOR)) {
			w->ShowConsole(true);
		}
		if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::NEXT)) {
			w->ShowConsole(false);
		}

		if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::T)) {
			w->SetWindowPosition(0, 0);
		}
		if (!machine.Update(dt))
		{
			return -1;
		}

		//w->SetTitle("Gametech frame time:" + std::to_string(1000.0f * dt));
		w->SetTitle("Gametech frame rate:" + std::to_string(1.0f / dt));
	}
	Window::DestroyGameWindow();
}