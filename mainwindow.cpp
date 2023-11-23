#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    getAutoRun(HKEY_CURRENT_USER, CURRENT_USER_AUTORUN_PATH, userAutoRun);
    //getAutoRun(HKEY_LOCAL_MACHINE, LOCAL_MACHINE_AUTORUN_PATH, machineAutoRun);
    printAutoRun(userAutoRun);
    //printAutoRun(machineAutoRun);
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
        BYTE data[10240];

        while (true) {
            lResult = RegEnumValue(hKey, index, valueName, &valueNameSize, NULL, &valueType, data, &dataSize);
            if (lResult == ERROR_SUCCESS) {
                QString nameStr = QString::fromWCharArray(valueName);
                QString dataStr = QString::fromUtf16(reinterpret_cast<const ushort*>(data), dataSize / sizeof(ushort));
                AutoRun[nameStr.toStdString()] = dataStr.toStdString();

                // 重置缓冲区和大小
                index++;
                valueNameSize = sizeof(valueName) / sizeof(valueName[0]);
                dataSize = sizeof(data) / sizeof(data[0]);
            } else if (lResult == ERROR_NO_MORE_ITEMS) {
                break;  // 已经枚举完所有项，退出循环
            } else {
                // 处理错误情况
                qDebug() << "Error enumerating registry values. Error code: " << lResult;
                break;
            }
        }
        RegCloseKey(hKey);
    }
}

void MainWindow::printAutoRun(const unordered_map<string, string> & AutoRun) {
    for (const auto& entry : AutoRun) {
        qDebug() << "Key:" << QString::fromStdString(entry.first) << ", Value:" << QString::fromStdString(entry.second);
    }
}

