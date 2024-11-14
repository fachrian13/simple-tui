#include "simple.h"
#include <iostream>

using simple::COLOR;
using simple::pixel;
using simple::buffer;

int main() {
	buffer b = buffer(120, 30, pixel(COLOR::GREEN, COLOR::BRIGHT_RED, '*'));

	std::cout << b.toString() << std::flush;
}