#include "cserialportsetting.h"
#include "ui_serialportsetting.h"

CSerialPortSetting::CSerialPortSetting(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CSerialPortSetting)
{
    ui->setupUi(this);
}

CSerialPortSetting::~CSerialPortSetting()
{
    delete ui;
}
