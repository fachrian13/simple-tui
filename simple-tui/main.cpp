#include <iostream>
#include "simple.h"

using Simple::Buffer;
using Simple::Pixel;
using Simple::Color;

int main() {
	SetConsoleOutputCP(CP_UTF8);

	bool state = true;

	HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
	Buffer b = Buffer(100, 20, Pixel(Color::Default, Color::BrightRed));

	auto bClick = Button("Click Me!");
	auto bFocused = Button("This button is focused", [&state]() { state = false; });
	auto iText = Input();
	auto iPlaceholder = Input("Placeholder");

	auto vContainer = VContainer(bClick, bFocused, iText, iPlaceholder);
	vContainer->Focused(true);

	while (state) {
		auto home = VLayout(
			Text("Vertical 1 "),
			Text("Vertical 2 "),
			HLayout(bClick, bFocused),
			HLayout(iText, Text(" "), iPlaceholder)
		);
		home->Init();
		home->Set({ 0, 0, home->Width, home->Height });
		home->Render(b);
		std::cout << b.ToString() << "\x1b[H";
		b.Clear();

		INPUT_RECORD rec[128];
		DWORD size;
		ReadConsoleInput(hIn, rec, 128, &size);

		for (DWORD i = 0; i < size; ++i)
			if (rec[i].EventType == KEY_EVENT && rec[i].Event.KeyEvent.bKeyDown)
				vContainer->OnKey(rec[i].Event.KeyEvent);
	}

	return 0;
}
