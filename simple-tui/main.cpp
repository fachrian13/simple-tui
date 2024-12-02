#include <iostream>
#include "simple.h"

using Simple::Buffer;
using Simple::Pixel;
using Simple::Color;

int main() {
	SetConsoleOutputCP(CP_UTF8);

	bool state = true;

	HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
	Buffer b = Buffer(100, 20);

	auto bClick = Button("Click Me!");
	auto bFocused = Button("This button is focused", [&state]() { state = false; });
	auto dName = Dropdown(
		{
			"Royalty Navarro",
			"Reid Malone",
			"Skyler Quintana",
			"Kelvin Camacho",
			"Armani Spencer",
			"Ace Kent",
			"Jazmine Cain",
			"Benson Hahn",
			"Fallon Pugh",
			"Judson Lucero",
			"Ila Russo",
			"Jamie Peralta",
			"Malayah Stuart",
			"Dion Molina",
			"Alexandria Fernandez",
			"Bentley Bass",
			"Zahra Graves",
			"Cesar Snyder",
			"Callie Salazar",
			"Brody Phan",
			"Elsa Moreno",
			"Myles Molina",
			"Alexandria Medina",
			"George Gillespie",
			"Alianna Perry",
			"Waylon Cline",
			"Lina Montes",
			"Darren Graves",
			"Elle Correa",
			"Zakai Correa",
			"Valery Horton",
			"Garrett Villarreal",
			"Jazlyn Callahan",
			"Quinton Keith",
			"Elyse Vance",
			"Casen Whitaker",
			"Ivanna Haley",
			"Leif Hunt",
			"Genevieve Copeland",
			"Axton McKee",
			"Kori Bush",
			"Tyson Fischer",
			"Maci Briggs",
			"Case McConnell",
			"Denise Hull",
			"Salem Fry",
			"Clarissa Figueroa",
			"Spencer Mullen",
			"Shay Matthews",
			"Preston Booker"
		},
		"Silakan Pilih"
	);
	auto iText = Input();
	iText->Hide = true;
	auto iPlaceholder = Input("Placeholder");
	auto r1 = Radio();
	auto r2 = Radio();
	auto gR = MakeGroup(r1, r2);
	auto c1 = CheckBox();
	auto c2 = CheckBox();

	auto vContainer = VContainer(
		HContainer(bClick, bFocused),
		dName,
		HContainer(iText, iPlaceholder),
		r1,
		r2,
		c1,
		c2
	);
	vContainer->Focused(true);

	auto home = VLayout(
		Text("Vertical 1 "),
		Text("Vertical 2 "),
		HLayout(bClick, bFocused),
		dName,
		HLayout(iText, Text(" "), iPlaceholder),
		r1,
		r2,
		c1,
		c2
	);

	while (state) {
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
