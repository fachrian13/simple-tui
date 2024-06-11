#ifndef _BUTTON_
#define _BUTTON_

#include "renderable.h"
#include "selectable.h"
#include <functional>

class Button final : public Renderable, public Selectable {
public:
	Button(std::string, std::function<void()>);
	void onClick();
	Canvas& render() override;
	void select() override;
	void release() override;

private:
	Canvas canvas;
	std::string name;
	std::function<void()> logic;
};

Button::Button(std::string name, std::function<void()> logic) :
	name(name),
	canvas(name.size() + 4, 1, Pixel(Color::Gray, Color::Black)),
	logic(logic)
{}

void Button::onClick() {
	self.logic();
}

Canvas& Button::render() {
	/*self.canvas.at(0, 0).value = '[';*/
	for (szt i = 0; i < self.name.size(); ++i)
		self.canvas.at(0, i + 2).value = self.name.at(i);
	/*self.canvas.at(0, self.canvas.getWidth() - 1).value = ']';*/

	return self.canvas;
}

void Button::select() {
	self.canvas.setColor(Color::White, Color::Black);
}

void Button::release() {
	self.canvas.setColor(Color::Gray, Color::Black);
}

#endif