#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("AutoRun");
    setWindowIcon(QIcon("://Properties_32x32.png"));

    connect(ui->userTable, &QTableWidget::itemClicked, this, [=](QTableWidgetItem *item){
        if (item != nullptr) {
            user_index = item->row();
        }
    });
    connect(ui->machineTable, &QTableWidget::itemClicked, this, [=](QTableWidgetItem *item){
        if (item != nullptr) {
            machine_index = item->row();
        }
    });

    showRegistry();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::getAutoRun(HKEY rootKey, LPCTSTR lpSubKey, unordered_map<string, string> & AutoRun)
{
    HKEY hKey;
    LONG lResult = RegOpenKeyEx(rootKey, lpSubKey, 0, KEY_READ, &hKey);
    if (lResult == ERROR_SUCCESS) {
        DWORD index = 0;
        TCHAR valueName[255];
        DWORD valueNameSize = sizeof(valueName) / sizeof(valueName[0]);
        DWORD valueType, dataSize;
        BYTE data[1048];
        dataSize = sizeof(valueName) / sizeof(valueName[0]);

        while (true) {
            lResult = RegEnumValue(hKey, index, valueName, &valueNameSize, NULL, &valueType, data, &dataSize);
            if (lResult == ERROR_SUCCESS) {
                QString nameStr = QString::fromWCharArray(valueName);
                QString dataStr = QString::fromUtf16(reinterpret_cast<const ushort*>(data), dataSize / sizeof(ushort));
                AutoRun[nameStr.toStdString()] = dataStr.toStdString();

                index++;
                valueNameSize = sizeof(valueName) / sizeof(valueName[0]);
                dataSize = sizeof(data) / sizeof(data[0]);
            } else if (lResult == ERROR_NO_MORE_ITEMS) {
                break;
            } else {
                qDebug() << "Error enumerating registry values. Error code: " << lResult;
                break;
            }
        }
        RegCloseKey(hKey);
    }
}

void MainWindow::initializeVector(unordered_map<string, string> AutoRun, QVector<RegItem*> & vector)
{
    int index = 0;
    for (const auto& entry : AutoRun) {
        RegItem * item = new RegItem(index, QString::fromStdString(entry.first), QString::fromStdString(entry.second));
        vector.push_back(item);
        index++;
    }
}

void MainWindow::updateTable(QTableWidget * table, QVector<RegItem*> vector)
{
    for (int i=0;i<vector.size();i++) {
        table->setItem(i, 0, vector[i]->key());
        table->setItem(i, 1, vector[i]->value());
    }
    table->resizeColumnsToContents();
}

void MainWindow::addRegistry(HKEY rootKey, LPCTSTR lpSubKey, QString newKey, QString newValue)
{
    HKEY hKey;
    LONG lResult = RegCreateKeyEx(rootKey, lpSubKey, 0, NULL, 0, KEY_SET_VALUE, NULL, &hKey, NULL);
    if (lResult == ERROR_SUCCESS) {
        std::wstring keyData = newKey.toStdWString();
        std::wstring valueData = newValue.toStdWString();

        lResult = RegSetValueEx(hKey, keyData.c_str(), 0, REG_SZ, reinterpret_cast<const BYTE*>(valueData.c_str()), static_cast<DWORD>(valueData.size() * sizeof(wchar_t)));

        if (lResult != ERROR_SUCCESS) {
            qDebug() << "Error writing to registry. Error code: " << lResult;
            if (lResult == ERROR_ACCESS_DENIED) {
                QMessageBox::critical(this, "Error", "Permissions denied!", QMessageBox::Ok);
            }
        }

        RegCloseKey(hKey);
    } else {
        qDebug() << "Error creating/opening registry key. Error code: " << lResult;
        if (lResult == ERROR_ACCESS_DENIED) {
            QMessageBox::critical(this, "Error", "Permissions denied!", QMessageBox::Ok);
        }
    }

    showRegistry();
}

void MainWindow::deleteRegistry(HKEY rootKey, LPCTSTR lpSubKey, QString deleteKey)
{
    HKEY hKey;
    LONG lResult = RegOpenKeyEx(rootKey, lpSubKey, 0, KEY_SET_VALUE, &hKey);
    if (lResult == ERROR_SUCCESS) {
        lResult = RegDeleteValue(hKey, reinterpret_cast<LPCWSTR>(deleteKey.utf16()));
        if (lResult != ERROR_SUCCESS) {
            qDebug() << "Error deleting registry value. Error code: " << lResult;
            if (lResult == ERROR_ACCESS_DENIED) {
                QMessageBox::critical(this, "Error", "Permissions denied!", QMessageBox::Ok);
            }
        }
        RegCloseKey(hKey);
    } else {
        qDebug() << "Error opening registry key. Error code: " << lResult;
        if (lResult == ERROR_ACCESS_DENIED) {
            QMessageBox::critical(this, "Error", "Permissions denied!", QMessageBox::Ok);
        }
    }

    showRegistry();
}

void MainWindow::getName(QTableWidget * table, int index, QString & name)
{
    if (index >= 0) {
        name = table->item(index, 0)->text();
    }
}

void MainWindow::printAutoRun(const unordered_map<string, string> & AutoRun) {
    for (const auto& entry : AutoRun) {
        qDebug() << "Key:" << QString::fromStdString(entry.first) << ", Value:" << QString::fromStdString(entry.second);
    }
}

void MainWindow::showRegistry()
{
    userAutoRun.clear();
    machineAutoRun.clear();
    userVector.clear();
    machineVector.clear();
    ui->userTable->clear();
    ui->machineTable->clear();

    getAutoRun(HKEY_CURRENT_USER, CURRENT_USER_AUTORUN_PATH, userAutoRun);
    getAutoRun(HKEY_LOCAL_MACHINE, LOCAL_MACHINE_AUTORUN_PATH, machineAutoRun);
//    printAutoRun(userAutoRun);
//    printAutoRun(machineAutoRun);
    initializeVector(userAutoRun, userVector);
    initializeVector(machineAutoRun, machineVector);

    ui->userTable->setRowCount(userVector.size());
    ui->machineTable->setRowCount(machineVector.size());

    updateTable(ui->userTable, userVector);
    updateTable(ui->machineTable, machineVector);

    QStringList headerLabels;
    headerLabels << "Name" << "Value";
    ui->userTable->setHorizontalHeaderLabels(headerLabels);
    ui->machineTable->setHorizontalHeaderLabels(headerLabels);
}

void MainWindow::on_userAddRegistryButton_clicked()
{
    QString newName, newValue;
    add * addWindow = new add(newName, newValue);
    if (addWindow->exec()) {
        addRegistry(HKEY_CURRENT_USER, CURRENT_USER_AUTORUN_PATH, newName, newValue);
    }
}

void MainWindow::on_userDeleteRegistryButton_clicked()
{
    QString deleteName;
    getName(ui->userTable, user_index, deleteName);

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirmation", "Continue to delete '" + deleteName + "'?", QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        deleteRegistry(HKEY_CURRENT_USER, CURRENT_USER_AUTORUN_PATH, deleteName);
    }
}

void MainWindow::on_machineAddRegistryButton_clicked()
{
    QString newName, newValue;
    add * addWindow = new add(newName, newValue);
    if (addWindow->exec()) {
        addRegistry(HKEY_LOCAL_MACHINE, LOCAL_MACHINE_AUTORUN_PATH, newName, newValue);
    }
}

void MainWindow::on_machineDeleteRegistryButton_clicked()
{
    QString deleteName;
    getName(ui->machineTable, machine_index, deleteName);
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirmation", "Continue to delete '" + deleteName + "'?", QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        deleteRegistry(HKEY_LOCAL_MACHINE, LOCAL_MACHINE_AUTORUN_PATH, deleteName);
    }
}

