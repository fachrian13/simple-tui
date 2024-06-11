#ifndef _TEXT_
#define _TEXT_

#include "renderable.h"

class Text final : public Renderable {
public:
	Text(std::string);
	Canvas& render() override;

private:
	Canvas canvas;
	std::string content;
};

Text::Text(std::string value) :
	content(value),
	canvas(value.size(), 1)
{}

Canvas& Text::render() {
	for (szt i = 0; i < self.content.size(); ++i)
		self.canvas.at(0, i).value = self.content.at(i);

	return self.canvas;
}

#endif