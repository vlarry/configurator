#ifndef CSERIALPORTSETTING_H
#define CSERIALPORTSETTING_H

#include <QWidget>

namespace Ui {
class CSerialPortSetting;
}

class CSerialPortSetting : public QWidget
{
    Q_OBJECT

public:
    explicit CSerialPortSetting(QWidget *parent = 0);
    ~CSerialPortSetting();

private:
    Ui::CSerialPortSetting *ui;
};

#endif // CSERIALPORTSETTING_H
