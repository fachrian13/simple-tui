#ifndef _WINDOW_
#define _WINDOW_

#include "canvas.h"
#include "component/base.h"
#include "component/vlayout.h"
#include <iostream>
#include <memory>
#include <vector>

class Window final {
public:
	Window(int, int);
	template<class... T>
	void add(T...);
	void run();

private:
	inline void render();
	inline void print();

private:
	int width = 0;
	int height = 0;
	Canvas canvas;
	std::vector<std::shared_ptr<Base>> components;
};

Window::Window(int width, int height) :
	width(width),
	height(height),
	canvas(width, height)
{}

template<class... T>
void Window::add(T... component) {
	(self.components.push_back(std::move(component)), ...);
}

void Window::run() {
	self.render();
	self.print();
}

inline void Window::render() {
	Canvas rendered = vlayout(self.components)->render();

	for (int h = 0; h < rendered.getHeight(); ++h) {
		if (h == self.height)
			break;

		for (int w = 0; w < rendered.getWidth(); ++w) {
			if (w == self.width)
				break;

			self.canvas.at(h, w) = rendered.at(h, w);
		}
	}
}

inline void Window::print() {
	std::string result;

	for (int h = 0; h < self.height; ++h) {
		for (int w = 0; w < self.width; ++w)
			result += self.canvas.at(h, w).toString();

		result += "\n";
	}
	result.pop_back();
	result += "\x1b[0;0H";

	std::cout << result << std::flush;
}

#endif