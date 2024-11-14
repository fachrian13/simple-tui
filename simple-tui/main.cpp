#include "simple.h"
#include <iostream>

using simple::COLOR;
using simple::pixel;
using simple::buffer;

int main() {
	buffer b = buffer(120, 30, pixel(COLOR::GREEN, COLOR::BRIGHT_RED));

	auto bLogin = button("Login");
	auto bExit = button("Exit");
	auto v = vlayout(
		text("VERTICAL-1 "),
		text("VERTICAL-2 "),
		hlayout(
			text("HORIZONTAL-1 "),
			text("HORIZONTAL-2 "),
			vlayout(
				text("VERTICAL-1 "),
				text("VERTICAL-2 "),
				hlayout(
					text("HORIZONTAL-1 "),
					text("HORIZONTAL-2 "),
					text("HORIZONTAL-3 "),
					text("HORIZONTAL-4 ")
				),
				text("VERTICAL-3 "),
				text("VERTICAL-4 ")
			),
			text("HORIZONTAL-3 "),
			text("HORIZONTAL-4 ")
		),
		text("VERTICAL-3 "),
		text("VERTICAL-4 ")
	);
	v->init();
	v->set({ 0, 0, v->width, v->height });
	v->render(b);

	std::cout << b.toString() << std::flush;
}