#ifndef _HORIZONTAL_LAYOUT_
#define _HORIZONTAL_LAYOUT_

#include "renderable.h"

class HorizontalLayout final : public Renderable {
public:
	HorizontalLayout(std::initializer_list<Renderable*>);
	Canvas& render() override;

private:
	Canvas canvas;
	std::vector<Renderable*> components;
	~HorizontalLayout();
};

HorizontalLayout::HorizontalLayout(std::initializer_list<Renderable*> components) :
	components(components)
{}

Canvas& HorizontalLayout::render() {
	std::vector<Canvas> rendered;
	for (const auto& component : self.components)
		rendered.push_back(component->render());

	int width = 0;
	int height = 0;
	for (const auto& canvas : rendered)
		width += canvas.getWidth(), height = std::max(height, canvas.getHeight());

	self.canvas = Canvas(width, height);

	int xCursor = 0;
	int yCursor = 0;
	for (const auto& canvas : rendered) {
		for (int h = 0, y = yCursor; h < canvas.getHeight(); ++h, ++y)
			for (int w = 0, x = xCursor; w < canvas.getWidth(); ++w, ++x)
				self.canvas.at(y, x) = canvas.at(h, w);

		xCursor += canvas.getWidth();
	}

	return self.canvas;
}

HorizontalLayout::~HorizontalLayout() {
	for (auto& component : self.components)
		delete component;
}

#endif