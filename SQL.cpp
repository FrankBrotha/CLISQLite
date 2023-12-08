#include "SQL.h"
#include "vector"
#include "sqlite3.h"

SQL::SQL(Controller *_controller, const string &dbName, bool new_db) {
    controller = _controller;
    int rc;
    if (new_db)
        rc = sqlite3_open(dbName.c_str(), &dataBasePointer);
    else
        rc = sqlite3_open_v2(dbName.c_str(), &dataBasePointer, SQLITE_OPEN_READWRITE, NULL);
    if (rc != SQLITE_OK || dbName == "") {
        if (new_db)
            controller->onCreateError();
        else
            controller->onOpenError();
    }
//    else
//        controller->sqlInitCompele();

}

SQL::~SQL() {
    sqlite3_close(dataBasePointer);
}

std::vector<TableInfo> SQL::getData() {
    std::vector<TableInfo> dataBase;
    sqlite3_stmt *tableStmt;
    std::string tableQuery = "SELECT name FROM sqlite_master WHERE type='table';";
    if (sqlite3_prepare_v2(dataBasePointer, tableQuery.c_str(), -1, &tableStmt, nullptr) != SQLITE_OK) {
        controller->onCreateError();
    }
    while (sqlite3_step(tableStmt) == SQLITE_ROW) {
        TableInfo new_table;
        new_table.name = (reinterpret_cast<const char *>(sqlite3_column_text(tableStmt, 0)));

        sqlite3_stmt *countStmt;
        std::string countQuery = "SELECT COUNT(*) FROM " + std::string(new_table.name) + ";";
        if (sqlite3_prepare_v2(dataBasePointer, countQuery.c_str(), -1, &countStmt, nullptr) != SQLITE_OK) {
            controller->onCreateError();
        }
        sqlite3_step(countStmt);


        std::string dataQueryString = "SELECT * FROM " + std::string(new_table.name) + ";";
        sqlite3_stmt *dataStmt;
        if (sqlite3_prepare_v2(dataBasePointer, dataQueryString.c_str(), -1, &dataStmt, nullptr) != SQLITE_OK) {
            controller->onCreateError();
        }

        new_table.tableData.resize(sqlite3_column_int(countStmt, 0),
                                   std::vector<std::string>(sqlite3_column_count(dataStmt)));
        sqlite3_finalize(countStmt);

        for (int i = 0; i < sqlite3_column_count(dataStmt); i++)
            new_table.columnNames.push_back(std::string(sqlite3_column_name(dataStmt, i)));

        int tempId = 0;
        while (sqlite3_step(dataStmt) == SQLITE_ROW) {
            for (int i = 0; i < sqlite3_column_count(dataStmt); i++) {
                if (sqlite3_column_text(dataStmt, i) == nullptr)
                    new_table.tableData[tempId][i] = "NULL";

                else
                    new_table.tableData[tempId][i] = reinterpret_cast<const char *>(sqlite3_column_text(dataStmt, i));
            }
            tempId++;
        }


        std::string primaryKeysColumnQuery = "PRAGMA table_info(" + new_table.name + ")" + ";";
        sqlite3_stmt *primaryKeysColumnStmt;
        if (sqlite3_prepare_v2(dataBasePointer, primaryKeysColumnQuery.c_str(), -1, &primaryKeysColumnStmt, nullptr) !=
            SQLITE_OK) {
            controller->onCreateError();
        }
        while (sqlite3_step(primaryKeysColumnStmt) == SQLITE_ROW) {
            int pk = sqlite3_column_int(primaryKeysColumnStmt, 5);
            if (pk == 1) {
                new_table.primaryKeyColumnName = reinterpret_cast<const char *>(sqlite3_column_text(
                        primaryKeysColumnStmt,
                        1));
                break;
            } else
                new_table.primaryKeyColumnName = "rowid";
        }

        sqlite3_finalize(primaryKeysColumnStmt);
        std::string primaryKeysQuery = "SELECT " + new_table.primaryKeyColumnName + " FROM " + new_table.name + ";";
        sqlite3_stmt *primaryKeysStmt;
        if (sqlite3_prepare_v2(dataBasePointer, primaryKeysQuery.c_str(), -1, &primaryKeysStmt, nullptr) !=
            SQLITE_OK) {
            controller->onCreateError();
        }
        while (sqlite3_step(primaryKeysStmt) == SQLITE_ROW) {
            new_table.primaryKeys.push_back(
                    reinterpret_cast<const char *>(sqlite3_column_text(primaryKeysColumnStmt, 0)));
        }
        sqlite3_finalize(primaryKeysStmt);
        sqlite3_finalize(dataStmt);
        dataBase.push_back(new_table);
    }
    sqlite3_finalize(tableStmt);
    return dataBase;
}

std::map<std::string, string> SQL::getColumnData(string tableName, string columnName) {
    map<string, string> data;
    // Получаем информацию о таблице из sqlite_master
    sqlite3_stmt *stmt;


    // Получаем информацию о каждом столбце из pragma_table_info
    std::string columnInfoQuery = "PRAGMA table_info(" + std::string(tableName) + ")";
    if (sqlite3_prepare_v2(dataBasePointer, columnInfoQuery.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        controller->onCreateError();
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        if (reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1)) == columnName) {
            data["name"] = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));
            data["notNull"] = sqlite3_column_int(stmt, 3) == 1 ? "true" : "false";
            data["primaryKey"] = sqlite3_column_int(stmt, 5) == 1 ? "true" : "false";
            data["autoIncrement"] = sqlite3_column_int(stmt, 6) == 1 ? "true" : "false";
            data["unique"] = sqlite3_column_int(stmt, 8) == 1 ? "true" : "false";
            break;
        }
    }

    sqlite3_finalize(stmt);
    return data;
}
string SQL::renameColumn(std::string tableName, std::string currentColumnName, std::string newColumnName) {
    string sql = "ALTER TABLE "+ tableName +" RENAME COLUMN '"+ currentColumnName + "' TO '" + newColumnName +"'";
    int rc = sqlite3_exec(dataBasePointer, sql.c_str(), nullptr, nullptr, nullptr);
    if (rc != SQLITE_OK) {
            return sqlite3_errmsg(dataBasePointer);
    }
    else
        return "OK";
}

string SQL::changeCellData(string tableName, string columnName, string primaryKeyColumnName, string primaryKey, string newData) {
    std::string sql = "UPDATE '" + tableName +
                      "' SET '" + columnName +
                      "' = '" + newData +
                      "' WHERE " + primaryKeyColumnName + " = '" + primaryKey+"'";
    int rc = sqlite3_exec(dataBasePointer, sql.c_str(), nullptr, nullptr, nullptr);
    if (rc != SQLITE_OK) {
        return sqlite3_errmsg(dataBasePointer);
    }
    else
        return "OK";
}




