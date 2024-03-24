#ifndef _VERTICAL_LAYOUT_
#define _VERTICAL_LAYOUT_

#include "base.h"
#include <algorithm>
#include <memory>

class VerticalLayout final : public Base {
public:
	VerticalLayout(std::vector<std::shared_ptr<Base>>);
	const Canvas& render() override;

private:
	Canvas canvas;
	std::vector<std::shared_ptr<Base>> components;
};

VerticalLayout::VerticalLayout(std::vector<std::shared_ptr<Base>> components) :
	components(std::move(components))
{}

const Canvas& VerticalLayout::render() {
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

template<class... T>
std::shared_ptr<Base> vlayout(T... components) {
	return std::make_shared<VerticalLayout>(std::vector<std::shared_ptr<Base>>{ components... });
}

#endif