#include "configuratorwindow.h"
#include "ui_configuratorwindow.h"
//------------------------------------------------------
ConfiguratorWindow::ConfiguratorWindow(QWidget* parent):
    QMainWindow(parent),
    ui(new Ui::ConfiguratorWindow),
    m_modbusDevice(Q_NULLPTR),
    m_request_type(EMPTY_TYPE),
    m_panel(Q_NULLPTR),
    m_calc_timer(Q_NULLPTR),
    m_timeout_timer(Q_NULLPTR)
{
    ui->setupUi(this);

    m_panel = new QPanel(this);
    m_panel->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    m_panel->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
    addDockWidget(Qt::RightDockWidgetArea, m_panel);
    
    m_modbusDevice  = new QModbusRtuSerialMaster(this);    
    m_calc_timer    = new QTimer;
    m_timeout_timer = new QTimer;
    
    connect(m_modbusDevice, SIGNAL(stateChanged(QModbusDevice::State)), this, 
                            SLOT(stateChanged(QModbusDevice::State)));
    connect(ui->pbtnPortCtrl, SIGNAL(clicked()), this, SLOT(serialPortCtrl()));
    connect(ui->tbtnPortRefresh, SIGNAL(clicked()), this, SLOT(refreshSerialPort()));
    connect(m_timeout_timer, &QTimer::timeout, this, 
                            &ConfiguratorWindow::timeout);
    connect(ui->sboxTimeout, SIGNAL(valueChanged(int)), this, 
                             SLOT(timeoutChanged(int)));
    connect(m_calc_timer, &QTimer::timeout, this, &ConfiguratorWindow::calcValue);
    connect(ui->pbtnReadCalibration, &QPushButton::clicked, this, &ConfiguratorWindow::readCalibration);
    connect(ui->pbtnWriteCalibration, &QPushButton::clicked, this, &ConfiguratorWindow::writeCalibration);
    
    refreshSerialPort();
    
    m_input_channel_cell.append(ui->le_State_Ch_0);
    m_input_channel_cell.append(ui->le_n_min_Ch_0);
    m_input_channel_cell.append(ui->le_n_max_Ch_0);
    m_input_channel_cell.append(ui->le_n_err_Ch_0);
    m_input_channel_cell.append(ui->le_n_skp_Ch_0);
    m_input_channel_cell.append(ui->le_Am_Ch_0);
    m_input_channel_cell.append(ui->le_A_Ch_0);
    m_input_channel_cell.append(ui->le_B_Ch_0);
    m_input_channel_cell.append(ui->le_C_Ch_0);
    m_input_channel_cell.append(ui->le_fi_Ch_0);
    m_input_channel_cell.append(ui->le_R2_Ch_0);
    
    m_input_channel_cell.append(ui->le_State_Ch_1);
    m_input_channel_cell.append(ui->le_n_min_Ch_1);
    m_input_channel_cell.append(ui->le_n_max_Ch_1);
    m_input_channel_cell.append(ui->le_n_err_Ch_1);
    m_input_channel_cell.append(ui->le_n_skp_Ch_1);
    m_input_channel_cell.append(ui->le_Am_Ch_1);
    m_input_channel_cell.append(ui->le_A_Ch_1);
    m_input_channel_cell.append(ui->le_B_Ch_1);
    m_input_channel_cell.append(ui->le_C_Ch_1);
    m_input_channel_cell.append(ui->le_fi_Ch_1);
    m_input_channel_cell.append(ui->le_R2_Ch_1);
    
    m_input_channel_cell.append(ui->le_State_Ch_2);
    m_input_channel_cell.append(ui->le_n_min_Ch_2);
    m_input_channel_cell.append(ui->le_n_max_Ch_2);
    m_input_channel_cell.append(ui->le_n_err_Ch_2);
    m_input_channel_cell.append(ui->le_n_skp_Ch_2);
    m_input_channel_cell.append(ui->le_Am_Ch_2);
    m_input_channel_cell.append(ui->le_A_Ch_2);
    m_input_channel_cell.append(ui->le_B_Ch_2);
    m_input_channel_cell.append(ui->le_C_Ch_2);
    m_input_channel_cell.append(ui->le_fi_Ch_2);
    m_input_channel_cell.append(ui->le_R2_Ch_2);
    
    m_input_channel_cell.append(ui->le_State_Ch_3);
    m_input_channel_cell.append(ui->le_n_min_Ch_3);
    m_input_channel_cell.append(ui->le_n_max_Ch_3);
    m_input_channel_cell.append(ui->le_n_err_Ch_3);
    m_input_channel_cell.append(ui->le_n_skp_Ch_3);
    m_input_channel_cell.append(ui->le_Am_Ch_3);
    m_input_channel_cell.append(ui->le_A_Ch_3);
    m_input_channel_cell.append(ui->le_B_Ch_3);
    m_input_channel_cell.append(ui->le_C_Ch_3);
    m_input_channel_cell.append(ui->le_fi_Ch_3);
    m_input_channel_cell.append(ui->le_R2_Ch_3);
    
    m_input_channel_cell.append(ui->le_State_Ch_4);
    m_input_channel_cell.append(ui->le_n_min_Ch_4);
    m_input_channel_cell.append(ui->le_n_max_Ch_4);
    m_input_channel_cell.append(ui->le_n_err_Ch_4);
    m_input_channel_cell.append(ui->le_n_skp_Ch_4);
    m_input_channel_cell.append(ui->le_Am_Ch_4);
    m_input_channel_cell.append(ui->le_A_Ch_4);
    m_input_channel_cell.append(ui->le_B_Ch_4);
    m_input_channel_cell.append(ui->le_C_Ch_4);
    m_input_channel_cell.append(ui->le_fi_Ch_4);
    m_input_channel_cell.append(ui->le_R2_Ch_4);
    
    m_input_channel_cell.append(ui->le_State_Ch_5);
    m_input_channel_cell.append(ui->le_n_min_Ch_5);
    m_input_channel_cell.append(ui->le_n_max_Ch_5);
    m_input_channel_cell.append(ui->le_n_err_Ch_5);
    m_input_channel_cell.append(ui->le_n_skp_Ch_5);
    m_input_channel_cell.append(ui->le_Am_Ch_5);
    m_input_channel_cell.append(ui->le_A_Ch_5);
    m_input_channel_cell.append(ui->le_B_Ch_5);
    m_input_channel_cell.append(ui->le_C_Ch_5);
    m_input_channel_cell.append(ui->le_fi_Ch_5);
    m_input_channel_cell.append(ui->le_R2_Ch_5);
    
    m_input_channel_cell.append(ui->le_State_Ch_6);
    m_input_channel_cell.append(ui->le_n_min_Ch_6);
    m_input_channel_cell.append(ui->le_n_max_Ch_6);
    m_input_channel_cell.append(ui->le_n_err_Ch_6);
    m_input_channel_cell.append(ui->le_n_skp_Ch_6);
    m_input_channel_cell.append(ui->le_Am_Ch_6);
    m_input_channel_cell.append(ui->le_A_Ch_6);
    m_input_channel_cell.append(ui->le_B_Ch_6);
    m_input_channel_cell.append(ui->le_C_Ch_6);
    m_input_channel_cell.append(ui->le_fi_Ch_6);
    m_input_channel_cell.append(ui->le_R2_Ch_6);
    
    m_input_channel_cell.append(ui->le_State_Ch_7);
    m_input_channel_cell.append(ui->le_n_min_Ch_7);
    m_input_channel_cell.append(ui->le_n_max_Ch_7);
    m_input_channel_cell.append(ui->le_n_err_Ch_7);
    m_input_channel_cell.append(ui->le_n_skp_Ch_7);
    m_input_channel_cell.append(ui->le_Am_Ch_7);
    m_input_channel_cell.append(ui->le_A_Ch_7);
    m_input_channel_cell.append(ui->le_B_Ch_7);
    m_input_channel_cell.append(ui->le_C_Ch_7);
    m_input_channel_cell.append(ui->le_fi_Ch_7);
    m_input_channel_cell.append(ui->le_R2_Ch_7);
    
    m_input_channel_cell.append(ui->le_State_Ch_8);
    m_input_channel_cell.append(ui->le_n_min_Ch_8);
    m_input_channel_cell.append(ui->le_n_max_Ch_8);
    m_input_channel_cell.append(ui->le_n_err_Ch_8);
    m_input_channel_cell.append(ui->le_n_skp_Ch_8);
    m_input_channel_cell.append(ui->le_Am_Ch_8);
    m_input_channel_cell.append(ui->le_A_Ch_8);
    m_input_channel_cell.append(ui->le_B_Ch_8);
    m_input_channel_cell.append(ui->le_C_Ch_8);
    m_input_channel_cell.append(ui->le_fi_Ch_8);
    m_input_channel_cell.append(ui->le_R2_Ch_8);
    
    m_input_channel_cell.append(ui->le_State_Ch_9);
    m_input_channel_cell.append(ui->le_n_min_Ch_9);
    m_input_channel_cell.append(ui->le_n_max_Ch_9);
    m_input_channel_cell.append(ui->le_n_err_Ch_9);
    m_input_channel_cell.append(ui->le_n_skp_Ch_9);
    m_input_channel_cell.append(ui->le_Am_Ch_9);
    m_input_channel_cell.append(ui->le_A_Ch_9);
    m_input_channel_cell.append(ui->le_B_Ch_9);
    m_input_channel_cell.append(ui->le_C_Ch_9);
    m_input_channel_cell.append(ui->le_fi_Ch_9);
    m_input_channel_cell.append(ui->le_R2_Ch_9);
    
    m_input_channel_cell.append(ui->le_State_Ch_10);
    m_input_channel_cell.append(ui->le_n_min_Ch_10);
    m_input_channel_cell.append(ui->le_n_max_Ch_10);
    m_input_channel_cell.append(ui->le_n_err_Ch_10);
    m_input_channel_cell.append(ui->le_n_skp_Ch_10);
    m_input_channel_cell.append(ui->le_Am_Ch_10);
    m_input_channel_cell.append(ui->le_A_Ch_10);
    m_input_channel_cell.append(ui->le_B_Ch_10);
    m_input_channel_cell.append(ui->le_C_Ch_10);
    m_input_channel_cell.append(ui->le_fi_Ch_10);
    m_input_channel_cell.append(ui->le_R2_Ch_10);
    
    m_input_channel_cell.append(ui->le_State_Ch_22);
    m_input_channel_cell.append(ui->le_n_min_Ch_22);
    m_input_channel_cell.append(ui->le_n_max_Ch_22);
    m_input_channel_cell.append(ui->le_n_err_Ch_22);
    m_input_channel_cell.append(ui->le_n_skp_Ch_22);
    m_input_channel_cell.append(ui->le_Am_Ch_22);
    m_input_channel_cell.append(ui->le_A_Ch_22);
    m_input_channel_cell.append(ui->le_B_Ch_22);
    m_input_channel_cell.append(ui->le_C_Ch_22);
    m_input_channel_cell.append(ui->le_fi_Ch_22);
    m_input_channel_cell.append(ui->le_R2_Ch_22);
    
    m_input_channel_cell.append(ui->le_State_Ch_23);
    m_input_channel_cell.append(ui->le_n_min_Ch_23);
    m_input_channel_cell.append(ui->le_n_max_Ch_23);
    m_input_channel_cell.append(ui->le_n_err_Ch_23);
    m_input_channel_cell.append(ui->le_n_skp_Ch_23);
    m_input_channel_cell.append(ui->le_Am_Ch_23);
    m_input_channel_cell.append(ui->le_A_Ch_23);
    m_input_channel_cell.append(ui->le_B_Ch_23);
    m_input_channel_cell.append(ui->le_C_Ch_23);
    m_input_channel_cell.append(ui->le_fi_Ch_23);
    m_input_channel_cell.append(ui->le_R2_Ch_23);
    
    m_calib_cell.append(ui->leTextCalibFactorCurrentPhase_A);
    m_calib_cell.append(ui->leTextCalibFactorCurrentPhase_B);
    m_calib_cell.append(ui->leTextCalibFactorCurrentPhase_C);
    m_calib_cell.append(ui->leTextCalibFactorCurrent3I0);
    m_calib_cell.append(ui->leTextCalibFactorPowerPhase_A);
    m_calib_cell.append(ui->leTextCalibFactorPowerPhase_B);
    m_calib_cell.append(ui->leTextCalibFactorPowerPhase_C);
    m_calib_cell.append(ui->leTextCalibFactorPower3I0);
    m_calib_cell.append(ui->leTextCalibFactorPowerTotal);
    m_calib_cell.append(ui->leTextCalibFactorPowerPhase_A_B);
    m_calib_cell.append(ui->leTextCalibFactorPowerPhase_B_C);
    m_calib_cell.append(ui->leTextCalibFactorPowerPhase_C_A);
    m_calib_cell.append(ui->leTextCalibFactorPower3U0x);
    m_calib_cell.append(ui->leTextCalibFactorPowerUAx);
    m_calib_cell.append(ui->leTextCalibFactorPowerUBx);
    m_calib_cell.append(ui->leTextCalibFactorPowerUCx);
    m_calib_cell.append(ui->leTextCalibFactorChannel3U0);
    m_calib_cell.append(ui->leTextCalibFactorChannel3Us);
}
//---------------------------------------
ConfiguratorWindow::~ConfiguratorWindow()
{
    if(m_modbusDevice)
    {
        m_modbusDevice->disconnectDevice();
    }
    
    delete m_modbusDevice;
    delete ui;
}
//---------------------------------------
void ConfiguratorWindow::serialPortCtrl()
{
    if(!m_modbusDevice || ui->cboxPortName->count() == 0)
        return;
        
    statusBar()->clearMessage();
    
    if(m_modbusDevice->state() != QModbusDevice::ConnectedState)
    {
    	m_modbusDevice->setConnectionParameter(QModbusDevice::SerialPortNameParameter, ui->cboxPortName->currentText());
        m_modbusDevice->setConnectionParameter(QModbusDevice::SerialBaudRateParameter, ui->cboxPortSpeed->currentText().toInt());
        m_modbusDevice->setConnectionParameter(QModbusDevice::SerialDataBitsParameter, ui->cboxDataBit->currentText().toInt());
        m_modbusDevice->setConnectionParameter(QModbusDevice::SerialStopBitsParameter, ui->cboxStopBit->currentText().toInt());
        
        int parity = (ui->cboxParity->currentText().toUpper() == tr("NO"))?QSerialPort::NoParity:
                     (ui->cboxParity->currentText().toUpper() == tr("EVEN"))?QSerialPort::EvenParity:
                     (ui->cboxParity->currentText().toUpper() == tr("ODD"))?QSerialPort::OddParity:
                     (ui->cboxParity->currentText().toUpper() == tr("MARK"))?QSerialPort::MarkParity:QSerialPort::SpaceParity;
        
        m_modbusDevice->setConnectionParameter(QModbusDevice::SerialParityParameter, parity);
        m_modbusDevice->setTimeout(ui->sboxTimeout->value());
        
        if(!m_modbusDevice->connectDevice())
        {
            statusBar()->showMessage(tr("Ошибка соединения: ") + m_modbusDevice->errorString(), 5000);
            
            return;
        }
        
        m_calc_timer->start(ui->sboxTimeoutCalc->value());
    }
    else
    {
        m_modbusDevice->disconnectDevice();
    }
}
//---------------------------------------------------------------
void ConfiguratorWindow::stateChanged(QModbusDevice::State state)
{
    if(state == QModbusDevice::UnconnectedState)
    {
        ui->pbtnPortCtrl->setChecked(false);
        ui->pbtnPortCtrl->setText(tr("Открыть"));
        statusBar()->showMessage(tr("Соединение закрыто"), 5000);
    }
    else if(state == QModbusDevice::ConnectedState)
    {
        ui->pbtnPortCtrl->setChecked(true);
        ui->pbtnPortCtrl->setText(tr("Закрыть"));
        statusBar()->showMessage(tr("Соединение установлено"), 5000);
    }
}
//------------------------------------------
void ConfiguratorWindow::refreshSerialPort()
{
    QStringList port_list;
    
    for(QSerialPortInfo& port_info: QSerialPortInfo::availablePorts())
    {
        port_list << port_info.portName();
    }
    
    if(port_list.isEmpty())
    {
        statusBar()->showMessage(tr("Нет доступных портов"), 3000);
        return;
    }
    
    ui->cboxPortName->clear();
    ui->cboxPortName->addItems(port_list);
}
//----------------------------------
void ConfiguratorWindow::calcValue()
{
    m_request_type = CALC_TYPE;
    
    QModbusDataUnit unit(QModbusDataUnit::InputRegisters, 64, 110);
    
    request(unit);
}
//----------------------------------------
void ConfiguratorWindow::readCalibration()
{
    m_request_type = CALIB_TYPE;
    
    QModbusDataUnit unit(QModbusDataUnit::HoldingRegisters, 362, 36);
    
    request(unit);
}
//-----------------------------------------
void ConfiguratorWindow::writeCalibration()
{
    QVector<quint16> data;
    
    union
    {
        quint16 b[2];
        float   v;
    } value;
    
    for(quint8 i = 0; i < m_calib_cell.count(); i++)
    {
        value.v = m_calib_cell.at(i)->text().toFloat();
        
        data.append(value.b[1]);
        data.append(value.b[0]);
    }
    
    QModbusDataUnit unit(QModbusDataUnit::HoldingRegisters, 362, data);
    
    request(unit, false);
}
//----------------------------------
void ConfiguratorWindow::readReady()
{
    m_timeout_timer->stop();
    
    QModbusReply* reply = qobject_cast<QModbusReply*>(sender());
    
    if(!reply)
        return;
    
    if(m_request_type == CALC_TYPE)
        m_panel->setData(reply->result().values());
    else if(m_request_type == CALIB_TYPE)
    {
        QVector<quint16> data = reply->result().values();
                
        if(data.count() == 36)
        {
            union
            {
                quint16 b[2];
                float   v;
            } value;
            
            for(quint8 i = 0, j = 0; i < data.count() - 1; i += 2, j++)
            {
                value.b[0] = data.at(i + 1);
                value.b[1] = data.at(i);
                
                m_calib_cell.at(j)->setText(QString::number(value.v, 'f', 4));
            }
        }
        else
            statusBar()->showMessage(tr("Получены не все данные"));
    }
}
//--------------------------------
void ConfiguratorWindow::timeout()
{
    m_timeout_timer->stop();
//    QMessageBox::warning(this, tr("Ошибка ожидания ответа"), tr("Время ожидания ответа от устройства истекло"));
}
//-----------------------------------------------------
void ConfiguratorWindow::timeoutChanged(int newTimeout)
{
    m_modbusDevice->setTimeout(newTimeout);
}
//-----------------------------
void ConfiguratorWindow::show()
{
    QMainWindow::show();
    
    ui->tabwgtRegisters->setCurrentIndex(0);
    ui->tabwgtSettings->setCurrentIndex(0);
    ui->tabwgtInputChannels->setCurrentIndex(0);
    
    setWindowState(Qt::WindowFullScreen);
    setWindowState(Qt::WindowMaximized);
}
//------------------------------------------------------------------
void ConfiguratorWindow::request(QModbusDataUnit& unit, bool isRead)
{
    m_timeout_timer->start(ui->sboxTimeout->value());
    
    QModbusReply* reply;
    
    if(isRead)
        reply = m_modbusDevice->sendReadRequest(unit, ui->sboxSlaveID->value());
    else
        reply = m_modbusDevice->sendWriteRequest(unit, ui->sboxSlaveID->value());
    
    if(reply)
    {
        if(!reply->isFinished())
        {
            connect(reply, &QModbusReply::finished, this, &ConfiguratorWindow::readReady);
        }
        else
        {
            delete reply;
        }
    }
    else
    {
        statusBar()->showMessage(tr("Ошибка чтения: ") + m_modbusDevice->errorString(), 5000);
    }
}
