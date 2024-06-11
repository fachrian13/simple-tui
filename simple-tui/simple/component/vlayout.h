#ifndef _VERTICAL_LAYOUT_
#define _VERTICAL_LAYOUT_

#include "renderable.h"
#include <algorithm>

class VerticalLayout final : public Renderable {
public:
	VerticalLayout(std::initializer_list<Renderable*>);
	Canvas& render() override;
	~VerticalLayout();

private:
	Canvas canvas;
	std::vector<Renderable*> components;
};

VerticalLayout::VerticalLayout(std::initializer_list<Renderable*> components) :
	components(components)
{}

Canvas& VerticalLayout::render() {
	std::vector<Canvas> rendered;
	for (const auto& component : self.components)
		rendered.push_back(component->render());

	int width = 0;
	int height = 0;
	for (const auto& canvas : rendered)
		width = std::max(width, canvas.getWidth()), height += canvas.getHeight();

	self.canvas = Canvas(width, height);

	int yCursor = 0;
	for (const auto& canvas : rendered)
		for (int h = 0; h < canvas.getHeight(); ++h, ++yCursor)
			for (int w = 0; w < canvas.getWidth(); ++w)
				self.canvas.at(yCursor, w) = canvas.at(h, w);

	return self.canvas;
}

VerticalLayout::~VerticalLayout() {
	for (auto& component : self.components)
		delete component;
}

#endif