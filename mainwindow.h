#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <Windows.h>
#include <QDebug>
#include <QMessageBox>
#include <QTableWidget>
#include <QVector>
#include "global.h"
#include "regItem.h"
#include "add.h"
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

    void getAutoRun(HKEY rootKey, LPCTSTR lpSubKey, unordered_map<string, string> & AutoRun);
    void initializeVector(unordered_map<string, string> AutoRun, QVector<RegItem*> & vector);
    void updateTable(QTableWidget * table, QVector<RegItem*> vector);
    void addRegistry(HKEY rootKey, LPCTSTR lpSubKey, QString newKey, QString newValue);
    void deleteRegistry(HKEY rootKey, LPCTSTR lpSubKey, QString deleteKey);
    void getName(QTableWidget * table, int index, QString & name);
    void printAutoRun(const unordered_map<string, string> & AutoRun);
    void showRegistry();

private slots:
    void on_userAddRegistryButton_clicked();
    void on_userDeleteRegistryButton_clicked();
    void on_machineAddRegistryButton_clicked();
    void on_machineDeleteRegistryButton_clicked();

private:
    Ui::MainWindow *ui;

    unordered_map<string, string> userAutoRun;
    unordered_map<string, string> machineAutoRun;
    QVector<RegItem*> userVector;
    QVector<RegItem*> machineVector;
    int user_index = -1;
    int machine_index = -1;
};
#endif // MAINWINDOW_H
