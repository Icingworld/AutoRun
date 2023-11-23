#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <Windows.h>
#include <QDebug>
#include "global.h"
#include <unordered_map>
#include <string>

using std::unordered_map;
using std::string;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void getAutoRun(HKEY rootKey, LPCTSTR lpSubKey,unordered_map<string, string> & AutoRun);
    void printAutoRun(const unordered_map<string, string> & AutoRun);

private:
    Ui::MainWindow *ui;

    unordered_map<string, string> userAutoRun;
    unordered_map<string, string> machineAutoRun;
};
#endif // MAINWINDOW_H
