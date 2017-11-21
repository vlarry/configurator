#include "configuratorwindow.h"
#include "ui_configuratorwindow.h"
//------------------------------------------------------
ConfiguratorWindow::ConfiguratorWindow(QWidget* parent):
    QMainWindow(parent),
    ui(new Ui::ConfiguratorWindow),
    m_modbusDevice(nullptr),
    m_panel(nullptr),
    m_tim_calculate(nullptr),
    m_protect_mtz_group(nullptr),
    m_terminal(nullptr),
    m_logFile(nullptr)
{
    ui->setupUi(this);

    m_modbusDevice              = new CModbus(this);
    m_panel                     = new QPanel(this);
    m_tim_calculate             = new QTimer(this);
    m_protect_mtz_group         = new QButtonGroup(ui->tabProtectionMTZ);
    m_protect_earthly_group     = new QButtonGroup(ui->tabProtectionEarthly);
    m_protect_power_group       = new QButtonGroup(ui->tabProtectionPower);
    m_protect_motor_group       = new QButtonGroup(ui->tabProtectionMotor);
    m_protect_frequency_group   = new QButtonGroup(ui->tabProtectionFrenquency);
    m_protect_external_group    = new QButtonGroup(ui->tabProtectionExternal);
    m_protect_temperature_group = new QButtonGroup(ui->tabProtectionTemperature);
    m_protect_level_group       = new QButtonGroup(ui->tabProtectionLevels);
    m_terminal                  = new CTerminal(this);
    m_logFile                   = new QFile("Log.txt");
    
    m_panel->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    m_panel->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
    addDockWidget(Qt::RightDockWidgetArea, m_panel);
    
    initButtonGroup();
    initConnect();
    
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
    if(m_logFile)
    {
        m_logFile->write("Завершение работы программы...\n\n");
                
        if(m_logFile->isOpen())
            m_logFile->close();
    }
    
    delete m_logFile;
    m_logFile = nullptr;
    
    delete m_terminal;
    m_terminal = nullptr;
    
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
    
    if(state)
    {
        if(!m_logFile->open(QFile::Append))
        {
            statusBar()->showMessage(tr("Ошибка. Невозможно открыть log-файл"));
        }
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
    emit m_modbusDevice->infoLog(tr("Получен ответ: ") + QString::number(unit.valueCount()) + tr(" байт \n"));
    
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
    ui->tabwgtProtections->setCurrentIndex(0);
    ui->tabwgtProtectionLevel->setCurrentIndex(0);
    
    setWindowState(Qt::WindowFullScreen);
    setWindowState(Qt::WindowMaximized);
    
    ui->gboxProtectionPropertiesMTZ2->hide();
    ui->gboxProtectionPropertiesMTZ3->hide();
    ui->gboxProtectionPropertiesMTZ4->hide();
    
    m_terminal->hide();
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
    if(id >= 0 && id < m_protect_mtz_group->buttons().count())
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
//------------------------------------------------------
void ConfiguratorWindow::protectEarthlyChangedID(int id)
{
    if(id >= 0 && id < m_protect_earthly_group->buttons().count())
    {
        ui->gboxProtectionPropertiesEarthly_OZZ1->hide();
        ui->gboxProtectionPropertiesEarthly_OZZ2->hide();
        ui->gboxProtectionPropertiesEarthly_NZZ1->hide();
        ui->gboxProtectionPropertiesEarthly_NZZ2->hide();
        
        ui->pbtnProtectionEarthly_OZZ1->setStyleSheet("QPushButton { background: none }");
        ui->pbtnProtectionEarthly_OZZ2->setStyleSheet("QPushButton { background: none }");
        ui->pbtnProtectionEarthly_NZZ1->setStyleSheet("QPushButton { background: none }");
        ui->pbtnProtectionEarthly_NZZ2->setStyleSheet("QPushButton { background: none }");
        
        QPushButton* btn = qobject_cast<QPushButton*>(m_protect_earthly_group->button(id));
        
        btn->setStyleSheet(tr("QPushButton { background: green; color: yellow }"));
        
        switch(id)
        {
            case 0:
                ui->gboxProtectionPropertiesEarthly_OZZ1->show();
            break;
            
            case 1:
                ui->gboxProtectionPropertiesEarthly_OZZ2->show();
            break;
            
            case 2:
                ui->gboxProtectionPropertiesEarthly_NZZ1->show();
            break;
            
            case 3:
                ui->gboxProtectionPropertiesEarthly_NZZ2->show();
            break;
        }
    }
}
//----------------------------------------------------
void ConfiguratorWindow::protectPowerChangedID(int id)
{
    if(id >= 0 && id < m_protect_power_group->buttons().count())
    {
        ui->gboxProtectionPropertiesPower_Umax1->hide();
        ui->gboxProtectionPropertiesPower_Umax2->hide();
        ui->gboxProtectionPropertiesPower_Umin1->hide();
        ui->gboxProtectionPropertiesPower_Umin2->hide();
        ui->gboxProtectionPropertiesPower_3UO->hide();
        
        ui->pbtnProtectionPower_Umax1->setStyleSheet("QPushButton { background: none }");
        ui->pbtnProtectionPower_Umax2->setStyleSheet("QPushButton { background: none }");
        ui->pbtnProtectionPower_Umin1->setStyleSheet("QPushButton { background: none }");
        ui->pbtnProtectionPower_Umin2->setStyleSheet("QPushButton { background: none }");
        ui->pbtnProtectionPower_3UO->setStyleSheet("QPushButton { background: none }");
        
        QPushButton* btn = qobject_cast<QPushButton*>(m_protect_power_group->button(id));
        
        btn->setStyleSheet(tr("QPushButton { background: green; color: yellow }"));
        
        switch(id)
        {
            case 0:
                ui->gboxProtectionPropertiesPower_Umax1->show();
            break;
            
            case 1:
                ui->gboxProtectionPropertiesPower_Umax2->show();
            break;
            
            case 2:
                ui->gboxProtectionPropertiesPower_Umin1->show();
            break;
            
            case 3:
                ui->gboxProtectionPropertiesPower_Umin2->show();
            break;
                
            case 4:
                ui->gboxProtectionPropertiesPower_3UO->show();
            break;
        }
    }
}
//----------------------------------------------------
void ConfiguratorWindow::protectMotorChangedID(int id)
{
    if(id >= 0 && id < m_protect_motor_group->buttons().count())
    {
        ui->gboxProtectionPropertiesMotor_StartingCurrent->hide();
        ui->gboxProtectionPropertiesMotor_Imin->hide();
        
        ui->pbtnProtectionMotorStarting->setStyleSheet("QPushButton { background: none }");
        ui->pbtnProtectionMotorImin->setStyleSheet("QPushButton { background: none }");
        
        QPushButton* btn = qobject_cast<QPushButton*>(m_protect_motor_group->button(id));
        
        btn->setStyleSheet(tr("QPushButton { background: green; color: yellow }"));
        
        switch(id)
        {
            case 0:
                ui->gboxProtectionPropertiesMotor_StartingCurrent->show();
            break;
            
            case 1:
                ui->gboxProtectionPropertiesMotor_Imin->show();
            break;
        }
    }
}
//--------------------------------------------------------
void ConfiguratorWindow::protectFrequencyChangedID(int id)
{
    if(id >= 0 && id < m_protect_frequency_group->buttons().count())
    {
        ui->gboxProtectionPropertiesFrequency_ACR1->hide();
        ui->gboxProtectionPropertiesFrequency_ACR2->hide();
        ui->gboxProtectionPropertiesFrequency_ACR3->hide();
        
        ui->pbtnProtectionFrequency_ACR1->setStyleSheet("QPushButton { background: none }");
        ui->pbtnProtectionFrequency_ACR2->setStyleSheet("QPushButton { background: none }");
        ui->pbtnProtectionFrequency_ACR3->setStyleSheet("QPushButton { background: none }");
        
        QPushButton* btn = qobject_cast<QPushButton*>(m_protect_frequency_group->button(id));
        
        btn->setStyleSheet(tr("QPushButton { background: green; color: yellow }"));
        
        switch(id)
        {
            case 0:
                ui->gboxProtectionPropertiesFrequency_ACR1->show();
            break;
            
            case 1:
                ui->gboxProtectionPropertiesFrequency_ACR2->show();
            break;
            
            case 2:
                ui->gboxProtectionPropertiesFrequency_ACR3->show();
            break;
        }
    }
}
//-------------------------------------------------------
void ConfiguratorWindow::protectExternalChangedID(int id)
{
    if(id >= 0 && id < m_protect_external_group->buttons().count())
    {
        ui->gboxProtectionPropertiesExternalArc->hide();
        ui->gboxProtectionPropertiesExternal1->hide();
        ui->gboxProtectionPropertiesExternal2->hide();
        ui->gboxProtectionPropertiesExternal3->hide();
        
        ui->pbtnProtectionExternal_Arc->setStyleSheet("QPushButton { background: none }");
        ui->pbtnProtectionExternal1->setStyleSheet("QPushButton { background: none }");
        ui->pbtnProtectionExternal2->setStyleSheet("QPushButton { background: none }");
        ui->pbtnProtectionExternal3->setStyleSheet("QPushButton { background: none }");
        
        QPushButton* btn = qobject_cast<QPushButton*>(m_protect_external_group->button(id));
        
        btn->setStyleSheet(tr("QPushButton { background: green; color: yellow }"));
        
        switch(id)
        {
            case 0:
                ui->gboxProtectionPropertiesExternalArc->show();
            break;
            
            case 1:
                ui->gboxProtectionPropertiesExternal1->show();
            break;
            
            case 2:
                ui->gboxProtectionPropertiesExternal2->show();
            break;
            
            case 3:
                ui->gboxProtectionPropertiesExternal3->show();
            break;
        }
    }
}
//----------------------------------------------------------
void ConfiguratorWindow::protectTemperatureChangedID(int id)
{
    if(id >= 0 && id < m_protect_temperature_group->buttons().count())
    {
        ui->gboxProtectionPropertiesTemperature1->hide();
        ui->gboxProtectionPropertiesTemperature2->hide();
        
        ui->pbtnProtectionTemp1->setStyleSheet("QPushButton { background: none }");
        ui->pbtnProtectionTemp2->setStyleSheet("QPushButton { background: none }");
        
        QPushButton* btn = qobject_cast<QPushButton*>(m_protect_temperature_group->button(id));
        
        btn->setStyleSheet(tr("QPushButton { background: green; color: yellow }"));
        
        switch(id)
        {
            case 0:
                ui->gboxProtectionPropertiesTemperature1->show();
            break;
            
            case 1:
                ui->gboxProtectionPropertiesTemperature2->show();
            break;
        }
    }
}
//----------------------------------------------------
void ConfiguratorWindow::protectLevelChangedID(int id)
{
    if(id >= 0 && id < m_protect_level_group->buttons().count())
    {
        ui->gboxProtectionPropertiesLevel1->hide();
        ui->gboxProtectionPropertiesLevel2->hide();
        
        ui->pbtnProtectionLevel1->setStyleSheet("QPushButton { background: none }");
        ui->pbtnProtectionLevel2->setStyleSheet("QPushButton { background: none }");
        
        QPushButton* btn = qobject_cast<QPushButton*>(m_protect_level_group->button(id));
        
        btn->setStyleSheet(tr("QPushButton { background: green; color: yellow }"));
        
        switch(id)
        {
            case 0:
                ui->gboxProtectionPropertiesLevel1->show();
            break;
            
            case 1:
                ui->gboxProtectionPropertiesLevel2->show();
            break;
        }
    }
}
//--------------------------------------------------------
void ConfiguratorWindow::errorDevice(const QString& error)
{
    statusBar()->showMessage(error, 5000);
}
//---------------------------------------------------
void ConfiguratorWindow::terminalVisiblity(int state)
{
    if(state == Qt::Checked)
        m_terminal->show();
    else if(state == Qt::Unchecked)
        m_terminal->hide();
    
    ui->chboxTerminal->setCheckState((Qt::CheckState)state);
}
//---------------------------------------------------
void ConfiguratorWindow::saveLog(const QString& info)
{
    qDebug() << info;
    
    if(m_logFile->isOpen())
    {
        m_logFile->write(info.toStdString().c_str());
    }
}
//----------------------------------------
void ConfiguratorWindow::initButtonGroup()
{
    // группа токовых защит
    m_protect_mtz_group->addButton(ui->pbtnProtectionMTZ1);
    m_protect_mtz_group->addButton(ui->pbtnProtectionMTZ2);
    m_protect_mtz_group->addButton(ui->pbtnProtectionMTZ3);
    m_protect_mtz_group->addButton(ui->pbtnProtectionMTZ4);
    m_protect_mtz_group->setId(ui->pbtnProtectionMTZ1, 0);
    m_protect_mtz_group->setId(ui->pbtnProtectionMTZ2, 1);
    m_protect_mtz_group->setId(ui->pbtnProtectionMTZ3, 2);
    m_protect_mtz_group->setId(ui->pbtnProtectionMTZ4, 3);
    
    // группа земляных защит
    m_protect_earthly_group->addButton(ui->pbtnProtectionEarthly_OZZ1);
    m_protect_earthly_group->addButton(ui->pbtnProtectionEarthly_OZZ2);
    m_protect_earthly_group->addButton(ui->pbtnProtectionEarthly_NZZ1);
    m_protect_earthly_group->addButton(ui->pbtnProtectionEarthly_NZZ2);
    m_protect_earthly_group->setId(ui->pbtnProtectionEarthly_OZZ1, 0);
    m_protect_earthly_group->setId(ui->pbtnProtectionEarthly_OZZ2, 1);
    m_protect_earthly_group->setId(ui->pbtnProtectionEarthly_NZZ1, 2);
    m_protect_earthly_group->setId(ui->pbtnProtectionEarthly_NZZ2, 3);
    
    // группа защит по напряжению
    m_protect_power_group->addButton(ui->pbtnProtectionPower_Umax1);
    m_protect_power_group->addButton(ui->pbtnProtectionPower_Umax2);
    m_protect_power_group->addButton(ui->pbtnProtectionPower_Umin1);
    m_protect_power_group->addButton(ui->pbtnProtectionPower_Umin2);
    m_protect_power_group->addButton(ui->pbtnProtectionPower_3UO);
    m_protect_power_group->setId(ui->pbtnProtectionPower_Umax1, 0);
    m_protect_power_group->setId(ui->pbtnProtectionPower_Umax2, 1);
    m_protect_power_group->setId(ui->pbtnProtectionPower_Umin1, 2);
    m_protect_power_group->setId(ui->pbtnProtectionPower_Umin2, 3);
    m_protect_power_group->setId(ui->pbtnProtectionPower_3UO, 4);
    
    // группа защиты двигателей
    m_protect_motor_group->addButton(ui->pbtnProtectionMotorStarting);
    m_protect_motor_group->addButton(ui->pbtnProtectionMotorImin);
    m_protect_motor_group->setId(ui->pbtnProtectionMotorStarting, 0);
    m_protect_motor_group->setId(ui->pbtnProtectionMotorImin, 1);
    
    // группа частотных защит
    m_protect_frequency_group->addButton(ui->pbtnProtectionFrequency_ACR1);
    m_protect_frequency_group->addButton(ui->pbtnProtectionFrequency_ACR2);
    m_protect_frequency_group->addButton(ui->pbtnProtectionFrequency_ACR3);
    m_protect_frequency_group->setId(ui->pbtnProtectionFrequency_ACR1, 0);
    m_protect_frequency_group->setId(ui->pbtnProtectionFrequency_ACR2, 1);
    m_protect_frequency_group->setId(ui->pbtnProtectionFrequency_ACR3, 2);
    
    // группа внешних защит
    m_protect_external_group->addButton(ui->pbtnProtectionExternal_Arc);
    m_protect_external_group->addButton(ui->pbtnProtectionExternal1);
    m_protect_external_group->addButton(ui->pbtnProtectionExternal2);
    m_protect_external_group->addButton(ui->pbtnProtectionExternal3);
    m_protect_external_group->addButton(ui->pbtnProtectionExternal_Arc, 0);
    m_protect_external_group->addButton(ui->pbtnProtectionExternal1, 1);
    m_protect_external_group->addButton(ui->pbtnProtectionExternal2, 2);
    m_protect_external_group->addButton(ui->pbtnProtectionExternal3, 3);
    
    // группа температурных защит
    m_protect_temperature_group->addButton(ui->pbtnProtectionTemp1);
    m_protect_temperature_group->addButton(ui->pbtnProtectionTemp2);
    m_protect_temperature_group->setId(ui->pbtnProtectionTemp1, 0);
    m_protect_temperature_group->setId(ui->pbtnProtectionTemp2, 1);
    
    // группа уровневых защит
    m_protect_level_group->addButton(ui->pbtnProtectionLevel1);
    m_protect_level_group->addButton(ui->pbtnProtectionLevel2);
    m_protect_level_group->setId(ui->pbtnProtectionLevel1, 0);
    m_protect_level_group->setId(ui->pbtnProtectionLevel2, 1);
    
    protectMTZChangedID(0);
    protectEarthlyChangedID(0);
    protectPowerChangedID(0);
    protectMotorChangedID(0);
    protectFrequencyChangedID(0);
    protectExternalChangedID(0);
    protectTemperatureChangedID(0);
    protectLevelChangedID(0);
    
    m_protect_mtz_group->setExclusive(true);
    m_protect_earthly_group->setExclusive(true);
    m_protect_power_group->setExclusive(true);
    m_protect_motor_group->setExclusive(true);
    m_protect_frequency_group->setExclusive(true);
    m_protect_external_group->setExclusive(true);
    m_protect_temperature_group->setExclusive(true);
    m_protect_level_group->setExclusive(true);
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
//------------------------------------
void ConfiguratorWindow::initConnect()
{
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
    connect(m_protect_earthly_group, SIGNAL(buttonClicked(int)), this, SLOT(protectEarthlyChangedID(int)));
    connect(m_protect_power_group, SIGNAL(buttonClicked(int)), this, SLOT(protectPowerChangedID(int)));
    connect(m_protect_motor_group, SIGNAL(buttonClicked(int)), this, SLOT(protectMotorChangedID(int)));
    connect(m_protect_frequency_group, SIGNAL(buttonClicked(int)), this, SLOT(protectFrequencyChangedID(int)));
    connect(m_protect_external_group, SIGNAL(buttonClicked(int)), this, SLOT(protectExternalChangedID(int)));
    connect(m_protect_temperature_group, SIGNAL(buttonClicked(int)), this, SLOT(protectTemperatureChangedID(int)));
    connect(m_protect_level_group, SIGNAL(buttonClicked(int)), this, SLOT(protectLevelChangedID(int)));
    connect(ui->pbtnReadProtection, &QPushButton::clicked, this, &ConfiguratorWindow::protection_read);
    connect(ui->pbtnWriteProtection, &QPushButton::clicked, this, &ConfiguratorWindow::protection_write);
    connect(m_modbusDevice, &CModbus::errorDevice, this, &ConfiguratorWindow::errorDevice);
    connect(ui->sboxTimeout, SIGNAL(valueChanged(int)), this, SLOT(timeoutValueChanged(int)));
    connect(ui->sboxNumRepeat, SIGNAL(valueChanged(int)), this, SLOT(numberRepeatChanged(int)));
    connect(ui->chboxTerminal, &QCheckBox::stateChanged, this, &ConfiguratorWindow::terminalVisiblity);
    connect(m_modbusDevice, &CModbus::rawData, m_terminal, &CTerminal::appendData);
    connect(m_terminal, &CTerminal::closeTerminal, this, &ConfiguratorWindow::terminalVisiblity);
    connect(m_modbusDevice, &CModbus::infoLog, this, &ConfiguratorWindow::saveLog);
}
