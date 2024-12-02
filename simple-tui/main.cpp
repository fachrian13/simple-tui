#define NOMINMAX
#include <iostream>
#include <windows.h>
#include "simple.h"

using Simple::Buffer;
using Simple::Pixel;
using Simple::Color;

int main() {
    SetConsoleOutputCP(CP_UTF8);

    Buffer b = Buffer(100, 20, Pixel(Color::Green, Color::BrightRed));

    auto home = VLayout(
        Text("Vertical 1 "),
        Text("Vertical 2 "),
        HLayout(
            Text("Horizontal 1 "),
            Text("Horizontal 2 "),
            VLayout(
                Text("Vertical 1 "),
                Text("Vertical 2 "),
                HLayout(
                    Text("Horizontal 1 "),
                    Text("Horizontal 2 "),
                    Text("Horizontal 3 "),
                    Text("Horizontal 4 ")
                ),
                Text("Vertical 3 "),
                Text("Vertical 4 ")
            ),
            Text("Horizontal 3 "),
            Text("Horizontal 4 ")
        ),
        Text("Vertical 3 "),
        Text("Vertical 4 ")
    );
    home->Init();
    home->Set({ 0, 0, home->Width, home->Height });
    home->Render(b);
    
    std::cout << b.ToString();
}
