#include "configuratorwindow.h"
#include "ui_configuratorwindow.h"
//------------------------------------------------------
ConfiguratorWindow::ConfiguratorWindow(QWidget* parent):
    QMainWindow(parent),
    ui(new Ui::ConfiguratorWindow),
    m_modbusDevice(nullptr),
    m_calculateWidget(nullptr),
    m_terminal(nullptr),
    m_logFile(nullptr),
    m_tim_calculate(nullptr),
    m_protect_mtz_group(nullptr),
    m_protect_earthly_group(nullptr),
    m_protect_power_group(nullptr),
    m_protect_motor_group(nullptr),
    m_protect_frequency_group(nullptr),
    m_protect_external_group(nullptr),
    m_protect_temperature_group(nullptr),
    m_protect_level_group(nullptr),
    m_additional_group(nullptr)
{
    ui->setupUi(this);

    m_modbusDevice              = new CModbus(this);
    m_calculateWidget           = new QPanel(this);
    m_tim_calculate             = new QTimer(this);
    m_protect_mtz_group         = new QButtonGroup;
    m_protect_earthly_group     = new QButtonGroup;
    m_protect_power_group       = new QButtonGroup;
    m_protect_motor_group       = new QButtonGroup;
    m_protect_frequency_group   = new QButtonGroup;
    m_protect_external_group    = new QButtonGroup;
    m_protect_temperature_group = new QButtonGroup;
    m_protect_level_group       = new QButtonGroup;
    m_switch_device_group       = new QButtonGroup;
    m_additional_group          = new QButtonGroup;
    m_terminal                  = new CTerminal(this);
    m_logFile                   = new QFile("Log.txt");
    
    m_calculateWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    m_calculateWidget->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
    addDockWidget(Qt::RightDockWidgetArea, m_calculateWidget);
    
    initMenuPanel();
    initButtonGroup();
    initConnect();

    if(!m_logFile->open(QFile::ReadWrite))
    {
        statusBar()->showMessage(tr("Ошибка. Невозможно открыть log-файл"));
    }
    else
        m_logFile->write(tr("Запуск программы...").toStdString().c_str());

    refreshSerialPort();

    m_in_analog_cell.append(ui->leMainKtt);
    m_in_analog_cell.append(ui->leMainKt0);
    m_in_analog_cell.append(ui->leMainKtn);
    m_in_analog_cell.append(ui->leTextCalibFactorCurrentPhase_A);
    m_in_analog_cell.append(ui->leTextCalibFactorCurrentPhase_B);
    m_in_analog_cell.append(ui->leTextCalibFactorCurrentPhase_C);
    m_in_analog_cell.append(ui->leTextCalibFactorCurrent3I0);
    m_in_analog_cell.append(ui->leTextCalibFactorPowerPhase_A);
    m_in_analog_cell.append(ui->leTextCalibFactorPowerPhase_B);
    m_in_analog_cell.append(ui->leTextCalibFactorPowerPhase_C);
    m_in_analog_cell.append(ui->leTextCalibFactorPower3I0);
    m_in_analog_cell.append(ui->leTextCalibFactorPowerTotal);
    m_in_analog_cell.append(ui->leTextCalibFactorPowerPhase_A_B);
    m_in_analog_cell.append(ui->leTextCalibFactorPowerPhase_B_C);
    m_in_analog_cell.append(ui->leTextCalibFactorPowerPhase_C_A);
    m_in_analog_cell.append(ui->leTextCalibFactorPower3U0x);
    m_in_analog_cell.append(ui->leTextCalibFactorPowerUAx);
    m_in_analog_cell.append(ui->leTextCalibFactorPowerUBx);
    m_in_analog_cell.append(ui->leTextCalibFactorPowerUCx);
    m_in_analog_cell.append(ui->leTextCalibFactorChannel3U0);
    m_in_analog_cell.append(ui->leTextCalibFactorChannel3Us);

    m_protectionMTZ_cell.append(ui->leProtectionMTZ1_CurrentStart);
    m_protectionMTZ_cell.append(ui->leProtectionMTZ1_Timeout);
    m_protectionMTZ_cell.append(ui->leProtectionMTZ1_EnterBlocking);
    m_protectionMTZ_cell.append(ui->leProtectionMTZ1_CurrentKvz);
    m_protectionMTZ_cell.append(ui->leProtectionMTZ2_CurrentStart);
    m_protectionMTZ_cell.append(ui->leProtectionMTZ2_Timeout);
    m_protectionMTZ_cell.append(ui->leProtectionMTZ2_Acceleration);
    m_protectionMTZ_cell.append(ui->leProtectionMTZ2_CurrentKvz);
    m_protectionMTZ_cell.append(ui->leProtectionMTZ3_CurrentStart);
    m_protectionMTZ_cell.append(ui->leProtectionMTZ3_Timeout);
    m_protectionMTZ_cell.append(ui->leProtectionMTZ3_Factor);
    m_protectionMTZ_cell.append(ui->leProtectionMTZ3_CurrentKvz);
    m_protectionMTZ_cell.append(ui->leProtectionMTZ4_CurrentStart);
    m_protectionMTZ_cell.append(ui->leProtectionMTZ4_Timeout);
    m_protectionMTZ_cell.append(ui->leProtectionMTZ4_Angle);
    m_protectionMTZ_cell.append(ui->leProtectionMTZ4_PowerStart);
    m_protectionMTZ_cell.append(ui->leProtectionMTZ4_CurrentKvz);
    m_protectionMTZ_cell.append(ui->leProtectionMTZ4_PowerKvz);

    m_protectionEarthy_cell.append(ui->leProtectionEarthly_OZZ1_StartCurrent);
    m_protectionEarthy_cell.append(ui->leProtectionEarthly_OZZ1_Pause);
    m_protectionEarthy_cell.append(ui->leProtectionEarthly_OZZ1_KvzCurrent);
    m_protectionEarthy_cell.append(ui->leProtectionEarthly_OZZ2_StartCurrent);
    m_protectionEarthy_cell.append(ui->leProtectionEarthly_OZZ2_Pause);
    m_protectionEarthy_cell.append(ui->leProtectionEarthly_OZZ2_KvzCurrent);
    m_protectionEarthy_cell.append(ui->leProtectionEarthly_NZZ1_StartCurrent);
    m_protectionEarthy_cell.append(ui->leProtectionEarthly_NZZ1_StartPower);
    m_protectionEarthy_cell.append(ui->leProtectionEarthly_NZZ1_Angle);
    m_protectionEarthy_cell.append(ui->leProtectionEarthly_NZZ1_Pause);
    m_protectionEarthy_cell.append(ui->leProtectionEarthly_NZZ1_KvzCurrent);
    m_protectionEarthy_cell.append(ui->leProtectionEarthly_NZZ1_KvzPower);
    m_protectionEarthy_cell.append(ui->leProtectionEarthly_NZZ2_StartCurrent);
    m_protectionEarthy_cell.append(ui->leProtectionEarthly_NZZ2_StartPower);
    m_protectionEarthy_cell.append(ui->leProtectionEarthly_NZZ2_Angle);
    m_protectionEarthy_cell.append(ui->leProtectionEarthly_NZZ2_Pause);
    m_protectionEarthy_cell.append(ui->leProtectionEarthly_NZZ2_KvzCurrent);
    m_protectionEarthy_cell.append(ui->leProtectionEarthly_NZZ2_KvzPower);

    m_protectionPower_cell.append(ui->leProtectionPower_3UO_StartCurrent);
    m_protectionPower_cell.append(ui->leProtectionPower_3UO_Pause);
    m_protectionPower_cell.append(ui->leProtectionPower_3UO_KvzPower);
    m_protectionPower_cell.append(ui->leProtectionPower_Umax1_StartPower);
    m_protectionPower_cell.append(ui->leProtectionPower_Umax1_Pause);
    m_protectionPower_cell.append(ui->leProtectionPower_Umax1_KvzPower);
    m_protectionPower_cell.append(ui->leProtectionPower_Umax2_StartPower);
    m_protectionPower_cell.append(ui->leProtectionPower_Umax2_Pause);
    m_protectionPower_cell.append(ui->leProtectionPower_Umax2_KvzPower);
    m_protectionPower_cell.append(ui->leProtectionPower_Umin1_StartPower);
    m_protectionPower_cell.append(ui->leProtectionPower_Umin1_Pause);
    m_protectionPower_cell.append(ui->leProtectionPower_Umin1_KvzPower);
    m_protectionPower_cell.append(ui->leProtectionPower_Umin2_StartPower);
    m_protectionPower_cell.append(ui->leProtectionPower_Umin2_Pause);
    m_protectionPower_cell.append(ui->leProtectionPower_Umin2_KvzPower);

    m_protectionMotor_cell.append(ui->leProtectionMotorStarting_StartCurrent);
    m_protectionMotor_cell.append(ui->leProtectionMotorStarting_Pause);
    m_protectionMotor_cell.append(ui->leProtectionMotorStarting_KvzCurrent);
    m_protectionMotor_cell.append(ui->leProtectionMotorImin_StartPower);
    m_protectionMotor_cell.append(ui->leProtectionMotorImin_Pause);
    m_protectionMotor_cell.append(ui->leProtectionMotorImin_KvzPower);

    m_protectionFrequency_cell.append(ui->leProtectionFrequency_ACR1_StartFreq);
    m_protectionFrequency_cell.append(ui->leProtectionFrequency_ACR1_Pause);
    m_protectionFrequency_cell.append(ui->leProtectionFrequency_ACR1_Umin);
    m_protectionFrequency_cell.append(ui->leProtectionFrequency_ACR1_KvzPower);
    m_protectionFrequency_cell.append(ui->leProtectionFrequency_ACR2_StartFreq);
    m_protectionFrequency_cell.append(ui->leProtectionFrequency_ACR2_Pause);
    m_protectionFrequency_cell.append(ui->leProtectionFrequency_ACR2_Umin);
    m_protectionFrequency_cell.append(ui->leProtectionFrequency_ACR2_KvzPower);
    m_protectionFrequency_cell.append(ui->leProtectionFrequency_ACR3_StartFreq);
    m_protectionFrequency_cell.append(ui->leProtectionFrequency_ACR3_Pause);
    m_protectionFrequency_cell.append(ui->leProtectionFrequency_ACR3_Umin);
    m_protectionFrequency_cell.append(ui->leProtectionFrequency_ACR3_KvzPower);

    m_protectionExternal_cell.append(ui->leProtectionExternal_Arc_CurrentStart);
    m_protectionExternal_cell.append(ui->leProtectionExternal_Arc_KvzCurrent);
    m_protectionExternal_cell.append(ui->leProtectionExternal_Ext1_Pause);
    m_protectionExternal_cell.append(ui->leProtectionExternal_Ext2_Pause);
    m_protectionExternal_cell.append(ui->leProtectionExternal_Ext3_Pause);

    m_protectionTemperature_cell.append(ui->leProtectionTemp1_StartTemp);
    m_protectionTemperature_cell.append(ui->leProtectionTemp1_Pause);
    m_protectionTemperature_cell.append(ui->leProtectionTemp1_KvzTemp);
    m_protectionTemperature_cell.append(ui->leProtectionTemp2_StartTemp);
    m_protectionTemperature_cell.append(ui->leProtectionTemp2_Pause);
    m_protectionTemperature_cell.append(ui->leProtectionTemp2_KvzTemp);

    m_protectionLevel_cell.append(ui->leProtectionLevel1_Pause);
    m_protectionLevel_cell.append(ui->leProtectionLevel2_Pause);

    m_protectionBru_cell.append(ui->leProtectionBRUTmeasure);
    m_protectionBru_cell.append(ui->leProtectionBRUSet);
    m_protectionBru_cell.append(ui->leProtectionBRUOffPause);
    m_protectionBru_cell.append(ui->leProtectionBRUDiscret);

    m_protectionVacuum_cell.append(ui->leProtectionVacuumSet);
    m_protectionVacuum_cell.append(ui->leProtectionVacuumPause);
    m_protectionVacuum_cell.append(ui->leProtectionVacuumKvzPower);

    m_automation_cell.append(ui->leAdditionalAVRCtrlUwork);
    m_automation_cell.append(ui->leAdditionalAVRUrzr);
    m_automation_cell.append(ui->leAdditionalAVRPause);
    m_automation_cell.append(ui->leAdditionalAVRTime);
    m_automation_cell.append(ui->leAdditionalAPVTBlockOn);
    m_automation_cell.append(ui->leAdditionalAPVTime);

    m_switch_device_cell.append(ui->leSwitchOFF_TCmdOn);
    m_switch_device_cell.append(ui->leSwitchOFF_TCmdOff);
    m_switch_device_cell.append(ui->leSwitchOFF_Tcallback);
    m_switch_device_cell.append(ui->leSwitchOFF_I_OnState);
    m_switch_device_cell.append(ui->leKCU_Tdefect);
    m_switch_device_cell.append(ui->leKCU_Ikvz);
    m_switch_device_cell.append(ui->leDisconnect_Timp);
    m_switch_device_cell.append(ui->leDisconnectKNC_Tpause);
    m_switch_device_cell.append(ui->leSCHR_TCmdCtrl);
    m_switch_device_cell.append(ui->leSCHR_Tcallback);
    m_switch_device_cell.append(ui->leLR_TCmdCtrl);
    m_switch_device_cell.append(ui->leLR_Tcallback);
    m_switch_device_cell.append(ui->leZR_TCmdCtrl);
    m_switch_device_cell.append(ui->leZR_Tcallback);
    m_switch_device_cell.append(ui->leTruck_TCmdCtrl);
    m_switch_device_cell.append(ui->leTruck_Tcallback);
    m_switch_device_cell.append(ui->leCtrl_TSet1);
    m_switch_device_cell.append(ui->leCtrl_TSet2);

    for(QLineEdit* ledit: m_in_analog_cell)
    {
        ledit->setValidator(new QDoubleValidator);
    }

    for(QLineEdit* ledit: m_protectionMTZ_cell)
    {
        if(ledit != ui->leProtectionMTZ4_Angle)
            ledit->setValidator(new QDoubleValidator);
        else
            ledit->setValidator(new QIntValidator);
    }

    for(QLineEdit* ledit: m_protectionEarthy_cell)
    {
        if(ledit != ui->leProtectionEarthly_NZZ1_Angle || ledit != ui->leProtectionEarthly_NZZ2_Angle)
            ledit->setValidator(new QDoubleValidator);
        else
            ledit->setValidator(new QIntValidator);
    }

    for(QLineEdit* ledit: m_protectionPower_cell)
    {
        ledit->setValidator(new QDoubleValidator);
    }

    for(QLineEdit* ledit: m_protectionMotor_cell)
    {
        ledit->setValidator(new QDoubleValidator);
    }

    for(QLineEdit* ledit: m_protectionFrequency_cell)
    {
        ledit->setValidator(new QDoubleValidator);
    }

    for(QLineEdit* ledit: m_protectionExternal_cell)
    {
        ledit->setValidator(new QDoubleValidator);
    }

    for(QLineEdit* ledit: m_protectionTemperature_cell)
    {
        ledit->setValidator(new QDoubleValidator);
    }

    for(QLineEdit* ledit: m_protectionLevel_cell)
    {
        ledit->setValidator(new QDoubleValidator);
    }

    for(QLineEdit* ledit: m_protectionBru_cell)
    {
        ledit->setValidator(new QDoubleValidator);
    }

    for(QLineEdit* ledit: m_protectionVacuum_cell)
    {
        ledit->setValidator(new QDoubleValidator);
    }

    for(QLineEdit* ledit: m_automation_cell)
    {
        ledit->setValidator(new QDoubleValidator);
    }

    for(QLineEdit* ledit: m_switch_device_cell)
    {
        ledit->setValidator(new QDoubleValidator);
    }
}
//---------------------------------------
ConfiguratorWindow::~ConfiguratorWindow()
{
    if(m_logFile)
    {
        if(m_logFile->isOpen())
        {
            m_logFile->write("Завершение работы программы...\n\n");
            m_logFile->close();
        }
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

    if(state)
        saveLog(tr("Порт <") + m_modbusDevice->portName() + tr("> открыт."));
    else
        saveLog(tr("Порт <") + m_modbusDevice->portName() + tr("> закрыт."));
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
//--------------------------------------
void ConfiguratorWindow::calculateRead()
{
    CDataUnitType unit(ui->sboxSlaveID->value(), CDataUnitType::ReadInputRegisters, 
                       CALCULATE_ADDRESS, QVector<quint16>() << 110);
    unit.setProperty(tr("REQUEST"), CALCULATE_TYPE);

    m_modbusDevice->request(unit);
}
//-------------------------------------
void ConfiguratorWindow::inAnalogRead()
{
    CDataUnitType unit(ui->sboxSlaveID->value(), CDataUnitType::ReadHoldingRegisters, 
                       IN_ANALOG_ADDRESS, QVector<quint16>() << 42);
    unit.setProperty(tr("REQUEST"), IN_ANALOG_TYPE);

    m_modbusDevice->request(unit);
}
//--------------------------------------
void ConfiguratorWindow::inAnalogWrite()
{
    QVector<quint16> data;
    
    union
    {
        quint16 b[2];
        float   v;
    } value;
    
    for(quint8 i = 0; i < m_in_analog_cell.count(); i++)
    {
        value.v = m_in_analog_cell.at(i)->text().toFloat();
        
        data.append(value.b[1]);
        data.append(value.b[0]);
    }
    
    CDataUnitType unit(ui->sboxSlaveID->value(), CDataUnitType::WriteMultipleRegisters, 
                       IN_ANALOG_ADDRESS, data);
    unit.setProperty(tr("REQUEST"), IN_ANALOG_TYPE);

    m_modbusDevice->request(unit);
}
//---------------------------------------
void ConfiguratorWindow::protectionMTZStateRead()
{
    CDataUnitType unit(ui->sboxSlaveID->value(), CDataUnitType::ReadHoldingRegisters, 
                       PROTECTION_MTZ_ADDRESS, QVector<quint16>() << 4);
    unit.setProperty(tr("REQUEST"), PROTECTION_MTZ_TYPE);

    m_modbusDevice->request(unit);
}
//----------------------------------------
void ConfiguratorWindow::protectionMTZStateWrite()
{
    QVector<quint16> data;
    
    data.append((quint16)ui->cboxProtectionMTZ1_Ctrl->currentIndex());
    data.append((quint16)ui->cboxProtectionMTZ2_Ctrl->currentIndex());
    data.append((quint16)ui->cboxProtectionMTZ3_Ctrl->currentIndex());
    data.append((quint16)ui->cboxProtectionMTZ4_Ctrl->currentIndex());
    
    CDataUnitType unit(ui->sboxSlaveID->value(), CDataUnitType::WriteMultipleRegisters, 
                       PROTECTION_MTZ_ADDRESS, data);
    unit.setProperty(tr("REQUEST"), PROTECTION_MTZ_TYPE);

    m_modbusDevice->request(unit);
}
//---------------------------------------------
void ConfiguratorWindow::protectionMTZSetRead()
{
    CDataUnitType unit(ui->sboxSlaveID->value(), CDataUnitType::ReadHoldingRegisters,
                       PROTECTION_MTZ_SET_ADDRESS, QVector<quint16>() << 38);
    unit.setProperty(tr("REQUEST"), PROTECTION_MTZ_SET_TYPE);

    m_modbusDevice->request(unit);
}
//----------------------------------------------
void ConfiguratorWindow::protectionMTZSetWrite()
{
    QVector<quint16> data;

    union
    {
        quint16 b[2];
        float   v;
    } value;

    for(quint8 i = 0, j = 0; i < m_protectionMTZ_cell.count() + 1; i++)
    {
        if(i == 4)
            value.v = 0;
        else
            value.v = m_protectionMTZ_cell.at(j++)->text().toFloat();

        data.append(value.b[1]);
        data.append(value.b[0]);
    }

    CDataUnitType unit(ui->sboxSlaveID->value(), CDataUnitType::WriteMultipleRegisters,
                       PROTECTION_MTZ_SET_ADDRESS, data);
    unit.setProperty(tr("REQUEST"), PROTECTION_MTZ_SET_TYPE);

    m_modbusDevice->request(unit);
}
//------------------------------------------------
void ConfiguratorWindow::protectionEarthySetRead()
{
    CDataUnitType unit(ui->sboxSlaveID->value(), CDataUnitType::ReadHoldingRegisters,
                       PROTECTION_EARTHY_SET_ADDRESS, QVector<quint16>() << 36);
    unit.setProperty(tr("REQUEST"), PROTECTION_EARTHY_SET_TYPE);

    m_modbusDevice->request(unit);
}
//-------------------------------------------------
void ConfiguratorWindow::protectionEarthySetWrite()
{
    QVector<quint16> data;

    union
    {
        quint16 b[2];
        float   v;
    } value;

    for(quint8 i = 0, j = 0; i < m_protectionEarthy_cell.count(); i++)
    {
        value.v = m_protectionEarthy_cell.at(j++)->text().toFloat();

        data.append(value.b[1]);
        data.append(value.b[0]);
    }

    CDataUnitType unit(ui->sboxSlaveID->value(), CDataUnitType::WriteMultipleRegisters,
                       PROTECTION_EARTHY_SET_ADDRESS, data);
    unit.setProperty(tr("REQUEST"), PROTECTION_EARTHY_SET_TYPE);

    m_modbusDevice->request(unit);
}
//-----------------------------------------------
void ConfiguratorWindow::protectionPowerSetRead()
{
    CDataUnitType unit(ui->sboxSlaveID->value(), CDataUnitType::ReadHoldingRegisters,
                       PROTECTION_POWER_SET_ADDRESS, QVector<quint16>() << 30);
    unit.setProperty(tr("REQUEST"), PROTECTION_POWER_SET_TYPE);

    m_modbusDevice->request(unit);
}
//------------------------------------------------
void ConfiguratorWindow::protectionPowerSetWrite()
{
    QVector<quint16> data;

    union
    {
        quint16 b[2];
        float   v;
    } value;

    for(quint8 i = 0, j = 0; i < m_protectionPower_cell.count(); i++)
    {
        value.v = m_protectionPower_cell.at(j++)->text().toFloat();

        data.append(value.b[1]);
        data.append(value.b[0]);
    }

    CDataUnitType unit(ui->sboxSlaveID->value(), CDataUnitType::WriteMultipleRegisters,
                       PROTECTION_POWER_SET_ADDRESS, data);
    unit.setProperty(tr("REQUEST"), PROTECTION_POWER_SET_TYPE);

    m_modbusDevice->request(unit);
}
//-----------------------------------------------
void ConfiguratorWindow::protectionMotorSetRead()
{
    CDataUnitType unit(ui->sboxSlaveID->value(), CDataUnitType::ReadHoldingRegisters,
                       PROTECTION_MOTOR_SET_ADDRESS, QVector<quint16>() << 12);
    unit.setProperty(tr("REQUEST"), PROTECTION_MOTOR_SET_TYPE);

    m_modbusDevice->request(unit);
}
//------------------------------------------------
void ConfiguratorWindow::protectionMotorSetWrite()
{
    QVector<quint16> data;

    union
    {
        quint16 b[2];
        float   v;
    } value;

    for(quint8 i = 0, j = 0; i < m_protectionMotor_cell.count(); i++)
    {
        value.v = m_protectionMotor_cell.at(j++)->text().toFloat();

        data.append(value.b[1]);
        data.append(value.b[0]);
    }

    CDataUnitType unit(ui->sboxSlaveID->value(), CDataUnitType::WriteMultipleRegisters,
                       PROTECTION_MOTOR_SET_ADDRESS, data);
    unit.setProperty(tr("REQUEST"), PROTECTION_MOTOR_SET_TYPE);

    m_modbusDevice->request(unit);
}
//---------------------------------------------------
void ConfiguratorWindow::protectionFrequencySetRead()
{
    CDataUnitType unit(ui->sboxSlaveID->value(), CDataUnitType::ReadHoldingRegisters,
                       PROTECTION_FREQUENCY_SET_ADDRESS, QVector<quint16>() << 24);
    unit.setProperty(tr("REQUEST"), PROTECTION_FREQUENCY_SET_TYPE);

    m_modbusDevice->request(unit);
}
//----------------------------------------------------
void ConfiguratorWindow::protectionFrequencySetWrite()
{
    QVector<quint16> data;

    union
    {
        quint16 b[2];
        float   v;
    } value;

    for(quint8 i = 0, j = 0; i < m_protectionFrequency_cell.count(); i++)
    {
        value.v = m_protectionFrequency_cell.at(j++)->text().toFloat();

        data.append(value.b[1]);
        data.append(value.b[0]);
    }

    CDataUnitType unit(ui->sboxSlaveID->value(), CDataUnitType::WriteMultipleRegisters,
                       PROTECTION_FREQUENCY_SET_ADDRESS, data);
    unit.setProperty(tr("REQUEST"), PROTECTION_FREQUENCY_SET_TYPE);

    m_modbusDevice->request(unit);
}
//--------------------------------------------------
void ConfiguratorWindow::protectionExternalSetRead()
{
    CDataUnitType unit(ui->sboxSlaveID->value(), CDataUnitType::ReadHoldingRegisters,
                       PROTECTION_EXTERNAL_SET_ADDRESS, QVector<quint16>() << 10);
    unit.setProperty(tr("REQUEST"), PROTECTION_EXTERNAL_SET_TYPE);

    m_modbusDevice->request(unit);
}
//---------------------------------------------------
void ConfiguratorWindow::protectionExternalSetWrite()
{
    QVector<quint16> data;

    union
    {
        quint16 b[2];
        float   v;
    } value;

    for(quint8 i = 0, j = 0; i < m_protectionExternal_cell.count(); i++)
    {
        value.v = m_protectionExternal_cell.at(j++)->text().toFloat();

        data.append(value.b[1]);
        data.append(value.b[0]);
    }

    CDataUnitType unit(ui->sboxSlaveID->value(), CDataUnitType::WriteMultipleRegisters,
                       PROTECTION_EXTERNAL_SET_ADDRESS, data);
    unit.setProperty(tr("REQUEST"), PROTECTION_EXTERNAL_SET_TYPE);

    m_modbusDevice->request(unit);
}
//-----------------------------------------------------
void ConfiguratorWindow::protectionTemperatureSetRead()
{
    CDataUnitType unit(ui->sboxSlaveID->value(), CDataUnitType::ReadHoldingRegisters,
                       PROTECTION_TEMP_SET_ADDRESS, QVector<quint16>() << 12);
    unit.setProperty(tr("REQUEST"), PROTECTION_TEMP_SET_TYPE);

    m_modbusDevice->request(unit);
}
//------------------------------------------------------
void ConfiguratorWindow::protectionTemperatureSetWrite()
{
    QVector<quint16> data;

    union
    {
        quint16 b[2];
        float   v;
    } value;

    for(quint8 i = 0, j = 0; i < m_protectionTemperature_cell.count(); i++)
    {
        value.v = m_protectionTemperature_cell.at(j++)->text().toFloat();

        data.append(value.b[1]);
        data.append(value.b[0]);
    }

    CDataUnitType unit(ui->sboxSlaveID->value(), CDataUnitType::WriteMultipleRegisters,
                       PROTECTION_TEMP_SET_ADDRESS, data);
    unit.setProperty(tr("REQUEST"), PROTECTION_TEMP_SET_TYPE);

    m_modbusDevice->request(unit);
}
//-----------------------------------------------
void ConfiguratorWindow::protectionLevelSetRead()
{
    CDataUnitType unit(ui->sboxSlaveID->value(), CDataUnitType::ReadHoldingRegisters,
                       PROTECTION_LEVEL_SET_ADDRESS, QVector<quint16>() << 4);
    unit.setProperty(tr("REQUEST"), PROTECTION_LEVEL_SET_TYPE);

    m_modbusDevice->request(unit);
}
//------------------------------------------------
void ConfiguratorWindow::protectionLevelSetWrite()
{
    QVector<quint16> data;

    union
    {
        quint16 b[2];
        float   v;
    } value;

    for(quint8 i = 0, j = 0; i < m_protectionLevel_cell.count(); i++)
    {
        value.v = m_protectionLevel_cell.at(j++)->text().toFloat();

        data.append(value.b[1]);
        data.append(value.b[0]);
    }

    CDataUnitType unit(ui->sboxSlaveID->value(), CDataUnitType::WriteMultipleRegisters,
                       PROTECTION_LEVEL_SET_ADDRESS, data);
    unit.setProperty(tr("REQUEST"), PROTECTION_LEVEL_SET_TYPE);

    m_modbusDevice->request(unit);
}
//---------------------------------------------
void ConfiguratorWindow::protectionBruSetRead()
{
    CDataUnitType unit(ui->sboxSlaveID->value(), CDataUnitType::ReadHoldingRegisters,
                       PROTECTION_BRU_SET_ADDRESS, QVector<quint16>() << 8);
    unit.setProperty(tr("REQUEST"), PROTECTION_BRU_SET_TYPE);

    m_modbusDevice->request(unit);
}
//----------------------------------------------
void ConfiguratorWindow::protectionBruSetWrite()
{
    QVector<quint16> data;

    union
    {
        quint16 b[2];
        float   v;
    } value;

    for(quint8 i = 0, j = 0; i < m_protectionBru_cell.count(); i++)
    {
        value.v = m_protectionBru_cell.at(j++)->text().toFloat();

        data.append(value.b[1]);
        data.append(value.b[0]);
    }

    CDataUnitType unit(ui->sboxSlaveID->value(), CDataUnitType::WriteMultipleRegisters,
                       PROTECTION_BRU_SET_ADDRESS, data);
    unit.setProperty(tr("REQUEST"), PROTECTION_BRU_SET_TYPE);

    m_modbusDevice->request(unit);
}
//------------------------------------------------
void ConfiguratorWindow::protectionVacuumSetRead()
{
    CDataUnitType unit(ui->sboxSlaveID->value(), CDataUnitType::ReadHoldingRegisters,
                       PROTECTION_VACUUM_SET_ADDRESS, QVector<quint16>() << 6);
    unit.setProperty(tr("REQUEST"), PROTECTION_VACUUM_SET_TYPE);

    m_modbusDevice->request(unit);
}
//-------------------------------------------------
void ConfiguratorWindow::protectionVacuumSetWrite()
{
    QVector<quint16> data;

    union
    {
        quint16 b[2];
        float   v;
    } value;

    for(quint8 i = 0, j = 0; i < m_protectionVacuum_cell.count(); i++)
    {
        value.v = m_protectionVacuum_cell.at(j++)->text().toFloat();

        data.append(value.b[1]);
        data.append(value.b[0]);
    }

    CDataUnitType unit(ui->sboxSlaveID->value(), CDataUnitType::WriteMultipleRegisters,
                       PROTECTION_VACUUM_SET_ADDRESS, data);
    unit.setProperty(tr("REQUEST"), PROTECTION_VACUUM_SET_TYPE);

    m_modbusDevice->request(unit);
}
//------------------------------------------
void ConfiguratorWindow::automationSetRead()
{
    CDataUnitType unit(ui->sboxSlaveID->value(), CDataUnitType::ReadHoldingRegisters,
                       AUTOMATION_SET_ADDRESS, QVector<quint16>() << 12);
    unit.setProperty(tr("REQUEST"), AUTOMATION_SET_TYPE);

    m_modbusDevice->request(unit);
}
//-------------------------------------------
void ConfiguratorWindow::automationSetWrite()
{
    QVector<quint16> data;

    union
    {
        quint16 b[2];
        float   v;
    } value;

    for(quint8 i = 0, j = 0; i < m_automation_cell.count(); i++)
    {
        value.v = m_automation_cell.at(j++)->text().toFloat();

        data.append(value.b[1]);
        data.append(value.b[0]);
    }

    CDataUnitType unit(ui->sboxSlaveID->value(), CDataUnitType::WriteMultipleRegisters,
                       AUTOMATION_SET_ADDRESS, data);
    unit.setProperty(tr("REQUEST"), AUTOMATION_SET_TYPE);

    m_modbusDevice->request(unit);
}
//--------------------------------------------
void ConfiguratorWindow::switchDeviceSetRead()
{
    CDataUnitType unit(ui->sboxSlaveID->value(), CDataUnitType::ReadHoldingRegisters,
                       SWITCH_DEV_SET_ADDRESS, QVector<quint16>() << 36);
    unit.setProperty(tr("REQUEST"), SWITCH_DEV_SET_TYPE);

    m_modbusDevice->request(unit);
}
//---------------------------------------------
void ConfiguratorWindow::switchDeviceSetWrite()
{
    QVector<quint16> data;

    union
    {
        quint16 b[2];
        float   v;
    } value;

    for(quint8 i = 0, j = 0; i < m_switch_device_cell.count(); i++)
    {
        value.v = m_switch_device_cell.at(j++)->text().toFloat();

        data.append(value.b[1]);
        data.append(value.b[0]);
    }

    CDataUnitType unit(ui->sboxSlaveID->value(), CDataUnitType::WriteMultipleRegisters,
                       SWITCH_DEV_SET_ADDRESS, data);
    unit.setProperty(tr("REQUEST"), SWITCH_DEV_SET_TYPE);

    m_modbusDevice->request(unit);
}
//--------------------------------------------------------
void ConfiguratorWindow::responseRead(CDataUnitType& unit)
{
    if(unit.is_empty())
        return;
    
    qDebug() << "Получен ответ: " << unit.valueCount();
    emit m_modbusDevice->infoLog(tr("Получен ответ: ") + QString::number(unit.valueCount()) + tr(" байт \n"));
    
    switch((RequestType)unit.property("REQUEST").toInt())
    {
        case PROTECTION_MTZ_TYPE: // чтение состояний токовых защит
            displayProtectionMTZStateValues(unit.values());
        break;

        case PROTECTION_MTZ_SET_TYPE: // чтение состояний токовых защит
            displayProtectionMTZSetValues(unit.values());
        break;

        case PROTECTION_EARTHY_SET_TYPE: // чтение состояний земляных защит
            displayProtectionEarthySetValues(unit.values());
        break;

        case PROTECTION_POWER_SET_TYPE: // чтение состояний защит по напряжению
            displayProtectionPowerSetValues(unit.values());
        break;

        case PROTECTION_MOTOR_SET_TYPE: // чтение состояний защит двигателей
            displayProtectionMotorSetValues(unit.values());
        break;

        case PROTECTION_FREQUENCY_SET_TYPE: // чтение состояний частотных защит
            displayProtectionFrequencySetValues(unit.values());
        break;

        case PROTECTION_EXTERNAL_SET_TYPE: // чтение состояний внешних защит
            displayProtectionExternalSetValues(unit.values());
        break;

        case PROTECTION_TEMP_SET_TYPE: // чтение состояний температурных защит
            displayProtectionTemperatureSetValues(unit.values());
        break;

        case PROTECTION_LEVEL_SET_TYPE: // чтение состояний уровневых защит
            displayProtectionLevelSetValues(unit.values());
        break;

        case PROTECTION_BRU_SET_TYPE: // чтение состояний защит БРУ
            displayProtectionBruSetValues(unit.values());
        break;

        case PROTECTION_VACUUM_SET_TYPE: // чтение состояний вакуумных защит
            displayProtectionVacuumSetValues(unit.values());
        break;

        case AUTOMATION_SET_TYPE: // чтение состояний автоматики
            displayAutomationValues(unit.values());
        break;

        case SWITCH_DEV_SET_TYPE: // чтение состояний коммутационных аппаратов
            displaySwitchDeviceValues(unit.values());
        break;
        
        case IN_ANALOG_TYPE: // чтение калибровок
            displayInAnalogValues(unit.values());
        break;
        
        case CALCULATE_TYPE: // чтение расчетных величин
            displayCalculateValues(unit.values());
        break;
    }
}
//-----------------------------
void ConfiguratorWindow::show()
{
    QMainWindow::show();

    setWindowState(Qt::WindowFullScreen);
    setWindowState(Qt::WindowMaximized);
    
    m_terminal->hide();
    ui->stwgtMain->setCurrentIndex(0);
}
//--------------------------------------------------------------------
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
    quint8 count = m_protect_mtz_group->buttons().count();

    if(id >= 0 && id < count)
    {
        for(quint8 i = 0; i < count; i++)
        {
           QPushButton* btn = qobject_cast<QPushButton*>(m_protect_mtz_group->button(i));

           if(i == id)
           {
               btn->setStyleSheet(tr("QPushButton { background: green; color: yellow }"));
           }
           else
               btn->setStyleSheet(tr("QPushButton { background: none }"));
        }

        ui->stwgtProtectionPropertiesMTZ->setCurrentIndex(id);
    }
}
//------------------------------------------------------
void ConfiguratorWindow::protectEarthlyChangedID(int id)
{
    quint8 count = m_protect_earthly_group->buttons().count();

    if(id >= 0 && id < count)
    {
        for(quint8 i = 0; i < count; i++)
        {
           QPushButton* btn = qobject_cast<QPushButton*>(m_protect_earthly_group->button(i));

           if(i == id)
           {
               btn->setStyleSheet(tr("QPushButton { background: green; color: yellow }"));
           }
           else
               btn->setStyleSheet(tr("QPushButton { background: none }"));
        }

        ui->stwgtProtectionPropertiesEarthly->setCurrentIndex(id);
    }
}
//----------------------------------------------------
void ConfiguratorWindow::protectPowerChangedID(int id)
{
    quint8 count = m_protect_power_group->buttons().count();

    if(id >= 0 && id < count)
    {
        for(quint8 i = 0; i < count; i++)
        {
           QPushButton* btn = qobject_cast<QPushButton*>(m_protect_power_group->button(i));

           if(i == id)
           {
               btn->setStyleSheet(tr("QPushButton { background: green; color: yellow }"));
           }
           else
               btn->setStyleSheet(tr("QPushButton { background: none }"));
        }

        ui->stwgtProtectionPropertiesPower->setCurrentIndex(id);
    }
}
//----------------------------------------------------
void ConfiguratorWindow::protectMotorChangedID(int id)
{
    quint8 count = m_protect_motor_group->buttons().count();

    if(id >= 0 && id < count)
    {
        for(quint8 i = 0; i < count; i++)
        {
           QPushButton* btn = qobject_cast<QPushButton*>(m_protect_motor_group->button(i));

           if(i == id)
           {
               btn->setStyleSheet(tr("QPushButton { background: green; color: yellow }"));
           }
           else
               btn->setStyleSheet(tr("QPushButton { background: none }"));
        }

        ui->stwgtProtectionPropertiesMotor->setCurrentIndex(id);
    }
}
//--------------------------------------------------------
void ConfiguratorWindow::protectFrequencyChangedID(int id)
{
    quint8 count = m_protect_frequency_group->buttons().count();

    if(id >= 0 && id < count)
    {
        for(quint8 i = 0; i < count; i++)
        {
           QPushButton* btn = qobject_cast<QPushButton*>(m_protect_frequency_group->button(i));

           if(i == id)
           {
               btn->setStyleSheet(tr("QPushButton { background: green; color: yellow }"));
           }
           else
               btn->setStyleSheet(tr("QPushButton { background: none }"));
        }

        ui->stwgtProtectionPropertiesFrequency->setCurrentIndex(id);
    }
}
//-------------------------------------------------------
void ConfiguratorWindow::protectExternalChangedID(int id)
{
    quint8 count = m_protect_external_group->buttons().count();

    if(id >= 0 && id < count)
    {
        for(quint8 i = 0; i < count; i++)
        {
           QPushButton* btn = qobject_cast<QPushButton*>(m_protect_external_group->button(i));

           if(i == id)
           {
               btn->setStyleSheet(tr("QPushButton { background: green; color: yellow }"));
           }
           else
               btn->setStyleSheet(tr("QPushButton { background: none }"));
        }

        ui->stwgtProtectionPropertiesExternal->setCurrentIndex(id);
    }
}
//----------------------------------------------------------
void ConfiguratorWindow::protectTemperatureChangedID(int id)
{
    quint8 count = m_protect_temperature_group->buttons().count();

    if(id >= 0 && id < count)
    {
        for(quint8 i = 0; i < count; i++)
        {
           QPushButton* btn = qobject_cast<QPushButton*>(m_protect_temperature_group->button(i));

           if(i == id)
           {
               btn->setStyleSheet(tr("QPushButton { background: green; color: yellow }"));
           }
           else
               btn->setStyleSheet(tr("QPushButton { background: none }"));
        }

        ui->stwgtProtectionPropertiesTemperature->setCurrentIndex(id);
    }
}
//----------------------------------------------------
void ConfiguratorWindow::protectLevelChangedID(int id)
{
    quint8 count = m_protect_level_group->buttons().count();

    if(id >= 0 && id < count)
    {
        for(quint8 i = 0; i < count; i++)
        {
           QPushButton* btn = qobject_cast<QPushButton*>(m_protect_level_group->button(i));

           if(i == id)
           {
               btn->setStyleSheet(tr("QPushButton { background: green; color: yellow }"));
           }
           else
               btn->setStyleSheet(tr("QPushButton { background: none }"));
        }

        ui->stwgtProtectionPropertiesLevel->setCurrentIndex(id);
    }
}
//----------------------------------------------------
void ConfiguratorWindow::switchDeviceChangedID(int id)
{
    quint8 count = m_switch_device_group->buttons().count();

    if(id >= 0 && id < count)
    {
        for(quint8 i = 0; i < count; i++)
        {
           QPushButton* btn = qobject_cast<QPushButton*>(m_switch_device_group->button(i));

           if(i == id)
           {
               btn->setStyleSheet(tr("QPushButton { background: green; color: yellow }"));
           }
           else
               btn->setStyleSheet(tr("QPushButton { background: none }"));
        }

        ui->stwgtSwitchDevice->setCurrentIndex(id);
    }
}
//--------------------------------------------------
void ConfiguratorWindow::additionalChangedID(int id)
{
    quint8 count = m_additional_group->buttons().count();

    if(id >= 0 && id < count)
    {
        for(quint8 i = 0; i < count; i++)
        {
           QPushButton* btn = qobject_cast<QPushButton*>(m_additional_group->button(i));

           if(i == id)
           {
               btn->setStyleSheet(tr("QPushButton { background: green; color: yellow }"));
           }
           else
               btn->setStyleSheet(tr("QPushButton { background: none }"));
        }

        ui->stwgtAdditional->setCurrentIndex(id);
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
    if(m_logFile->isOpen())
    {
        m_logFile->write(info.toStdString().c_str());
    }
}
//------------------------------------------------------------------
void ConfiguratorWindow::itemClicked(QTreeWidgetItem* item, int col)
{
    if(!item)
        return;

    QString itemName = item->text(col).toUpper();

    if(itemName == tr("ОСНОВНЫЕ"))
    {
        ui->stwgtMain->setCurrentIndex(0);
    }
    else if(itemName == tr("КАЛИБРОВКИ"))
    {
        ui->stwgtMain->setCurrentIndex(1);
    }
    else if(itemName == tr("МАКСИМАЛЬНЫЕ ТОКОВЫЕ"))
    {
        ui->stwgtMain->setCurrentIndex(2);
    }
    else if(itemName == tr("ЗЕМЛЯНЫЕ"))
    {
        ui->stwgtMain->setCurrentIndex(3);
    }
    else if(itemName == tr("ЗАЩИТЫ ПО НАПРЯЖЕНИЮ"))
    {
        ui->stwgtMain->setCurrentIndex(4);
    }
    else if(itemName == tr("ЗАЩИТЫ ДВИГАТЕЛЯ"))
    {
        ui->stwgtMain->setCurrentIndex(5);
    }
    else if(itemName == tr("ЗАЩИТЫ ПО ЧАСТОТЕ"))
    {
        ui->stwgtMain->setCurrentIndex(6);
    }
    else if(itemName == tr("ВНЕШНИЕ ЗАЩИТЫ"))
    {
        ui->stwgtMain->setCurrentIndex(7);
    }
    else if(itemName == tr("ТЕМПЕРАТУРНЫЕ ЗАЩИТЫ"))
    {
        ui->stwgtMain->setCurrentIndex(8);
    }
    else if(itemName == tr("УРОВНЕВЫЕ ЗАЩИТЫ"))
    {
        ui->stwgtMain->setCurrentIndex(9);
    }
    else if(itemName == tr("ЗАЩИТЫ БРУ"))
    {
        ui->stwgtMain->setCurrentIndex(10);
    }
    else if(itemName == tr("ВАКУУМНЫЕ ЗАЩИТЫ"))
    {
        ui->stwgtMain->setCurrentIndex(11);
    }
    else if(itemName == tr("КОММУТАЦИОННЫЕ АППАРАТЫ"))
    {
        ui->stwgtMain->setCurrentIndex(12);
    }
    else if(itemName == tr("АВТОМАТИКА"))
    {
        ui->stwgtMain->setCurrentIndex(13);
    }
    else if(itemName == tr("АВАРИЙ"))
    {
        ui->stwgtMain->setCurrentIndex(14);
    }
    else if(itemName == tr("СОБЫТИЙ"))
    {
        ui->stwgtMain->setCurrentIndex(15);
    }
    else if(itemName == tr("ОСЦИЛЛОГРАФ"))
    {
        ui->stwgtMain->setCurrentIndex(16);
    }
    else if(itemName == tr("ПЕРВИЧНЫЕ ВЕЛИЧИНЫ"))
    {
        ui->stwgtMain->setCurrentIndex(17);
    }
    else if(itemName == tr("ВТОРИЧНЫЕ ВЕЛИЧИНЫ"))
    {
        ui->stwgtMain->setCurrentIndex(18);
    }
    else if(itemName == tr("ЭЛЕКТРОЭНЕРГИЯ"))
    {
        ui->stwgtMain->setCurrentIndex(19);
    }
    else if(itemName == tr("ДИСКРЕТНЫЕ ВХОДЫ"))
    {
        ui->stwgtMain->setCurrentIndex(20);
    }
    else if(itemName == tr("ДИСКРЕТНЫЕ ВЫХОДЫ"))
    {
        ui->stwgtMain->setCurrentIndex(21);
    }
}
//-------------------------------------
void ConfiguratorWindow::readSettings()
{
    qint32 index = ui->stwgtMain->currentIndex();

    if(index >= 0 && index < 22) // выбрана группа "Настройки"
    {
        inAnalogRead(); // чтение настроек "Основные" и "Калибровки"
        protectionMTZStateRead(); // чтение настроек состояния токовых защит
        protectionMTZSetRead(); // чтение настроек токовых защит
        protectionEarthySetRead(); // чтение настроек земляных защит
        protectionPowerSetRead(); // чтение настроек защит по напряжению
        protectionMotorSetRead(); // чтение настроек защит двигателей
        protectionFrequencySetRead(); // чтение настроек частотных защит
        protectionExternalSetRead(); // чтение настроек внешних защит
        protectionTemperatureSetRead(); // чтение настроек температурных защит
        protectionLevelSetRead(); // чтение настроек уровневых защит
        protectionVacuumSetRead(); // чтение настроек вакуумных защит
        protectionBruSetRead(); // чтение настроек защит БРУ
        automationSetRead(); // чтение настроек автоматики
        switchDeviceSetRead(); // чтение настроек коммутационных аппаратов
    }
}
//---------------------------------------
void ConfiguratorWindow::readSetCurrent()
{
    qint32 index = ui->stwgtMain->currentIndex();

    switch(index)
    {
        case 0:
        case 1:
            inAnalogRead(); // чтение настроек "Основные" и "Калибровки"
        break;

        case 2:
            protectionMTZStateRead(); // чтение настроек состояния токовых защит
            protectionMTZSetRead(); // чтение настроек токовых защит
        break;

        case 3:
            protectionEarthySetRead(); // чтение настроек земляных защит
        break;

        case 4:
            protectionPowerSetRead(); // чтение настроек защит по напряжению
        break;

        case 5:
            protectionMotorSetRead(); // чтение настроек защит двигателей
        break;

        case 6:
            protectionFrequencySetRead(); // чтение настроек частотных защит
        break;

        case 7:
            protectionExternalSetRead(); // чтение настроек внешних защит
        break;

        case 8:
            protectionTemperatureSetRead(); // чтение настроек температурных защит
        break;

        case 9:
            protectionLevelSetRead(); // чтение настроек уровневых защит
        break;

        case 10:
            protectionVacuumSetRead(); // чтение настроек вакуумных защит
        break;

        case 11:
            protectionBruSetRead(); // чтение настроек защит БРУ
        break;

        case 12:
            automationSetRead(); // чтение настроек автоматики
        break;

        case 13:
            switchDeviceSetRead(); // чтение настроек коммутационных аппаратов
        break;

        case 14:
        break;

        case 15:
        break;

        case 16:
        break;

        case 17:
        break;

        case 18:
        break;

        case 19:
        break;

        case 20:
        break;

        case 21:
        break;
    }
}
//--------------------------------------
void ConfiguratorWindow::writeSettings()
{
    qint32 index = ui->stwgtMain->currentIndex();

    if(index >= 0 && index < 22) // выбрана группа "Настройки"
    {
        inAnalogWrite(); // запись настроек "Основные" и "Калибровки"
        protectionMTZStateWrite(); // запись настроек состояния токовых защит
        protectionMTZSetWrite(); // запись настроек токовых защит
        protectionEarthySetWrite(); // запись настроек земляных защит
        protectionPowerSetWrite(); // запись настроек защит по напряжению
        protectionMotorSetWrite(); // запись настроек защит двигателей
        protectionFrequencySetWrite(); // запись настроек частотных защит
        protectionExternalSetWrite(); // запись настроек внешних защит
        protectionTemperatureSetWrite(); // запись настроек температурных защит
        protectionLevelSetWrite(); // запись настроек уровневых защит
        protectionVacuumSetWrite(); // запись настроек вакуумных защит
        protectionBruSetWrite(); // запись настроек защит БРУ
        automationSetWrite(); // запись настроек автоматики
        switchDeviceSetWrite(); // запись настроек коммутационных аппаратов
    }
}
//----------------------------------------
void ConfiguratorWindow::writeSetCurrent()
{
    qint32 index = ui->stwgtMain->currentIndex();

    switch(index)
    {
        case 0:
        case 1:
            inAnalogWrite(); // запись настроек "Основные" и "Калибровки"
        break;

        case 2:
            protectionMTZStateWrite(); // запись настроек состояния токовых защит
            protectionMTZSetWrite(); // запись настроек токовых защит
        break;

        case 3:
            protectionEarthySetWrite(); // запись настроек земляных защит
        break;

        case 4:
            protectionPowerSetWrite(); // запись настроек защит по напряжению
        break;

        case 5:
            protectionMotorSetWrite(); // запись настроек защит двигателей
        break;

        case 6:
            protectionFrequencySetWrite(); // запись настроек частотных защит
        break;

        case 7:
            protectionExternalSetWrite(); // запись настроек внешних защит
        break;

        case 8:
            protectionTemperatureSetWrite(); // запись настроек температурных защит
        break;

        case 9:
            protectionLevelSetWrite(); // запись настроек уровневых защит
        break;

        case 10:
            protectionVacuumSetWrite(); // запись настроек вакуумных защит
        break;

        case 11:
            protectionBruSetWrite(); // запись настроек защит БРУ
        break;

        case 12:
            automationSetWrite(); // запись настроек автоматики
        break;

        case 13:
            switchDeviceSetWrite(); // запись настроек коммутационных аппаратов
        break;

        case 14:
        break;

        case 15:
        break;

        case 16:
        break;

        case 17:
        break;

        case 18:
        break;

        case 19:
        break;

        case 20:
        break;

        case 21:
        break;
    }
}
//-------------------------------------------------
void ConfiguratorWindow::expandItemTree(bool state)
{
    if(state)
    {
        ui->treewgtDeviceMenu->expandAll();
        ui->tbntExpandItems->setIcon(QIcon(tr(":/images/resource/images/branch_open.png")));
    }
    else
    {
        ui->treewgtDeviceMenu->collapseAll();
        ui->tbntExpandItems->setIcon(QIcon(tr(":/images/resource/images/branch_close.png")));
    }
}
//--------------------------------------
void ConfiguratorWindow::initMenuPanel()
{
    itemSettings   = new QTreeWidgetItem(ui->treewgtDeviceMenu);
    itemJournals   = new QTreeWidgetItem(ui->treewgtDeviceMenu);
    itemMeasures   = new QTreeWidgetItem(ui->treewgtDeviceMenu);
    itemMonitoring = new QTreeWidgetItem(ui->treewgtDeviceMenu);

    itemSetInputAnalogs   = new QTreeWidgetItem(itemSettings);
    itemSetProtections    = new QTreeWidgetItem(itemSettings);
    itemSetDevConnections = new QTreeWidgetItem(itemSettings);
    itemSetAutomation     = new QTreeWidgetItem(itemSettings);

    itemJournalCrashs = new QTreeWidgetItem(itemJournals);
    itemJournalEvents = new QTreeWidgetItem(itemJournals);
    itemJournalOscill = new QTreeWidgetItem(itemJournals);

    itemMeasPrimaryValues   = new QTreeWidgetItem(itemMeasures);
    itemMeasSecondaryValues = new QTreeWidgetItem(itemMeasures);
    itemMeasPowerElectric   = new QTreeWidgetItem(itemMeasures);

    itemMonitorInputDiscrets  = new QTreeWidgetItem(itemMonitoring);
    itemMonitorOutputDiscrets = new QTreeWidgetItem(itemMonitoring);

    itemInAnalogMain        = new QTreeWidgetItem(itemSetInputAnalogs);
    itemInAnalogCalibration = new QTreeWidgetItem(itemSetInputAnalogs);

    itemProtectCurrentMax  = new QTreeWidgetItem(itemSetProtections);
    itemProtectEarthy      = new QTreeWidgetItem(itemSetProtections);
    itemProtectPower       = new QTreeWidgetItem(itemSetProtections);
    itemProtectMotor       = new QTreeWidgetItem(itemSetProtections);
    itemProtectFrequency   = new QTreeWidgetItem(itemSetProtections);
    itemProtectExternal    = new QTreeWidgetItem(itemSetProtections);
    itemProtectTemperature = new QTreeWidgetItem(itemSetProtections);
    itemProtectLevel       = new QTreeWidgetItem(itemSetProtections);
    itemProtectBRU         = new QTreeWidgetItem(itemSetProtections);
    itemProtectVacuum      = new QTreeWidgetItem(itemSetProtections);

    itemSettings->setText(0, tr("Настройки"));
    itemJournals->setText(0, tr("Журналы"));
    itemMeasures->setText(0, tr("Измерения"));
    itemMonitoring->setText(0, tr("Мониторинг"));

    itemSetInputAnalogs->setText(0, tr("Аналоговые входы"));
    itemSetProtections->setText(0, tr("Защита"));
    itemSetDevConnections->setText(0, tr("Коммутационные аппараты"));
    itemSetAutomation->setText(0, tr("Автоматика"));

    itemJournalCrashs->setText(0, tr("Аварий"));
    itemJournalEvents->setText(0, tr("Событий"));
    itemJournalOscill->setText(0, tr("Осциллограф"));

    itemMeasPrimaryValues->setText(0, tr("Первичные величины"));
    itemMeasSecondaryValues->setText(0, tr("Вторичные величины"));
    itemMeasPowerElectric->setText(0, tr("Электроэнергия"));

    itemMonitorInputDiscrets->setText(0, tr("Дискретные входы"));
    itemMonitorOutputDiscrets->setText(0, tr("Дискретные выходы"));

    itemInAnalogMain->setText(0, tr("Основные"));
    itemInAnalogCalibration->setText(0, tr("Калибровки"));

    itemProtectCurrentMax->setText(0, tr("Максимальные токовые"));
    itemProtectEarthy->setText(0, tr("Земляные"));
    itemProtectPower->setText(0, tr("Защиты по напряжению"));
    itemProtectMotor->setText(0, tr("Защиты двигателя"));
    itemProtectFrequency->setText(0, tr("Защиты по частоте"));
    itemProtectExternal->setText(0, tr("Внешние защиты"));
    itemProtectTemperature->setText(0, tr("Температурные защиты"));
    itemProtectLevel->setText(0, tr("Уровневые защиты"));
    itemProtectBRU->setText(0, tr("Защиты БРУ"));
    itemProtectVacuum->setText(0, tr("Вакуумные защиты"));

    ui->treewgtDeviceMenu->addTopLevelItem(itemSettings);
    ui->treewgtDeviceMenu->addTopLevelItem(itemJournals);
    ui->treewgtDeviceMenu->addTopLevelItem(itemMeasures);
    ui->treewgtDeviceMenu->addTopLevelItem(itemMonitoring);

    itemSettings->addChild(itemSetInputAnalogs);
    itemSettings->addChild(itemSetProtections);
    itemSettings->addChild(itemSetDevConnections);
    itemSettings->addChild(itemSetAutomation);

    itemJournals->addChild(itemJournalCrashs);
    itemJournals->addChild(itemJournalEvents);
    itemJournals->addChild(itemJournalOscill);

    itemMeasures->addChild(itemMeasPrimaryValues);
    itemMeasures->addChild(itemMeasSecondaryValues);
    itemMeasures->addChild(itemMeasPowerElectric);

    itemMonitoring->addChild(itemMonitorInputDiscrets);
    itemMonitoring->addChild(itemMonitorOutputDiscrets);

    itemSetInputAnalogs->addChild(itemInAnalogMain);
    itemSetInputAnalogs->addChild(itemInAnalogCalibration);

    itemSetProtections->addChild(itemProtectCurrentMax);
    itemSetProtections->addChild(itemProtectEarthy);
    itemSetProtections->addChild(itemProtectPower);
    itemSetProtections->addChild(itemProtectMotor);
    itemSetProtections->addChild(itemProtectFrequency);
    itemSetProtections->addChild(itemProtectExternal);
    itemSetProtections->addChild(itemProtectTemperature);
    itemSetProtections->addChild(itemProtectLevel);
    itemSetProtections->addChild(itemProtectBRU);
    itemSetProtections->addChild(itemProtectVacuum);
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
    m_protect_level_group->addButton(ui->pbtnProtectionLevelSignStart);
    m_protect_level_group->setId(ui->pbtnProtectionLevel1, 0);
    m_protect_level_group->setId(ui->pbtnProtectionLevel2, 1);
    m_protect_level_group->setId(ui->pbtnProtectionLevelSignStart, 2);

    // группа коммутационных устройств
    m_switch_device_group->addButton(ui->pbtnSwDevBreaker);
    m_switch_device_group->addButton(ui->pbtnSwDevBlock);
    m_switch_device_group->addButton(ui->pbtnSwDevSCHR);
    m_switch_device_group->addButton(ui->pbtnSwDevLR);
    m_switch_device_group->addButton(ui->pbtnSwDevZR);
    m_switch_device_group->addButton(ui->pbtnSwDevTruck);
    m_switch_device_group->addButton(ui->pbtnSwDevCtrl);
    m_switch_device_group->setId(ui->pbtnSwDevBreaker, 0);
    m_switch_device_group->setId(ui->pbtnSwDevBlock, 1);
    m_switch_device_group->setId(ui->pbtnSwDevSCHR, 2);
    m_switch_device_group->setId(ui->pbtnSwDevLR, 3);
    m_switch_device_group->setId(ui->pbtnSwDevZR, 4);
    m_switch_device_group->setId(ui->pbtnSwDevTruck, 5);
    m_switch_device_group->setId(ui->pbtnSwDevCtrl, 6);

    // группа дополнительных настроек
    m_additional_group->addButton(ui->pbtnAddAVR);
    m_additional_group->addButton(ui->pbtnAddAPV);
    m_additional_group->addButton(ui->pbtnAddAPV_Start);
    m_additional_group->setId(ui->pbtnAddAVR, 0);
    m_additional_group->setId(ui->pbtnAddAPV, 1);
    m_additional_group->setId(ui->pbtnAddAPV_Start, 2);
    
    protectMTZChangedID(0);
    protectEarthlyChangedID(0);
    protectPowerChangedID(0);
    protectMotorChangedID(0);
    protectFrequencyChangedID(0);
    protectExternalChangedID(0);
    protectTemperatureChangedID(0);
    protectLevelChangedID(0);
    switchDeviceChangedID(0);
    additionalChangedID(0);
    
    m_protect_mtz_group->setExclusive(true);
    m_protect_earthly_group->setExclusive(true);
    m_protect_power_group->setExclusive(true);
    m_protect_motor_group->setExclusive(true);
    m_protect_frequency_group->setExclusive(true);
    m_protect_external_group->setExclusive(true);
    m_protect_temperature_group->setExclusive(true);
    m_protect_level_group->setExclusive(true);
    m_switch_device_group->setExclusive(true);
    m_additional_group->setExclusive(true);
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
    m_calculateWidget->setData(values);
}
//---------------------------------------------------------------------
void ConfiguratorWindow::displayInAnalogValues(QVector<quint16> values)
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
        
        m_in_analog_cell.at(j)->setText(QString::number(cell_value.value, 'f', 6));
    }
}
//-----------------------------------------------------------------------
void ConfiguratorWindow::displayProtectionMTZStateValues(QVector<quint16> values)
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
//-----------------------------------------------------------------------------
void ConfiguratorWindow::displayProtectionMTZSetValues(QVector<quint16> values)
{
    union
    {
        quint16 word[2];
        float   value;
    } cell_value;

    if(values.count() == 38)
    {
        for(quint8 i = 0, j = 0; i < values.count() - 1; i += 2, j++)
        {
            if(i == 6)
            {
                j--;
                continue;
            }

            quint16 value1 = values.at(i + 1);
            quint16 value2 = values.at(i);

            cell_value.word[0] = value1;
            cell_value.word[1] = value2;

            QLineEdit* item = m_protectionMTZ_cell.at(j);

            if(item != ui->leProtectionMTZ4_Angle)
                item->setText(QString::number(cell_value.value, 'f', 6));
            else
                item->setText(QString::number((int)cell_value.value));
        }
    }
}
//--------------------------------------------------------------------------------
void ConfiguratorWindow::displayProtectionEarthySetValues(QVector<quint16> values)
{
    union
    {
        quint16 word[2];
        float   value;
    } cell_value;

    if(values.count() == 36)
    {
        for(quint8 i = 0, j = 0; i < values.count() - 1; i += 2, j++)
        {
            quint16 value1 = values.at(i + 1);
            quint16 value2 = values.at(i);

            cell_value.word[0] = value1;
            cell_value.word[1] = value2;

            QLineEdit* item = m_protectionEarthy_cell.at(j);

            if(item != ui->leProtectionEarthly_NZZ1_Angle && item != ui->leProtectionEarthly_NZZ2_Angle)
                item->setText(QString::number(cell_value.value, 'f', 6));
            else
                item->setText(QString::number((int)cell_value.value));
        }
    }
}
//-------------------------------------------------------------------------------
void ConfiguratorWindow::displayProtectionPowerSetValues(QVector<quint16> values)
{
    union
    {
        quint16 word[2];
        float   value;
    } cell_value;

    if(values.count() == 30)
    {
        for(quint8 i = 0, j = 0; i < values.count() - 1; i += 2, j++)
        {
            quint16 value1 = values.at(i + 1);
            quint16 value2 = values.at(i);

            cell_value.word[0] = value1;
            cell_value.word[1] = value2;

            QLineEdit* item = m_protectionPower_cell.at(j);

            item->setText(QString::number(cell_value.value, 'f', 6));
        }
    }
}
//-------------------------------------------------------------------------------
void ConfiguratorWindow::displayProtectionMotorSetValues(QVector<quint16> values)
{
    union
    {
        quint16 word[2];
        float   value;
    } cell_value;

    if(values.count() == 12)
    {
        for(quint8 i = 0, j = 0; i < values.count() - 1; i += 2, j++)
        {
            quint16 value1 = values.at(i + 1);
            quint16 value2 = values.at(i);

            cell_value.word[0] = value1;
            cell_value.word[1] = value2;

            QLineEdit* item = m_protectionMotor_cell.at(j);

            item->setText(QString::number(cell_value.value, 'f', 6));
        }
    }
}
//-----------------------------------------------------------------------------------
void ConfiguratorWindow::displayProtectionFrequencySetValues(QVector<quint16> values)
{
    union
    {
        quint16 word[2];
        float   value;
    } cell_value;

    if(values.count() == 24)
    {
        for(quint8 i = 0, j = 0; i < values.count() - 1; i += 2, j++)
        {
            quint16 value1 = values.at(i + 1);
            quint16 value2 = values.at(i);

            cell_value.word[0] = value1;
            cell_value.word[1] = value2;

            QLineEdit* item = m_protectionFrequency_cell.at(j);

            item->setText(QString::number(cell_value.value, 'f', 6));
        }
    }
}
//----------------------------------------------------------------------------------
void ConfiguratorWindow::displayProtectionExternalSetValues(QVector<quint16> values)
{
    union
    {
        quint16 word[2];
        float   value;
    } cell_value;

    if(values.count() == 10)
    {
        for(quint8 i = 0, j = 0; i < values.count() - 1; i += 2, j++)
        {
            quint16 value1 = values.at(i + 1);
            quint16 value2 = values.at(i);

            cell_value.word[0] = value1;
            cell_value.word[1] = value2;

            QLineEdit* item = m_protectionExternal_cell.at(j);

            item->setText(QString::number(cell_value.value, 'f', 6));
        }
    }
}
//-------------------------------------------------------------------------------------
void ConfiguratorWindow::displayProtectionTemperatureSetValues(QVector<quint16> values)
{
    union
    {
        quint16 word[2];
        float   value;
    } cell_value;

    if(values.count() == 12)
    {
        for(quint8 i = 0, j = 0; i < values.count() - 1; i += 2, j++)
        {
            quint16 value1 = values.at(i + 1);
            quint16 value2 = values.at(i);

            cell_value.word[0] = value1;
            cell_value.word[1] = value2;

            QLineEdit* item = m_protectionTemperature_cell.at(j);

            item->setText(QString::number(cell_value.value, 'f', 6));
        }
    }
}
//-------------------------------------------------------------------------------
void ConfiguratorWindow::displayProtectionLevelSetValues(QVector<quint16> values)
{
    union
    {
        quint16 word[2];
        float   value;
    } cell_value;

    if(values.count() == 4)
    {
        for(quint8 i = 0, j = 0; i < values.count() - 1; i += 2, j++)
        {
            quint16 value1 = values.at(i + 1);
            quint16 value2 = values.at(i);

            cell_value.word[0] = value1;
            cell_value.word[1] = value2;

            QLineEdit* item = m_protectionLevel_cell.at(j);

            item->setText(QString::number(cell_value.value, 'f', 6));
        }
    }
}
//-----------------------------------------------------------------------------
void ConfiguratorWindow::displayProtectionBruSetValues(QVector<quint16> values)
{
    union
    {
        quint16 word[2];
        float   value;
    } cell_value;

    if(values.count() == 8)
    {
        for(quint8 i = 0, j = 0; i < values.count() - 1; i += 2, j++)
        {
            quint16 value1 = values.at(i + 1);
            quint16 value2 = values.at(i);

            cell_value.word[0] = value1;
            cell_value.word[1] = value2;

            QLineEdit* item = m_protectionBru_cell.at(j);

            item->setText(QString::number(cell_value.value, 'f', 6));
        }
    }
}
//--------------------------------------------------------------------------------
void ConfiguratorWindow::displayProtectionVacuumSetValues(QVector<quint16> values)
{
    union
    {
        quint16 word[2];
        float   value;
    } cell_value;

    if(values.count() == 6)
    {
        for(quint8 i = 0, j = 0; i < values.count() - 1; i += 2, j++)
        {
            quint16 value1 = values.at(i + 1);
            quint16 value2 = values.at(i);

            cell_value.word[0] = value1;
            cell_value.word[1] = value2;

            QLineEdit* item = m_protectionVacuum_cell.at(j);

            item->setText(QString::number(cell_value.value, 'f', 6));
        }
    }
}
//-----------------------------------------------------------------------
void ConfiguratorWindow::displayAutomationValues(QVector<quint16> values)
{
    union
    {
        quint16 word[2];
        float   value;
    } cell_value;

    if(values.count() == 12)
    {
        for(quint8 i = 0, j = 0; i < values.count() - 1; i += 2, j++)
        {
            quint16 value1 = values.at(i + 1);
            quint16 value2 = values.at(i);

            cell_value.word[0] = value1;
            cell_value.word[1] = value2;

            QLineEdit* item = m_automation_cell.at(j);

            item->setText(QString::number(cell_value.value, 'f', 6));
        }
    }
}
//-------------------------------------------------------------------------
void ConfiguratorWindow::displaySwitchDeviceValues(QVector<quint16> values)
{
    union
    {
        quint16 word[2];
        float   value;
    } cell_value;

    if(values.count() == 36)
    {
        for(quint8 i = 0, j = 0; i < values.count() - 1; i += 2, j++)
        {
            quint16 value1 = values.at(i + 1);
            quint16 value2 = values.at(i);

            cell_value.word[0] = value1;
            cell_value.word[1] = value2;

            QLineEdit* item = m_switch_device_cell.at(j);

            item->setText(QString::number(cell_value.value, 'f', 6));
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
    connect(m_tim_calculate, &QTimer::timeout, this, &ConfiguratorWindow::calculateRead);
//    connect(ui->pbtnReadInputAnalog, &QPushButton::clicked, this, &ConfiguratorWindow::calibrationRead);
//    connect(ui->pbtnWriteInputAnalog, &QPushButton::clicked, this, &ConfiguratorWindow::calibrationWrite);
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
    connect(m_switch_device_group, SIGNAL(buttonClicked(int)), this, SLOT(switchDeviceChangedID(int)));
    connect(m_additional_group, SIGNAL(buttonClicked(int)), this, SLOT(additionalChangedID(int)));
//    connect(ui->pbtnReadProtection, &QPushButton::clicked, this, &ConfiguratorWindow::protectionRead);
//    connect(ui->pbtnWriteProtection, &QPushButton::clicked, this, &ConfiguratorWindow::protectionWrite);
    connect(m_modbusDevice, &CModbus::errorDevice, this, &ConfiguratorWindow::errorDevice);
    connect(ui->sboxTimeout, SIGNAL(valueChanged(int)), this, SLOT(timeoutValueChanged(int)));
    connect(ui->sboxNumRepeat, SIGNAL(valueChanged(int)), this, SLOT(numberRepeatChanged(int)));
    connect(ui->chboxTerminal, &QCheckBox::stateChanged, this, &ConfiguratorWindow::terminalVisiblity);
    connect(m_modbusDevice, &CModbus::rawData, m_terminal, &CTerminal::appendData);
    connect(m_terminal, &CTerminal::closeTerminal, this, &ConfiguratorWindow::terminalVisiblity);
    connect(m_modbusDevice, &CModbus::infoLog, this, &ConfiguratorWindow::saveLog);
    connect(ui->treewgtDeviceMenu, &QTreeWidget::itemClicked, this, &ConfiguratorWindow::itemClicked);
    connect(ui->pbtnReadAllBlock, &QPushButton::clicked, this, &ConfiguratorWindow::readSettings);
    connect(ui->pbtnWriteAllBlock, &QPushButton::clicked, this, &ConfiguratorWindow::writeSettings);
    connect(ui->pbtnReadCurrentBlock, &QPushButton::clicked, this, &ConfiguratorWindow::readSetCurrent);
    connect(ui->pbtnWriteCurrentBlock, &QPushButton::clicked, this, &ConfiguratorWindow::writeSetCurrent);
    connect(ui->tbntExpandItems, &QToolButton::clicked, this, &ConfiguratorWindow::expandItemTree);
}
