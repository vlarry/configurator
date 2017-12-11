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
    sendGeneralReadRequest(tr("M01"), tr("KU0X_"), CDataUnitType::ReadHoldingRegisters, 42);
}
//--------------------------------------
void ConfiguratorWindow::inAnalogWrite()
{
    sendGeneralWriteRequest(tr("M01"), tr("KU0X_"));
}
//-----------------------------------------
void ConfiguratorWindow::controlStateRead()
{
    sendGeneralReadRequest(tr("K01"), tr("M95"), CDataUnitType::ReadHoldingRegisters, 54);
}
//------------------------------------------
void ConfiguratorWindow::controlStateWrite()
{
    sendGeneralWriteRequest(tr("K01"), tr("M95"));
}
//---------------------------------------------
void ConfiguratorWindow::protectionMTZSetRead()
{
    sendGeneralReadRequest(tr("M06"), tr("X05a"), CDataUnitType::ReadHoldingRegisters, 38);
}
//----------------------------------------------
void ConfiguratorWindow::protectionMTZSetWrite()
{
    sendGeneralWriteRequest(tr("M06"), tr("X05a"));
}
//------------------------------------------------
void ConfiguratorWindow::protectionEarthySetRead()
{
    sendGeneralReadRequest(tr("M23"), tr("X09a"), CDataUnitType::ReadHoldingRegisters, 36);
}
//-------------------------------------------------
void ConfiguratorWindow::protectionEarthySetWrite()
{
    sendGeneralWriteRequest(tr("M23"), tr("X09a"));
}
//-----------------------------------------------
void ConfiguratorWindow::protectionPowerSetRead()
{
    sendGeneralReadRequest(tr("M49"), tr("X14"), CDataUnitType::ReadHoldingRegisters, 30);
}
//------------------------------------------------
void ConfiguratorWindow::protectionPowerSetWrite()
{
    sendGeneralWriteRequest(tr("M49"), tr("X14"));
}
//-----------------------------------------------
void ConfiguratorWindow::protectionMotorSetRead()
{
    sendGeneralReadRequest(tr("M20"), tr("X10"), CDataUnitType::ReadHoldingRegisters, 12);
}
//------------------------------------------------
void ConfiguratorWindow::protectionMotorSetWrite()
{
    sendGeneralWriteRequest(tr("M20"), tr("X10"));
}
//---------------------------------------------------
void ConfiguratorWindow::protectionFrequencySetRead()
{
    sendGeneralReadRequest(tr("M52"), tr("X18"), CDataUnitType::ReadHoldingRegisters, 24);
}
//----------------------------------------------------
void ConfiguratorWindow::protectionFrequencySetWrite()
{
    sendGeneralWriteRequest(tr("M52"), tr("X18"));
}
//--------------------------------------------------
void ConfiguratorWindow::protectionExternalSetRead()
{
    sendGeneralReadRequest(tr("M64"), tr("M76"), CDataUnitType::ReadHoldingRegisters, 10);
}
//---------------------------------------------------
void ConfiguratorWindow::protectionExternalSetWrite()
{
    sendGeneralWriteRequest(tr("M64"), tr("M76"));
}
//-----------------------------------------------------
void ConfiguratorWindow::protectionTemperatureSetRead()
{
    sendGeneralReadRequest(tr("M67"), tr("X21"), CDataUnitType::ReadHoldingRegisters, 12);
}
//------------------------------------------------------
void ConfiguratorWindow::protectionTemperatureSetWrite()
{
    sendGeneralWriteRequest(tr("M67"), tr("X21"));
}
//-----------------------------------------------
void ConfiguratorWindow::protectionLevelSetRead()
{
    sendGeneralReadRequest(tr("M78"), tr("M79"), CDataUnitType::ReadHoldingRegisters, 4);
}
//------------------------------------------------
void ConfiguratorWindow::protectionLevelSetWrite()
{
    sendGeneralWriteRequest(tr("M78"), tr("M79"));
}
//---------------------------------------------
void ConfiguratorWindow::protectionBruSetRead()
{
    sendGeneralReadRequest(tr("M96"), tr("M99"), CDataUnitType::ReadHoldingRegisters, 8);
}
//----------------------------------------------
void ConfiguratorWindow::protectionBruSetWrite()
{
    sendGeneralWriteRequest(tr("M96"), tr("M99"));
}
//------------------------------------------------
void ConfiguratorWindow::protectionVacuumSetRead()
{
    sendGeneralReadRequest(tr("M91"), tr("X23"), CDataUnitType::ReadHoldingRegisters, 6);
}
//-------------------------------------------------
void ConfiguratorWindow::protectionVacuumSetWrite()
{
    sendGeneralWriteRequest(tr("M91"), tr("X23"));
}
//------------------------------------------
void ConfiguratorWindow::automationSetRead()
{
    sendGeneralReadRequest(tr("M82"), tr("M89"), CDataUnitType::ReadHoldingRegisters, 12);
}
//-------------------------------------------
void ConfiguratorWindow::automationSetWrite()
{
    sendGeneralWriteRequest(tr("M82"), tr("M89"));
}
//--------------------------------------------
void ConfiguratorWindow::switchDeviceSetRead()
{
    sendGeneralReadRequest(tr("K02"), tr("K10"), CDataUnitType::ReadHoldingRegisters, 30);
}
//---------------------------------------------
void ConfiguratorWindow::switchDeviceSetWrite()
{
    sendGeneralWriteRequest(tr("K02"), tr("K10"));
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
        displayGeneralResponse(unit);
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
        break;

        case 23: // привязки входов
        break;

        case 24: // привязки выходов (реле)
            sendPurposeReadRequest(tr("DO1"), tr("DO2"));
            sendPurposeReadRequest(tr("DO4"), tr("DO5"));
            sendPurposeReadRequest(tr("DO6"), tr("DO7"));
            sendPurposeReadRequest(tr("DO8"), tr("DO9"));
            sendPurposeReadRequest(tr("DO10"), tr("DO11"));
            sendPurposeReadRequest(tr("DO12"), tr("DO13"));
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
    // инициализация виджетов визуализирующих состояние настроек
    addNewGeneralBind(tr("K01"), ui->cboxK01, 0, LIST);
    addNewGeneralBind(tr("K03"), ui->cboxK03, 1, LIST);
    addNewGeneralBind(tr("K06"), ui->cboxK06, 2, LIST);
    addNewGeneralBind(tr("K07"), ui->cboxK07, 3, LIST);
    addNewGeneralBind(tr("K13"), ui->cboxK13, 4, LIST);
    addNewGeneralBind(tr("K14"), ui->cboxK14, 5, LIST);
    addNewGeneralBind(tr("K15"), ui->cboxK15, 6, LIST);
    addNewGeneralBind(tr("K17"), ui->cboxK17, 7, LIST);
    addNewGeneralBind(tr("K18"), ui->cboxK18, 8, LIST);
    addNewGeneralBind(tr("K23"), ui->cboxK23, 9, LIST);
    addNewGeneralBind(tr("K26"), ui->cboxK26, 10, LIST);
    addNewGeneralBind(tr("K32"), ui->cboxK32, 11, LIST);
    addNewGeneralBind(tr("K33"), ui->cboxK33, 12, LIST);
    addNewGeneralBind(tr("K34"), ui->cboxK34, 13, LIST);
    addNewGeneralBind(tr("K35"), ui->cboxK35, 14, LIST);
    addNewGeneralBind(tr("K36"), ui->cboxK36, 15, LIST);
    addNewGeneralBind(tr("K37"), ui->cboxK37, 16, LIST);
    addNewGeneralBind(tr("K38"), ui->cboxK38, 17, LIST);
    addNewGeneralBind(tr("K39"), ui->cboxK39, 18, LIST);
    addNewGeneralBind(tr("K40"), ui->cboxK40, 19, LIST);
    addNewGeneralBind(tr("K41"), ui->cboxK41, 20, LIST);
    addNewGeneralBind(tr("M04"), ui->cboxM04, 21, LIST);
    addNewGeneralBind(tr("M05"), ui->cboxM05, 22, LIST);
    addNewGeneralBind(tr("M09"), ui->cboxM09, 23, LIST);
    addNewGeneralBind(tr("M13"), ui->cboxM13, 24, LIST);
    addNewGeneralBind(tr("M16"), ui->cboxM16, 25, LIST);
    addNewGeneralBind(tr("M19"), ui->cboxM19, 26, LIST);
    addNewGeneralBind(tr("M22"), ui->cboxM22, 27, LIST);
    addNewGeneralBind(tr("M25"), ui->cboxM25, 28, LIST);
    addNewGeneralBind(tr("M29"), ui->cboxM29, 29, LIST);
    addNewGeneralBind(tr("M32"), ui->cboxM32, 30, LIST);
    addNewGeneralBind(tr("M35"), ui->cboxM35, 31, LIST);
    addNewGeneralBind(tr("M38"), ui->cboxM38, 32, LIST);
    addNewGeneralBind(tr("M39"), ui->cboxM39, 33, LIST);
    addNewGeneralBind(tr("M40"), ui->cboxM40, 34, LIST);
    addNewGeneralBind(tr("M43"), ui->cboxM43, 35, LIST);
    addNewGeneralBind(tr("M44"), ui->cboxM44, 36, LIST);
    addNewGeneralBind(tr("M45"), ui->cboxM45, 37, LIST);
    addNewGeneralBind(tr("M48"), ui->cboxM48, 38, LIST);
    addNewGeneralBind(tr("M51"), ui->cboxM51, 39, LIST);
    addNewGeneralBind(tr("M55"), ui->cboxM55, 40, LIST);
    addNewGeneralBind(tr("M59"), ui->cboxM59, 41, LIST);
    addNewGeneralBind(tr("M63"), ui->cboxM63, 42, LIST);
    addNewGeneralBind(tr("M65"), ui->cboxM65, 43, LIST);
    addNewGeneralBind(tr("M66"), ui->cboxM66, 44, LIST);
    addNewGeneralBind(tr("M71"), ui->cboxM71, 45, LIST);
    addNewGeneralBind(tr("M73"), ui->cboxM73, 46, LIST);
    addNewGeneralBind(tr("M75"), ui->cboxM75, 47, LIST);
    addNewGeneralBind(tr("M77"), ui->cboxM77, 48, LIST);
    addNewGeneralBind(tr("M81"), ui->cboxM81, 49, LIST);
    addNewGeneralBind(tr("M87"), ui->cboxM87, 50, LIST);
    addNewGeneralBind(tr("M90"), ui->cboxM90, 51, LIST);
    addNewGeneralBind(tr("M93"), ui->cboxM93, 52, LIST);
    addNewGeneralBind(tr("M95"), ui->cboxM95, 53, LIST);

    // инициализация виджетов максимальных токовых защит
    addNewGeneralBind(tr("M06"), ui->leM06, 128, FLOAT);
    addNewGeneralBind(tr("M08"), ui->leM08, 130, FLOAT);
    addNewGeneralBind(tr("K31"), ui->leK31, 132, FLOAT);
    addNewGeneralBind(tr("X01"), ui->leX01, 134, FLOAT);
    addNewGeneralBind(tr("X02"), nullptr, 136, FLOAT);
    addNewGeneralBind(tr("M10"), ui->leM10, 138, FLOAT);
    addNewGeneralBind(tr("M11"), ui->leM11, 140, FLOAT);
    addNewGeneralBind(tr("M12"), ui->leM12, 142, FLOAT);
    addNewGeneralBind(tr("X03"), ui->leX03, 144, FLOAT);
    addNewGeneralBind(tr("M14"), ui->leM14, 146, FLOAT);
    addNewGeneralBind(tr("K22"), ui->leK22, 148, FLOAT);
    addNewGeneralBind(tr("M15"), ui->leM15, 150, FLOAT);
    addNewGeneralBind(tr("X04"), ui->leX04, 152, FLOAT);
    addNewGeneralBind(tr("M17"), ui->leM17, 154, FLOAT);
    addNewGeneralBind(tr("M18"), ui->leM18, 156, FLOAT);
    addNewGeneralBind(tr("K20"), ui->leK20, 158, FLOAT);
    addNewGeneralBind(tr("M07"), ui->leM07, 160, FLOAT);
    addNewGeneralBind(tr("X05"), ui->leX05, 162, FLOAT);
    addNewGeneralBind(tr("X05a"), ui->leX05a, 164, FLOAT);

    // инициализация виджетов земельных защит
    addNewGeneralBind(tr("M23"), ui->leM23, 166, FLOAT);
    addNewGeneralBind(tr("M24"), ui->leM24, 168, FLOAT);
    addNewGeneralBind(tr("X07"), ui->leX07, 170, FLOAT);
    addNewGeneralBind(tr("K24"), ui->leK24, 172, FLOAT);
    addNewGeneralBind(tr("K25"), ui->leK25, 174, FLOAT);
    addNewGeneralBind(tr("X07a"), ui->leX07a, 176, FLOAT);
    addNewGeneralBind(tr("M26"), ui->leM26, 178, FLOAT);
    addNewGeneralBind(tr("M27"), ui->leM27, 180, FLOAT);
    addNewGeneralBind(tr("K21"), ui->leK21, 182, FLOAT);
    addNewGeneralBind(tr("M28"), ui->leM28, 184, FLOAT);
    addNewGeneralBind(tr("X08"), ui->leX08, 186, FLOAT);
    addNewGeneralBind(tr("X09"), ui->leX09, 188, FLOAT);
    addNewGeneralBind(tr("K27"), ui->leK27, 190, FLOAT);
    addNewGeneralBind(tr("K28"), ui->leK28, 192, FLOAT);
    addNewGeneralBind(tr("K29"), ui->leK29, 194, FLOAT);
    addNewGeneralBind(tr("K30"), ui->leK30, 196, FLOAT);
    addNewGeneralBind(tr("X08a"), ui->leX08a, 198, FLOAT);
    addNewGeneralBind(tr("X09a"), ui->leX09a, 200, FLOAT);

    // инициализация виджетов защит напряжения
    addNewGeneralBind(tr("M49"), ui->leM49, 202, FLOAT);
    addNewGeneralBind(tr("M50"), ui->leM50, 204, FLOAT);
    addNewGeneralBind(tr("X15"), ui->leX15, 206, FLOAT);
    addNewGeneralBind(tr("M33"), ui->leM33, 208, FLOAT);
    addNewGeneralBind(tr("M34"), ui->leM34, 210, FLOAT);
    addNewGeneralBind(tr("X11"), ui->leX11, 212, FLOAT);
    addNewGeneralBind(tr("M36"), ui->leM36, 214, FLOAT);
    addNewGeneralBind(tr("M37"), ui->leM37, 216, FLOAT);
    addNewGeneralBind(tr("X12"), ui->leX12, 218, FLOAT);
    addNewGeneralBind(tr("M41"), ui->leM41, 220, FLOAT);
    addNewGeneralBind(tr("M42"), ui->leM42, 222, FLOAT);
    addNewGeneralBind(tr("X13"), ui->leX13, 224, FLOAT);
    addNewGeneralBind(tr("M46"), ui->leM46, 226, FLOAT);
    addNewGeneralBind(tr("M47"), ui->leM47, 228, FLOAT);
    addNewGeneralBind(tr("X14"), ui->leX14, 230, FLOAT);

    // инициализация виджетов защит двигателя
    addNewGeneralBind(tr("M20"), ui->leM20, 232, FLOAT);
    addNewGeneralBind(tr("M21"), ui->leM21, 234, FLOAT);
    addNewGeneralBind(tr("X06"), ui->leX06, 236, FLOAT);
    addNewGeneralBind(tr("M30"), ui->leM30, 238, FLOAT);
    addNewGeneralBind(tr("M31"), ui->leM31, 240, FLOAT);
    addNewGeneralBind(tr("X10"), ui->leX10, 242, FLOAT);

    // инициализация виджетов защит по частоте
    addNewGeneralBind(tr("M52"), ui->leM52, 244, FLOAT);
    addNewGeneralBind(tr("M53"), ui->leM53, 246, FLOAT);
    addNewGeneralBind(tr("M54"), ui->leM54, 248, FLOAT);
    addNewGeneralBind(tr("X16"), ui->leX16, 250, FLOAT);
    addNewGeneralBind(tr("M56"), ui->leM56, 252, FLOAT);
    addNewGeneralBind(tr("M57"), ui->leM57, 254, FLOAT);
    addNewGeneralBind(tr("M58"), ui->leM58, 256, FLOAT);
    addNewGeneralBind(tr("X17"), ui->leX17, 258, FLOAT);
    addNewGeneralBind(tr("M60"), ui->leM60, 260, FLOAT);
    addNewGeneralBind(tr("M61"), ui->leM61, 262, FLOAT);
    addNewGeneralBind(tr("M62"), ui->leM62, 264, FLOAT);
    addNewGeneralBind(tr("X18"), ui->leX18, 266, FLOAT);

    // инициализация виджетов внешних защит
    addNewGeneralBind(tr("M64"), ui->leM64, 268, FLOAT);
    addNewGeneralBind(tr("X19"), ui->leX19, 270, FLOAT);
    addNewGeneralBind(tr("M72"), ui->leM72, 272, FLOAT);
    addNewGeneralBind(tr("M74"), ui->leM74, 274, FLOAT);
    addNewGeneralBind(tr("M76"), ui->leM76, 276, FLOAT);

    // инициализация виджетов защит по температуре
    addNewGeneralBind(tr("M67"), ui->leM67, 278, FLOAT);
    addNewGeneralBind(tr("M69"), ui->leM69, 280, FLOAT);
    addNewGeneralBind(tr("X20"), ui->leX20, 282, FLOAT);
    addNewGeneralBind(tr("M68"), ui->leM68, 284, FLOAT);
    addNewGeneralBind(tr("M70"), ui->leM70, 286, FLOAT);
    addNewGeneralBind(tr("X21"), ui->leX21, 288, FLOAT);

    // инициализация виджетов уровневых защит
    addNewGeneralBind(tr("M78"), ui->leM78, 290, FLOAT);
    addNewGeneralBind(tr("M79"), ui->leM79, 292, FLOAT);

    // инициализация виджетов защит  БРУ
    addNewGeneralBind(tr("M96"), ui->leM96, 294, FLOAT);
    addNewGeneralBind(tr("M97"), ui->leM97, 296, FLOAT);
    addNewGeneralBind(tr("M98"), ui->leM98, 298, FLOAT);
    addNewGeneralBind(tr("M99"), ui->leM99, 300, FLOAT);

    // инициализация виджетов вакуумных защит
    addNewGeneralBind(tr("M91"), ui->leM91, 302, FLOAT);
    addNewGeneralBind(tr("M92"), ui->leM92, 304, FLOAT);
    addNewGeneralBind(tr("X23"), ui->leX23, 306, FLOAT);

    // инициализация виджетов автоматики
    addNewGeneralBind(tr("M82"), ui->leM82, 308, FLOAT);
    addNewGeneralBind(tr("M83"), ui->leM83, 310, FLOAT);
    addNewGeneralBind(tr("M84"), ui->leM84, 312, FLOAT);
    addNewGeneralBind(tr("M85"), ui->leM85, 314, FLOAT);
    addNewGeneralBind(tr("M88"), ui->leM88, 316, FLOAT);
    addNewGeneralBind(tr("M89"), ui->leM89, 318, FLOAT);

    // инициализация виджетов коммутационных аппаратов
    addNewGeneralBind(tr("K02"), ui->leK02, 320, FLOAT);
    addNewGeneralBind(tr("K04"), ui->leK04, 322, FLOAT);
    addNewGeneralBind(tr("K05"), ui->leK05, 324, FLOAT);
    addNewGeneralBind(tr("K09"), ui->leK09, 326, FLOAT);
    addNewGeneralBind(tr("K08"), ui->leK08, 328, FLOAT);
    addNewGeneralBind(tr("X22"), ui->leX22, 330, FLOAT);
    addNewGeneralBind(tr("K12"), nullptr, 332, FLOAT);
    addNewGeneralBind(tr("K19"), nullptr, 334, FLOAT);
    addNewGeneralBind(tr("K42"), ui->leK42, 336, FLOAT);
    addNewGeneralBind(tr("K46"), ui->leK46, 338, FLOAT);
    addNewGeneralBind(tr("K43"), ui->leK43, 340, FLOAT);
    addNewGeneralBind(tr("K47"), ui->leK47, 342, FLOAT);
    addNewGeneralBind(tr("K44"), ui->leK44, 344, FLOAT);
    addNewGeneralBind(tr("K48"), ui->leK48, 346, FLOAT);
    addNewGeneralBind(tr("K45"), ui->leK45, 348, FLOAT);
    addNewGeneralBind(tr("K49"), ui->leK49, 350, FLOAT);
    addNewGeneralBind(tr("T02"), ui->leT02, 352, FLOAT);
    addNewGeneralBind(tr("K10"), nullptr, 354, FLOAT);

    // инициализация виджетов аналоговые входы
    addNewGeneralBind(tr("M01"), ui->leM01, 356, FLOAT);
    addNewGeneralBind(tr("M02"), ui->leM02, 358, FLOAT);
    addNewGeneralBind(tr("M03"), ui->leM03, 360, FLOAT);
    addNewGeneralBind(tr("KIA"), ui->leKIA, 362, FLOAT);
    addNewGeneralBind(tr("KIB"), ui->leKIB, 364, FLOAT);
    addNewGeneralBind(tr("KIC"), ui->leKIC, 366, FLOAT);
    addNewGeneralBind(tr("K3I0"), ui->leK3I0, 368, FLOAT);
    addNewGeneralBind(tr("KUA"), ui->leKUA, 370, FLOAT);
    addNewGeneralBind(tr("KUB"), ui->leKUB, 372, FLOAT);
    addNewGeneralBind(tr("KUC"), ui->leKUC, 374, FLOAT);
    addNewGeneralBind(tr("K3U0"), ui->leK3U0, 376, FLOAT);
    addNewGeneralBind(tr("KUS"), ui->leKUS, 378, FLOAT);
    addNewGeneralBind(tr("KUAB"), ui->leKUAB, 380, FLOAT);
    addNewGeneralBind(tr("KUBC"), ui->leKUBC, 382, FLOAT);
    addNewGeneralBind(tr("KUCA"), ui->leKUCA, 384, FLOAT);
    addNewGeneralBind(tr("K3U0X"), ui->leK3U0X, 386, FLOAT);
    addNewGeneralBind(tr("KUAX"), ui->leKUAX, 388, FLOAT);
    addNewGeneralBind(tr("KUBX"), ui->leKUBX, 390, FLOAT);
    addNewGeneralBind(tr("KUCX"), ui->leKUCX, 392, FLOAT);
    addNewGeneralBind(tr("KU0X"), ui->leKU0X, 394, FLOAT);
    addNewGeneralBind(tr("KU0X_"), ui->leKU0X_, 396, FLOAT);
}
//----------------------------------------
void ConfiguratorWindow::initPurposeBind()
{
    addNewPurposeBind(tr("DO1"), 1024);
    addNewPurposeBind(tr("DO2"), 1048);
    addNewPurposeBind(tr("DO4"), 1072);
    addNewPurposeBind(tr("DO5"), 1096);
    addNewPurposeBind(tr("DO6"), 1120);
    addNewPurposeBind(tr("DO7"), 1144);
    addNewPurposeBind(tr("DO8"), 1168);
    addNewPurposeBind(tr("DO9"), 1192);
    addNewPurposeBind(tr("DO10"), 1216);
    addNewPurposeBind(tr("DO11"), 1240);
    addNewPurposeBind(tr("DO12"), 1264);
    addNewPurposeBind(tr("DO13"), 1288);
    addNewPurposeBind(tr("LED1"), 1312);
    addNewPurposeBind(tr("LED2"), 1336);
    addNewPurposeBind(tr("LED3"), 1360);
    addNewPurposeBind(tr("LED4"), 1384);
    addNewPurposeBind(tr("LED5"), 1408);
    addNewPurposeBind(tr("LED6"), 1432);
    addNewPurposeBind(tr("LED7"), 1456);
    addNewPurposeBind(tr("LED8"), 1480);
    addNewPurposeBind(tr("K10"), 1504);
    addNewPurposeBind(tr("K11"), 1528);
    addNewPurposeBind(tr("I49"), 1552);
    addNewPurposeBind(tr("M86"), 1576);
    addNewPurposeBind(tr("M80"), 1600);
    addNewPurposeBind(tr("N27"), 1624);
    addNewPurposeBind(tr("N28"), 1648);
    addNewPurposeBind(tr("N29"), 1672);
    addNewPurposeBind(tr("N30"), 1696);
    addNewPurposeBind(tr("N31"), 1720);
    addNewPurposeBind(tr("N32"), 1744);
    addNewPurposeBind(tr("N33"), 1768);
    addNewPurposeBind(tr("N34"), 1792);
    addNewPurposeBind(tr("N35"), 1816);
    addNewPurposeBind(tr("N36"), 1840);
    addNewPurposeBind(tr("N37"), 1864);
    addNewPurposeBind(tr("N38"), 1888);
    addNewPurposeBind(tr("N39"), 1912);
    addNewPurposeBind(tr("N40"), 1936);
    addNewPurposeBind(tr("N41"), 1960);
    addNewPurposeBind(tr("N42"), 1984);
    addNewPurposeBind(tr("N43"), 2008);
    addNewPurposeBind(tr("N44"), 2032);
    addNewPurposeBind(tr("N45"), 2056);
    addNewPurposeBind(tr("N46"), 2080);
    addNewPurposeBind(tr("N49"), 2104);
    addNewPurposeBind(tr("N68"), 2128);
    addNewPurposeBind(tr("N69"), 2152);
    addNewPurposeBind(tr("N70"), 2176);
    addNewPurposeBind(tr("N72"), 2200);
}
//----------------------------------------
void ConfiguratorWindow::initModelTables()
{
    // инициализация таблицы привязок светодиодов
    QVector<CRow> led_rows;

    led_rows.append(CRow(tr("Светодиод 1"), 3));
    led_rows.append(CRow(tr("Светодиод 2"), 3));
    led_rows.append(CRow(tr("Светодиод 3"), 3));
    led_rows.append(CRow(tr("Светодиод 4"), 3));
    led_rows.append(CRow(tr("Светодиод 5"), 3));
    led_rows.append(CRow(tr("Светодиод 6"), 3));
    led_rows.append(CRow(tr("Светодиод 7"), 3));
    led_rows.append(CRow(tr("Светодиод 8"), 3));

    QStringList led_columns = QStringList() << tr("Переменная 1") << tr("Переменная 2") << tr("Переменная 3");

    CDataTable led_output_table(led_rows, led_columns);

    led_output_table.setDisableColumns(0, QVector<int>() << 1);
    led_output_table.setDisableColumns(1, QVector<int>() << 0 << 2);

    initTable(ui->tablewgtLedPurpose, led_output_table);

    // инициализация таблицы привязок входов
    QVector<CRow> input_rows;

    input_rows.append(CRow(tr("Вход 1"), 3));
    input_rows.append(CRow(tr("Вход 2"), 3));
    input_rows.append(CRow(tr("Вход 3"), 3));
    input_rows.append(CRow(tr("Вход 4"), 3));
    input_rows.append(CRow(tr("Вход 5"), 3));
    input_rows.append(CRow(tr("Вход 6"), 3));
    input_rows.append(CRow(tr("Вход 7"), 3));
    input_rows.append(CRow(tr("Вход 8"), 3));
    input_rows.append(CRow(tr("Вход 9"), 3));
    input_rows.append(CRow(tr("Вход 10"), 3));
    input_rows.append(CRow(tr("Вход 11"), 3));
    input_rows.append(CRow(tr("Вход 12"), 3));
    input_rows.append(CRow(tr("Вход 13"), 3));
    input_rows.append(CRow(tr("Вход 14"), 3));
    input_rows.append(CRow(tr("Вход 15"), 3));
    input_rows.append(CRow(tr("Вход 16"), 3));
    input_rows.append(CRow(tr("Вход 17"), 3));
    input_rows.append(CRow(tr("Вход 18"), 3));
    input_rows.append(CRow(tr("Вход 19"), 3));
    input_rows.append(CRow(tr("Вход 20"), 3));

    QStringList input_columns = QStringList() << tr("Переменная 1") << tr("Переменная 2") << tr("Переменная 3");

    CDataTable input_table(input_rows, input_columns);

    initTable(ui->tablewgtDiscreteInputPurpose, input_table);

    // инициализация таблицы привязок реле
    QStringList out_columns;

    QFile file(":/files/resource/files/variables.txt");

    if(!file.open(QIODevice::ReadOnly))
    {
        statusBar()->showMessage(tr("Не обнаружен файл с именами переменных"));
        exit(1);
    }

    QTextStream in(&file);

    while(!in.atEnd())
        out_columns << in.readLine();

    QVector<CRow> out_rows = QVector<CRow>() << CRow(tr("Реле 1"), out_columns.count()) << CRow(tr("Реле 2"), out_columns.count())
                                             << CRow(tr("Реле 4"), out_columns.count()) << CRow(tr("Реле 5"), out_columns.count())
                                             << CRow(tr("Реле 6"), out_columns.count()) << CRow(tr("Реле 7"), out_columns.count())
                                             << CRow(tr("Реле 8"), out_columns.count()) << CRow(tr("Реле 9"), out_columns.count())
                                             << CRow(tr("Реле 10"), out_columns.count()) << CRow(tr("Реле 11"), out_columns.count())
                                             << CRow(tr("Реле 12"), out_columns.count()) << CRow(tr("Реле 13"), out_columns.count());

    CDataTable out_relay_table(out_rows, out_columns);

    initTable(ui->tablewgtRelayPurpose, out_relay_table);
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
//-----------------------------------------------------------
void ConfiguratorWindow::displayGeneralResponse(CDataUnitType& unit)
{
    if(unit.is_empty())
        return;

    QString first_key  = unit.property("FIRST").toString();
    QString second_key = unit.property("LAST").toString();

    QVector<QWidget*> list = listWidget(first_key, second_key);

    if(list.isEmpty())
        return;

    int index = 0;

    union
    {
        quint16 b[2];
        float   f;
        int     i;
    } value;

    for(QWidget* wgt: list)
    {
        if(index >= unit.valueCount())
            break;

        if(!wgt)
        {
            index += 2;
            continue;
        }

        WidgetType type = (WidgetType)wgt->property("TYPE").toInt();

        if(type != LIST)
        {
            QLineEdit* edit = qobject_cast<QLineEdit*>(wgt);

            if(edit)
            {
                quint16 val1 = unit.values().at(index + 1);
                quint16 val2 = unit.values().at(index);

                value.b[0] = val1;
                value.b[1] = val2;

                QString str = "";

                if(type == INT)
                    str = QString::number(value.i);
                else if(type == FLOAT)
                    str = QString::number(value.f, 'f', 6);

                if(!str.isEmpty())
                    edit->setText(str);

                index += 2;
            }
        }
        else if(type == LIST)
        {
            QComboBox* box = qobject_cast<QComboBox*>(wgt);

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

    int size = m_purpose_list[indexes.y()].second - m_purpose_list[indexes.x()].second + 24;

    if(size != unit.valueCount())
        return;

    QTableView* table = nullptr;

    if(indexes.x() >= 0 && indexes.y() <= 11) // выходы: реле
        table = ui->tablewgtRelayPurpose;
    else if(indexes.x() >= 12 && indexes.y() <= 19) // выходы: светодиоды
        table = ui->tablewgtLedPurpose;

    if(!table)
        return;

    CMatrixPurposeModel* model = static_cast<CMatrixPurposeModel*>(table->model());

    if(!model)
        return;

    CDataTable& data = model->dataTable();

    for(int i = 0; i < (indexes.y() - indexes.x() + 1); i++)
    {
        for(int j = i*24, m = 0; j < i*24 + 24; j += 2, m++)
        {
            quint32 value = unit.value(j + 1) | ((unit.value(j) << 16));

            for(int k = 0; k < (int)sizeof(value)*8; k++)
            {
                bool state = (value >> k)&0x00000001;

                if(m == 11 && k >= 6)
                    break;

                data[i][m*32 + k].setState(state);
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
//-----------------------------------------------------------------------------------
void ConfiguratorWindow::addNewGeneralBind(const QString& key, QWidget* widget, int address,
                                    ConfiguratorWindow::WidgetType wtype)
{
    if(key.isEmpty() || !widget)
        return;

    widget->setProperty("ADDRESS", address);
    widget->setProperty("TYPE", wtype);

    m_cell_list.append(qMakePair(key, widget));
}
//-------------------------------------------------------------------------
void ConfiguratorWindow::addNewPurposeBind(const QString& key, int address)
{
    m_purpose_list.append(qMakePair(key, address));
}
//------------------------------------------------------------------------------------
int ConfiguratorWindow::sizeBindBlock(const QString& first_key, const QString& second_key)
{
    int iFirst = -1, iSecond = -1;

    for(quint8 i = 0; i < m_cell_list.count(); i++)
    {
        QPair<QString, QWidget*> pair = m_cell_list.at(i);

        if(pair.first == first_key)
            iFirst = i;

        if(pair.first == second_key)
            iSecond = i;
    }

    if(iFirst != -1 && iSecond != -1 && (iSecond >= iFirst))
        return ((iSecond - iFirst) + 1);

    return -1;
}
//----------------------------------------------------------------------------------------
void ConfiguratorWindow::sendGeneralReadRequest(const QString& first, const QString& last,
                                                CDataUnitType::FunctionType type, int size)
{
    if(size <= 0)
        return;

    int addr = addressGeneralKey(first);

    CDataUnitType unit(ui->sboxSlaveID->value(), type, addr, QVector<quint16>() << size);

    unit.setProperty(tr("REQUEST"), GENERAL_TYPE);
    unit.setProperty(tr("FIRST"), first);
    unit.setProperty(tr("LAST"), last);

    m_modbusDevice->request(unit);
}
//-----------------------------------------------------------------------------------------
void ConfiguratorWindow::sendGeneralWriteRequest(const QString& first, const QString& last)
{
    if(first.isEmpty() || last.isEmpty())
        return;

    int addr = addressGeneralKey(first);

    if(addr == -1)
        return;

    union
    {
        quint16 b[2];
        float f;
        int i;
    } value;

    QVector<quint16>  data;
    QVector<QWidget*> list = listWidget(first, last);

    if(list.isEmpty())
        return;

    for(QWidget* wgt: list)
    {
        if(!wgt)
        {
            data.append(0.0f);
            data.append(0.0f);

            continue;
        }

        WidgetType type = (WidgetType)wgt->property("TYPE").toInt();

        if(type != LIST)
        {
            QLineEdit* edit = qobject_cast<QLineEdit*>(wgt);

            if(!edit)
                continue;

            if(type == INT)
                value.i = edit->text().toInt();
            else if(type == FLOAT)
                value.f = edit->text().toFloat();

            data.append(value.b[1]);
            data.append(value.b[0]);
        }
        else if(type == LIST)
        {
            QComboBox* box = qobject_cast<QComboBox*>(wgt);

            if(!box)
                continue;

            data.append((quint16)box->currentIndex());
        }
    }

    CDataUnitType::FunctionType funType = ((data.count() == 1)?CDataUnitType::WriteSingleRegister:
                                                               CDataUnitType::WriteMultipleRegisters);

    CDataUnitType unit(ui->sboxSlaveID->value(), funType, addr, data);

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
//----------------------------------------------------------
int ConfiguratorWindow::addressGeneralKey(const QString& key) const
{
    for(QPair<QString, QWidget*> pair: m_cell_list)
    {
        if(pair.first == key)
        {
            if(pair.second)
            {
                return pair.second->property("ADDRESS").toInt();
            }
        }
    }

    return -1;
}
//-----------------------------------------------------------------
int ConfiguratorWindow::addressPurposeKey(const QString& key) const
{
    for(QPair<QString, int> pair: m_purpose_list)
    {
        if(pair.first == key)
        {
            return pair.second;
        }
    }

    return -1;
}
//-----------------------------------------------------------------------------------------
QVector<QWidget*> ConfiguratorWindow::listWidget(const QString& first, const QString& last)
{
    QVector<QWidget*> list;

    bool flag_key = false;

    for(int i = 0; i < m_cell_list.count(); i++)
    {
        if(!flag_key && m_cell_list.at(i).first == first)
        {
            list.append(m_cell_list.at(i).second);
            flag_key = true;
        }
        else if(flag_key && m_cell_list.at(i).first == last)
        {
            list.append(m_cell_list.at(i).second);
            break;
        }
        else if(flag_key)
        {
            list.append(m_cell_list.at(i).second);
        }
    }

    return list;
}
//-----------------------------------------------------------------------------------
QPoint ConfiguratorWindow::indexPurposeKey(const QString& first, const QString& last)
{
    QPoint  indexes(-1, -1);
    QString key = first;

    for(int i = 0; i < m_purpose_list.count(); i++)
    {
        if(m_purpose_list[i].first == key)
        {
            if(indexes.x() == -1)
            {
                indexes.setX(i);
                key = last;
            }
            else
            {
                indexes.setY(i);
                break;
            }
        }
    }

    return indexes;
}
//------------------------------------
void ConfiguratorWindow::initConnect()
{
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
