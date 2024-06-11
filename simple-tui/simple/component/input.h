#ifndef _INPUT_
#define _INPUT_

#include "renderable.h"
#include "selectable.h"

class Input final : public Renderable, public Selectable {
public:
	Input(int, std::string);
	Input& hide(bool);
	std::string getValue() const;
	Canvas& render() override;
	void select() override;
	void release() override;

private:
	Canvas canvas;
	bool password = false;
	int width;
	std::string content;
	std::string placeholder;
};

Input::Input(int width, std::string placeholder) :
	width(width),
	placeholder(placeholder),
	canvas(width, 1, Pixel(Color::Gray, Color::Black, Style::Underline))
{}

Input& Input::hide(bool value = true) {
	self.password = value;

	return self;
}

std::string Input::getValue() const {
	return self.content;
}

Canvas& Input::render() {
	if (self.content.empty()) {
		if (!self.placeholder.empty()) {
			for (szt i = 0; i < self.placeholder.size(); ++i) {
				if (i == self.width)
					break;

				self.canvas.at(0, i).value = self.placeholder.at(i);
			}
		}
	}
	else {
		if (!self.password) {
			for (szt i = 0; i < self.content.size(); ++i) {
				if (i == self.width)
					break;

				self.canvas.at(0, i).value = self.content.at(i);
			}
		}
		else {
			for (szt i = 0; i < self.content.size(); ++i) {
				if (i == self.width)
					break;

				self.canvas.at(0, i).value = '*';
			}
		}
	}

	return self.canvas;
}

void Input::select() {
	self.canvas.setColor(Color::White, Color::Black);
}

void Input::release() {
	self.canvas.setColor(Color::Gray, Color::Black);
}

#endif