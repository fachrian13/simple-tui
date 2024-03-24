#include "simple/window.h"
#include "simple/component/hlayout.h"
#include "simple/component/text.h"
#include "simple/component/vlayout.h"
#include "simple/component/button.h"
#include "simple/component/input.h"

int main() {
	Window loginWindow = Window(120, 30);
	Input userInput = Input(30, "Ucup Mirin");
	Input passInput = Input(30, "");
	passInput.hide();

	loginWindow.add(
		hlayout(
			text("Username "),
			std::make_shared<Input>(userInput)
		),
		hlayout(
			text("Password "),
			std::make_shared<Input>(passInput)
		),
		hlayout(
			button("Login"),
			button("Exit")
		)
	);
	loginWindow.run();

	std::cin.get();
}