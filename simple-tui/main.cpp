#include <iostream>
#include "simple.h"

using Simple::Buffer;
using Simple::Pixel;
using Simple::Color;

int main() {
    SetConsoleOutputCP(CP_UTF8);

    Buffer b = Buffer(100, 20, Pixel(Color::Default, Color::BrightRed));
    auto bClick = Button("Click Me!");
    auto vContainer = VContainer(bClick);
    auto home = VLayout(
        Text("Vertical 1 "),
        Text("Vertical 2 "),
        bClick
    );
    home->Init();
    home->Set({ 0, 0, home->Width, home->Height });
    home->Render(b);
    
    std::cout << b.ToString() << std::endl << bClick.use_count();
}
