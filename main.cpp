#include <ncurses.h>
#include "Controller.h"
#include "iostream"

int main(int argc, char *argv[]) {
    setlocale(LC_ALL, "");
    if (argc > 2 && (strcmp(argv[2], "-n") != 0))
        Controller ob(argv[2], true);
    else if (argc > 1)
        Controller ob(argv[1]);
    else
        Controller ob("");

}
