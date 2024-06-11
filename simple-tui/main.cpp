#include "simple/window.h"
#include "simple/component/hlayout.h"
#include "simple/component/text.h"
#include "simple/component/vlayout.h"
#include "simple/component/button.h"
#include "simple/component/input.h"

int main() {
	Window loginWindow = Window(120, 30);
	Input userInput = Input(30, "Ucup Mirin");
	Input passInput = Input(30, "")
		.hide(true);
	Input userInput1 = Input(30, "Sasuke bin samsudin");
	Input passInput1 = Input(30, "*****")
		.hide(true);

	loginWindow.add(
		hlayout(
			vlayout(
				hlayout(
					text("Username "),
					input(userInput)
				),
				hlayout(
					text("Password "),
					input(passInput)
				),
				hlayout(
					button("Login"),
					button("Exit")
				)
			),
			vlayout(
				hlayout(
					text("Username "),
					input(userInput)
				),
				hlayout(
					text("Password "),
					input(passInput1)
				),
				hlayout(
					button("Login"),
					button("Exit")
				)
			)
		)
	);

	while (true) {
		loginWindow.run();
	}

	std::cin.get();
}