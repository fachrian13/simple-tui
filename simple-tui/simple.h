#ifndef _SIMPLE_TUI_
#define _SIMPLE_TUI_
#define NOMINMAX

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
			virtual void OnKey(KEY_EVENT_RECORD) {}

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
		void OnKey(KEY_EVENT_RECORD) override {}
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
		void OnKey(KEY_EVENT_RECORD) override {}
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
		}

	private:
		std::string name;
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

#endif