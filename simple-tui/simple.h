#ifndef _SIMPLE_TUI_
#define _SIMPLE_TUI_
#define NOMINMAX

#include <algorithm>
#include <functional>
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
		{
		}
		pixel(COLOR background, COLOR foreground, char value) :
			background(background),
			foreground(foreground),
			value(value)
		{
		}

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
		{
		}
		buffer(int width, int height, pixel style) :
			width(width),
			height(height),
			style(style),
			pixels(width* height, style)
		{
		}

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
		int getWidth() {
			return this->width;
		}
		int getHeight() {
			return this->height;
		}

		void clear() {
			this->pixels = std::vector<pixel>(this->width * this->height, this->style);
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
		class node_style : public node {
		public:
			node_style(std::shared_ptr<node> target) :
				target(std::move(target))
			{
			}

			virtual void init() override {
				this->target->init();
			}
			virtual void set(rect dimension) override {
				this->target->set(dimension);
			}
			virtual void render(buffer& buf) override {
				this->target->render(buf);
			}

		protected:
			std::shared_ptr<node> target;
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
		class buttons_group; class check_button {
		public:
			bool checked() {
				return this->isChecked;
			}
			void checked(bool flag) {
				this->isChecked = flag;
			}
			void setGroup(buttons_group* group) {
				this->group = group;
			}

		protected:
			buttons_group* group = nullptr;

		private:
			bool isChecked = false;
		};
		class buttons_group final {
		public:
			buttons_group(std::vector<std::shared_ptr<check_button>> buttons) {
				for (const auto& button : buttons) {
					button->setGroup(this);
					this->buttons.push_back(std::move(button));
				}
			}
			void clear() {
				for (const auto& button : this->buttons)
					button->checked(false);
			}

		private:
			std::vector<std::shared_ptr<check_button>> buttons;
		};
	}
	using buttons_group = base::buttons_group;

	class vertical_layout final : public base::node {
	public:
		vertical_layout(std::vector<std::shared_ptr<base::node>> nodes) :
			nodes(std::move(nodes))
		{
		}

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
		{
		}

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
		{
		}

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

	class border final : public base::node_style {
	public:
		border(std::shared_ptr<node> node) :
			node_style(std::move(node))
		{
		}

		void init() override {
			node_style::init();

			node::width = node_style::target->width + 2;
			node::height = node_style::target->height + 2;
		}
		void set(rect dimension) override {
			node::set(dimension);

			rect dim = node::dimension;
			dim.left += 1;
			dim.top += 1;
			dim.right -= 1;
			dim.bottom -= 1;
			node_style::set(dim);
		}
		void render(buffer& buf) override {
			node_style::render(buf);

			buf.at(node::dimension.top, node::dimension.left).value = '+';
			buf.at(node::dimension.top, node::dimension.right - 1).value = '+';
			buf.at(node::dimension.bottom - 1, node::dimension.left).value = '+';
			buf.at(node::dimension.bottom - 1, node::dimension.right - 1).value = '+';

			for (int x = node::dimension.left + 1; x < node::dimension.right - 1; x++) {
				buf.at(node::dimension.top, x).value = '-';
				buf.at(node::dimension.bottom - 1, x).value = '-';
			}

			for (int y = node::dimension.top + 1; y < node::dimension.bottom - 1; ++y) {
				buf.at(y, node::dimension.left).value = '|';
				buf.at(y, node::dimension.right - 1).value = '|';
			}
		}
	};
	class background final : public base::node_style {
	public:
		background(std::shared_ptr<node> node, COLOR color) :
			node_style(std::move(node)),
			color(color)
		{
		}

		void init() override {
			node_style::init();

			node::width = node_style::target->width;
			node::height = node_style::target->height;
		}
		void set(rect dimension) override {
			node::set(dimension);
			node_style::set(dimension);
		}
		void render(buffer& buf) override {
			node_style::render(buf);

			for (int y = node::dimension.top; y < node::dimension.bottom; ++y)
				for (int x = node::dimension.left; x < node::dimension.right; ++x)
					buf.at(y, x).background = this->color;
		}

	private:
		COLOR color = COLOR::DEFAULT;
	};
	class foreground final : public base::node_style {
	public:
		foreground(std::shared_ptr<node> node, COLOR color) :
			node_style(std::move(node)),
			color(color)
		{
		}

		void init() override {
			node_style::init();

			node::width = node_style::target->width;
			node::height = node_style::target->height;
		}
		void set(rect dimension) override {
			node::set(dimension);
			node_style::set(dimension);
		}
		void render(buffer& buf) override {
			node_style::render(buf);

			for (int y = node::dimension.top; y < node::dimension.bottom; ++y)
				for (int x = node::dimension.left; x < node::dimension.right; ++x)
					buf.at(y, x).foreground = this->color;
		}

	private:
		COLOR color = COLOR::DEFAULT;
	};

	class vertical_container final : public base::component {
	public:
		vertical_container(std::vector<std::shared_ptr<component>> components) :
			components(std::move(components))
		{
		}

		void focused(bool flag) override {
			component::focused(flag);
			this->components.at(this->focusedComponent)->focused(flag);
		}
		bool onkey(KEY_EVENT_RECORD key) override {
			auto& focused = this->components.at(this->focusedComponent);

			if (focused->onkey(key))
				return true;

			if ((key.dwControlKeyState & SHIFT_PRESSED) && key.wVirtualKeyCode == VK_TAB) {
				if (this->focusedComponent > 0) {
					focused->focused(false);
					this->components.at(--this->focusedComponent)->focused(true);
					return true;
				}
			}
			else {
				if (key.wVirtualKeyCode == VK_DOWN || key.wVirtualKeyCode == VK_TAB || key.uChar.AsciiChar == 'j') {
					if (this->focusedComponent < this->components.size() - 1) {
						focused->focused(false);
						this->components.at(++this->focusedComponent)->focused(true);
						return true;
					}
				}

				if (key.wVirtualKeyCode == VK_UP || key.uChar.AsciiChar == 'k') {
					if (this->focusedComponent > 0) {
						focused->focused(false);
						this->components.at(--this->focusedComponent)->focused(true);
						return true;
					}
				}
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
		{
		}

		void focused(bool flag) override {
			component::focused(flag);
			this->components.at(this->focusedComponent)->focused(flag);
		}
		bool onkey(KEY_EVENT_RECORD key) override {
			auto& focused = this->components.at(this->focusedComponent);

			if (focused->onkey(key))
				return true;

			if ((key.dwControlKeyState & SHIFT_PRESSED) && key.wVirtualKeyCode == VK_TAB) {
				if (this->focusedComponent > 0) {
					focused->focused(false);
					this->components.at(--this->focusedComponent)->focused(true);
					return true;
				}
			}
			else {
				if (key.wVirtualKeyCode == VK_RIGHT || key.wVirtualKeyCode == VK_TAB || key.uChar.AsciiChar == 'l') {
					if (this->focusedComponent < this->components.size() - 1) {
						focused->focused(false);
						this->components.at(++this->focusedComponent)->focused(true);
						return true;
					}
				}

				if (key.wVirtualKeyCode == VK_LEFT || key.uChar.AsciiChar == 'h') {
					if (this->focusedComponent > 0) {
						focused->focused(false);
						this->components.at(--this->focusedComponent)->focused(true);
						return true;
					}
				}
			}

			return false;
		}


	private:
		size_t focusedComponent = 0;
		std::vector<std::shared_ptr<component>> components;
	};
	class button final : public base::node, public base::component {
	public:
		button(std::string name, std::function<void()> logic) :
			name(name),
			logic(logic)
		{
		}

		void init() override {
			node::width = int(this->name.size()) + 4;
			node::height = 1;
		}
		void render(buffer& buf) override {
			if (component::focused()) {
				for (int y = node::dimension.top; y < node::dimension.bottom; ++y)
					for (int x = node::dimension.left; x < node::dimension.right; ++x)
						buf.at(y, x).invert = true;
			}

			buf.at(node::dimension.top, node::dimension.left).value = '[';
			buf.at(node::dimension.top, node::dimension.right - 1).value = ']';
			for (int y = node::dimension.top, i = 0; y < node::dimension.bottom; ++y)
				for (int x = node::dimension.left + 2; x < node::dimension.right - 2; ++x, ++i)
					buf.at(y, x).value = this->name.at(i);
		}

		bool onkey(KEY_EVENT_RECORD key) override {
			switch (key.wVirtualKeyCode) {
			case VK_RETURN:
				this->logic();
				return true;
			}

			return false;
		}

	private:
		std::string name;
		std::function<void()> logic;
	};
	class input final : public base::node, public base::component {
	public:
		input(std::string placeholder) :
			placeholder(placeholder)
		{
		}
		const std::string getValue() {
			return this->value;
		}

		void init() override {
			if (node::width == 0)
				node::width = 30;
			if (node::height == 0)
				node::height = 1;
		}
		void render(buffer& buf) override {
			for (int y = node::dimension.top; y < node::dimension.bottom; ++y)
				for (int x = node::dimension.left; x < node::dimension.right; ++x)
					buf.at(y, x).invert = true;

			if (component::focused())
				buf.at(node::dimension.top + this->yCursor, node::dimension.left + this->xCursor).invert = false;

			if (this->value.empty() && !this->placeholder.empty()) {
				for (int y = node::dimension.top, i = 0; y < node::dimension.bottom; ++y) {
					for (int x = node::dimension.left; x < node::dimension.right; ++x, ++i) {
						if (i < this->placeholder.size()) {
							buf.at(y, x).italic = true;
							buf.at(y, x).value = this->placeholder.at(i);
						}
						else break;
					}
				}
			}
			else if (this->hide) {
				for (int y = node::dimension.top, i = this->textBegin; y < node::dimension.bottom; ++y)
					for (int x = node::dimension.left; x < node::dimension.right; ++x, ++i)
						if (i < this->value.size())
							buf.at(y, x).value = '*';
						else break;
			}
			else {
				for (int y = node::dimension.top, i = this->textBegin; y < node::dimension.bottom; ++y)
					for (int x = node::dimension.left; x < node::dimension.right; ++x, ++i)
						if (i < this->value.size())
							buf.at(y, x).value = this->value.at(i);
						else break;
			}
		}

		bool onkey(KEY_EVENT_RECORD key) override {
			auto moveCursor = [&](int y, int x) {
				if (x > 0) {
					if (this->xCursor < node::width - 1)
						++this->xCursor;
					else if (this->yCursor < node::height - 1) {
						this->xCursor = 0;
						++this->yCursor;
					}
					else {
						this->xCursor = 0;
						this->textBegin += node::width;
					}
				}
				else if (x < 0) {
					if (this->xCursor > 0)
						--this->xCursor;
					else if (this->yCursor > 0) {
						this->xCursor = node::width - 1;
						--this->yCursor;
					}
					else {
						this->xCursor = node::width - 1;
						this->textBegin -= node::width;
					}
				}

				if (y > 0) {
					if (this->yCursor < node::height - 1)
						++this->yCursor;
					else this->textBegin += node::width;
				}
				else if (y < 0) {
					if (this->yCursor > 0)
						--this->yCursor;
					else this->textBegin -= node::width;
				}
				};

			switch (key.wVirtualKeyCode) {
			case VK_LEFT:
				if (this->index > 0) {
					--this->index;
					moveCursor(0, -1);
					return true;
				}
				break;
			case VK_UP:
				if (this->index - node::width > 0) {
					this->index -= node::width;
					moveCursor(-1, 0);
					return true;
				}
				break;
			case VK_RIGHT:
				if (this->index < this->value.size()) {
					++this->index;
					moveCursor(0, 1);
					return true;
				}
				break;
			case VK_DOWN:
				if (this->index + node::width <= this->value.size()) {
					this->index += node::width;
					moveCursor(1, 0);
					return true;
				}
				break;
			case VK_BACK:
				if (this->index > 0) {
					this->value.erase(this->value.begin() + --this->index);
					moveCursor(0, -1);
					return true;
				}
				break;
			default:
				if (this->pattern(key.uChar.AsciiChar) && this->index < this->limit) {
					value.insert(value.begin() + index++, key.uChar.AsciiChar);
					moveCursor(0, 1);
					return true;
				}
			}

			return false;
		}

	public:
		bool hide = false;
		int limit = std::numeric_limits<int>::max();
		std::function<bool(int)> pattern = [](int ch) { return ch > 0x1F && ch < 0x7F; };

	private:
		std::string value;
		std::string placeholder;
		int index = 0;
		int textBegin = 0;
		int xCursor = 0;
		int yCursor = 0;
	};
	class dropdown final : public base::node, public base::component {
	public:
		dropdown(std::vector<std::string> values, std::string placeholder) :
			values(values),
			placeholder(placeholder)
		{
		}

		void init() override {
			if (node::width == 0)
				node::width = int(std::max_element(this->values.begin(), this->values.end(), [](const std::string& a, const std::string& b) { return a.size() < b.size(); })->size());
			node::height = component::focused() ? std::min(7, int(this->values.size())) : 1;
		}
		void render(buffer& buf) override {
			for (int y = node::dimension.top; y < node::dimension.bottom; ++y)
				for (int x = node::dimension.left; x < node::dimension.right; ++x)
					buf.at(y, x).invert = true;

			if (component::focused()) {
				for (int x = node::dimension.left, i = 0; x < node::dimension.right; ++x, ++i)
					buf.at(node::dimension.top + this->yCursor, x).invert = false;

				for (int y = node::dimension.top, i = this->textBegin; y < node::dimension.bottom; ++y, ++i)
					for (int x = node::dimension.left, ii = 0; x < node::dimension.right; ++x, ++ii)
						if (ii < this->values.at(i).size())
							buf.at(y, x).value = this->values.at(i).at(ii);
						else break;
			}
			else {
				if (this->selectedValue.empty() && !this->placeholder.empty()) {
					for (int y = node::dimension.top, i = 0; y < node::dimension.bottom; ++y) {
						for (int x = node::dimension.left; x < node::dimension.right; ++x, ++i) {
							if (i < this->placeholder.size()) {
								buf.at(y, x).italic = true;
								buf.at(y, x).value = this->placeholder.at(i);
							}
							else break;
						}
					}
				}
				else {
					for (int y = node::dimension.top, i = 0; y < node::dimension.bottom; ++y)
						for (int x = node::dimension.left; x < node::dimension.right; ++x, ++i)
							if (i < this->selectedValue.size())
								buf.at(y, x).value = this->selectedValue.at(i);
							else break;
				}
			}
		}

		bool onkey(KEY_EVENT_RECORD key) override {
			auto moveCursor = [&](int y) {
				if (y > 0) {
					if (this->yCursor < node::height - 1)
						++this->yCursor;
					else ++textBegin;
				}
				else if (y < 0) {
					if (this->yCursor > 0)
						--this->yCursor;
					else --textBegin;
				}
				};

			switch (key.wVirtualKeyCode) {
			case VK_DOWN:
				if (this->index < this->values.size() - 1) {
					++this->index;
					moveCursor(1);
					return true;
				}
				return false;
			case VK_UP:
				if (this->index > 0) {
					--this->index;
					moveCursor(-1);
					return true;
				}
				return false;
			case VK_RETURN:
				this->selectedValue = this->values.at(this->index);
				return false;
			default:
				switch (key.uChar.AsciiChar) {
				case 'j':
					if (this->index < this->values.size() - 1) {
						++this->index;
						moveCursor(1);
						return true;
					}
					return false;
				case 'k':
					if (this->index > 0) {
						--this->index;
						moveCursor(-1);
						return true;
					}
					return false;
				}
			}

			return false;
		}

	private:
		std::string selectedValue;
		std::string placeholder;
		std::vector<std::string> values;
		int index = 0;
		int yCursor = 0;
		int textBegin = 0;
	};
	class radio final : public base::node, public base::component, public base::check_button {
	public:
		radio(std::string name) :
			name(name)
		{
		}
		std::string getName() {
			return this->name;
		}

		void init() override {
			node::width = !this->name.empty() ? int(this->name.size()) + 3 : 3;
			node::height = 1;
		}
		void render(buffer& buf) override {
			buf.at(node::dimension.top, node::dimension.left).value = '(';
			buf.at(node::dimension.top, node::dimension.left + 2).value = ')';

			if (!this->name.empty())
				for (int y = node::dimension.top, i = 0; y < node::dimension.bottom; ++y)
					for (int x = node::dimension.left + 3; x < node::dimension.right; ++x, ++i)
						buf.at(y, x).value = this->name.at(i);

			if (component::focused())
				for (int y = node::dimension.top; y < node::dimension.bottom; ++y)
					for (int x = node::dimension.left; x < node::dimension.right; ++x)
						buf.at(y, x).invert = true;

			if (check_button::checked())
				buf.at(node::dimension.top, node::dimension.left + 1).value = '*';
		}

		bool onkey(KEY_EVENT_RECORD key) override {
			if (key.wVirtualKeyCode == VK_RETURN || key.uChar.AsciiChar == ' ') {
				if (check_button::group)
					check_button::group->clear();

				check_button::checked(true);
				return true;
			}

			return false;
		}

	private:
		std::string name;
	};
	class checkbox final : public base::node, public base::component, public base::check_button {
	public:
		checkbox(std::string name) :
			name(name)
		{
		}
		std::string getName() {
			return this->name;
		}

		void init() override {
			node::width = !this->name.empty() ? int(this->name.size()) + 3 : 3;
			node::height = 1;
		}
		void render(buffer& buf) override {
			buf.at(node::dimension.top, node::dimension.left).value = '[';
			buf.at(node::dimension.top, node::dimension.left + 2).value = ']';

			if (!this->name.empty())
				for (int y = node::dimension.top, i = 0; y < node::dimension.bottom; ++y)
					for (int x = node::dimension.left + 3; x < node::dimension.right; ++x, ++i)
						buf.at(y, x).value = this->name.at(i);

			if (component::focused())
				for (int y = node::dimension.top; y < node::dimension.bottom; ++y)
					for (int x = node::dimension.left; x < node::dimension.right; ++x)
						buf.at(y, x).invert = true;

			if (check_button::checked())
				buf.at(node::dimension.top, node::dimension.left + 1).value = '*';
		}

		bool onkey(KEY_EVENT_RECORD key) override {
			if (key.wVirtualKeyCode == VK_RETURN || key.uChar.AsciiChar == ' ') {
				check_button::checked(!check_button::checked());
				return true;
			}

			return false;
		}

	private:
		std::string name;
	};
}

std::shared_ptr<simple::base::node> operator |(
	std::shared_ptr<simple::base::node> left,
	std::function<std::shared_ptr<simple::base::node>(std::shared_ptr<simple::base::node>)> nodeStyle
	) {
	return nodeStyle(std::move(left));
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

std::shared_ptr<simple::base::node> border(std::shared_ptr<simple::base::node> target) {
	return std::make_shared<simple::border>(std::move(target));
}
std::shared_ptr<simple::base::node> background(std::shared_ptr<simple::base::node> target, simple::COLOR color) {
	return std::make_shared<simple::background>(std::move(target), color);
}
std::function<std::shared_ptr<simple::base::node>(std::shared_ptr<simple::base::node>)> background(simple::COLOR color) {
	return [color](std::shared_ptr<simple::base::node> node) { return background(std::move(node), color); };
}
std::shared_ptr<simple::base::node> foreground(std::shared_ptr<simple::base::node> target, simple::COLOR color) {
	return std::make_shared<simple::foreground>(std::move(target), color);
}
std::function<std::shared_ptr<simple::base::node>(std::shared_ptr<simple::base::node>)> foreground(simple::COLOR color) {
	return [color](std::shared_ptr<simple::base::node> node) { return foreground(std::move(node), color); };
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
std::shared_ptr<simple::button> button(std::string name, std::function<void()> logic) {
	return std::make_shared<simple::button>(name, logic);
}
std::shared_ptr<simple::button> button(std::string name) {
	return std::make_shared<simple::button>(name, []() {});
}
std::shared_ptr<simple::input> input(std::string placeholder) {
	return std::make_shared<simple::input>(placeholder);
}
std::shared_ptr<simple::input> input() {
	return std::make_shared<simple::input>("");
}
std::shared_ptr<simple::dropdown> dropdown(std::vector<std::string> values, std::string placeholder) {
	return std::make_shared<simple::dropdown>(values, placeholder);
}
std::shared_ptr<simple::dropdown> dropdown(std::vector<std::string> values) {
	return std::make_shared<simple::dropdown>(values, "");
}
std::shared_ptr<simple::radio> radio(std::string name) {
	return std::make_shared<simple::radio>(name);
}
std::shared_ptr<simple::radio> radio() {
	return std::make_shared<simple::radio>("");
}
std::shared_ptr<simple::checkbox> checkbox(std::string name) {
	return std::make_shared<simple::checkbox>(name);
}
std::shared_ptr<simple::checkbox> checkbox() {
	return std::make_shared<simple::checkbox>("");
}

#endif