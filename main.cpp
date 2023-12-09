#include <ncurses.h>
#include "Controller.h"
#include "iostream"

int main(int argc, char *argv[]) {
    setlocale(LC_ALL, "ru_RU.UTF-8");
    if (argc > 1)
        Controller ob(argv[1]);
    else
        Controller ob("");

}
