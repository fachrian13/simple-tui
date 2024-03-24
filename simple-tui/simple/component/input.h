#ifndef _INPUT_
#define _INPUT_

#include "base.h"
#include <memory>

class Input final : public Base {
public:
	Input(int, std::string);
	void hide(bool);
	std::string getValue() const;
	const Canvas& render() override;

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

void Input::hide(bool value = true) {
	self.password = value;
}

std::string Input::getValue() const {
	return self.content;
}

const Canvas& Input::render() {
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

std::shared_ptr<Base> input(int width, std::string placeholder = "") {
	return std::make_shared<Input>(width, placeholder);
}

#endif