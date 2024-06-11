#ifndef _PIXEL_
#define _PIXEL_

#include "tools.h"
#include <string>

enum Style {
	Bold = 0x001,
	Dim = 0x002,
	Italic = 0x004,
	Underline = 0x008,
	Blinking = 0x010,
	Reverse = 0x020,
	Hidden = 0x040,
	Strikethrough = 0x080,
	Reset = 0x100
};

enum Color {
	Black = 0x1E,
	Red = 0x1F,
	Green = 0x20,
	Yellow = 0x21,
	Blue = 0x22,
	Magenta = 0x23,
	Cyan = 0x24,
	White = 0x25,
	Default = 0x27,
	Gray = 0x5A,
	BrightRed = 0x5B,
	BrightGreen = 0x5C,
	BrightYellow = 0x5D,
	BrightBlue = 0x5E,
	BrightMagenta = 0x5F,
	BrightCyan = 0x60,
	BrightWhite = 0x61
};

class Pixel final {
public:
	Pixel();
	Pixel(Color, Color, int, char);
	std::string toString();

public:
	Color background = Color::Default;
	Color foreground = Color::Default;
	int style = Style::Reset;
	char value = ' ';
};

Pixel::Pixel() {}

Pixel::Pixel(Color background, Color foreground, int style = Style::Reset, char value = ' ') :
	background(background),
	foreground(foreground),
	style(style),
	value(value)
{}

std::string Pixel::toString() {
	std::string result = "\x1b[";

	if (self.style & Style::Reset) result += "0;";
	if (self.style & Style::Bold) result += "1;";
	if (self.style & Style::Dim) result += "2;";
	if (self.style & Style::Italic) result += "3;";
	if (self.style & Style::Underline) result += "4;";
	if (self.style & Style::Blinking) result += "5;";
	if (self.style & Style::Reverse) result += "7;";
	if (self.style & Style::Hidden) result += "8;";
	if (self.style & Style::Strikethrough) result += "9;";

	result += std::to_string(self.foreground);
	result += ";";
	result += std::to_string(self.background + 10);
	result += "m";
	result += self.value;

	return result;
}

#endif