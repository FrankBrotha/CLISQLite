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
    if (side)
        consoleView->rightMainTable();
    else
        consoleView->leftMainTable();
}

void Controller::setVisibleColumns(bool side) {
    // right - 1
    // left - 0
    if (data.size() == 0 || data[consoleView->getMainTable()].columnNames.size() == 0)
        return;
    string mas[5];
    if (side) {
        if ((currentColumns > data[consoleView->getMainTable()].columnNames.size())) {
            return;
        }
        if (init && currentColumns + 5 < data[consoleView->getMainTable()].columnNames.size())
            currentColumns += 5;
        int maxColumns = currentColumns + 5;
        for (int i = currentColumns; i < maxColumns; i++)
            if (data[consoleView->getMainTable()].columnNames[i] == "") {
                maxColumns = i;
                break;
            }

        int curMasId = 0;
        for (int i = currentColumns; i < maxColumns; i++) {
            mas[curMasId] = data[consoleView->getMainTable()].columnNames[i];
            curMasId++;
        }

        consoleView->setVisibleColumns(mas);
    } else {
        if (currentColumns <= 0)
            return;
        currentColumns -= 5;
        int maxColumns = currentColumns + 5;
        for (int i = currentColumns; i < maxColumns; i++)
            if (data[consoleView->getMainTable()].columnNames[i] == "") {
                maxColumns = i;
                break;
            }
        int curMasId = 0;
        for (int i = currentColumns; i < maxColumns; i++) {
            mas[curMasId] = data[consoleView->getMainTable()].columnNames[i];
            curMasId++;
        }
        consoleView->setVisibleColumns(mas);
    }
}

void Controller::setVisibleRows(bool side, bool change) {
    // up - 1
    // down - 0
    if (data.size()==0 || data[consoleView->getMainTable()].tableData.size()==0)
        return;
    string mas[8][5];
    if (side) {
        if (change && currentRows>0)
            currentRows-=8;
        int maxRows = currentRows + 8;
        for (int i = currentRows; i < maxRows; i++)
            if (data[consoleView->getMainTable()].tableData[i].size() == 0) {
                maxRows = i;
                break;
            }
        int curMasRowId = 0;
        for (int i = currentRows; i < maxRows; i++) {
            int curMasColumnId = 0;
            for (int j = currentColumns; j < currentColumns + 5; j++) {
                mas[curMasRowId][curMasColumnId] = data[consoleView->getMainTable()].tableData[i][j];
                curMasColumnId++;
            }
            curMasRowId++;
        }
        consoleView->setVisibleRows(mas);
    } else {
        if (change && currentRows+8 < data[consoleView->getMainTable()].tableData.size())
            currentRows += 8;
        int maxRows = currentRows + 8;
        for (int i = currentRows; i < maxRows; i++)
            if (data[consoleView->getMainTable()].tableData[i].size() == 0) {
                maxRows = i;
                break;
            }
        int curMasRowId = 0;

        for (int i = currentRows; i < maxRows; i++) {
            int curMasColumnId = 0;
            for (int j = currentColumns; j < currentColumns + 5; j++) {
                mas[curMasRowId][curMasColumnId] = data[consoleView->getMainTable()].tableData[i][j];
                curMasColumnId++;
            }
            curMasRowId++;
        }
        consoleView->setVisibleRows(mas);

    }

}

void Controller::setVisibleTables() {
    string mas[5];
    if (data.size()==0)
        return;
    for (int i = 0; i < 2; i++) {
        mas[i] = data[i].name;
    }
    consoleView->setVisibleTables(mas);
}

void Controller::controlGUI() {
    int ch;
    // 1 right 0 left
    setVisibleColumns(true);
    // 1 up 0 down
    setVisibleRows(false, false);
    setVisibleTables();
    consoleView->showDB();
    init = true;
    while (true) {
        ch = getch();
        if (ch == 'x') {
            return;
        } else if (ch == 'd') {
            setVisibleColumns(true);
            setVisibleRows(false, false);
            consoleView->showDB();
        } else if (ch == 'a') {
            setVisibleColumns(false);
            setVisibleRows(false, false);
            consoleView->showDB();
        } else if (ch == 's') {
            setVisibleRows(false,true);
            consoleView->showDB();
        } else if (ch == 'w') {
            setVisibleRows(true, true);
            consoleView->showDB();
        }
        else
            consoleView->showDB();


    }
}
