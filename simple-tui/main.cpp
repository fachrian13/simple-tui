#include "simple.h"
#include <iostream>

using simple::COLOR;
using simple::pixel;
using simple::buffer;

int main() {
	buffer b = buffer(120, 30);

	auto iUsername = input("Ucup mirin");
	auto iPassword = input();
	auto bLogin = button("Login");
	auto bExit = button("Exit");
	auto vl = vlayout(
		hlayout(text("Username :"), iUsername),
		hlayout(text("Password :"), iPassword),
		bLogin,
		bExit
	);
	auto vc = vcontainer(
		iUsername,
		iPassword,
		bLogin,
		bExit
	);
	vc->focused(true);

	while (true) {
		vl->init();
		vl->set({ 0, 0, vl->width, vl->height });
		vl->render(b);
	}

	std::cout << b.toString() << std::flush;
}