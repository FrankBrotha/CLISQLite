#include <ncurses.h>
#include "Controller.h"
#include "iostream"

int main(int argc, char *argv[]) {
    setlocale(LC_ALL, "");
    if (argc > 1)
        Controller ob(argv[1]);
    else
        Controller ob("");

}
