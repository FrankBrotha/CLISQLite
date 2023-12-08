#ifndef CLISQL_CLI_H
#define CLISQL_CLI_H

#include "iostream"
#include "map"

class CLI {
private:
    bool closed = false;
    int currentTable = 0;
    int visibleCursorX = 0;
    int visibleCursorY = 0;
    std::vector<std::string> visibleTables;
    std::string visibleColumns[5];
    std::string visibleRows[8][5];


public:

    explicit CLI();

    ~CLI();

    void onOpenError();

    void onCreateError();

    void onExtenstionError();

    void showDB(std::string temparg=" ");

    std::string showInputWindowField(std::map<std::string, std::string>);

    std::string showInputWindowCell(std::string columnName,std::string data);

    std::string showInputWindowTable();

    void setVisibleColumns(std::string _visibleColumns[5]);

    void setVisibleRows(std::string _visibleRows[8][5]);

    void setVisibleTables(std::vector<std::string>);

    void setCurrentTable(int _currentTable);

    void setVisibleCursor(int x, int y);

    void showError(std::string error);
};


#endif //CLISQL_CLI_H
