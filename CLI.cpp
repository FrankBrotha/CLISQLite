#include "CLI.h"
#include <ncurses.h>

#include <utility>
#include "vector"

CLI::CLI() {

    initscr();
    cbreak();
    noecho();
    curs_set(0);
}

CLI::~CLI() {
    if (!closed)
        endwin();
}

void CLI::showDB() {
    clear();
    std::string tablesNames = "";
    int tablesBorders[6] = {0,16,32,48,64,80};
    for (int i = 0; i < 5; i++)
        tablesNames += visibleTables[i] + " ";
    int startTables = 40 - tablesNames.length();
    for (int i = 0; i < 5; i++) {
        if (i == mainTable)
            attron(A_UNDERLINE | A_BOLD);
        mvprintw(0, startTables, visibleTables[i].c_str());
        attroff(A_UNDERLINE | A_BOLD);
        startTables += visibleTables[i].length() + 2;
        mvprintw(0,startTables-1," ");

    }


    mvprintw(1, 0, "+");
    for (int i=1;i<80;i++)
        mvaddch(1, i, '-');
    mvprintw(1, 80, "+");

    mvprintw(20, 0, "+");
    for (int i=1;i<80;i++)
        mvaddch(20, i, '-');
    mvprintw(20, 80, "+");


    for (auto i:tablesBorders) {
        for (int j=2; j<20;j++)
            mvprintw(j,i,"|");
    }

    for (int i=0; i<5; i++) {
        for (int j=0;j<visibleColumns[i].length();j++) {
            if (tablesBorders[i]+j!=tablesBorders[i+1])
                mvaddch(2,tablesBorders[i]+j+1,visibleColumns[i][j]);
            else if(tablesBorders[i]+j!=tablesBorders[i+1]*2)
                mvaddch(3,tablesBorders[i]+j+1,visibleColumns[i][j]);
        }
    }
    for (int i=1;i<80;i++)
        mvaddch(4, i, '=');

    for (int i=0;i<8;i++) {
        for (int j=0;j<5;j++)
            for (int ch=0;ch<visibleRows[i][j].length();ch++) {
                if (tablesBorders[j]+ch!=tablesBorders[j+1]) {
                    mvaddch(5 + i * 2, tablesBorders[j] + ch + 1, visibleRows[i][j][ch]);
                }
            }
    }
    for (int i=0;i<8;i++)
        for (int j=0;j<5;j++)
            for (int ch=0; ch<15;ch++)
                mvaddch(6 + i * 2, tablesBorders[j]+ch+1, '-');

    mvprintw(getmaxy(stdscr) - 1, 0, "нажми x для выхода, дурак");
    refresh();

}

void CLI::onOpenError() {
    endwin();
    closed = true;
    printf("Не удалось открыть базу данных, используйте аргумент -n для её создания\n");
}

void CLI::onCreateError() {
    endwin();
    closed = true;
    printf("Не удалось создать базу данных\n");
}

void CLI::onExtenstionError() {
    endwin();
    closed = true;
    printf("Неправильно указан файл\n");
}


void CLI::setVisibleColumns(std::string _visibleColumns[5]) {
    for (int i = 0; i < 5; i++)
        visibleColumns[i] = _visibleColumns[i];

}

void CLI::setVisibleRows(std::string _visibleRows[10][5]) {
    for (int i = 0; i < 10; i++)
        for (int j = 0; j < 5; j++)
            visibleRows[i][j] = _visibleRows[i][j];

}

void CLI::setVisibleTables(std::string _visibleTables[5]) {
    for (int i = 0; i < 5; i++)
        visibleTables[i] = _visibleTables[i];
}

void CLI::rightMainTable() {
    if (mainTable < 5)
        mainTable++;
}

void CLI::leftMainTable() {
    if (mainTable > 0)
        mainTable--;
}

int CLI::getMainTable() {
    return mainTable;
}