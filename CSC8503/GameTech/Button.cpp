#include "Button.h"

void NCL::CSC8503::Button::ExecuteOnPress() {
	for (int i = 0; i < OnPress.size(); i++) {
		if (OnPress[i] != nullptr)
			OnPress[i]();
	}
}
