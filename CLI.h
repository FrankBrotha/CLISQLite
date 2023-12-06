#ifndef CLISQL_CLI_H
#define CLISQL_CLI_H
#include "iostream"
#include "map"
class CLI {
private:
    bool closed=false;
    int currentTable = 0;
    std::string visibleTables[5]={"","","","",""};
    std::string visibleColumns[5];
    std::string visibleRows[8][5];
public:
    explicit CLI();
    ~CLI();
    void onOpenError();
    void onCreateError();
    void onExtenstionError();
    void showDB();
    void setVisibleColumns(std::string _visibleColumns[5]);
    void setVisibleRows(std::string _visibleRows[8][5]);
    void setVisibleTables(std::string _visibleTables[5]);
    void setCurrentTable(int _currentTable);
};


#endif //CLISQL_CLI_H
