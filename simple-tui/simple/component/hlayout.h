#ifndef _HORIZONTAL_LAYOUT_
#define _HORIZONTAL_LAYOUT_

#include "base.h"
#include <memory>

class HorizontalLayout final : public Base {
public:
	HorizontalLayout(std::vector<std::shared_ptr<Base>>);
	const Canvas& render() override;

private:
	Canvas canvas;
	std::vector<std::shared_ptr<Base>> components;
};

HorizontalLayout::HorizontalLayout(std::vector<std::shared_ptr<Base>> components) :
	components(std::move(components))
{}

const Canvas& HorizontalLayout::render() {
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

template<class... T>
std::shared_ptr<Base> hlayout(T... components) {
	return std::make_shared<HorizontalLayout>(std::vector<std::shared_ptr<Base>>{ components... });
}

#endif