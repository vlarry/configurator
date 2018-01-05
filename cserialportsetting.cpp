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
