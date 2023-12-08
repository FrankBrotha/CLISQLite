#ifndef CLISQL_SQL_H
#define CLISQL_SQL_H

#include "sqlite3.h"
#include "string"
#include "map"
#include "Controller.h"
#include "TableInfo.h"

class Controller;

using namespace std;

class SQL {
public:
    SQL(Controller *, const string &, bool new_db);

    ~SQL();

    std::vector<TableInfo> getData();

    std::map<std::string, std::string> getColumnData(string tableName, string columnName);

    string renameColumn(string tableName, string currentColumnName, string newColumnName);

    string changeCellData(string tableName, string columnName, string primaryKeyColumnName,
                          string primaryKey, string newData);

    string renameTable(string oldTableName, string newTableName);

private:
    void onOpenError();

    sqlite3 *dataBasePointer = nullptr;
    Controller *controller = nullptr;
};


#endif //CLISQL_SQL_H
