#include "cserialportsetting.h"
#include "ui_serialportsetting.h"
//------------------------------------------------------
CSerialPortSetting::CSerialPortSetting(QWidget* parent):
    QWidget(parent),
    ui(new Ui::CSerialPortSetting),
    m_group_databits(nullptr),
    m_group_stopbits(nullptr),
    m_group_parity(nullptr)
{
    ui->setupUi(this);

    m_group_databits = new QButtonGroup(this);
    m_group_stopbits = new QButtonGroup(this);
    m_group_parity   = new QButtonGroup(this);

    m_group_databits->addButton(ui->radioButtonDataBit5, QSerialPort::Data5);
    m_group_databits->addButton(ui->radioButtonDataBit6, QSerialPort::Data6);
    m_group_databits->addButton(ui->radioButtonDataBit7, QSerialPort::Data7);
    m_group_databits->addButton(ui->radioButtonDataBit8, QSerialPort::Data8);

    m_group_stopbits->addButton(ui->radioButtonStopBit1, QSerialPort::OneStop);
    m_group_stopbits->addButton(ui->radioButtonStopBit1_5, QSerialPort::OneAndHalfStop);
    m_group_stopbits->addButton(ui->radioButtonStopBit2, QSerialPort::TwoStop);

    m_group_parity->addButton(ui->radioButtonParityNone, QSerialPort::NoParity);
    m_group_parity->addButton(ui->radioButtonPatityEven, QSerialPort::EvenParity);
    m_group_parity->addButton(ui->radioButtonParityOdd, QSerialPort::OddParity);
    m_group_parity->addButton(ui->radioButtonParitySpace, QSerialPort::SpaceParity);
    m_group_parity->addButton(ui->radioButtonParityMark, QSerialPort::MarkParity);

    ui->tabWidgetSettings->setCurrentIndex(0);

    connect(ui->toolButtonPortRefresh, &QToolButton::clicked, this, &CSerialPortSetting::refreshSerialPort);
    connect(ui->sboxTimeout, SIGNAL(valueChanged(int)), this, SIGNAL(timeout(int)));
    connect(ui->sboxNumRepeat, SIGNAL(valueChanged(int)), this, SIGNAL(numberRepeat(int)));
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
QSerialPort::DataBits CSerialPortSetting::dataBits() const
{
    return QSerialPort::DataBits(m_group_databits->checkedId());
}
//--------------------------------------------------------
QSerialPort::StopBits CSerialPortSetting::stopBits() const
{
    return QSerialPort::StopBits(m_group_stopbits->checkedId());
}
//--------------------------------------------------------
QSerialPort::BaudRate CSerialPortSetting::baudrate() const
{
    return QSerialPort::BaudRate(ui->cboxBaudrate->currentText().toInt());
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
//---------------------------------------------------
int CSerialPortSetting::modbusIntervalSilence() const
{
    return ui->spinBoxIntervalSilence->value();
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
//----------------------------------------
int CSerialPortSetting::deviceSync() const
{
    return ui->spinboxSyncTime->value();
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
//------------------------------------------------------------------
void CSerialPortSetting::setDataBits(QSerialPort::DataBits databits)
{
    switch((int)databits)
    {
        case QSerialPort::Data5:
            ui->radioButtonDataBit5->setChecked(true);
        break;

        case QSerialPort::Data6:
            ui->radioButtonDataBit6->setChecked(true);
        break;

        case QSerialPort::Data7:
            ui->radioButtonDataBit7->setChecked(true);
        break;

        case QSerialPort::Data8:
            ui->radioButtonDataBit8->setChecked(true);
        break;
    }
}
//------------------------------------------------------------------
void CSerialPortSetting::setStopBits(QSerialPort::StopBits stopbits)
{
    switch((int)stopbits)
    {
        case QSerialPort::OneStop:
            ui->radioButtonStopBit1->setChecked(true);
        break;

        case QSerialPort::OneAndHalfStop:
            ui->radioButtonStopBit1_5->setChecked(true);
        break;

        case QSerialPort::TwoStop:
            ui->radioButtonStopBit2->setChecked(true);
        break;
    }
}
//------------------------------------------------------------
void CSerialPortSetting::setParity(QSerialPort::Parity parity)
{
    switch((int)parity)
    {
        case QSerialPort::NoParity:
            ui->radioButtonParityNone->setChecked(true);
        break;

        case QSerialPort::EvenParity:
            ui->radioButtonPatityEven->setChecked(true);
        break;

        case QSerialPort::OddParity:
            ui->radioButtonParityOdd->setChecked(true);
        break;

        case QSerialPort::MarkParity:
            ui->radioButtonParityMark->setChecked(true);
        break;

        case QSerialPort::SpaceParity:
            ui->radioButtonParitySpace->setChecked(true);
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
//---------------------------------------------------
void CSerialPortSetting::setDeviceSync(int time_sync)
{
    ui->spinboxSyncTime->setValue(time_sync);
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
//----------------------------------------------------------
void CSerialPortSetting::setModbusIntervalSilence(int value)
{
    ui->spinBoxIntervalSilence->setValue(value);
}
//-----------------------------
void CSerialPortSetting::show()
{
    QWidget::show();

    setFixedSize(width(), height());

    m_default_baudrate         = baudrate();
    m_default_databits         = dataBits();
    m_default_stopbits         = stopBits();
    m_default_parity           = parity();
    m_default_autospeed        = autospeedState();
    m_default_id               = deviceID();
    m_default_interval_silence = modbusIntervalSilence();
    m_default_timeout          = modbusTimeout();
    m_default_trycount         = modbusTryCount();
    m_default_sync             = deviceSync();
}
//--------------------------------------
void CSerialPortSetting::cancelProcess()
{
    setBaudrate(m_default_baudrate);
    setDataBits(m_default_databits);
    setStopBits(m_default_stopbits);
    setParity(m_default_parity);
    setAutospeed(m_default_autospeed);
    setDeviceID(m_default_id);
    setModbusIntervalSilence(m_default_interval_silence);
    setModbusTimeout(m_default_timeout);
    setModbusTryCount(m_default_trycount);
    setDeviceSync(m_default_sync);

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
    return QSerialPort::Parity(m_group_parity->checkedId());
}
