#ifndef _SIMPLE_TUI_
#define _SIMPLE_TUI_
#define NOMINMAX

#include <functional>
#include <memory>
#include <sstream>
#include <string>
#include <vector>
#include <windows.h>

namespace Simple {
	enum class Palette16 : int {
		Black = 30,
		Red = 31,
		Green = 32,
		Yellow = 33,
		Blue = 34,
		Magenta = 35,
		Cyan = 36,
		White = 37,
		Default = 39,
		Reset = 0,
		BrightBlack = 90,
		BrightRed = 91,
		BrightGreen = 92,
		BrightYellow = 93,
		BrightBlue = 94,
		BrightMagenta = 95,
		BrightCyan = 96,
		BrightWhite = 97
	};
	enum class Palette256 : int { /* THANKS TO CHATGPT */
		Black = 0,
		Red = 1,
		Green = 2,
		Yellow = 3,
		Blue = 4,
		Magenta = 5,
		Cyan = 6,
		White = 7,
		Gray = 8,
		LightRed = 9,
		LightGreen = 10,
		LightYellow = 11,
		LightBlue = 12,
		LightMagenta = 13,
		LightCyan = 14,
		LightWhite = 15,
		DeepRed = 16,
		RustRed = 17,
		BurntOrange = 18,
		Goldenrod = 19,
		OliveGreen = 20,
		MossGreen = 21,
		TealGreen = 22,
		SeafoamGreen = 23,
		SkyBlue = 24,
		OceanBlue = 25,
		RoyalBlue = 26,
		Violet = 27,
		Purple = 28,
		Lavender = 29,
		Plum = 30,
		Fuchsia = 31,
		EmeraldGreen = 32,
		LimeGreen = 33,
		BrightYellow = 34,
		Sunflower = 35,
		Amber = 36,
		Copper = 37,
		Saffron = 38,
		EmeraldIsle = 39,
		Turquoise = 40,
		Periwinkle = 41,
		CobaltBlue = 42,
		MidnightBlue = 43,
		NavyBlue = 44,
		SlateBlue = 45,
		Indigo = 46,
		MagentaRose = 47,
		CherryRed = 48,
		Coral = 49,
		SalmonPink = 50,
		Peach = 51,
		HoneyYellow = 52,
		LemonYellow = 53,
		GoldenYellow = 54,
		Chartreuse = 55,
		CeleryGreen = 56,
		LightSeaGreen = 57,
		MintGreen = 58,
		ForestGreen = 59,
		DarkTeal = 60,
		CypressGreen = 61,
		PineGreen = 62,
		OliveDrab = 63,
		SandBrown = 64,
		SiennaBrown = 65,
		Cinnamon = 66,
		EarthBrown = 67,
		Chestnut = 68,
		Mahogany = 69,
		Burgundy = 70,
		Grape = 71,
		LavenderBlush = 72,
		SoftViolet = 73,
		DeepPurple = 74,
		HeatherPurple = 75,
		Mulberry = 76,
		Mauve = 77,
		Pink = 78,
		CottonCandy = 79,
		BlushPink = 80,
		HotPink = 81,
		DeepPink = 82,
		BrightMagenta = 83,
		VividFuchsia = 84,
		Crimson = 85,
		Scarlet = 86,
		Firebrick = 87,
		DarkRed = 88,
		WineRed = 89,
		BloodRed = 90,
		BrickRed = 91,
		Tangerine = 92,
		Apricot = 93,
		Papaya = 94,
		GoldenBrown = 95,
		Ochre = 96,
		Mustard = 97,
		Khaki = 98,
		Sandstone = 99,
		Beige = 100,
		Wheat = 101,
		Honeydew = 102,
		Ivory = 103,
		Vanilla = 104,
		Almond = 105,
		LightPeach = 106,
		Cantaloupe = 107,
		Melon = 108,
		CoralPink = 109,
		PaleRose = 110,
		MistyRose = 111,
		BabyPink = 112,
		SoftPeach = 113,
		Peony = 114,
		CarnationPink = 115,
		DeepCarnation = 116,
		BalletSlipper = 117,
		Watermelon = 118,
		Fandango = 119,
		Ruby = 120,
		Sunset = 121,
		Daffodil = 122,
		GoldenHaze = 123,
		LemonChiffon = 124,
		LightAmber = 125,
		PapayaWhip = 126,
		BlanchedAlmond = 127,
		SaffronGold = 128,
		Tawny = 129,
		TuscanTan = 130,
		DesertSand = 131,
		Sepia = 132,
		Mocha = 133,
		Coffee = 134,
		Latte = 135,
		LightCocoa = 136,
		Chocolate = 137,
		Mushroom = 138,
		Pewter = 139,
		SlateGray = 140,
		Charcoal = 141,
		Ash = 142,
		Granite = 143,
		Gunmetal = 144,
		DarkSlate = 145,
		Lead = 146,
		Shadow = 147,
		Silver = 148,
		Steel = 149,
		Zinc = 150,
		Titanium = 151,
		PewterBlue = 152,
		Stone = 153,
		Moss = 154,
		Bamboo = 155,
		Citrus = 156,
		Olive = 157,
		Teal = 158,
		Seaweed = 159,
		MarineBlue = 160,
		Aquamarine = 161,
		TurquoiseGreen = 162,
		TropicalBlue = 163,
		ArcticBlue = 164,
		Blueberry = 165,
		Midnight = 166,
		DarkSapphire = 167,
		IndigoBlue = 168,
		Sapphire = 169,
		Cerulean = 170,
		PeriwinkleBlue = 171,
		LavenderBlue = 172,
		VioletBlue = 173,
		PurpleRain = 174,
		DaffodilBlue = 175,
		MauveBlue = 176,
		CosmicPurple = 177,
		GalaxyBlue = 178,
		FrozenBlue = 179,
		Aqua = 180,
		TropicalAqua = 181,
		LagoonBlue = 182,
		MysticGreen = 183,
		MistBlue = 184,
		FrostedBlue = 185,
		PeacockBlue = 186,
		DeepWaterBlue = 187,
		SkyBlueLight = 188,
		SeafoamBlue = 189,
		IceBlue = 190,
		FrozenLavender = 191,
		Cobalt = 192,
		BlueMoon = 193,
		RoyalPurple = 194,
		LightOrchid = 195,
		Lupine = 196,
		PlumPurple = 197,
		GrapeJam = 198,
		Purpleberry = 199,
		CrimsonRed = 200,
		FadedRed = 201,
		Firestorm = 202,
		Vermilion = 203,
		Rust = 204,
		BurntSienna = 205,
		CopperPenny = 206,
		SunsetOrange = 207,
		TangerineTango = 208,
		Flame = 209,
		Sunburst = 210,
		GoldenOrange = 211,
		Butterscotch = 212,
		AutumnOrange = 213,
		Pumpkin = 214,
		Melonade = 215,
		Marigold = 216,
		Flamingo = 217,
		Carnation = 218,
		StrawberryRed = 219,
		CherryBlossom = 220,
		LushGreen = 221,
		VividGreen = 222,
		ElectricGreen = 223,
		Lime = 224,
		Mint = 225,
		PastelGreen = 226,
		PaleEmerald = 227,
		Jade = 228,
		Verdant = 229,
		TurquoiseBlue = 230,
		DeepOcean = 231,
		CharcoalGray = 232,
		Graphite = 233,
		SmokyGray = 234,
		Ashen = 235,
		LeadGray = 236,
		Slate = 237,
		Foggy = 238,
		StoneGray = 239,
		Cinder = 240,
		Rustic = 241,
		Soot = 242,
		PewterGray = 243,
		GunmetalGray = 244,
		DarkCharcoal = 245,
		DeepSlate = 246,
		StormyGray = 247,
		DimGray = 248,
		NightGray = 249,
		Fog = 250,
		SmokyWhite = 251,
		Cloud = 252,
		Platinum = 253,
		SilverMist = 254,
		GhostWhite = 255
	};

	class Color final {
	public:
		Color(Palette16 value) :
			colorType(Type::Palette16),
			Red(static_cast<int>(value)),
			Green(static_cast<int>(value)),
			Blue(static_cast<int>(value))
		{
		}
		Color(Palette256 value) :
			colorType(Type::Palette256),
			Red(static_cast<int>(value)),
			Green(static_cast<int>(value)),
			Blue(static_cast<int>(value))
		{
		}
		Color(int red, int green, int blue) :
			colorType(Type::RGB),
			Red(red),
			Green(green),
			Blue(blue)
		{
		}
		Color(std::string hex) {
			if (hex.size() != 7 || hex[0] != '#')
				return;

			this->colorType = Type::RGB;
			this->Red = std::stoi(hex.substr(1, 2), nullptr, 16); // Red
			this->Green = std::stoi(hex.substr(3, 2), nullptr, 16); // Green
			this->Blue = std::stoi(hex.substr(5, 2), nullptr, 16); // Blue
		}
		std::string Foreground() {
			std::ostringstream ostr;

			switch (this->colorType) {
			case Type::Palette16:
				ostr << "\x1b[" << this->Red << "m";
				break;
			case Type::Palette256:
				ostr << "\x1b[38;5;" << this->Red << "m";
				break;
			case Type::RGB:
				ostr << "\x1b[38;2;" << this->Red << ";" << this->Green << ";" << this->Blue << "m";
				break;
			}

			return ostr.str();
		}
		void Foreground(std::ostringstream& ostr) {
			switch (this->colorType) {
			case Type::Palette16:
				ostr << "\x1b[" << this->Red << "m";
				break;
			case Type::Palette256:
				ostr << "\x1b[38;5;" << this->Red << "m";
				break;
			case Type::RGB:
				ostr << "\x1b[38;2;" << this->Red << ";" << this->Green << ";" << this->Blue << "m";
				break;
			}
		}
		std::string Background() {
			std::ostringstream ostr;

			switch (this->colorType) {
			case Type::Palette16:
				ostr << "\x1b[" << this->Red + 10 << "m";
				break;
			case Type::Palette256:
				ostr << "\x1b[48;5;" << this->Red << "m";
				break;
			case Type::RGB:
				ostr << "\x1b[48;2;" << this->Red << ";" << this->Green << ";" << this->Blue << "m";
				break;
			}

			return ostr.str();
		}
		void Background(std::ostringstream& ostr) {
			switch (this->colorType) {
			case Type::Palette16:
				ostr << "\x1b[" << this->Red + 10 << "m";
				break;
			case Type::Palette256:
				ostr << "\x1b[48;5;" << this->Red << "m";
				break;
			case Type::RGB:
				ostr << "\x1b[48;2;" << this->Red << ";" << this->Green << ";" << this->Blue << "m";
				break;
			}
		}
		bool operator ==(const Color& other) {
			return this->Red == other.Red && this->Green == other.Green && this->Blue == other.Blue;
		}
		bool operator !=(const Color& other) {
			return !(*this == other);
		}

	public:
		int Red = 0;
		int Green = 0;
		int Blue = 0;

	private:
		enum class Type {
			Palette16,
			Palette256,
			RGB
		};
		Type colorType = Type::Palette16;
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
		bool operator ==(const Pixel& other) {
			return
				this->Bold == other.Bold &&
				this->Dim == other.Dim &&
				this->Italic == other.Italic &&
				this->Underline == other.Underline &&
				this->Blink == other.Blink &&
				this->Invert == other.Invert &&
				this->Invisible == other.Invisible &&
				this->Strikethrough == other.Strikethrough &&
				this->Background == other.Background &&
				this->Foreground == other.Foreground;
		}
		bool operator !=(const Pixel& other) {
			return !(*this == other);
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
		Color Background = Palette16::Default;
		Color Foreground = Palette16::Default;
		std::string Value = " ";
	};
	class Buffer final {
	public:
		Buffer() = default;
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
			static Pixel defaultPixel;

			if (y < height && x < width)
				return this->pixels.at(y * this->width + x);

			return defaultPixel;
		}
		std::string ToString() {
			std::ostringstream result;
			Pixel prev;

			for (int y = 0; y < this->height; ++y) {
				if (y > 0)
					result << "\n";

				for (int x = 0; x < this->width; ++x) {
					Pixel& next = this->pixels.at(y * this->width + x);

					if (prev.Bold != next.Bold)
						result << (next.Bold ? "\x1b[1m" : "\x1b[22m");
					if (prev.Dim != next.Dim)
						result << (next.Dim ? "\x1b[2m" : "\x1b[22m");
					if (prev.Italic != next.Italic)
						result << (next.Italic ? "\x1b[3m" : "\x1b[23m");
					if (prev.Underline != next.Underline)
						result << (next.Underline ? "\x1b[4m" : "\x1b[24m");
					if (prev.Blink != next.Blink)
						result << (next.Blink ? "\x1b[5m" : "\x1b[25m");
					if (prev.Invert != next.Invert)
						result << (next.Invert ? "\x1b[7m" : "\x1b[27m");
					if (prev.Invisible != next.Invisible)
						result << (next.Invisible ? "\x1b[8m" : "\x1b[28m");
					if (prev.Strikethrough != next.Strikethrough)
						result << (next.Strikethrough ? "\x1b[9m" : "\x1b[29m");
					if (prev.Foreground != next.Foreground)
						next.Foreground.Foreground(result);
					if (prev.Background != next.Background)
						next.Background.Background(result);

					result << next.Value;

					prev = next;
				}
			}
			result << "\x1b[m";

			return result.str();
		}
		const Pixel& GetStyle() const {
			return this->style;
		}
		const int& GetWidth() const {
			return this->width;
		}
		const int& GetHeight() const {
			return this->height;
		}
		void Clear() {
			std::fill(this->pixels.begin(), this->pixels.end(), this->style);
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

	class SelectedGroup;
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
				return this->$;
			}
			virtual void Focused(bool flag) {
				this->$ = flag;
			}
			virtual bool OnKey(KEY_EVENT_RECORD) {
				return false;
			}

		private:
			bool $ = false;
		};
		class Selectable {
		public:
			Selectable() = default;
			Selectable(std::string name) :
				Name(std::move(name))
			{
			}
			const bool& Selected() const {
				return this->$;
			}
			virtual void Selected(bool flag) {
				this->$ = flag;
			}
			void SetGroup(SelectedGroup* group) {
				this->Group = group;
			}
			const std::string& GetName() const {
				return this->Name;
			}

		protected:
			std::string Name;
			SelectedGroup* Group = nullptr;

		private:
			bool $ = false;
		};
		class Modifier : public Renderable {
		public:
			Modifier(std::shared_ptr<Renderable>&& object) :
				Object(std::move(object))
			{
			}

			virtual void Init() override {
				this->Object->Init();
			}
			virtual void Set(Rect dimension) override {
				this->Object->Set(dimension);
			}
			virtual void Render(Buffer& buffer) override {
				this->Object->Render(buffer);
			}

		protected:
			std::shared_ptr<Renderable> Object;
		};
	}
	namespace Utils {
		class RowType {
		public:
			std::string Left;
			std::string Middle;
			std::string Right;
		};
		class BorderStyle {
		public:
			std::string Horizontal;
			std::string Vertical;
			RowType Top;
			RowType Middle;
			RowType Bottom;
		};

		template<class Type, class... Args>
		std::vector<Type> ToVector(Args&&... value) {
			return std::vector<Type>{std::forward<Type>(value)...};
		}
	}

	class SelectedGroup final {
	public:
		SelectedGroup(std::vector<std::shared_ptr<Base::Selectable>>&& objects) {
			for (auto& object : objects) {
				object->SetGroup(this);
				this->objects.push_back(std::move(object));
			}
		}
		void Clear() {
			for (const auto& object : this->objects)
				object->Selected(false);
		}
		const std::shared_ptr<Base::Selectable>& Selected() const {
			for (const auto& object : this->objects)
				if (object->Selected())
					return object;
		}

	private:
		std::vector<std::shared_ptr<Base::Selectable>> objects;
	};

	class VerticalLayout final : public Base::Renderable {
	public:
		VerticalLayout(std::vector<std::shared_ptr<Renderable>>&& objects) :
			objects(std::move(objects))
		{
		}

		void Init() override {
			Renderable::Width = 0;
			Renderable::Height = 0;

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
			Renderable::Width = 0;
			Renderable::Height = 0;

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
		Text(std::string value) :
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
					buffer.At(y, x).Value = this->value.at(i);
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
		Button(std::string name) :
			name(std::move(name))
		{
		}
		Button(std::string name, std::function<void()> logic) :
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
				if (this->logic) {
					this->logic();
					return true;
				}
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
		Input(std::string placeholder) :
			placeholder(std::move(placeholder))
		{
		}

		void Init() override {
			if (Renderable::Width == 0)
				Renderable::Width = 30;
			if (Renderable::Height == 0)
				Renderable::Height = 1;
		}
		void Render(Buffer& buffer) override {
			for (int y = Renderable::Dimension.Top; y < Renderable::Dimension.Bottom; ++y)
				for (int x = Renderable::Dimension.Left; x < Renderable::Dimension.Right; ++x)
					buffer.At(y, x).Invert = true;

			if (Focusable::Focused())
				buffer.At(Renderable::Dimension.Top + this->yCursor, Renderable::Dimension.Left + this->xCursor).Invert = false;

			if (this->value.empty() && !this->placeholder.empty()) {
				for (int y = Renderable::Dimension.Top, i = 0; y < Renderable::Dimension.Bottom; ++y) {
					for (int x = Renderable::Dimension.Left; x < Renderable::Dimension.Right; ++x, ++i) {
						if (i < this->placeholder.size()) {
							buffer.At(y, x).Italic = true;
							buffer.At(y, x).Value = this->placeholder.at(i);
						}
						else break;
					}
				}
			}
			else if (this->Hide) {
				for (int y = Renderable::Dimension.Top, i = this->textBegin; y < Renderable::Dimension.Bottom; ++y)
					for (int x = Renderable::Dimension.Left; x < Renderable::Dimension.Right; ++x, ++i)
						if (i < this->value.size())
							buffer.At(y, x).Value = u8"•";
						else break;
			}
			else {
				for (int y = Renderable::Dimension.Top, i = this->textBegin; y < Renderable::Dimension.Bottom; ++y)
					for (int x = Renderable::Dimension.Left; x < Renderable::Dimension.Right; ++x, ++i)
						if (i < this->value.size())
							buffer.At(y, x).Value = this->value.at(i);
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
	class Dropdown final : public Base::Renderable, public Base::Focusable {
	public:
		Dropdown(std::vector<std::string> values) :
			values(std::move(values))
		{
		}
		Dropdown(std::vector<std::string> values, std::string placeholder) :
			values(std::move(values)),
			placeholder(std::move(placeholder))
		{
		}

		void Init() override {
			if (Renderable::Width == 0)
				Renderable::Width = int(std::max_element(
					this->values.begin(),
					this->values.end(),
					[](const std::string& a, const std::string& b) {
						return a.size() < b.size();
					}
				)->size()) + 1;
			Renderable::Height = Focusable::Focused() ? std::min(7, int(this->values.size())) : 1;
		}
		void Render(Buffer& buffer) override {
			for (int y = Renderable::Dimension.Top; y < Renderable::Dimension.Bottom; ++y)
				for (int x = Renderable::Dimension.Left; x < Renderable::Dimension.Right; ++x)
					buffer.At(y, x).Invert = true;

			if (Focusable::Focused()) {
				for (int x = Renderable::Dimension.Left, i = 0; x < Renderable::Dimension.Right; ++x, ++i)
					buffer.At(Renderable::Dimension.Top + this->yCursor, x).Invert = false;

				for (int y = Renderable::Dimension.Top, i = this->textBegin; y < Renderable::Dimension.Bottom; ++y, ++i) {
					for (int x = Renderable::Dimension.Left + 1, ii = 0; x < Renderable::Dimension.Right; ++x, ++ii)
						if (ii < this->values.at(i).size())
							buffer.At(y, x).Value = this->values.at(i).at(ii);
						else break;

					if (i == this->selectedIndex)
						buffer.At(y, Renderable::Dimension.Left).Value = u8"*";
				}
			}
			else {
				if (this->selectedValue.empty() && !this->placeholder.empty()) {
					for (int y = Renderable::Dimension.Top, i = 0; y < Renderable::Dimension.Bottom; ++y) {
						for (int x = Renderable::Dimension.Left; x < Renderable::Dimension.Right; ++x, ++i) {
							if (i < this->placeholder.size()) {
								buffer.At(y, x).Italic = true;
								buffer.At(y, x).Value = this->placeholder.at(i);
							}
							else break;
						}
					}
				}
				else {
					for (int y = Renderable::Dimension.Top, i = 0; y < Renderable::Dimension.Bottom; ++y)
						for (int x = Renderable::Dimension.Left; x < Renderable::Dimension.Right; ++x, ++i)
							if (i < this->selectedValue.size())
								buffer.At(y, x).Value = this->selectedValue.at(i);
							else break;
				}
			}
		}

		bool OnKey(KEY_EVENT_RECORD key) override {
			auto moveCursor = [&](int y) {
				if (y > 0) {
					if (this->yCursor < Renderable::Height - 1)
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
				this->selectedIndex = this->index;
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
		int selectedIndex = -1;
		int yCursor = 0;
		int textBegin = 0;
	};
	class Radio final : public Base::Renderable, public Base::Focusable, public Base::Selectable {
	public:
		Radio() = default;
		Radio(std::string name) :
			Selectable(std::move(name))
		{
		}

		void Init() override {
			Renderable::Width = 3 + int(Selectable::Name.size());
			Renderable::Height = 1;
		}
		void Render(Buffer& buffer) override {
			buffer.At(Renderable::Dimension.Top, Renderable::Dimension.Left).Value = "(";
			buffer.At(Renderable::Dimension.Top, Renderable::Dimension.Left + 2).Value = ")";

			if (!Selectable::Name.empty())
				for (int y = Renderable::Dimension.Top, i = 0; y < Renderable::Dimension.Bottom; ++y)
					for (int x = Renderable::Dimension.Left + 3; x < Renderable::Dimension.Right; ++x, ++i)
						buffer.At(y, x).Value = Selectable::Name.at(i);

			if (Selectable::Selected())
				buffer.At(Renderable::Dimension.Top, Renderable::Dimension.Left + 1).Value = u8"\u25CF";

			if (Focusable::Focused())
				for (int y = Renderable::Dimension.Top; y < Renderable::Dimension.Bottom; ++y)
					for (int x = Renderable::Dimension.Left; x < Renderable::Dimension.Right; ++x)
						buffer.At(y, x).Invert = true;
		}

		bool OnKey(KEY_EVENT_RECORD key) override {
			if (key.wVirtualKeyCode == VK_RETURN || key.uChar.AsciiChar == ' ') {
				if (Selectable::Group)
					Selectable::Group->Clear();

				Selectable::Selected(true);
				return true;
			}

			return false;
		}
	};
	class CheckBox final : public Base::Renderable, public Base::Focusable, public Base::Selectable {
	public:
		CheckBox() = default;
		CheckBox(std::string name) :
			Selectable(std::move(name))
		{
		}

		void Init() override {
			Renderable::Width = 3 + int(Selectable::Name.size());
			Renderable::Height = 1;
		}
		void Render(Buffer& buffer) override {
			buffer.At(Renderable::Dimension.Top, Renderable::Dimension.Left).Value = "[";
			buffer.At(Renderable::Dimension.Top, Renderable::Dimension.Left + 2).Value = "]";

			if (!Selectable::Name.empty())
				for (int y = Renderable::Dimension.Top, i = 0; y < Renderable::Dimension.Bottom; ++y)
					for (int x = Renderable::Dimension.Left + 3; x < Renderable::Dimension.Right; ++x, ++i)
						buffer.At(y, x).Value = Selectable::Name.at(i);

			if (Selectable::Selected())
				buffer.At(Renderable::Dimension.Top, Renderable::Dimension.Left + 1).Value = u8"\u25A0";

			if (Focusable::Focused())
				for (int y = Renderable::Dimension.Top; y < Renderable::Dimension.Bottom; ++y)
					for (int x = Renderable::Dimension.Left; x < Renderable::Dimension.Right; ++x)
						buffer.At(y, x).Invert = true;
		}

		bool OnKey(KEY_EVENT_RECORD key) override {
			if (key.wVirtualKeyCode == VK_RETURN || key.uChar.AsciiChar == ' ') {
				if (Selectable::Group)
					Selectable::Group->Clear();

				Selectable::Selected(!Selectable::Selected());
				return true;
			}

			return false;
		}
	};
	class Toggle final : public Base::Renderable, public Base::Focusable, public Base::Selectable {
	public:
		Toggle() = default;

		void Init() override {
			Renderable::Width = Selectable::Selected() ? 4 : 5;
			Renderable::Height = 1;
		}
		void Render(Buffer& buffer) override {
			buffer.At(Renderable::Dimension.Top, Renderable::Dimension.Left).Value = "[";
			buffer.At(Renderable::Dimension.Top, Renderable::Dimension.Right - 1).Value = "]";

			if (Selectable::Selected()) {
				buffer.At(Renderable::Dimension.Top, Renderable::Dimension.Left + 1).Value = "O";
				buffer.At(Renderable::Dimension.Top, Renderable::Dimension.Left + 2).Value = "N";
			}
			else {
				buffer.At(Renderable::Dimension.Top, Renderable::Dimension.Left + 1).Value = "O";
				buffer.At(Renderable::Dimension.Top, Renderable::Dimension.Left + 2).Value = "F";
				buffer.At(Renderable::Dimension.Top, Renderable::Dimension.Left + 3).Value = "F";
			}

			if (Focusable::Focused())
				for (int y = Renderable::Dimension.Top; y < Renderable::Dimension.Bottom; ++y)
					for (int x = Renderable::Dimension.Left; x < Renderable::Dimension.Right; ++x)
						buffer.At(y, x).Invert = true;
		}

		bool OnKey(KEY_EVENT_RECORD key) override {
			if (key.wVirtualKeyCode == VK_RETURN || key.uChar.AsciiChar == ' ') {
				if (Selectable::Group)
					Selectable::Group->Clear();

				Selectable::Selected(!Selectable::Selected());
				return true;
			}

			return false;
		}
	};

	class Border final : public Base::Modifier {
	public:
		Border(std::shared_ptr<Renderable>&& object) :
			Modifier(std::move(object))
		{
		}
		Border(std::shared_ptr<Renderable>&& object, Utils::BorderStyle style) :
			Modifier(std::move(object)),
			style(std::move(style))
		{
		}

		void Init() override {
			Modifier::Init();
			Renderable::Width = Modifier::Object->Width + 2;
			Renderable::Height = Modifier::Object->Height + 2;
		}
		void Set(Rect dimension) override {
			Modifier::Set({ dimension.Left + 1, dimension.Top + 1, dimension.Right - 1, dimension.Bottom - 1 });
			Renderable::Set(dimension);
		}
		void Render(Buffer& buffer) override {
			Modifier::Render(buffer);

			buffer.At(Renderable::Dimension.Top, Renderable::Dimension.Left).Value = this->style.Top.Left;
			buffer.At(Renderable::Dimension.Top, Renderable::Dimension.Right - 1).Value = this->style.Top.Right;
			buffer.At(Renderable::Dimension.Bottom - 1, Renderable::Dimension.Left).Value = this->style.Bottom.Left;
			buffer.At(Renderable::Dimension.Bottom - 1, Renderable::Dimension.Right - 1).Value = this->style.Bottom.Right;

			for (int x = Renderable::Dimension.Left + 1; x < Renderable::Dimension.Right - 1; ++x) {
				buffer.At(Renderable::Dimension.Top, x).Value = this->style.Horizontal;
				buffer.At(Renderable::Dimension.Bottom - 1, x).Value = this->style.Horizontal;
			}
			for (int y = Renderable::Dimension.Top + 1; y < Renderable::Dimension.Bottom - 1; ++y) {
				buffer.At(y, Renderable::Dimension.Left).Value = this->style.Vertical;
				buffer.At(y, Renderable::Dimension.Right - 1).Value = this->style.Vertical;
			}
		}

	private:
		Utils::BorderStyle style = {
			"-", "|",
			{"+", "+", "+"},
			{"+", "+", "+"},
			{"+", "+", "+"}
		};
	};
	class Background final : public Base::Modifier {
	public:
		Background(std::shared_ptr<Base::Renderable>&& object, Color color) :
			Modifier(std::move(object)),
			color(std::move(color))
		{
		}

		void Init() override {
			Modifier::Init();
			Renderable::Width = Modifier::Object->Width;
			Renderable::Height = Modifier::Object->Height;
		}
		void Set(Rect dimension) override {
			Modifier::Set(dimension);
			Renderable::Set(dimension);
		}
		void Render(Buffer& buffer) override {
			Modifier::Render(buffer);

			for (int y = Renderable::Dimension.Top; y < Renderable::Dimension.Bottom; ++y)
				for (int x = Renderable::Dimension.Left; x < Renderable::Dimension.Right; ++x)
					if (buffer.At(y, x).Background == buffer.GetStyle().Background)
						buffer.At(y, x).Background = this->color;
		}

	private:
		Color color;
	};
	class Foreground final : public Base::Modifier {
	public:
		Foreground(std::shared_ptr<Base::Renderable>&& object, Color color) :
			Modifier(std::move(object)),
			color(std::move(color))
		{
		}

		void Init() override {
			Modifier::Init();
			Renderable::Width = Modifier::Object->Width;
			Renderable::Height = Modifier::Object->Height;
		}
		void Set(Rect dimension) override {
			Modifier::Set(dimension);
			Renderable::Set(dimension);
		}
		void Render(Buffer& buffer) override {
			Modifier::Render(buffer);

			for (int y = Renderable::Dimension.Top; y < Renderable::Dimension.Bottom; ++y)
				for (int x = Renderable::Dimension.Left; x < Renderable::Dimension.Right; ++x)
					if (buffer.At(y, x).Foreground == buffer.GetStyle().Foreground)
						buffer.At(y, x).Foreground = this->color;
		}

	private:
		Color color;
	};
}

std::shared_ptr<Simple::Base::Renderable> operator |(
	std::shared_ptr<Simple::Base::Renderable>&& rval,
	std::function<std::shared_ptr<Simple::Base::Renderable>(std::shared_ptr<Simple::Base::Renderable>)> nval
	) {
	return nval(std::move(rval));
}

template<class... Args>
Simple::SelectedGroup MakeGroup(Args&&... objects) {
	return Simple::SelectedGroup(
		Simple::Utils::ToVector<std::shared_ptr<Simple::Base::Selectable>>(
			std::forward<Args>(objects)...
		)
	);
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
std::shared_ptr<Simple::Dropdown> Dropdown(std::vector<std::string> value) {
	return std::make_shared<Simple::Dropdown>(std::move(value));
}
std::shared_ptr<Simple::Dropdown> Dropdown(std::vector<std::string> value, std::string placeholder) {
	return std::make_shared<Simple::Dropdown>(std::move(value), std::move(placeholder));
}
std::shared_ptr<Simple::Radio> Radio() {
	return std::make_shared<Simple::Radio>();
}
std::shared_ptr<Simple::Radio> Radio(std::string name) {
	return std::make_shared<Simple::Radio>(std::move(name));
}
std::shared_ptr<Simple::CheckBox> CheckBox() {
	return std::make_shared<Simple::CheckBox>();
}
std::shared_ptr<Simple::CheckBox> CheckBox(std::string name) {
	return std::make_shared<Simple::CheckBox>(std::move(name));
}
std::shared_ptr<Simple::Toggle> Toggle() {
	return std::make_shared<Simple::Toggle>();
}

std::shared_ptr<Simple::Base::Renderable> Border(std::shared_ptr<Simple::Base::Renderable>&& object) {
	return std::make_shared<Simple::Border>(std::move(object));
}
std::function<std::shared_ptr<Simple::Base::Renderable>(std::shared_ptr<Simple::Base::Renderable>)> BorderStyle(Simple::Utils::BorderStyle style) {
	return [style](std::shared_ptr<Simple::Base::Renderable>&& object) {
		return std::make_shared<Simple::Border>(std::move(object), std::move(style));
		};
}
std::function<std::shared_ptr<Simple::Base::Renderable>(std::shared_ptr<Simple::Base::Renderable>)> Background(Simple::Color color) {
	return [color](std::shared_ptr<Simple::Base::Renderable>&& object) {
		return std::make_shared<Simple::Background>(std::move(object), std::move(color));
		};
}
std::function<std::shared_ptr<Simple::Base::Renderable>(std::shared_ptr<Simple::Base::Renderable>)> Foreground(Simple::Color color) {
	return [color](std::shared_ptr<Simple::Base::Renderable>&& object) {
		return std::make_shared<Simple::Foreground>(std::move(object), std::move(color));
		};
}

static const Simple::Utils::BorderStyle Ascii = {
	"-", "|",
	{"+", "+", "+"},
	{"+", "+", "+"},
	{"+", "+", "+"}
};
static const Simple::Utils::BorderStyle Line = {
	u8"━", u8"┃",
	{u8"┏", u8"┳", u8"┓"},
	{u8"┣", u8"╋", u8"┫"},
	{u8"┗", u8"┻", u8"┛"}
};
static const Simple::Utils::BorderStyle DoubleLine = {
	u8"═", u8"║",
	{u8"╔", u8"╦", u8"╗"},
	{u8"╠", u8"╬", u8"╣"},
	{u8"╚", u8"╩", u8"╝"}
};
static const Simple::Utils::BorderStyle Invisible = {
	" ", " ",
	{" ", " ", " "},
	{" ", " ", " "},
	{" ", " ", " "}
};
static const Simple::Utils::BorderStyle Rounded = {
	u8"─", u8"│",
	{u8"╭", u8"─", u8"╮"},
	{u8"│", u8" ", u8"│"},
	{u8"╰", u8"─", u8"╯"}
};
static const Simple::Utils::BorderStyle Dashed = {
	u8"╌", u8"╎",
	{u8"╌", u8"╌", u8"╌"},
	{u8"╎", u8" ", u8"╎"},
	{u8"╌", u8"╌", u8"╌"}
};

#endif