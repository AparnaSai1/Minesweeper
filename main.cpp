#include "welcome.h"
#include "board.h"
#include <SFML/Graphics.hpp>
#include <cctype> // For std::isalpha

int main() {
    WelcomeWindow welcome;
    bool gameShouldStart = welcome.runWelcomeWindow();
    if(gameShouldStart)
    {
        Board gamewindow;
        gamewindow.runGameWindow();
    }
    return 0;
}
