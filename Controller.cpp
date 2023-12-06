#include "Controller.h"
#include "ncurses.h"
#include "map"

using namespace std;

Controller::Controller(string dbName, bool new_db) {
    if (dbName.size() >= 3 && dbName.substr(dbName.size() - 3) == ".db") {

        sqlModel = new SQL(this, dbName, new_db);
        if (error == 0)
            sqlInitCompele();
    } else
        onExtensionError();
}

Controller::~Controller() {
    delete sqlModel;
    delete consoleView;
}

void Controller::onOpenError() {
    consoleView->onOpenError();
    error = 1;
}

void Controller::onCreateError() {
    consoleView->onCreateError();
    error = 1;
};

void Controller::onExtensionError() {
    consoleView->onExtenstionError();
}

void Controller::sqlInitCompele() {
    data = sqlModel->getData();
    controlGUI();

}


void Controller::setMainTable(bool side) {
    // right - 1
    // left - 0
    if (data.size() == 0)
        return;
    if (side && mainTable < data.size() - 1) {
        mainTable++;
        currentColumns = 0;
        currentRows = 0;
    } else if (!side && mainTable > 0) {
        mainTable--;
        currentColumns = 0;
        currentRows = 0;
    }
    consoleView->setCurrentTable(mainTable);
    switchRightVisibleColumns(false);
    switchDownVisibleRows(false);
}

void Controller::switchRightVisibleColumns(bool change) {
    if (data.size() == 0 || data[mainTable].columnNames.size() == 0 ||
        currentColumns > data[mainTable].columnNames.size())
        return;
    string mas[5];
    if (change && currentColumns + 5 < data[mainTable].columnNames.size())
        currentColumns += 5;
    int maxColumns = currentColumns + 5;
    if (data[mainTable].columnNames.size() < maxColumns) {
        maxColumns = data[mainTable].columnNames.size();
    }
    int curMasId = 0;
    for (int i = currentColumns; i < maxColumns; i++) {
        mas[curMasId] = data[mainTable].columnNames[i];
        curMasId++;
    }

    consoleView->setVisibleColumns(mas);
}

void Controller::switchLeftVisibleColumns() {
    if (data.size() == 0 || data[mainTable].columnNames.size() == 0 || currentColumns <= 0)
        return;
    string mas[5];
    currentColumns -= 5;
    int maxColumns = currentColumns + 5;
    if (data[mainTable].columnNames.size() < maxColumns) {
        maxColumns = data[mainTable].columnNames.size();
    }
    int curMasId = 0;
    for (int i = currentColumns; i < maxColumns; i++) {
        mas[curMasId] = data[mainTable].columnNames[i];
        curMasId++;
    }
    consoleView->setVisibleColumns(mas);
}

void Controller::switchDownVisibleRows(bool change) {
    if (data.size() == 0 || data[mainTable].columnNames.size() == 0)
        return;
    string mas[8][5];
    if (change && currentRows + 8 < data[mainTable].tableData.size())
        currentRows += 8;
    int maxRows = currentRows + 8;
    if (data[mainTable].tableData.size() < maxRows) {
        maxRows = data[mainTable].tableData.size();
    }
    int curMasRowId = 0;

    for (int i = currentRows; i < maxRows; i++) {
        int curMasColumnId = 0;
        for (int j = currentColumns; j < currentColumns + 5; j++) {
            mas[curMasRowId][curMasColumnId] = data[mainTable].tableData[i][j];
            curMasColumnId++;
        }
        curMasRowId++;
    }
    consoleView->setVisibleRows(mas);
}

void Controller::switchUpVisibleRows() {
    if (data.size() == 0 || data[mainTable].columnNames.size() == 0)
        return;
    string mas[8][5];
    if (currentRows > 0)
        currentRows -= 8;
    int maxRows = currentRows + 8;
    if (data[mainTable].tableData.size() < maxRows) {
        maxRows = data[mainTable].tableData.size();
    }

    int curMasRowId = 0;
    for (int i = currentRows; i < maxRows; i++) {
        int curMasColumnId = 0;
        for (int j = currentColumns; j < currentColumns + 5; j++) {
            mas[curMasRowId][curMasColumnId] = data[mainTable].tableData[i][j];
            curMasColumnId++;
        }
        curMasRowId++;
    }
    consoleView->setVisibleRows(mas);
}


void Controller::setVisibleTables() {
    vector<std::string> mas;
    if (data.size() == 0)
        return;
    for (auto & i : data) {
        mas.push_back(i.name);
    }
    consoleView->setVisibleTables(mas);
}

void Controller::controlGUI() {
    int ch;
    // 1 right 0 left
    switchRightVisibleColumns(false);
    // 1 up 0 down
    switchDownVisibleRows(false);
    setVisibleTables();
    consoleView->showDB();
    while (true) {
        ch = getch();
        if (ch == 'x') {
            return;
        } else if (ch == 'd') {
            switchRightVisibleColumns(true);
            switchDownVisibleRows(false);
            consoleView->showDB();
        } else if (ch == 'a') {
            switchLeftVisibleColumns();
            switchDownVisibleRows(false);
            consoleView->showDB();
        } else if (ch == 's') {
            switchDownVisibleRows(true);
            consoleView->showDB();
        } else if (ch == 'w') {
            switchUpVisibleRows();
            consoleView->showDB();
        } else if (ch == 'e') {
            setMainTable(true);
            consoleView->showDB();
        } else if (ch == 'q') {
            setMainTable(false);
            consoleView->showDB();
        } else
            consoleView->showDB();

    }
}
