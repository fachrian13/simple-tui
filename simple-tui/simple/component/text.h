#ifndef _TEXT_
#define _TEXT_

#include "base.h"
#include <memory>

class Text final : public Base {
public:
	Text(std::string);
	const Canvas& render() override;

private:
	Canvas canvas;
	std::string content;
};

Text::Text(std::string value) :
	content(value),
	canvas(value.size(), 1)
{}

const Canvas& Text::render() {
	for (szt i = 0; i < self.content.size(); ++i)
		self.canvas.at(0, i).value = self.content.at(i);

	return self.canvas;
}

std::shared_ptr<Base> text(std::string value) {
	return std::make_shared<Text>(value);
}

#endif