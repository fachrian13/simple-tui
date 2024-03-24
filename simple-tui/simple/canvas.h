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

#endif