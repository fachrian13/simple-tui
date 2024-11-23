#include "simple.h"
#include <iostream>

using simple::COLOR;
using simple::pixel;
using simple::buffer;

HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);

COORD fullscreen() {
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(hOut, &csbi);
	return csbi.dwSize;
}

int main() {
	COORD size = fullscreen();
	buffer b = buffer(size.X, size.Y);

	auto iUsername = input("John Doe");
	iUsername->limit = 8;
	iUsername->pattern = isalnum;
	auto iPassword = input();
	iPassword->hide = true;
	auto bLogin = button("Login");
	auto bExit = button("Exit", []() { exit(0); });

	auto vc = vcontainer(
		iUsername,
		iPassword,
		bLogin,
		bExit
	);
	vc->focused(true);
	std::cout << "\x1b[?25l" << std::flush;
	while (true) {
		auto vl = vlayout(
			hlayout(text("Username :"), iUsername),
			hlayout(text("Password :"), iPassword),
			bLogin,
			bExit
		);

		vl->init();
		int x = (b.getWidth() - vl->width) / 2;
		int y = (b.getHeight() - vl->height) / 2;
		vl->set({ x, y, x + vl->width, y + vl->height });
		b.clear();
		vl->render(b);
		std::cout << b.toString() << "\x1b[H" << std::flush;

		INPUT_RECORD record[128];
		DWORD  eventsRead;
		ReadConsoleInput(hIn, record, 128, &eventsRead);
		for (DWORD i = 0; i < eventsRead; ++i)
			if (record[i].EventType == KEY_EVENT && record[i].Event.KeyEvent.bKeyDown)
				vc->onkey(record[i].Event.KeyEvent);
	}
}