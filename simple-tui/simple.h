#ifndef _SIMPLE_TUI_
#define _SIMPLE_TUI_

#include <string>
#include <vector>

namespace simple {
	enum COLOR : int {
		BLACK = 30,
		RED = 31,
		GREEN = 32,
		YELLOW = 33,
		BLUE = 34,
		MAGENTA = 35,
		CYAN = 36,
		WHITE = 37,
		DEFAULT = 39,
		BRIGHT_BLACK = 90,
		BRIGHT_RED = 91,
		BRIGHT_GREEN = 92,
		BRIGHT_YELLOW = 93,
		BRIGHT_BLUE = 94,
		BRIGHT_MAGENTA = 95,
		BRIGHT_CYAN = 96,
		BRIGHT_WHITE = 97
	};
	class pixel final {
	public:
		pixel() = default;
		pixel(COLOR background, COLOR foreground) :
			background(background),
			foreground(foreground)
		{}
		pixel(COLOR background, COLOR foreground, char value) :
			background(background),
			foreground(foreground),
			value(value)
		{}

	public:
		bool bold = false;
		bool dim = false;
		bool italic = false;
		bool underline = false;
		bool blink = false;
		bool invert = false;
		bool invisible = false;
		bool strikethrough = false;
		COLOR background = COLOR::DEFAULT;
		COLOR foreground = COLOR::DEFAULT;
		char value = ' ';
	};
	class buffer final {
	public:
		buffer(int width, int height) :
			width(width),
			height(height),
			pixels(width* height)
		{}
		buffer(int width, int height, pixel style) :
			width(width),
			height(height),
			style(style),
			pixels(width* height, style)
		{}

		pixel& at(size_t y, size_t x) {
			return this->pixels.at(y * this->width + x);
		}
		std::string toString() {
			std::string result;

			for (int y = 0; y < this->height; ++y) {
				for (int x = 0; x < this->width; ++x) {
					pixel& p = this->pixels.at(y * this->width + x);

					result.append("\x1b[");
					result.append(p.bold ? "1;" : "22;");
					result.append(p.dim ? "2;" : "22;");
					result.append(p.italic ? "3;" : "23;");
					result.append(p.underline ? "4;" : "24;");
					result.append(p.blink ? "5;" : "25;");
					result.append(p.invert ? "7;" : "27;");
					result.append(p.invisible ? "8;" : "28;");
					result.append(p.strikethrough ? "9;" : "29;");
					result.append(std::to_string(p.foreground) + ";");
					result.append(std::to_string(p.background + 10) + "m");
					result.push_back(p.value);
				}
				result.push_back('\n');
			}
			result.pop_back();
			result.append("\x1b[m");

			return result;
		}

	private:
		int width = 0;
		int height = 0;
		pixel style;
		std::vector<pixel> pixels;
	};
}

#endif