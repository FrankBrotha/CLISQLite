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

void CLI::showError(std::string error) {
    WINDOW *frame_input_win = newwin(12, 32, 6, 29);
    WINDOW *error_message_win = newwin(8,30,9,30);
    box(frame_input_win, 0, 0);
    mvwprintw(frame_input_win, 1, 10, "ОШИБКА");
    int n = 0;

    mvwprintw(error_message_win, 0, 0, error.c_str());
    wrefresh(frame_input_win);
    wrefresh(error_message_win);
    delwin(frame_input_win);
    delwin(error_message_win);

}

void CLI::showDB(std::string temparg) {
    clear();
    std::string tablesNames = "";
    int tablesBorders[6] = {0, 16, 32, 48, 64, 80};
    for (int i = 0; i < visibleTables.size(); i++)
        tablesNames += visibleTables[i] + " ";
    int startTables = 0;
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
            else if (tablesBorders[i] + j - 14 < tablesBorders[i + 1])
                mvaddch(3, tablesBorders[i] + j + 1 - 15, visibleColumns[i][j]);
        }
        attroff(A_UNDERLINE | A_ITALIC);
    }
    for (int i = 1; i < 80; i++)
        mvaddch(4, i, '=');

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 5; j++) {
            if (visibleCursorX == j && visibleCursorY == i + 1) {
                attron(A_UNDERLINE | A_ITALIC);
            }
            for (int ch = 0; ch < visibleRows[i][j].length(); ch++) {
                if (tablesBorders[j] + ch < tablesBorders[j + 1] - 1) {
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
    mvprintw(getmaxy(stdscr) - 1, 0, temparg.c_str());
    refresh();

}

std::string CLI::showInputWindowField(std::map<std::string, std::string> data) {
    WINDOW *frame_input_win = newwin(13, 32, 5, 29);
    box(frame_input_win, 0, 0);
    WINDOW *input_win = newwin(3, 30, 14, 30);
    mvwprintw(frame_input_win, 1, 1, " Поле: ");
    int n = 0;
    for (int i = 8; i < 61; i++) {
        if (n < data["name"].length() && i < 31)
            mvwaddch(frame_input_win, 1, i, data["name"][n]);
        else if (n < data["name"].length())
            mvwaddch(frame_input_win, 2, i - 30, data["name"][n]);
        n++;
    }
    //+ data["name"]).c_str()
    mvwprintw(frame_input_win, 3, 1, (" Not Null: " + data["notNull"]).c_str());
    mvwprintw(frame_input_win, 4, 1, (" Primary Key: " + data["primaryKey"]).c_str());
    mvwprintw(frame_input_win, 5, 1, (" Auto Increment: " + data["autoIncrement"]).c_str());
    mvwprintw(frame_input_win, 6, 1, (" Unique: " + data["unique"]).c_str());
    mvwprintw(frame_input_win, 8, 2, "Введите новое название:");
    wrefresh(frame_input_win);
    wrefresh(input_win);
    char buffer[90] = "";
    int ch;
    int index = 0;
    //echo();
    curs_set(1);
    while (true) {
        ch = wgetch(input_win);
        if (ch == 10) {
            break;
        } else if (ch == 27) {
            buffer[0] = '\0';
            break;
        } else if (ch == 127) {
            if (index > 0) {
                index--;
                buffer[index] = '\0';
            }
        } else if (index < sizeof(buffer) - 1) {
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

std::string CLI::showInputWindowCell(std::string columnName, std::string data) {
    WINDOW *frame_input_win = newwin(12, 32, 6, 29);
    box(frame_input_win, 0, 0);
    WINDOW *input_win = newwin(3, 30, 14, 30);
    mvwprintw(frame_input_win, 1, 1, " Поле: ");
    int n = 0;
    for (int i = 8; i < 61; i++) {
        if (n < columnName.length() && i < 31)
            mvwaddch(frame_input_win, 1, i, columnName[n]);
        else if (n < columnName.length())
            mvwaddch(frame_input_win, 2, i - 30, columnName[n]);
        n++;
    }
    mvwprintw(frame_input_win, 3, 1, " Старое значение: ");
    n = 0;
    for (int i = 19; i < 61; i++) {
        if (n < data.length() && i < 31)
            mvwaddch(frame_input_win, 3, i, data[n]);
        else if (n < data.length())
            mvwaddch(frame_input_win, 4, i - 30, data[n]);
        n++;
    }
    mvwprintw(frame_input_win, 7, 1, " Введите новое значение : ");
    wrefresh(frame_input_win);
    wrefresh(input_win);
    char buffer[90] = "";
    int ch;
    int index = 0;
    curs_set(1);
    while (true) {
        ch = wgetch(input_win);
        if (ch == 10) {
            break;
        } else if (ch == 27) {
            buffer[0] = '\0';
            break;
        } else if (ch == 127) {
            if (index > 0) {
                index--;
                buffer[index] = '\0';
            }
        } else if (index < sizeof(buffer) - 1) {
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

std::string CLI::showInputWindowTable(std::string currentName) {
    WINDOW *frame_input_win = newwin(9, 32, 6, 29);
    box(frame_input_win, 0, 0);
    WINDOW *input_win = newwin(3, 30, 11, 30);
    mvwprintw(frame_input_win, 1, 1, " Таблица: ");
    int n = 0;
    for (int i = 11; i < 61; i++) {
        if (n < currentName.length() && i < 31)
            mvwaddch(frame_input_win, 1, i, currentName[n]);
        else if (n < currentName.length())
            mvwaddch(frame_input_win, 2, i - 30, currentName[n]);
        n++;
    }
    mvwprintw(frame_input_win, 4, 1, " Введите новое значение: ");
    wrefresh(frame_input_win);
    wrefresh(input_win);
    char buffer[90] = "";
    int ch;
    int index = 0;
    curs_set(1);
    while (true) {
        ch = wgetch(input_win);
        if (ch == 10) {
            break;
        } else if (ch == 27) {
            buffer[0] = '\0';
            break;
        } else if (ch == 127) {
            if (index > 0) {
                index--;
                buffer[index] = '\0';
            }
        } else if (index < sizeof(buffer) - 1) {
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

void CLI::onSQLError(std::string error) {
    endwin();
    closed = true;
    printf((error+"\n").c_str());
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