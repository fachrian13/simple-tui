#ifndef _SIMPLE_TUI_
#define _SIMPLE_TUI_
#define NOMINMAX

#include <algorithm>
#include <memory>
#include <string>
#include <vector>
#include <windows.h>

namespace simple {
	enum COLOR : int {
		BLACK = 30,
		RED = 31,
		GREEN = 32,
		YELLOW = 33,
		BLUE = 34,
		MAGENTA = 35,
		CYAN = 36,
		WHITE = 37,
		DEFAULT = 39,
		BRIGHT_BLACK = 90,
		BRIGHT_RED = 91,
		BRIGHT_GREEN = 92,
		BRIGHT_YELLOW = 93,
		BRIGHT_BLUE = 94,
		BRIGHT_MAGENTA = 95,
		BRIGHT_CYAN = 96,
		BRIGHT_WHITE = 97
	};
	class pixel final {
	public:
		pixel() = default;
		pixel(COLOR background, COLOR foreground) :
			background(background),
			foreground(foreground)
		{}
		pixel(COLOR background, COLOR foreground, char value) :
			background(background),
			foreground(foreground),
			value(value)
		{}

	public:
		bool bold = false;
		bool dim = false;
		bool italic = false;
		bool underline = false;
		bool blink = false;
		bool invert = false;
		bool invisible = false;
		bool strikethrough = false;
		COLOR background = COLOR::DEFAULT;
		COLOR foreground = COLOR::DEFAULT;
		char value = ' ';
	};
	class buffer final {
	public:
		buffer(int width, int height) :
			width(width),
			height(height),
			pixels(width* height)
		{}
		buffer(int width, int height, pixel style) :
			width(width),
			height(height),
			style(style),
			pixels(width* height, style)
		{}

		pixel& at(size_t y, size_t x) {
			return this->pixels.at(y * this->width + x);
		}
		std::string toString() {
			std::string result;

			for (int y = 0; y < this->height; ++y) {
				for (int x = 0; x < this->width; ++x) {
					pixel& p = this->pixels.at(y * this->width + x);

					result.append("\x1b[");
					result.append(p.bold ? "1;" : "22;");
					result.append(p.dim ? "2;" : "22;");
					result.append(p.italic ? "3;" : "23;");
					result.append(p.underline ? "4;" : "24;");
					result.append(p.blink ? "5;" : "25;");
					result.append(p.invert ? "7;" : "27;");
					result.append(p.invisible ? "8;" : "28;");
					result.append(p.strikethrough ? "9;" : "29;");
					result.append(std::to_string(p.foreground) + ";");
					result.append(std::to_string(p.background + 10) + "m");
					result.push_back(p.value);
				}
				result.push_back('\n');
			}
			result.pop_back();
			result.append("\x1b[m");

			return result;
		}

	private:
		int width = 0;
		int height = 0;
		pixel style;
		std::vector<pixel> pixels;
	};
	class rect final {
	public:
		int left = 0;
		int top = 0;
		int right = 0;
		int bottom = 0;
	};

	namespace base {
		class node {
		public:
			virtual void init() {}
			virtual void set(rect dimension) {
				this->dimension = dimension;
			}
			virtual void render(buffer&) {}

		public:
			int width = 0;
			int height = 0;
			rect dimension;
		};
		class component {
		public:
			bool focused() {
				return this->focus;
			}
			virtual void focused(bool flag) {
				this->focus = flag;
			}
			virtual bool onkey(KEY_EVENT_RECORD) {
				return false;
			}

		private:
			bool focus = false;
		};
	}

	class vertical_layout final : public base::node {
	public:
		vertical_layout(std::vector<std::shared_ptr<base::node>> nodes) :
			nodes(std::move(nodes))
		{}

		void init() override {
			for (const auto& node : this->nodes) {
				node->init();

				node::width = std::max(node::width, node->width);
				node::height += node->height;
			}
		}
		void set(rect dimension) override {
			node::set(dimension);

			int top = dimension.top;
			for (const auto& node : this->nodes) {
				int left = dimension.left;
				int right = left + node->width;
				int bottom = top + node->height;

				node->set({ left, top, right, bottom });

				top = bottom;
			}
		}
		void render(buffer& buf) override {
			for (const auto& node : this->nodes)
				node->render(buf);
		}

	private:
		std::vector<std::shared_ptr<base::node>> nodes;
	};
	class horizontal_layout final : public base::node {
	public:
		horizontal_layout(std::vector<std::shared_ptr<base::node>> nodes) :
			nodes(std::move(nodes))
		{}

		void init() override {
			for (const auto& node : this->nodes) {
				node->init();

				node::width += node->width;
				node::height = std::max(node::height, node->height);
			}
		}
		void set(rect dimension) override {
			node::set(dimension);

			int left = dimension.left;
			for (const auto& node : this->nodes) {
				int top = dimension.top;
				int right = left + node->width;
				int bottom = top + node->height;

				node->set({ left, top, right, bottom });

				left = right;
			}
		}
		void render(buffer& buf) override {
			for (const auto& node : this->nodes)
				node->render(buf);
		}

	private:
		std::vector<std::shared_ptr<base::node>> nodes;
	};
	class text final : public base::node {
	public:
		text(std::string text) :
			value(text)
		{}

		void init() override {
			node::width = int(this->value.size());
			node::height = 1;
		}
		void render(buffer& buf) override {
			for (int y = node::dimension.top, i = 0; y < node::dimension.bottom; ++y)
				for (int x = node::dimension.left; x < node::dimension.right; ++x, ++i)
					buf.at(y, x).value = this->value.at(i);
		}

	private:
		std::string value;
	};

	class vertical_container final : public base::component {
	public:
		vertical_container(std::vector<std::shared_ptr<component>> components) :
			components(std::move(components))
		{}

		void focused(bool flag) override {
			component::focused(flag);
			this->components.at(this->focusedComponent)->focused(flag);
		}
		bool onkey(KEY_EVENT_RECORD key) override {
			switch (key.wVirtualKeyCode) {
			case VK_DOWN:
				if (this->focusedComponent < this->components.size() - 1) {
					this->components.at(this->focusedComponent)->focused(false);
					this->components.at(++this->focusedComponent)->focused(true);
				}
				return true;
			case VK_UP:
				if (this->focusedComponent > 0) {
					this->components.at(this->focusedComponent)->focused(false);
					this->components.at(--this->focusedComponent)->focused(true);
				}
				return true;
			}

			return false;
		}

	private:
		size_t focusedComponent = 0;
		std::vector<std::shared_ptr<component>> components;
	};
	class horizontal_container final : public base::component {
	public:
		horizontal_container(std::vector<std::shared_ptr<component>> components) :
			components(std::move(components))
		{}

		void focused(bool flag) override {
			component::focused(flag);
			this->components.at(this->focusedComponent)->focused(flag);
		}
		bool onkey(KEY_EVENT_RECORD key) override {
			switch (key.wVirtualKeyCode) {
			case VK_RIGHT:
				if (this->focusedComponent < this->components.size() - 1) {
					this->components.at(this->focusedComponent)->focused(false);
					this->components.at(++this->focusedComponent)->focused(true);
				}
				return true;
			case VK_LEFT:
				if (this->focusedComponent > 0) {
					this->components.at(this->focusedComponent)->focused(false);
					this->components.at(--this->focusedComponent)->focused(true);
				}
				return true;
			}

			return false;
		}

	private:
		size_t focusedComponent = 0;
		std::vector<std::shared_ptr<component>> components;
	};
	class button final : public base::node, public base::component {
	public:
		button(std::string name) :
			name(name)
		{}

		void init() override {
			node::width = int(this->name.size()) + 2;
			node::height = 1;
		}
		void render(buffer& buf) override {
			for (int y = node::dimension.top; y < node::dimension.bottom; ++y)
				for (int x = node::dimension.left; x < node::dimension.right; ++x)
					buf.at(y, x).invert = true;

			if (component::focused()) {
				for (int y = node::dimension.top; y < node::dimension.bottom; ++y)
					for (int x = node::dimension.left; x < node::dimension.right; ++x)
						buf.at(y, x).invert = false;
			}

			buf.at(node::dimension.top, node::dimension.left).value = '[';
			buf.at(node::dimension.top, node::dimension.right - 1).value = ']';
			for (int y = node::dimension.top, i = 0; y < node::dimension.bottom; ++y)
				for (int x = node::dimension.left + 1; x < node::dimension.right - 1; ++x, ++i)
					buf.at(y, x).value = this->name.at(i);
		}

	private:
		std::string name;
	};
}

template<class T, class... A>
std::shared_ptr<simple::base::node> vlayout(T node, A... args) {
	std::vector<std::shared_ptr<simple::base::node>> nodes;

	nodes.push_back(std::move(node));
	((nodes.push_back(std::move(args))), ...);

	return std::make_shared<simple::vertical_layout>(std::move(nodes));
}
template<class T, class... A>
std::shared_ptr<simple::base::node> hlayout(T node, A... args) {
	std::vector<std::shared_ptr<simple::base::node>> nodes;

	nodes.push_back(std::move(node));
	((nodes.push_back(std::move(args))), ...);

	return std::make_shared<simple::horizontal_layout>(std::move(nodes));
}
std::shared_ptr<simple::base::node> text(std::string value) {
	return std::make_shared<simple::text>(value);
}

template<class T, class... A>
std::shared_ptr<simple::base::component> vcontainer(T node, A... args) {
	std::vector<std::shared_ptr<simple::base::component>> nodes;

	nodes.push_back(std::move(node));
	((nodes.push_back(std::move(args))), ...);

	return std::make_shared<simple::vertical_container>(std::move(nodes));
}
template<class T, class... A>
std::shared_ptr<simple::base::component> hcontainer(T node, A... args) {
	std::vector<std::shared_ptr<simple::base::component>> nodes;

	nodes.push_back(std::move(node));
	((nodes.push_back(std::move(args))), ...);

	return std::make_shared<simple::horizontal_container>(std::move(nodes));
}
std::shared_ptr<simple::button> button(std::string name) {
	return std::make_shared<simple::button>(name);
}

#endif