#ifndef _SIMPLE_TUI_
#define _SIMPLE_TUI_
#define NOMINMAX

#include <string>
#include <vector>
#include <memory>

namespace Simple {
	enum Color : int {
		Black = 30,
		Red = 31,
		Green = 32,
		Yellow = 33,
		Blue = 34,
		Magenta = 35,
		Cyan = 36,
		White = 37,
		Default = 39,
		BrightBlack = 90,
		BrightRed = 91,
		BrightGreen = 92,
		BrightYellow = 93,
		BrightBlue = 94,
		BrightMagenta = 95,
		BrightCyan = 96,
		BrightWhite = 97
	};

	class Pixel final {
	public:
		Pixel() = default;
		Pixel(Color background, Color foreground) :
			Background(background),
			Foreground(foreground)
		{
		}
		Pixel(Color background, Color foreground, std::string value) :
			Background(background),
			Foreground(foreground),
			Value(value)
		{
		}

	public:
		Color Background = Color::Default;
		Color Foreground = Color::Default;
		std::string Value = " ";
	};
	class Buffer final {
	public:
		Buffer(int width, int height) :
			width(width),
			height(height),
			pixels(width* height)
		{
		}
		Buffer(int width, int height, Pixel style) :
			width(width),
			height(height),
			style(style),
			pixels(width* height, style)
		{
		}

		Pixel& At(size_t y, size_t x) {
			return this->pixels.at(y * this->width + x);
		}
		std::string ToString() {
			std::string result;

			for (int y = 0; y < this->height; ++y) {
				for (int x = 0; x < this->width; ++x) {
					Pixel& p = this->pixels.at(y * this->width + x);

					result += "\x1b[";
					result += std::to_string(p.Foreground);
					result += ";";
					result += std::to_string(p.Background + 10);
					result += "m";
					result += p.Value;
				}

				result += "\n";
			}

			result.pop_back();
			result += "\x1b[m";

			return result;
		}

	private:
		int width = 0;
		int height = 0;
		Pixel style;
		std::vector<Pixel> pixels;
	};
	class Rect final {
	public:
		int Left = 0;
		int Top = 0;
		int Right = 0;
		int Bottom = 0;
	};

	namespace Base {
		class Node {
		public:
			virtual void Init() {}
			virtual void Set(Rect dimension) {
				this->Dimension = dimension;
			}
			virtual void Render(Buffer& buffer) {}

		public:
			int Width = 0;
			int Height = 0;
			Rect Dimension;
		};
	}
	namespace Utils {
		template<class Type, class... Args>
		std::vector<Type> ToVector(Args&&... value) {
			return std::vector<Type>{std::forward<Type>(value)...};
		}
	}

	class VerticalLayout final : public Base::Node {
	public:
		VerticalLayout(std::vector<std::shared_ptr<Node>>&& nodes) :
			nodes(std::move(nodes))
		{
		}

		void Init() override {
			for (const auto& node : this->nodes) {
				node->Init();

				Node::Width = std::max(this->Width, node->Width);
				Node::Height += node->Height;
			}
		}
		void Set(Rect dimension) override {
			Node::Set(dimension);

			int t = Node::Dimension.Top;
			for (const auto& node : this->nodes) {
				int l = Node::Dimension.Left;
				int r = l + node->Width;
				int b = t + node->Height;

				node->Set({ l, t, r, b });

				t = b;
			}
		}
		void Render(Buffer& buffer) override {
			for (const auto& node : this->nodes)
				node->Render(buffer);
		}

	private:
		std::vector<std::shared_ptr<Node>> nodes;
	};
	class HorizontalLayout final : public Base::Node {
	public:
		HorizontalLayout(std::vector<std::shared_ptr<Node>>&& nodes) :
			nodes(std::move(nodes))
		{
		}

		void Init() override {
			for (const auto& node : this->nodes) {
				node->Init();

				Node::Width += node->Width;
				Node::Height = std::max(this->Height, node->Height);
			}
		}
		void Set(Rect dimension) override {
			Node::Set(dimension);

			int l = Node::Dimension.Left;
			for (const auto& node : this->nodes) {
				int t = Node::Dimension.Top;
				int r = l + node->Width;
				int b = t + node->Height;

				node->Set({ l, t, r, b });

				l = r;
			}
		}
		void Render(Buffer& buffer) override {
			for (const auto& node : this->nodes)
				node->Render(buffer);
		}

	private:
		std::vector<std::shared_ptr<Node>> nodes;
	};
	class Text final : public Base::Node {
	public:
		Text(std::string value) :
			value(std::move(value))
		{
		}

		void Init() override {
			Node::Width = int(this->value.size());
			Node::Height = 1;
		}
		void Render(Buffer& buffer) override {
			for (int y = 0, i = 0; y < Node::Height; ++y)
				for (int x = 0; x < Node::Width; ++x, ++i)
					buffer.At(y, x).Value = value.at(i);
		}

	private:
		std::string value;
	};
}

template<class Type, class... Args>
std::shared_ptr<Simple::Base::Node> VLayout(Type&& node, Args&&... nodes) {
	return std::make_shared<Simple::VerticalLayout>(
		Simple::Utils::ToVector<std::shared_ptr<Simple::Base::Node>>(
			node, nodes...
		)
	);
}
std::shared_ptr<Simple::Base::Node> Text(std::string value) {
	return std::make_shared<Simple::Text>(std::move(value));
}

#endif