#ifndef REGITEM_H
#define REGITEM_H

#include <QTableWidgetItem>

class RegItem
{
private:
    int _row;
    QTableWidgetItem * _key;
    QTableWidgetItem * _value;
public:
    RegItem(int ROW, QString KEY, QString VALUE) {
        _row = ROW;
        _key = new QTableWidgetItem(KEY);
        _value = new QTableWidgetItem(VALUE);
    };
    ~RegItem() {
        delete _key;
        delete _value;
    };
    int row() { return _row; }
    QTableWidgetItem * key() { return _key; }
    QTableWidgetItem * value() { return _value; }
};


#endif // REGITEM_H
