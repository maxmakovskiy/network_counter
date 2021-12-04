#include <iostream>
#include "manager.h"
//#include "test.h"

int main()
{
    network_counter::SnifferManager manager;
    manager.AssignFile("log1.txt");
    manager.Process();
    

    return 0;
}


