#ifndef _SIMPLE_TUI_
#define _SIMPLE_TUI_
#define NOMINMAX

#include <functional>
#include <memory>
#include <string>
#include <vector>
#include <windows.h>

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
		bool Bold = false;
		bool Dim = false;
		bool Italic = false;
		bool Underline = false;
		bool Blink = false;
		bool Invert = false;
		bool Invisible = false;
		bool Strikethrough = false;
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
					result += p.Bold ? "1;" : "22;";
					result += p.Dim ? "2;" : "22;";
					result += p.Italic ? "3;" : "23;";
					result += p.Underline ? "4;" : "24;";
					result += p.Blink ? "5;" : "25;";
					result += p.Invert ? "7;" : "27;";
					result += p.Invisible ? "8;" : "28;";
					result += p.Strikethrough ? "9;" : "29;";
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
		void Clear() {
			this->pixels = std::vector<Pixel>(this->width * this->height, this->style);
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
		class Renderable {
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
		class Focusable {
		public:
			bool Focused() {
				return this->focused;
			}
			virtual void Focused(bool flag) {
				this->focused = flag;
			}
			virtual bool OnKey(KEY_EVENT_RECORD) {
				return false;
			}

		private:
			bool focused = false;
		};
	}
	namespace Utils {
		template<class Type, class... Args>
		std::vector<Type> ToVector(Args&&... value) {
			return std::vector<Type>{std::forward<Type>(value)...};
		}
	}

	class VerticalLayout final : public Base::Renderable {
	public:
		VerticalLayout(std::vector<std::shared_ptr<Renderable>>&& objects) :
			objects(std::move(objects))
		{
		}

		void Init() override {
			for (const auto& object : this->objects) {
				object->Init();

				Renderable::Width = std::max(Renderable::Width, object->Width);
				Renderable::Height += object->Height;
			}
		}
		void Set(Rect dimension) override {
			Renderable::Set(dimension);

			int t = Renderable::Dimension.Top;
			for (const auto& object : this->objects) {
				int l = Renderable::Dimension.Left;
				int r = l + object->Width;
				int b = t + object->Height;

				object->Set({ l, t, r, b });

				t = b;
			}
		}
		void Render(Buffer& buffer) override {
			for (const auto& object : this->objects)
				object->Render(buffer);
		}

	private:
		std::vector<std::shared_ptr<Renderable>> objects;
	};
	class HorizontalLayout final : public Base::Renderable {
	public:
		HorizontalLayout(std::vector<std::shared_ptr<Renderable>>&& objects) :
			objects(std::move(objects))
		{
		}

		void Init() override {
			for (const auto& object : this->objects) {
				object->Init();

				Renderable::Width += object->Width;
				Renderable::Height = std::max(Renderable::Height, object->Height);
			}
		}
		void Set(Rect dimension) override {
			Renderable::Set(dimension);

			int l = Renderable::Dimension.Left;
			for (const auto& object : this->objects) {
				int t = Renderable::Dimension.Top;
				int r = l + object->Width;
				int b = t + object->Height;

				object->Set({ l, t, r, b });

				l = r;
			}
		}
		void Render(Buffer& buffer) override {
			for (const auto& object : this->objects)
				object->Render(buffer);
		}

	private:
		std::vector<std::shared_ptr<Renderable>> objects;
	};
	class Text final : public Base::Renderable {
	public:
		Text(std::string&& value) :
			value(std::move(value))
		{
		}

		void Init() override {
			Renderable::Width = int(this->value.size());
			Renderable::Height = 1;
		}
		void Render(Buffer& buffer) override {
			for (int y = Renderable::Dimension.Top, i = 0; y < Renderable::Dimension.Bottom; ++y)
				for (int x = Renderable::Dimension.Left; x < Renderable::Dimension.Right; ++x, ++i)
					buffer.At(y, x).Value = value.at(i);
		}

	private:
		std::string value;
	};

	class VerticalContainer final : public Base::Focusable {
	public:
		VerticalContainer(std::vector<std::shared_ptr<Focusable>>&& objects) :
			objects(std::move(objects))
		{
		}

		void Focused(bool flag) override {
			Focusable::Focused(flag);

			this->objects.at(focusedObject)->Focused(flag);
		}
		bool OnKey(KEY_EVENT_RECORD key) override {
			auto& focused = this->objects.at(this->focusedObject);

			if (focused->OnKey(key))
				return true;

			if ((key.dwControlKeyState & SHIFT_PRESSED) && key.wVirtualKeyCode == VK_TAB) {
				if (this->focusedObject > 0) {
					focused->Focused(false);
					this->objects.at(--this->focusedObject)->Focused(true);
					return true;
				}
			}
			else {
				if (key.wVirtualKeyCode == VK_DOWN || key.wVirtualKeyCode == VK_TAB || key.uChar.AsciiChar == 'j') {
					if (this->focusedObject < this->objects.size() - 1) {
						focused->Focused(false);
						this->objects.at(++this->focusedObject)->Focused(true);
						return true;
					}
				}

				if (key.wVirtualKeyCode == VK_UP || key.uChar.AsciiChar == 'k') {
					if (this->focusedObject > 0) {
						focused->Focused(false);
						this->objects.at(--this->focusedObject)->Focused(true);
						return true;
					}
				}
			}

			return false;
		}
	private:
		size_t focusedObject = 0;
		std::vector<std::shared_ptr<Focusable>> objects;
	};
	class HorizontalContainer final : public Base::Focusable {
	public:
		HorizontalContainer(std::vector<std::shared_ptr<Focusable>>&& objects) :
			objects(std::move(objects))
		{
		}

		void Focused(bool flag) override {
			Focusable::Focused(flag);

			this->objects.at(focusedObject)->Focused(flag);
		}
		bool OnKey(KEY_EVENT_RECORD key) override {
			auto& focused = this->objects.at(this->focusedObject);

			if (focused->OnKey(key))
				return true;

			if ((key.dwControlKeyState & SHIFT_PRESSED) && key.wVirtualKeyCode == VK_TAB) {
				if (this->focusedObject > 0) {
					focused->Focused(false);
					this->objects.at(--this->focusedObject)->Focused(true);
					return true;
				}
			}
			else {
				if (key.wVirtualKeyCode == VK_RIGHT || key.wVirtualKeyCode == VK_TAB || key.uChar.AsciiChar == 'l') {
					if (this->focusedObject < this->objects.size() - 1) {
						focused->Focused(false);
						this->objects.at(++this->focusedObject)->Focused(true);
						return true;
					}
				}

				if (key.wVirtualKeyCode == VK_LEFT || key.uChar.AsciiChar == 'h') {
					if (this->focusedObject > 0) {
						focused->Focused(false);
						this->objects.at(--this->focusedObject)->Focused(true);
						return true;
					}
				}
			}

			return false;
		}
	private:
		size_t focusedObject = 0;
		std::vector<std::shared_ptr<Focusable>> objects;
	};
	class Button final : public Base::Renderable, public Base::Focusable {
	public:
		Button(std::string&& name) :
			name(std::move(name))
		{
		}
		Button(std::string&& name, std::function<void()>&& logic) :
			name(std::move(name)),
			logic(std::move(logic))
		{
		}

		void Init() override {
			Renderable::Width = int(this->name.size()) + 2;
			Renderable::Height = 1;
		}
		void Render(Buffer& buffer) override {
			buffer.At(Renderable::Dimension.Top, Renderable::Dimension.Left).Value = "[";
			buffer.At(Renderable::Dimension.Top, Renderable::Dimension.Right - 1).Value = "]";
			for (int y = Renderable::Dimension.Top, i = 0; y < Renderable::Dimension.Bottom; ++y)
				for (int x = Renderable::Dimension.Left + 1; x < Renderable::Dimension.Right - 1; ++x, ++i)
					buffer.At(y, x).Value = this->name.at(i);

			if (Focusable::Focused())
				for (int y = Renderable::Dimension.Top; y < Renderable::Dimension.Bottom; ++y)
					for (int x = Renderable::Dimension.Left; x < Renderable::Dimension.Right; ++x)
						buffer.At(y, x).Invert = true;
		}

		bool OnKey(KEY_EVENT_RECORD key) override {
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
	class Input final : public Base::Renderable, public Base::Focusable {
	public:
		Input() = default;
		Input(std::string&& placeholder) :
			placeholder(std::move(placeholder))
		{
		}

		void Init() override {
			if (Renderable::Width == 0)
				Renderable::Width = 30;
			if (Renderable::Height == 0)
				Renderable::Height = 1;
		}
		void Render(Buffer& buf) override {
			for (int y = Renderable::Dimension.Top; y < Renderable::Dimension.Bottom; ++y)
				for (int x = Renderable::Dimension.Left; x < Renderable::Dimension.Right; ++x)
					buf.At(y, x).Invert = true;

			if (Focusable::Focused())
				buf.At(Renderable::Dimension.Top + this->yCursor, Renderable::Dimension.Left + this->xCursor).Invert = false;

			if (this->value.empty() && !this->placeholder.empty()) {
				for (int y = Renderable::Dimension.Top, i = 0; y < Renderable::Dimension.Bottom; ++y) {
					for (int x = Renderable::Dimension.Left; x < Renderable::Dimension.Right; ++x, ++i) {
						if (i < this->placeholder.size()) {
							buf.At(y, x).Italic = true;
							buf.At(y, x).Value = this->placeholder.at(i);
						}
						else break;
					}
				}
			}
			else if (this->Hide) {
				for (int y = Renderable::Dimension.Top, i = this->textBegin; y < Renderable::Dimension.Bottom; ++y)
					for (int x = Renderable::Dimension.Left; x < Renderable::Dimension.Right; ++x, ++i)
						if (i < this->value.size())
							buf.At(y, x).Value = u8"•";
						else break;
			}
			else {
				for (int y = Renderable::Dimension.Top, i = this->textBegin; y < Renderable::Dimension.Bottom; ++y)
					for (int x = Renderable::Dimension.Left; x < Renderable::Dimension.Right; ++x, ++i)
						if (i < this->value.size())
							buf.At(y, x).Value = this->value.at(i);
						else break;
			}
		}

		bool OnKey(KEY_EVENT_RECORD key) override {
			auto moveCursor = [&](int y, int x) {
				if (x > 0) {
					if (this->xCursor < Renderable::Width - 1)
						++this->xCursor;
					else if (this->yCursor < Renderable::Height - 1) {
						this->xCursor = 0;
						++this->yCursor;
					}
					else {
						this->xCursor = 0;
						this->textBegin += Renderable::Width;
					}
				}
				else if (x < 0) {
					if (this->xCursor > 0)
						--this->xCursor;
					else if (this->yCursor > 0) {
						this->xCursor = Renderable::Width - 1;
						--this->yCursor;
					}
					else {
						this->xCursor = Renderable::Width - 1;
						this->textBegin -= Renderable::Width;
					}
				}

				if (y > 0) {
					if (this->yCursor < Renderable::Height - 1)
						++this->yCursor;
					else this->textBegin += Renderable::Width;
				}
				else if (y < 0) {
					if (this->yCursor > 0)
						--this->yCursor;
					else this->textBegin -= Renderable::Width;
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
				if (this->index - Renderable::Width >= 0) {
					this->index -= Renderable::Width;
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
				if (this->index + Renderable::Width <= this->value.size()) {
					this->index += Renderable::Width;
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
				if (this->Pattern(key.uChar.AsciiChar) && this->index < this->Limit) {
					value.insert(value.begin() + index++, key.uChar.AsciiChar);
					moveCursor(0, 1);
					return true;
				}
			}

			return false;
		}

	public:
		bool Hide = false;
		int Limit = std::numeric_limits<int>::max();
		std::function<bool(int)> Pattern = [](int ch) { return ch > 0x1F && ch < 0x7F; };

	private:
		std::string value;
		std::string placeholder;
		int index = 0;
		int textBegin = 0;
		int xCursor = 0;
		int yCursor = 0;;
	};
}

template<class Type, class... Args>
std::shared_ptr<Simple::Base::Renderable> VLayout(Type&& object, Args&&... objects) {
	return std::make_shared<Simple::VerticalLayout>(
		Simple::Utils::ToVector<std::shared_ptr<Simple::Base::Renderable>>(
			std::forward<Type>(object), std::forward<Args>(objects)...
		)
	);
}
template<class Type, class... Args>
std::shared_ptr<Simple::Base::Renderable> HLayout(Type&& object, Args&&... objects) {
	return std::make_shared<Simple::HorizontalLayout>(
		Simple::Utils::ToVector<std::shared_ptr<Simple::Base::Renderable>>(
			std::forward<Type>(object), std::forward<Args>(objects)...
		)
	);
}
std::shared_ptr<Simple::Base::Renderable> Text(std::string value) {
	return std::make_shared<Simple::Text>(std::move(value));
}

template<class Type, class... Args>
std::shared_ptr<Simple::Base::Focusable> VContainer(Type&& object, Args&&... objects) {
	return std::make_shared<Simple::VerticalContainer>(
		Simple::Utils::ToVector<std::shared_ptr<Simple::Base::Focusable>>(
			std::forward<Type>(object), std::forward<Args>(objects)...
		)
	);
}
template<class Type, class... Args>
std::shared_ptr<Simple::Base::Focusable> HContainer(Type&& object, Args&&... objects) {
	return std::make_shared<Simple::HorizontalContainer>(
		Simple::Utils::ToVector<std::shared_ptr<Simple::Base::Focusable>>(
			std::forward<Type>(object), std::forward<Args>(objects)...
		)
	);
}
std::shared_ptr<Simple::Button> Button(std::string value) {
	return std::make_shared<Simple::Button>(std::move(value));
}
std::shared_ptr<Simple::Button> Button(std::string value, std::function<void()> logic) {
	return std::make_shared<Simple::Button>(std::move(value), std::move(logic));
}
std::shared_ptr<Simple::Input> Input() {
	return std::make_shared<Simple::Input>();
}
std::shared_ptr<Simple::Input> Input(std::string placeholder) {
	return std::make_shared<Simple::Input>(std::move(placeholder));
}

#endif