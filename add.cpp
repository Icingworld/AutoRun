#include "add.h"
#include "ui_add.h"

add::add(QString & NEWNAME, QString & NEWVALUE, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::add),
    newName(NEWNAME),
    newValue(NEWVALUE)
{
    ui->setupUi(this);
    setWindowTitle("Add Registry");
    setWindowIcon(QIcon("://Properties_32x32.png"));
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    ui->nameEdit->setFocus();
}

add::~add()
{
    delete ui;
}

void add::on_buttonBox_accepted()
{
    newName = ui->nameEdit->text();
    newValue = ui->valueEdit->text();
}

