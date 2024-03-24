#ifndef _BUTTON_
#define _BUTTON_

#include "base.h"
#include <functional>
#include <memory>

class Button final : public Base {
public:
	Button(std::string, std::function<void()>);
	void onClick();
	const Canvas& render() override;

private:
	Canvas canvas;
	std::string name;
	std::function<void()> logic;
};

Button::Button(std::string name, std::function<void()> logic) :
	name(name),
	canvas(name.size() + 4, 1),
	logic(logic)
{}

void Button::onClick() {
	self.logic();
}

const Canvas& Button::render() {
	self.canvas.at(0, 0).value = '[';
	self.canvas.at(0, 1).value = ' ';
	for (szt i = 0; i < self.name.size(); ++i)
		self.canvas.at(0, i + 2).value = self.name.at(i);
	self.canvas.at(0, self.canvas.getWidth() - 2).value = ' ';
	self.canvas.at(0, self.canvas.getWidth() - 1).value = ']';

	return self.canvas;
}

std::shared_ptr<Base> button(std::string name, std::function<void()> logic = []() {}) {
	return std::make_shared<Button>(name, logic);
}

#endif