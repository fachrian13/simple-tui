#include "simple/component/hlayout.h"
#include "simple/component/text.h"
#include "simple/component/vlayout.h"
#include "simple/component/button.h"
#include "simple/component/input.h"
#include "simple/container.h"
#include <iostream>

int main() {
	auto iUsername = Input(30, "fahrisyn13");
	auto iPassword = Input(30, "********");
	auto bLogin = Button("Login", []() {});
	auto bExit = Button("Exit", []() {});
	auto lHome = VerticalLayout({
		new HorizontalLayout({
			new Text("Username"),
			&iUsername
		}),
		new HorizontalLayout({
			new Text("Password"),
			&iPassword
		}),
		new HorizontalLayout({
			&bLogin,
			new Text(" "),
			&bExit
		})
	});
	auto container = Container({
		&iUsername,
		&iPassword,
		&bLogin,
		&bExit
	});

	while (true) {
		std::cout << lHome.render().toString() << std::flush;
		container.readInput();
	}
}