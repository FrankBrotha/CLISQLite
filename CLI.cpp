#include "CLI.h"
#include <ncurses.h>

#include "vector"

CLI::CLI() {

    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);

}

CLI::~CLI() {
    if (!closed)
        endwin();
}

void CLI::showDB(std::string temparg) {
    clear();
    std::string tablesNames = "";
    int tablesBorders[6] = {0, 16, 32, 48, 64, 80};
    for (int i = 0; i < visibleTables.size(); i++)
        tablesNames += visibleTables[i] + " ";
    int startTables = 40 - tablesNames.length();
    for (int i = 0; i < visibleTables.size(); i++) {
        if (i == currentTable)
            attron(A_UNDERLINE | A_BOLD);
        mvprintw(0, startTables, visibleTables[i].c_str());
        attroff(A_UNDERLINE | A_BOLD);
        startTables += visibleTables[i].length() + 2;
        mvprintw(0, startTables - 1, " ");

    }

    mvprintw(1, 0, "+");
    for (int i = 1; i < 80; i++)
        mvaddch(1, i, '-');
    mvprintw(1, 80, "+");

    mvprintw(20, 0, "+");
    for (int i = 1; i < 80; i++)
        mvaddch(20, i, '-');
    mvprintw(20, 80, "+");


    for (auto i: tablesBorders) {
        for (int j = 2; j < 20; j++)
            mvprintw(j, i, "|");
    }

    for (int i = 0; i < 5; i++) {
        if (visibleCursorX == i && visibleCursorY == 0) {
            attron(A_UNDERLINE | A_ITALIC);
        }
        for (int j = 0; j < visibleColumns[i].length(); j++) {
            if (tablesBorders[i] + j < tablesBorders[i + 1] - 1)
                mvaddch(2, tablesBorders[i] + j + 1, visibleColumns[i][j]);
            else if (tablesBorders[i] + j - 15 < tablesBorders[i + 1])
                mvaddch(3, tablesBorders[i] + j + 1 - 15, visibleColumns[i][j]);
        }
        attroff(A_UNDERLINE | A_ITALIC);
    }
    for (int i = 1; i < 80; i++)
        mvaddch(4, i, '=');

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 5; j++) {
            if (visibleCursorX == j && visibleCursorY == i+1) {
                attron(A_UNDERLINE | A_ITALIC);
            }
            for (int ch = 0; ch < visibleRows[i][j].length(); ch++) {
                if (tablesBorders[j] + ch != tablesBorders[j + 1]) {
                    mvaddch(5 + i * 2, tablesBorders[j] + ch + 1, visibleRows[i][j][ch]);
                }
            }
            attroff(A_UNDERLINE | A_ITALIC);
        }
    }
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 5; j++)
            for (int ch = 0; ch < 15; ch++)
                mvaddch(6 + i * 2, tablesBorders[j] + ch + 1, '-');

    //mvprintw(getmaxy(stdscr) - 1, 0, "нажми x для выхода, дурак");
    mvprintw(getmaxy(stdscr)-1,0, temparg.c_str());
    refresh();

}

std::string CLI::showInputWindowField(std::string columnData) {
    WINDOW *frame_input_win = newwin(12,32,6,29);
    box(frame_input_win, 0, 0);
    WINDOW *input_win = newwin(4, 30, 13, 30);
    mvwprintw(frame_input_win, 6, 2, "Введите новый текст:");
    wrefresh(frame_input_win);
    wrefresh(input_win);
    char buffer[119] = "";
    int ch;
    int index = 0;
    //echo();
    curs_set(1);
    while (true) {
        ch = wgetch(input_win);
        if (ch == 10) {
            break;
        }
        else if (ch == 27) {
            buffer[0] = '\0';
            break;
        }
        else if (ch == 127) {
            if (index > 0) {
                index--;
                buffer[index] = '\0';
            }
        }
        else if (index<sizeof(buffer)-1) {
            buffer[index] = ch;
            index++;
        }
        wclear(input_win);
        mvwprintw(input_win, 0, 0, buffer);
        wrefresh(input_win);
    }
    curs_set(0);
    delwin(frame_input_win);
    delwin(input_win);
    return {buffer};

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

void CLI::setVisibleRows(std::string _visibleRows[8][5]) {
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 5; j++)
            visibleRows[i][j] = _visibleRows[i][j];

}

void CLI::setVisibleTables(std::vector<std::string> _visibleTables) {
    visibleTables.clear();
    for (int i = 0; i < _visibleTables.size(); i++)
        visibleTables.push_back(_visibleTables[i]);
}

void CLI::setCurrentTable(int _currentTable) {
    currentTable = _currentTable;
}

void CLI::setVisibleCursor(int x, int y) {
    visibleCursorX = x;
    visibleCursorY = y;
}