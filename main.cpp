#include <iostream>
#include "Application.hpp"

int main(int args, char* argc[])
{
    int width = 720;
    int height = 480;
    
    Application app;
    app.Init(width, height);
    app.start();
    
    return 0;
}
