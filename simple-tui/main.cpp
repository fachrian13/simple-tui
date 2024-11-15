#include "simple.h"
#include <iostream>

using simple::COLOR;
using simple::pixel;
using simple::buffer;

int main() {
	buffer b = buffer(120, 30);
	HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);

	auto iUsername = input("Masukkan karakter");
	iUsername->height = 3;
	auto iPassword = input("Masukkan karakter");
	auto bLogin = button("Login");
	auto bExit = button("Exit", []() { exit(0); });
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

	INPUT_RECORD rec[128];
	DWORD numberOfEventsRead;
	while (true) {
		b.clear();
		vl->init();
		vl->set({ 0, 0, vl->width, vl->height });
		vl->render(b);
		std::cout << "\x1b[H" << b.toString() << std::flush;

		ReadConsoleInput(hInput, rec, 128, &numberOfEventsRead);

		for (DWORD i = 0; i < numberOfEventsRead; ++i)
			if (rec[i].EventType == KEY_EVENT && rec[i].Event.KeyEvent.bKeyDown)
				vc->onkey(rec[i].Event.KeyEvent);
	}
}