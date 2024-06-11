#ifndef _CANVAS_
#define _CANVAS_

#include "pixel.h"
#include <vector>

class Canvas final {
public:
	Canvas();
	Canvas(szt, szt, Pixel);
	Pixel& at(szt, szt);
	Pixel at(szt, szt) const;
	int getWidth() const;
	int getHeight() const;
	std::string toString();
	void setColor(Color, Color);

private:
	int width = 0;
	int height = 0;
	std::vector<std::vector<Pixel>> pixels;
};

Canvas::Canvas() {}

Canvas::Canvas(szt width, szt height, Pixel style = Pixel()) :
	width(i32(width)),
	height(i32(height)),
	pixels(height, std::vector<Pixel>(width, style))
{}

Pixel& Canvas::at(szt y, szt x) {
	return self.pixels.at(y).at(x);
}

Pixel Canvas::at(szt y, szt x) const {
	return self.pixels.at(y).at(x);
}

int Canvas::getWidth() const {
	return self.width;
}

int Canvas::getHeight() const {
	return self.height;
}

std::string Canvas::toString() {
	std::string result = "\x1b[0;0H";

	for (int h = 0; h < self.height; ++h) {
		for (int w = 0; w < self.width; ++w)
			result += self.pixels.at(h).at(w).toString();

		result += "\n";
	}
	result.pop_back();

	return result;
}

void Canvas::setColor(Color background, Color foreground) {
	for (int h = 0; h < self.height; ++h)
		for (int w = 0; w < self.width; ++w)
			self.pixels.at(h).at(w).background = background, self.pixels.at(h).at(w).foreground = foreground;
}


#endif