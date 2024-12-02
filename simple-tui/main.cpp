#include "simple.h"
#include <iostream>
#include <windows.h>

using Simple::Buffer;
using Simple::Pixel;
using Simple::Color;

int main() {
    SetConsoleOutputCP(CP_UTF8);

    Buffer b = Buffer(100, 20);

    auto home = VLayout(Text("Hello"), Text("World"));
    home->Init();
    home->Set({ 0, 0, home->Width, home->Height });
    home->Render(b);
    
    std::cout << b.ToString();
}
