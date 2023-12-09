#ifndef CLISQL_CONTROLLER_H
#define CLISQL_CONTROLLER_H
#include "CLI.h"
#include "SQL.h"
#include "string"
#include "TableInfo.h"
#include "vector"
class CLI;
class SQL;
class Controller {
private:
    CLI* consoleView = new CLI();
    SQL* sqlModel = nullptr;
    int error=0;
    int currentColumns = 0;
    int currentRows = 0;
    int mainTable = 0;
    int cursorX = 0;
    int cursorY = 0;
    void controlGUI();
    std::vector<TableInfo> data;
    void switchRightVisibleColumns(bool);
    void switchLeftVisibleColumns();
    void switchUpVisibleRows();
    void switchDownVisibleRows(bool change);
    void setMainTable(bool side);
    void setVisibleTables();
    void updateVisibleCursor();
    void switchRightCursor();
    void switchLeftCursor();
    void switchUpCursor();
    void switchDownCursor();
    bool notEmptyCheck();
    bool renameColumn();
    bool changeCellData();
    bool renameTable();
public:
    explicit Controller(std::string dbName);
    ~Controller();
    void onOpenError();
    void onSQLError(std::string error);
    void onExtensionError();
    void sqlInitCompele();


};


#endif //CLISQL_CONTROLLER_H
