#ifndef ADD_H
#define ADD_H

#include <QDialog>
#include <QIcon>

namespace Ui {
class add;
}

class add : public QDialog
{
    Q_OBJECT

public:
    explicit add(QString & NEWNAME, QString & NEWVALUE, QWidget *parent = nullptr);
    ~add();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::add *ui;
    QString & newName;
    QString & newValue;
};

#endif // ADD_H
