#include "configuratorwindow.h"
#include "ui_configuratorwindow.h"
#include "ui_configuratorwindow.h"
//------------------------------------------------------
ConfiguratorWindow::ConfiguratorWindow(QWidget* parent):
    QMainWindow(parent),
    ui(new Ui::ConfiguratorWindow),
    m_modbusDevice(nullptr),
    m_serialPortSettings(nullptr),
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
    m_versionWidget(nullptr),
    m_journal_parameters( { -1, 0, 0, 0, 0 } ),
    m_variables(QVector<CColumn::column_t>()),
    m_status_bar(nullptr),
    m_watcher(nullptr),
    m_progressbar(nullptr),
    m_settings(nullptr),
    m_active_journal_current(nullptr),
    m_journal_read_current(nullptr)
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
    m_serialPortSettings        = new CSerialPortSetting;
    m_status_bar                = new CStatusBar(statusBar());
    m_watcher                   = new QFutureWatcher<void>(this);
    m_progressbar               = new CProgressBarWidget(this);
    m_settings                  = new QSettings(QSettings::IniFormat, QSettings::UserScope, ORGANIZATION_NAME, "configurator",
                                                this);
    
    m_calculateWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    m_calculateWidget->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
    m_calculateWidget->setWindowTitle(tr("Расчетные величины"));
    addDockWidget(Qt::RightDockWidgetArea, m_calculateWidget);

    m_status_bar->addWidget(m_progressbar);
    statusBar()->addPermanentWidget(m_status_bar, 100);

    initJournals();
    initMenuPanel();
    initButtonGroup();
    initConnect();
    initCellBind(); // инициализация привязки настроек к адресам
    initPurposeBind(); // инициализация привязки "матрицы привязок выходов" к адресам
    initModelTables();
    initEventJournal(); // инициализация списка событий журнала
    initDeviceCode(); // инициализация списка кодов устройств

    if(!m_logFile->open(QFile::ReadWrite))
    {
        m_status_bar->setStatusMessage(tr("Ошибка. Невозможно открыть log-файл"));
    }
    else
        m_logFile->write(tr("Запуск программы...").toStdString().c_str());

    refreshSerialPort();
}
//---------------------------------------
ConfiguratorWindow::~ConfiguratorWindow()
{
    saveSattings();

    if(m_watcher->isRunning())
    {
        m_watcher->cancel();
        m_watcher->waitForFinished();
    }

    if(m_logFile)
    {
        if(m_logFile->isOpen())
        {
            m_logFile->write("Завершение работы программы...\n\n");
            m_logFile->close();
        }
    }

    if(m_system_db.isOpen())
        m_system_db.close();

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
        
    m_status_bar->clearStatusMessage();
    
    if(ui->pbtnPortCtrl->isChecked())
    {
        m_modbusDevice->setPortName(ui->cboxPortName->currentText());
        m_modbusDevice->setBaudrate(ui->cboxBaudrate->currentText().toInt());
        m_modbusDevice->setDatabits((QSerialPort::DataBits)m_serialPortSettings->dataBits().toInt());

        quint32 stopbits = ((m_serialPortSettings->stopBits() == "1.5")?3:m_serialPortSettings->stopBits().toInt());
        quint32 parity   = ((m_serialPortSettings->parity().toUpper() == tr("NO"))?0:
                           (m_serialPortSettings->parity().toUpper() == tr("EVEN"))?2:
                           (m_serialPortSettings->parity().toUpper() == tr("ODD"))?3:
                           (m_serialPortSettings->parity() == tr("SPACE")))?4:5;
        
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
    m_status_bar->setStatusMessage(((state)?tr("Соединение с устройством установлено"):
                                      tr("Соединение с устройством разорвано")), 5000);
    
    if(ui->checkboxCalibTimeout->isChecked() && state)
        chboxCalculateTimeoutStateChanged(true);
    else
        m_tim_calculate->stop();

    if(state)
    {
        saveLog(tr("Порт <") + m_modbusDevice->portName() + tr("> открыт."));
        updateParameterJournal();
        m_status_bar->clearSerialNumber(); // удаляем старый серийный номер
        deviceSync(true);
    }
    else
    {
        saveLog(tr("Порт <") + m_modbusDevice->portName() + tr("> закрыт."));
        deviceSync();
        m_status_bar->connectStateChanged(false);
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
//-------------------------------------------
void ConfiguratorWindow::serialPortSettings()
{
    m_serialPortSettings->show();
}
//--------------------------------------
void ConfiguratorWindow::calculateRead()
{
    CDataUnitType unit(ui->sboxSlaveID->value(), CDataUnitType::ReadInputRegisters,
                       CALCULATE_ADDRESS, QVector<quint16>() << 110);
    unit.setProperty(tr("REQUEST"), CALCULATE_TYPE);

    m_modbusDevice->request(unit);
}
//------------------------------------
void ConfiguratorWindow::journalRead()
{
    if(m_journal_parameters.start != -1 && m_journal_parameters.count == m_journal_parameters.read)
    {
        // если это не первый вызов метода чтения и количество прочитанных сообщений равно общему числу сообщений,
        // которое необходимо получить, то значит все события вычитаны и мы обнуляем счетчики
        m_journal_parameters.start = -1;
        m_journal_parameters.read  = 0;
        m_journal_parameters.count = 0;
        m_journal_parameters.shift = 0;

        m_journal_read_current->header()->setTextElapsedTime(m_time_process.elapsed());
        m_journal_read_current->header()->setTextTableCountMessages(m_journal_read_current->table()->rowCount());
//        m_journal_read_current->table()->sortByColumn(1, Qt::AscendingOrder);
//        m_journal_read_current->table()->setSortingEnabled(true);

        if(m_journal_read_current)
            emit m_journal_read_current->header()->stateButtonReadOff();

        m_journal_read_current = nullptr;

        updateParameterJournal();

        return;
    }

    if(m_journal_parameters.start == -1) // первый вызов - инициализация переменных
    {
        if(!m_active_journal_current) // если активынй журнал не выбран, значит ошибка
        {
            QMessageBox::critical(this, tr("Чтение журнала"), tr("Не выбран текущий активный журнал.\nПоробуйте еще раз."));
            return;
        }

        m_journal_read_current = m_active_journal_current; // устанавливаем текущий активный журнал текущим журналом чтения

        QString journal_type = m_journal_read_current->property("TYPE").toString();

        if(!m_journal_read_current || journal_type.isEmpty())
            return;

        if(m_journal_read_current->table()->rowCount() > 0)
            clearJournal();

        m_journal_read_current->header()->setTextDeviceCountMessages(0, m_journal_parameters.total);

        m_time_process.start();

        CFilter filter;

        if(m_filter.find(journal_type) != m_filter.end())
        {
            filter = m_filter[journal_type];
        }

        if(filter) // вкладка с выбором интервала активана
        {
            if(filter.type() == CFilter::INTERVAL) // выбран режим счтитывания по интервалу
            {
                m_journal_parameters.start = filter.interval().begin;
                m_journal_parameters.count = filter.interval().end - filter.interval().begin;
            }
//            else if(filter.type() == CFilter::DATE) // выбран режим считывания по календарю
//            {

//            }

            if(m_journal_parameters.start >= 256) // если начальная точка больше или равна размеру сектора (256*16=4096)
            {
                int part = m_journal_parameters.start/256; // получаем номер текущего сектора
                m_journal_parameters.shift = part*4096; // сохраняем новое значение указателя на текущий сектор
            }

            setJournalPtrShift(); // вызываем метод перевода сектора

            m_journal_parameters.start %= 256; // получаем остаток для сохранения текущего события
        }
        else
        {
            m_journal_parameters.start = 0;
            m_journal_parameters.count = m_journal_parameters.total;

            if(m_journal_parameters.shift != 0)
            {
                m_journal_parameters.shift = 0;

                setJournalPtrShift();
            }
        }
    }

    if(m_journal_parameters.start == 256) // прочитали до конца очередного сектора - переводим указатель
    {
        m_journal_parameters.shift += 4096;
        m_journal_parameters.start  = 0;

        setJournalPtrShift();
    }

    int addr = m_journal_parameters.start*8 + 4096; // 8 - количество ячеек на событие, т.е. размер события 16 байт
                                                         // 4096 смещение регистра для получения начальной страницы событий
    int var_count = (((m_journal_parameters.count - m_journal_parameters.read) >= 8)?64:
                      (m_journal_parameters.count - m_journal_parameters.read)*8); // 64/8 = 8 событий в запросе
                                                         // если осталось больше или равно 8ми событий иначе считаем количество
                                                         // событий из разницы общего их количества и прочитанного

    CDataUnitType unit(ui->sboxSlaveID->value(), CDataUnitType::ReadInputRegisters, addr, QVector<quint16>() << var_count);
    unit.setProperty(tr("REQUEST"), READ_EVENT_JOURNAL);

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
//------------------------------------------------------------
void ConfiguratorWindow::processReadJournalEvent(bool checked)
{
    if(!checked)
    {
        m_journal_parameters.start = -1;
        m_journal_parameters.read  = 0;
        m_journal_parameters.count = 0;
        m_journal_parameters.shift = 0;

        if(m_journal_read_current)
            emit m_journal_read_current->header()->stateButtonReadOff();

        m_journal_read_current = nullptr;

        return;
    }

    journalRead();
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

    emit m_modbusDevice->infoLog(tr("Получен ответ: ") + QString::number(unit.valueCount()) + tr(" байт \n"));

    quint16 error = unit.error();

    if(error != CDataUnitType::NO_DEVICE_ERROR) // если ошибка, то выводим ее
    {
        QMessageBox::warning(this, tr("Ответ устройства"),
                             tr("В ответе обнаружена ошибка.\n") + unit.errorStringList());
    }
    
    RequestType type = (RequestType)unit.property(tr("REQUEST")).toInt();

    if(type == CALCULATE_TYPE)
        displayCalculateValues(unit.values());
    else if(type == GENERAL_TYPE)
        displaySettingResponse(unit);
    else if(type == PURPOSE_OUT_TYPE)
        displayPurposeResponse(unit);
    else if(type == PURPOSE_INPUT_TYPE)
        displayPurposeDIResponse(unit);
    else if(type == READ_EVENT_JOURNAL || type == READ_EVENT_COUNT || type == READ_EVENT_SHIFT_PTR)
        processReadJournal(unit);
    else if(type == READ_SERIAL_NUMBER)
        displayDeviceSerialNumber(unit.values());
}
//------------------------------------
void ConfiguratorWindow::exitFromApp()
{
    close();
}
//-----------------------------
void ConfiguratorWindow::show()
{
    QMainWindow::show();

    setWindowState(Qt::WindowMaximized);
    
    m_terminal->hide();
    ui->stwgtMain->setCurrentIndex(0);

    m_versionWidget = new CVersionSoftware(this);
    versionParser();

    ui->tabwgtMenu->setCurrentIndex(3);
    m_status_bar->connectStateChanged(false);

    ui->tabwgtMenu->setTabEnabled(4, false);

    loadSettings();
}
//-------------------------------------------------------
void ConfiguratorWindow::resizeEvent(QResizeEvent* event)
{
    QMainWindow::resizeEvent(event);
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
    m_status_bar->setStatusMessage(error, 5000);
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

//    if(ui->treewgtDeviceMenu->indexOfTopLevelItem(item) == 1) // если выбран пункт меню "Журналы"
//    {
//        ui->tabwgtMenu->setTabEnabled(4, true);
//    }
//    else
//        ui->tabwgtMenu->setTabEnabled(4, false);

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
    else if(itemName == tr("ПОЛУЧАСОВОК"))
    {
        ui->stwgtMain->setCurrentIndex(16);
    }
    else if(itemName == tr("ИЗОЛЯЦИИ"))
    {
        ui->stwgtMain->setCurrentIndex(17);
    }
    else if(itemName == tr("ОСЦИЛЛОГРАФ"))
    {
        ui->stwgtMain->setCurrentIndex(18);
    }
    else if(itemName == tr("ПЕРВИЧНЫЕ ВЕЛИЧИНЫ"))
    {
        ui->stwgtMain->setCurrentIndex(19);
    }
    else if(itemName == tr("ВТОРИЧНЫЕ ВЕЛИЧИНЫ"))
    {
        ui->stwgtMain->setCurrentIndex(20);
    }
    else if(itemName == tr("ЭЛЕКТРОЭНЕРГИЯ"))
    {
        ui->stwgtMain->setCurrentIndex(21);
    }
    else if(itemName == tr("ДИСКРЕТНЫЕ ВХОДЫ"))
    {
        ui->stwgtMain->setCurrentIndex(22);
    }
    else if(itemName == tr("ДИСКРЕТНЫЕ ВЫХОДЫ"))
    {
        ui->stwgtMain->setCurrentIndex(23);
    }
    else if(itemName == tr("НАЗНАЧЕНИЕ СВЕТОДИОДОВ"))
    {
        ui->stwgtMain->setCurrentIndex(24);
    }
    else if(itemName == tr("НАЗНАЧЕНИЕ ДИСКРЕТНЫХ ВХОДОВ"))
    {
        ui->stwgtMain->setCurrentIndex(25);
    }
    else if(itemName == tr("НАЗНАЧЕНИЕ РЕЛЕ"))
    {
        ui->stwgtMain->setCurrentIndex(26);
    }
    else if(itemName == tr("НАЗНАЧЕНИЕ КЛАВИАТУРЫ"))
    {
        ui->stwgtMain->setCurrentIndex(27);
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

        case 15: // чтение журнала событий
//            eventJournalRead();
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
            sendPurposeDIReadRequest(512, 590);
            sendPurposeDIReadRequest(592, 670);
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

        case 22: // привязки выходов (светодиодов)
            sendPurposeWriteRequest(tr("LED1"), tr("LED2"));
            sendPurposeWriteRequest(tr("LED3"), tr("LED4"));
            sendPurposeWriteRequest(tr("LED5"), tr("LED6"));
            sendPurposeWriteRequest(tr("LED7"), tr("LED8"));
        break;

        case 23: // привязки входов
            sendPurposeDIWriteRequest(512, 590);
            sendPurposeDIWriteRequest(592, 670);
        break;

        case 24: // привязки выходов (реле)
            sendPurposeWriteRequest(tr("DO1"), tr("DO2"));
            sendPurposeWriteRequest(tr("DO4"), tr("DO5"));
            sendPurposeWriteRequest(tr("DO6"), tr("DO7"));
            sendPurposeWriteRequest(tr("DO8"), tr("DO9"));
            sendPurposeWriteRequest(tr("DO10"), tr("DO11"));
            sendPurposeWriteRequest(tr("DO12"), tr("DO13"));
        break;

        case 25: // привязки выходов (клавиатуры)
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

    itemJournalCrashs    = new QTreeWidgetItem(itemJournals);
    itemJournalEvents    = new QTreeWidgetItem(itemJournals);
    itemJournalHalfHour  = new QTreeWidgetItem(itemJournals);
    itemJournalIsolation = new QTreeWidgetItem(itemJournals);
    itemJournalOscill    = new QTreeWidgetItem(itemJournals);

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
    itemJournalHalfHour->setText(0, tr("Получасовок"));
    itemJournalIsolation->setText(0, tr("Изоляции"));
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
    itemJournals->addChild(itemJournalHalfHour);
    itemJournals->addChild(itemJournalIsolation);
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
    QSqlQuery query(m_system_db);

    if(!query.exec("SELECT * FROM iodevice WHERE type = 'SET';"))
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
    if(!m_system_db.isOpen())
        return;

    QSqlQuery query(m_system_db);

    QStringList type_list;
    type_list << tr("INPUT") << tr("RELAY") << tr("LED");

    for(const QString& type: type_list)
    {
        if(query.exec(tr("SELECT key, address, description FROM iodevice WHERE type = '") + type + "';"))
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
    QSqlQuery query(m_system_db);

    if(query.exec("SELECT * FROM variable;"))
    {
        while(query.next())
        {
            CColumn::column_t column = qMakePair(query.value(tr("key")).toString(),
                                                 qMakePair(query.value(tr("name")).toString(),
                                                           query.value(tr("description")).toString()));
            m_variables << column;
        }
    }

    if(m_variables.isEmpty())
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

        QStringList columnList;

        for(CColumn::column_t& column: m_variables)
        {
            columnList << column.first;
        }

        for(int i = index.x(); i <= index.y(); i++)
        {
            QVector<int> indexes = indexVariableFromKey(columnList, m_purpose_list[i].first);
            CRow row(m_purpose_list[i].first, m_purpose_list[i].second.second.first, m_variables.count());

            row.setActiveColumnList(indexes);
            rows.append(row);
        }

        if(rows.isEmpty())
            return;

        CDataTable data(rows, m_variables);

        initTable(wgt, data);
    }
}
//-----------------------------------------
void ConfiguratorWindow::initEventJournal()
{
    // инициализация списка журнала событий
    QSqlQuery query(m_system_db);

    if(!query.exec("SELECT code, name FROM event_t;"))
    {
        qDebug() << "Ошибка чтения типов журнала событий: " << query.lastError().text();
        return;
    }

    while(query.next())
    {
        m_event_list << event_t({ query.value("code").toInt(), query.value("name").toString(), QVector<event_t>() });
    }

    if(m_event_list.isEmpty())
        return;

    for(event_t& event: m_event_list)
    {
        if(!query.exec("SELECT code, name FROM event_category WHERE event_t = " + QString::number(event.code) + ";"))
        {
            qDebug() << "Ошибка чтения категорий журнала событий: " << query.lastError().text();
            return;
        }

        while(query.next())
        {
            event.sub_event << event_t({ query.value("code").toInt(), query.value("name").toString(), QVector<event_t>() });
        }
    }

    for(event_t& event: m_event_list)
    {
        if(event.sub_event.isEmpty())
            continue;

        for(event_t& category: event.sub_event)
        {
            if(!query.exec("SELECT code, name FROM event_parameter WHERE event_category = " +
                           QString::number(category.code) + ";"))
            {
                qDebug() << "Ошибка чтения параметров журнала событий: " << query.lastError().text();
                return;
            }

            while(query.next())
            {
                category.sub_event << event_t({ query.value("code").toInt(), query.value("name").toString(),
                                                QVector<event_t>() });
            }
        }
    }
}
//---------------------------------------
void ConfiguratorWindow::initDeviceCode()
{
    QSqlQuery query(m_system_db);

    if(!query.exec("SELECT code, name FROM device_code;"))
    {
        qDebug() << "Ошибка чтения кодов устройств: " << query.lastError().text();
        return;
    }

    while(query.next())
    {
        m_device_code_list[query.value("code").toInt()] = query.value("name").toString();
    }
}
//-------------------------------------
void ConfiguratorWindow::initJournals()
{
    QStringList journalHeaders = QStringList() << tr("ID") << tr("Дата") << tr("Время") << tr("Тип") << tr("Категория") <<
                                                  tr("Параметр");

    ui->widgetJournalCrash->setProperty("NAME", tr("аварий"));
    ui->widgetJournalEvent->setProperty("NAME", tr("событий"));
    ui->widgetJournalHalfHour->setProperty("NAME", tr("получасовок"));
    ui->widgetJournalIsolation->setProperty("NAME", tr("изоляций"));

    ui->widgetJournalCrash->setProperty("TYPE", tr("CRASH"));
    ui->widgetJournalEvent->setProperty("TYPE", tr("EVENT"));
    ui->widgetJournalHalfHour->setProperty("TYPE", tr("HALFHOUR"));
    ui->widgetJournalIsolation->setProperty("TYPE", tr("ISOLATION"));

    ui->widgetJournalCrash->setTableHeaders(journalHeaders);
    ui->widgetJournalEvent->setTableHeaders(journalHeaders);
    ui->widgetJournalHalfHour->setTableHeaders(journalHeaders);
    ui->widgetJournalIsolation->setTableHeaders(journalHeaders);

    QVector<int> length_list = QVector<int>() << 50 << 100 << 100 << 100 << 200 << 300;

    ui->widgetJournalCrash->setTableColumnWidth(length_list);
    ui->widgetJournalEvent->setTableColumnWidth(length_list);
    ui->widgetJournalHalfHour->setTableColumnWidth(length_list);
    ui->widgetJournalIsolation->setTableColumnWidth(length_list);
}
//----------------------------------------
void ConfiguratorWindow::connectSystemDb()
{
    m_system_db = QSqlDatabase::addDatabase("QSQLITE");
    m_system_db.setDatabaseName("db/system.db");

    if(!m_system_db.open())
    {
        QMessageBox::critical(this, tr("Системная база данных"), tr("Невозможно открыть системную базу данных: ") +
                              m_system_db.lastError().text());
        exit(1);
    }
}
//------------------------------------------------------------------------
bool ConfiguratorWindow::connectDb(QSqlDatabase*& db, const QString& path)
{
    db = new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE", "db"));
    db->setDatabaseName(path);

    if(!db->open())
    {
        QMessageBox::critical(this, tr("База данных журнала событий"), tr("Невозможно открыть базу данных журнала событий: ") +
                              db->lastError().text());
        disconnectDb(db);

        return false;
    }

    return true;
}
//-----------------------------------------------------
void ConfiguratorWindow::disconnectDb(QSqlDatabase* db)
{
    if(db->isOpen())
        db->close();

    delete db;

    QSqlDatabase::removeDatabase("db");
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

    QString first = unit.property(tr("FIRST")).toString(); // получаем первый ключ
    QString last  = unit.property(tr("LAST")).toString();  // получаем второй ключ

    QTableView* table = tableMatrixFromKeys(first, last); // поиск таблицы по ключам

    if(!table)
        return;

    CMatrixPurposeModel* model = static_cast<CMatrixPurposeModel*>(table->model());

    if(!model)
        return;

    CDataTable& data = model->dataTable(); // получаем ссылку на данные таблицы

    int var_count = data.columnCounts()/16; // получаем количество байт для хранения переменных

    if(data.columnCounts()%16) // если есть остаток, значит +1 байт для хранения всех переменных
        var_count++;           // например LED имеют 115 возможных привязок к переменным - 115/16 = 7,1875 -> 7 + 1 = 8 байт

    int offset_row = data.indexRowFromKey(first);

    if(offset_row == -1)
        return;

    int row_count = unit.valueCount()/24; // расчет количества строк из количества пришедших данных;
                                          // полная строка со всеми переменными равна 48 байт, т.е. 24 ячейки
                                          // привязка переменной равна 1 бит (нет/есть)
    int limit = var_count*16 - (var_count*16 - data.columnCounts()); // предел количества переменных - т.к. всего 48 байт, т.е.
                                                                     // максимальное число переменных - 384, а используется, н-р,
                                                                     // для LED всего 115, то все что свыше этого значения мы
                                                                     // игнорируем (резерв)

    for(int i = 0, offset_data = 0; i < row_count; i++, offset_data += 24 - var_count)
    {
        int row = i + offset_row; // получаем индекс текущей строки учитываю смещение, н-р: LED1 - смещение 0, и если прочитать
                                  // LED3, то получим смещение 2, т.е. третья строка таблицы

        for(int j = 0; j < var_count - 1; j += 2)
        {
            int index = i*var_count + offset_data + j; // получаем индекс текущего слова (32 бита)

            quint32 value = (unit.value(index + 1) << 16) | unit.value(index); // получаем значение (32 бита) с состояними 32х
                                                                               // переменных

            for(int k = 0; k < 32; k++)
            {
                int bit = j/2*32 + k;

                if(bit >= limit)
                    break;

                bool state = (value >> k)&0x00000001;

                data[row][bit].setState(state);
            }
        }
    }

    model->updateData();
}
//--------------------------------------------------------------------
void ConfiguratorWindow::displayPurposeDIResponse(CDataUnitType& unit)
{
    if(unit.is_empty())
        return;

    int first_addr = unit.property(tr("FIRST_ADDRESS")).toInt();
    int last_addr  = unit.property(tr("LAST_ADDRESS")).toInt();

    if((last_addr - first_addr + 2) != unit.valueCount())
        return;

    int column_offset = (first_addr - 512)/2;

    CMatrixPurposeModel* model = static_cast<CMatrixPurposeModel*>(ui->tablewgtDiscreteInputPurpose->model());
    CDataTable&          data  = model->dataTable();

    for(int i = 0; i < unit.valueCount() - 1; i += 2)
    {
        quint32 value = (unit.value(i + 1) << 16) | unit.value(i);

        for(int j = 0; j < data.count(); j++)
        {
            QVector<int> list = data.columnIndexListActive(j);

            bool state = (value >> j)&0x00000001;

            int column = list[i/2 + column_offset];

            data[j][column].setState(state);
        }
    }

    model->updateData();
}
//--------------------------------------------------------------------------------
void ConfiguratorWindow::displayJournalResponse(const QVector<quint16>& data_list)
{
    if(!m_journal_read_current)
    {
        return;
    }

    QVector<quint8> data;

    for(int i = 0; i < data_list.count(); i++)
    {
        data << (quint8)((data_list[i] >> 8)&0x00FF);
        data << (quint8)(data_list[i]&0x00FF);
    }

    QDir dir;

    if(!dir.exists("log"))
        dir.mkdir("log");

    QFile file1("log/displayJournalResponce.log");

    file1.open(QFile::WriteOnly);

    QTextStream out1(&file1);

    for(int i = 0; i < data.count(); i += 16)
    {
        quint16 id = ((data[i + 1] << 8) | data[i]);

        quint16 year = ((data[i + 2]&0xFC) >> 2);

        year += ((year < 2000)?2000:0); // приводит дату к 'yyyy'

        quint8 month = ((data[i + 2]&0x03) << 2) | ((data[i + 3]&0xC0) >> 6);
        quint8 day   = ((data[i + 3]&0x3E) >> 1);

        quint8 hour    = ((data[i + 3]&0x01) << 4) | ((data[i + 4]&0xF0) >> 4);
        quint8 minute  = ((data[i + 4]&0x0F) << 2) | ((data[i + 5]&0xC0) >> 6);
        quint8 second  = (data[i + 5]&0x3F);
        quint8 msecond = data[i + 6];

        quint8  type_event      = data[i + 7];
        quint8  category_event  = data[i + 8];
        quint16 parameter_event = data[i + 9] | data[i + 10] << 8;

        QDate d(year, month, day);
        QTime t(hour, minute, second);

        QVector<event_t> etype = ((!m_event_list.isEmpty())?m_event_list:QVector<event_t>());

        if(!etype.isEmpty())
        {
            QVector<event_t> ecategory  = QVector<event_t>();
            QVector<event_t> eparameter = QVector<event_t>();

            if(etype.count() > type_event)
                ecategory = etype[type_event].sub_event;

            if(ecategory.count() > category_event)
                eparameter = ecategory[category_event].sub_event;

            int row = m_journal_read_current->table()->rowCount();

            m_journal_read_current->table()->insertRow(row);

            QString etype_str = tr("Неизвестный тип");

            if(etype.count() > type_event)
                etype_str = etype[type_event].name;

            m_journal_read_current->table()->setItem(row, 0, new QTableWidgetItem(QString::number(id)));
            m_journal_read_current->table()->setItem(row, 1, new CTableWidgetItem(d.toString("dd.MM.yyyy")));

            QString s = ((msecond < 10)?"00":(msecond < 100 && msecond >= 10)?"0":"") + QString::number(msecond);

            m_journal_read_current->table()->setItem(row, 2, new QTableWidgetItem(t.toString("HH:mm:ss") + QString(":") + s));
            m_journal_read_current->table()->setItem(row, 3, new QTableWidgetItem(QTableWidgetItem(etype_str + QString(" (%1)").
                                                                                  arg(type_event))));

            QString ecategory_str  = (ecategory.isEmpty())?tr("Неизвестная категория"):ecategory[category_event].name;
            QString eparameter_str = ((eparameter.isEmpty() || (eparameter.count() <= parameter_event))?
                                       tr("Неизвестный параметр"):eparameter[parameter_event].name);

            m_journal_read_current->table()->setItem(row, 4, new QTableWidgetItem(ecategory_str +
                                                                               QString(" (%1)").arg(QString::number(category_event))));
            m_journal_read_current->table()->setItem(row, 5, new QTableWidgetItem(eparameter_str +
                                                                               QString(" (%1)").arg(QString::number(parameter_event))));

            m_journal_read_current->table()->item(row, 0)->setTextAlignment(Qt::AlignCenter);
            m_journal_read_current->table()->item(row, 1)->setTextAlignment(Qt::AlignCenter);
            m_journal_read_current->table()->item(row, 2)->setTextAlignment(Qt::AlignCenter);

            out1 << i << ": " << m_journal_read_current->table()->item(row, 0)->text() << "\t" <<
                                m_journal_read_current->table()->item(row, 1)->text() << "\t" <<
                                m_journal_read_current->table()->item(row, 2)->text() << "\t" <<
                                m_journal_read_current->table()->item(row, 3)->text() << "\t" <<
                                m_journal_read_current->table()->item(row, 4)->text() << "\t" <<
                                m_journal_read_current->table()->item(row, 5)->text() << "\n";

            if(m_journal_read_current->header()->stateCheckbox())
                m_journal_read_current->table()->scrollToBottom();
        }
    }

    m_journal_read_current->header()->setTextDeviceCountMessages(m_journal_parameters.read, m_journal_parameters.total);

    QFile file2("log/displayJournalResponce_parameters.log");
    file2.open(QFile::WriteOnly);
    QTextStream out2(&file2);

    out2 << "count = " << m_journal_parameters.count << ", read = " << m_journal_parameters.read << ", shift = " <<
                          m_journal_parameters.shift << ", start = " << m_journal_parameters.start << ", total = " <<
                          m_journal_parameters.total << ";\n";

    file2.close();
    file1.close();
}
//------------------------------------------------------------------------------
void ConfiguratorWindow::displayDeviceSerialNumber(const QVector<quint16>& data)
{
    if(data.count() == 8) // пришел серийный номер, иначе сообщение с ошибкой
    {
        m_status_bar->connectStateChanged(true); // обновляем состояние соединения с устройством

        quint16 dev_code     = data[0]; // код устройства
        quint32 sn_code      = (quint32)(data[1] << 16) | (data[2]); // порядковый номер
        quint16 party_num    = data[3]; // номер партии
        quint16 firmware_var = data[4]; // вариант прошивки
        quint8  year         = (quint8)(((data[5] >> 8)&0xFF)*10) + (quint8)(data[5]&0xFF); // год
        quint8  month        = (quint8)(((data[6] >> 8)&0xFF)*10) + (quint8)(data[6]&0xFF); // месяц
        quint8  day          = (quint8)(((data[7] >> 8)&0xFF)*10) + (quint8)(data[7]&0xFF); // день

        QString dev_code_str = m_device_code_list[dev_code];

        if(dev_code_str.isEmpty())
            dev_code_str = "0";

        QString str;
        QString date = "0";

        if(year != 0 && month != 0 && day != 0)
            date = QDate(year, month, day).toString("yy.MM.dd");

        str = QString("S/n: %1-%2-%3-%4-%5").arg(dev_code_str).
                                             arg(sn_code).
                                             arg(party_num).
                                             arg(firmware_var).
                                             arg(date);

        m_status_bar->setSerialNumber(str);
    }
}
//--------------------------------------
void ConfiguratorWindow::versionParser()
{
    QFile file(tr(":/files/resource/files/version.txt"));

    if(!file.open(QFile::ReadOnly))
    {
        m_status_bar->setStatusMessage(tr("Нет файла версии или он поврежден..."), 2000);
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

    for(int i = index.x(); i <= index.y(); i++)
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

    CDataUnitType unit(ui->sboxSlaveID->value(), funType, addressSettingKey(first), data);

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

    int size = laddr - faddr + 24; // получаем размер считываемого блока с учетом выравнивания в 48 байт (одна строка)

    CDataUnitType unit(ui->sboxSlaveID->value(), CDataUnitType::ReadHoldingRegisters, faddr,
                                                 QVector<quint16>() << size);

    unit.setProperty(tr("REQUEST"), PURPOSE_OUT_TYPE);
    unit.setProperty(tr("FIRST"), first);
    unit.setProperty(tr("LAST"), last);

    m_modbusDevice->request(unit);
}
//-----------------------------------------------------------------------------------------
void ConfiguratorWindow::sendPurposeWriteRequest(const QString& first, const QString& last)
{
    QTableView* table = tableMatrixFromKeys(first, last);

    if(!table)
        return;

    CDataTable data = static_cast<CMatrixPurposeModel*>(table->model())->dataTable();

    int bIndex = data.indexRowFromKey(first);
    int eIndex = data.indexRowFromKey(last);

    if(bIndex == -1 || eIndex == -1)
        return;

    QVector<quint16> values;

    int var_count = data.columnCounts()/16;

    if(data.columnCounts()%16)
        var_count++;

    for(int i = bIndex; i <= eIndex; i++)
    {
        for(int j = 0, offset = 0; j < 24 - 1; j += 2, offset += 32)
        {
            quint32 value = 0;

            for(int k = 0; k < 32; k++)
            {
                int bit = offset + k;

                if(bit >= var_count*16 - (var_count*16 - data.columnCounts()))
                    break;

                bool state = data[i][bit].state();

                if(state)
                    value |= 1 << k;
            }

            quint16 lbs = (quint16)value&0x0000FFFF;
            quint16 mbs = (quint16)((value >> 16)&0x0000FFFF);

            values << lbs << mbs;
        }
    }

    CDataUnitType::FunctionType funType = ((values.count() == 1)?CDataUnitType::WriteSingleRegister:
                                                                 CDataUnitType::WriteMultipleRegisters);

    CDataUnitType unit(ui->sboxSlaveID->value(), funType, addressPurposeKey(first), values);

    unit.setProperty(tr("FIRST"), first);
    unit.setProperty(tr("LAST"), last);

    m_modbusDevice->request(unit);
}
//------------------------------------------------------------------------------
void ConfiguratorWindow::sendPurposeDIReadRequest(int first_addr, int last_addr)
{
    int size = last_addr - first_addr + 2;

    CDataUnitType unit(ui->sboxSlaveID->value(), CDataUnitType::ReadHoldingRegisters, first_addr,
                                                 QVector<quint16>() << size);

    unit.setProperty(tr("REQUEST"), PURPOSE_INPUT_TYPE);
    unit.setProperty(tr("FIRST_ADDRESS"), first_addr);
    unit.setProperty(tr("LAST_ADDRESS"), last_addr);

    m_modbusDevice->request(unit);
}
//-------------------------------------------------------------------------------
void ConfiguratorWindow::sendPurposeDIWriteRequest(int first_addr, int last_addr)
{
    CMatrixPurposeModel* model = static_cast<CMatrixPurposeModel*>(ui->tablewgtDiscreteInputPurpose->model());

    if(!model)
        return;

    CDataTable   data        = model->dataTable();
    QVector<int> column_list = data.columnIndexListActive(0);

    int bIndex = (first_addr - 512)/2;
    int eIndex = (670 - last_addr)/2;

    QVector<quint16> values;

    for(int i = bIndex; i < column_list.count() - eIndex; i++)
    {
        quint32 value = 0;

        for(int j = 0; j < data.count(); j++)
        {
            value |= (data[j][column_list[i]].state()) << j;
        }

        values << (quint16)(value&0x0000FFFF) << (quint16)((value >> 16)&0x0000FF);
    }

    CDataUnitType::FunctionType funType = ((values.count() == 1)?CDataUnitType::WriteSingleRegister:
                                                                 CDataUnitType::WriteMultipleRegisters);

    CDataUnitType unit(ui->sboxSlaveID->value(), funType, first_addr, values);

    unit.setProperty(tr("FIRST_ADDRESS"), first_addr);
    unit.setProperty(tr("LAST_ADDRESS"), last_addr);

    m_modbusDevice->request(unit);
}
//-------------------------------------
void ConfiguratorWindow::clearIOTable()
{
    QTableView* table = nullptr;

    switch(ui->stwgtMain->currentIndex())
    {
        case 22:
            table = ui->tablewgtLedPurpose;
        break;

        case 23:
            table = ui->tablewgtDiscreteInputPurpose;
        break;

        case 24:
            table = ui->tablewgtRelayPurpose;
        break;
    }

    if(!table)
        return;

    CMatrixPurposeModel* model = static_cast<CMatrixPurposeModel*>(table->model());

    if(!model)
        return;

    CDataTable& data = model->dataTable();

    for(int i = 0; i < data.count(); i++)
    {
        for(int j = 0; j < data.columnCounts(); j++)
        {
            data[i][j].setState(false);
        }
    }

    model->updateData();
}
//-------------------------------------
void ConfiguratorWindow::clearJournal()
{
    if(!m_active_journal_current)
        return;

    QString journal_name = m_active_journal_current->property("NAME").toString();

    m_active_journal_current->tableClear();
    m_active_journal_current->headerClear();

    m_status_bar->setStatusMessage(tr("Очистка таблицы журнала %1").arg(journal_name), 2000);
}
//--------------------------------------
void ConfiguratorWindow::menuPanelCtrl()
{
    if(ui->dockwgtDeviceMenu->isHidden())
        ui->dockwgtDeviceMenu->show();
    else
        ui->dockwgtDeviceMenu->hide();
}
//------------------------------------------
void ConfiguratorWindow::variablePanelCtrl()
{
    if(m_calculateWidget->isHidden())
        m_calculateWidget->show();
    else
        m_calculateWidget->hide();
}
//-----------------------------------------
void ConfiguratorWindow::startExportToPDF()
{
    if(!m_active_journal_current)
    {
        QMessageBox::warning(this, tr("Эксорт в PDF"), tr("Выберите текущий журнал для экспорта"));
        return;
    }

    QString journal_name = m_active_journal_current->property("NAME").toString();
    QString sn_device    = "s/n: " + m_status_bar->serialNumberText();

    // выбираем файл для экспорта
    QDir dir;

    if(!dir.exists("reports"))
    {
        dir.mkdir("reports");
    }

    QString selectedFilter    = tr("PDF (*.pdf)");
    QString journal_full_name = tr("Журнал %1-%2").arg(journal_name).arg(m_status_bar->serialNumberText());
    QString journal_path      = QFileDialog::getSaveFileName(this, tr("Экспорт журнала %1 в PDF").arg(journal_name),
                                                             dir.absolutePath() + QString("/reports/%1.%2").
                                                             arg(journal_full_name).arg("pdf"),
                                                             tr("PDF (*.pdf);;Все файлы (*.*)"), &selectedFilter);

    if(journal_path.isEmpty())
        return;

    m_progressbar->setProgressTitle(tr("Экспорт в PDF"));
    m_progressbar->progressStart();

    connect(m_watcher, &QFutureWatcher<void>::finished, m_progressbar, &CProgressBarWidget::progressStop);

    QFuture<void> future = QtConcurrent::run(this, &exportToPDF, m_active_journal_current, journal_name, sn_device, journal_path);
    m_watcher->setFuture(future);
}
//-------------------------------------
void ConfiguratorWindow::filterDialog()
{
    if(!m_active_journal_current)
    {
        QMessageBox::warning(this, tr("Фильтр"), tr("Выберите текущий журнал"));
        return;
    }

    QString       key   = m_active_journal_current->property("TYPE").toString();
    QTableWidget* table = m_active_journal_current->table();

    if(key.isEmpty() || !table)
        return;

    if(m_filter.find(key) == m_filter.end()) // если запись уже присутствует
    {
        m_filter.take(key); // удаляем запись по ключу
    }

    QDate dBegin = QDate::currentDate();
    QDate dEnd   = QDate::currentDate();

    if(table->rowCount() > 0)
    {
        dBegin = QDate::fromString(table->item(0, 1)->text(), "dd.MM.yyyy");
        dEnd   = QDate::fromString(table->item(table->rowCount() - 1, 1)->text(), "dd.MM.yyyy");
    }

    CFilter::FilterIntervalType tinterval = { m_journal_parameters.total, 0, m_journal_parameters.total };
    CFilter::FilterDateType     tdate     = { dBegin, dEnd };

    m_filter[key] = CFilter(tinterval, tdate);

    CFilterDialog* filterDlg = new CFilterDialog(m_filter[key], this);

    if(filterDlg->exec() == QDialog::Accepted)
    {
        m_filter[key] = filterDlg->filter();
    }

    delete filterDlg;
}
/*!
 * \brief ConfiguratorWindow::createJournalTable
 * \return Возвращает true, если таблица успешно создана
 */
bool ConfiguratorWindow::createJournalTable(QSqlDatabase* db)
{
    QSqlQuery query(*db);

    // создание cлужебной таблицы для хранения скрытых от пользователя данных
    QString db_str = "CREATE TABLE table_info ("
                     "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                     "journal_type STRING(255) NOT NULL);";

    if(!query.exec(db_str))
    {
        QMessageBox::warning(this, tr("Создание таблицы"), tr("Невозможно создать служебную таблицу журналов: ") +
                                   query.lastError().text());
        return false;
    }

    // создание таблицы для хранения имен списка журналов
    db_str = "CREATE TABLE journal_list ("
             "id INTEGER PRIMARY KEY AUTOINCREMENT, "
             "name STRING(255), "
             "sn_device STRING(255));";

    if(!query.exec(db_str))
    {
        QMessageBox::warning(this, tr("Создание таблицы"), tr("Невозможно создать таблицу списка журналов: ") +
                                   query.lastError().text());
        return false;
    }

    // создание таблицы для хранения журналов
    db_str = "CREATE TABLE journals ("
             "id_msg INTEGER NOT NULL, "
             "date STRING(25) NOT NULL, "
             "time STRING(25), "
             "type STRING(255), "
             "category STRING(255), "
             "parameter STRING(255), "
             "sn_device INTEGER NOT NULL, "
             "CONSTRAINT new_pk PRIMARY KEY (id_msg, date, time, sn_device));";

    if(!query.exec(db_str))
    {
        QMessageBox::warning(this, tr("Создание таблицы"), tr("Невозможно создать таблицу журналов: ") +
                                   query.lastError().text());
        return false;
    }

    return true;
}
/*!
 * \brief   Метод проверяет ведется ли работа с каким-либо журналом
 * \param   widget - указатель на CJournalWidget, если используется какой-либо журнал, то указывает на него
 * \return  истина - в текущий момент открыт один из журналов
 */
bool ConfiguratorWindow::currentJournal(const CJournalWidget*& widget)
{
    int  index  = ui->stwgtMain->currentIndex();
    bool result = false;

    if(index < JOURNAL_INDEX_CRASH || index > JOURNAL_INDEX_OSCILLOSCOPE)
        return false;

    QWidget* curWgt = ui->stwgtMain->currentWidget();

    for(const QObject* obj: curWgt->children())
    {
        if(obj->isWidgetType() && QString(obj->metaObject()->className()) == "CJournalWidget")
        {
            widget = qobject_cast<const CJournalWidget*>(obj);
            result = true;
        }
    }

    return result;
}
//-------------------------------------
void ConfiguratorWindow::loadSettings()
{
    if(m_settings)
    {
        m_settings->beginGroup("serial-port");
            ui->cboxBaudrate->setCurrentText(m_settings->value("baudrate", "115200").toString());
            m_serialPortSettings->setDataBits(m_settings->value("databits", "8").toString());
            m_serialPortSettings->setStopBits(m_settings->value("stopbits", "1").toString());
            m_serialPortSettings->setParity(m_settings->value("parity", "Even").toString());
        m_settings->endGroup();

        m_settings->beginGroup("modbus");
            m_serialPortSettings->setModbusTimeout(m_settings->value("timeout", 500).toInt());
            m_serialPortSettings->setModbusTryCount(m_settings->value("trycount", 3).toInt());
        m_settings->endGroup();

        m_settings->beginGroup("device");
            ui->sboxTimeoutCalc->setValue(m_settings->value("timeoutcalculate", 1000).toInt());
            ui->checkboxCalibTimeout->setChecked(m_settings->value("timeoutcalculateenable", true).toBool());
            ui->spinboxSyncTime->setValue(m_settings->value("synctime", 1000).toInt());
        m_settings->endGroup();

        m_settings->beginGroup("mainwindow");
            restoreGeometry(m_settings->value("geometry").toByteArray());
        m_settings->endGroup();
    }
}
//-------------------------------------
void ConfiguratorWindow::saveSattings()
{
    if(m_settings)
    {
        m_settings->beginGroup("serial-port");
            m_settings->setValue("baudrate", ui->cboxBaudrate->currentText());
            m_settings->setValue("databits", m_serialPortSettings->dataBits());
            m_settings->setValue("stopbits", m_serialPortSettings->stopBits());
            m_settings->setValue("parity", m_serialPortSettings->parity());
        m_settings->endGroup();

        m_settings->beginGroup("modbus");
            m_settings->setValue("timeout", m_serialPortSettings->modbusTimeout());
            m_settings->setValue("trycount", m_serialPortSettings->modbusTryCount());
        m_settings->endGroup();

        m_settings->beginGroup("device");
            m_settings->setValue("timeoutcalculate", ui->sboxTimeoutCalc->value());
            m_settings->setValue("timeoutcalculateenable", ui->checkboxCalibTimeout->isChecked());
            m_settings->setValue("synctime", ui->spinboxSyncTime->value());
        m_settings->endGroup();

        m_settings->beginGroup("mainwindow");
            m_settings->setValue("geometry", saveGeometry());
        m_settings->endGroup();
    }
}
//----------------------------------------------------------------------------------------------------------------------
void ConfiguratorWindow::exportToPDF(const CJournalWidget* widget, const QString& reportName, const QString& sn_device,
                                                                   const QString& filename)
{
    QPrinter* printer = new QPrinter(QPrinter::ScreenResolution);

    printer->setOutputFormat(QPrinter::PdfFormat);
    printer->setPaperSize(QPrinter::A4);
    printer->setPageMargins(15, 10, 10, 10, QPrinter::Millimeter);
    printer->setOutputFileName(filename);

    QTextDocument* reportPDF = new QTextDocument;
    reportPDF->setPageSize(printer->pageRect().size());

    QTextTableFormat tableFormat;
    QVector<QTextLength> columnLength;

    columnLength << QTextLength(QTextLength::PercentageLength, 1);
    columnLength << QTextLength(QTextLength::PercentageLength, 1);
    columnLength << QTextLength(QTextLength::PercentageLength, 1);
    columnLength << QTextLength(QTextLength::VariableLength, 60);
    columnLength << QTextLength(QTextLength::PercentageLength, 20);
    columnLength << QTextLength(QTextLength::PercentageLength, 20);

    tableFormat.setColumnWidthConstraints(columnLength);
    tableFormat.setCellPadding(5);
    tableFormat.setCellSpacing(0);
    tableFormat.setHeaderRowCount(1);
    tableFormat.setBorderBrush(Qt::SolidPattern);
    tableFormat.setBorderStyle(QTextFrameFormat::BorderStyle_Ridge);
    tableFormat.setBorder(1);
    tableFormat.setWidth(QTextLength(QTextLength::PercentageLength, 100));

    QTextCursor cursor(reportPDF);
    QTextBlockFormat blockFormat;
    blockFormat.setPageBreakPolicy(QTextFormat::PageBreak_Auto);

    cursor.insertBlock(blockFormat);

    QTextBlockFormat blockHeaderFormat = cursor.blockFormat();
    blockHeaderFormat.setAlignment(Qt::AlignCenter);

    QTextCharFormat charHeaderFormat = cursor.charFormat();
    charHeaderFormat.setFontPointSize(18);
    charHeaderFormat.setFontWeight(QFont::Bold);

    QTextCharFormat charHeaderNextFormat = cursor.charFormat();
    charHeaderNextFormat.setFontPointSize(16);
    charHeaderNextFormat.setFontWeight(QFont::Bold);

    cursor.insertBlock(blockHeaderFormat, charHeaderFormat);
    cursor.insertText(reportName);

    cursor.insertBlock();
    cursor.setCharFormat(charHeaderNextFormat);
    cursor.insertText(tr("Блок БЗУ-2-16"));

    cursor.insertBlock();
    cursor.setCharFormat(charHeaderNextFormat);
    cursor.insertText(sn_device);

    cursor.insertBlock(blockFormat);

    int columnCount = widget->table()->columnCount();

    QPoint pos = QPoint(0, widget->table()->rowCount() - 1);

    QString key = widget->property("TYPE").toString();

    if(m_filter.find(key) != m_filter.end()) // получае данные от фильтра, если он задействован
    {
        CFilter filter = m_filter[key];

        if(filter && filter.type() == CFilter::DATE) // если фильтр активен
        {
            pos = indexDateFilter(widget->table(), filter.date().begin, filter.date().end);
        }
    }

    int rows = pos.y() - pos.x() + 1; // считаем количество строк для экспорта

    QTextTable* textTable = cursor.insertTable(rows + 1, columnCount, tableFormat);

    QTextBlockFormat blockTableHeaderFormat;
    blockTableHeaderFormat.setAlignment(Qt::AlignHCenter);

    for(int i = 0; i < columnCount; i++)
    {
        QTextTableCell cell = textTable->cellAt(0, i);
        Q_ASSERT(cell.isValid());
        QTextCharFormat tableHeaderFormat = cell.format();
        tableHeaderFormat.setFontWeight(QFont::Bold);
        tableHeaderFormat.setVerticalAlignment(QTextCharFormat::AlignBottom);
        cell.setFormat(tableHeaderFormat);
        QTextCursor cellCursor = cell.firstCursorPosition();
        cellCursor.setBlockFormat(blockTableHeaderFormat);
        cellCursor.insertText(widget->table()->horizontalHeaderItem(i)->text());
    }

    for(int i = 0; i < rows; i++)
    {
        for(int j = 0; j < columnCount; j++)
        {
            QTextTableCell cell = textTable->cellAt(i + 1, j);
            Q_ASSERT(cell.isValid());
            QTextCursor cellCursor = cell.firstCursorPosition();
            cellCursor.insertText(widget->table()->item(pos.x() + i, j)->text());
        }
    }

    cursor.movePosition(QTextCursor::End);

    QPainter painter(printer);

    QSizeF pageSize = printer->pageRect().size();

    const qreal  footerHeight = painter.fontMetrics().height();
    const QRectF textRect(0, 0, pageSize.width(), pageSize.height() - footerHeight);

    reportPDF->setPageSize(textRect.size());

    const int pageCount = reportPDF->pageCount();

    emit m_progressbar->settingChanged(0, pageCount, tr("стр"));

    bool firstPage = true;
    for (int pageIndex = 0; pageIndex < pageCount; ++pageIndex)
    {
        if(!firstPage)
        {
            printer->newPage();
        }

        painter.drawImage(textRect, QImage(":/images/resource/images/background_report.png"));

        if(pageIndex == 0)
        {
            QRectF headerRect = textRect;
            headerRect.setBottom(textRect.top());
            headerRect.setHeight(footerHeight);

            painter.drawText(headerRect, Qt::AlignVCenter|Qt::AlignLeft, widget->table()->item(pos.x(), 1)->text() +
                                         " - " + widget->table()->item(pos.y(), 1)->text());

            painter.drawText(headerRect, Qt::AlignVCenter|Qt::AlignRight, QObject::tr("Страниц: %1").arg(pageCount));
        }

        painter.save();
            const QRectF textPageRect(0, pageIndex*reportPDF->pageSize().height(), reportPDF->pageSize().width(),
                                                                                   reportPDF->pageSize().height());
            painter.setClipRect(textRect);
            painter.translate(0, -textPageRect.top());
            painter.translate(textRect.left(), textRect.top());
            reportPDF->drawContents(&painter);
        painter.restore();

        QRectF footerRect = textRect;
        footerRect.setTop(textRect.bottom());
        footerRect.setHeight(footerHeight);

        painter.drawText(footerRect, Qt::AlignVCenter|Qt::AlignLeft, QDate::currentDate().toString("dd.MM.yyyy"));

        painter.drawText(footerRect, Qt::AlignVCenter|Qt::AlignRight,
                                     QObject::tr("Страница %1 из %2").arg(pageIndex +1 ).arg(pageCount));

        emit m_progressbar->increment();

        firstPage = false;
    }
}
//--------------------------------------------
void ConfiguratorWindow::exportPurposeToJSON()
{
    CDataTable data;
    QString    fileNameDefault;
    QString    typeName;

    if(ui->stwgtMain->currentIndex() == 24)
    {
        data            = static_cast<CMatrixPurposeModel*>(ui->tablewgtLedPurpose->model())->dataTable();
        typeName        = "LED";
        fileNameDefault = "led";
    }
    else if(ui->stwgtMain->currentIndex() == 25)
    {
        data            = static_cast<CMatrixPurposeModel*>(ui->tablewgtDiscreteInputPurpose->model())->dataTable();
        typeName        = "INPUT";
        fileNameDefault = "input";
    }
    else if(ui->stwgtMain->currentIndex() == 26)
    {
        data            = static_cast<CMatrixPurposeModel*>(ui->tablewgtRelayPurpose->model())->dataTable();
        typeName        = "RELAY";
        fileNameDefault = "relay";
    }

    if(data.count() == 0)
        return;

    QDir dir;

    if(!dir.exists("profiles"))
        dir.mkdir("profiles");

    QString fileName = QFileDialog::getSaveFileName(this, tr("Открытие профиля привязок"),
                                                    QString(dir.absolutePath() + "/%1/%2").arg("profiles").arg(fileNameDefault),
                                                    tr("Профили привязок (*.prf)"));

    if(fileName.isEmpty())
        return;

    QFile file(fileName);

    if(!file.open(QFile::WriteOnly))
    {
        QMessageBox::warning(this, tr("Сохранение профиля привязок"), tr("Невозможно создать файл для записи \"") +
                             fileName + QString("\""));

        return;
    }

    QJsonObject json;
    QJsonArray  rowArr;
    QJsonArray  columnKeyArr;

    json["type"] = typeName;

    for(int i = 0; i < data.columnCounts(); i++)
    {
        QJsonObject tcolumnKeyObj;

        tcolumnKeyObj["key"] = data.columnData(i).first;

        columnKeyArr.append(tcolumnKeyObj);
    }

    json["headers"] = columnKeyArr;

    for(int i = 0; i < data.count(); i++)
    {
        QJsonObject trowCurObj;
        QJsonArray  columnArr;

        for(int j = 0; j < data.columnCounts(); j++)
        {
            QJsonObject tcolumnObj;

            tcolumnObj["status"] = data[i][j].status();
            tcolumnObj["state"]  = data[i][j].state();

            columnArr.append(tcolumnObj);
        }

        trowCurObj["key"]     = data[i].key();
        trowCurObj["name"]    = data[i].header();
        trowCurObj["columns"] = columnArr;

        rowArr.append(trowCurObj);
    }

    json["data"] = rowArr;

    QJsonDocument doc(json);

    file.write(doc.toJson());
    file.close();
}
//----------------------------------------------
void ConfiguratorWindow::importPurposeFromJSON()
{
    QString fileNameDefault;
    QString typeName;

    CMatrixPurposeModel* model = nullptr;

    if(ui->stwgtMain->currentIndex() == 24)
    {
        fileNameDefault = "led";
        typeName        = "LED";

        model = static_cast<CMatrixPurposeModel*>(ui->tablewgtLedPurpose->model());
    }
    else if(ui->stwgtMain->currentIndex() == 25)
    {
        fileNameDefault = "inputs";
        typeName        = "INPUT";

        model = static_cast<CMatrixPurposeModel*>(ui->tablewgtDiscreteInputPurpose->model());
    }
    else if(ui->stwgtMain->currentIndex() == 26)
    {
        fileNameDefault = "relay";
        typeName        = "RELAY";

        model = static_cast<CMatrixPurposeModel*>(ui->tablewgtRelayPurpose->model());
    }

    QDir dir;

    QString fileName = QFileDialog::getOpenFileName(this, tr("Открытие профиля привязок"),
                                                    QString(dir.absolutePath() + "/%1/%2").arg("profiles").arg(fileNameDefault),
                                                    tr("Профили привязок (*.prf)"));

    if(fileName.isEmpty())
        return;

    QFile file(fileName);

    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox::warning(this, tr("Импорт профиля привязок"), tr("Невозможно открыть файл профиля привязок: ") +
                                   file.errorString());

        return;
    }

    QJsonDocument json = QJsonDocument::fromJson(file.readAll());

    file.close();

    QJsonObject rootObj = json.object(); // корневой объект

    if(rootObj["type"].toString().toUpper() != typeName)
        return;

    QJsonArray headerArr = rootObj.value("headers").toArray(); // получение массива данных - заголовки колонок

    if(headerArr.isEmpty())
        return;

    QVector<CColumn::column_t> headers;

    for(int i = 0; i < headerArr.count(); i++)
    {
        QJsonObject headerObj = headerArr[i].toObject();

        if(headerObj.isEmpty())
            continue;

        headers << columnFromKey(headerObj["key"].toString());
    }

    QJsonArray dataArr = rootObj.value("data").toArray(); // получение массива данных - строки

    if(dataArr.isEmpty())
        return;

    QVector<CRow> rows;

    for(int i = 0; i < dataArr.count(); i++)
    {
        QJsonObject rowObj = dataArr[i].toObject(); // получаем объект строки из массива

        if(rowObj.isEmpty())
            continue;

        QJsonArray colArr = rowObj["columns"].toArray(); // получение массива данных - колонки

        if(colArr.isEmpty())
            continue;

        QVector<CColumn> columns;

        for(int j = 0; j < colArr.count(); j++)
        {
            QJsonObject colObj = colArr[j].toObject(); // получаем колонку из массива

            columns << CColumn(colObj["state"].toBool(), colObj["status"].toBool());
        }

        rows << CRow(rowObj["key"].toString(), rowObj["header"].toString(), columns);
    }

    CDataTable dataTable(rows, headers);

    if(model)
        model->setDataTable(dataTable);
}
//--------------------------------------------------------------
void ConfiguratorWindow::processReadJournal(CDataUnitType& unit)
{
    RequestType type = (RequestType)unit.property(tr("REQUEST")).toInt();

    if(type == READ_EVENT_SHIFT_PTR)
    {
        if(unit.valueCount() == 1) // подтверждение записи указателя сдвига
        {

        }
        else if(unit.valueCount() == 2) // получение значения указателя сдвига
        {
            qint32 ptr_value = (((unit.value(0) << 16)&0xFFFF) | (unit.value(1)&0xFFFF));

            m_journal_parameters.shift = ptr_value;
        }
    }
    else if(type == READ_EVENT_COUNT)
    {
        if(unit.valueCount() == 2)
        {
            qint32 msg_count = (((unit.value(0) << 16)&0xFFFF) | (unit.value(1)&0xFFFF));

            m_journal_parameters.total = msg_count;
        }
    }
    else if(type == READ_EVENT_JOURNAL)
    {
        if(!m_journal_read_current)
            return;

        if(unit.valueCount() < 8)
            return;

        m_journal_parameters.start += unit.valueCount()/8; // увеличиваем локальный счетчик прочитанных сообщений
        m_journal_parameters.read  += unit.valueCount()/8; // увеличиваем глобальный счетчик прочитанных сообщений

        displayJournalResponse(unit.values());
        journalRead();
    }
}
//-----------------------------------------------
void ConfiguratorWindow::updateParameterJournal()
{
    readEventJournalCount();
    setJournalPtrShift();
}
//---------------------------------------------------------
void ConfiguratorWindow::widgetStackIndexChanged(int index)
{
    if(index >= JOURNAL_INDEX_CRASH && index <= JOURNAL_INDEX_ISOLATION)
    {
        ui->tabwgtMenu->setTabEnabled(4, true);

        int width = ui->stwgtMain->width() - 760;

        ui->widgetJournalCrash->setTableColumnWidth(3, width);
        ui->widgetJournalEvent->setTableColumnWidth(3, width);
        ui->widgetJournalHalfHour->setTableColumnWidth(3, width);
        ui->widgetJournalIsolation->setTableColumnWidth(3, width);

        if(!currentJournal(m_active_journal_current))
            m_active_journal_current = nullptr;

        m_active_journal_current->header()->setTextDeviceCountMessages(0, m_journal_parameters.total);
    }
    else
    {
        ui->tabwgtMenu->setTabEnabled(4, false);
        m_active_journal_current = nullptr;
    }
}
//-------------------------------------------
void ConfiguratorWindow::setJournalPtrShift()
{
    QVector<quint16> values = QVector<quint16>() << (quint16)((m_journal_parameters.shift >> 16)&0xFFFF) <<
                                                    (quint16)(m_journal_parameters.shift&0xFFFF);

    CDataUnitType unit(ui->sboxSlaveID->value(), CDataUnitType::WriteMultipleRegisters, 0x300C, values);
    unit.setProperty(tr("REQUEST"), READ_EVENT_SHIFT_PTR);

    m_modbusDevice->request(unit);
}
//----------------------------------------------------------------------
void ConfiguratorWindow::valueEventJournalInternalChanged(int new_value)
{
    m_journal_parameters.count = m_journal_parameters.total - new_value;
}
//------------------------------------------------
void ConfiguratorWindow::timeoutSyncSerialNumber()
{
    CDataUnitType unit(ui->sboxSlaveID->value(), CDataUnitType::ReadInputRegisters, 0x0000, QVector<quint16>() << 8);

    unit.setProperty(tr("REQUEST"), READ_SERIAL_NUMBER);

    m_modbusDevice->request(unit);

    m_sync_timer.start(ui->spinboxSyncTime->value());
}
//---------------------------------------------
void ConfiguratorWindow::importJournalToTable()
{
    if(!m_active_journal_current) // не выбран текущий журнал
    {
        QMessageBox::warning(this, tr("Импорт журнала"), tr("Выберите пожалуйста журнал в который необходимо произвести импорт"));
        return;
    }

    // Получение таблицы и заголовка текущего журнала
    QTableWidget*   table  = m_active_journal_current->table();
    CHeaderJournal* header = m_active_journal_current->header();

    if(!table || !header)
        return;

    if(table->rowCount() > 0) // таблица пуста
    {
        clearJournal();
    }

    QString journal_name = m_active_journal_current->property("NAME").toString();
    QString journal_type = m_active_journal_current->property("TYPE").toString();

    if(journal_name.isEmpty() || journal_type.isEmpty())
    {
        QMessageBox::warning(this, tr("Экспорт журнала"), tr("Не удалось определить имя или тип журнала"));
        return;
    }

    // выбираем файл для импорта
    QDir dir;
    QString selectedFilter    = tr("Базы данных (*.db)");
    QString journal_full_name = tr("Журнал %1-%2").arg(journal_name).arg(m_status_bar->serialNumberText());
    QString journal_path      = QFileDialog::getOpenFileName(this, tr("Импорт журнала %1 из базы данных").arg(journal_name),
                                                             dir.absolutePath() + QString("/db/%1.%2").
                                                             arg(journal_full_name).arg("db"),
                                                             tr("Базы данных (*.db);;Все файлы (*.*)"), &selectedFilter);

    if(journal_path.isEmpty())
        return;

    QSqlDatabase* db = nullptr;

    if(!connectDb(db, journal_path)) // открываем базу данных
    {
        QMessageBox::warning(this, tr("Импорт журнала"), tr("Невозможно открыть базу данных журналов \"%1\"").arg(journal_name));
        disconnectDb(db);
        return;
    }

    // проверяем служебную информацию таблицы
    int count = recordCountDb(db, "table_info", "journal_type", QString("\"%1\"").arg(journal_type));

    if(count == 0) // такой записи в служебной таблице нет - тип журнала не совпадает с текущим
    {
        QMessageBox::warning(this, tr("Импорт журнала"), tr("Импортируемы журнал не является журналом %1.\n%2.").arg(journal_name).
                                                         arg("Выберите другой журнал"));
    }
    else if(count == -1) // открытая база не является базой журналов
    {
        QMessageBox::warning(this, tr("Импорт журнала"), tr("Импортируемый журнал не является каким-либо журналом.\n"
                                                            "Выберите другой файл."));
    }

    if(count <= 0) // выходим, если выбрана не та база
    {
        disconnectDb(db);
        return;
    }

    QSqlQuery query(*db);
    QString   query_str = "SELECT * FROM journal_list;";

    // получаем список журналов из базы
    if(!query.exec(query_str))
    {
        QMessageBox::warning(this, tr("Импорт журнала"), tr("Невозможно получить список журналов из базы %1:\n%2").
                                                         arg(journal_name).arg(query.lastError().text()));
        disconnectDb(db);
        return;
    }

    QVector<CEventListJournals::cell_t> journal_list;

    if(query.first()) // получен список журналов
    {
         journal_list << CEventListJournals::cell_t({ query.value("name").toString(), query.value("sn_device").toString(),
                                                      query.value("id").toInt() });

         while(query.next())
         {
             journal_list << CEventListJournals::cell_t({ query.value("name").toString(), query.value("sn_device").toString(),
                                                          query.value("id").toInt() });
         }
    }

    if(journal_list.isEmpty())
    {
        QMessageBox::warning(this, tr("Импорт журнала"), tr("Список журналов %1 пуст.").arg(journal_name));
        disconnectDb(db);
        return;
    }

    CEventListJournals* journalDialog = new CEventListJournals(journal_name, journal_list, this);

    int     id           = -1;
    QString serialNumber = "";

    if(journalDialog->exec() == QDialog::Accepted)
    {
        id           = journalDialog->currentId();
        serialNumber = journalDialog->currentSerialNumber();
    }

    if(id == -1 || serialNumber.isEmpty())
        return;

    if(m_status_bar->isState()) // если есть связь с устройством - проверяем сейрийный номер выбранного журнала с текущим
    {
        if(serialNumber != m_status_bar->serialNumberText()) // серийные номера не совпадают
        {
            QMessageBox::StandardButton reply;
            reply = QMessageBox::question(this, tr("Журнала журнала"),
                                          tr("Этот журнал не от этого устройства.\nВсе равно загрузить?"),
                                          QMessageBox::Yes | QMessageBox::No);

            if(reply == QMessageBox::No) // уходим, если нет
            {
                disconnectDb(db);
                return;
            }
        }
    }

    int rows = recordCountDb(db, "journals", "sn_device", QString::number(id));

    if(rows <= 0)
    {
        QMessageBox::warning(this, tr("Импорт журнала"), tr("База журнала %1 пуста.").arg(journal_name));
        disconnectDb(db);
        return;
    }

    QTime timer;
    timer.start();

    QString str = QString("SELECT * FROM journals WHERE sn_device=%1").arg(id);

    int max_read_msg = rows; // максимальное значение считываемых записей (используется для прогрессбара)

    if(m_filter.find(journal_type) != m_filter.end())
    {
        CFilter filter = m_filter[journal_type];

        if(filter && filter.type() == CFilter::DATE)
        {
            str += QString(" AND date BETWEEN \"%1\" AND \"%2\"").arg(filter.date().begin.toString(Qt::ISODate)).
                    arg(filter.date().end.toString(Qt::ISODate));

            max_read_msg = recordCountDb(db, "journals", "sn_device", QString::number(id), "date", QStringList() <<
                                         QString("\'%1\'").arg(filter.date().begin.toString(Qt::ISODate)) <<
                                         QString("\'%1\'").arg(filter.date().end.toString(Qt::ISODate)));
        }
    }

    str += ";";

    if(!query.exec(str))
    {
        QMessageBox::warning(this, tr("Импорт журнала"),
                                   tr("Не возможно получить записи журнала %1: %2").arg(journal_name).arg(query.lastError().text()));
    }

//    table->setSortingEnabled(false);

    m_progressbar->setProgressTitle("Импорт журнала событий");
    m_progressbar->progressStart();
    m_progressbar->setSettings(0, max_read_msg, "");

    int msg_count = 0;

    while(query.next()) // заносим данные в таблицу журналов событий
    {
        int row = table->rowCount();

        table->insertRow(row);

        QString id_msg    = query.value("id_msg").toString();
        QString date      = QDate::fromString(query.value("date").toString(), Qt::ISODate).toString("dd.MM.yyyy");
        QString time      = query.value("time").toString();
        QString type      = query.value("type").toString();
        QString category  = query.value("category").toString();
        QString parameter = query.value("parameter").toString();

        table->setItem(row, 0, new QTableWidgetItem(id_msg));
        table->setItem(row, 1, new CTableWidgetItem(date));
        table->setItem(row, 2, new QTableWidgetItem(time));
        table->setItem(row, 3, new QTableWidgetItem(type));
        table->setItem(row, 4, new QTableWidgetItem(category));
        table->setItem(row, 5, new QTableWidgetItem(parameter));

        table->item(row, 0)->setTextAlignment(Qt::AlignCenter);
        table->item(row, 1)->setTextAlignment(Qt::AlignCenter);
        table->item(row, 2)->setTextAlignment(Qt::AlignCenter);

        m_progressbar->progressIncrement();

        msg_count++;
    }

    if(m_filter.find(journal_type) != m_filter.end())
    {
        CFilter::FilterDateType d = { QDate::fromString(table->item(0, 1)->text(), "dd.MM.yyyy"),
                                      QDate::fromString(table->item(table->rowCount() - 1, 1)->text(), "dd.MM.yyyy")};
        m_filter[journal_type].setDate(d);
    }

//    table->sortByColumn(1, Qt::AscendingOrder);
//    table->setSortingEnabled(true);
    header->setTextTableCountMessages(msg_count);
    header->setTextElapsedTime(timer.elapsed());

    m_progressbar->progressStop();

    if(header->stateCheckbox())
        table->scrollToBottom();

    disconnectDb(db);
}
//------------------------------------------
void ConfiguratorWindow::exportJournalToDb()
{
    if(!m_active_journal_current) // не выбран текущий журнал
    {
        QMessageBox::warning(this, tr("Экспорт журнала"), tr("Выберите пожалуйста журнал из которого необходимо произвести экспорт"));
        return;
    }

    // Получение таблицы и заголовка текущего журнала
    QTableWidget*   table  = m_active_journal_current->table();
    CHeaderJournal* header = m_active_journal_current->header();

    if(!table || !header)
        return;

    if(table->rowCount() == 0) // таблица пуста
    {
        QMessageBox::information(this, tr("Экспорт журнала"), tr("Текущий журнал пуст"));
        return;
    }

    QString journal_name = m_active_journal_current->property("NAME").toString();
    QString journal_type = m_active_journal_current->property("TYPE").toString();

    if(journal_name.isEmpty() || journal_type.isEmpty())
    {
        QMessageBox::warning(this, tr("Экспорт журнала"), tr("Не удалось определить имя или тип журнала"));
        return;
    }

    // выбираем файл для экспорта
    QDir dir;
    QString selectedFilter    = tr("Базы данных (*.db)");
    QString journal_full_name = tr("Журнал %1-%2").arg(journal_name).arg(m_status_bar->serialNumberText());
    QString journal_path      = QFileDialog::getSaveFileName(this, tr("Экспорт журнала %1 в базу данных").arg(journal_name),
                                                             dir.absolutePath() + QString("/db/%1.%2").
                                                             arg(journal_full_name).arg("db"),
                                                             tr("Базы данных (*.db);;Все файлы (*.*)"), &selectedFilter,
                                                             QFileDialog::DontConfirmOverwrite);

    if(journal_path.isEmpty())
        return;

    QSqlDatabase* db           = nullptr;
    QString       baseNameFile = QFileInfo(journal_path).baseName();

    QFileInfo fi;
    bool      isNewBase = true;

    if(fi.exists(journal_path)) // Файл уже существует
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, tr("Экспорт журнала"),
                                      tr("Такая база уже существует. Перезаписать данные?"),
                                      QMessageBox::Yes | QMessageBox::No);

        if(reply == QMessageBox::Yes) // удаляемы старый файл базы данных
        {
            QFile file(journal_path);

            if(!file.remove())
            {
                QMessageBox::warning(this, tr("Удаление базы журналов"),
                                     tr("Невозможно удалить базу %1!\nВозможно уже используется или у Вас нет прав.").arg(baseNameFile));
                return;
            }
        }
        else // дописываем данные в существующий файл
        {
            isNewBase = false;
        }
    }

    if(!connectDb(db, journal_path))
    {
        QMessageBox::warning(this, tr("Экспорт журнала"), tr("Невозможно открыть/создать файл \"%1\".").arg(baseNameFile));
        disconnectDb(db);
        return;
    }

    QSqlQuery query(*db);
    QString   query_str;

    if(!isNewBase) // дозаписываем данные в существующий журнал
    {
        query_str = "SELECT journal_type FROM table_info;";

        if(!query.exec(query_str)) // читаем служебную информацию о типе журнала
        {
            QMessageBox::warning(this, tr("Экспорт журнала"), tr("Невозможно открыть базу \"%1\" для дозаписи новых данных").
                                       arg(baseNameFile));
            disconnectDb(db);
            return;
        }

        if(query.first())
        {
            QString type = query.value("journal_type").toString();

            if(type.toUpper() != journal_type.toUpper()) // типы журналов не совпадают
            {
                QMessageBox::warning(this, tr("Экспорт журнала"), tr("Невозможно выполнить запись журнала в эту базу.\n"
                                                                     "Требуется тип журнала \"%1\", а текущий имеет тип \"%2\"").
                                                                     arg(type.toLower()).arg(journal_type.toLower()));
                disconnectDb(db);
                return;
            }
        }
    }
    else // создаем новую базу данных
    {
        if(!createJournalTable(db)) // создаем таблицы
        {
            disconnectDb(db);
            return;
        }

        query_str = "INSERT INTO table_info (journal_type)"
                    "VALUES (\"" + journal_type + "\");";

        if(!query.exec(query_str)) // вставляем служебную информацию о типе журнала
        {
            QMessageBox::warning(this, tr("Экспорт журнала"), tr("Невозможно вставить служебную информацию в базу данных: %1").
                                                              arg(query.lastError().text()));
            disconnectDb(db);
            return;
        }
    }

    QString type_format = journal_type.toLower(); // форматируем тип журнала в его название в базе данных
    type_format[0] = type_format[0].toUpper();    // например: если тип "EVENT", то после получим "Event"

    QString name_db = QString("%1Journal-%2").arg(type_format).arg(m_status_bar->serialNumberText());

    if(recordCountDb(db, "journal_list", "name", QString("\"%1\"").arg(name_db)) == 0) // если такого имени в таблице журналов нет,
    {                                                                                  // то создаем такую запись
        query_str = "INSERT INTO journal_list (name, sn_device)" +
                    QString("VALUES (\"%1\", \"%2\");").arg(name_db).arg(m_status_bar->serialNumberText());

        if(!query.exec(query_str))
        {
            QMessageBox::warning(this, tr("Экспорт журнала"), tr("Невозможно записать новый журнал в базу данных: %1").
                                       arg(query.lastError().text()));
            disconnectDb(db);
            return;
        }
    }

    // получаем id записи из базы данных с текущим именем
    query_str = QString("SELECT id FROM journal_list WHERE name=\"%1\";").arg(name_db);

    if(!query.exec(query_str))
    {
        QMessageBox::warning(this, tr("Экспорт журнала"), tr("Невозможно прочитать id записи \"%1\"").arg(query_str));
        disconnectDb(db);
        return;
    }

    int id = -1;

    if(query.first())
    {
        id = query.value("id").toInt();
    }

    if(id == -1) // не смогли прочитать id - на выход
        return;

    QPoint pos(0, table->rowCount() - 1);

    if(m_filter.find(journal_type) != m_filter.end()) // фильтр для текущего журнала существует
    {
        CFilter filter = m_filter[journal_type]; // получаем фильтр

        if(filter && filter.type() == CFilter::DATE) // фильтр активен
        {
            CFilter::FilterDateType date = filter.date();
            pos = indexDateFilter(table, date.begin, date.end); // получаем новые позиции начала/конца записей (фильтруем)
        }
    }

    m_progressbar->setProgressTitle(tr("Экспорт журнала %1").arg(journal_name));
    m_progressbar->progressStart();
    m_progressbar->setSettings(0, (pos.y() - pos.x()) + 1, "");

    db->transaction();

    int i = 0;

    if(!dir.exists("log"))
        dir.mkdir("log");

    QFile file1("log/exportJournalToDb.log");

    file1.open(QFile::WriteOnly);

    QTextStream out1(&file1);

    QFile file2("log/exportJournalToDb_values.log");

    file2.open(QFile::WriteOnly);

    QTextStream out2(&file2);

    for(i = pos.x(); i <= pos.y(); i++)
    {
        int     id_msg    = table->item(i, 0)->text().toInt();
        QString date      = QDate::fromString(table->item(i, 1)->text(),
                                              "dd.MM.yyyy").toString(Qt::ISODate); // приведение строки к yyyy-MM-dd для sqlite
        QString time      = table->item(i, 2)->text();
        QString type      = table->item(i, 3)->text();
        QString category  = table->item(i, 4)->text();
        QString parameter = table->item(i, 5)->text();

        query.prepare(QString("INSERT OR REPLACE INTO journals (id_msg, date, time, type, category, parameter, sn_device)"
                              "VALUES(:id_msg, :date, :time, :type, :category, :parameter, :sn_device)"));
        query.bindValue(":id_msg", id_msg);
        query.bindValue(":date", date);
        query.bindValue(":time", time);
        query.bindValue(":type", type);
        query.bindValue(":category", category);
        query.bindValue(":parameter", parameter);
        query.bindValue(":sn_device", id);

        out2 << "id_msg = " << id_msg << "; date = " << date << "; time = " << time << "; type = " << type << "; category = " <<
                category << "; parameter = " << parameter << "; sn_device = " << id << "\n";

        if(!query.exec())
        {
            out1 << "\n-\t" << query.lastError().text() << "\n\n";
            out2 << "\n-\t" << "id_msg = " << id_msg << "; date = " << date << "; time = " << time << "; type = " << type <<
                    "; category = " << category << "; parameter = " << parameter << "; sn_device = " << id << "\n\n";
        }

        out1 << i << ": " << query.lastQuery() << "\n";

        m_progressbar->progressIncrement();
    }

    db->commit();
    disconnectDb(db);

    m_progressbar->progressStop();

    file2.close();
    file1.close();
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
//-------------------------------------------------
void ConfiguratorWindow::readShiftPrtEventJournal()
{
    CDataUnitType unit(ui->sboxSlaveID->value(), CDataUnitType::ReadHoldingRegisters, 0x300C, QVector<quint16>() << 2);

    unit.setProperty(tr("REQUEST"), READ_EVENT_SHIFT_PTR);

    m_modbusDevice->request(unit);
}
//----------------------------------------------
void ConfiguratorWindow::readEventJournalCount()
{
    CDataUnitType unit(ui->sboxSlaveID->value(), CDataUnitType::ReadInputRegisters, 34, QVector<quint16>() << 2);

    unit.setProperty(tr("REQUEST"), READ_EVENT_COUNT);

    m_modbusDevice->request(unit);
}
//---------------------------------------------
void ConfiguratorWindow::deviceSync(bool state)
{
    if(state)
    {
        if(!m_sync_timer.isActive())
        {
            m_sync_timer.start(ui->spinboxSyncTime->value());
            timeoutSyncSerialNumber();
        }
    }
    else
    {
        if(m_sync_timer.isActive())
            m_sync_timer.stop();
    }
}
/*!
 * \brief ConfiguratorWindow::recordCountDb
 * \param db           Указатель на базу данных
 * \param table_name   Имя таблицы
 * \param parameter    Параметр по которому идет выборка
 * \param value        Значение параметра
 * \param subparamter  По умолчаниею пустой (используется для поиска по диапазону)
 * \param range        Диапазон значений (список - 2 переменные)
 * \return             Количество записей
 *
 * Метод подсчитывает количество записей (parameter = value). Дополнительно можно указать диапазон по подпараметру
 * (subparameter между значениями min и max)
 */
int ConfiguratorWindow::recordCountDb(QSqlDatabase* db, const QString& table_name, const QString& parameter, const QString& value,
                                                        const QString& subparamter, const QStringList& range)
{
    if(!db->isOpen())
    {
        return -1;
    }

    QSqlQuery query(*db);

    QString str = QString("SELECT COUNT(*) FROM %1 WHERE %2=%3").arg(table_name).arg(parameter).arg(value);

    if(!subparamter.isEmpty() && range.count() == 2) // если указан дополнительный параметр и указаны два предела
    {
        str += QString(" AND %1 BETWEEN %2 AND %3").arg(subparamter).arg(range.at(0)).arg(range.at(1));
    }

    str += ';';

    if(!query.exec(str))
    {
        return -1;
    }

    if(!query.first())
        return -1;

    return query.value(0).toInt();
}
//---------------------------------------------------------------------------------------------------
QPoint ConfiguratorWindow::indexDateFilter(QTableWidget* table, const QDate& begin, const QDate& end)
{
    int  rows    = table->rowCount();
    bool isBegin = false;

    QPoint res(0, rows - 1);

    for(int i = 0; i < rows; i++)
    {
        QTableWidgetItem* item = table->item(i, 1);

        if(!item)
            continue;

        QDate date = QDate::fromString(item->text(), "dd.MM.yyyy");

        if(date.year() > 1900 && date.year() < 2000)
            date.setDate(date.year() + 100, date.month(), date.day());

        if((date == begin || date > begin) && !isBegin) // текущая дата равна искомой или больше искомой
        {                                                     // и флаг первого совадения ложь
            res.setX(i); // сохраняем текущий индекс
            isBegin = true;
        }
        else if(date == end && isBegin) // текущая дата равна искмой даты конца и флаг истина - сохраняем текущий индекс
        {
            res.setY(i);
        }
        else if(date > end) // текущая дата больше искомой - выходим
            break;
    }

    return res;
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
//--------------------------------------------------------------------------------------------
QTableView* ConfiguratorWindow::tableMatrixFromKeys(const QString& first, const QString& last)
{
    if(first.isEmpty() || last.isEmpty())
        return nullptr;

    QPoint indexes = indexPurposeKey(first, last);

    if(indexes.x() == -1 || indexes.y() == -1)
        return nullptr;

    if(m_purpose_list[indexes.x()].first.contains(tr("DI"), Qt::CaseInsensitive) &&
       m_purpose_list[indexes.y()].first.contains(tr("DI"), Qt::CaseInsensitive)) // входы
    {
        return ui->tablewgtDiscreteInputPurpose;
    }
    else if(m_purpose_list[indexes.x()].first.contains(tr("DO"), Qt::CaseInsensitive) &&
            m_purpose_list[indexes.y()].first.contains(tr("DO"), Qt::CaseInsensitive)) // выходы: реле
    {
        return ui->tablewgtRelayPurpose;
    }
    else if(m_purpose_list[indexes.x()].first.contains(tr("LED"), Qt::CaseInsensitive) &&
            m_purpose_list[indexes.y()].first.contains(tr("LED"), Qt::CaseInsensitive)) // выходы: светодиоды
    {
        return ui->tablewgtLedPurpose;
    }

    return nullptr;
}
//---------------------------------------------------------------------
CColumn::column_t ConfiguratorWindow::columnFromKey(const QString& key)
{
    if(m_variables.isEmpty())
        return CColumn::column_t();

    for(int i = 0; i < m_variables.count(); i++)
    {
        if(m_variables[i].first.toUpper() == key.toUpper())
            return m_variables[i];
    }

    return CColumn::column_t();
}
//------------------------------------
void ConfiguratorWindow::initConnect()
{
    connectSystemDb();

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
    connect(m_serialPortSettings, &CSerialPortSetting::timeout, this, &ConfiguratorWindow::timeoutValueChanged);
    connect(m_serialPortSettings, &CSerialPortSetting::numberRepeat, this, &ConfiguratorWindow::numberRepeatChanged);
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
    connect(ui->pbtnSerialPortSettings, &QPushButton::clicked, this, &ConfiguratorWindow::serialPortSettings);
    connect(ui->pbtnClearLedOutput, &QPushButton::clicked, this, &ConfiguratorWindow::clearIOTable);
    connect(ui->pbtnClearDiscreteInput, &QPushButton::clicked, this, &ConfiguratorWindow::clearIOTable);
    connect(ui->pbtnClearRelayOutput, &QPushButton::clicked, this, &ConfiguratorWindow::clearIOTable);
    connect(ui->pbtnClearKeyboardPurpose, &QPushButton::clicked, this, &ConfiguratorWindow::clearIOTable);
    connect(ui->widgetJournalEvent, &CJournalWidget::clickedButtonRead, this, &ConfiguratorWindow::processReadJournalEvent);
    connect(ui->widgetJournalEvent->header(), &CHeaderJournal::clickedButtonClear, this, &ConfiguratorWindow::clearJournal);
    connect(m_modbusDevice, &CModbus::connectDeviceState, ui->widgetJournalEvent->header(),
                                                          &CHeaderJournal::stateEnabledButtonReadChanged);
    connect(ui->pbtnMenuExit, &QPushButton::clicked, this, &ConfiguratorWindow::exitFromApp);
    connect(ui->pbtnMenuPanelMenuCtrl, &QPushButton::clicked, this, &ConfiguratorWindow::menuPanelCtrl);
    connect(ui->pbtnMenuPanelVariableCtrl, &QPushButton::clicked, this, &ConfiguratorWindow::variablePanelCtrl);
    connect(ui->pbtnMenuExportToPDF, &QPushButton::clicked, this, &ConfiguratorWindow::startExportToPDF);
    connect(ui->pbtnExportPurpose, &QPushButton::clicked, this, &ConfiguratorWindow::exportPurposeToJSON);
    connect(ui->pbtnImportPurpose, &QPushButton::clicked, this, &ConfiguratorWindow::importPurposeFromJSON);
    connect(ui->stwgtMain, &QStackedWidget::currentChanged, this, &ConfiguratorWindow::widgetStackIndexChanged);
    connect(&m_sync_timer, &QTimer::timeout, this, &ConfiguratorWindow::timeoutSyncSerialNumber);
    connect(ui->pushbtnImportEventDb, &QPushButton::clicked, this, &ConfiguratorWindow::importJournalToTable);
    connect(ui->pushbtnExportEventJournalDb, &QPushButton::clicked, this, &ConfiguratorWindow::exportJournalToDb);
    connect(ui->pbtnFilter, &QPushButton::clicked, this, &ConfiguratorWindow::filterDialog);
}
