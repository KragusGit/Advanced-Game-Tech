#include "../../Common/Window.h"

//#include "../CSC8503Common/StateMachine.h"
//#include "../CSC8503Common/StateTransition.h"
//#include "../CSC8503Common/State.h"

#include "../CSC8503Common/NavigationGrid.h"

#include "TutorialGame.h"
#include "../CSC8503Common/PushdownMachine.h"
#include "../CSC8503Common/PushdownState.h"
#include "../CSC8503Common/BehaviourAction.h"
#include "../CSC8503Common/BehaviourSequence.h"
#include "../CSC8503Common/BehaviourSelector.h"
#include "MainMenu.h"
#include "Level1.h"
#include "Level2.h"
#include "Level3.h"
#include "Level4.h"
#include "Level5.h"
#include <Map>


using namespace NCL;
using namespace CSC8503;

vector < Vector3 > testNodes;
int lastFinishedLevel;
int endscreenResult = -1;

GameWorld* world;
GameTechRenderer* renderer;

class PauseScreen : public PushdownState {
	Menu* pauseMenu;
	vector<string> options;
	PushdownResult OnUpdate(float dt, PushdownState** newState) override {
		if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::P)) {
			return PushdownResult::Pop;
		}

		pauseMenu->Update(dt);

		if (pauseMenu->OptionFinalized()) {
			switch (pauseMenu->GetCurrentSelectedOptionIndex()) {
			case 0:return PushdownResult::Pop; break;
			default:break;
			}
		}
		return PushdownResult::NoChange;
	}
	void OnSleep() override { options.clear(); delete pauseMenu; }
	void OnAwake() override {
		options.push_back(" Press P or Return to Unpause!");
		pauseMenu = new Menu(options, renderer, "Pause Menu!");
	}
public:
};


class EndScreen : public PushdownState {
	Menu* endMenu;
	vector<string> options;
	bool win;

	PushdownResult OnUpdate(float dt, PushdownState** newState) override {

		endMenu->Update(dt);

		if (endMenu->OptionFinalized()) {
			if (win) {
				switch (endMenu->GetCurrentSelectedOptionIndex()) {
				case 0:selectedResult = Result::NextLevel; break;
				case 1:selectedResult = Result::Retry; break;
				case 2:selectedResult = Result::Exit; break;
				default:
					selectedResult = Result::Exit; break;
				}
			}
			else {
				switch (endMenu->GetCurrentSelectedOptionIndex()) {
				case 0:selectedResult = Result::Retry; break;
				case 1:selectedResult = Result::Exit; break;
				default:
					selectedResult = Result::Exit; break;
				}

			}
			switch (selectedResult) {
			case Result::None:endscreenResult = -1; break;
			case Result::Retry:endscreenResult = 0; break;
			case Result::NextLevel:endscreenResult = 1; break;
			case Result::Exit:endscreenResult = 2; break;
			default:endscreenResult = -1; break;
			}
			return PushdownResult::Pop;
		}

		return PushdownResult::NoChange;
	}

	void OnSleep() override { delete endMenu; };
	void OnAwake() override {
		endMenu = new Menu(options, renderer, win ? "Well Done!" : "Game Over!");
	}
public:
	enum Result {
		None,//-1
		Retry,//0
		NextLevel,//1
		Exit//2
	};
	Result selectedResult = Result::None;
	EndScreen(bool win) {
		if (!win) {
			options.push_back("Retry");
			options.push_back("Exit");
		}
		else {
			options.push_back("NextLevel");
			options.push_back("Retry");
			options.push_back("Exit");
		}

		this->win = win;
	}

};




class GameLevel :public PushdownState {
	Level* level;
	bool levelInitialized = false;
	bool levelCompleted = false;
	PushdownResult OnUpdate(float dt, PushdownState** newState) override {
		if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::ESCAPE) || lastFinishedLevel == -1 || !levelInitialized)
			return PushdownResult::Pop;


		//if (Window::GetKeyboard()->KeyDown(KeyboardKeys::P)) {
		//	*newState = new PauseScreen();
		//	return PushdownResult::Push;
		//}

		if (!levelCompleted) {
			level->UpdateGame(dt);
			//		Debug::Print("Press P to pause!", Vector2(10, 95));
			if (level->GameWon()) {
				levelCompleted = true;
				delete level;
				level = nullptr;
				levelInitialized = false;
				*newState = new EndScreen(true);
				return PushdownResult::Push;
			}

			if (level->GameLost()) {
				levelCompleted = true;
				delete level;
				level = nullptr;
				levelInitialized = false;
				*newState = new EndScreen(false);
				return PushdownResult::Push;
			}
		}
		return PushdownResult::NoChange;
	}
	void OnSleep() override {
		if (level != nullptr) {
			delete level;
			level = nullptr;
		}
	}

	void OnAwake() override {
		levelCompleted = false;
		levelInitialized = false;
		if (endscreenResult != -1) {
			switch (endscreenResult) {
			case 0:		break;
			case 1: ++lastFinishedLevel; break;
			case 2:		endscreenResult = -1; return;
			default:	endscreenResult = -1; return;
			}
			endscreenResult = -1;
		}

		//You are about to witness level names in sequence which is much more appropriatly named than naming of Iceland and Greenland. 
		if (!levelInitialized) {
			switch (lastFinishedLevel) {
			case -1:
			case 0:level = new LevelThree(renderer); lastFinishedLevel = 0; levelInitialized = true; break;
			case 1:level = new LevelFour(renderer); levelInitialized = true; break;
			case 2:level = new LevelOne(renderer);  levelInitialized = true; break;
			case 3:level = new LevelTwo(renderer);  levelInitialized = true; break;
			case 4:level = new LevelFive(renderer);  levelInitialized = true; break;

			default:
				lastFinishedLevel = -1; break;
			}
		}
	}
public:

	~GameLevel() {
		if (level != nullptr)
			delete level;
	}
};

class LevelSelection : public PushdownState {
	Menu* levelMenu;
	vector<string> options;

	PushdownResult OnUpdate(float dt, PushdownState** newState) override {
		if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::ESCAPE))
			return PushdownResult::Pop;
		levelMenu->Update(dt);

		if (levelMenu->OptionFinalized()) {
			switch (levelMenu->GetCurrentSelectedOptionIndex()) {
			case 0: {lastFinishedLevel = 0;
				*newState = new GameLevel();
				return PushdownResult::Push;
				break; }
			case 1: {
				lastFinishedLevel = 1;
				*newState = new GameLevel();
				return PushdownResult::Push;
				break;
			}
			case 2: {
				lastFinishedLevel = 2;
				*newState = new GameLevel();
				return PushdownResult::Push;
				break;
			}
			case 3: {
				lastFinishedLevel = 3; *newState = new GameLevel();
				return PushdownResult::Push;
				break;
			}
			case 4: {
				lastFinishedLevel = 4;
				*newState = new GameLevel();
				return PushdownResult::Push;
				break;
			}
			case 5:lastFinishedLevel = -1; return PushdownResult::Pop;
			}
			return PushdownResult::Pop;
		}
		return PushdownResult::NoChange;

	}
	void OnSleep()override { options.clear(); delete levelMenu; };
	void OnAwake() override {
		options.push_back("Level 1");
		options.push_back("Level 2");
		options.push_back("Level 3");
		options.push_back("Level 4");
		options.push_back("Level 5");
		options.push_back("Exit");
		levelMenu = new Menu(options, renderer, "Level Selection Menu!");
	}
public:
};

class IntroScreen : public PushdownState {
	Menu* mainMenu;
	vector<string> options;
	PushdownResult OnUpdate(float dt, PushdownState** newState) override {

		Debug::Print("Total Levels Completed : " + std::to_string(lastFinishedLevel), Vector2(10, 90));
		mainMenu->Update(dt);
		if (mainMenu->OptionFinalized()) {
			switch (mainMenu->GetCurrentSelectedOptionIndex()) {
			case 0:
				*newState = new GameLevel();
				return PushdownResult::Push;
				break;
			case 1:*newState = new LevelSelection();  //It was working but suddenly stopped. Did not have time to resolve.
				return PushdownResult::Push;
				break;
			case 2:return PushdownResult::Pop;
			default:
				break;
			}
		}

		return PushdownResult::NoChange;
	}

	void OnSleep() override {

	}

	void OnAwake() override {
		endscreenResult = -1;
		if (mainMenu != nullptr)
			mainMenu->Reset();
		else {
			options.clear();
			options.push_back("Play");
			options.push_back("LevelSelect");
			options.push_back("Quit");
			mainMenu = new Menu(options, renderer);
		}
	}

	~IntroScreen() {
		delete mainMenu;
	}

};


void InitiatePushdown(Window* w) {
	PushdownMachine machine(new IntroScreen());
	while (w->UpdateWindow()) {
		float dt = w->GetTimer()->GetTimeDeltaSeconds();
		if (dt > 0.1f) {
			std::cout << "Skipping large time delta" << std::endl;
			continue; //must have hit a breakpoint or something to have a 1 second frame time!
		}

		if (!machine.Update(dt)) {
			return;
		}

	}
}


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
	Window* w = Window::CreateGameWindow("CSC8503 Game technology!", 1280, 720);

	if (!w->HasInitialised()) {
		return -1;
	}

	lastFinishedLevel = -1;
	endscreenResult = -1;
	world = new GameWorld();
	renderer = new GameTechRenderer(*world);


	srand(time(0));
	w->ShowOSPointer(true);
	w->LockMouseToWindow(true);
	w->GetTimer()->GetTimeDeltaSeconds();
	InitiatePushdown(w);
	Window::DestroyGameWindow();
}

