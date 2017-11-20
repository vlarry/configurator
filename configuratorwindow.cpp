#include "configuratorwindow.h"
#include "ui_configuratorwindow.h"
//------------------------------------------------------
ConfiguratorWindow::ConfiguratorWindow(QWidget* parent):
    QMainWindow(parent),
    ui(new Ui::ConfiguratorWindow),
    m_modbusDevice(nullptr),
    m_panel(nullptr),
    m_tim_calculate(nullptr),
    m_protect_mtz_group(nullptr)
{
    ui->setupUi(this);

    m_modbusDevice      = new CModbus(this);
    m_panel             = new QPanel(this);
    m_tim_calculate     = new QTimer;
    m_protect_mtz_group = new QButtonGroup(ui->tabProtectionMTZ);
    
    m_panel->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    m_panel->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
    addDockWidget(Qt::RightDockWidgetArea, m_panel);
    
    m_protect_mtz_group->addButton(ui->pbtnProtectionMTZ1);
    m_protect_mtz_group->addButton(ui->pbtnProtectionMTZ2);
    m_protect_mtz_group->addButton(ui->pbtnProtectionMTZ3);
    m_protect_mtz_group->addButton(ui->pbtnProtectionMTZ4);
    
    m_protect_mtz_group->setId(ui->pbtnProtectionMTZ1, 0);
    m_protect_mtz_group->setId(ui->pbtnProtectionMTZ2, 1);
    m_protect_mtz_group->setId(ui->pbtnProtectionMTZ3, 2);
    m_protect_mtz_group->setId(ui->pbtnProtectionMTZ4, 3);
    
    protectMTZChangedID(0);
    
    m_protect_mtz_group->setExclusive(true);
    
    connect(ui->pbtnPortCtrl, &QPushButton::clicked, this, &ConfiguratorWindow::serialPortCtrl);
    connect(m_modbusDevice, &CModbus::connectDeviceState, this, &ConfiguratorWindow::stateChanged);
    connect(ui->tbtnPortRefresh, &QToolButton::clicked, this, &ConfiguratorWindow::refreshSerialPort);
    connect(m_modbusDevice, &CModbus::dataReady, this, &ConfiguratorWindow::responseRead);
    connect(m_tim_calculate, &QTimer::timeout, this, &ConfiguratorWindow::calculate_value);
    connect(ui->pbtnReadCalibration, &QPushButton::clicked, this, &ConfiguratorWindow::calibration_read);
    connect(ui->pbtnWriteCalibration, &QPushButton::clicked, this, &ConfiguratorWindow::calibration_write);
    connect(ui->checkboxCalibTimeout, &QCheckBox::clicked, this, &ConfiguratorWindow::chboxCalculateTimeoutStateChanged);
    connect(ui->sboxTimeoutCalc, SIGNAL(valueChanged(int)), this, SLOT(timeCalculateChanged(int)));
    connect(m_protect_mtz_group, SIGNAL(buttonClicked(int)), this, SLOT(protectMTZChangedID(int)));
    connect(ui->pbtnReadProtection, &QPushButton::clicked, this, &ConfiguratorWindow::protection_read);
    connect(ui->pbtnWriteProtection, &QPushButton::clicked, this, &ConfiguratorWindow::protection_write);
    connect(m_modbusDevice, &CModbus::errorDevice, this, &ConfiguratorWindow::errorDevice);
    connect(ui->sboxTimeout, SIGNAL(valueChanged(int)), this, SLOT(timeoutValueChanged(int)));
    connect(ui->sboxNumRepeat, SIGNAL(valueChanged(int)), this, SLOT(numberRepeatChanged(int)));
    
    ui->tabwgtRegisters->setDisabled(true);
    refreshSerialPort();
    
    m_calculate_cell.append(ui->le_State_Ch_0);
    m_calculate_cell.append(ui->le_n_min_Ch_0);
    m_calculate_cell.append(ui->le_n_max_Ch_0);
    m_calculate_cell.append(ui->le_n_err_Ch_0);
    m_calculate_cell.append(ui->le_n_skp_Ch_0);
    m_calculate_cell.append(ui->le_Am_Ch_0);
    m_calculate_cell.append(ui->le_A_Ch_0);
    m_calculate_cell.append(ui->le_B_Ch_0);
    m_calculate_cell.append(ui->le_C_Ch_0);
    m_calculate_cell.append(ui->le_fi_Ch_0);
    m_calculate_cell.append(ui->le_R2_Ch_0);
    
    m_calculate_cell.append(ui->le_State_Ch_1);
    m_calculate_cell.append(ui->le_n_min_Ch_1);
    m_calculate_cell.append(ui->le_n_max_Ch_1);
    m_calculate_cell.append(ui->le_n_err_Ch_1);
    m_calculate_cell.append(ui->le_n_skp_Ch_1);
    m_calculate_cell.append(ui->le_Am_Ch_1);
    m_calculate_cell.append(ui->le_A_Ch_1);
    m_calculate_cell.append(ui->le_B_Ch_1);
    m_calculate_cell.append(ui->le_C_Ch_1);
    m_calculate_cell.append(ui->le_fi_Ch_1);
    m_calculate_cell.append(ui->le_R2_Ch_1);
    
    m_calculate_cell.append(ui->le_State_Ch_2);
    m_calculate_cell.append(ui->le_n_min_Ch_2);
    m_calculate_cell.append(ui->le_n_max_Ch_2);
    m_calculate_cell.append(ui->le_n_err_Ch_2);
    m_calculate_cell.append(ui->le_n_skp_Ch_2);
    m_calculate_cell.append(ui->le_Am_Ch_2);
    m_calculate_cell.append(ui->le_A_Ch_2);
    m_calculate_cell.append(ui->le_B_Ch_2);
    m_calculate_cell.append(ui->le_C_Ch_2);
    m_calculate_cell.append(ui->le_fi_Ch_2);
    m_calculate_cell.append(ui->le_R2_Ch_2);
    
    m_calculate_cell.append(ui->le_State_Ch_3);
    m_calculate_cell.append(ui->le_n_min_Ch_3);
    m_calculate_cell.append(ui->le_n_max_Ch_3);
    m_calculate_cell.append(ui->le_n_err_Ch_3);
    m_calculate_cell.append(ui->le_n_skp_Ch_3);
    m_calculate_cell.append(ui->le_Am_Ch_3);
    m_calculate_cell.append(ui->le_A_Ch_3);
    m_calculate_cell.append(ui->le_B_Ch_3);
    m_calculate_cell.append(ui->le_C_Ch_3);
    m_calculate_cell.append(ui->le_fi_Ch_3);
    m_calculate_cell.append(ui->le_R2_Ch_3);
    
    m_calculate_cell.append(ui->le_State_Ch_4);
    m_calculate_cell.append(ui->le_n_min_Ch_4);
    m_calculate_cell.append(ui->le_n_max_Ch_4);
    m_calculate_cell.append(ui->le_n_err_Ch_4);
    m_calculate_cell.append(ui->le_n_skp_Ch_4);
    m_calculate_cell.append(ui->le_Am_Ch_4);
    m_calculate_cell.append(ui->le_A_Ch_4);
    m_calculate_cell.append(ui->le_B_Ch_4);
    m_calculate_cell.append(ui->le_C_Ch_4);
    m_calculate_cell.append(ui->le_fi_Ch_4);
    m_calculate_cell.append(ui->le_R2_Ch_4);
    
    m_calculate_cell.append(ui->le_State_Ch_5);
    m_calculate_cell.append(ui->le_n_min_Ch_5);
    m_calculate_cell.append(ui->le_n_max_Ch_5);
    m_calculate_cell.append(ui->le_n_err_Ch_5);
    m_calculate_cell.append(ui->le_n_skp_Ch_5);
    m_calculate_cell.append(ui->le_Am_Ch_5);
    m_calculate_cell.append(ui->le_A_Ch_5);
    m_calculate_cell.append(ui->le_B_Ch_5);
    m_calculate_cell.append(ui->le_C_Ch_5);
    m_calculate_cell.append(ui->le_fi_Ch_5);
    m_calculate_cell.append(ui->le_R2_Ch_5);
    
    m_calculate_cell.append(ui->le_State_Ch_6);
    m_calculate_cell.append(ui->le_n_min_Ch_6);
    m_calculate_cell.append(ui->le_n_max_Ch_6);
    m_calculate_cell.append(ui->le_n_err_Ch_6);
    m_calculate_cell.append(ui->le_n_skp_Ch_6);
    m_calculate_cell.append(ui->le_Am_Ch_6);
    m_calculate_cell.append(ui->le_A_Ch_6);
    m_calculate_cell.append(ui->le_B_Ch_6);
    m_calculate_cell.append(ui->le_C_Ch_6);
    m_calculate_cell.append(ui->le_fi_Ch_6);
    m_calculate_cell.append(ui->le_R2_Ch_6);
    
    m_calculate_cell.append(ui->le_State_Ch_7);
    m_calculate_cell.append(ui->le_n_min_Ch_7);
    m_calculate_cell.append(ui->le_n_max_Ch_7);
    m_calculate_cell.append(ui->le_n_err_Ch_7);
    m_calculate_cell.append(ui->le_n_skp_Ch_7);
    m_calculate_cell.append(ui->le_Am_Ch_7);
    m_calculate_cell.append(ui->le_A_Ch_7);
    m_calculate_cell.append(ui->le_B_Ch_7);
    m_calculate_cell.append(ui->le_C_Ch_7);
    m_calculate_cell.append(ui->le_fi_Ch_7);
    m_calculate_cell.append(ui->le_R2_Ch_7);
    
    m_calculate_cell.append(ui->le_State_Ch_8);
    m_calculate_cell.append(ui->le_n_min_Ch_8);
    m_calculate_cell.append(ui->le_n_max_Ch_8);
    m_calculate_cell.append(ui->le_n_err_Ch_8);
    m_calculate_cell.append(ui->le_n_skp_Ch_8);
    m_calculate_cell.append(ui->le_Am_Ch_8);
    m_calculate_cell.append(ui->le_A_Ch_8);
    m_calculate_cell.append(ui->le_B_Ch_8);
    m_calculate_cell.append(ui->le_C_Ch_8);
    m_calculate_cell.append(ui->le_fi_Ch_8);
    m_calculate_cell.append(ui->le_R2_Ch_8);
    
    m_calculate_cell.append(ui->le_State_Ch_9);
    m_calculate_cell.append(ui->le_n_min_Ch_9);
    m_calculate_cell.append(ui->le_n_max_Ch_9);
    m_calculate_cell.append(ui->le_n_err_Ch_9);
    m_calculate_cell.append(ui->le_n_skp_Ch_9);
    m_calculate_cell.append(ui->le_Am_Ch_9);
    m_calculate_cell.append(ui->le_A_Ch_9);
    m_calculate_cell.append(ui->le_B_Ch_9);
    m_calculate_cell.append(ui->le_C_Ch_9);
    m_calculate_cell.append(ui->le_fi_Ch_9);
    m_calculate_cell.append(ui->le_R2_Ch_9);
    
    m_calculate_cell.append(ui->le_State_Ch_10);
    m_calculate_cell.append(ui->le_n_min_Ch_10);
    m_calculate_cell.append(ui->le_n_max_Ch_10);
    m_calculate_cell.append(ui->le_n_err_Ch_10);
    m_calculate_cell.append(ui->le_n_skp_Ch_10);
    m_calculate_cell.append(ui->le_Am_Ch_10);
    m_calculate_cell.append(ui->le_A_Ch_10);
    m_calculate_cell.append(ui->le_B_Ch_10);
    m_calculate_cell.append(ui->le_C_Ch_10);
    m_calculate_cell.append(ui->le_fi_Ch_10);
    m_calculate_cell.append(ui->le_R2_Ch_10);
    
    m_calculate_cell.append(ui->le_State_Ch_22);
    m_calculate_cell.append(ui->le_n_min_Ch_22);
    m_calculate_cell.append(ui->le_n_max_Ch_22);
    m_calculate_cell.append(ui->le_n_err_Ch_22);
    m_calculate_cell.append(ui->le_n_skp_Ch_22);
    m_calculate_cell.append(ui->le_Am_Ch_22);
    m_calculate_cell.append(ui->le_A_Ch_22);
    m_calculate_cell.append(ui->le_B_Ch_22);
    m_calculate_cell.append(ui->le_C_Ch_22);
    m_calculate_cell.append(ui->le_fi_Ch_22);
    m_calculate_cell.append(ui->le_R2_Ch_22);
    
    m_calculate_cell.append(ui->le_State_Ch_23);
    m_calculate_cell.append(ui->le_n_min_Ch_23);
    m_calculate_cell.append(ui->le_n_max_Ch_23);
    m_calculate_cell.append(ui->le_n_err_Ch_23);
    m_calculate_cell.append(ui->le_n_skp_Ch_23);
    m_calculate_cell.append(ui->le_Am_Ch_23);
    m_calculate_cell.append(ui->le_A_Ch_23);
    m_calculate_cell.append(ui->le_B_Ch_23);
    m_calculate_cell.append(ui->le_C_Ch_23);
    m_calculate_cell.append(ui->le_fi_Ch_23);
    m_calculate_cell.append(ui->le_R2_Ch_23);
    
    m_calibration_cell.append(ui->leTextCalibFactorCurrentPhase_A);
    m_calibration_cell.append(ui->leTextCalibFactorCurrentPhase_B);
    m_calibration_cell.append(ui->leTextCalibFactorCurrentPhase_C);
    m_calibration_cell.append(ui->leTextCalibFactorCurrent3I0);
    m_calibration_cell.append(ui->leTextCalibFactorPowerPhase_A);
    m_calibration_cell.append(ui->leTextCalibFactorPowerPhase_B);
    m_calibration_cell.append(ui->leTextCalibFactorPowerPhase_C);
    m_calibration_cell.append(ui->leTextCalibFactorPower3I0);
    m_calibration_cell.append(ui->leTextCalibFactorPowerTotal);
    m_calibration_cell.append(ui->leTextCalibFactorPowerPhase_A_B);
    m_calibration_cell.append(ui->leTextCalibFactorPowerPhase_B_C);
    m_calibration_cell.append(ui->leTextCalibFactorPowerPhase_C_A);
    m_calibration_cell.append(ui->leTextCalibFactorPower3U0x);
    m_calibration_cell.append(ui->leTextCalibFactorPowerUAx);
    m_calibration_cell.append(ui->leTextCalibFactorPowerUBx);
    m_calibration_cell.append(ui->leTextCalibFactorPowerUCx);
    m_calibration_cell.append(ui->leTextCalibFactorChannel3U0);
    m_calibration_cell.append(ui->leTextCalibFactorChannel3Us);
}
//---------------------------------------
ConfiguratorWindow::~ConfiguratorWindow()
{
    if(m_modbusDevice)
    {
        m_modbusDevice->disconnectDevice();
    }
    
    delete m_modbusDevice;
    m_modbusDevice = nullptr;
    
    delete ui;
}
//---------------------------------------
void ConfiguratorWindow::serialPortCtrl()
{
    if(!m_modbusDevice || ui->cboxPortName->count() == 0)
        return;
        
    statusBar()->clearMessage();
    
    if(ui->pbtnPortCtrl->isChecked())
    {
        m_modbusDevice->setPortName(ui->cboxPortName->currentText());
        m_modbusDevice->setBaudrate(ui->cboxBaudrate->currentText().toInt());
        m_modbusDevice->setDatabits((QSerialPort::DataBits)ui->cboxDataBit->currentText().toInt());

        quint32 stopbits = ((ui->cboxStopBit->currentText() == "1.5")?3:ui->cboxStopBit->currentText().toInt());
        quint32 parity   = ((ui->cboxParity->currentText().toUpper() == tr("NO"))?0:
                           (ui->cboxParity->currentText().toUpper() == tr("EVEN"))?2:
                           (ui->cboxParity->currentText().toUpper() == tr("ODD"))?3:
                           (ui->cboxParity->currentText().toUpper() == tr("SPACE")))?4:5;
        
        m_modbusDevice->setStopbits((QSerialPort::StopBits)stopbits);
        m_modbusDevice->setParity((QSerialPort::Parity)parity);
        
        m_modbusDevice->connectDevice();
    }
    else
    {
        m_modbusDevice->disconnectDevice();
    }
}
//-----------------------------------------------
void ConfiguratorWindow::stateChanged(bool state)
{
    ui->pbtnPortCtrl->setChecked(state);
    
    ui->pbtnPortCtrl->setText(((state)?tr("Закрыть"):tr("Открыть")));
    statusBar()->showMessage(((state)?tr("Соединение с устройством установлено"):
                                      tr("Соединение с устройством разорвано")), 5000);
    
    if(ui->checkboxCalibTimeout->isChecked() && state)
        chboxCalculateTimeoutStateChanged(true);
    else
        m_tim_calculate->stop();
    
    ui->tabwgtRegisters->setEnabled(state);
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
        QMessageBox::warning(nullptr, tr("Com-порт"), 
                             tr("Не удалось найти ни одного доступного последовательного порта на этом компьютере"));
        
        return;
    }
    
    ui->cboxPortName->clear();
    ui->cboxPortName->addItems(port_list);
}
//----------------------------------------
void ConfiguratorWindow::calculate_value()
{
    CDataUnitType unit(ui->sboxSlaveID->value(), CDataUnitType::ReadInputRegisters, 
                       CalculateAddress, QVector<quint16>() << 110);
    m_modbusDevice->request(unit);
}
//-----------------------------------------
void ConfiguratorWindow::calibration_read()
{
    CDataUnitType unit(ui->sboxSlaveID->value(), CDataUnitType::ReadHoldingRegisters, 
                       CalibrationAddress, QVector<quint16>() << 36);
    m_modbusDevice->request(unit);
}
//------------------------------------------
void ConfiguratorWindow::calibration_write()
{
    QVector<quint16> data;
    
    union
    {
        quint16 b[2];
        float   v;
    } value;
    
    for(quint8 i = 0; i < m_calibration_cell.count(); i++)
    {
        value.v = m_calibration_cell.at(i)->text().toFloat();
        
        data.append(value.b[1]);
        data.append(value.b[0]);
    }
    
    CDataUnitType unit(ui->sboxSlaveID->value(), CDataUnitType::WriteMultipleRegisters, 
                       CalibrationAddress, data);
    m_modbusDevice->request(unit);
}
//----------------------------------------
void ConfiguratorWindow::protection_read()
{
    CDataUnitType unit(ui->sboxSlaveID->value(), CDataUnitType::ReadHoldingRegisters, 
                       ProtectionAddress, QVector<quint16>() << 4);
    m_modbusDevice->request(unit);
}
//-----------------------------------------
void ConfiguratorWindow::protection_write()
{
    QVector<quint16> data;
    
    data.append((quint16)ui->cboxProtectionMTZ1_Ctrl->currentIndex());
    data.append((quint16)ui->cboxProtectionMTZ2_Ctrl->currentIndex());
    data.append((quint16)ui->cboxProtectionMTZ3_Ctrl->currentIndex());
    data.append((quint16)ui->cboxProtectionMTZ4_Ctrl->currentIndex());
    
    CDataUnitType unit(ui->sboxSlaveID->value(), CDataUnitType::WriteMultipleRegisters, 
                       ProtectionAddress, data);
    m_modbusDevice->request(unit);
}
//--------------------------------------------------------
void ConfiguratorWindow::responseRead(CDataUnitType& unit)
{
    if(unit.is_empty())
        return;
    
    qDebug() << "Получен ответ: " << unit.valueCount();
    
    switch(unit.valueCount())
    {
        case 4: // чтение состояний токовых защит
            displayProtectionValues(unit.values());
        break;
        
        case 36: // чтение калибровок
            displayCalibrationValues(unit.values());
        break;
        
        case 110: // чтение расчетных величин
            displayCalculateValues(unit.values());
        break;
    }
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
    
    ui->gboxProtectionPropertiesMTZ2->hide();
    ui->gboxProtectionPropertiesMTZ3->hide();
    ui->gboxProtectionPropertiesMTZ4->hide();
}
//------------------------------------------------------------------
void ConfiguratorWindow::chboxCalculateTimeoutStateChanged(bool state)
{
    if(state)
    {
        m_tim_calculate->start(ui->sboxTimeoutCalc->value());
    }
    else
    {
        m_tim_calculate->stop();
    }
}
//--------------------------------------------------------
void ConfiguratorWindow::timeCalculateChanged(int newTime)
{
    if(m_tim_calculate->isActive())
    {
        m_tim_calculate->stop();
        m_tim_calculate->start(newTime);
    }
}
//-------------------------------------------------------
void ConfiguratorWindow::timeoutValueChanged(int newTime)
{
    m_modbusDevice->setTimeoutRepeat(newTime);
}
//------------------------------------------------------
void ConfiguratorWindow::numberRepeatChanged(int number)
{
    m_modbusDevice->setRequestCountRepeat(number);
}
//--------------------------------------------------
void ConfiguratorWindow::protectMTZChangedID(int id)
{
    if(id >= 0 && id < 4)
    {
        ui->gboxProtectionPropertiesMTZ1->hide();
        ui->gboxProtectionPropertiesMTZ2->hide();
        ui->gboxProtectionPropertiesMTZ3->hide();
        ui->gboxProtectionPropertiesMTZ4->hide();
        
        ui->pbtnProtectionMTZ1->setStyleSheet("QPushButton { background: none }");
        ui->pbtnProtectionMTZ2->setStyleSheet("QPushButton { background: none }");
        ui->pbtnProtectionMTZ3->setStyleSheet("QPushButton { background: none }");
        ui->pbtnProtectionMTZ4->setStyleSheet("QPushButton { background: none }");
        
        QPushButton* btn = qobject_cast<QPushButton*>(m_protect_mtz_group->button(id));
        
        btn->setStyleSheet(tr("QPushButton { background: green; color: yellow }"));
        
        switch(id)
        {
            case 0:
                ui->gboxProtectionPropertiesMTZ1->show();
            break;
            
            case 1:
                ui->gboxProtectionPropertiesMTZ2->show();
            break;
            
            case 2:
                ui->gboxProtectionPropertiesMTZ3->show();
            break;
            
            case 3:
                ui->gboxProtectionPropertiesMTZ4->show();
            break;
        }
    }
}
//--------------------------------------------------------
void ConfiguratorWindow::errorDevice(const QString& error)
{
    statusBar()->showMessage(error, 5000);
}
//----------------------------------------------------------------------
void ConfiguratorWindow::displayCalculateValues(QVector<quint16> values)
{
//    union
//    {
//        quint16 word[2];
//        float   value;
//    } cell_value;
    
//    for(quint8 i = 0, j = 0; i < values.count() - 1; i += 2, j++)
//    {
//        quint16 value1 = values.at(i);
//        quint16 value2 = values.at(i + 1);
        
//        cell_value.word[0] = value1;
//        cell_value.word[1] = value2;
        
//        m_calculate_cell.at(j)->setText(/*QString::number(cell_value.value, 'f', 6)*/"yes");
//    }
    m_panel->setData(values);
}
//------------------------------------------------------------------------
void ConfiguratorWindow::displayCalibrationValues(QVector<quint16> values)
{
    union
    {
        quint16 word[2];
        float   value;
    } cell_value;
    
    for(quint8 i = 0, j = 0; i < values.count() - 1; i += 2, j++)
    {
        quint16 value1 = values.at(i + 1);
        quint16 value2 = values.at(i);
        
        cell_value.word[0] = value1;
        cell_value.word[1] = value2;
        
        m_calibration_cell.at(j)->setText(QString::number(cell_value.value, 'f', 6));
    }
}
//-----------------------------------------------------------------------
void ConfiguratorWindow::displayProtectionValues(QVector<quint16> values)
{     
    if(values.count() == 4)
    {
        for(quint8 i = 0; i < values.count(); i++)
        {
            quint16 value = values.at(i);
            
            QComboBox* cbItem = (i == 0)?ui->cboxProtectionMTZ1_Ctrl:(i == 1)?ui->cboxProtectionMTZ2_Ctrl:
                                (i == 2)?ui->cboxProtectionMTZ3_Ctrl:ui->cboxProtectionMTZ4_Ctrl;
            
            cbItem->setCurrentIndex(value);
        }
    }
}
