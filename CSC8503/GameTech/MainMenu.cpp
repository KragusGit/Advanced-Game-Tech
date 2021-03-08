#include "MainMenu.h"

using namespace NCL;
using namespace CSC8503;

Menu::Menu(vector<string> options, GameTechRenderer* renderer, string title ) {
	this->renderer = renderer;
	Debug::SetRenderer(renderer);
	menuOptions = options;
	this->title = title;
}

Menu::~Menu() {
}

void Menu::Reset() {

	Debug::SetRenderer(renderer);
	isOptionFinalized = false;
	selectedOption = -1;
}

void Menu::Update(float dt)

{
	if (!isOptionFinalized) {
		if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::UP)) {
			selectedOption = selectedOption > 0 ? --selectedOption : menuOptions.size() - 1;
		}

		if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::DOWN)) {
			selectedOption = selectedOption < menuOptions.size() - 1 ? ++selectedOption : 0;
		}

		if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::RETURN)) {
			isOptionFinalized = true;
		}

	}

	Debug::Print(title, Vector2(2, 10), Debug::BLACK);
	for (int i = 0; i < menuOptions.size(); ++i) {
		Debug::Print(menuOptions[i], Vector2(2, 30 + (i * 10)), i == selectedOption ? Debug::WHITE : Debug::BLACK);
	}
	Debug::FlushRenderables(dt);
	renderer->Update(dt);
	renderer->Render();
}
