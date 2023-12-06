#ifndef CLISQL_TABLEINFO_H
#define CLISQL_TABLEINFO_H
#include <vector>
#include <string>

struct TableInfo {
    std::string name;
    std::string primaryKeyColumnName;
    std::vector<std::string> columnNames;
    std::vector<std::string> primaryKeys;
    std::vector<std::vector<std::string>> tableData;
};


#endif //CLISQL_TABLEINFO_H
