#include <iostream>
//#include "manager.h"
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>

int main()
{
    using namespace ftxui;

    Element doc = hbox({
                text("Left") | border,
                text("Middle") | border | flex,
                text("right") | border
            });
   
    auto screen = Screen::Create(Dimension::Full(),
            Dimension::Fit(doc));
    Render(screen, doc);
    screen.Print();

/*
    network_counter::SnifferManager manager;
//    manager.AssignFile("log1.txt");
    manager.Process();
*/

    return 0;
}


