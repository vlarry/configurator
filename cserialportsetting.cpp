#include "cserialportsetting.h"
#include "ui_serialportsetting.h"
//------------------------------------------------------
CSerialPortSetting::CSerialPortSetting(QWidget* parent):
    QWidget(parent),
    ui(new Ui::CSerialPortSetting)
{
    ui->setupUi(this);

    connect(ui->sboxTimeout, SIGNAL(valueChanged(int)), this, SIGNAL(timeout(int)));
    connect(ui->sboxNumRepeat, SIGNAL(valueChanged(int)), this, SIGNAL(numberRepeat(int)));

    setWindowFlag(Qt::Dialog);
}
//---------------------------------------
CSerialPortSetting::~CSerialPortSetting()
{
    delete ui;
}
//------------------------------------------------
const QString CSerialPortSetting::dataBits() const
{
    return ui->cboxDataBit->currentText();
}
//------------------------------------------------
const QString CSerialPortSetting::stopBits() const
{
    return ui->cboxStopBit->currentText();
}
//-------------------------------------------
int CSerialPortSetting::modbusTimeout() const
{
    return ui->sboxTimeout->value();
}
//--------------------------------------------
int CSerialPortSetting::modbusTryCount() const
{
    return ui->sboxNumRepeat->value();
}
//-----------------------------------------------------------
void CSerialPortSetting::setDataBits(const QString& databits)
{
    ui->cboxDataBit->setCurrentText(databits);
}
//-----------------------------------------------------------
void CSerialPortSetting::setStopBits(const QString& stopbits)
{
    ui->cboxStopBit->setCurrentText(stopbits);
}
//-------------------------------------------------------
void CSerialPortSetting::setParity(const QString& parity)
{
    ui->cboxParity->setCurrentText(parity);
}
//--------------------------------------------------
void CSerialPortSetting::setModbusTimeout(int value)
{
    ui->sboxTimeout->setValue(value);
}
//---------------------------------------------------
void CSerialPortSetting::setModbusTryCount(int value)
{
    ui->sboxNumRepeat->setValue(value);
}
//-----------------------------
void CSerialPortSetting::show()
{
    QWidget::show();

    setFixedSize(width(), height());
}
//----------------------------------------------
const QString CSerialPortSetting::parity() const
{
    return ui->cboxParity->currentText();
}
