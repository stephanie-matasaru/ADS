#include <iostream>
#include "IODialog.h"
#include "Menu.h"

int main() {
    IODialog::clearScreen();
    IODialog::printWelcomeBanner();

    Menu menu;
    menu.run();

    return 0;
}