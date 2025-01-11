
#include "UI/Window.h"

int main() {

    UI::Window window("SFML UI Demo Window", 1600, 900);
    window.runEventLoop();

    return 0;
}