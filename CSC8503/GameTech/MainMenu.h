#pragma once
#include <string>
#include <vector>
#include "GameTechRenderer.h"

using namespace std;
namespace NCL {
	namespace CSC8503 {

		class Menu {
		public:
			Menu(vector<string> options, GameTechRenderer* renderer, string title = "");
			~Menu();
			void Reset();
			void Update(float dt);
			bool OptionFinalized() { return isOptionFinalized; }
			int GetCurrentSelectedOptionIndex() { return selectedOption; }

		protected:
			GameTechRenderer* renderer;
			GameWorld* world;
			vector<string > menuOptions;
			int selectedOption;
			bool isOptionFinalized;
			string title = "";

		};
	}
}