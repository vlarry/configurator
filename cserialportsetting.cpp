#include "cserialportsetting.h"
#include "ui_serialportsetting.h"
//------------------------------------------------------
CSerialPortSetting::CSerialPortSetting(QWidget* parent):
    QWidget(parent),
    ui(new Ui::CSerialPortSetting)
{
    ui->setupUi(this);

    connect(ui->toolButtonPortRefresh, &QToolButton::clicked, this, &CSerialPortSetting::refreshSerialPort);
    connect(ui->checkBoxAutoSpeed, &QCheckBox::clicked, this, &CSerialPortSetting::autospeed);

    connect(ui->pushButtonOk, &QPushButton::clicked, this, &CSerialPortSetting::okProcess);
    connect(ui->pushButtonCancel, &QPushButton::clicked, this, &CSerialPortSetting::cancelProcess);

    setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);
}
//---------------------------------------
CSerialPortSetting::~CSerialPortSetting()
{
    delete ui;
}
//--------------------------------------------------------
QSerialPort::BaudRate CSerialPortSetting::baudrate() const
{
    return QSerialPort::BaudRate(ui->cboxBaudrate->currentText().toInt());
}
//---------------------------------------------
bool CSerialPortSetting::autospeedState() const
{
    return ui->checkBoxAutoSpeed->isChecked();
}
//--------------------------------------
int CSerialPortSetting::deviceID() const
{
    return ui->sboxSlaveID->value();
}
//------------------------------------------------
QString CSerialPortSetting::serialPortName() const
{
    return ui->cboxPortName->currentText();
}
//---------------------------------------------------------------------
const QVector<QSerialPort::BaudRate> CSerialPortSetting::baudrateList()
{
    QVector<QSerialPort::BaudRate> list;

    for(int i = 0; i < ui->cboxBaudrate->count(); i++)
    {
        list << QSerialPort::BaudRate(ui->cboxBaudrate->itemText(i).toInt());
    }

    return list;
}
//-----------------------------------------------
void CSerialPortSetting::setAutospeed(bool state)
{
    ui->checkBoxAutoSpeed->setChecked(state);
}
//-----------------------------------------------------------------
void CSerialPortSetting::setSerialPortList(const QStringList& list)
{
    ui->cboxPortName->clear();

    if(!list.isEmpty())
        ui->cboxPortName->addItems(list);
}
//------------------------------------------------------------
void CSerialPortSetting::setParity(QSerialPort::Parity parity)
{
    switch((int)parity)
    {
        case QSerialPort::NoParity:
            ui->comboBoxParity->setCurrentIndex(0);
        break;

        case QSerialPort::EvenParity:
            ui->comboBoxParity->setCurrentIndex(1);
        break;

        case QSerialPort::OddParity:
            ui->comboBoxParity->setCurrentIndex(2);
        break;

        case QSerialPort::MarkParity:
            ui->comboBoxParity->setCurrentIndex(3);
        break;

        case QSerialPort::SpaceParity:
            ui->comboBoxParity->setCurrentIndex(4);
        break;
    }
}
//------------------------------------------------------------------
void CSerialPortSetting::setBaudrate(QSerialPort::BaudRate baudrate)
{
    int index = ui->cboxBaudrate->findText(QString::number(baudrate));

    if(index != -1)
        ui->cboxBaudrate->setCurrentIndex(index);
}
//------------------------------------------
void CSerialPortSetting::setDeviceID(int id)
{
    ui->sboxSlaveID->setValue(id);
}
//-----------------------------
void CSerialPortSetting::show()
{
    QWidget::show();

    setFixedSize(width(), height());

    m_default_baudrate         = baudrate();
    m_default_parity           = parity();
    m_default_autospeed        = autospeedState();
    m_default_id               = deviceID();
}
//--------------------------------------
void CSerialPortSetting::cancelProcess()
{
    setBaudrate(m_default_baudrate);
    setParity(m_default_parity);
    setAutospeed(m_default_autospeed);
    setDeviceID(m_default_id);

    hide();
}
//----------------------------------
void CSerialPortSetting::okProcess()
{
    emit updateSettings();
    hide();
}
//-----------------------------------------------------
void CSerialPortSetting::closeEvent(QCloseEvent* event)
{
    cancelProcess();
    QWidget::closeEvent(event);
}
//----------------------------------------------------
QSerialPort::Parity CSerialPortSetting::parity() const
{
    QSerialPort::Parity p;

    switch(ui->comboBoxParity->currentIndex())
    {
        case 0:
            p = QSerialPort::NoParity;
        break;

        case 1:
            p = QSerialPort::EvenParity;
        break;

        case 2:
            p = QSerialPort::OddParity;
        break;

        case 3:
            p = QSerialPort::SpaceParity;
        break;

        case 4:
            p = QSerialPort::MarkParity;
        break;
    }

    return QSerialPort::Parity(p);
}
