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
    m_additional_group(nullptr),
    m_versionWidget(nullptr)
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
    initCellBind(); // инициализация привязки настроек к адресам
    initPurposeBind(); // инициализация привязки "матрицы привязок выходов" к адресам
    initModelTables();

    if(!m_logFile->open(QFile::ReadWrite))
    {
        statusBar()->showMessage(tr("Ошибка. Невозможно открыть log-файл"));
    }
    else
        m_logFile->write(tr("Запуск программы...").toStdString().c_str());

    refreshSerialPort();
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
    sendSettingReadRequest(tr("M01"), tr("KU0X_"), CDataUnitType::ReadHoldingRegisters, 42);
}
//--------------------------------------
void ConfiguratorWindow::inAnalogWrite()
{
    sendSettingWriteRequest(tr("M01"), tr("KU0X_"));
}
//-----------------------------------------
void ConfiguratorWindow::controlStateRead()
{
    sendSettingReadRequest(tr("K01"), tr("M95"), CDataUnitType::ReadHoldingRegisters, 54);
}
//------------------------------------------
void ConfiguratorWindow::controlStateWrite()
{
    sendSettingWriteRequest(tr("K01"), tr("M95"));
}
//---------------------------------------------
void ConfiguratorWindow::protectionMTZSetRead()
{
    sendSettingReadRequest(tr("M06"), tr("X05a"), CDataUnitType::ReadHoldingRegisters, 38);
}
//----------------------------------------------
void ConfiguratorWindow::protectionMTZSetWrite()
{
    sendSettingWriteRequest(tr("M06"), tr("X05a"));
}
//------------------------------------------------
void ConfiguratorWindow::protectionEarthySetRead()
{
    sendSettingReadRequest(tr("M23"), tr("X09a"), CDataUnitType::ReadHoldingRegisters, 36);
}
//-------------------------------------------------
void ConfiguratorWindow::protectionEarthySetWrite()
{
    sendSettingWriteRequest(tr("M23"), tr("X09a"));
}
//-----------------------------------------------
void ConfiguratorWindow::protectionPowerSetRead()
{
    sendSettingReadRequest(tr("M49"), tr("X14"), CDataUnitType::ReadHoldingRegisters, 30);
}
//------------------------------------------------
void ConfiguratorWindow::protectionPowerSetWrite()
{
    sendSettingWriteRequest(tr("M49"), tr("X14"));
}
//-----------------------------------------------
void ConfiguratorWindow::protectionMotorSetRead()
{
    sendSettingReadRequest(tr("M20"), tr("X10"), CDataUnitType::ReadHoldingRegisters, 12);
}
//------------------------------------------------
void ConfiguratorWindow::protectionMotorSetWrite()
{
    sendSettingWriteRequest(tr("M20"), tr("X10"));
}
//---------------------------------------------------
void ConfiguratorWindow::protectionFrequencySetRead()
{
    sendSettingReadRequest(tr("M52"), tr("X18"), CDataUnitType::ReadHoldingRegisters, 24);
}
//----------------------------------------------------
void ConfiguratorWindow::protectionFrequencySetWrite()
{
    sendSettingWriteRequest(tr("M52"), tr("X18"));
}
//--------------------------------------------------
void ConfiguratorWindow::protectionExternalSetRead()
{
    sendSettingReadRequest(tr("M64"), tr("M76"), CDataUnitType::ReadHoldingRegisters, 10);
}
//---------------------------------------------------
void ConfiguratorWindow::protectionExternalSetWrite()
{
    sendSettingWriteRequest(tr("M64"), tr("M76"));
}
//-----------------------------------------------------
void ConfiguratorWindow::protectionTemperatureSetRead()
{
    sendSettingReadRequest(tr("M67"), tr("X21"), CDataUnitType::ReadHoldingRegisters, 12);
}
//------------------------------------------------------
void ConfiguratorWindow::protectionTemperatureSetWrite()
{
    sendSettingWriteRequest(tr("M67"), tr("X21"));
}
//-----------------------------------------------
void ConfiguratorWindow::protectionLevelSetRead()
{
    sendSettingReadRequest(tr("M78"), tr("M79"), CDataUnitType::ReadHoldingRegisters, 4);
}
//------------------------------------------------
void ConfiguratorWindow::protectionLevelSetWrite()
{
    sendSettingWriteRequest(tr("M78"), tr("M79"));
}
//---------------------------------------------
void ConfiguratorWindow::protectionBruSetRead()
{
    sendSettingReadRequest(tr("M96"), tr("M99"), CDataUnitType::ReadHoldingRegisters, 8);
}
//----------------------------------------------
void ConfiguratorWindow::protectionBruSetWrite()
{
    sendSettingWriteRequest(tr("M96"), tr("M99"));
}
//------------------------------------------------
void ConfiguratorWindow::protectionVacuumSetRead()
{
    sendSettingReadRequest(tr("M91"), tr("X23"), CDataUnitType::ReadHoldingRegisters, 6);
}
//-------------------------------------------------
void ConfiguratorWindow::protectionVacuumSetWrite()
{
    sendSettingWriteRequest(tr("M91"), tr("X23"));
}
//------------------------------------------
void ConfiguratorWindow::automationSetRead()
{
    sendSettingReadRequest(tr("M82"), tr("M89"), CDataUnitType::ReadHoldingRegisters, 12);
}
//-------------------------------------------
void ConfiguratorWindow::automationSetWrite()
{
    sendSettingWriteRequest(tr("M82"), tr("M89"));
}
//--------------------------------------------
void ConfiguratorWindow::switchDeviceSetRead()
{
    sendSettingReadRequest(tr("K02"), tr("K10"), CDataUnitType::ReadHoldingRegisters, 30);
}
//---------------------------------------------
void ConfiguratorWindow::switchDeviceSetWrite()
{
    sendSettingWriteRequest(tr("K02"), tr("K10"));
}
//--------------------------------------------------------
void ConfiguratorWindow::responseRead(CDataUnitType& unit)
{
    if(unit.is_empty())
        return;
    
    qDebug() << "Получен ответ: " << unit.valueCount();
    emit m_modbusDevice->infoLog(tr("Получен ответ: ") + QString::number(unit.valueCount()) + tr(" байт \n"));
    
    RequestType type = (RequestType)unit.property(tr("REQUEST")).toInt();

    if(type == CALCULATE_TYPE)
        displayCalculateValues(unit.values());
    else if(type == GENERAL_TYPE)
        displaySettingResponse(unit);
    else if(type == PURPOSE_OUT_TYPE)
        displayPurposeResponse(unit);
}
//-----------------------------
void ConfiguratorWindow::show()
{
    QMainWindow::show();

    setWindowState(Qt::WindowFullScreen);
    setWindowState(Qt::WindowMaximized);
    
    m_terminal->hide();
    ui->stwgtMain->setCurrentIndex(0);

    m_versionWidget = new CVersionSoftware(this);
    versionParser();
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
    else if(itemName == tr("НАЗНАЧЕНИЕ СВЕТОДИОДОВ"))
    {
        ui->stwgtMain->setCurrentIndex(22);
    }
    else if(itemName == tr("НАЗНАЧЕНИЕ ДИСКРЕТНЫХ ВХОДОВ"))
    {
        ui->stwgtMain->setCurrentIndex(23);
    }
    else if(itemName == tr("НАЗНАЧЕНИЕ РЕЛЕ"))
    {
        ui->stwgtMain->setCurrentIndex(24);
    }
    else if(itemName == tr("НАЗНАЧЕНИЕ КЛАВИАТУРЫ"))
    {
        ui->stwgtMain->setCurrentIndex(25);
    }
}
//-------------------------------------
void ConfiguratorWindow::readSettings()
{
    if(!m_modbusDevice->is_open())
        return;

    qint32 index = ui->stwgtMain->currentIndex();

    if(index >= 0 && index < 22) // выбрана группа "Настройки"
    {
        controlStateRead(); // чтение настроек состояний
        inAnalogRead(); // чтение настроек "Основные" и "Калибровки"
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
    if(!m_modbusDevice->is_open())
        return;

    qint32 index = ui->stwgtMain->currentIndex();

    switch(index)
    {
        case 0:
        case 1:
            inAnalogRead(); // чтение настроек "Основные" и "Калибровки"
        break;

        case 2:
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
            protectionBruSetRead(); // чтение настроек защит БРУ
        break;

        case 11:
            protectionVacuumSetRead(); // чтение настроек вакуумных защит
        break;

        case 12:
            switchDeviceSetRead(); // чтение настроек коммутационных аппаратов
        break;

        case 13:
            automationSetRead(); // чтение настроек автоматики
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

        case 22: // привязки выходов (светодиодов)
            sendPurposeReadRequest(tr("LED1"), tr("LED2"));
            sendPurposeReadRequest(tr("LED3"), tr("LED4"));
            sendPurposeReadRequest(tr("LED5"), tr("LED6"));
            sendPurposeReadRequest(tr("LED7"), tr("LED8"));
        break;

        case 23: // привязки входов
        break;

        case 24: // привязки выходов (реле)
            sendPurposeReadRequest(tr("DO1"), tr("DO2"));
//            sendPurposeReadRequest(tr("DO4"), tr("DO5"));
//            sendPurposeReadRequest(tr("DO6"), tr("DO7"));
//            sendPurposeReadRequest(tr("DO8"), tr("DO9"));
//            sendPurposeReadRequest(tr("DO10"), tr("DO11"));
//            sendPurposeReadRequest(tr("DO12"), tr("DO13"));
        break;

        case 25: // привязки выходов (клавиатуры)
        break;
    }
}
//--------------------------------------
void ConfiguratorWindow::writeSettings()
{
    if(!m_modbusDevice->is_open())
        return;

    qint32 index = ui->stwgtMain->currentIndex();

    if(index >= 0 && index < 22) // выбрана группа "Настройки"
    {
        controlStateWrite(); // запись настроек состояния
        inAnalogWrite(); // запись настроек "Основные" и "Калибровки"
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
    if(!m_modbusDevice->is_open())
        return;

    qint32 index = ui->stwgtMain->currentIndex();

    switch(index)
    {
        case 0:
        case 1:
            inAnalogWrite(); // запись настроек "Основные" и "Калибровки"
        break;

        case 2:
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
            protectionBruSetWrite(); // запись настроек защит БРУ
        break;

        case 11:
            protectionVacuumSetWrite(); // запись настроек вакуумных защит
        break;

        case 12:
            switchDeviceSetWrite(); // запись настроек коммутационных аппаратов
        break;

        case 13:
            automationSetWrite(); // запись настроек автоматики
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
        ui->tbntExpandItems->setToolTip(tr("Свернуть меню"));
    }
    else
    {
        ui->treewgtDeviceMenu->collapseAll();
        ui->tbntExpandItems->setIcon(QIcon(tr(":/images/resource/images/branch_close.png")));
        ui->tbntExpandItems->setToolTip(tr("Развернуть меню"));
    }
}
//-----------------------------------------
void ConfiguratorWindow::versionSowftware()
{
    m_versionWidget->show();
}
//--------------------------------------
void ConfiguratorWindow::initMenuPanel()
{
    itemSettings   = new QTreeWidgetItem(ui->treewgtDeviceMenu);
    itemJournals   = new QTreeWidgetItem(ui->treewgtDeviceMenu);
    itemMeasures   = new QTreeWidgetItem(ui->treewgtDeviceMenu);
    itemMonitoring = new QTreeWidgetItem(ui->treewgtDeviceMenu);

    itemSetInputAnalogs     = new QTreeWidgetItem(itemSettings);
    itemSetProtections      = new QTreeWidgetItem(itemSettings);
    itemSetDevConnections   = new QTreeWidgetItem(itemSettings);
    itemSetAutomation       = new QTreeWidgetItem(itemSettings);
    itemSetLedPurpose       = new QTreeWidgetItem(itemSettings);
    itemSetDiscretInPurpose = new QTreeWidgetItem(itemSettings);
    itemSetRelayPurpose     = new QTreeWidgetItem(itemSettings);
    itemSetKeyboardPurpose  = new QTreeWidgetItem(itemSettings);

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
    itemSetLedPurpose->setText(0, tr("Назначение светодиодов"));
    itemSetDiscretInPurpose->setText(0, tr("Назначение дискретных входов"));
    itemSetRelayPurpose->setText(0, tr("Назначение реле"));
    itemSetKeyboardPurpose->setText(0, tr("Назначение клавиатуры"));

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
    itemSettings->addChild(itemSetLedPurpose);
    itemSettings->addChild(itemSetDiscretInPurpose);
    itemSettings->addChild(itemSetRelayPurpose);
    itemSettings->addChild(itemSetKeyboardPurpose);

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
    m_additional_group->addButton(ui->pbtnAddAVRSection);
    m_additional_group->addButton(ui->pbtnAddAPV);
    m_additional_group->addButton(ui->pbtnAddAPV_Start);
    m_additional_group->setId(ui->pbtnAddAVR, 0);
    m_additional_group->setId(ui->pbtnAddAVRSection, 1);
    m_additional_group->setId(ui->pbtnAddAPV, 2);
    m_additional_group->setId(ui->pbtnAddAPV_Start, 3);
    
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
//-------------------------------------
void ConfiguratorWindow::initCellBind()
{
    QSqlQuery query(tr("SELECT * FROM iodevice WHERE type = 'SET'"));

    if(!query.exec())
        return;

    while(query.next())
    {
        QString key  = query.value(tr("key")).toString();
        QString name = query.value(tr("widget")).toString();
        int     addr = query.value(tr("address")).toInt();

        m_cell_list.append(qMakePair(key, qMakePair(addr, name)));
    }
}
//----------------------------------------
void ConfiguratorWindow::initPurposeBind()
{
    if(!m_db.isOpen())
        return;

    QSqlQuery query;

    QStringList type_list;
    type_list << tr("INPUT") << tr("RELAY") << tr("LED");

    for(const QString& type: type_list)
    {
        if(query.exec(tr("SELECT key, address, description FROM iodevice WHERE type = '") + type + "'"))
        {
            while(query.next())
            {
                QString          key  = query.value(tr("key")).toString();
                int              addr = query.value(tr("address")).toInt();
                QString          desc = query.value(tr("description")).toString();
                QVector<QString> var_list;

                QSqlQuery query_var(tr("SELECT var_key FROM purpose WHERE io_key = '") + key + "'");

                if(query_var.exec())
                {
                    while(query_var.next())
                    {
                        var_list.append(query_var.value(0).toString());
                    }
                }

                m_purpose_list.append(qMakePair(key, qMakePair(addr, qMakePair(desc, var_list))));
            }
        }
    }
}
//----------------------------------------
void ConfiguratorWindow::initModelTables()
{
    QSqlQuery query(tr("SELECT * FROM variable"));

    QStringList columns;

    if(query.exec())
    {
        while(query.next())
        {
            columns << query.value(tr("key")).toString();
        }
    }

    if(columns.isEmpty())
        return;

    QList<QTableView*> view_list = QList<QTableView*>() << ui->tablewgtLedPurpose << ui->tablewgtDiscreteInputPurpose
                                                        << ui->tablewgtRelayPurpose << ui->tablewgtKeyboardPurpose;

    for(QTableView* wgt: view_list)
    {
        QString first = "";
        QString last  = "";

        if(wgt == ui->tablewgtLedPurpose)
        {
            first = tr("LED1");
            last  = tr("LED8");
        }
        else if(wgt == ui->tablewgtDiscreteInputPurpose)
        {
            first = tr("DI01");
            last  = tr("DI20");
        }
        else if(wgt == ui->tablewgtRelayPurpose)
        {
            first = tr("DO1");
            last  = tr("DO13");
        }
        else if(wgt == ui->tablewgtKeyboardPurpose)
        {
            break; // заглушка
        }

        if(first.isEmpty() || last.isEmpty())
            break;

        QPoint index = indexPurposeKey(first, last);

        if(index.x() == -1 || index.y() == -1)
            break;

        QVector<CRow> rows;

        for(int i = index.x(); i <= index.y(); i++)
        {
            QVector<int> indexes = indexVariableFromKey(columns, m_purpose_list[i].first);
            CRow row(m_purpose_list[i].second.second.first, columns.count());

            row.setActiveColumnList(indexes);

            rows.append(row);
        }

        if(rows.isEmpty())
            return;

        CDataTable data(rows, columns);

        initTable(wgt, data);
    }
}
//----------------------------------
void ConfiguratorWindow::connectDb()
{
    m_db = QSqlDatabase::addDatabase("QSQLITE");

    m_db.setDatabaseName("db/db.db");

    if(!m_db.open())
    {
        QMessageBox::critical(this, tr("База данных"), tr("Невозможно открыть базу данных: ") + m_db.lastError().text());
        exit(1);
    }
}
//---------------------------------------------------------------------
void ConfiguratorWindow::initTable(QTableView* table, CDataTable& data)
{
    CHeaderTable* header_horizontal = new CHeaderTable(Qt::Horizontal, table);
    CHeaderTable* header_vertical   = new CHeaderTable(Qt::Vertical, table);

    table->setHorizontalHeader(header_horizontal);
    table->setVerticalHeader(header_vertical);

    CMatrixPurposeModel* model = new CMatrixPurposeModel(data);

    table->setItemDelegate(new CTableItemDelegate);
    table->setModel(model);
    table->resizeColumnsToContents();
    table->resizeRowsToContents();
}
//----------------------------------------------------------------------
void ConfiguratorWindow::displayCalculateValues(QVector<quint16> values)
{
    m_calculateWidget->setData(values);
}
//------------------------------------------------------------------
void ConfiguratorWindow::displaySettingResponse(CDataUnitType& unit)
{
    if(unit.is_empty())
        return;

    QString first = unit.property("FIRST").toString();
    QString last  = unit.property("LAST").toString();

    QPoint indexWgt = indexSettingKey(first, last);

    if(indexWgt.x() == -1 || indexWgt.y() == -1)
        return;

    int index = 0;

    union
    {
        quint16 w[2];
        float   f;
        int     i;
    } value;

    for(int i = indexWgt.x(); i <= indexWgt.y(); i++)
    {
        if(index >= unit.valueCount())
            break;

        QString nameWgt = m_cell_list[i].second.second;

        if(nameWgt.isEmpty())
        {
            index += 2;
            continue;
        }

        QWidget* widget = findChild<QWidget*>(nameWgt);

        QString classWgt = widget->metaObject()->className();

        if(classWgt == tr("QLineEdit"))
        {
            QLineEdit* edit = qobject_cast<QLineEdit*>(widget);

            if(edit)
            {
                quint16 val1 = unit.values().at(index + 1);
                quint16 val2 = unit.values().at(index);

                value.w[0] = val1;
                value.w[1] = val2;

                QString str = QString::number(value.f, 'f', 6);

                if(!str.isEmpty())
                    edit->setText(str);

                index += 2;
            }
        }
        else if(classWgt == tr("QComboBox"))
        {
            QComboBox* box = qobject_cast<QComboBox*>(widget);

            if(box)
            {
                quint16 i = unit.values().at(index);

                box->setCurrentIndex(i);

                index++;
            }
        }
    }
}
//------------------------------------------------------------------
void ConfiguratorWindow::displayPurposeResponse(CDataUnitType& unit)
{
    if(unit.valueCount() == 0)
        return;

    QString first = unit.property(tr("FIRST")).toString();
    QString last  = unit.property(tr("LAST")).toString();

    if(first.isEmpty() || last.isEmpty())
        return;

    QPoint indexes = indexPurposeKey(first, last);

    if(indexes.x() == -1 || indexes.y() == -1)
        return;

    int size = m_purpose_list[indexes.y()].second.first - m_purpose_list[indexes.x()].second.first + 24;

    if(size != unit.valueCount())
        return;

    QTableView* table = nullptr;

    if(m_purpose_list[indexes.x()].first.contains(tr("DI"), Qt::CaseInsensitive) &&
       m_purpose_list[indexes.x()].first.contains(tr("DI"), Qt::CaseInsensitive)) // входы
    {
        table  = ui->tablewgtDiscreteInputPurpose;
    }
    else if(m_purpose_list[indexes.x()].first.contains(tr("DO"), Qt::CaseInsensitive) &&
            m_purpose_list[indexes.x()].first.contains(tr("DO"), Qt::CaseInsensitive)) // выходы: реле
    {
        table  = ui->tablewgtRelayPurpose;
    }
    else if(m_purpose_list[indexes.x()].first.contains(tr("LED"), Qt::CaseInsensitive) &&
            m_purpose_list[indexes.x()].first.contains(tr("LED"), Qt::CaseInsensitive)) // выходы: светодиоды
    {
        table  = ui->tablewgtLedPurpose;
    }

    if(!table)
        return;

    CMatrixPurposeModel* model = static_cast<CMatrixPurposeModel*>(table->model());

    if(!model)
        return;

    CDataTable& data = model->dataTable();

    int var_count = data.columnCounts()/16;

    if(data.columnCounts()%16)
        var_count++;

    int row_count = indexes.y() - indexes.x() + 1;

    for(int i = 0; i < row_count; i++)
    {
        qDebug() << "i: " << i;

        for(int j = 0, m = 0; j < var_count*2; j += 2, m++)
        {
            qDebug() << "j: " << j << ", m: " << m;

            int pos = i*var_count*2;

            quint32 value      = unit.value(pos + 1) | ((unit.value(pos) << 16));
            int     size_dword = (int)sizeof(value)*8;

            for(int k = 0; k < size_dword; k++)
            {
                bool state = (value >> k)&0x00000001;

                if(i == row_count - 1 && k >= var_count*16 - data.columnCounts())
                    break;

                int column = m*32 + k;

                qDebug() << "k = " << k;
//                data[i][column].setState(state);
            }
        }
    }

    model->updateData();
}
//--------------------------------------
void ConfiguratorWindow::versionParser()
{
    QFile file(tr(":/files/resource/files/version.txt"));

    if(!file.open(QFile::ReadOnly))
    {
        statusBar()->showMessage(tr("Нет файла версии или он поврежден..."), 2000);
        file.close();

        return;
    }

    QTextStream in(&file);

    bool fMajor = false;
    bool fMinor = false;
    bool fBuild = false;
    bool fDesc  = false;

    QString version = "";
    QString desc    = "";
    QString param   = tr("MAJOR");

    QMap<QString, QString> ver;

    while(!in.atEnd())
    {
        QString line = in.readLine();

        if(line.isEmpty())
            continue;

        if(!fMajor || !fMinor || !fBuild || !fDesc)
        {
            if(line.contains(param, Qt::CaseInsensitive))
            {
                QStringList list = line.split(":");

                if(!list.isEmpty())
                {
                    QString temp = "";

                    if(list.count() > 1)
                        temp = list.at(1);

                    temp = temp.replace(tr(" "), "");

                    if(!fMajor)
                    {
                        version = tr("v") + temp + QString(".");
                        fMajor  = true;
                        param   = tr("MINOR");

                        continue;
                    }
                    else if(fMajor && !fMinor)
                    {
                        version += ((temp.toInt() < 10)?tr("0") + temp:temp) + tr(" build ");
                        fMinor   = true;
                        param    = tr("BUILD");

                        continue;
                    }
                    else if(fMajor && fMinor && !fBuild)
                    {
                        version += temp;
                        fBuild   = true;
                        param    = tr("<DESCRIPTION>");

                        continue;
                    }
                    else if(fMajor && fMinor && fBuild && !fDesc)
                    {
                        fDesc = true;
                        param = tr("</DESCRIPTION>");

                        continue;
                    }
                }
            }
        }
        else if(fDesc)
        {
            if(!line.contains(param, Qt::CaseInsensitive))
                desc += line + "\n";
            else
            {
                ver.insert(version, desc);

                fMajor = fMinor = fBuild = fDesc = false;

                version.clear();
                desc.clear();

                param = tr("MAJOR");
            }
        }
    }

    file.close();

    m_versionWidget->setText(ver);

    QString title = this->windowTitle();

    if(!ver.isEmpty())
        title += tr(" ") + ver.lastKey();

    this->setWindowTitle(title);
}
//---------------------------------------------------------------------------------
int ConfiguratorWindow::sizeBlockSetting(const QString& first, const QString& last)
{
    int iFirst = -1, iSecond = -1;

    for(quint8 i = 0; i < m_cell_list.count(); i++)
    {
        QPair<QString, QPair<int, QString> > pair = m_cell_list.at(i);

        if(pair.first == first)
            iFirst = i;

        if(pair.first == last)
            iSecond = i;
    }

    if(iFirst != -1 && iSecond != -1 && (iSecond >= iFirst))
        return ((iSecond - iFirst) + 1);

    return -1;
}
//----------------------------------------------------------------------------------------
void ConfiguratorWindow::sendSettingReadRequest(const QString& first, const QString& last,
                                                CDataUnitType::FunctionType type, int size)
{
    if(size <= 0)
        return;

    int addr = addressSettingKey(first);

    CDataUnitType unit(ui->sboxSlaveID->value(), type, addr, QVector<quint16>() << size);

    unit.setProperty(tr("REQUEST"), GENERAL_TYPE);
    unit.setProperty(tr("FIRST"), first);
    unit.setProperty(tr("LAST"), last);

    m_modbusDevice->request(unit);
}
//-----------------------------------------------------------------------------------------
void ConfiguratorWindow::sendSettingWriteRequest(const QString& first, const QString& last)
{
    if(first.isEmpty() || last.isEmpty())
        return;

    QPoint index = indexSettingKey(first, last);

    if(index.x() == -1 || index.y() == -1)
        return;

    union
    {
        quint16 w[2];
        float f;
        int i;
    } value;

    QVector<quint16>  data;

    for(int i = index.x(); i < index.y(); i++)
    {
        QString nameWgt = m_cell_list[i].second.second;

        if(nameWgt.isEmpty())
        {
            data.append(0.0f);
            data.append(0.0f);

            continue;
        }

        QWidget* widget = findChild<QWidget*>(nameWgt);

        if(!widget)
            continue;

        QString classWgt = widget->metaObject()->className();

        if(classWgt == tr("QLineEdit"))
        {
            QLineEdit* edit = qobject_cast<QLineEdit*>(widget);

            if(!edit)
                continue;

            value.f = edit->text().toFloat();

            data.append(value.w[1]);
            data.append(value.w[0]);
        }
        else if(classWgt == tr("QComboBox"))
        {
            QComboBox* box = qobject_cast<QComboBox*>(widget);

            if(!box)
                continue;

            data.append((quint16)box->currentIndex());
        }
    }

    CDataUnitType::FunctionType funType = ((data.count() == 1)?CDataUnitType::WriteSingleRegister:
                                                               CDataUnitType::WriteMultipleRegisters);

    CDataUnitType unit(ui->sboxSlaveID->value(), funType, index.x(), data);

    unit.setProperty(tr("FIRST"), first);
    unit.setProperty(tr("LAST"), last);

    m_modbusDevice->request(unit);
}
//----------------------------------------------------------------------------------------
void ConfiguratorWindow::sendPurposeReadRequest(const QString& first, const QString& last)
{
    int faddr = addressPurposeKey(first);
    int laddr = addressPurposeKey(last);

    if(faddr == -1 || laddr == -1)
        return;

    int size = laddr - faddr + 24;

    CDataUnitType unit(ui->sboxSlaveID->value(), CDataUnitType::ReadHoldingRegisters, faddr,
                       QVector<quint16>() << size);

    unit.setProperty(tr("REQUEST"), PURPOSE_OUT_TYPE);
    unit.setProperty(tr("FIRST"), first);
    unit.setProperty(tr("LAST"), last);

    m_modbusDevice->request(unit);
}
//-----------------------------------------------------------------
int ConfiguratorWindow::addressSettingKey(const QString& key) const
{
    for(QPair<QString, QPair<int, QString> > pair: m_cell_list)
    {
        if(pair.first == key)
        {
            return pair.second.first;
        }
    }

    return -1;
}
//-----------------------------------------------------------------
int ConfiguratorWindow::addressPurposeKey(const QString& key) const
{
    for(QPair<QString, QPair<int, QPair<QString, QVector<QString> > > > purpose: m_purpose_list)
    {
        if(purpose.first == key)
        {
            return purpose.second.first;
        }
    }

    return -1;
}
//-----------------------------------------------------------------------------------
QPoint ConfiguratorWindow::indexSettingKey(const QString& first, const QString& last)
{
    QPoint index(-1, -1);
    QString key = first;

    for(int i = 0; i < m_cell_list.count(); i++)
    {
        if(m_cell_list[i].first == key)
        {
            if(index.x() == -1)
            {
                index.setX(i);
                key = last;
            }
            else
            {
                index.setY(i);
                break;
            }
        }
    }

    return index;
}
//-----------------------------------------------------------------------------------
QPoint ConfiguratorWindow::indexPurposeKey(const QString& first, const QString& last)
{
    QPoint  index(-1, -1);
    QString key = first;

    for(int i = 0; i < m_purpose_list.count(); i++)
    {
        if(m_purpose_list[i].first == key)
        {
            if(index.x() == -1)
            {
                index.setX(i);
                key = last;
            }
            else
            {
                index.setY(i);
                break;
            }
        }
    }

    return index;
}
//------------------------------------------------------------------------------------------------------
QVector<int> ConfiguratorWindow::indexVariableFromKey(const QStringList& variables, const QString& key)
{
    QVector<int> indexes = QVector<int>();

    QSqlQuery query(tr("SELECT var_key FROM purpose WHERE io_key = '") + key + "'");

    QStringList list = QStringList();

    if(query.exec())
    {
        while(query.next())
        {
            list << query.value(0).toString();
        }
    }

    if(!list.isEmpty())
    {
        for(int i = 0; i < list.count(); i++)
        {
            QString var1 = list.at(i);

            for(int j = 0; j < variables.count(); j++)
            {
                QString var2 = variables.at(j);

                if(var1 == var2)
                {
                    indexes << j;
                }
            }
        }
    }

    return indexes;
}
//------------------------------------
void ConfiguratorWindow::initConnect()
{
    connectDb();

    connect(ui->pbtnPortCtrl, &QPushButton::clicked, this, &ConfiguratorWindow::serialPortCtrl);
    connect(m_modbusDevice, &CModbus::connectDeviceState, this, &ConfiguratorWindow::stateChanged);
    connect(ui->tbtnPortRefresh, &QToolButton::clicked, this, &ConfiguratorWindow::refreshSerialPort);
    connect(m_modbusDevice, &CModbus::dataReady, this, &ConfiguratorWindow::responseRead);
    connect(m_tim_calculate, &QTimer::timeout, this, &ConfiguratorWindow::calculateRead);
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
    connect(ui->pbtnVersionSoftware, &QPushButton::clicked, this, &ConfiguratorWindow::versionSowftware);
}
