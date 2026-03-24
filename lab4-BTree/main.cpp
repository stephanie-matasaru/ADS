#include <iostream>
#include "IODialog.h"
#include "Menu.h"

int main() {
    IODialog::clearScreen();
    IODialog::printWelcomeBanner();

    int t = IODialog::readDegree();

    Menu menu(t);
    menu.run();

    return 0;
}