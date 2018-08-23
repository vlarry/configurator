#include "configuratorwindow.h"
#include "ui_configuratorwindow.h"
//------------------------------------------------------
ConfiguratorWindow::ConfiguratorWindow(QWidget* parent):
    QMainWindow(parent),
    ui(new Ui::ConfiguratorWindow),
    m_modbus(nullptr),
    m_serialPortSettings_window(nullptr),
    m_terminal_window(nullptr),
    m_output_window(nullptr),
    m_monitor_purpose_window(nullptr),
    m_outputall_window(nullptr),
    m_inputs_window(nullptr),
    m_debuginfo_window(nullptr),
    m_popup(nullptr),
    m_tim_calculate(nullptr),
    m_tim_debug_info(nullptr),
    m_version_window(nullptr),
    m_timer_synchronization(nullptr),
    m_timer_new_address_set(nullptr),
    m_status_bar(nullptr),
    m_watcher(nullptr),
    m_progressbar(nullptr),
    m_settings(nullptr),
    m_active_journal_current(nullptr),
    m_journal_read_current(nullptr),
    m_journal_timer(nullptr)
{
    ui->setupUi(this);

    m_modbus                    = new CModBus(this);
    m_serialPortSettings_window = new CSerialPortSetting;
    m_terminal_window           = new CDragWidget(tr("Терминал"));
    m_output_window             = new CIndicatorState(this);
    m_monitor_purpose_window    = new CMonitorPurpose(tr("Монитор привязок по К10/К11"), this);
    m_outputall_window          = new COutputAll(tr("Все выходы"), this);
    m_inputs_window             = new COutputAll(tr("Входы"), this);
    m_debuginfo_window          = new CDebugInfo(tr("Отладочная информация"), this);
    m_status_window             = new CStatusInfo;
    m_status_bar                = new CStatusBar(statusBar());
    m_popup                     = new PopUp(this);
    m_watcher                   = new QFutureWatcher<void>(this);
    m_progressbar               = new CProgressBarWidget(this);
    m_settings                  = new QSettings(QSettings::IniFormat, QSettings::UserScope, ORGANIZATION_NAME,
                                                "configurator", this);

    m_tim_calculate                 = new QTimer(this);
    m_timer_new_address_set         = new QTimer(this);
    m_tim_debug_info                = new QTimer(this);
    m_timer_synchronization         = new QTimer(this);
    m_journal_timer                 = new QTimer(this);

    m_terminal_window->setWidget(new CTerminal(this));

    m_status_bar->addWidget(m_progressbar);
    statusBar()->addPermanentWidget(m_status_bar, 100);

    bool is_remove = deleteLogFile();

    qInfo() << tr("Запуск программы...");

    if(is_remove)
        qInfo() << tr("Файл логирования <log.txt> удален.");

    connectSystemDb();
    initJournals();
    initMenuPanel();
    initConnect();
    initCellBind(); // инициализация привязки настроек к адресам
    initPurposeBind(); // инициализация привязки "матрицы привязок выходов" к адресам
    initModelTables();
    initDeviceCode(); // инициализация списка кодов устройств
    initEventJournal(); // инициализация параметров журнала событий
    initCrashJournal(); // инициализация параметров журнала аварий
    initHalfhourJournal(); // инициализация параметров журнала получасовок
    initProtectionList(); // инициализация списка защит
    initIndicatorStates(); // инициализация окна отображения состояний индикаторов
    initMonitorPurpose();
    initOutputAll();
    initInputs();
    initDebugInfo();
    initWordStatus();

    ui->pushButtonMenuDeviceCtrl->setText(tr("Панель меню"));
    ui->pushButtonVariableCtrl->setText(tr("Панель измерений"));
    ui->pushButtonPanelMessage->setText(tr("Панель сообщений"));
    ui->pushButtonMenuDeviceCtrl->setDir(CDockPanelItemCtrl::Left);
    ui->pushButtonVariableCtrl->setDir(CDockPanelItemCtrl::Right);
    ui->pushButtonPanelMessage->setDir(CDockPanelItemCtrl::Bottom);

    refreshSerialPort();

    setWindowFlag(Qt::FramelessWindowHint);
}
//---------------------------------------
ConfiguratorWindow::~ConfiguratorWindow()
{
    saveSettings();

    if(m_timer_synchronization->isActive())
        m_timer_synchronization->stop();

    if(m_watcher->isRunning())
    {
        m_watcher->cancel();
        m_watcher->waitForFinished();
    }

    if(m_modbus)
    {
        if(m_modbus->channel()->isOpen())
            emit m_modbus->close();
    }

    qInfo() << tr("Завершение работы программы.");

    if(m_system_db.isOpen())
        m_system_db.close();

    delete m_terminal_window;
    m_terminal_window = nullptr;
    
    delete m_modbus;
    m_modbus = nullptr;
    
    delete ui;
}
//---------------------------------------
void ConfiguratorWindow::serialPortCtrl()
{
    if(!m_modbus || m_serialPortSettings_window->serialPortName().isEmpty())
    {
        if(ui->toolButtonConnect->isChecked())
            ui->toolButtonConnect->setChecked(false);

        noConnectMessage();

        return;
    }
        
    m_status_bar->clearStatusMessage();
    
    if(ui->toolButtonConnect->isChecked())
    {
        CConnect::SerialPortType settings =
        {
            m_serialPortSettings_window->serialPortName(),
            m_serialPortSettings_window->baudrate(),
            m_serialPortSettings_window->dataBits(),
            m_serialPortSettings_window->parity(),
            m_serialPortSettings_window->stopBits()
        };

        m_modbus->channel()->setSettings(settings);
        emit m_modbus->open();
    }
    else
    {
        m_progressbar->progressStop();
        emit m_modbus->close();
    }
}
//-----------------------------------------------
void ConfiguratorWindow::stateChanged(bool state)
{
    ui->toolButtonConnect->setChecked(state);

    m_status_bar->setStatusMessage(((state)?tr("Соединение с устройством установлено на скорости: %1 бод").
                                            arg(m_modbus->channel()->settings().baudrate):
                                            tr("Соединение с устройством разорвано")), 5000);
    
    if(ui->checkboxCalibTimeout->isChecked() && state)
        chboxCalculateTimeoutStateChanged(true);
    else
        m_tim_calculate->stop();

    if(state)
    {
        qInfo() << tr("Порт <%1> открыт.").arg(m_modbus->channel()->settings().name);
        ui->toolButtonConnect->setText(tr("Отключиться"));
        ui->toolButtonConnect->setIconSize(QSize(24, 24));
        ui->toolButtonConnect->setIcon(QIcon(":/images/resource/images/disconnect_serial.png"));
        ui->toolButtonConnect->setToolTip(tr("Отключение от БЗУ"));
        m_status_bar->clearSerialNumber(); // удаляем старый серийный номер
        synchronization(true); // запускаем синхронизацию
    }
    else
    {
        qInfo() << tr("Порт <%1> закрыт.").arg(m_modbus->channel()->settings().name);
        ui->toolButtonConnect->setText(tr("Подключиться"));
        ui->toolButtonConnect->setIconSize(QSize(24, 24));
        ui->toolButtonConnect->setIcon(QIcon(":/images/resource/images/flag.png"));
        ui->toolButtonConnect->setToolTip(tr("Подключение к БЗУ"));
        synchronization(); // отключение синхронизации
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
    }
    
    m_serialPortSettings_window->setSerialPortList(port_list);
}
//-------------------------------------------
void ConfiguratorWindow::serialPortSettings()
{
    m_serialPortSettings_window->show();
}
//------------------------------------------------
void ConfiguratorWindow::blockProtectionCtrlRead()
{
    for(const block_protection_t& block: m_block_list)
    {
        sendRequestRead(block.address, 24, READ_BLOCK_PROTECTION);
    }
}
//-------------------------------------------------
void ConfiguratorWindow::blockProtectionCtrlWrite()
{
    CMatrixPurposeModel* model = static_cast<CMatrixPurposeModel*>(ui->tablewgtProtectionCtrl->model());

    if(!model)
        return;

    CMatrix matrix = model->matrix();

    if(matrix.rowCount() == 0)
        return;

    for(int i = 0; i < matrix.rowCount(); i++)
    {
        QVector<block_protection_purpose_t> purpose_list = m_block_list[i].purpose;
        QVector<quint16> values = QVector<quint16>(24, 0);

        for(int j = 0, pos = 0; j < matrix.columnCount(); j++, pos++)
        {
            if(i == j)
            {
                pos--;
                continue;
            }

            block_protection_purpose_t purpose = purpose_list[pos];

            int col = purpose.bit/16;
            int bit = purpose.bit%16;

            bool state = matrix[i][j].data().state;

            quint16 value = values[col];

            if(state)
                value |= (1 << bit);

            values[col] = value;
        }

        QVector<quint16> tvalues;

        for(int k = 0; k < values.count() - 1; k += 2)
            tvalues << values[k + 1] << values[k];

        sendRequestWrite(m_block_list[i].address, tvalues, -1);
    }
}
//--------------------------------------
void ConfiguratorWindow::calculateRead()
{
    CModBusDataUnit unit_part1(m_serialPortSettings_window->deviceID(), CModBusDataUnit::ReadInputRegisters,
                             CALCULATE_ADDRESS_PART1, QVector<quint16>() << 66);
    unit_part1.setProperty(tr("REQUEST"), CALCULATE_TYPE);
    unit_part1.setProperty("PART", CALCULATE_ADDRESS_PART1);

    sendCalculateRead(unit_part1);

    CModBusDataUnit unit_part2(m_serialPortSettings_window->deviceID(), CModBusDataUnit::ReadInputRegisters,
                             CALCULATE_ADDRESS_PART2, QVector<quint16>() << 8);
    unit_part2.setProperty(tr("REQUEST"), CALCULATE_TYPE);
    unit_part2.setProperty("PART", CALCULATE_ADDRESS_PART2);

    sendCalculateRead(unit_part2);
}
//--------------------------------------
void ConfiguratorWindow::debugInfoRead()
{
    sendDebugInfoRead(0);
    sendDebugInfoRead(1);
    sendDebugInfoRead(2);
    sendDebugInfoRead(3);
    sendDebugInfoRead(4);
    sendDebugInfoRead(5);
    sendDebugInfoRead(6);
    sendDebugInfoRead(7);
    sendDebugInfoRead(8);
    sendDebugInfoRead(9);
    sendDebugInfoRead(10);
    sendDebugInfoRead(11);
    sendDebugInfoRead(12);
}
//------------------------------------------------------
void ConfiguratorWindow::journalRead(const QString& key)
{
    if(key.isEmpty() || m_journal_set.find(key) == m_journal_set.end())
        return;

    journal_set_t& set = m_journal_set[key];
    int sector_size = 4096/set.message.size; // размер сектора в сообщениях, т.е. сколько можно сообщений считать
                                             // без перехода

    if(!set.isStart) // если это первый запрос сообщения
    {
        m_journal_read_current = m_active_journal_current; // сохраняем текущий виджет таблицы

        if(m_journal_read_current->table()->rowCount() > 0) // есть данные в таблице
        {
            int result = QMessageBox::question(this, tr("Чтение журнала"), tr("Таблица журнала не пустая. Очистить?"));

            if(result == QMessageBox::Yes)
                clearJournal(); // очищаем таблицу
        }

        m_journal_read_current->header()->setTextDeviceCountMessages(0, set.message.read_total);

        set.isStart = true;
        set.shift_ptr = 0;
        set.buffer.clear();
        set.message.read_count = 0;
        set.message.read_current = 0;
        set.message.read_start = 0;
        set.message.read_limit = set.message.read_total;

        if(m_filter.find(key) != m_filter.end()) // если фильтр журнала существует
        {
            CFilter filter = m_filter[key]; // получаем фильтр журнала

            if(filter) // фильтр активен
            {
                if(filter.type() == CFilter::INTERVAL) // если выбран фильр по интервалу
                {
                    int pos_beg = filter.interval().begin;
                    int pos_end = filter.interval().end;

                    set.message.read_start = pos_beg; // устанавливаем номер сообщения с которого будем читать
                    set.message.read_limit = pos_end - pos_beg + 1; // устанавливаем сообщение до которого будем читать
                }
            }
        }

        if(set.message.read_start >= sector_size) // если начальное сообщение находится не в на первой странице
        {
            set.shift_ptr            = (--set.message.read_start/sector_size)*4096; // получаем смещение указателя
            set.message.read_current = set.message.read_start%sector_size; // устанавливаем текущее сообщение (для определения перехода указатеяля)
        }

        setJournalPtrShift(key, set.shift_ptr);

        m_time_process.start();

        m_progressbar->setProgressTitle(tr("Чтение журнала %1").arg(m_journal_read_current->property("NAME").toString()));
        m_progressbar->progressStart();
        m_progressbar->setSettings(set.message.read_count, set.message.read_limit, "сообщений");

        connect(ui->pushButtonJournalRead, &QPushButton::clicked, this, &ConfiguratorWindow::stopProgressbar);
    }

    if(set.message.read_current == sector_size) // дочитали до конца очередной страницы - переводим указатель
    {
        set.shift_ptr += 4096;
        set.message.read_current = 0;

        setJournalPtrShift(key, set.shift_ptr);
    }

    // Расчет размера сообещния
    int request_size = set.message.read_number*set.message.size;
    int msg_size     = 0;

    if(request_size > 250) // размер запроса превышает 250 байт (максимальный размер запроса 250 байт + 5 байт накладные расходы
    {
        if(!set.buffer.isEmpty()) // буфер данных не пустой
        {
            msg_size = request_size/2 - set.msg_part; // высчитываем разницу между отправленными данными и оставшимися
        }
        else
        {
            msg_size = request_size/4; // запрос делится на две части (делитель 4, т.к. request_size в байтах, а запрос в двухбайтовых
            set.msg_part = msg_size; // размер запроса заносится в переменную, которая хранит размеры частей запроса
        }
    }
    else // иначе отправляем одним запросом
    {
        msg_size = (((set.message.read_limit - set.message.read_count) >= set.message.read_number)?request_size/2:
                     (set.message.read_limit - set.message.read_count)*(set.message.size/2));
    }

    // Расчет адреса
    int address = set.message.read_current*(set.message.size/2) + set.address.start_page;

    if(set.msg_part > 0 && set.buffer.count() > 0) // размер части запроса не равен нулю и буфер хранящий часть ответа не пустой
    {
        address += set.msg_part;
    }

    CModBusDataUnit unit(m_serialPortSettings_window->deviceID(), CModBusDataUnit::ReadInputRegisters, address,
                       QVector<quint16>() << msg_size);
    unit.setProperty(tr("REQUEST"), READ_JOURNAL);
    unit.setProperty(tr("JOURNAL"), key);

    m_modbus->sendData(unit);

    m_journal_timer->start(1000);
    m_time_process_speed.start();
}
/*!
 * \brief ConfiguratorWindow::inputAnalogGeneralRead
 *
 * Чтение настроек "Настройки-аналоговые-основные"
 */
void ConfiguratorWindow::inputAnalogGeneralRead()
{
    sendSettingReadRequest("M01", "M03", CModBusDataUnit::ReadHoldingRegisters, 6,
                           DEVICE_MENU_ITEM_SETTINGS_ITEM_IN_ANALOG);
    sendSettingControlReadRequest("M04", DEVICE_MENU_ITEM_SETTINGS_ITEM_IN_ANALOG); // чтение состояния настройки
}
/*!
 * \brief ConfiguratorWindow::inputAnalogCalibrateRead
 * Чтение настроек "Настройки-аналоговые-калибровки"
 */
void ConfiguratorWindow::inputAnalogCalibrateRead()
{
    sendSettingReadRequest("KIA", "KU0X_", CModBusDataUnit::ReadHoldingRegisters, 36,
                           DEVICE_MENU_ITEM_SETTINGS_ITEM_IN_ANALOG);
}
/*!
 * \brief ConfiguratorWindow::inputAnalogGroupRead
 *
 * Чтение группы настроек "Настройки-аналоговые"
 */
void ConfiguratorWindow::inputAnalogGroupRead()
{
    inputAnalogCalibrateRead();
    inputAnalogGeneralRead();
}
/*!
 * \brief ConfiguratorWindow::inputAnalogGeneralWrite
 *
 * Запись настроек основные
 */
void ConfiguratorWindow::inputAnalogGeneralWrite()
{
    sendSettingWriteRequest("M01", "M03", DEVICE_MENU_ITEM_SETTINGS_ITEM_IN_ANALOG);
    sendSettingControlWriteRequest("M04", DEVICE_MENU_ITEM_SETTINGS_ITEM_IN_ANALOG); // запись состояния настройки
}
/*!
 * \brief ConfiguratorWindow::inputAnalogCalibrateWrite
 *
 * Запись настроек калибровки
 */
void ConfiguratorWindow::inputAnalogCalibrateWrite()
{
    sendSettingWriteRequest("KIA", "KU0X_", DEVICE_MENU_ITEM_SETTINGS_ITEM_IN_ANALOG);
}
/*!
 * \brief ConfiguratorWindow::inputAnalogGroupWrite
 *
 * Запись настроек группы аналоговые
 */
void ConfiguratorWindow::inputAnalogGroupWrite()
{
    inputAnalogGeneralWrite();
    inputAnalogCalibrateWrite();
}
/*!
 * \brief ConfiguratorWindow::protectionMTZ1Write
 *
 * Запись настроек МТЗ1
 */
void ConfiguratorWindow::protectionMTZ1Write()
{
    sendSettingControlWriteRequest("M05", DEVICE_MENU_PROTECT_ITEM_CURRENT);
    sendSettingWriteRequest("M06", "X01", DEVICE_MENU_PROTECT_ITEM_CURRENT);
    sendProtectionWorkModeRequest("MTZ1", FUN_SAVE, DEVICE_MENU_PROTECT_ITEM_CURRENT);
}
/*!
 * \brief ConfiguratorWindow::protectionMTZ2Write
 *
 * Запись настроек МТЗ2
 */
void ConfiguratorWindow::protectionMTZ2Write()
{
    sendSettingControlWriteRequest("M09", DEVICE_MENU_PROTECT_ITEM_CURRENT);
    sendSettingWriteRequest("M10", "X03", DEVICE_MENU_PROTECT_ITEM_CURRENT);
    sendProtectionWorkModeRequest("MTZ2", FUN_SAVE, DEVICE_MENU_PROTECT_ITEM_CURRENT);
}
/*!
 * \brief ConfiguratorWindow::protectionMTZ3Write
 *
 * Запись настроек МТЗ3
 */
void ConfiguratorWindow::protectionMTZ3Write()
{
    sendSettingControlWriteRequest("M13", DEVICE_MENU_PROTECT_ITEM_CURRENT);
    sendSettingControlWriteRequest("TZ", DEVICE_MENU_PROTECT_ITEM_CURRENT);
    sendProtectionWorkModeRequest("MTZ3", FUN_SAVE, DEVICE_MENU_PROTECT_ITEM_CURRENT);
    sendSettingWriteRequest("M14", "K22", DEVICE_MENU_PROTECT_ITEM_CURRENT);
    sendSettingWriteRequest("X04", "X04", DEVICE_MENU_PROTECT_ITEM_CURRENT);
    sendSettingWriteRequest("TZ1", "TZ7", DEVICE_MENU_PROTECT_ITEM_CURRENT);
}
/*!
 * \brief ConfiguratorWindow::protectionMTZ3SetCharWrite
 *
 *  Запись токовых характеристик защиты МТЗ3
 */
void ConfiguratorWindow::protectionMTZ3SetCharWrite()
{
    sendSettingWriteRequest("TZ1", "TZ7", DEVICE_MENU_PROTECT_ITEM_CURRENT);
}
/*!
 * \brief ConfiguratorWindow::protectionMTZ3ProperySteepWrite
 *
 *  Запись характеристики защиты МТЗ3 - Крутая
 */
void ConfiguratorWindow::protectionMTZ3PropertySteepWrite()
{
    sendSettingWriteRequest("TZ1", "TZ1", DEVICE_MENU_PROTECT_ITEM_CURRENT);
}
/*!
 * \brief ConfiguratorWindow::protectionMTZ3ProperySlopWrite
 *
 * Запись характеристики защиты МТЗ3 - Пологая
 */
void ConfiguratorWindow::protectionMTZ3PropertySlopWrite()
{
    sendSettingWriteRequest("TZ2", "TZ2", DEVICE_MENU_PROTECT_ITEM_CURRENT);
}
/*!
 * \brief ConfiguratorWindow::protectionMTZ3ProperyInversionWrite
 *
 * Запись характеристики защиты МТЗ3 - Инверсная
 */
void ConfiguratorWindow::protectionMTZ3PropertyInversionWrite()
{
    sendSettingWriteRequest("TZ3", "TZ3", DEVICE_MENU_PROTECT_ITEM_CURRENT);
}
/*!
 * \brief ConfiguratorWindow::protectionMTZ3ProperyDInversionWrite
 *
 * Запись характеристики защиты МТЗ3 - Длительно инверсная
 */
void ConfiguratorWindow::protectionMTZ3PropertyDInversionWrite()
{
    sendSettingWriteRequest("TZ4", "TZ4", DEVICE_MENU_PROTECT_ITEM_CURRENT);
}
/*!
 * \brief ConfiguratorWindow::protectionMTZ3ProperyBackWrite
 *
 * Запись характеристики защиты МТЗ3 - Возвратно зависимая
 */
void ConfiguratorWindow::protectionMTZ3PropertyBackWrite()
{
    sendSettingWriteRequest("TZ5", "TZ5", DEVICE_MENU_PROTECT_ITEM_CURRENT);
}
/*!
 * \brief ConfiguratorWindow::protectionMTZ3ProperyStrongInversionWrite
 *
 * Запись характеристики защиты МТЗ3 - Сильно инверсная
 */
void ConfiguratorWindow::protectionMTZ3PropertyStrongInversionWrite()
{
    sendSettingWriteRequest("TZ6", "TZ6", DEVICE_MENU_PROTECT_ITEM_CURRENT);
}
/*!
 * \brief ConfiguratorWindow::protectionMTZ3ProperyExtremalInversionWrite
 *
 * Запись характеристики защиты МТЗ3 - Экстремально инверсная
 */
void ConfiguratorWindow::protectionMTZ3PropertyExtremalInversionWrite()
{
    sendSettingWriteRequest("TZ7", "TZ7", DEVICE_MENU_PROTECT_ITEM_CURRENT);
}
/*!
 * \brief ConfiguratorWindow::protectionMTZ4Write
 *
 * Запись настроек МТЗ4
 */
void ConfiguratorWindow::protectionMTZ4Write()
{
    sendSettingControlWriteRequest("M16", DEVICE_MENU_PROTECT_ITEM_CURRENT);
    sendSettingWriteRequest("M17", "X05a", DEVICE_MENU_PROTECT_ITEM_CURRENT);
    sendProtectionWorkModeRequest("MTZ4", FUN_SAVE, DEVICE_MENU_PROTECT_ITEM_CURRENT);
}
/*!
 * \brief ConfiguratorWindow::protectionMTZGroupWrite
 *
 * Запись настроек группы МТЗ
 */
void ConfiguratorWindow::protectionMTZGroupWrite()
{
    protectionMTZ1Write();
    protectionMTZ2Write();
    protectionMTZ3Write();
    protectionMTZ4Write();
}
/*!
 * \brief ConfiguratorWindow::protectionUmax1Write
 *
 * Запись защиты Umax1
 */
void ConfiguratorWindow::protectionUmax1Write()
{
    sendSettingControlWriteRequest("M32", DEVICE_MENU_PROTECT_ITEM_POWER);
    sendSettingWriteRequest("M33", "X11", DEVICE_MENU_PROTECT_ITEM_POWER);
    sendProtectionWorkModeRequest("UMAX1", FUN_SAVE, DEVICE_MENU_PROTECT_ITEM_POWER);
}
/*!
 * \brief ConfiguratorWindow::protectionUmax2Write
 *
 * Запись защиты Umax2
 */
void ConfiguratorWindow::protectionUmax2Write()
{
    sendSettingControlWriteRequest("M35", DEVICE_MENU_PROTECT_ITEM_POWER);
    sendSettingWriteRequest("M36", "X12", DEVICE_MENU_PROTECT_ITEM_POWER);
    sendProtectionWorkModeRequest("UMAX2", FUN_SAVE, DEVICE_MENU_PROTECT_ITEM_POWER);
}
/*!
 * \brief ConfiguratorWindow::protectionUmin1Write
 *
 * Запись защиты Umin1
 */
void ConfiguratorWindow::protectionUmin1Write()
{
    sendSettingControlWriteRequest("M38", DEVICE_MENU_PROTECT_ITEM_POWER);
    sendSettingControlWriteRequest("M39", DEVICE_MENU_PROTECT_ITEM_POWER);
    sendSettingControlWriteRequest("M40", DEVICE_MENU_PROTECT_ITEM_POWER);
    sendSettingWriteRequest("M41", "X13", DEVICE_MENU_PROTECT_ITEM_POWER);
    sendProtectionWorkModeRequest("UMIN1", FUN_SAVE, DEVICE_MENU_PROTECT_ITEM_POWER);
}
/*!
 * \brief ConfiguratorWindow::protectionUmin2Write
 *
 * Запись защиты Umin2
 */
void ConfiguratorWindow::protectionUmin2Write()
{
    sendSettingControlWriteRequest("M43", DEVICE_MENU_PROTECT_ITEM_POWER);
    sendSettingControlWriteRequest("M44", DEVICE_MENU_PROTECT_ITEM_POWER);
    sendSettingControlWriteRequest("M45", DEVICE_MENU_PROTECT_ITEM_POWER);
    sendSettingWriteRequest("M46", "X14", DEVICE_MENU_PROTECT_ITEM_POWER);
    sendProtectionWorkModeRequest("UMIN2", FUN_SAVE, DEVICE_MENU_PROTECT_ITEM_POWER);
}
/*!
 * \brief ConfiguratorWindow::protection3U0Write
 *
 * Запись защиты 3U0
 */
void ConfiguratorWindow::protection3U0Write()
{
    sendSettingControlWriteRequest("M48", DEVICE_MENU_PROTECT_ITEM_POWER);
    sendSettingWriteRequest("M49", "X15", DEVICE_MENU_PROTECT_ITEM_POWER);
    sendProtectionWorkModeRequest("3U0", FUN_SAVE, DEVICE_MENU_PROTECT_ITEM_POWER);
}
/*!
 * \brief ConfiguratorWindow::protectionPowerWrite
 *
 * Запись настроек группы По Напряжению
 */
void ConfiguratorWindow::protectionPowerGroupWrite()
{
    protectionUmax1Write();
    protectionUmax2Write();
    protectionUmin1Write();
    protectionUmin2Write();
    protection3U0Write();
}
/*!
 * \brief ConfiguratorWindow::protectionOZZ1Write
 *
 * Запись защиты ОЗЗ1
 */
void ConfiguratorWindow::protectionOZZ1Write()
{
    sendSettingControlWriteRequest("M22", DEVICE_MENU_PROTECT_ITEM_DIRECTED);
    sendSettingWriteRequest("M23", "X07", DEVICE_MENU_PROTECT_ITEM_DIRECTED);
    sendProtectionWorkModeRequest("OZZ1", FUN_SAVE, DEVICE_MENU_PROTECT_ITEM_DIRECTED);
}
/*!
 * \brief ConfiguratorWindow::protectionOZZ2Write
 *
 * Запись защиты ОЗЗ2
 */
void ConfiguratorWindow::protectionOZZ2Write()
{
    sendSettingControlWriteRequest("K23", DEVICE_MENU_PROTECT_ITEM_DIRECTED);
    sendSettingWriteRequest("K24", "X07a", DEVICE_MENU_PROTECT_ITEM_DIRECTED);
    sendProtectionWorkModeRequest("OZZ2", FUN_SAVE, DEVICE_MENU_PROTECT_ITEM_DIRECTED);
}
/*!
 * \brief ConfiguratorWindow::protectionNZZ1Write
 *
 * Запись защиты НЗЗ1
 */
void ConfiguratorWindow::protectionNZZ1Write()
{
    sendSettingControlWriteRequest("M25", DEVICE_MENU_PROTECT_ITEM_DIRECTED);
    sendSettingWriteRequest("M26", "X09", DEVICE_MENU_PROTECT_ITEM_DIRECTED);
    sendSettingWriteRequest("M26C", "M28C", DEVICE_MENU_PROTECT_ITEM_DIRECTED);
    sendProtectionWorkModeRequest("NZZ1", FUN_SAVE, DEVICE_MENU_PROTECT_ITEM_DIRECTED);
}
/*!
 * \brief ConfiguratorWindow::protectionNZZ2Write
 *
 * Запись защиты НЗЗ2
 */
void ConfiguratorWindow::protectionNZZ2Write()
{
    sendSettingControlWriteRequest("K26", DEVICE_MENU_PROTECT_ITEM_DIRECTED);
    sendSettingWriteRequest("K27", "X09a", DEVICE_MENU_PROTECT_ITEM_DIRECTED);
    sendSettingWriteRequest("K27C", "K30C", DEVICE_MENU_PROTECT_ITEM_DIRECTED);
    sendProtectionWorkModeRequest("NZZ2", FUN_SAVE, DEVICE_MENU_PROTECT_ITEM_DIRECTED);
}
/*!
 * \brief ConfiguratorWindow::protectionDirectedGroupWrite
 *
 * Запись группы защит Направленные
 */
void ConfiguratorWindow::protectionDirectedGroupWrite()
{
    protectionOZZ1Write();
    protectionOZZ2Write();
    protectionNZZ1Write();
    protectionNZZ2Write();
}
/*!
 * \brief ConfiguratorWindow::protectionAchr1Write
 *
 * Запись защиты АЧР1
 */
void ConfiguratorWindow::protectionAchr1Write()
{
    sendSettingControlWriteRequest("M51", DEVICE_MENU_PROTECT_ITEM_FREQUENCY);
    sendSettingWriteRequest("M52", "X16", DEVICE_MENU_PROTECT_ITEM_FREQUENCY);
    sendProtectionWorkModeRequest("ACHR1", FUN_SAVE, DEVICE_MENU_PROTECT_ITEM_FREQUENCY);
}
/*!
 * \brief ConfiguratorWindow::protectionAchr2Write
 *
 * Запись защиты АЧР2
 */
void ConfiguratorWindow::protectionAchr2Write()
{
    sendSettingControlWriteRequest("M55", DEVICE_MENU_PROTECT_ITEM_FREQUENCY);
    sendSettingWriteRequest("M56", "X17", DEVICE_MENU_PROTECT_ITEM_FREQUENCY);
    sendProtectionWorkModeRequest("ACHR2", FUN_SAVE, DEVICE_MENU_PROTECT_ITEM_FREQUENCY);
}
/*!
 * \brief ConfiguratorWindow::protectionAchr3Write
 *
 * Запись защиты АЧР3
 */
void ConfiguratorWindow::protectionAchr3Write()
{
    sendSettingControlWriteRequest("M59", DEVICE_MENU_PROTECT_ITEM_FREQUENCY);
    sendSettingWriteRequest("M60", "X18", DEVICE_MENU_PROTECT_ITEM_FREQUENCY);
    sendProtectionWorkModeRequest("ACHR3", FUN_SAVE, DEVICE_MENU_PROTECT_ITEM_FREQUENCY);
}
/*!
 * \brief ConfiguratorWindow::protectionFrequencyGroupWrite
 *
 * Запись группы защит по частоте
 */
void ConfiguratorWindow::protectionFrequencyGroupWrite()
{
    protectionAchr1Write();
    protectionAchr2Write();
    protectionAchr3Write();
}
/*!
 * \brief ConfiguratorWindow::protectionArcWrite
 *
 * Запись защиты Дуговая
 */
void ConfiguratorWindow::protectionArcWrite()
{
    sendSettingControlWriteRequest("M63", DEVICE_MENU_PROTECT_ITEM_EXTERNAL);
    sendSettingWriteRequest("M64", "X19", DEVICE_MENU_PROTECT_ITEM_EXTERNAL);
    sendProtectionWorkModeRequest("ARC", FUN_SAVE, DEVICE_MENU_PROTECT_ITEM_EXTERNAL);
}
/*!
 * \brief ConfiguratorWindow::protectionExt1Write
 *
 * Запись защиты Внешняя1
 */
void ConfiguratorWindow::protectionExt1Write()
{
    sendSettingControlWriteRequest("M71", DEVICE_MENU_PROTECT_ITEM_EXTERNAL);
    sendSettingWriteRequest("M72", "M72", DEVICE_MENU_PROTECT_ITEM_EXTERNAL);
    sendProtectionWorkModeRequest("EXT1", FUN_SAVE, DEVICE_MENU_PROTECT_ITEM_EXTERNAL);
}
/*!
 * \brief ConfiguratorWindow::protectionExt2Write
 *
 * Запись защиты Внешняя2
 */
void ConfiguratorWindow::protectionExt2Write()
{
    sendSettingControlWriteRequest("M73", DEVICE_MENU_PROTECT_ITEM_EXTERNAL);
    sendSettingWriteRequest("M74", "M74", DEVICE_MENU_PROTECT_ITEM_EXTERNAL);
    sendProtectionWorkModeRequest("EXT2", FUN_SAVE, DEVICE_MENU_PROTECT_ITEM_EXTERNAL);
}
/*!
 * \brief ConfiguratorWindow::protectionExt3Write
 *
 * Запись защиты Внешняя3
 */
void ConfiguratorWindow::protectionExt3Write()
{
    sendSettingControlWriteRequest("M75", DEVICE_MENU_PROTECT_ITEM_EXTERNAL);
    sendSettingWriteRequest("M76", "M76", DEVICE_MENU_PROTECT_ITEM_EXTERNAL);
    sendProtectionWorkModeRequest("EXT3", FUN_SAVE, DEVICE_MENU_PROTECT_ITEM_EXTERNAL);
}
/*!
 * \brief ConfiguratorWindow::protectionExternalGroupWrite
 *
 * Запись группы защит Внешние
 */
void ConfiguratorWindow::protectionExternalGroupWrite()
{
    protectionArcWrite();
    protectionExt1Write();
    protectionExt2Write();
    protectionExt3Write();
}
/*!
 * \brief ConfiguratorWindow::protectionStartingWrite
 *
 * Запись защиты Пусковая
 */
void ConfiguratorWindow::protectionStartingWrite()
{
    sendSettingControlWriteRequest("M19", DEVICE_MENU_PROTECT_ITEM_MOTOR);
    sendSettingWriteRequest("M20", "X06", DEVICE_MENU_PROTECT_ITEM_MOTOR);
    sendProtectionWorkModeRequest("STARTING", FUN_SAVE, DEVICE_MENU_PROTECT_ITEM_MOTOR);
}
/*!
 * \brief ConfiguratorWindow::protectionIminWrite
 *
 * Запись защиты Imin
 */
void ConfiguratorWindow::protectionIminWrite()
{
    sendSettingControlWriteRequest("M29", DEVICE_MENU_PROTECT_ITEM_MOTOR);
    sendSettingWriteRequest("M30", "X10", DEVICE_MENU_PROTECT_ITEM_MOTOR);
    sendProtectionWorkModeRequest("IMIN", FUN_SAVE, DEVICE_MENU_PROTECT_ITEM_MOTOR);
}
/*!
 * \brief ConfiguratorWindow::protectionMotorGroupWrite
 *
 * Запись группы защит для двигателя
 */
void ConfiguratorWindow::protectionMotorGroupWrite()
{
    protectionStartingWrite();
    protectionIminWrite();
}
/*!
 * \brief ConfiguratorWindow::protectionTemp1Write
 *
 * Запись защиты Температурная1
 */
void ConfiguratorWindow::protectionTemp1Write()
{
    sendSettingControlWriteRequest("M65", DEVICE_MENU_PROTECT_ITEM_TEMPERATURE);
    sendSettingControlWriteRequest("M66", DEVICE_MENU_PROTECT_ITEM_TEMPERATURE);
    sendSettingWriteRequest("M67", "X20", DEVICE_MENU_PROTECT_ITEM_TEMPERATURE);
    sendProtectionWorkModeRequest("TEMP1", FUN_SAVE, DEVICE_MENU_PROTECT_ITEM_TEMPERATURE);
}
/*!
 * \brief ConfiguratorWindow::protectionTemp2Write
 *
 * Запись защиты Температурная2
 */
void ConfiguratorWindow::protectionTemp2Write()
{
    sendSettingControlWriteRequest("M65", DEVICE_MENU_PROTECT_ITEM_TEMPERATURE);
    sendSettingControlWriteRequest("M66", DEVICE_MENU_PROTECT_ITEM_TEMPERATURE);
    sendSettingWriteRequest("M68", "X21", DEVICE_MENU_PROTECT_ITEM_TEMPERATURE);
    sendProtectionWorkModeRequest("TEMP2", FUN_SAVE, DEVICE_MENU_PROTECT_ITEM_TEMPERATURE);
}
/*!
 * \brief ConfiguratorWindow::protectionTemperatureGroupWrite
 *
 * Запись группы защит по Температуре
 */
void ConfiguratorWindow::protectionTemperatureGroupWrite()
{
    protectionTemp1Write();
    protectionTemp2Write();
}
/*!
 * \brief ConfiguratorWindow::protectionLevel1Write
 *
 * Запись защиты Уровневая1
 */
void ConfiguratorWindow::protectionLevel1Write()
{
    sendSettingControlWriteRequest("M77", DEVICE_MENU_PROTECT_ITEM_RESERVE);
    sendSettingWriteRequest("M78", "M78", DEVICE_MENU_PROTECT_ITEM_RESERVE);
    sendProtectionWorkModeRequest("LEVEL1", FUN_SAVE, DEVICE_MENU_PROTECT_ITEM_RESERVE);
}
/*!
 * \brief ConfiguratorWindow::protectionLevel2Write
 *
 * Запись защиты Уровневая2
 */
void ConfiguratorWindow::protectionLevel2Write()
{
    sendSettingControlWriteRequest("M77", DEVICE_MENU_PROTECT_ITEM_RESERVE);
    sendSettingWriteRequest("M79", "M79", DEVICE_MENU_PROTECT_ITEM_RESERVE);
    sendProtectionWorkModeRequest("LEVEL2", FUN_SAVE, DEVICE_MENU_PROTECT_ITEM_RESERVE);
}
/*!
 * \brief ConfiguratorWindow::protectionSignalStartWrite
 *
 * Запись защиты Сигнал Пуска
 */
void ConfiguratorWindow::protectionSignalStartWrite()
{
//    QVector<QComboBox*> box_list = QVector<QComboBox*>() << ui->cboxN50 << ui->cboxN52 << ui->cboxN53 << ui->cboxN54 <<
//                                                            ui->cboxN55 << ui->cboxN56 << ui->cboxN57 << ui->cboxN58 <<
//                                                            ui->cboxN59 << ui->cboxV04 << ui->cboxV07 << ui->cboxV10 <<
//                                                            ui->cboxV13 << ui->cboxV16 << ui->cboxV19 << ui->cboxV22 <<
//                                                            ui->cboxV25 << ui->cboxV28 << ui->cboxV31 << ui->cboxV36 <<
//                                                            ui->cboxV39 << ui->cboxV44 << ui->cboxV50 << ui->cboxV62 <<
//                                                            ui->cboxV65 << ui->cboxV68 << ui->cboxV76 << ui->cboxV77 <<
//                                                            ui->cboxV81 << ui->cboxV86 << ui->cboxV90 << ui->cboxV95 <<
//                                                            ui->cboxV96;

//    QVector<quint16> data(24, 0); // 24 ячейки со значением нуль

//    for(QComboBox* box: box_list)
//    {
//        QString key = box->objectName().remove("cbox");

//        if(key.isEmpty())
//            continue;

//        int bit     = m_variable_bits[key];
//        int val_pos = bit/16;
//        int bit_pos = bit%16;

//        if(val_pos < data.count())
//        {
//            int item_pos = box->currentIndex();

//            if(item_pos == 1)
//                data[val_pos] |= (1 << bit_pos);
//        }
//    }

//    QVector<quint16> tdata;

//    for(int i = 0; i < data.count() - 1; i += 2) // меняем местами старший и младший байт
//    {
//        tdata << data[i + 1] << data[i];
//    }

//    int addr = addressSettingKey("M80");

//    CModBusDataUnit unit(m_serialPortSettings_window->deviceID(), CModBusDataUnit::WriteMultipleRegisters, addr, tdata);

//    unit.setProperty("REQUEST", PORTECT_RESERVE_SIGNAL_START);

//    m_modbus->sendData(unit);
}
/*!
 * \brief ConfiguratorWindow::protectionReserveGroupWrite
 *
 * Запись группы защит Резервные
 */
void ConfiguratorWindow::protectionReserveGroupWrite()
{
    protectionLevel1Write();
    protectionLevel2Write();
    protectionSignalStartWrite();
}
/*!
 * \brief ConfiguratorWindow::protectionBRUWrite
 *
 * Запись защиты БРУ
 */
void ConfiguratorWindow::protectionBRUWrite()
{
    sendSettingControlWriteRequest("M93", DEVICE_MENU_PROTECT_ITEM_CONTROL);
    sendSettingControlWriteRequest("M95", DEVICE_MENU_PROTECT_ITEM_CONTROL);
    sendSettingWriteRequest("M96", "M99", DEVICE_MENU_PROTECT_ITEM_CONTROL);
    sendProtectionWorkModeRequest("BRU", FUN_SAVE, DEVICE_MENU_PROTECT_ITEM_CONTROL);
}
/*!
 * \brief ConfiguratorWindow::protectionVacuumWrite
 *
 * Запись защиты Вакуум
 */
void ConfiguratorWindow::protectionVacuumWrite()
{
    sendSettingControlWriteRequest("M90", DEVICE_MENU_PROTECT_ITEM_CONTROL);
    sendSettingWriteRequest("M91", "X23", DEVICE_MENU_PROTECT_ITEM_CONTROL);
    sendProtectionWorkModeRequest("VACUUM", FUN_SAVE, DEVICE_MENU_PROTECT_ITEM_CONTROL);
}
/*!
 * \brief ConfiguratorWindow::protectionControlGroupWrite
 *
 * Запись группы защит Предварительного Контроля
 */
void ConfiguratorWindow::protectionControlGroupWrite()
{
    protectionBRUWrite();
    protectionVacuumWrite();
}
/*!
 * \brief ConfiguratorWindow::automationSwitchWrite
 *
 * Запись автоматик Выключатель
 */
void ConfiguratorWindow::automationSwitchWrite()
{
    sendSettingControlWriteRequest("K32", DEVICE_MENU_ITEM_AUTOMATION_ROOT);
    sendSettingControlWriteRequest("K01", DEVICE_MENU_ITEM_AUTOMATION_ROOT);
    sendSettingControlWriteRequest("K03", DEVICE_MENU_ITEM_AUTOMATION_ROOT);
    sendSettingControlWriteRequest("K06", DEVICE_MENU_ITEM_AUTOMATION_ROOT);
    sendSettingControlWriteRequest("K17", DEVICE_MENU_ITEM_AUTOMATION_ROOT);
    sendSettingControlWriteRequest("K07", DEVICE_MENU_ITEM_AUTOMATION_ROOT);
    sendSettingWriteRequest("K02", "K02", DEVICE_MENU_ITEM_AUTOMATION_ROOT);
    sendSettingWriteRequest("K50", "K50", DEVICE_MENU_ITEM_AUTOMATION_ROOT);
    sendSettingWriteRequest("K04", "K04", DEVICE_MENU_ITEM_AUTOMATION_ROOT);
    sendSettingWriteRequest("K51", "K51", DEVICE_MENU_ITEM_AUTOMATION_ROOT);
    sendSettingWriteRequest("K05", "K05", DEVICE_MENU_ITEM_AUTOMATION_ROOT);
    sendSettingWriteRequest("K08", "K08", DEVICE_MENU_ITEM_AUTOMATION_ROOT);
    sendSettingWriteRequest("K09", "K09", DEVICE_MENU_ITEM_AUTOMATION_ROOT);
    sendSettingWriteRequest("X22", "X22", DEVICE_MENU_ITEM_AUTOMATION_ROOT);
}
/*!
 * \brief ConfiguratorWindow::automationSwitchTruckWrite
 *
 * Запись автоматик Тележка Выключателя
 */
void ConfiguratorWindow::automationSwitchTruckWrite()
{
    sendSettingControlWriteRequest("K37", DEVICE_MENU_ITEM_AUTOMATION_ROOT);
    sendSettingControlWriteRequest("K41", DEVICE_MENU_ITEM_AUTOMATION_ROOT);
    sendSettingWriteRequest("K45", "K49", DEVICE_MENU_ITEM_AUTOMATION_ROOT);
    sendSettingWriteRequest("K58", "K59", DEVICE_MENU_ITEM_AUTOMATION_ROOT);
}
/*!
 * \brief ConfiguratorWindow::automationBlockWrite
 *
 * Запись автоматик Блокировок
 */
void ConfiguratorWindow::automationBlockWrite()
{
    sendSettingControlWriteRequest("K13", DEVICE_MENU_ITEM_AUTOMATION_ROOT);
    sendSettingControlWriteRequest("K14", DEVICE_MENU_ITEM_AUTOMATION_ROOT);
    sendSettingControlWriteRequest("K15", DEVICE_MENU_ITEM_AUTOMATION_ROOT);
}
/*!
 * \brief ConfiguratorWindow::automationBusWrite
 *
 * Запись автоматик Шинные разъединители
 */
void ConfiguratorWindow::automationBusWrite()
{
    sendSettingControlWriteRequest("K34", DEVICE_MENU_ITEM_AUTOMATION_ROOT);
    sendSettingControlWriteRequest("K38", DEVICE_MENU_ITEM_AUTOMATION_ROOT);
    sendSettingWriteRequest("K42", "K46", DEVICE_MENU_ITEM_AUTOMATION_ROOT);
    sendSettingWriteRequest("K52", "K53", DEVICE_MENU_ITEM_AUTOMATION_ROOT);
}
/*!
 * \brief ConfiguratorWindow::automationLineWrite
 *
 * Запись автоматик Линейные разъединители
 */
void ConfiguratorWindow::automationLineWrite()
{
    sendSettingControlWriteRequest("K35", DEVICE_MENU_ITEM_AUTOMATION_ROOT);
    sendSettingControlWriteRequest("K39", DEVICE_MENU_ITEM_AUTOMATION_ROOT);
    sendSettingWriteRequest("K43", "K47", DEVICE_MENU_ITEM_AUTOMATION_ROOT);
    sendSettingWriteRequest("K54", "K55", DEVICE_MENU_ITEM_AUTOMATION_ROOT);
}
/*!
 * \brief ConfiguratorWindow::automationEarthWrite
 *
 * Запись автоматик Землянные разъединители
 */
void ConfiguratorWindow::automationEarthWrite()
{
    sendSettingControlWriteRequest("K36", DEVICE_MENU_ITEM_AUTOMATION_ROOT);
    sendSettingControlWriteRequest("K40", DEVICE_MENU_ITEM_AUTOMATION_ROOT);
    sendSettingWriteRequest("K44", "K48", DEVICE_MENU_ITEM_AUTOMATION_ROOT);
    sendSettingWriteRequest("K56", "K57", DEVICE_MENU_ITEM_AUTOMATION_ROOT);
}
/*!
 * \brief ConfiguratorWindow::automationDisconnectorsGroupWrite
 *
 * Запись группы автоматик Разъединители
 */
void ConfiguratorWindow::automationDisconnectorsGroupWrite()
{
    automationBusWrite();
    automationLineWrite();
    automationEarthWrite();
}
/*!
 * \brief ConfiguratorWindow::automationCtrlTNWrite
 *
 * Запись автоматик Контроль ТН
 */
void ConfiguratorWindow::automationCtrlTNWrite()
{
    sendSettingControlWriteRequest("K18", DEVICE_MENU_ITEM_AUTOMATION_ROOT);
    sendSettingWriteRequest("K19", "K19", DEVICE_MENU_ITEM_AUTOMATION_ROOT);
}
/*!
 * \brief ConfiguratorWindow::automationAVRWrite
 *
 * Запись автоматик АВР
 */
void ConfiguratorWindow::automationAVRWrite()
{
    sendSettingControlWriteRequest("M81", DEVICE_MENU_ITEM_AUTOMATION_ROOT);
    sendSettingWriteRequest("M82", "M85", DEVICE_MENU_ITEM_AUTOMATION_ROOT);
}
/*!
 * \brief ConfiguratorWindow::automationAPVWrite
 *
 * Запись автоматик АПВ
 */
void ConfiguratorWindow::automationAPVWrite()
{
    sendSettingControlWriteRequest("M87", DEVICE_MENU_ITEM_AUTOMATION_ROOT);
    sendSettingWriteRequest("M88", "M89", DEVICE_MENU_ITEM_AUTOMATION_ROOT);
}
/*!
 * \brief ConfiguratorWindow::automationAPVSignalStartWrite
 *
 * Запись синалов пуска автоматики АПВ
 */
void ConfiguratorWindow::automationAPVSignalStartWrite()
{
//    QVector<QComboBox*> box_list = QVector<QComboBox*>() << ui->cboxN50_2 << ui->cboxN52_2 << ui->cboxN53_2 << ui->cboxN54_2 <<
//                                                            ui->cboxN55_2 << ui->cboxN56_2 << ui->cboxN57_2 << ui->cboxN58_2 <<
//                                                            ui->cboxN59_2 << ui->cboxV04_2 << ui->cboxV07_2 << ui->cboxV10_2 <<
//                                                            ui->cboxV13_2 << ui->cboxV16_2 << ui->cboxV19_2 << ui->cboxV22_2 <<
//                                                            ui->cboxV25_2 << ui->cboxV28_2 << ui->cboxV31_2 << ui->cboxV36_2 <<
//                                                            ui->cboxV39_2 << ui->cboxV44_2 << ui->cboxV50_2 << ui->cboxV62_2 <<
//                                                            ui->cboxV65_2 << ui->cboxV68_2 << ui->cboxV76_2 << ui->cboxV77_2 <<
//                                                            ui->cboxV81_2 << ui->cboxV86_2 << ui->cboxV90_2 << ui->cboxV95_2 <<
//                                                            ui->cboxV96_2;

//    QVector<quint16> data(24, 0); // 24 ячейки со значением нуль

//    for(QComboBox* box: box_list)
//    {
//        QString key = (box->objectName().remove("cbox")).remove("_2");

//        if(key.isEmpty())
//            continue;

//        int bit     = m_variable_bits[key];
//        int val_pos = bit/16;
//        int bit_pos = bit%16;

//        if(val_pos < data.count())
//        {
//            int item_pos = box->currentIndex();

//            if(item_pos == 1)
//                data[val_pos] |= (1 << bit_pos);
//        }
//    }

//    QVector<quint16> tdata;

//    for(int i = 0; i < data.count() - 1; i += 2) // меняем местами старший и младший байт
//    {
//        tdata << data[i + 1] << data[i];
//    }

//    int addr = addressSettingKey("M86");

//    CModBusDataUnit unit(m_serialPortSettings_window->deviceID(), CModBusDataUnit::WriteMultipleRegisters, addr, tdata);

//    unit.setProperty("REQUEST", AUTOMATION_SIGNAL_START);

//    m_modbus->sendData(unit);
}
/*!
 * \brief ConfiguratorWindow::purposeLedsWrite
 *
 * Запись настроек привязок Светодиодов
 */
void ConfiguratorWindow::purposeLedsWrite()
{
    sendPurposeWriteRequest("LED1", "LED2");
    sendPurposeWriteRequest("LED3", "LED4");
    sendPurposeWriteRequest("LED5", "LED6");
    sendPurposeWriteRequest("LED7", "LED8");
}
/*!
 * \brief ConfiguratorWindow::purposeInputWrite
 *
 * Запись настроек привязок Дискретных входов
 */
void ConfiguratorWindow::purposeInputWrite()
{
    sendPurposeDIWriteRequest(512, 590);
    sendPurposeDIWriteRequest(592, 670);
    sendPurposeInverseDIWriteRequest(768, 846);
    sendPurposeInverseDIWriteRequest(848, 926);
}
/*!
 * \brief ConfiguratorWindow::purposeRelayWrite
 *
 * Запись настроек привязок Реле
 */
void ConfiguratorWindow::purposeRelayWrite()
{
    sendPurposeWriteRequest("DO1", "DO2");
    sendPurposeWriteRequest("DO4", "DO5");
    sendPurposeWriteRequest("DO6", "DO7");
    sendPurposeWriteRequest("DO8", "DO9");
    sendPurposeWriteRequest("DO10", "DO11");
    sendPurposeWriteRequest("DO12", "DO13");
}
//----------------------------------------------
void ConfiguratorWindow::purposeMemoryOutLedWrite()
{
    CMatrixPurposeModel* model = static_cast<CMatrixPurposeModel*>(ui->tablewgtLedPurpose->model());

    if(!model)
        return;

    CMatrix matrix = model->matrix();
    QVector<quint16> data;

    for(int i = 0; i < matrix.columnCount(); i++)
    {
        CColumn column = matrix[0][i];
        data << ((column.data().state == CHECKED)?1:0);
    }

    if(!data.isEmpty())
        sendRequestWrite(0x90C, data, CModBusDataUnit::WriteMultipleRegisters);
}
//---------------------------------------------------
void ConfiguratorWindow::purposeMemoryOutRelayWrite()
{
    CMatrixPurposeModel* model = static_cast<CMatrixPurposeModel*>(ui->tablewgtRelayPurpose->model());

    if(!model)
        return;

    CMatrix matrix = model->matrix();
    QVector<quint16> data;

    for(int i = 0; i < matrix.columnCount(); i++)
    {
        CColumn column = matrix[0][i];
        data << ((column.data().state == CHECKED)?1:0);
    }

    if(!data.isEmpty())
        sendRequestWrite(0x900, data, CModBusDataUnit::WriteMultipleRegisters);
}
/*!
 * \brief ConfiguratorWindow::dateTimeWrite
 *
 * Запись настроек дата/время
 */
void ConfiguratorWindow::dateTimeWrite(const QDateTime& dateTime)
{
    if(!m_modbus->channel()->isOpen())
    {
        noConnectMessage();
        return;
    }

    QDateTime dt;

    if(dateTime.isValid())
        dt = dateTime;
    else
        dt = QDateTime(ui->dateEdit->date(), ui->timeEdit->time());

    quint16 year_month = ((((dt.date().year() - 2000) << 8)&0xFF00) | (dt.date().month()&0x00FF));
    quint16 date_wday  = (((dt.date().day() << 8)&0xFF00) | (dt.date().dayOfWeek()&0x00FF));
    quint16 hour       = (dt.time().hour()&0x00FF);
    quint16 min_second = (((dt.time().minute() << 8)&0xFF00) | (dt.time().second()&0x00FF));

    QVector<quint16> data = QVector<quint16>() << year_month << date_wday << hour << min_second;

    CModBusDataUnit unit(m_serialPortSettings_window->deviceID(), CModBusDataUnit::WriteMultipleRegisters, 0x2000, data);

    unit.setProperty(tr("REQUEST"), DATETIME_TYPE);

    m_modbus->sendData(unit);
}
//------------------------------------------------
void ConfiguratorWindow::synchronizationDateTime()
{
    dateTimeWrite(QDateTime::currentDateTime());
}
/*!
 * \brief ConfiguratorWindow::settingCommunicationsWrite
 *
 * Запись настроек связи
 */
void ConfiguratorWindow::settingCommunicationsWrite()
{
    int answer = QMessageBox::question(this, tr("Запись настроек связи"),
                                             tr("Вы действительно хотите перезаписать настройки связи?"),
                                             QMessageBox::Yes | QMessageBox::No);

    if(answer == QMessageBox::No)
        return;

    sendRequestWrite(0x26, QVector<quint16>() << (quint16)ui->spinBoxCommunicationRequestTimeout->value(), 255);
    sendRequestWrite(0x27, QVector<quint16>() << (quint16)ui->spinBoxCommunicationTimeoutSpeed->value(), 255);
    sendRequestWrite(0x25, QVector<quint16>() << (quint16)ui->spinBoxCommunicationAddress->value(), 255);

    qDebug() << tr("Сохранение таймаутов: %1, %2.").arg(ui->spinBoxCommunicationTimeoutSpeed->value()).
                                                    arg(ui->spinBoxCommunicationAddress->value());

    connect(m_timer_new_address_set, &QTimer::timeout, this, &ConfiguratorWindow::setNewAddress);
    m_timer_new_address_set->start(500);
}
/*!
 * \brief ConfiguratorWindow::protectionMTZ1Read
 *
 * Чтение защиты МТЗ1
 */
void ConfiguratorWindow::protectionMTZ1Read()
{
    sendSettingControlReadRequest("M05", DEVICE_MENU_PROTECT_ITEM_CURRENT);
    sendSettingReadRequest("M06", "X01", CModBusDataUnit::ReadHoldingRegisters, 8, DEVICE_MENU_PROTECT_ITEM_CURRENT);
    sendProtectionWorkModeRequest("MTZ1", FUN_READ, DEVICE_MENU_PROTECT_ITEM_CURRENT);
}
/*!
 * \brief ConfiguratorWindow::protectionMTZ2Read
 *
 * Чтение защиты МТЗ2
 */
void ConfiguratorWindow::protectionMTZ2Read()
{
    sendSettingControlReadRequest("M09", DEVICE_MENU_PROTECT_ITEM_CURRENT);
    sendSettingReadRequest(tr("M10"), tr("X03"), CModBusDataUnit::ReadHoldingRegisters, 8, DEVICE_MENU_PROTECT_ITEM_CURRENT);
    sendProtectionWorkModeRequest("MTZ2", FUN_READ, DEVICE_MENU_PROTECT_ITEM_CURRENT);
}
/*!
 * \brief ConfiguratorWindow::protectionMTZ3Read
 *
 * Чтение защиты МТЗ3
 */
void ConfiguratorWindow::protectionMTZ3Read()
{
    sendSettingControlReadRequest("M13", DEVICE_MENU_PROTECT_ITEM_CURRENT);
    sendSettingControlReadRequest("TZ", DEVICE_MENU_PROTECT_ITEM_CURRENT);
    sendProtectionWorkModeRequest("MTZ3", FUN_READ, DEVICE_MENU_PROTECT_ITEM_CURRENT);
    sendSettingReadRequest("M14", "K22", CModBusDataUnit::ReadHoldingRegisters, 4, DEVICE_MENU_PROTECT_ITEM_CURRENT);
    sendSettingReadRequest("X04", "X04", CModBusDataUnit::ReadHoldingRegisters, 2, DEVICE_MENU_PROTECT_ITEM_CURRENT);
    sendSettingReadRequest("TZ1", "TZ7", CModBusDataUnit::ReadHoldingRegisters, 14, DEVICE_MENU_PROTECT_ITEM_CURRENT);
}
/*!
 * \brief ConfiguratorWindow::protectionMTZ3SetCharRead
 *
 * Чтение токовых характеристик защиты МТЗ3
 */
void ConfiguratorWindow::protectionMTZ3SetCharRead()
{
    sendSettingReadRequest("TZ1", "TZ7", CModBusDataUnit::ReadHoldingRegisters, 14, DEVICE_MENU_PROTECT_ITEM_CURRENT);
}
/*!
 * \brief ConfiguratorWindow::protectionMTZ3ProperySteepRead
 *
 * Чтение характеристики защиты МТЗ3 - Крутая
 */
void ConfiguratorWindow::protectionMTZ3PropertySteepRead()
{
    sendSettingReadRequest("TZ1", "TZ1", CModBusDataUnit::ReadHoldingRegisters, 2, DEVICE_MENU_PROTECT_ITEM_CURRENT);
}
/*!
 * \brief ConfiguratorWindow::protectionMTZ3ProperySlopRead
 *
 * Чтение характеристики защиты МТЗ3 - Пологая
 */
void ConfiguratorWindow::protectionMTZ3PropertySlopRead()
{
    sendSettingReadRequest("TZ2", "TZ2", CModBusDataUnit::ReadHoldingRegisters, 2, DEVICE_MENU_PROTECT_ITEM_CURRENT);
}
/*!
 * \brief ConfiguratorWindow::protectionMTZ3ProperyInversionRead
 *
 * Чтение характеристики защиты МТЗ3 - Инверсная
 */
void ConfiguratorWindow::protectionMTZ3PropertyInversionRead()
{
    sendSettingReadRequest("TZ3", "TZ3", CModBusDataUnit::ReadHoldingRegisters, 2, DEVICE_MENU_PROTECT_ITEM_CURRENT);
}
/*!
 * \brief ConfiguratorWindow::protectionMTZ3ProperyDInversionRead
 *
 * Чтение характеристики защиты МТЗ3 - Длительно инверсная
 */
void ConfiguratorWindow::protectionMTZ3PropertyDInversionRead()
{
    sendSettingReadRequest("TZ4", "TZ4", CModBusDataUnit::ReadHoldingRegisters, 2, DEVICE_MENU_PROTECT_ITEM_CURRENT);
}
/*!
 * \brief ConfiguratorWindow::protectionMTZ3ProperyBackRead
 *
 * Чтение характеристики защиты МТЗ3 - Обратно зависимая
 */
void ConfiguratorWindow::protectionMTZ3PropertyBackRead()
{
    sendSettingReadRequest("TZ5", "TZ5", CModBusDataUnit::ReadHoldingRegisters, 2, DEVICE_MENU_PROTECT_ITEM_CURRENT);
}
/*!
 * \brief ConfiguratorWindow::protectionMTZ3ProperyStrongInversionRead
 *
 * Чтение характеристики защиты МТЗ3 - Сильно инверсная
 */
void ConfiguratorWindow::protectionMTZ3PropertyStrongInversionRead()
{
    sendSettingReadRequest("TZ6", "TZ6", CModBusDataUnit::ReadHoldingRegisters, 2, DEVICE_MENU_PROTECT_ITEM_CURRENT);
}
/*!
 * \brief ConfiguratorWindow::protectionMTZ3ProperyExtremalInversionRead
 *
 * Чтение характеристики защиты МТЗ3 - Экстремально инверсная
 */
void ConfiguratorWindow::protectionMTZ3PropertyExtremalInversionRead()
{
    sendSettingReadRequest("TZ7", "TZ7", CModBusDataUnit::ReadHoldingRegisters, 2, DEVICE_MENU_PROTECT_ITEM_CURRENT);
}
/*!
 * \brief ConfiguratorWindow::protectionMTZ4Read
 *
 * Чтение защиты МТЗ4
 */
void ConfiguratorWindow::protectionMTZ4Read()
{
    sendSettingControlReadRequest("M16", DEVICE_MENU_PROTECT_ITEM_CURRENT);
    sendSettingReadRequest("M17", "X05a", CModBusDataUnit::ReadHoldingRegisters, 12, DEVICE_MENU_PROTECT_ITEM_CURRENT);
    sendProtectionWorkModeRequest("MTZ4", FUN_READ, DEVICE_MENU_PROTECT_ITEM_CURRENT);
}
/*!
 * \brief ConfiguratorWindow::protectionMTZGroupRead
 *
 * Чтение группы защит МТЗ
 */
void ConfiguratorWindow::protectionMTZGroupRead()
{
    protectionMTZ1Read();
    protectionMTZ2Read();
    protectionMTZ3Read();
    protectionMTZ4Read();
}
/*!
 * \brief ConfiguratorWindow::protectionUmax1Read
 *
 * Чтение защиты Umax1
 */
void ConfiguratorWindow::protectionUmax1Read()
{
    sendSettingControlReadRequest("M32", DEVICE_MENU_PROTECT_ITEM_POWER);
    sendSettingReadRequest("M33", "X11", CModBusDataUnit::ReadHoldingRegisters, 6, DEVICE_MENU_PROTECT_ITEM_POWER);
    sendProtectionWorkModeRequest("UMAX1", FUN_READ, DEVICE_MENU_PROTECT_ITEM_POWER);
}
/*!
 * \brief ConfiguratorWindow::protectionUmax2Read
 *
 * Чтение защиты Umax2
 */
void ConfiguratorWindow::protectionUmax2Read()
{
    sendSettingControlReadRequest("M35", DEVICE_MENU_PROTECT_ITEM_POWER);
    sendSettingReadRequest("M36", "X12", CModBusDataUnit::ReadHoldingRegisters, 6, DEVICE_MENU_PROTECT_ITEM_POWER);
    sendProtectionWorkModeRequest("UMAX2", FUN_READ, DEVICE_MENU_PROTECT_ITEM_POWER);
}
/*!
 * \brief ConfiguratorWindow::protectionUmin1Read
 *
 * Чтение защиты Umin1
 */
void ConfiguratorWindow::protectionUmin1Read()
{
    sendSettingControlReadRequest("M38", DEVICE_MENU_PROTECT_ITEM_POWER);
    sendSettingControlReadRequest("M39", DEVICE_MENU_PROTECT_ITEM_POWER);
    sendSettingControlReadRequest("M40", DEVICE_MENU_PROTECT_ITEM_POWER);
    sendSettingReadRequest("M41", "X13", CModBusDataUnit::ReadHoldingRegisters, 6, DEVICE_MENU_PROTECT_ITEM_POWER);
    sendProtectionWorkModeRequest("UMIN1", FUN_READ, DEVICE_MENU_PROTECT_ITEM_POWER);
}
/*!
 * \brief ConfiguratorWindow::protectionUmin2Read
 *
 * Чтение защиты Umin2
 */
void ConfiguratorWindow::protectionUmin2Read()
{
    sendSettingControlReadRequest("M43", DEVICE_MENU_PROTECT_ITEM_POWER);
    sendSettingControlReadRequest("M44", DEVICE_MENU_PROTECT_ITEM_POWER);
    sendSettingControlReadRequest("M45", DEVICE_MENU_PROTECT_ITEM_POWER);
    sendSettingReadRequest("M46", "X14", CModBusDataUnit::ReadHoldingRegisters, 6, DEVICE_MENU_PROTECT_ITEM_POWER);
    sendProtectionWorkModeRequest("UMIN2", FUN_READ, DEVICE_MENU_PROTECT_ITEM_POWER);
}
/*!
 * \brief ConfiguratorWindow::protection3U0Read
 *
 * Чтение защиты 3U0
 */
void ConfiguratorWindow::protection3U0Read()
{
    sendSettingControlReadRequest("M48", DEVICE_MENU_PROTECT_ITEM_POWER);
    sendSettingReadRequest("M49", "X15", CModBusDataUnit::ReadHoldingRegisters, 6, DEVICE_MENU_PROTECT_ITEM_POWER);
    sendProtectionWorkModeRequest("3U0", FUN_READ, DEVICE_MENU_PROTECT_ITEM_POWER);
}
/*!
 * \brief ConfiguratorWindow::protectionPowerGroupRead
 *
 * Чтение группы защит по напряжению
 */
void ConfiguratorWindow::protectionPowerGroupRead()
{
    protectionUmax1Read();
    protectionUmax2Read();
    protectionUmin1Read();
    protectionUmin2Read();
    protection3U0Read();
}
/*!
 * \brief ConfiguratorWindow::protectionDirectedOZZ1Read
 *
 * Чтение защиты ОЗЗ1
 */
void ConfiguratorWindow::protectionOZZ1Read()
{
    sendSettingControlReadRequest("M22", DEVICE_MENU_PROTECT_ITEM_DIRECTED);
    sendSettingReadRequest("M23", "X07", CModBusDataUnit::ReadHoldingRegisters, 6, DEVICE_MENU_PROTECT_ITEM_DIRECTED);
    sendProtectionWorkModeRequest("OZZ1", FUN_READ, DEVICE_MENU_PROTECT_ITEM_DIRECTED);
}
/*!
 * \brief ConfiguratorWindow::protectionDirectedOZZ2Read
 *
 * Чтение защиты ОЗЗ2
 */
void ConfiguratorWindow::protectionOZZ2Read()
{
    sendSettingControlReadRequest("K23", DEVICE_MENU_PROTECT_ITEM_DIRECTED);
    sendSettingReadRequest("K24", "X07a", CModBusDataUnit::ReadHoldingRegisters, 6, DEVICE_MENU_PROTECT_ITEM_DIRECTED);
    sendProtectionWorkModeRequest("OZZ2", FUN_READ, DEVICE_MENU_PROTECT_ITEM_DIRECTED);
}
/*!
 * \brief ConfiguratorWindow::protectionDirectedNZZ1Read
 *
 * Чтение защиты НЗЗ1
 */
void ConfiguratorWindow::protectionNZZ1Read()
{
    sendSettingControlReadRequest("M25", DEVICE_MENU_PROTECT_ITEM_DIRECTED);
    sendSettingReadRequest("M26", "X09", CModBusDataUnit::ReadHoldingRegisters, 12, DEVICE_MENU_PROTECT_ITEM_DIRECTED);
    sendSettingReadRequest("M26C", "M28C", CModBusDataUnit::ReadHoldingRegisters, 8, DEVICE_MENU_PROTECT_ITEM_DIRECTED);
    sendProtectionWorkModeRequest("NZZ1", FUN_READ, DEVICE_MENU_PROTECT_ITEM_DIRECTED);
}
/*!
 * \brief ConfiguratorWindow::protectionDirectedNZZ2Read
 *
 * Чтение защиты НЗЗ2
 */
void ConfiguratorWindow::protectionNZZ2Read()
{
    sendSettingControlReadRequest("K26", DEVICE_MENU_PROTECT_ITEM_DIRECTED);
    sendSettingReadRequest("K27", "X09a", CModBusDataUnit::ReadHoldingRegisters, 12, DEVICE_MENU_PROTECT_ITEM_DIRECTED);
    sendSettingReadRequest("K27C", "K30C", CModBusDataUnit::ReadHoldingRegisters, 8, DEVICE_MENU_PROTECT_ITEM_DIRECTED);
    sendProtectionWorkModeRequest("NZZ2", FUN_READ, DEVICE_MENU_PROTECT_ITEM_DIRECTED);
}
/*!
 * \brief ConfiguratorWindow::protectionDirectedGroupRead
 *
 * Чтение группы направленных защит
 */
void ConfiguratorWindow::protectionDirectedGroupRead()
{
    protectionOZZ1Read();
    protectionOZZ2Read();
    protectionNZZ1Read();
    protectionNZZ2Read();
}
/*!
 * \brief ConfiguratorWindow::protectionAchr1Read
 *
 * Чтение защиты АЧР1
 */
void ConfiguratorWindow::protectionAchr1Read()
{
    sendSettingControlReadRequest("M51", DEVICE_MENU_PROTECT_ITEM_FREQUENCY);
    sendSettingReadRequest("M52", "X16", CModBusDataUnit::ReadHoldingRegisters, 8, DEVICE_MENU_PROTECT_ITEM_FREQUENCY);
    sendProtectionWorkModeRequest("ACHR1", FUN_READ, DEVICE_MENU_PROTECT_ITEM_FREQUENCY);
}
/*!
 * \brief ConfiguratorWindow::protectionAchr2Read
 *
 * Чтение защиты АЧР2
 */
void ConfiguratorWindow::protectionAchr2Read()
{
    sendSettingControlReadRequest("M55", DEVICE_MENU_PROTECT_ITEM_FREQUENCY);
    sendSettingReadRequest("M56", "X17", CModBusDataUnit::ReadHoldingRegisters, 8, DEVICE_MENU_PROTECT_ITEM_FREQUENCY);
    sendProtectionWorkModeRequest("ACHR2", FUN_READ, DEVICE_MENU_PROTECT_ITEM_FREQUENCY);
}
/*!
 * \brief ConfiguratorWindow::protectionAchr3Read
 *
 * Чтение защиты АЧР3
 */
void ConfiguratorWindow::protectionAchr3Read()
{
    sendSettingControlReadRequest("M59", DEVICE_MENU_PROTECT_ITEM_FREQUENCY);
    sendSettingReadRequest("M60", "X18", CModBusDataUnit::ReadHoldingRegisters, 8, DEVICE_MENU_PROTECT_ITEM_FREQUENCY);
    sendProtectionWorkModeRequest("ACHR3", FUN_READ, DEVICE_MENU_PROTECT_ITEM_FREQUENCY);
}
/*!
 * \brief ConfiguratorWindow::protectionFrequencyGroupRead
 *
 * Чтение группы защит по частоте
 */
void ConfiguratorWindow::protectionFrequencyGroupRead()
{
    protectionAchr1Read();
    protectionAchr2Read();
    protectionAchr3Read();
}
/*!
 * \brief ConfiguratorWindow::protectionArc
 *
 * Чтение защиты Дуговая
 */
void ConfiguratorWindow::protectionArcRead()
{
    sendSettingControlReadRequest("M63", DEVICE_MENU_PROTECT_ITEM_EXTERNAL);
    sendSettingReadRequest("M64", "X19", CModBusDataUnit::ReadHoldingRegisters, 4, DEVICE_MENU_PROTECT_ITEM_EXTERNAL);
    sendProtectionWorkModeRequest("ARC", FUN_READ, DEVICE_MENU_PROTECT_ITEM_EXTERNAL);
}
/*!
 * \brief ConfiguratorWindow::protectionExt1
 *
 * Чтение защиты Внешняя1
 */
void ConfiguratorWindow::protectionExt1Read()
{
    sendSettingControlReadRequest("M71", DEVICE_MENU_PROTECT_ITEM_EXTERNAL);
    sendSettingReadRequest("M72", "M72", CModBusDataUnit::ReadHoldingRegisters, 2, DEVICE_MENU_PROTECT_ITEM_EXTERNAL);
    sendProtectionWorkModeRequest("EXT1", FUN_READ, DEVICE_MENU_PROTECT_ITEM_EXTERNAL);
}
/*!
 * \brief ConfiguratorWindow::protectionExt2
 *
 * Чтение защыты Внешняя2
 */
void ConfiguratorWindow::protectionExt2Read()
{
    sendSettingControlReadRequest("M73", DEVICE_MENU_PROTECT_ITEM_EXTERNAL);
    sendSettingReadRequest("M74", "M74", CModBusDataUnit::ReadHoldingRegisters, 2, DEVICE_MENU_PROTECT_ITEM_EXTERNAL);
    sendProtectionWorkModeRequest("EXT2", FUN_READ, DEVICE_MENU_PROTECT_ITEM_EXTERNAL);
}
/*!
 * \brief ConfiguratorWindow::protectionExt3
 *
 * Чтение защиты Внешняя3
 */
void ConfiguratorWindow::protectionExt3Read()
{
    sendSettingControlReadRequest("M75", DEVICE_MENU_PROTECT_ITEM_EXTERNAL);
    sendSettingReadRequest("M76", "M76", CModBusDataUnit::ReadHoldingRegisters, 2, DEVICE_MENU_PROTECT_ITEM_EXTERNAL);
    sendProtectionWorkModeRequest("EXT3", FUN_READ, DEVICE_MENU_PROTECT_ITEM_EXTERNAL);
}
/*!
 * \brief ConfiguratorWindow::protectionExternal
 *
 * Чтение группы внешних защит
 */
void ConfiguratorWindow::protectionExternalGroupRead()
{
    protectionArcRead();
    protectionExt1Read();
    protectionExt2Read();
    protectionExt3Read();
}
/*!
 * \brief ConfiguratorWindow::protectionStartingRead
 *
 * Чтение защиты Пусковая
 */
void ConfiguratorWindow::protectionStartingRead()
{
    sendSettingControlReadRequest("M19", DEVICE_MENU_PROTECT_ITEM_MOTOR);
    sendSettingReadRequest("M20", "X06", CModBusDataUnit::ReadHoldingRegisters, 6, DEVICE_MENU_PROTECT_ITEM_MOTOR);
    sendProtectionWorkModeRequest("STARTING", FUN_READ, DEVICE_MENU_PROTECT_ITEM_MOTOR);
}
/*!
 * \brief ConfiguratorWindow::protectionIminRead
 *
 * Чтение защиты Imin
 */
void ConfiguratorWindow::protectionIminRead()
{
    sendSettingControlReadRequest("M29", DEVICE_MENU_PROTECT_ITEM_MOTOR);
    sendSettingReadRequest("M30", "X10", CModBusDataUnit::ReadHoldingRegisters, 6, DEVICE_MENU_PROTECT_ITEM_MOTOR);
    sendProtectionWorkModeRequest("IMIN", FUN_READ, DEVICE_MENU_PROTECT_ITEM_MOTOR);
}
/*!
 * \brief ConfiguratorWindow::protectionMotorRead
 *
 * Чтение защит для Двигателя
 */
void ConfiguratorWindow::protectionMotorGroupRead()
{
    protectionStartingRead();
    protectionIminRead();
}
/*!
 * \brief ConfiguratorWindow::protectionTemp1Read
 *
 * Чтение защиты Температурная1
 */
void ConfiguratorWindow::protectionTemp1Read()
{
    sendSettingControlReadRequest("M65", DEVICE_MENU_PROTECT_ITEM_TEMPERATURE);
    sendSettingControlReadRequest("M66", DEVICE_MENU_PROTECT_ITEM_TEMPERATURE);
    sendSettingReadRequest("M67", "X20", CModBusDataUnit::ReadHoldingRegisters, 6, DEVICE_MENU_PROTECT_ITEM_TEMPERATURE);
    sendProtectionWorkModeRequest("TEMP1", FUN_READ, DEVICE_MENU_PROTECT_ITEM_TEMPERATURE);
}
/*!
 * \brief ConfiguratorWindow::protectionTemp2Read
 *
 * Чтение защиты Температурная2
 */
void ConfiguratorWindow::protectionTemp2Read()
{
    sendSettingControlReadRequest("M65", DEVICE_MENU_PROTECT_ITEM_TEMPERATURE);
    sendSettingControlReadRequest("M66", DEVICE_MENU_PROTECT_ITEM_TEMPERATURE);
    sendSettingReadRequest("M68", "X21", CModBusDataUnit::ReadHoldingRegisters, 6, DEVICE_MENU_PROTECT_ITEM_TEMPERATURE);
    sendProtectionWorkModeRequest("TEMP2", FUN_READ, DEVICE_MENU_PROTECT_ITEM_TEMPERATURE);
}
/*!
 * \brief ConfiguratorWindow::protectionTemperatureRead
 *
 * Чтение защит по Температуре
 */
void ConfiguratorWindow::protectionTemperatureGroupRead()
{
    protectionTemp1Read();
    protectionTemp2Read();
}
/*!
 * \brief ConfiguratorWindow::protectionLevel1Read
 *
 * Чтение защиты Уровень1
 */
void ConfiguratorWindow::protectionLevel1Read()
{
    sendSettingControlReadRequest("M77", DEVICE_MENU_PROTECT_ITEM_RESERVE);
    sendSettingReadRequest("M78", "M78", CModBusDataUnit::ReadHoldingRegisters, 2, DEVICE_MENU_PROTECT_ITEM_RESERVE);
    sendProtectionWorkModeRequest("LEVEL1", FUN_READ, DEVICE_MENU_PROTECT_ITEM_RESERVE);
}
/*!
 * \brief ConfiguratorWindow::protectionLevel2Read
 *
 * Чтение защиты Уровень2
 */
void ConfiguratorWindow::protectionLevel2Read()
{
    sendSettingControlReadRequest("M77", DEVICE_MENU_PROTECT_ITEM_RESERVE);
    sendSettingReadRequest("M79", "M79", CModBusDataUnit::ReadHoldingRegisters, 2, DEVICE_MENU_PROTECT_ITEM_RESERVE);
    sendProtectionWorkModeRequest("LEVEL2", FUN_READ, DEVICE_MENU_PROTECT_ITEM_RESERVE);
}
/*!
 * \brief ConfiguratorWindow::protectionSignalStartRead
 *
 * Чтение защиты Сигнала пуска
 */
void ConfiguratorWindow::protectionSignalStartRead()
{
    int addr = addressSettingKey("M80");

    CModBusDataUnit unit(m_serialPortSettings_window->deviceID(), CModBusDataUnit::ReadHoldingRegisters, addr,
                         QVector<quint16>() << 24);

    unit.setProperty("REQUEST", PORTECT_RESERVE_SIGNAL_START);

    m_modbus->sendData(unit);
}
/*!
 * \brief ConfiguratorWindow::protectionReserveRead
 *
 * Чтение группы резервных защит
 */
void ConfiguratorWindow::protectionReserveGroupRead()
{
    protectionLevel1Read();
    protectionLevel2Read();
    protectionSignalStartRead();
}
/*!
 * \brief ConfiguratorWindow::protectionBRURead
 *
 * Чтение защиты БРУ
 */
void ConfiguratorWindow::protectionBRURead()
{
    sendSettingControlReadRequest("M93", DEVICE_MENU_PROTECT_ITEM_CONTROL);
    sendSettingControlReadRequest("M95", DEVICE_MENU_PROTECT_ITEM_CONTROL);
    sendSettingReadRequest("M96", "M99", CModBusDataUnit::ReadHoldingRegisters, 8, DEVICE_MENU_PROTECT_ITEM_CONTROL);
    sendProtectionWorkModeRequest("BRU", FUN_READ, DEVICE_MENU_PROTECT_ITEM_CONTROL);
}
/*!
 * \brief ConfiguratorWindow::protectionVacuumRead
 *
 * Чтение защиты Вакуум
 */
void ConfiguratorWindow::protectionVacuumRead()
{
    sendSettingControlReadRequest("M90", DEVICE_MENU_PROTECT_ITEM_CONTROL);
    sendSettingReadRequest("M91", "X23", CModBusDataUnit::ReadHoldingRegisters, 6, DEVICE_MENU_PROTECT_ITEM_CONTROL);
    sendProtectionWorkModeRequest("VACUUM", FUN_READ, DEVICE_MENU_PROTECT_ITEM_CONTROL);
}
/*!
 * \brief ConfiguratorWindow::protectionControlRead
 *
 * Чтение групп защиты Предварительного контроля
 */
void ConfiguratorWindow::protectionControlGroupRead()
{
    protectionBRURead();
    protectionVacuumRead();
}
/*!
 * \brief ConfiguratorWindow::amplitudeReadOfCurrent
 *
 * Чтение амплитуд по фазам
 */
void ConfiguratorWindow::amplitudeReadOfCurrent()
{
    if(ui->widgetCalibrationOfCurrent->ctrl3I0()->isChecked())
        sendRequestRead(235, 2, AMPLITUDE_READ_CH2, CModBusDataUnit::ReadInputRegisters);
    if(ui->widgetCalibrationOfCurrent->ctrlIa()->isChecked())
        sendRequestRead(250, 2, AMPLITUDE_READ_CH3, CModBusDataUnit::ReadInputRegisters);
    if(ui->widgetCalibrationOfCurrent->ctrlIb()->isChecked())
        sendRequestRead(265, 2, AMPLITUDE_READ_CH4, CModBusDataUnit::ReadInputRegisters);
    if(ui->widgetCalibrationOfCurrent->ctrlIc()->isChecked())
        sendRequestRead(280, 2, AMPLITUDE_READ_CH5, CModBusDataUnit::ReadInputRegisters);
}
/*!
 * \brief ConfiguratorWindow::automationSwitchRead
 *
 * Чтение автоматика Выключатель
 */
void ConfiguratorWindow::automationSwitchRead()
{
    sendSettingControlReadRequest("K32", DEVICE_MENU_ITEM_AUTOMATION_ROOT);
    sendSettingControlReadRequest("K01", DEVICE_MENU_ITEM_AUTOMATION_ROOT);
    sendSettingControlReadRequest("K03", DEVICE_MENU_ITEM_AUTOMATION_ROOT);
    sendSettingControlReadRequest("K06", DEVICE_MENU_ITEM_AUTOMATION_ROOT);
    sendSettingControlReadRequest("K17", DEVICE_MENU_ITEM_AUTOMATION_ROOT);
    sendSettingControlReadRequest("K07", DEVICE_MENU_ITEM_AUTOMATION_ROOT);
    sendSettingReadRequest("K02", "K02", CModBusDataUnit::ReadHoldingRegisters, 2, DEVICE_MENU_ITEM_AUTOMATION_ROOT);
    sendSettingReadRequest("K50", "K50", CModBusDataUnit::ReadHoldingRegisters, 2, DEVICE_MENU_ITEM_AUTOMATION_ROOT);
    sendSettingReadRequest("K04", "K04", CModBusDataUnit::ReadHoldingRegisters, 2, DEVICE_MENU_ITEM_AUTOMATION_ROOT);
    sendSettingReadRequest("K51", "K51", CModBusDataUnit::ReadHoldingRegisters, 2, DEVICE_MENU_ITEM_AUTOMATION_ROOT);
    sendSettingReadRequest("K05", "K05", CModBusDataUnit::ReadHoldingRegisters, 2, DEVICE_MENU_ITEM_AUTOMATION_ROOT);
    sendSettingReadRequest("K08", "K08", CModBusDataUnit::ReadHoldingRegisters, 2, DEVICE_MENU_ITEM_AUTOMATION_ROOT);
    sendSettingReadRequest("K09", "K09", CModBusDataUnit::ReadHoldingRegisters, 2, DEVICE_MENU_ITEM_AUTOMATION_ROOT);
    sendSettingReadRequest("X22", "X22", CModBusDataUnit::ReadHoldingRegisters, 2, DEVICE_MENU_ITEM_AUTOMATION_ROOT);
}
/*!
 * \brief ConfiguratorWindow::automationSwitchTruckRead
 *
 * Чтение автоматика Тележка Выключателя
 */
void ConfiguratorWindow::automationSwitchTruckRead()
{
    sendSettingControlReadRequest("K37", DEVICE_MENU_ITEM_AUTOMATION_ROOT);
    sendSettingControlReadRequest("K41", DEVICE_MENU_ITEM_AUTOMATION_ROOT);
    sendSettingReadRequest("K45", "K49", CModBusDataUnit::ReadHoldingRegisters, 4, DEVICE_MENU_ITEM_AUTOMATION_ROOT);
    sendSettingReadRequest("K58", "K59", CModBusDataUnit::ReadHoldingRegisters, 4, DEVICE_MENU_ITEM_AUTOMATION_ROOT);
}
/*!
 * \brief ConfiguratorWindow::automationBlockRead
 *
 * Чтение автоматика Блокировки
 */
void ConfiguratorWindow::automationBlockRead()
{
    sendSettingControlReadRequest("K13", DEVICE_MENU_ITEM_AUTOMATION_ROOT);
    sendSettingControlReadRequest("K14", DEVICE_MENU_ITEM_AUTOMATION_ROOT);
    sendSettingControlReadRequest("K15", DEVICE_MENU_ITEM_AUTOMATION_ROOT);
}
/*!
 * \brief ConfiguratorWindow::automationBusRead
 *
 * Чтение автоматика Шинный разъединитель
 */
void ConfiguratorWindow::automationBusRead()
{
    sendSettingControlReadRequest("K34", DEVICE_MENU_ITEM_AUTOMATION_ROOT);
    sendSettingControlReadRequest("K38", DEVICE_MENU_ITEM_AUTOMATION_ROOT);
    sendSettingReadRequest("K42", "K46", CModBusDataUnit::ReadHoldingRegisters, 4, DEVICE_MENU_ITEM_AUTOMATION_ROOT);
    sendSettingReadRequest("K52", "K53", CModBusDataUnit::ReadHoldingRegisters, 4, DEVICE_MENU_ITEM_AUTOMATION_ROOT);
}
/*!
 * \brief ConfiguratorWindow::automationLineRead
 *
 * Чтение автоматика Линейные разъединители
 */
void ConfiguratorWindow::automationLineRead()
{
    sendSettingControlReadRequest("K35", DEVICE_MENU_ITEM_AUTOMATION_ROOT);
    sendSettingControlReadRequest("K39", DEVICE_MENU_ITEM_AUTOMATION_ROOT);
    sendSettingReadRequest("K43", "K47", CModBusDataUnit::ReadHoldingRegisters, 4, DEVICE_MENU_ITEM_AUTOMATION_ROOT);
    sendSettingReadRequest("K54", "K55", CModBusDataUnit::ReadHoldingRegisters, 4, DEVICE_MENU_ITEM_AUTOMATION_ROOT);
}
/*!
 * \brief ConfiguratorWindow::automationEarthRead
 *
 * Чтение автоматика Заземляющий разъединитель
 */
void ConfiguratorWindow::automationEarthRead()
{
    sendSettingControlReadRequest("K36", DEVICE_MENU_ITEM_AUTOMATION_ROOT);
    sendSettingControlReadRequest("K40", DEVICE_MENU_ITEM_AUTOMATION_ROOT);
    sendSettingReadRequest("K44", "K48", CModBusDataUnit::ReadHoldingRegisters, 4, DEVICE_MENU_ITEM_AUTOMATION_ROOT);
    sendSettingReadRequest("K56", "K57", CModBusDataUnit::ReadHoldingRegisters, 4, DEVICE_MENU_ITEM_AUTOMATION_ROOT);
}
/*!
 * \brief ConfiguratorWindow::automationDisconnectorsRead
 *
 * Чтение автоматика Разъединители
 */
void ConfiguratorWindow::automationDisconnectorsGroupRead()
{
    automationBusRead();
    automationLineRead();
    automationEarthRead();
}
/*!
 * \brief ConfiguratorWindow::automationCtrlTN
 *
 * Чтение автоматика Контроль ТН
 */
void ConfiguratorWindow::automationCtrlTNRead()
{
    sendSettingControlReadRequest("K18", DEVICE_MENU_ITEM_AUTOMATION_ROOT);
    sendSettingReadRequest("K19", "K19", CModBusDataUnit::ReadHoldingRegisters, 2, DEVICE_MENU_ITEM_AUTOMATION_ROOT);
}
/*!
 * \brief ConfiguratorWindow::automationAVRRead
 *
 * Чтение автоматика АВР
 */
void ConfiguratorWindow::automationAVRRead()
{
    sendSettingControlReadRequest("M81", DEVICE_MENU_ITEM_AUTOMATION_ROOT);
    sendSettingReadRequest("M82", "M85", CModBusDataUnit::ReadHoldingRegisters, 8, DEVICE_MENU_ITEM_AUTOMATION_ROOT);
}
/*!
 * \brief ConfiguratorWindow::automationAPVSignalStartRead
 *
 * Чтение состояний автоматики сигнала пуска
 */
void ConfiguratorWindow::automationAPVSignalStartRead()
{
    int addr = addressSettingKey("M86");

    CModBusDataUnit unit(m_serialPortSettings_window->deviceID(), CModBusDataUnit::ReadHoldingRegisters, addr, QVector<quint16>() << 24);

    unit.setProperty("REQUEST", AUTOMATION_SIGNAL_START);

    m_modbus->sendData(unit);
}
/*!
 * \brief ConfiguratorWindow::automationAPVRead
 *
 * Чтение автоматика АПВ
 */
void ConfiguratorWindow::automationAPVRead()
{
    sendSettingControlReadRequest("M87", DEVICE_MENU_ITEM_AUTOMATION_ROOT);
    sendSettingReadRequest("M88", "M89", CModBusDataUnit::ReadHoldingRegisters, 4, DEVICE_MENU_ITEM_AUTOMATION_ROOT);
}
/*!
 * \brief ConfiguratorWindow::automationGroupRead
 *
 * Чтение группы автоматика
 */
void ConfiguratorWindow::automationGroupRead()
{
    automationSwitchRead(); // автоматики защиты Выключатель
    automationSwitchTruckRead(); // автоматики защиты Тележка выключателя
    automationBlockRead(); // чтение автоматики Блокировки
    automationBusRead(); // чтение автоматики Шинный разъединитель
    automationLineRead(); // чтение автоматики Линейный разъединитель
    automationEarthRead(); // чтение автоматики Заземляющий разъединитель
    automationDisconnectorsGroupRead(); // чтение группы автоматики Разъединители
    automationCtrlTNRead(); // чтение автоматики Контроль ТН
    automationAVRRead(); // чтение автоматики АВР
    automationAPVRead(); // чтение автоматики АПВ
    automationAPVSignalStartRead(); // чтение пусковых сигналов АПВ
    automationAPVSignalStartRead(); // чтение автоматики АПВ сигналы пуска
}
/*!
 * \brief ConfiguratorWindow::calibrationOfCurrentWrite
 *
 * Запись калибровок по току
 */
void ConfiguratorWindow::calibrationOfCurrentWrite()
{
    float Ia   = 0;
    float Ib   = 0;
    float Ic   = 0;
    float _3I0 = 0;

    if(ui->widgetCalibrationOfCurrent->ctrlIa()->isChecked())
        Ia  = ui->widgetCalibrationOfCurrent->calibrationCurrentIa();
    if(ui->widgetCalibrationOfCurrent->ctrlIb()->isChecked())
        Ib = ui->widgetCalibrationOfCurrent->calibrationCurrentIb();
    if(ui->widgetCalibrationOfCurrent->ctrlIc()->isChecked())
        Ic  = ui->widgetCalibrationOfCurrent->calibrationCurrentIc();
    if(ui->widgetCalibrationOfCurrent->ctrl3I0()->isChecked())
        _3I0 = ui->widgetCalibrationOfCurrent->calibrationCurrent3I0();

    if(Ia == 0 && Ib == 0 && Ic == 0 && _3I0 == 0)
        return;

    QString str;
    QString textValue;

    textValue += ((Ia != 0)?QString("Ia = %1\n").arg(QLocale::system().toString(Ia, 'f', 6)):"");
    textValue += ((Ib != 0)?QString("Ib = %1\n").arg(QLocale::system().toString(Ib, 'f', 6)):"");
    textValue += ((Ic != 0)?QString("Ic = %1\n").arg(QLocale::system().toString(Ic, 'f', 6)):"");
    textValue += ((_3I0 != 0)?QString("3I0 = %1\n").arg(QLocale::system().toString(_3I0, 'f', 6)):"");

    str = tr("Вы хотите сохранить новые калибровки?\n%1").arg(textValue);

    int res = QMessageBox::question(this, tr("Запись калибровок по току"), str);

    qInfo() << tr("Запись новых калибровочных коэффициентов по току:\n%1").arg(textValue);

    if(res == QMessageBox::No)
    {
        qInfo() << tr("Отказ пользователя от записи калибровочных коэффициетов по току");
        return;
    }

    QString nameWgt;
    float   t_value;

    if(Ia != 0)
    {
        nameWgt = "lineEditKIA";
        t_value = Ia;
    }
    if(Ib != 0)
    {
        nameWgt = "lineEditKIB";
        t_value = Ib;
    }
    if(Ic != 0)
    {
        nameWgt = "lineEditKIC";
        t_value = Ic;
    }
    if(_3I0 != 0)
    {
        nameWgt = "lineEdit3I0";
        t_value = _3I0;
    }

    CLineEdit* lineEdit = qobject_cast<CLineEdit*>(groupMenuCellWidget(ui->tableWidgetSettingsAnalogGroupGeneral, nameWgt, 1));
    if(lineEdit)
        lineEdit->setText(QLocale::system().toString(t_value, 'f', 6));

    union
    {
        quint16 i[2];
        float   f;
    } value;

    value.f = Ia;
    CModBusDataUnit unit_Ia(m_serialPortSettings_window->deviceID(), CModBusDataUnit::WriteMultipleRegisters,
                            addressSettingKey("KIA"), QVector<quint16>() << value.i[1] << value.i[0]);

    value.f = Ib;
    CModBusDataUnit unit_Ib(m_serialPortSettings_window->deviceID(), CModBusDataUnit::WriteMultipleRegisters,
                            addressSettingKey("KIB"), QVector<quint16>() << value.i[1] << value.i[0]);

    value.f = Ic;
    CModBusDataUnit unit_Ic(m_serialPortSettings_window->deviceID(), CModBusDataUnit::WriteMultipleRegisters,
                            addressSettingKey("KIC"), QVector<quint16>() << value.i[1] << value.i[0]);

    value.f = _3I0;
    CModBusDataUnit unit_3I0(m_serialPortSettings_window->deviceID(), CModBusDataUnit::WriteMultipleRegisters,
                            addressSettingKey("K3I0"), QVector<quint16>() << value.i[1] << value.i[0]);

    if(Ia != 0)
        m_modbus->sendData(unit_Ia);
    if(Ib != 0)
        m_modbus->sendData(unit_Ib);
    if(Ic != 0)
        m_modbus->sendData(unit_Ic);
    if(_3I0!= 0)
        m_modbus->sendData(unit_3I0);

    qInfo() << tr("Запись новых калибровочных коэффициентов по току подтверждена");
}
//----------------------------------------
void ConfiguratorWindow::purposeLedsRead()
{
    sendPurposeReadRequest(tr("LED1"), tr("LED2"));
    sendPurposeReadRequest(tr("LED3"), tr("LED4"));
    sendPurposeReadRequest(tr("LED5"), tr("LED6"));
    sendPurposeReadRequest(tr("LED7"), tr("LED8"));
}
//-----------------------------------------
void ConfiguratorWindow::purposeInputRead()
{
    sendPurposeDIReadRequest(512, 590);
    sendPurposeDIReadRequest(592, 670);
    sendPurposeDIReadRequest(768, 846);
    sendPurposeDIReadRequest(848, 926);
}
//-----------------------------------------
void ConfiguratorWindow::purposeRelayRead()
{
    sendPurposeReadRequest(tr("DO1"), tr("DO2"));
    sendPurposeReadRequest(tr("DO4"), tr("DO5"));
    sendPurposeReadRequest(tr("DO6"), tr("DO7"));
    sendPurposeReadRequest(tr("DO8"), tr("DO9"));
    sendPurposeReadRequest(tr("DO10"), tr("DO11"));
    sendPurposeReadRequest(tr("DO12"), tr("DO13"));
}
//------------------------------------------------
void ConfiguratorWindow::purposeMemoryOutLedRead()
{
    sendRequestRead(0x90C, 8, PURPOSE_OUT_MEMORY_LED_TYPE);
}
//--------------------------------------------------
void ConfiguratorWindow::purposeMemoryOutRelayRead()
{
    sendRequestRead(0x900, 12, PURPOSE_OUT_MEMORY_RELAY_TYPE);
}
//-------------------------------------
void ConfiguratorWindow::dateTimeRead()
{
    CModBusDataUnit unit(m_serialPortSettings_window->deviceID(), CModBusDataUnit::ReadHoldingRegisters, 0x2000,
                         QVector<quint16>() << 4);

    unit.setProperty(tr("REQUEST"), DATETIME_TYPE);

    m_modbus->sendData(unit);
}
/*!
 * \brief ConfiguratorWindow::settingCommunicationsRead
 *
 * Чтение настроек связи
 */
void ConfiguratorWindow::settingCommunicationsRead()
{
    sendRequestRead(0x25, 1, COMMUNICATIONS_MODBUS_ADDRESS);
    sendRequestRead(0x26, 1, COMMUNICATIONS_MODBUS_TIM_REQUEST);
    sendRequestRead(0x27, 1, COMMUNICATIONS_MODBUS_TIM_SPEED);
}
//------------------------------------------------------
void ConfiguratorWindow::processReadJournals(bool state)
{
    if(!m_active_journal_current) // если активынй журнал не выбран, значит ошибка
    {
        QMessageBox::critical(this, tr("Чтение журнала"), tr("Не выбран текущий активный журнал.\nПоробуйте еще раз."));
        return;
    }

    QString key = m_active_journal_current->property("TYPE").toString();

    if(key.isEmpty() || m_journal_set.find(key) == m_journal_set.end())
    {
        return;
    }

    readJournalCount();

    if(!state)
    {
        journal_set_t& set = m_journal_set[key];

        set.message.read_limit = set.message.read_count;

        set.isStop = true;
    }

    journalRead(key);
}
//--------------------------------------
void ConfiguratorWindow::processExport()
{
    DeviceMenuItemType index = menuIndex();

    if(index >= DEVICE_MENU_ITEM_JOURNALS_CRASHES && index <= DEVICE_MENU_ITEM_JOURNALS_ISOLATION)
        exportJournalToDb();
    else if(index == DEVICE_MENU_ITEM_SETTINGS_ITEM_KEYBOARD || index == DEVICE_MENU_ITEM_SETTINGS_ITEM_LEDS ||
            index == DEVICE_MENU_ITEM_SETTINGS_ITEM_IO_MDVV01_INPUTS || index == DEVICE_MENU_ITEM_SETTINGS_ITEM_IO_MDVV01_RELAY ||
            index == DEVICE_MENU_ITEM_SETTINGS_ITEM_IO_MDVV02_INPUTS || index == DEVICE_MENU_ITEM_SETTINGS_ITEM_IO_MDVV02_RELAY)
    {
        exportPurposeToJSON();
    }
    else
        QMessageBox::warning(this, tr("Экспорт"), tr("Не выбран допустимый пункт меню"));
}
//--------------------------------------
void ConfiguratorWindow::processImport()
{
    DeviceMenuItemType index = menuIndex();

    if(index >= DEVICE_MENU_ITEM_JOURNALS_CRASHES && index <= DEVICE_MENU_ITEM_JOURNALS_ISOLATION)
        importJournalToTable();
    else if(index == DEVICE_MENU_ITEM_SETTINGS_ITEM_KEYBOARD || index == DEVICE_MENU_ITEM_SETTINGS_ITEM_LEDS ||
            index == DEVICE_MENU_ITEM_SETTINGS_ITEM_IO_MDVV01_INPUTS || index == DEVICE_MENU_ITEM_SETTINGS_ITEM_IO_MDVV01_RELAY ||
            index == DEVICE_MENU_ITEM_SETTINGS_ITEM_IO_MDVV02_INPUTS || index == DEVICE_MENU_ITEM_SETTINGS_ITEM_IO_MDVV02_RELAY)
    {
        importPurposeFromJSON();
    }
    else
        QMessageBox::warning(this, tr("Импорт"), tr("Не выбран допустимый пункт меню"));
}
//-----------------------------------------------------------
void ConfiguratorWindow::readyReadData(CModBusDataUnit& unit)
{
    if(!unit.isValid())
    {
        noConnectMessage();
        return;
    }

    RequestType type = (RequestType)unit.property(tr("REQUEST")).toInt();

    if(type == CALCULATE_TYPE)
    {
        showErrorMessage(tr("Чтение расчетных величин"), unit);

        RegisterAddress addr = RegisterAddress(unit.property("PART").toInt());

        if(addr == CALCULATE_ADDRESS_PART1)
            m_calculate_buffer = unit.values();
        else if(addr == CALCULATE_ADDRESS_PART2 && !m_calculate_buffer.isEmpty())
        {
            m_calculate_buffer += unit.values();
            displayCalculateValues(m_calculate_buffer);
        }
        else
            m_calculate_buffer.clear();
    }
    else if(type == GENERAL_TYPE)
    {
        showErrorMessage(tr("Чтение уставок"), unit);

        displaySettingResponse(unit);
    }
    else if(type == GENERAL_CONTROL_TYPE)
    {
        showErrorMessage(tr("Чтение уставок"), unit);

        displaySettingControlResponce(unit);
    }
    else if(type == PURPOSE_OUT_TYPE)
    {
        showErrorMessage(tr("Матрица привязок выходов"), unit);

        displayPurposeOutput(unit);
    }
    else if(type == PURPOSE_INPUT_TYPE || type == PURPOSE_INPUT_INVERSE_TYPE)
    {
        static QVector<quint16> input_list;
        static QVector<quint16> input_inverse_list;

        if(type == PURPOSE_INPUT_TYPE)
        {
            showErrorMessage(tr("Матрица привязок входов"), unit);

            if(input_list.isEmpty())
                input_list = unit.values();
            else
                input_list += unit.values();
        }
        else if(type == PURPOSE_INPUT_INVERSE_TYPE)
        {
            showErrorMessage(tr("Матрица привязок инверсий входов"), unit);

            if(input_inverse_list.isEmpty())
                input_inverse_list = unit.values();
            else
            {
                input_inverse_list += unit.values();

                displayPurposeDIResponse(input_list, input_inverse_list);

                input_list.clear();
                input_inverse_list.clear();
            }
        }
    }
    else if(type == READ_JOURNAL || type == READ_JOURNAL_COUNT || type == READ_JOURNAL_SHIFT_PTR)
    {
        processReadJournal(unit);
    }
    else if(type == READ_SERIAL_NUMBER)
    {
        showErrorMessage(tr("Чтение серийного номера"), unit);

        displayDeviceSerialNumber(unit.values());
    }
    else if(type == DATETIME_TYPE)
    {
        showErrorMessage(tr("Чтение настроек Дата/Время"), unit);

        displayDateTime(unit);
    }
    else if(type == PORTECT_RESERVE_SIGNAL_START)
    {
        showErrorMessage(tr("Чтение Защиты/Резервные/Сигналы Пуска"), unit);
        displayProtectReserveSignalStart(unit.values());
    }
    else if(type == AUTOMATION_SIGNAL_START)
    {
        displayAutomationAPVSignalStart(unit.values());
    }
    else if(type == COMMUNICATIONS_MODBUS_TIM_REQUEST)
    {
        showErrorMessage(tr("Чтение времени запроса устройства"), unit);
        displayCommunicationTimeoutRequest(unit.values());
    }
    else if(type == COMMUNICATIONS_MODBUS_TIM_SPEED)
    {
        showErrorMessage(tr("Чтение скорости устройства"), unit);
        displayCommunicationTimeoutSpeed(unit.values());
    }
    else if(type == COMMUNICATIONS_MODBUS_ADDRESS)
    {
        showErrorMessage(tr("Чтение адреса устройства"), unit);
        displayCommunicationAddress(unit.values());
    }
    else if(type == PROTECTION_WORK_MODE_TYPE)
    {
        displayProtectionWorkMode(unit);
    }
    else if(type == MONITONR_PURPOSE_K10_K11_TYPE)
    {
        showErrorMessage(tr("Мониторинг привязок К10/К11"), unit);

        displayMonitorK10_K11(unit);
    }
    else if(type == READ_OUTPUT_ALL)
    {
        showErrorMessage(tr("Чтение состояний все выходы"), unit);

        displayOutputAllRead(unit);
    }
    else if(type == READ_INPUTS)
    {
        showErrorMessage(tr("Чтение состояний входов"), unit);
        displayInputsRead(unit.values());
    }
    else if(type == READ_BLOCK_PROTECTION)
    {
        showErrorMessage(tr("Чтение блокировок защит"), unit);
        displayBlockProtectionRead(unit.values());
    }
    else if(type == READ_DEBUG_INFO)
    {
        showErrorMessage(tr("Чтение отладочной информации"), unit);
        displayDebugInfo(unit);
    }
    else if(type == READ_STATUS_MCP_INFO || type == READ_STATUS_MODULE_INFO)
    {
        displayStatusInfo(unit);
    }
    else if(type == PURPOSE_OUT_MEMORY_LED_TYPE || type == PURPOSE_OUT_MEMORY_RELAY_TYPE)
    {
        displayMemoryOut(unit.values());
    }
    if(type >= CALIBRATION_CURRENT_IA && type <= CALIBRATION_CURRENT_3I0)
    {
        showErrorMessage(tr("Чтение расчетных значений токов"), unit);

        if(unit.count() != 2)
            return;

        union
        {
            quint16 v[2];
            float   f;
        } value;

        value.v[0] = unit[1];
        value.v[1] = unit[0];

        switch(type)
        {
            case CALIBRATION_CURRENT_IA:
                ui->widgetCalibrationOfCurrent->addCalibrationIa(value.f);
                emit ui->widgetCalibrationOfCurrent->dataIncrement();
            break;

            case CALIBRATION_CURRENT_IB:
                ui->widgetCalibrationOfCurrent->addCalibrationIb(value.f);
                if(!ui->widgetCalibrationOfCurrent->ctrlIa()->isChecked())
                    emit ui->widgetCalibrationOfCurrent->dataIncrement();
            break;

            case CALIBRATION_CURRENT_IC:
                ui->widgetCalibrationOfCurrent->addCalibrationIc(value.f);
                if(!ui->widgetCalibrationOfCurrent->ctrlIa()->isChecked() &&
                        !ui->widgetCalibrationOfCurrent->ctrlIb()->isChecked())
                {
                    emit ui->widgetCalibrationOfCurrent->dataIncrement();
                }
            break;

            case CALIBRATION_CURRENT_3I0:
                ui->widgetCalibrationOfCurrent->addCalibration3I0(value.f);
                if(!ui->widgetCalibrationOfCurrent->ctrlIa()->isChecked() &&
                        !ui->widgetCalibrationOfCurrent->ctrlIb()->isChecked() &&
                        !ui->widgetCalibrationOfCurrent->ctrlIc()->isChecked())
                {
                    emit ui->widgetCalibrationOfCurrent->dataIncrement();
                }
            break;

            default: break;
        }
    }
    else if(type >= AMPLITUDE_READ_CH2 && type <= AMPLITUDE_READ_CH5)
    {
        showErrorMessage(tr("Чтение амплитуд по каналам"), unit);

        if(unit.count() != 2)
            return;

        union
        {
            quint16 v[2];
            float   f;
        } value;

        value.v[0] = unit[1];
        value.v[1] = unit[0];

        switch(type)
        {
            case AMPLITUDE_READ_CH2:
                ui->widgetCalibrationOfCurrent->setDeviation3I0(value.f);
            break;

            case AMPLITUDE_READ_CH3:
                ui->widgetCalibrationOfCurrent->setDeviationIa(value.f);
            break;

            case AMPLITUDE_READ_CH4:
                ui->widgetCalibrationOfCurrent->setDeviationIb(value.f);
            break;

            case AMPLITUDE_READ_CH5:
                ui->widgetCalibrationOfCurrent->setDeviationIc(value.f);
            break;

            default: break;
        }
    }
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
    
    m_terminal_window->hide();
    ui->stwgtMain->setCurrentIndex(0);

    m_version_window = new CVersionSoftware(this);
    versionParser();

    ui->tabwgtMenu->setCurrentIndex(TAB_SET_INDEX);
    m_status_bar->connectStateChanged(false);

    ui->tabwgtMenu->setTabEnabled(TAB_READ_WRITE_INDEX, false);
    ui->tabwgtMenu->setTabEnabled(TAB_FILTER_INDEX, false);

    ui->pushButtonJournalRead->setVisible(false);  // скрытие кнопки чтения журналов
    ui->pushButtonJournalClear->setVisible(false); // скрытие кнопки очистки журналов
    ui->pushButtonDefaultSettings->setVisible(false); // скрытие кнопки сброса настроек по умолчанию

    ui->comboBoxCommunicationParity->setCurrentIndex(1);

    ui->splitterCentralWidget->setCollapsible(ui->splitterCentralWidget->indexOf(ui->menuDeviceDockPanel), false);
    ui->splitterCentralWidget->setCollapsible(ui->splitterCentralWidget->indexOf(ui->variableDockPanel), false);
    ui->splitterPanelMessage->setCollapsible(ui->splitterPanelMessage->indexOf(ui->framePanelMessage), false);

    ui->variableDockPanel->setMinimumWidth(ui->pushButtonVariableCtrl->minimumWidth());
    ui->menuDeviceDockPanel->setMinimumWidth(ui->pushButtonMenuDeviceCtrl->minimumWidth());
    ui->framePanelMessage->setMinimumHeight(ui->pushButtonPanelMessage->minimumHeight());

    loadSettings();

    QDateTime dt(QDateTime::currentDateTime());

    ui->dateEdit->setDate(dt.date());
    ui->timeEdit->setTime(dt.time());
    ui->lineEditWeekDay->setText(dt.date().toString("dddd"));

    // управление отображением панелей
    panelVisibleCtrl(ui->centralWgt);
    panelVisibleCtrl(ui->variableDockPanel);
    panelVisibleCtrl(ui->framePanelMessage);

    if(ui->menuDeviceDockPanel->width() == ui->pushButtonMenuDeviceCtrl->minimumWidth())
    {
        ui->pushButtonMenuDeviceCtrl->setState(CDockPanelItemCtrl::Close);
    }
    else
    {
        ui->pushButtonMenuDeviceCtrl->setState(CDockPanelItemCtrl::Open);
    }

    if(ui->variableDockPanel->width() == ui->pushButtonVariableCtrl->minimumWidth())
    {
        ui->pushButtonVariableCtrl->setState(CDockPanelItemCtrl::Close);
    }
    else
    {
        ui->pushButtonVariableCtrl->setState(CDockPanelItemCtrl::Open);
    }

    if(ui->framePanelMessage->height() == ui->pushButtonPanelMessage->minimumHeight())
    {
        ui->pushButtonPanelMessage->setState(CDockPanelItemCtrl::Close);
    }
    else
    {
        ui->pushButtonPanelMessage->setState(CDockPanelItemCtrl::Open);
    }

    if(ui->checkBoxPanelMessage->isChecked())
        ui->framePanelMessage->show();
    else
        ui->framePanelMessage->hide();

    ui->pbtnMenuNewProject->setShortcut(QKeySequence("CTRL+N"));
    ui->pbtnMenuOpenProject->setShortcut(QKeySequence("CTRL+O"));
    ui->pbtnMenuSaveProject->setShortcut(QKeySequence("CTRL+S"));
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
//--------------------------------------------------------
void ConfiguratorWindow::errorDevice(const QString& error)
{
    m_status_bar->setStatusMessage(error, 2000);
}
//---------------------------------------------------------
void ConfiguratorWindow::errorConnect(const QString& error)
{
    QMessageBox::warning(this, tr("Ошибка подлкючения"), error);
}
//---------------------------------------------------
void ConfiguratorWindow::terminalVisiblity(int state)
{
    if(state == Qt::Checked)
        m_terminal_window->show();
    else if(state == Qt::Unchecked)
        m_terminal_window->hide();
    
    ui->chboxTerminal->setCheckState((Qt::CheckState)state);
}
/*!
 * \brief ConfiguratorWindow::indicatorVisiblity
 * \param state Состояние окна индикаторов
 *
 * Управляет видимостью окна отображения состояния индикаторов
 */
void ConfiguratorWindow::indicatorVisiblity(bool state)
{
    if(state)
    {
        sendOutputAllRequest();
        m_output_window->show();
    }
    else
    {
        m_output_window->hide();

        QStringList ledList   = m_output_window->ledList();
        QStringList relayList = m_output_window->relayList();

        QSqlQuery query(m_system_db);

        m_system_db.transaction();

        for(int i = 0; i < ledList.count(); i++)
        {
            query.exec(QString("UPDATE indicator SET name = \'%1\' WHERE row = %2 AND type = \'%3\';").
                       arg(ledList.at(i)).
                       arg(i + 1).
                       arg("LED"));
        }

        for(int i = 0; i < relayList.count(); i++)
        {
            query.exec(QString("UPDATE indicator SET name = \'%1\' WHERE row = %2 AND type = \'%3\';").
                       arg(relayList.at(i)).
                       arg(i + 1).
                       arg("RELAY"));
        }

        m_system_db.commit();
    }
}
//---------------------------------------------------------
void ConfiguratorWindow::monitorK10K11Visiblity(bool state)
{
    if(state)
    {
        sendMonitorPurposeK10_K11Request();
        m_monitor_purpose_window->show();
    }
    else
        m_monitor_purpose_window->hide();

    ui->pushButtonMonitorK10_K11->setChecked(state);
}
//-----------------------------------------------------
void ConfiguratorWindow::outputAllVisiblity(bool state)
{
    if(state)
    {
        sendOutputAllRequest();
        m_outputall_window->show();
    }
    else
        m_outputall_window->hide();
}
//-------------------------------------------------
void ConfiguratorWindow::inputVisiblity(bool state)
{
    if(state)
    {
        sendInputStatesRequest();
        m_inputs_window->show();
    }
    else
        m_inputs_window->hide();
}
//-----------------------------------------------------
void ConfiguratorWindow::debugInfoVisiblity(bool state)
{
    if(state)
    {
        debugInfoRead();
        m_debuginfo_window->show();
    }
    else
    {
        m_debuginfo_window->hide();
    }
}
//------------------------------------------------------
void ConfiguratorWindow::statusInfoVisiblity(bool state)
{
    if(state)
    {
        readStatusInfo();
        m_status_window->show();
    }
    else
    {
        m_status_window->hide();
    }
}
//--------------------------------------------------------------
void ConfiguratorWindow::itemClicked(QTreeWidgetItem* item, int)
{
    if(!item)
        return;

    DeviceMenuItemType type = DeviceMenuItemType(item->type());

    int menu_item = m_menu_items.value(type, -1);

    if(menu_item != -1)
    {
        ui->stwgtMain->setCurrentIndex(menu_item);
        widgetStackIndexChanged(menu_item);
    }
}
//-------------------------------------
void ConfiguratorWindow::readSettings()
{
    if(!m_modbus->channel()->isOpen())
    {
        noConnectMessage();
        return;
    }

    // чтение всех настроек
    inputAnalogGroupRead();
    protectionMTZGroupRead();
    protectionPowerGroupRead();
    protectionDirectedGroupRead();
    protectionFrequencyGroupRead();
    protectionExternalGroupRead();
    protectionMotorGroupRead();
    protectionTemperatureGroupRead();
    protectionReserveGroupRead();
    protectionControlGroupRead();
    automationSwitchRead();
    automationSwitchTruckRead();
    automationBlockRead();
    automationDisconnectorsGroupRead();
    automationCtrlTNRead();
    automationAVRRead();
    automationAPVRead();
    settingCommunicationsRead();
    dateTimeRead();
    purposeLedsRead();
    purposeMemoryOutLedRead();
    purposeRelayRead();
    purposeMemoryOutRelayRead();
    purposeInputRead();
    blockProtectionCtrlRead();
}
//---------------------------------------
void ConfiguratorWindow::readSetCurrent()
{
    if(!m_modbus->channel()->isOpen())
    {
        noConnectMessage();
        return;
    }

    DeviceMenuItemType index = menuIndex();

    switch(index)
    {
        case DEVICE_MENU_ITEM_SETTINGS_ITEM_IN_ANALOG_GENERAL:
            inputAnalogGeneralRead(); // чтение настроек "Основные"
        break;

        case DEVICE_MENU_ITEM_SETTINGS_ITEM_IN_ANALOG_CALIB:
            inputAnalogCalibrateRead(); // чтение настроек "Калибровки"
        break;

        case DEVICE_MENU_ITEM_SETTINGS_ITEM_IN_ANALOG:
            inputAnalogGroupRead(); // чтение группы настроек "Аналоговые"
        break;

        case DEVICE_MENU_PROTECT_ITEM_CURRENT_MTZ1: // чтение защиты МТЗ1
            protectionMTZ1Read();
        break;

        case DEVICE_MENU_PROTECT_ITEM_CURRENT_MTZ2: // чтение защиты МТЗ2
            protectionMTZ2Read();
        break;

        case DEVICE_MENU_PROTECT_ITEM_CURRENT_MTZ3: // чтение защиты МТЗ3
            protectionMTZ3Read();
        break;

        case DEVICE_MENU_PROTECT_ITEM_CURRENT_MTZ3_SET_CHAR: // чтение токовых характеристик защиты МТЗ3
            protectionMTZ3SetCharRead();
        break;

        case DEVICE_MENU_PROTECT_ITEM_CURRENT_MTZ3_PROP_STEEP:
            protectionMTZ3PropertySteepRead();
        break;

        case DEVICE_MENU_PROTECT_ITEM_CURRENT_MTZ3_PROP_SLOP:
            protectionMTZ3PropertySlopRead();
        break;

        case DEVICE_MENU_PROTECT_ITEM_CURRENT_MTZ3_PROP_INV:
            protectionMTZ3PropertyInversionRead();
        break;

        case DEVICE_MENU_PROTECT_ITEM_CURRENT_MTZ3_PROP_DINV:
            protectionMTZ3PropertyDInversionRead();
        break;

        case DEVICE_MENU_PROTECT_ITEM_CURRENT_MTZ3_PROP_BACK:
            protectionMTZ3PropertyBackRead();
        break;

        case DEVICE_MENU_PROTECT_ITEM_CURRENT_MTZ3_PROP_SINV:
            protectionMTZ3PropertyStrongInversionRead();
        break;

        case DEVICE_MENU_PROTECT_ITEM_CURRENT_MTZ3_PROP_EINV:
            protectionMTZ3PropertyExtremalInversionRead();
        break;

        case DEVICE_MENU_PROTECT_ITEM_CURRENT_MTZ4: // чтение защиты По току
            protectionMTZ4Read();
        break;

        case DEVICE_MENU_PROTECT_ITEM_CURRENT: // чтение группы защит МТЗ
            protectionMTZGroupRead();
        break;

        case DEVICE_MENU_PROTECT_ITEM_POWER_UMAX1: // чтение защиты Umax1
            protectionUmax1Read();
        break;

        case DEVICE_MENU_PROTECT_ITEM_POWER_UMAX2: // чтение защиты Umax2
            protectionUmax2Read();
        break;

        case DEVICE_MENU_PROTECT_ITEM_POWER_UMIN1: // чтение защиты Umin1
            protectionUmin1Read();
            automationSwitchRead();
        break;

        case DEVICE_MENU_PROTECT_ITEM_POWER_UMIN2: // чтение защиты Umin2
            protectionUmin2Read();
            automationSwitchRead();
        break;

        case DEVICE_MENU_PROTECT_ITEM_POWER_UMIN1_COREC_KCU: // чтение подпункта защиты Umin1 Коррекция КЦУ
            automationSwitchRead();
        break;

        case DEVICE_MENU_PROTECT_ITEM_POWER_UMIN2_COREC_KCU: // чтение подпункта защиты Umin2 Коррекция КЦУ
            automationSwitchRead();
        break;

        case DEVICE_MENU_PROTECT_ITEM_POWER_3U0: // чтение защиты 3U0
            protection3U0Read();
        break;

        case DEVICE_MENU_PROTECT_ITEM_POWER: // чтение группы защит По напряжению
            protectionPowerGroupRead();
        break;

        case DEVICE_MENU_PROTECT_ITEM_DIRECTED_OZZ1: // чтение защиты ОЗЗ1
            protectionOZZ1Read();
        break;

        case DEVICE_MENU_PROTECT_ITEM_DIRECTED_OZZ2: // чтение защиты ОЗЗ2
            protectionOZZ2Read();
        break;

        case DEVICE_MENU_PROTECT_ITEM_DIRECTED_NZZ1: // чтение защиты НЗЗ1
            protectionNZZ1Read();
        break;

        case DEVICE_MENU_PROTECT_ITEM_DIRECTED_NZZ2: // чтение защиты НЗЗ2
            protectionNZZ2Read();
        break;

        case DEVICE_MENU_PROTECT_ITEM_DIRECTED: // чтение направленных защит
            protectionDirectedGroupRead();
        break;

        case DEVICE_MENU_PROTECT_ITEM_FREQUENCY_ACHR1: // чтение защиты АЧР1
            protectionAchr1Read();
        break;

        case DEVICE_MENU_PROTECT_ITEM_FREQUENCY_ACHR2: // чтение защиты АЧР2
            protectionAchr2Read();
        break;

        case DEVICE_MENU_PROTECT_ITEM_FREQUENCY_ACHR3: // чтение защиты АЧР3
            protectionAchr3Read();
        break;

        case DEVICE_MENU_PROTECT_ITEM_FREQUENCY: // чтение защит по частоте
            protectionFrequencyGroupRead();
        break;

        case DEVICE_MENU_PROTECT_ITEM_EXTERNAL_ARC: // чтение защиты Дуговая
            protectionArcRead();
        break;

        case DEVICE_MENU_PROTECT_ITEM_EXTERNAL_EXT1: // чтение защиты Внешняя1
            protectionExt1Read();
        break;

        case DEVICE_MENU_PROTECT_ITEM_EXTERNAL_EXT2: // чтение защиты Внешняя2
            protectionExt2Read();
        break;

        case DEVICE_MENU_PROTECT_ITEM_EXTERNAL_EXT3: // чтение защиты Внешняя3
            protectionExt3Read();
        break;

        case DEVICE_MENU_PROTECT_ITEM_EXTERNAL: // чтение Внешних защит
            protectionExternalGroupRead();
        break;

        case DEVICE_MENU_PROTECT_ITEM_MOTOR_STARTING: // чтение защиты Пусковая
            protectionStartingRead();
        break;

        case DEVICE_MENU_PROTECT_ITEM_MOTOR_IMIN: // чтение защиты Imin
            protectionIminRead();
        break;

        case DEVICE_MENU_PROTECT_ITEM_MOTOR: // чтение защит для двигателя
            protectionMotorGroupRead();
        break;

        case DEVICE_MENU_PROTECT_ITEM_TEMPERATURE_TEMP1: // чтение защиты Температурная1
            protectionTemp1Read();
        break;

        case DEVICE_MENU_PROTECT_ITEM_TEMPERATURE_TEMP2: // чтение защиты Температурная2
            protectionTemp2Read();
        break;

        case DEVICE_MENU_PROTECT_ITEM_TEMPERATURE: // чтение защит по Температуре
            protectionTemperatureGroupRead();
        break;

        case DEVICE_MENU_PROTECT_ITEM_RESERVE_LEVEL1: // чтение защиты Уровневая1
            protectionLevel1Read();
        break;

        case DEVICE_MENU_PROTECT_ITEM_RESERVE_LEVEL2: // чтение защиты Уровневая2
            protectionLevel2Read();
        break;

        case DEVICE_MENU_PROTECT_ITEM_RESERVE_SIG_START: // чтение защиты Сигнал пуска
            protectionSignalStartRead();
        break;

        case DEVICE_MENU_PROTECT_ITEM_RESERVE: // чтение резервных защит
            protectionReserveGroupRead();
        break;

        case DEVICE_MENU_PROTECT_ITEM_CONTROL_BRU: // чтение защиты БРУ
            protectionBRURead();
        break;

        case DEVICE_MENU_PROTECT_ITEM_CONTROL_VACUUM: // чтение пзащиты Вакуум
            protectionVacuumRead();
        break;

        case DEVICE_MENU_PROTECT_ITEM_CONTROL: // чтение группы защит Предварительного контроля
            protectionControlGroupRead();
        break;

        case DEVICE_MENU_ITEM_AUTOMATION_SWITCH: // автоматики защиты Выключатель
            automationSwitchRead();
        break;

        case DEVICE_MENU_ITEM_AUTOMATION_SWITCH_TRUCK: // автоматики защиты Тележка выключателя
            automationSwitchTruckRead();
        break;

        case DEVICE_MENU_ITEM_AUTOMATION_BLOCKS: // чтение автоматики Блокировки
            automationBlockRead();
        break;

        case DEVICE_MENU_ITEM_AUTOMATION_DISCONNECTORS_BUS: // чтение автоматики Шинный разъединитель
            automationBusRead();
        break;

        case DEVICE_MENU_ITEM_AUTOMATION_DISCONNECTORS_LINE: // чтение автоматики Линейный разъединитель
            automationLineRead();
        break;

        case DEVICE_MENU_ITEM_AUTOMATION_DISCONNECTORS_EARTH: // чтение автоматики Заземляющий разъединитель
            automationEarthRead();
        break;

        case DEVICE_MENU_ITEM_AUTOMATION_DISCONNECTORS: // чтение группы автоматики Разъединители
            automationDisconnectorsGroupRead();
        break;

        case DEVICE_MENU_ITEM_AUTOMATION_CTRL_TN: // чтение автоматики Контроль ТН
            automationCtrlTNRead();
        break;

        case DEVICE_MENU_ITEM_AUTOMATION_AVR: // чтение автоматики АВР
            automationAVRRead();
        break;

        case DEVICE_MENU_ITEM_AUTOMATION_APV: // чтение автоматики АПВ
            automationAPVRead();
            automationAPVSignalStartRead();
        break;

        case DEVICE_MENU_ITEM_AUTOMATION_APV_SIGNAL_START: // чтение автоматики АПВ сигналы пуска
            automationAPVSignalStartRead();
        break;

        case DEVICE_MENU_ITEM_AUTOMATION_ROOT:
            automationGroupRead();
        break;

        case DEVICE_MENU_ITEM_SETTINGS_ITEM_LEDS: // чтение настройки Светодиоды
            purposeLedsRead();
            purposeMemoryOutLedRead();
        break;

        case DEVICE_MENU_ITEM_SETTINGS_ITEM_IO_MDVV01_INPUTS:
            purposeInputRead();
        break;

        case DEVICE_MENU_ITEM_SETTINGS_ITEM_IO_MDVV01_RELAY:
            purposeRelayRead();
            purposeMemoryOutRelayRead();
        break;

        case DEVICE_MENU_ITEM_SETTINGS_ITEM_DATETIME:
            dateTimeRead();
        break;

        case DEVICE_MENU_ITEM_SETTINGS_ITEM_COMMUNICATIONS:
            settingCommunicationsRead();
        break;

        case DEVICE_MENU_ITEM_SETTINGS_ITEM_IO_PROTECTION:
            blockProtectionCtrlRead();
        break;

        default: return;
    }
}
//--------------------------------------
void ConfiguratorWindow::writeSettings()
{
    if(!m_modbus->channel()->isOpen())
    {
        noConnectMessage();
        return;
    }

    int answer = QMessageBox::question(this, tr("Сохранение настроек утройства"),
                                             tr("Вы действительно хотите перезаписать настройки?"),
                                             QMessageBox::Yes | QMessageBox::No);

    if(answer == QMessageBox::No)
        return;

    // запись всех настроек
    inputAnalogGroupWrite();
    protectionMTZGroupWrite();
    protectionPowerGroupWrite();
    protectionDirectedGroupWrite();
    protectionFrequencyGroupWrite();
    protectionExternalGroupWrite();
    protectionMotorGroupWrite();
    protectionTemperatureGroupWrite();
    protectionReserveGroupWrite();
    protectionControlGroupWrite();
    automationSwitchWrite();
    automationSwitchTruckWrite();
    automationBlockWrite();
    automationDisconnectorsGroupWrite();
    automationCtrlTNWrite();
    automationAVRWrite();
    automationAPVWrite();
    dateTimeWrite();
    purposeLedsWrite();
    purposeMemoryOutLedWrite();
    purposeRelayWrite();
    purposeMemoryOutRelayWrite();
    purposeInputWrite();
    blockProtectionCtrlWrite();
    settingCommunicationsWrite();

    sendDeviceCommand(2);
}
//----------------------------------------
void ConfiguratorWindow::writeSetCurrent()
{
    DeviceMenuItemType index = menuIndex();

    switch(index)
    {
        case DEVICE_MENU_ITEM_SETTINGS_ITEM_IN_ANALOG_GENERAL:
            inputAnalogGeneralWrite(); // запись настроек "Основные"
        break;

        case DEVICE_MENU_ITEM_SETTINGS_ITEM_IN_ANALOG_CALIB:
            inputAnalogCalibrateWrite(); // запись настроек "Калибровки"
        break;

        case DEVICE_MENU_ITEM_SETTINGS_ITEM_IN_ANALOG:
            inputAnalogGroupWrite(); // запись группы настроек "Аналоговые"
        break;

        case DEVICE_MENU_PROTECT_ITEM_CURRENT_MTZ1: // запись защиты МТЗ1
            protectionMTZ1Write();
        break;

        case DEVICE_MENU_PROTECT_ITEM_CURRENT_MTZ2: // запись защиты МТЗ2
            protectionMTZ2Write();
        break;

        case DEVICE_MENU_PROTECT_ITEM_CURRENT_MTZ3: // запись защиты МТЗ3
            protectionMTZ3Write();
        break;

        case DEVICE_MENU_PROTECT_ITEM_CURRENT_MTZ3_SET_CHAR: // чтение токовых характеристик защиты МТЗ3
            protectionMTZ3SetCharWrite();
        break;

        case DEVICE_MENU_PROTECT_ITEM_CURRENT_MTZ3_PROP_STEEP:
            protectionMTZ3PropertySteepWrite();
        break;

        case DEVICE_MENU_PROTECT_ITEM_CURRENT_MTZ3_PROP_SLOP:
            protectionMTZ3PropertySlopWrite();
        break;

        case DEVICE_MENU_PROTECT_ITEM_CURRENT_MTZ3_PROP_INV:
            protectionMTZ3PropertyInversionWrite();
        break;

        case DEVICE_MENU_PROTECT_ITEM_CURRENT_MTZ3_PROP_DINV:
            protectionMTZ3PropertyDInversionWrite();
        break;

        case DEVICE_MENU_PROTECT_ITEM_CURRENT_MTZ3_PROP_BACK:
            protectionMTZ3PropertyBackWrite();
        break;

        case DEVICE_MENU_PROTECT_ITEM_CURRENT_MTZ3_PROP_SINV:
            protectionMTZ3PropertyStrongInversionWrite();
        break;

        case DEVICE_MENU_PROTECT_ITEM_CURRENT_MTZ3_PROP_EINV:
            protectionMTZ3PropertyExtremalInversionWrite();
        break;

        case DEVICE_MENU_PROTECT_ITEM_CURRENT_MTZ4: // запись защиты По току
            protectionMTZ4Write();
        break;

        case DEVICE_MENU_PROTECT_ITEM_CURRENT: // запись группы защит МТЗ
            protectionMTZGroupWrite();
        break;

        case DEVICE_MENU_PROTECT_ITEM_POWER_UMAX1: // запись защиты Umax1
            protectionUmax1Write();
        break;

        case DEVICE_MENU_PROTECT_ITEM_POWER_UMAX2: // запись защиты Umax2
            protectionUmax2Write();
        break;

        case DEVICE_MENU_PROTECT_ITEM_POWER_UMIN1: // запись защиты Umin1
            protectionUmin1Write();
            automationSwitchWrite();
        break;

        case DEVICE_MENU_PROTECT_ITEM_POWER_UMIN2: // запись защиты Umin2
            protectionUmin2Write();
            automationSwitchWrite();
        break;

        case DEVICE_MENU_PROTECT_ITEM_POWER_UMIN1_COREC_KCU: // запись подпункта защиты Umin1 Коррекция КЦУ
            automationSwitchWrite();
        break;

        case DEVICE_MENU_PROTECT_ITEM_POWER_UMIN2_COREC_KCU: // запись подпункта защиты Umin2 Коррекция КЦУ
            automationSwitchWrite();
        break;

        case DEVICE_MENU_PROTECT_ITEM_POWER_3U0: // запись защиты 3U0
            protection3U0Write();
        break;

        case DEVICE_MENU_PROTECT_ITEM_POWER: // запись группы защит По напряжению
            protectionPowerGroupWrite();
        break;

        case DEVICE_MENU_PROTECT_ITEM_DIRECTED_OZZ1: // запись защиты ОЗЗ1
            protectionOZZ1Write();
        break;

        case DEVICE_MENU_PROTECT_ITEM_DIRECTED_OZZ2: // запись защиты ОЗЗ2
            protectionOZZ2Write();
        break;

        case DEVICE_MENU_PROTECT_ITEM_DIRECTED_NZZ1: // запись защиты НЗЗ1
            protectionNZZ1Write();
        break;

        case DEVICE_MENU_PROTECT_ITEM_DIRECTED_NZZ2: // запись защиты НЗЗ2
            protectionNZZ2Write();
        break;

        case DEVICE_MENU_PROTECT_ITEM_DIRECTED: // запись направленных защит
            protectionDirectedGroupWrite();
        break;

        case DEVICE_MENU_PROTECT_ITEM_FREQUENCY_ACHR1: // запись защиты АЧР1
            protectionAchr1Write();
        break;

        case DEVICE_MENU_PROTECT_ITEM_FREQUENCY_ACHR2: // запись защиты АЧР2
            protectionAchr2Write();
        break;

        case DEVICE_MENU_PROTECT_ITEM_FREQUENCY_ACHR3: // запись защиты АЧР3
            protectionAchr3Write();
        break;

        case DEVICE_MENU_PROTECT_ITEM_FREQUENCY: // запись защит по частоте
            protectionFrequencyGroupWrite();
        break;

        case DEVICE_MENU_PROTECT_ITEM_EXTERNAL_ARC: // запись защиты Дуговая
            protectionArcWrite();
        break;

        case DEVICE_MENU_PROTECT_ITEM_EXTERNAL_EXT1: // запись защиты Внешняя1
            protectionExt1Write();
        break;

        case DEVICE_MENU_PROTECT_ITEM_EXTERNAL_EXT2: // запись защиты Внешняя2
            protectionExt2Write();
        break;

        case DEVICE_MENU_PROTECT_ITEM_EXTERNAL_EXT3: // запись защиты Внешняя3
            protectionExt3Write();
        break;

        case DEVICE_MENU_PROTECT_ITEM_EXTERNAL: // запись Внешних защит
            protectionExternalGroupWrite();
        break;

        case DEVICE_MENU_PROTECT_ITEM_MOTOR_STARTING: // запись защиты Пусковая
            protectionStartingWrite();
        break;

        case DEVICE_MENU_PROTECT_ITEM_MOTOR_IMIN: // запись защиты Imin
            protectionIminWrite();
        break;

        case DEVICE_MENU_PROTECT_ITEM_MOTOR: // запись защит для двигателя
            protectionMotorGroupWrite();
        break;

        case DEVICE_MENU_PROTECT_ITEM_TEMPERATURE_TEMP1: // запись защиты Температурная1
            protectionTemp1Write();
        break;

        case DEVICE_MENU_PROTECT_ITEM_TEMPERATURE_TEMP2: // запись защиты Температурная2
            protectionTemp2Write();
        break;

        case DEVICE_MENU_PROTECT_ITEM_TEMPERATURE: // запись защит по Температуре
            protectionTemperatureGroupWrite();
        break;

        case DEVICE_MENU_PROTECT_ITEM_RESERVE_LEVEL1: // запись защиты Уровневая1
            protectionLevel1Write();
        break;

        case DEVICE_MENU_PROTECT_ITEM_RESERVE_LEVEL2: // запись защиты Уровневая2
            protectionLevel2Write();
        break;

        case DEVICE_MENU_PROTECT_ITEM_RESERVE_SIG_START: // запись защиты Сигнал пуска
            protectionSignalStartWrite();
        break;

        case DEVICE_MENU_PROTECT_ITEM_RESERVE: // запись резервных защит
            protectionReserveGroupWrite();
        break;

        case DEVICE_MENU_PROTECT_ITEM_CONTROL_BRU: // запись защиты БРУ
            protectionBRUWrite();
        break;

        case DEVICE_MENU_PROTECT_ITEM_CONTROL_VACUUM: // запись пзащиты Вакуум
            protectionVacuumWrite();
        break;

        case DEVICE_MENU_PROTECT_ITEM_CONTROL: // запись группы защит Предварительного контроля
            protectionControlGroupWrite();
        break;

        case DEVICE_MENU_ITEM_AUTOMATION_SWITCH: // запись автоматики Выключатель
            automationSwitchWrite();
        break;

        case DEVICE_MENU_ITEM_AUTOMATION_SWITCH_TRUCK: // запись автоматики Тележка выключателя
            automationSwitchTruckWrite();
        break;

        case DEVICE_MENU_ITEM_AUTOMATION_BLOCKS: // запись автоматики Блокировки
            automationBlockWrite();
        break;

        case DEVICE_MENU_ITEM_AUTOMATION_DISCONNECTORS_BUS: // запись автоматики Шинный разъединитель
            automationBusWrite();
        break;

        case DEVICE_MENU_ITEM_AUTOMATION_DISCONNECTORS_LINE: // запись автоматики Линейный разъединитель
            automationLineWrite();
        break;

        case DEVICE_MENU_ITEM_AUTOMATION_DISCONNECTORS_EARTH: // запись автоматики Заземляющий разъединитель
            automationEarthWrite();
        break;

        case DEVICE_MENU_ITEM_AUTOMATION_DISCONNECTORS: // запись группы автоматики Разъединители
            automationDisconnectorsGroupWrite();
        break;

        case DEVICE_MENU_ITEM_AUTOMATION_CTRL_TN: // запись автоматики Контроль ТН
            automationCtrlTNWrite();
        break;

        case DEVICE_MENU_ITEM_AUTOMATION_AVR: // запись автоматики АВР
            automationAVRWrite();
        break;

        case DEVICE_MENU_ITEM_AUTOMATION_APV: // запись автоматики АПВ
            automationAPVWrite();
            automationAPVSignalStartWrite();
        break;

        case DEVICE_MENU_ITEM_AUTOMATION_APV_SIGNAL_START: // чтение автоматики АПВ сигналы пуска
            automationAPVSignalStartWrite();
        break;

        case DEVICE_MENU_ITEM_SETTINGS_ITEM_LEDS: // запись настройки Светодиоды
            purposeLedsWrite();
            purposeMemoryOutLedWrite();
        break;

        case DEVICE_MENU_ITEM_SETTINGS_ITEM_IO_MDVV01_INPUTS: // запись настройки Дискретные входы
            purposeInputWrite();
        break;

        case DEVICE_MENU_ITEM_SETTINGS_ITEM_IO_MDVV01_RELAY: // запись настройки Реле
            purposeRelayWrite();
            purposeMemoryOutRelayWrite();
        break;

        case DEVICE_MENU_ITEM_SETTINGS_ITEM_DATETIME: // запись настройки дата/время
            dateTimeWrite();
        break;

        case DEVICE_MENU_ITEM_SETTINGS_ITEM_COMMUNICATIONS:
            settingCommunicationsWrite();
        break;

        case DEVICE_MENU_ITEM_SETTINGS_ITEM_IO_PROTECTION:
            blockProtectionCtrlWrite();
        break;

        default: return;
    }

    sendDeviceCommand(2);
}
/*!
 * \brief ConfiguratorWindow::expandItemTree
 * \param state Состояние кнопки разворачивания
 *
 * Разворачивает и сворачивает дерево меню устройства
 */
void ConfiguratorWindow::expandItemTree(bool state)
{
    if(state)
    {
        ui->treewgtDeviceMenu->expandAll();
        ui->tbntExpandItems->setIcon(QIcon(":/images/resource/images/branch_open.png"));
        ui->tbntExpandItems->setToolTip(tr("Свернуть меню"));
    }
    else
    {
        ui->treewgtDeviceMenu->collapseAll();
        ui->tbntExpandItems->setIcon(QIcon(":/images/resource/images/branch_close.png"));
        ui->tbntExpandItems->setToolTip(tr("Развернуть меню"));
    }
}
//-----------------------------------------
void ConfiguratorWindow::versionSowftware()
{
    m_version_window->show();
}
//---------------------------------------------------------------
void ConfiguratorWindow::sendCalculateRead(CModBusDataUnit& unit)
{
    m_modbus->sendData(unit);
}
//--------------------------------------
void ConfiguratorWindow::initMenuPanel()
{
    QTreeWidgetItem* itemProtections = new QTreeWidgetItem(ui->treewgtDeviceMenu, DEVICE_MENU_ITEM_PROTECTION_ROOT); // Защиты
    QTreeWidgetItem* itemAutomation  = new QTreeWidgetItem(ui->treewgtDeviceMenu, DEVICE_MENU_ITEM_AUTOMATION_ROOT); // Автоматика
    QTreeWidgetItem* itemJournals    = new QTreeWidgetItem(ui->treewgtDeviceMenu, DEVICE_MENU_ITEM_JOURNALS_ROOT); // Журналы
    QTreeWidgetItem* itemMeasures    = new QTreeWidgetItem(ui->treewgtDeviceMenu, DEVICE_MENU_ITEM_MEASURES_ROOT); // Измерения
    QTreeWidgetItem* itemSettings    = new QTreeWidgetItem(ui->treewgtDeviceMenu, DEVICE_MENU_ITEM_SETTINGS_ROOT); // Настройки

    itemProtections->setText(0, tr("Защиты"));
    itemAutomation->setText(0, tr("Автоматика"));
    itemJournals->setText(0, tr("Журналы"));
    itemMeasures->setText(0, tr("Измерения"));
    itemSettings->setText(0, tr("Настройки"));

    // ЗАЩИТЫ
    QTreeWidgetItem* protectItemCurrent     = new QTreeWidgetItem(itemProtections, QStringList() << tr("По току"),
                                                                  DEVICE_MENU_PROTECT_ITEM_CURRENT); // по току
    QTreeWidgetItem* protectItemPower       = new QTreeWidgetItem(itemProtections, QStringList() << tr("По напряжению"),
                                                                  DEVICE_MENU_PROTECT_ITEM_POWER); // по напряжению
    QTreeWidgetItem* protectItemDirected    = new QTreeWidgetItem(itemProtections, QStringList() << tr("Направленные"),
                                                                  DEVICE_MENU_PROTECT_ITEM_DIRECTED); // направленные
    QTreeWidgetItem* protectItemFrequency   = new QTreeWidgetItem(itemProtections, QStringList() << tr("По частоте"),
                                                                  DEVICE_MENU_PROTECT_ITEM_FREQUENCY); // по частоте
    QTreeWidgetItem* protectItemExternal    = new QTreeWidgetItem(itemProtections, QStringList() << tr("Внешние"),
                                                                  DEVICE_MENU_PROTECT_ITEM_EXTERNAL); // внешние
    QTreeWidgetItem* protectItemMotor       = new QTreeWidgetItem(itemProtections, QStringList() << tr("Для двигателя"),
                                                                  DEVICE_MENU_PROTECT_ITEM_MOTOR); // для двигателя
    QTreeWidgetItem* protectItemTemperature = new QTreeWidgetItem(itemProtections, QStringList() << tr("По температуре"),
                                                                  DEVICE_MENU_PROTECT_ITEM_TEMPERATURE); // по температуре
    QTreeWidgetItem* protectItemReserve     = new QTreeWidgetItem(itemProtections, QStringList() << tr("Резервные"),
                                                                  DEVICE_MENU_PROTECT_ITEM_RESERVE); // резервные
    QTreeWidgetItem* protectItemControl     = new QTreeWidgetItem(itemProtections, QStringList() << tr("Предварительного контроля"),
                                                                  DEVICE_MENU_PROTECT_ITEM_CONTROL); // предварительного контроля

    itemProtections->addChildren(QList<QTreeWidgetItem*>() << protectItemCurrent << protectItemPower << protectItemDirected <<
                                                              protectItemFrequency << protectItemExternal << protectItemMotor <<
                                                              protectItemTemperature << protectItemReserve <<protectItemControl);
    // пункты защиты "по току"
//    QTreeWidgetItem* currentItemMTZ1 = new QTreeWidgetItem(protectItemCurrent, QStringList() << tr("МТЗ1"),
//                                                           DEVICE_MENU_PROTECT_ITEM_CURRENT_MTZ1); // защита МТЗ1
//    QTreeWidgetItem* currentItemMTZ2 = new QTreeWidgetItem(protectItemCurrent, QStringList() << tr("МТЗ2"),
//                                                           DEVICE_MENU_PROTECT_ITEM_CURRENT_MTZ2); // защита МТЗ2
//    QTreeWidgetItem* currentItemMTZ3 = new QTreeWidgetItem(protectItemCurrent, QStringList() << tr("МТЗ3"),
//                                                           DEVICE_MENU_PROTECT_ITEM_CURRENT_MTZ3); // защита МТЗ3

//    QTreeWidgetItem* currentItemMTZ3_set_character = new QTreeWidgetItem(currentItemMTZ3,
//                                                                         QStringList() << tr("Настройка характеристик"),
//                                                                         DEVICE_MENU_PROTECT_ITEM_CURRENT_MTZ3_SET_CHAR);

//    currentItemMTZ3->addChild(currentItemMTZ3_set_character);

//    QTreeWidgetItem* currentItemMTZ3_charact_steep   = new QTreeWidgetItem(currentItemMTZ3_set_character,
//                                                                           QStringList() << tr("Крутая"),
//                                                                           DEVICE_MENU_PROTECT_ITEM_CURRENT_MTZ3_PROP_STEEP);
//    QTreeWidgetItem* currentItemMTZ3_charact_sloping = new QTreeWidgetItem(currentItemMTZ3_set_character,
//                                                                           QStringList() << tr("Пологая"),
//                                                                           DEVICE_MENU_PROTECT_ITEM_CURRENT_MTZ3_PROP_SLOP);
//    QTreeWidgetItem* currentItemMTZ3_charact_inverse = new QTreeWidgetItem(currentItemMTZ3_set_character,
//                                                                           QStringList() << tr("Инверсная"),
//                                                                           DEVICE_MENU_PROTECT_ITEM_CURRENT_MTZ3_PROP_INV);
//    QTreeWidgetItem* currentItemMTZ3_charact_dur_inv = new QTreeWidgetItem(currentItemMTZ3_set_character,
//                                                                           QStringList() << tr("Длительно инверсная"),
//                                                                           DEVICE_MENU_PROTECT_ITEM_CURRENT_MTZ3_PROP_DINV);
//    QTreeWidgetItem* currentItemMTZ3_charact_back    = new QTreeWidgetItem(currentItemMTZ3_set_character,
//                                                                           QStringList() << tr("Обратно зависимая"),
//                                                                           DEVICE_MENU_PROTECT_ITEM_CURRENT_MTZ3_PROP_BACK);
//    QTreeWidgetItem* currentItemMTZ3_charact_str_inv = new QTreeWidgetItem(currentItemMTZ3_set_character,
//                                                                           QStringList() << tr("Сильно инверсная"),
//                                                                           DEVICE_MENU_PROTECT_ITEM_CURRENT_MTZ3_PROP_SINV);
//    QTreeWidgetItem* currentItemMTZ3_charact_ext_inv = new QTreeWidgetItem(currentItemMTZ3_set_character,
//                                                                           QStringList() << tr("Экстремально инверсная"),
//                                                                           DEVICE_MENU_PROTECT_ITEM_CURRENT_MTZ3_PROP_EINV);

//    currentItemMTZ3_set_character->addChildren(QList<QTreeWidgetItem*>() << currentItemMTZ3_charact_steep <<
//                                                                            currentItemMTZ3_charact_sloping <<
//                                                                            currentItemMTZ3_charact_inverse <<
//                                                                            currentItemMTZ3_charact_dur_inv <<
//                                                                            currentItemMTZ3_charact_back <<
//                                                                            currentItemMTZ3_charact_str_inv <<
//                                                                            currentItemMTZ3_charact_ext_inv);

//    QTreeWidgetItem* currentItemMTZ4 = new QTreeWidgetItem(protectItemCurrent, QStringList() << tr("МТЗ4"),
//                                                           DEVICE_MENU_PROTECT_ITEM_CURRENT_MTZ4); // защита МТЗ4

//    protectItemCurrent->addChildren(QList<QTreeWidgetItem*>() << currentItemMTZ1 << currentItemMTZ2 << currentItemMTZ3 <<
//                                                                 currentItemMTZ4);

//    // пункты защиты "по напряжению"
//    QTreeWidgetItem* powerItemUmax1 = new QTreeWidgetItem(protectItemPower, QStringList() << tr("Umax1"),
//                                                          DEVICE_MENU_PROTECT_ITEM_POWER_UMAX1); // защита Umax1
//    QTreeWidgetItem* powerItemUmax2 = new QTreeWidgetItem(protectItemPower, QStringList() << tr("Umax2"),
//                                                          DEVICE_MENU_PROTECT_ITEM_POWER_UMAX2); // защита Umax2
//    QTreeWidgetItem* powerItemUmin1 = new QTreeWidgetItem(protectItemPower, QStringList() << tr("Umin1"),
//                                                          DEVICE_MENU_PROTECT_ITEM_POWER_UMIN1); // защита Umin1
//    QTreeWidgetItem* powerItemUmin2 = new QTreeWidgetItem(protectItemPower, QStringList() << tr("Umin2"),
//                                                          DEVICE_MENU_PROTECT_ITEM_POWER_UMIN2); // защита Umin2
//    QTreeWidgetItem* powerItem3U0   = new QTreeWidgetItem(protectItemPower, QStringList() << tr("3U0"),
//                                                          DEVICE_MENU_PROTECT_ITEM_POWER_3U0); // защита 3U0

//    // подпункт защиты Umin1 "Коррекция КЦУ" - переход на вкладку Автоматика/Выключатель
//    QTreeWidgetItem* powerItemUmin1CorecKCU = new QTreeWidgetItem(powerItemUmin1, QStringList() << tr("Корр КЦУ"),
//                                                                  DEVICE_MENU_PROTECT_ITEM_POWER_UMIN1_COREC_KCU);
//    // подпункт защиты Umin2 "Коррекция КЦУ" - переход на вкладку Автоматика/Выключатель
//    QTreeWidgetItem* powerItemUmin2CorecKCU = new QTreeWidgetItem(powerItemUmin2, QStringList() << tr("Корр КЦУ"),
//                                                                  DEVICE_MENU_PROTECT_ITEM_POWER_UMIN2_COREC_KCU);

//    powerItemUmin1->addChild(powerItemUmin1CorecKCU);
//    powerItemUmin2->addChild(powerItemUmin2CorecKCU);
//    protectItemPower->addChildren(QList<QTreeWidgetItem*>() << powerItemUmax1 << powerItemUmax2 << powerItemUmin1 <<
//                                                               powerItemUmin2 << powerItem3U0);

//    // пункты защиты "направленные"
//    QTreeWidgetItem* directItemOZZ1 = new QTreeWidgetItem(protectItemDirected, QStringList() << tr("ОЗЗ1"),
//                                                          DEVICE_MENU_PROTECT_ITEM_DIRECTED_OZZ1); // защита ОЗЗ1
//    QTreeWidgetItem* directItemOZZ2 = new QTreeWidgetItem(protectItemDirected, QStringList() << tr("ОЗЗ2"),
//                                                          DEVICE_MENU_PROTECT_ITEM_DIRECTED_OZZ2); // защита ОЗЗ2
//    QTreeWidgetItem* directItemNZZ1 = new QTreeWidgetItem(protectItemDirected, QStringList() << tr("НЗЗ1"),
//                                                          DEVICE_MENU_PROTECT_ITEM_DIRECTED_NZZ1); // защита НЗЗ1
//    QTreeWidgetItem* directItemNZZ2 = new QTreeWidgetItem(protectItemDirected, QStringList() << tr("НЗЗ2"),
//                                                          DEVICE_MENU_PROTECT_ITEM_DIRECTED_NZZ2); // защита НЗЗ2

//    protectItemDirected->addChildren(QList<QTreeWidgetItem*>() << directItemOZZ1 << directItemOZZ2 << directItemNZZ1 <<
//                                                                  directItemNZZ2);

//    // пункты защиты "по частоте"
//    QTreeWidgetItem* freqItemACHR1 = new QTreeWidgetItem(protectItemFrequency, QStringList() << tr("АЧР1"),
//                                                         DEVICE_MENU_PROTECT_ITEM_FREQUENCY_ACHR1); // защита АЧР1
//    QTreeWidgetItem* freqItemACHR2 = new QTreeWidgetItem(protectItemFrequency, QStringList() << tr("АЧР2"),
//                                                         DEVICE_MENU_PROTECT_ITEM_FREQUENCY_ACHR2); // защита АЧР2
//    QTreeWidgetItem* freqItemACHR3 = new QTreeWidgetItem(protectItemFrequency, QStringList() << tr("АЧР3"),
//                                                         DEVICE_MENU_PROTECT_ITEM_FREQUENCY_ACHR3); // защита АЧР3

//    protectItemFrequency->addChildren(QList<QTreeWidgetItem*>() << freqItemACHR1 << freqItemACHR2 << freqItemACHR3);

//    // пункты защиты "внешние"
//    QTreeWidgetItem* extItemARC       = new QTreeWidgetItem(protectItemExternal, QStringList() << tr("Дуговая"),
//                                                            DEVICE_MENU_PROTECT_ITEM_EXTERNAL_ARC); // защита Дуговая
//    QTreeWidgetItem* extItemExternal1 = new QTreeWidgetItem(protectItemExternal, QStringList() << tr("Внешняя1"),
//                                                            DEVICE_MENU_PROTECT_ITEM_EXTERNAL_EXT1); // защита Внешняя1
//    QTreeWidgetItem* extItemExternal2 = new QTreeWidgetItem(protectItemExternal, QStringList() << tr("Внешняя2"),
//                                                            DEVICE_MENU_PROTECT_ITEM_EXTERNAL_EXT2); // защита Внешняя2
//    QTreeWidgetItem* extItemExternal3 = new QTreeWidgetItem(protectItemExternal, QStringList() << tr("Внешняя3"),
//                                                            DEVICE_MENU_PROTECT_ITEM_EXTERNAL_EXT3); // защита Внешняя3

//    protectItemExternal->addChildren(QList<QTreeWidgetItem*>() << extItemARC << extItemExternal1 << extItemExternal2 <<
//                                                                  extItemExternal3);

//    // пункты защиты "для двигателя"
//    QTreeWidgetItem* motorItemStarting = new QTreeWidgetItem(protectItemMotor, QStringList() << tr("Пусковая"),
//                                                             DEVICE_MENU_PROTECT_ITEM_MOTOR_STARTING); // защита Пусковая
//    QTreeWidgetItem* motorItemImin     = new QTreeWidgetItem(protectItemMotor, QStringList() << tr("Imin"),
//                                                             DEVICE_MENU_PROTECT_ITEM_MOTOR_IMIN); // защита Imin

//    protectItemMotor->addChildren(QList<QTreeWidgetItem*>() << motorItemStarting << motorItemImin);

//    // пункты защиты "по температуре"
//    QTreeWidgetItem* tempItemTemperature1 = new QTreeWidgetItem(protectItemTemperature, QStringList() << tr("Температурная1"),
//                                                                DEVICE_MENU_PROTECT_ITEM_TEMPERATURE_TEMP1); // защита Температурная1
//    QTreeWidgetItem* tempItemTemperature2 = new QTreeWidgetItem(protectItemTemperature, QStringList() << tr("Температурная2"),
//                                                                DEVICE_MENU_PROTECT_ITEM_TEMPERATURE_TEMP2); // защита Температурная2

//    protectItemTemperature->addChildren(QList<QTreeWidgetItem*>() << tempItemTemperature1 << tempItemTemperature2);

//    // пункты защиты "резервные"
//    QTreeWidgetItem* reserveItemLevel1      = new QTreeWidgetItem(protectItemReserve, QStringList() << tr("Уров1"),
//                                                                  DEVICE_MENU_PROTECT_ITEM_RESERVE_LEVEL1); // защита Уров1
//    QTreeWidgetItem* reserveItemLevel2      = new QTreeWidgetItem(protectItemReserve, QStringList() << tr("Уров2"),
//                                                                  DEVICE_MENU_PROTECT_ITEM_RESERVE_LEVEL2); // защита Уров2
//    QTreeWidgetItem* reserveItemSignalStart = new QTreeWidgetItem(protectItemReserve, QStringList() << tr("Сигнал пуска"),
//                                                                  DEVICE_MENU_PROTECT_ITEM_RESERVE_SIG_START); // защита Сигнал пуска

//    protectItemReserve->addChildren(QList<QTreeWidgetItem*>() << reserveItemLevel1 << reserveItemLevel2 << reserveItemSignalStart);

//    // пункты защиты "предварительного контроля"
//    QTreeWidgetItem* ctrlItemBRU    = new QTreeWidgetItem(protectItemControl, QStringList() << tr("БРУ"),
//                                                          DEVICE_MENU_PROTECT_ITEM_CONTROL_BRU); // защита БРУ
//    QTreeWidgetItem* ctrlItemVacuum = new QTreeWidgetItem(protectItemControl, QStringList() << tr("Вакууум"),
//                                                          DEVICE_MENU_PROTECT_ITEM_CONTROL_VACUUM); // защита Вакуум

//    protectItemControl->addChildren(QList<QTreeWidgetItem*>() << ctrlItemBRU << ctrlItemVacuum);

//    // АВТОМАТИКА
//    QTreeWidgetItem* automationSwitch        = new QTreeWidgetItem(itemAutomation, QStringList() << tr("Выключатель"),
//                                                                   DEVICE_MENU_ITEM_AUTOMATION_SWITCH); // автоматика Выключатель
//    QTreeWidgetItem* automationSwitchTruck   = new QTreeWidgetItem(itemAutomation, QStringList() << tr("Тележка выключателя"),
//                                                                   DEVICE_MENU_ITEM_AUTOMATION_SWITCH_TRUCK); // автоматика Выключатель
//    QTreeWidgetItem* automationBlocks        = new QTreeWidgetItem(itemAutomation, QStringList() << tr("Блокировки"),
//                                                                   DEVICE_MENU_ITEM_AUTOMATION_BLOCKS); // автоматика Выключатель
//    QTreeWidgetItem* automationDisconnectors = new QTreeWidgetItem(itemAutomation, QStringList() << tr("Разъединители"),
//                                                                   DEVICE_MENU_ITEM_AUTOMATION_DISCONNECTORS); // автоматика Выключатель

//    QTreeWidgetItem* automationDisconnectorsBus   = new QTreeWidgetItem(automationDisconnectors,
//                                                                        QStringList() << tr("Шинный разъединитель"),
//                                                                        DEVICE_MENU_ITEM_AUTOMATION_DISCONNECTORS_BUS);
//    QTreeWidgetItem* automationDisconnectorsLine  = new QTreeWidgetItem(automationDisconnectors,
//                                                                        QStringList() << tr("Линейный разъединитель"),
//                                                                        DEVICE_MENU_ITEM_AUTOMATION_DISCONNECTORS_LINE);
//    QTreeWidgetItem* automationDisconnectorsEarth = new QTreeWidgetItem(automationDisconnectors,
//                                                                        QStringList() << tr("Заземляющий разъединитель"),
//                                                                        DEVICE_MENU_ITEM_AUTOMATION_DISCONNECTORS_EARTH);

//    protectItemControl->addChildren(QList<QTreeWidgetItem*>() << automationDisconnectorsBus << automationDisconnectorsLine <<
//                                                                 automationDisconnectorsEarth);

//    QTreeWidgetItem* automationCtrlTN = new QTreeWidgetItem(itemAutomation, QStringList() << tr("Контроль ТН"),
//                                                                   DEVICE_MENU_ITEM_AUTOMATION_CTRL_TN); // автоматика Выключатель
//    QTreeWidgetItem* automationAVR    = new QTreeWidgetItem(itemAutomation, QStringList() << tr("АВР"),
//                                                                   DEVICE_MENU_ITEM_AUTOMATION_AVR); // автоматика Выключатель
//    QTreeWidgetItem* automationAPV    = new QTreeWidgetItem(itemAutomation, QStringList() << tr("АПВ"),
//                                                                   DEVICE_MENU_ITEM_AUTOMATION_APV); // автоматика Выключатель

//    QTreeWidgetItem* automationAPVSignalStart = new QTreeWidgetItem(automationAPV, QStringList() << tr("АПВ сигналы пуска"),
//                                                                    DEVICE_MENU_ITEM_AUTOMATION_APV_SIGNAL_START);

//    automationAPV->addChild(automationAPVSignalStart);

//    itemAutomation->addChildren(QList<QTreeWidgetItem*>() << automationSwitch << automationSwitchTruck << automationBlocks <<
//                                                             automationDisconnectors << automationCtrlTN << automationAVR <<
//                                                             automationAPV);

    // ЖУРНАЛЫ
    QTreeWidgetItem* journalCrash     = new QTreeWidgetItem(itemJournals, QStringList() << tr("Аварий"),
                                                            DEVICE_MENU_ITEM_JOURNALS_CRASHES);
    QTreeWidgetItem* journalEvents    = new QTreeWidgetItem(itemJournals, QStringList() << tr("Событий"),
                                                            DEVICE_MENU_ITEM_JOURNALS_EVENTS);
    QTreeWidgetItem* journalHalfHour  = new QTreeWidgetItem(itemJournals, QStringList() << tr("Получасовок"),
                                                            DEVICE_MENU_ITEM_JOURNALS_HALF_HOURS);
    QTreeWidgetItem* journalIsolation = new QTreeWidgetItem(itemJournals, QStringList() << tr("Изоляции"),
                                                            DEVICE_MENU_ITEM_JOURNALS_ISOLATION);

    itemAutomation->addChildren(QList<QTreeWidgetItem*>() << journalCrash << journalEvents << journalHalfHour << journalIsolation);

    // ИЗМЕРЕНИЯ
    QTreeWidgetItem* measureInputs = new QTreeWidgetItem(itemMeasures,
                                                         QStringList() << tr("Напряжения и токи на измерительных входах"),
                                                         DEVICE_MENU_ITEM_MEASURES_INPUTS);

    itemMeasures->addChild(measureInputs);

    // НАСТРОЙКИ
    QTreeWidgetItem* settingInputAnalog    = new QTreeWidgetItem(itemSettings, QStringList() << tr("Аналоговые входы"),
                                                                 DEVICE_MENU_ITEM_SETTINGS_ITEM_IN_ANALOG);
    QTreeWidgetItem* settingCommunications = new QTreeWidgetItem(itemSettings, QStringList() << tr("Связь"),
                                                                 DEVICE_MENU_ITEM_SETTINGS_ITEM_COMMUNICATIONS);
    QTreeWidgetItem* settingDateTime       = new QTreeWidgetItem(itemSettings, QStringList() << tr("Дата и время"),
                                                                 DEVICE_MENU_ITEM_SETTINGS_ITEM_DATETIME);
    QTreeWidgetItem* settingKeyboard       = new QTreeWidgetItem(itemSettings, QStringList() << tr("Клавиатура"),
                                                                 DEVICE_MENU_ITEM_SETTINGS_ITEM_KEYBOARD);
    QTreeWidgetItem* settingLeds           = new QTreeWidgetItem(itemSettings, QStringList() << tr("Светодиоды"),
                                                                 DEVICE_MENU_ITEM_SETTINGS_ITEM_LEDS);
    QTreeWidgetItem* settingIO             = new QTreeWidgetItem(itemSettings, QStringList() << tr("Входы и выходы"),
                                                                 DEVICE_MENU_ITEM_SETTINGS_ITEM_IO);

    itemSettings->addChildren(QList<QTreeWidgetItem*>() << settingInputAnalog << settingCommunications << settingDateTime <<
                                                           settingKeyboard << settingLeds << settingIO);

    // пункты настройки "Аналоговые входы"
//    QTreeWidgetItem* inputAnalogGeneral     = new QTreeWidgetItem(settingInputAnalog, QStringList() << tr("Основные"),
//                                                                  DEVICE_MENU_ITEM_SETTINGS_ITEM_IN_ANALOG_GENERAL);
//    QTreeWidgetItem* inputAnalogCalibration = new QTreeWidgetItem(settingInputAnalog, QStringList() << tr("Калибровка"),
//                                                                  DEVICE_MENU_ITEM_SETTINGS_ITEM_IN_ANALOG_CALIB);

//    settingInputAnalog->addChildren(QList<QTreeWidgetItem*>() << inputAnalogGeneral << inputAnalogCalibration);

    // пункты настройки "входы и выходы"
//    QTreeWidgetItem* ioMDVV01 = new QTreeWidgetItem(settingIO, QStringList() << tr("МДВВ-01"),
//                                                    DEVICE_MENU_ITEM_SETTINGS_ITEM_IO_MDVV01);
//    QTreeWidgetItem* ioMDVV02 = new QTreeWidgetItem(settingIO, QStringList() << tr("МДВВ-02"),
//                                                    DEVICE_MENU_ITEM_SETTINGS_ITEM_IO_MDVV02);

//    settingIO->addChildren(QList<QTreeWidgetItem*>() << ioMDVV01 << ioMDVV02);

    QTreeWidgetItem* ioRelayMDVV01    = new QTreeWidgetItem(settingIO, QStringList() << tr("Реле"),
                                                            DEVICE_MENU_ITEM_SETTINGS_ITEM_IO_MDVV01_RELAY);
    QTreeWidgetItem* ioDSInputMDVV01  = new QTreeWidgetItem(settingIO, QStringList() << tr("Дискретные входы"),
                                                            DEVICE_MENU_ITEM_SETTINGS_ITEM_IO_MDVV01_INPUTS);
    QTreeWidgetItem* ioProtectionCtrl = new QTreeWidgetItem(settingIO, QStringList() << tr("Управление защитами"),
                                                            DEVICE_MENU_ITEM_SETTINGS_ITEM_IO_PROTECTION);

    settingIO->addChildren(QList<QTreeWidgetItem*>() << ioRelayMDVV01 << ioDSInputMDVV01 << ioProtectionCtrl);

//    QTreeWidgetItem* ioRelayMDVV02   = new QTreeWidgetItem(ioMDVV02, QStringList() << tr("Реле"),
//                                                           DEVICE_MENU_ITEM_SETTINGS_ITEM_IO_MDVV02_RELAY);
//    QTreeWidgetItem* ioDSInputMDVV02 = new QTreeWidgetItem(ioMDVV02, QStringList() << tr("Дискретные входы"),
//                                                           DEVICE_MENU_ITEM_SETTINGS_ITEM_IO_MDVV02_INPUTS);

//    ioMDVV02->addChildren(QList<QTreeWidgetItem*>() << ioRelayMDVV02 << ioDSInputMDVV02);

    ui->treewgtDeviceMenu->addTopLevelItem(itemProtections);
    ui->treewgtDeviceMenu->addTopLevelItem(itemAutomation);
    ui->treewgtDeviceMenu->addTopLevelItem(itemJournals);
    ui->treewgtDeviceMenu->addTopLevelItem(itemMeasures);
    ui->treewgtDeviceMenu->addTopLevelItem(itemSettings);

    // заполнение карты меню устройства для доступа к настройкам при клике по пункту
    m_menu_items[DEVICE_MENU_ITEM_PROTECTION_ROOT] = 10000;
    m_menu_items[DEVICE_MENU_ITEM_AUTOMATION_ROOT] = 10000;
    m_menu_items[DEVICE_MENU_ITEM_JOURNALS_ROOT]   = 10000;
    m_menu_items[DEVICE_MENU_ITEM_MEASURES_ROOT]   = 10000;
    m_menu_items[DEVICE_MENU_ITEM_SETTINGS_ROOT]   = 10000;

    m_menu_items[DEVICE_MENU_PROTECT_ITEM_POWER]       = 10000;
    m_menu_items[DEVICE_MENU_PROTECT_ITEM_DIRECTED]    = 10000;
    m_menu_items[DEVICE_MENU_PROTECT_ITEM_FREQUENCY]   = 10000;
    m_menu_items[DEVICE_MENU_PROTECT_ITEM_EXTERNAL]    = 10000;
    m_menu_items[DEVICE_MENU_PROTECT_ITEM_MOTOR ]      = 10000;
    m_menu_items[DEVICE_MENU_PROTECT_ITEM_TEMPERATURE] = 10000;
    m_menu_items[DEVICE_MENU_PROTECT_ITEM_RESERVE]     = 10000;
    m_menu_items[DEVICE_MENU_PROTECT_ITEM_CONTROL]     = 10000;

    m_menu_items[DEVICE_MENU_ITEM_SETTINGS_ITEM_IN_ANALOG]         = 0;
    m_menu_items[DEVICE_MENU_PROTECT_ITEM_CURRENT]                 = 1;
    m_menu_items[DEVICE_MENU_PROTECT_ITEM_POWER]                   = 2;
    m_menu_items[DEVICE_MENU_PROTECT_ITEM_DIRECTED]                = 3;
    m_menu_items[DEVICE_MENU_PROTECT_ITEM_FREQUENCY]               = 4;
    m_menu_items[DEVICE_MENU_PROTECT_ITEM_EXTERNAL]                = 5;
    m_menu_items[DEVICE_MENU_PROTECT_ITEM_MOTOR]                   = 6;
    m_menu_items[DEVICE_MENU_PROTECT_ITEM_TEMPERATURE]             = 7;
    m_menu_items[DEVICE_MENU_PROTECT_ITEM_RESERVE]                 = 8;
    m_menu_items[DEVICE_MENU_PROTECT_ITEM_CONTROL]                 = 9;
    m_menu_items[DEVICE_MENU_ITEM_AUTOMATION_ROOT]                 = 10;
    m_menu_items[DEVICE_MENU_ITEM_JOURNALS_CRASHES]                = 11;
    m_menu_items[DEVICE_MENU_ITEM_JOURNALS_EVENTS]                 = 12;
    m_menu_items[DEVICE_MENU_ITEM_JOURNALS_HALF_HOURS]             = 13;
    m_menu_items[DEVICE_MENU_ITEM_JOURNALS_ISOLATION]              = 14;
    m_menu_items[DEVICE_MENU_ITEM_MEASURES_INPUTS]                 = 15;
    m_menu_items[DEVICE_MENU_ITEM_SETTINGS_ITEM_COMMUNICATIONS]    = 16;
    m_menu_items[DEVICE_MENU_ITEM_SETTINGS_ITEM_DATETIME]          = 17;
    m_menu_items[DEVICE_MENU_ITEM_SETTINGS_ITEM_KEYBOARD]          = 18;
    m_menu_items[DEVICE_MENU_ITEM_SETTINGS_ITEM_LEDS]              = 19;
    m_menu_items[DEVICE_MENU_ITEM_SETTINGS_ITEM_IO_MDVV01_INPUTS]  = 20;
    m_menu_items[DEVICE_MENU_ITEM_SETTINGS_ITEM_IO_MDVV01_RELAY]   = 21;
    m_menu_items[DEVICE_MENU_ITEM_SETTINGS_ITEM_IO_PROTECTION]     = 23;
    m_menu_items[DEVICE_MENU_ITEM_PROTECTION_ROOT]                 = 1; // при открытии меню Защиты открывается группа МТЗ

    QStringList columns = QStringList() << tr("Имя") << tr("Параметр") << tr("Предел");

    ui->tableWidgetSettingsAnalogGroupGeneral->setColumns(columns);
    ui->tableWidgetProtectionGroupMTZ->setColumns(columns);
    ui->tableWidgetProtectionGroupPower->setColumns(columns);
    ui->tableWidgetProtectionGroupDirect->setColumns(columns);
    ui->tableWidgetProtectionGroupFrequency->setColumns(columns);
    ui->tableWidgetProtectionGroupExternal->setColumns(columns);
    ui->tableWidgetProtectionGroupMotor->setColumns(columns);
    ui->tableWidgetProtectionGroupTemperature->setColumns(columns);
    ui->tableWidgetProtectionGroupReserve->setColumns(columns);
    ui->tableWidgetProtectionGroupControl->setColumns(columns);
    ui->tableWidgetAutomationGroup->setColumns(columns);

    CDeviceMenuTableWidget::group_t group;

    // группа аналоговые основные
    group = loadMenuGroup(tr("Основные"));
    ui->tableWidgetSettingsAnalogGroupGeneral->addGroup(group);
    group = loadMenuGroup(tr("Калибровка"));
    ui->tableWidgetSettingsAnalogGroupGeneral->addGroup(group);

    // группа по току
    group = loadMenuGroup(tr("МТЗ1"));
    ui->tableWidgetProtectionGroupMTZ->addGroup(group);
    group = loadMenuGroup(tr("МТЗ2"));
    ui->tableWidgetProtectionGroupMTZ->addGroup(group);
    group = loadMenuGroup(tr("МТЗ3"));
    ui->tableWidgetProtectionGroupMTZ->addGroup(group);
    group = loadMenuGroup(tr("МТЗ4"));
    ui->tableWidgetProtectionGroupMTZ->addGroup(group);

    // группа по напряжению
    group = loadMenuGroup(tr("Umax1"));
    ui->tableWidgetProtectionGroupPower->addGroup(group);
    group = loadMenuGroup(tr("Umax2"));
    ui->tableWidgetProtectionGroupPower->addGroup(group);
    group = loadMenuGroup(tr("Umin1"));
    ui->tableWidgetProtectionGroupPower->addGroup(group);
    group = loadMenuGroup(tr("Umin2"));
    ui->tableWidgetProtectionGroupPower->addGroup(group);
    group = loadMenuGroup(tr("3U0"));
    ui->tableWidgetProtectionGroupPower->addGroup(group);

    // группа направленные
    group = loadMenuGroup(tr("ОЗЗ1"));
    ui->tableWidgetProtectionGroupDirect->addGroup(group);
    group = loadMenuGroup(tr("ОЗЗ2"));
    ui->tableWidgetProtectionGroupDirect->addGroup(group);
    group = loadMenuGroup(tr("НЗЗ1"));
    ui->tableWidgetProtectionGroupDirect->addGroup(group);
    group = loadMenuGroup(tr("НЗЗ2"));
    ui->tableWidgetProtectionGroupDirect->addGroup(group);

    // группа по частоте
    group = loadMenuGroup(tr("АЧР1"));
    ui->tableWidgetProtectionGroupFrequency->addGroup(group);
    group = loadMenuGroup(tr("АЧР2"));
    ui->tableWidgetProtectionGroupFrequency->addGroup(group);
    group = loadMenuGroup(tr("АЧР3"));
    ui->tableWidgetProtectionGroupFrequency->addGroup(group);

    // группа внешние
    group = loadMenuGroup(tr("Дуговая"));
    ui->tableWidgetProtectionGroupExternal->addGroup(group);
    group = loadMenuGroup(tr("Внешняя1"));
    ui->tableWidgetProtectionGroupExternal->addGroup(group);
    group = loadMenuGroup(tr("Внешняя2"));
    ui->tableWidgetProtectionGroupExternal->addGroup(group);
    group = loadMenuGroup(tr("Внешняя3"));
    ui->tableWidgetProtectionGroupExternal->addGroup(group);

    // группа по двигателям
    group = loadMenuGroup(tr("Пусковая"));
    ui->tableWidgetProtectionGroupMotor->addGroup(group);
    group = loadMenuGroup(tr("Imin"));
    ui->tableWidgetProtectionGroupMotor->addGroup(group);

    // группа по температуре
    group = loadMenuGroup(tr("Температурная1"));
    ui->tableWidgetProtectionGroupTemperature->addGroup(group);
    group = loadMenuGroup(tr("Температурная2"));
    ui->tableWidgetProtectionGroupTemperature->addGroup(group);

    // группа резервные
    group = loadMenuGroup(tr("Уров1"));
    ui->tableWidgetProtectionGroupReserve->addGroup(group);
    group = loadMenuGroup(tr("Уров2"));
    ui->tableWidgetProtectionGroupReserve->addGroup(group);
    group = loadMenuGroup(tr("Сигнал пуска"));
    ui->tableWidgetProtectionGroupReserve->addGroup(group);

    // группа предварительного контроля
    group = loadMenuGroup(tr("БРУ"));
    ui->tableWidgetProtectionGroupControl->addGroup(group);
    group = loadMenuGroup(tr("Вакуум"));
    ui->tableWidgetProtectionGroupControl->addGroup(group);

    // группа автоматики
    group = loadMenuGroup(tr("Выключатель"));
    ui->tableWidgetAutomationGroup->addGroup(group);
    group = loadMenuGroup(tr("Тележка выключателя"));
    ui->tableWidgetAutomationGroup->addGroup(group);
    group = loadMenuGroup(tr("Блокировки"));
    ui->tableWidgetAutomationGroup->addGroup(group);
    group = loadMenuGroup(tr("Шинный разъединитель"));
    ui->tableWidgetAutomationGroup->addGroup(group);
    group = loadMenuGroup(tr("Линейный разъединитель"));
    ui->tableWidgetAutomationGroup->addGroup(group);
    group = loadMenuGroup(tr("Заземляющий разъединитель"));
    ui->tableWidgetAutomationGroup->addGroup(group);
    group = loadMenuGroup(tr("Контроль ТН"));
    ui->tableWidgetAutomationGroup->addGroup(group);
    group = loadMenuGroup(tr("АВР"));
    ui->tableWidgetAutomationGroup->addGroup(group);
    group = loadMenuGroup(tr("АПВ"));
    ui->tableWidgetAutomationGroup->addGroup(group);

    // формирование связей между отдельными ячейками
    // Объединение ячеек Датчик1 и Датчик2 Температуры1 с датчиками Температуры2
    QComboBox* cboxTemp1 = qobject_cast<QComboBox*>(groupMenuCellWidget(ui->tableWidgetProtectionGroupTemperature,
                                                                        QString("comboBoxM65"), 1));
    QComboBox* cboxTemp2 = qobject_cast<QComboBox*>(groupMenuCellWidget(ui->tableWidgetProtectionGroupTemperature,
                                                                        QString("comboBoxM66"), 1));
    QComboBox* cboxTemp1_1 = qobject_cast<QComboBox*>(groupMenuCellWidget(ui->tableWidgetProtectionGroupTemperature,
                                                                          QString("comboBoxM65_1"), 1));
    QComboBox* cboxTemp2_1 = qobject_cast<QComboBox*>(groupMenuCellWidget(ui->tableWidgetProtectionGroupTemperature,
                                                                          QString("comboBoxM66_1"), 1));

    if(cboxTemp1 && cboxTemp1_1)
    {
        connect(cboxTemp1, SIGNAL(currentIndexChanged(int)), cboxTemp1_1, SLOT(setCurrentIndex(int)));
        connect(cboxTemp1_1, SIGNAL(currentIndexChanged(int)), cboxTemp1, SLOT(setCurrentIndex(int)));
    }

    if(cboxTemp2 && cboxTemp2_1)
    {
        connect(cboxTemp2, SIGNAL(currentIndexChanged(int)), cboxTemp2_1, SLOT(setCurrentIndex(int)));
        connect(cboxTemp2_1, SIGNAL(currentIndexChanged(int)), cboxTemp2, SLOT(setCurrentIndex(int)));
    }

    // Объединение ячеек Управление Уроверь1 с Управлением Уровень2
    QComboBox* cboxLevel1 = qobject_cast<QComboBox*>(groupMenuCellWidget(ui->tableWidgetProtectionGroupReserve,
                                                                         QString("comboBoxM77"), 1));
    QComboBox* cboxLevel2 = qobject_cast<QComboBox*>(groupMenuCellWidget(ui->tableWidgetProtectionGroupReserve,
                                                                         QString("comboBoxM77_1"), 1));

    if(cboxLevel1 && cboxLevel2)
    {
        connect(cboxLevel1, SIGNAL(currentIndexChanged(int)), cboxLevel2, SLOT(setCurrentIndex(int)));
        connect(cboxLevel2, SIGNAL(currentIndexChanged(int)), cboxLevel1, SLOT(setCurrentIndex(int)));
    }
}
//-------------------------------------
void ConfiguratorWindow::initCellBind()
{
    QSqlQuery query(m_system_db);

    if(!query.exec("SELECT * FROM iodevice WHERE type = 'SET' OR type = 'MODIFY';"))
        return;

    while(query.next())
    {
        QString key          = query.value("key").toString();
        QString name         = query.value("description").toString();
        int     addr         = query.value("address").toInt();
        float   limit_min    = query.value("limit_min").toFloat();
        float   limit_max    = query.value("limit_max").toFloat();
        QString unit_meadure = query.value("unit_measure").toString();
        QString date_type    = query.value("date_type").toString();
        int     row          = query.value("row").toInt();

        m_cell_list.append(qMakePair(key, cell_t({ addr, name, limit_min, limit_max, unit_meadure, date_type, row })));
        m_limits[key] = cell_t({ addr, name, limit_min, limit_max, unit_meadure, date_type, row });
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
    QSqlQuery query("SELECT key, bit FROM variable;");

    if(query.exec())
    {
        while(query.next())
        {
            QString key = query.value("key").toString();
            int     bit = query.value("bit").toInt();

            m_variable_bits[key] = bit;
        }
    }

    group_t group = createVariableGroup("LED");

    if(!group.isEmpty())
    {
        QVector<QPair<QString, QString> > labels = loadLabelColumns("LED");
        initTable(ui->tablewgtLedPurpose, labels, group);
    }

    group = createVariableGroup("DO");

    if(!group.isEmpty())
    {
        QVector<QPair<QString, QString> > labels = loadLabelColumns("RELAY");
        initTable(ui->tablewgtRelayPurpose, labels, group);
    }

    group = createVariableGroup("DI");

    if(!group.isEmpty())
    {
        QVector<QPair<QString, QString> > labels = loadLabelColumns("INPUT");
        initTable(ui->tablewgtDiscreteInputPurpose, labels, group);
    }

    m_block_list = loadProtectionList();

    if(!m_block_list.isEmpty())
    {
        initTableProtection(ui->tablewgtProtectionCtrl, m_block_list);
    }
    else
        qWarning() << tr("Ошибка чтения привязок для таблицы Управления Защитами");
}
//-----------------------------------------
void ConfiguratorWindow::initEventJournal()
{
    if(!m_system_db.isOpen())
        return;

    // инициализация списка журнала событий
    QSqlQuery query(m_system_db);

    if(!query.exec("SELECT code, name FROM event_t;"))
    {
        qWarning() << tr("Ошибка чтения типов журнала событий: %1").arg(query.lastError().text());
        return;
    }

    QVector<event_t> events;

    while(query.next())
    {
        events << event_t({ query.value("code").toInt(), query.value("name").toString(), QVector<event_t>() });
    }

    if(events.isEmpty())
        return;

    for(event_t& event: events)
    {
        if(!query.exec("SELECT code, name FROM event_category WHERE event_t = " + QString::number(event.code) + ";"))
        {
            qWarning() << tr("Ошибка чтения категорий журнала событий: %1").arg(query.lastError().text());
            return;
        }

        while(query.next())
        {
            event.sub_event << event_t({ query.value("code").toInt(), query.value("name").toString(), QVector<event_t>() });
        }
    }

    for(event_t& event: events)
    {
        if(event.sub_event.isEmpty())
            continue;

        for(event_t& category: event.sub_event)
        {
            if(!query.exec("SELECT code, name FROM event_parameter WHERE event_category = " +
                           QString::number(category.code) + ";"))
            {
                qWarning() << tr("Ошибка чтения параметров журнала событий: %1").arg(query.lastError().text());
                return;
            }

            while(query.next())
            {
                category.sub_event << event_t({ query.value("code").toInt(), query.value("name").toString(),
                                                                             QVector<event_t>() });
            }
        }
    }

    QVariant event_variant = QVariant::fromValue(events);
    ui->widgetJournalEvent->setJournalDescription(event_variant);
}
//-----------------------------------------
void ConfiguratorWindow::initCrashJournal()
{
    if(!m_system_db.isOpen())
        return;

    QSqlQuery query(m_system_db);
    QString   title_msg = tr("Загрузка настроек защиты из БД");

    // Загружаем список защит
    if(!query.exec(QString("SELECT code, name FROM protection;")))
    {
        QMessageBox::warning(this, title_msg, tr("Не удалось загрузить список доступных защит: %1").arg(query.lastError().text()));
        return;
    }

    protection_list_item list_item;
    protection_list_set  list_set;

    while(query.next())
    {
        int     code         = query.value("code").toInt();
        QString name_protect = query.value("name").toString();

        QSqlQuery query_item(m_system_db);

        // Загружаем переменные списка защит по коду защиты
        if(!query_item.exec(QString("SELECT name, index_var, type, first FROM protection_items WHERE protect_id=%1").arg(code)))
        {
            QMessageBox::warning(this, title_msg, tr("Не удалось загрузить переменные защит: %1").arg(query_item.lastError().text()));
            return;
        }

        QVector<protection_item_t> items;

        while(query_item.next())
        {
            QString name      = query_item.value("name").toString();
            QString index_var = query_item.value("index_var").toString();
            QString type      = query_item.value("type").toString();
            int     first     = query_item.value("first").toInt();

            items << protection_item_t({ name, index_var, type, first });
        }

        list_item.insert(code, qMakePair(name_protect, items));
    }

    // Перебираем список защит и ищем переменные с типом LIST (список)
    for(int key: list_item.keys())
    {
        QPair<QString, QVector<protection_item_t> > pair = list_item[key];

        for(const protection_item_t& item: pair.second)
        {
            if(item.type == "LIST") // тип переменной является списком
            {
                if(!query.exec(QString("SELECT number FROM protection_set WHERE index_var=\"%1\";").arg(item.index)))
                {
                    QMessageBox::warning(this, title_msg, tr("Не удалось загрузить перечень списов вариантов переменных типа LIST: %1").
                                         arg(query.lastError().text()));
                    break;
                }

                while(query.next())
                {
                    int number = query.value("number").toInt();

                    QSqlQuery query_set(m_system_db);

                    QVector<QString> set_list;

                    if(!query_set.exec(QString("SELECT name FROM protection_set_list WHERE set_number=%1;").arg(number)))
                    {
                        QMessageBox::warning(this, title_msg, tr("Не удалось загрузить варианты переменных типа LIST: %1").
                                                              arg(query_set.lastError().text()));
                        break;
                    }

                    while(query_set.next())
                    {
                        QString name = query_set.value("name").toString();

                        set_list << name;
                    }

                    list_set.insert(item.index, set_list);
                }
            }
        }
    }

    // загружаем список внутренних переменных (358 шт)
    if(!query.exec("SELECT key, name, description FROM variable;"))
    {
        QMessageBox::warning(this, title_msg, tr("Не удалось загрузить список внутренних переменных: %1").arg(query.lastError().text()));
        return;
    }

    variable_list_t variable_list;

    while(query.next())
    {
        variable_list << variable_t({ query.value("key").toString(), query.value("name").toString(),
                                                                     query.value("description").toString() });
    }

    // загружаем выходы (светодиоды, реле и модифицируемые переменные)
    if(!query.exec("SELECT key, description FROM iodevice WHERE type=\"RELAY\" OR type=\"LED\" OR type=\"EMPTY\";"))
    {
        QMessageBox::warning(this, title_msg, tr("Не удалось загрузить список выходов и модифицируемых переменных: %1").
                                              arg(query.lastError().text()));
        return;
    }

    io_list_t out_list;

    while(query.next())
    {
        out_list << io_t({ query.value("key").toString(), query.value("description").toString() });
    }

    // загружаем входы
    if(!query.exec("SELECT key, description FROM iodevice WHERE type=\"INPUT\";"))
    {
        QMessageBox::warning(this, title_msg, tr("Не удалось загрузить список входов: %1").arg(query.lastError().text()));
        return;
    }

    io_list_t input_list;

    while(query.next())
    {
        input_list << io_t({ query.value("key").toString(), query.value("description").toString() });
    }

    // загружаем список расчетных величин из БД
    if(!query.exec("SELECT name, first, description FROM calc_value;"))
    {
        QMessageBox::warning(this, title_msg, tr("Не удалось загрузить список расчетных величин: %1").arg(query.lastError().text()));
        return;
    }

    calc_value_list_t calc_value_list;

    while(query.next())
    {
        calc_value_list << calc_value_t({ query.value("name").toString(), query.value("first").toInt(),
                                          query.value("description").toString() });
    }

    ui->variableWidget->setVariableNames(calc_value_list);

    protection_t protection = { list_item, list_set, variable_list, out_list, input_list, calc_value_list };

    ui->widgetJournalCrash->setJournalDescription(QVariant::fromValue(protection));
}
//--------------------------------------------
void ConfiguratorWindow::initHalfhourJournal()
{
    QSqlQuery query_halfhour(m_system_db);

    halfhour_labels_t columns, rows;

    if(query_halfhour.exec("SELECT * FROM halfhour;"))
    {
        while(query_halfhour.next())
        {
//            int     id   = query_halfhour.value("id").toInt();
            QString name = query_halfhour.value("name").toString();
            QString meas = query_halfhour.value("measure").toString();

            columns << halfhour_item_t({ name, meas });
        }
    }
    else
        qWarning() << tr("Ошибка чтения переменных halfhour: %1").arg(query_halfhour.lastError().text());

    if(query_halfhour.exec(QString("SELECT * FROM halfhour_var;")))
    {
        while(query_halfhour.next())
        {
            QString name        = query_halfhour.value("name").toString();
            QString description = query_halfhour.value("description").toString();

            rows << halfhour_item_t({ name, description });
        }
    }
    else
        qWarning() << tr("Ошибка чтения значений halfhour_val: %1").arg(query_halfhour.lastError().text());

    if(!columns.isEmpty() && !rows.isEmpty())
    {
        ui->widgetJournalHalfHour->setHalfhourHeaders(columns, rows);
        ui->widgetJournalHalfHour->setProperty("COLUMNS", QVariant::fromValue(columns));
        ui->widgetJournalHalfHour->setProperty("ROWS", QVariant::fromValue(rows));
    }
}
//---------------------------------------
void ConfiguratorWindow::initDeviceCode()
{
    QSqlQuery query(m_system_db);

    if(!query.exec("SELECT code, name FROM device_code;"))
    {
        qWarning() << tr("Ошибка чтения кодов устройств: %1").arg(query.lastError().text());
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
    QStringList eventJournalHeaders     = QStringList() << tr("ID") << tr("Дата") << tr("Время") <<
                                                           tr("Тип") << tr("Категория") << tr("Параметр");
    QStringList crashJournalHeaders     = QStringList() << tr("ID") << tr("Дата") << tr("Время") <<
                                                           tr("Защита");
    QStringList halfHourJournalHeaders  = QStringList() << tr("ID") << tr("Дата") << tr("Время") <<
                                                           tr("Тип записи") << tr("Время учета, сек");
    QStringList isolationJournalHeaders = QStringList() << tr("ID") << tr("Дата") << tr("Время");

    ui->widgetJournalCrash->setProperty("NAME", tr("аварий"));
    ui->widgetJournalEvent->setProperty("NAME", tr("событий"));
    ui->widgetJournalHalfHour->setProperty("NAME", tr("получасовок"));
    ui->widgetJournalIsolation->setProperty("NAME", tr("изоляций"));

    ui->widgetJournalCrash->setProperty("TYPE", tr("CRASH"));
    ui->widgetJournalEvent->setProperty("TYPE", tr("EVENT"));
    ui->widgetJournalHalfHour->setProperty("TYPE", tr("HALFHOUR"));
    ui->widgetJournalIsolation->setProperty("TYPE", tr("ISOLATION"));

    ui->widgetJournalCrash->setTableHeaders(CJournalWidget::CRASH_PROPERTY, crashJournalHeaders);
    ui->widgetJournalEvent->setTableHeaders(CJournalWidget::EVENT_PROPERTY, eventJournalHeaders);
    ui->widgetJournalHalfHour->setTableHeaders(CJournalWidget::HALFHOUR_PROPERTY, halfHourJournalHeaders);
    ui->widgetJournalIsolation->setTableHeaders(CJournalWidget::ISOLATION_PROPERTY, isolationJournalHeaders);

    QVector<int> length_list = QVector<int>() << 50 << 100 << 100 << 100 << 200 << 300;

    ui->widgetJournalCrash->setTableColumnWidth(QVector<int>() << 75 << 100 << 100 << 175);
    ui->widgetJournalEvent->setTableColumnWidth(length_list);
    ui->widgetJournalHalfHour->setTableColumnWidth(length_list);
    ui->widgetJournalIsolation->setTableColumnWidth(length_list);

    ui->widgetJournalCrash->setVisibleProperty(CJournalWidget::CRASH_PROPERTY, true);
    ui->widgetJournalHalfHour->setVisibleProperty(CJournalWidget::HALFHOUR_PROPERTY , true);

    m_journal_set["CRASH"]    = journal_set_t({ 0, 0, false, false, journal_address_t({ 0x26, 0x3011, 0x2000 }),
                                                journal_message_t({ 1, 0, 0, 0, 0, 0, 256 }), QVector<quint16>()});
    m_journal_set["EVENT"]    = journal_set_t({ 0, 0, false, false, journal_address_t({ 0x22, 0x300C, 0x1000 }),
                                                journal_message_t({ 8, 0, 0, 0, 0, 0, 16 }), QVector<quint16>()});
    m_journal_set["HALFHOUR"] = journal_set_t({ 0, 0, false, false, journal_address_t({ 0x2A, 0x3016, 0x5000 }),
                                                journal_message_t({ 2, 0, 0, 0, 0, 0, 64 }), QVector<quint16>()});
}
//-------------------------------------------
void ConfiguratorWindow::initProtectionList()
{
    QSqlQuery query(m_system_db);

    if(query.exec("SELECT * FROM protection;"))
    {
        while(query.next())
        {
            int     code     = query.value("code").toInt();
            QString var_name = query.value("var_name").toString();
            QString key      = query.value("key").toString();
            QString name     = query.value("name").toString();

            m_protections[key] = protection_group_t({ code, var_name, name });
        }
    }
}
//-------------------------------------------
void ConfiguratorWindow::initMonitorPurpose()
{
    QSqlQuery query(m_system_db);

    QStringList rows, columns;

    if(query.exec("SELECT * FROM variable;"))
    {
        while(query.next())
        {
            QString key  = query.value("key").toString();
            QString name = query.value("name").toString();

            if(key.toUpper() == "K10" || key.toUpper() == "K11")
                rows << QString("%1 (%2)").arg(key).arg(name);

            QString str = QString("%1 (%2)").arg(key).arg(name);

            if(str.count() > 22)
            {
                str  = str.remove(22, str.count() - 22);
                str += "...)";
            }

            columns << str;
        }
    }

    m_monitor_purpose_window->setHeaders(rows, columns);
}
//--------------------------------------
void ConfiguratorWindow::initOutputAll()
{
    ui->pushButtonOutputAll->setProperty("TYPE", "OUTPUTALL");

    QSqlQuery query(m_system_db);

    QStringList list;

    if(query.exec("SELECT description FROM iodevice WHERE type=\'RELAY\';"))
    {
        while(query.next())
        {
            list << query.value("description").toString();
        }
    }

    if(query.exec("SELECT description FROM iodevice WHERE type=\'LED\';"))
    {
        while(query.next())
        {
            list << query.value("description").toString();
        }
    }

    if(query.exec("SELECT key, description FROM iodevice WHERE type=\'MODIFY\';"))
    {
        while(query.next())
        {
            list << QString("%1 (%2)").arg(query.value("key").toString()).arg(query.value("description").toString());
        }
    }

    m_outputall_window->createList(list);
}
//-----------------------------------
void ConfiguratorWindow::initInputs()
{
    QSqlQuery query(m_system_db);

    QStringList list;

    if(query.exec("SELECT description FROM iodevice WHERE type=\'INPUT\';"))
    {
        while(query.next())
        {
            list << query.value("description").toString();
        }
    }

    m_inputs_window->createList(list);
}
//--------------------------------------
void ConfiguratorWindow::initDebugInfo()
{
    QSqlQuery   query(m_system_db);
    QStringList columns;
    QStringList rows = QStringList() << "Uc" << "3U0" << "3I0" << "Ia" << "Ib" << "Ic" << "Ua" << "Ub" << "Us" <<
                                        "Uab" << "Ubc" << "Uca" << "3U0x";

    if(query.exec("SELECT * FROM debug_info;"))
    {
        while(query.next())
        {
            QString name = QString("%1").arg(query.value("name").toString());
            columns << name;
        }

        m_debuginfo_window->setHedears(rows, columns);
    }
}
/*!
 * \brief ConfiguratorWindow::initWordStatus
 *
 * Инициализация списка статусов
 */
void ConfiguratorWindow::initWordStatus()
{
    QSqlQuery query(m_system_db);

    status_list_t status_info_list;

    if(query.exec("SELECT bit, name FROM status;"))
    {
        while(query.next())
        {
            int     bit  = query.value("bit").toInt();
            QString name = query.value("name").toString();

            status_info_list[bit] = name;
        }

        m_status_window->setStatusList(status_info_list);
    }
    else
        qWarning() << tr("Ошибка чтения базы данных \"Статус\": %1").arg(query.lastError().text());
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
//--------------------------------------------------------------------------------------------------------------------------
void ConfiguratorWindow::initTable(QTableView* table, QVector<QPair<QString, QString> >& column_labels, group_t& group_rows)
{
    CMatrixPurposeModel::IO_Type io_type = CMatrixPurposeModel::IO_INPUT;

    if(table == ui->tablewgtLedPurpose || table == ui->tablewgtRelayPurpose)
    {
        io_type = CMatrixPurposeModel::IO_OUTPUT;
    }

    HierarchicalHeaderView* hheader = new HierarchicalHeaderView(Qt::Horizontal, table);
    HierarchicalHeaderView* vheader = new HierarchicalHeaderView(Qt::Vertical, table);
    CMatrixPurposeModel*    model   = new CMatrixPurposeModel(column_labels, group_rows, io_type);

    bool is_inverse = false;

    if(table == ui->tablewgtDiscreteInputPurpose)
    {
        is_inverse = true;
    }

    table->setItemDelegate(new CTableItemDelegate(CTableItemDelegate::PURPOSE_TYPE, is_inverse));
    table->setHorizontalHeader(hheader);
    table->setVerticalHeader(vheader);
    table->setModel(model);

    int row_index = 0;

    if(table == ui->tablewgtLedPurpose || table == ui->tablewgtRelayPurpose)
        row_index = 1; // для таблиц привязок Светодиоды и Реле начало с первой строки, т.к. нулевая это Запоминание выходов

    for(int group_id: group_rows.keys())
    {
        group_item_t item = group_rows[group_id];

        if(item.name.isEmpty())
        {
            row_index++;
            continue;
        }

        if(item.var_list.isEmpty())
            continue;

        QFontMetrics fm_table = table->fontMetrics();
        int text_width = fm_table.width(item.name)*1.2;

        if((item.var_list.count()*fm_table.height()) < text_width)
        {
            int h = fm_table.height();

            if(text_width/item.var_list.count() <= (h + 5))
                h += 10;
            else
                h = text_width/item.var_list.count();

            for(int i = row_index; i < (row_index + item.var_list.count()); i++)
            {
                table->verticalHeader()->resizeSection(i, h);
            }
        }

        row_index += item.var_list.count();
    }
}
//----------------------------------------------------------------------------------------------
void ConfiguratorWindow::initTableProtection(QTableView* table, block_protection_list_t& labels)
{
    if(labels.isEmpty())
        return;

    QStringList list;

    for(const block_protection_t& protection: labels)
        list << protection.name;

    HierarchicalHeaderView* hheader = new HierarchicalHeaderView(Qt::Horizontal, table);
    HierarchicalHeaderView* vheader = new HierarchicalHeaderView(Qt::Vertical, table);
    CMatrixPurposeModel*    model   = new CMatrixPurposeModel(list);

    table->setItemDelegate(new CTableItemDelegate(CTableItemDelegate::PROTECTION_TYPE));
    table->setHorizontalHeader(hheader);
    table->setVerticalHeader(vheader);
    table->setModel(model);
    table->resizeColumnsToContents();
    table->resizeRowsToContents();

    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    table->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
}
/*!
 * \brief ConfiguratorWindow::initIndicatorStates
 *
 * Инициализация окна отображения состояний индикаторов (Светодиоды и реле)
 */
void ConfiguratorWindow::initIndicatorStates()
{
    ui->pushButtonIndicatorStates->setProperty("TYPE", "OUTPUT");

    QSqlQuery query(m_system_db);

    QStringList led_list;
    QStringList relay_list;

    if(query.exec("SELECT name FROM indicator WHERE type = \'LED\';"))
    {
        while(query.next())
        {
            led_list << query.value("name").toString();
        }
    }

    if(query.exec("SELECT name FROM indicator WHERE type = \'RELAY\';"))
    {
        while(query.next())
        {
            relay_list << query.value("name").toString();
        }
    }

    m_output_window->setLists(led_list, relay_list);
}
//----------------------------------------------------------------------
void ConfiguratorWindow::displayCalculateValues(QVector<quint16> values)
{
    if(values.size() == 74)
    {
        ui->variableWidget->setData(values);
    }

    m_calculate_buffer.clear();
}
//-----------------------------------------------------------
void ConfiguratorWindow::displayDateTime(CModBusDataUnit& unit)
{
    if(unit.count() != 4)
        return;

    int day    = (unit[1] >> 8)&0x00FF;
    int month  = unit[0]&0x00FF;
    int year   = (unit[0] >> 8)&0x00FF;
    int hour   = unit[2]&0x00FF;
    int minute = (unit[3] >> 8)&0x00FF;
    int second = unit[3]&0x00FF;

    if(year < 2000)
        year += 2000;

    QDateTime dt(QDate(year, month, day), QTime(hour, minute, second));

    ui->dateEdit->setDate(dt.date());
    ui->timeEdit->setTime(dt.time());
    ui->lineEditWeekDay->setText(dt.date().toString("dddd"));
}
//--------------------------------------------------------------------
void ConfiguratorWindow::displaySettingResponse(CModBusDataUnit& unit)
{
    if(!unit.isValid())
    {
        noConnectMessage();
        return;
    }

    QString                 first    = unit.property("FIRST").toString();
    QString                 last     = unit.property("LAST").toString();
    DeviceMenuItemType      group    = static_cast<DeviceMenuItemType>(unit.property("GROUP").toInt());
    QPoint                  indexKey = indexSettingKey(first, last);
    CDeviceMenuTableWidget* table    = groupMenuWidget(group);

    if(!table)
        return;

    if(indexKey.x() == -1 || indexKey.y() == -1)
        return;

    int index = 0;

    union
    {
        quint16 w[2];
        float   f;
        int     i;
    } value;

    for(int i = indexKey.x(); i <= indexKey.y(); i++)
    {
        if(index >= unit.count())
            break;

        QString    nameWgt  = QString("lineEdit%1").arg(m_cell_list[i].first);
        CLineEdit* lineEdit = qobject_cast<CLineEdit*>(groupMenuCellWidget(table, nameWgt, 1));

        if(!lineEdit)
        {
            index += 2;
            continue;
        }

        quint16 val1 = unit.values().at(index + 1);
        quint16 val2 = unit.values().at(index);

        value.w[0] = val1;
        value.w[1] = val2;

        QString str = QLocale::system().toString(value.f, 'f', 6);

        if(!str.isEmpty())
            lineEdit->setText(str);

        index += 2;
    }
}
/*!
 * \brief ConfiguratorWindow::statusInfo
 *
 * Отображение информации о статусах
 */
void ConfiguratorWindow::displayStatusInfo(const CModBusDataUnit& unit)
{
    RequestType type = (RequestType)unit.property("REQUEST").toInt();

    quint32 value = 0;

    switch (type)
    {
        case READ_STATUS_MCP_INFO:
            value = unit[1] | (unit[0] << 16);

            if(value == 0)
            {
                QMessageBox::information(m_status_window, tr("Информация о статусах"), tr("Нет ни одного активного статуса"));
                return;
            }

            m_status_window->updateMcpInfo(value);
        break;

        case READ_STATUS_MODULE_INFO:
            m_status_window->updateModuleInfo(unit.values());
        break;

        default: break;
    }
}
//-------------------------------------------------------------------------------
void ConfiguratorWindow::displayMemoryOut(const CModBusDataUnit::vlist_t& values)
{
    QTableView* table = nullptr;

    if(values.count() == 8) // Светодиоды
    {
        table = ui->tablewgtLedPurpose;
    }
    else if(values.count() == 12) // Реле
    {
        table = ui->tablewgtRelayPurpose;
    }

    if(!table)
        return;

    CMatrixPurposeModel* model = static_cast<CMatrixPurposeModel*>(table->model());

    if(!model)
        return;

    CMatrix& matrix = model->matrix();

    for(int i = 0; i < matrix.columnCount(); i++)
    {
        CColumn& column = matrix[0][i];
        bool value = values[i]&0x00FF;

        StateType state = ((value)?CHECKED:UNCHECKED);
        column.data().state = state;
    }

    model->updateData();
}
//---------------------------------------------------------------------------------
void ConfiguratorWindow::displaySettingControlResponce(const CModBusDataUnit& unit)
{
    if(!unit.isValid() || unit.count() == 0 || unit.count() > 1)
    {
        noConnectMessage();
        return;
    }

    RequestFunction requestFuncton = RequestFunction(unit.property("REQUEST_FUNCTION").toInt());

    if(requestFuncton == FUN_SAVE) // ответ на запись - выйти
        return;

    QString                 indexName = unit.property("INDEX").toString();
    QPoint                  index     = indexSettingKey(indexName, indexName);
    DeviceMenuItemType      group     = static_cast<DeviceMenuItemType>(unit.property("GROUP").toInt());
    CDeviceMenuTableWidget* table     = groupMenuWidget(group);

    if(!table)
        return;

    if(index.x() == -1 || index.x() >= m_cell_list.count())
        return;

    QString nameWgt = QString("comboBox%1").arg(m_cell_list[index.x()].first);

    if(nameWgt.isEmpty())
        return;

    QComboBox* comboBox = qobject_cast<QComboBox*>(groupMenuCellWidget(table, nameWgt, 1));

    if(!comboBox)
        return;

    quint16 i = unit[0];

    if(indexName.toUpper() != "TZ")
        i--;

    comboBox->setCurrentIndex(i);
}
//------------------------------------------------------------------
void ConfiguratorWindow::displayPurposeOutput(CModBusDataUnit& unit)
{
    qDebug() << "ЗАПРОС ПРИВЯЗОК ВЫХОДОВ";
    if(unit.count() == 0)
        return;

    QString first = unit.property(tr("FIRST")).toString(); // получаем первый ключ
    QString last  = unit.property(tr("LAST")).toString();  // получаем второй ключ

    if(first.isEmpty() || last.isEmpty())
        return;

    int faddr = addressPurposeKey(first);
    int eaddr = addressPurposeKey(last);

    int columnCount = (eaddr - faddr)/24 + 1; // 24 полуслова хранит до 384 переменных, т.е. при чтении одной
                                              // переменной мы получим 24 полуслова

    if(columnCount == 0)
        return;

    QTableView* table = tableMatrixFromKeys(first, last); // поиск таблицы по ключам

    if(!table)
        return;

    CMatrixPurposeModel* model = static_cast<CMatrixPurposeModel*>(table->model());

    if(!model)
        return;

    CMatrix& matrix = model->matrix();

    qDebug() << tr("Количество строк в моделе выходов: %1").arg(model->rowCount());

    int col_offset = matrix.columnIndexByKey(first); // получаем начальный индекс колонки для которой обрабатывается запрос
                                                     // например, если first = LED1, то получим 0, или first = LED3 -> 2
    if(col_offset == -1)
        return;

    QVector<quint16> values;

    for(int i = 0; i < unit.count() - 1; i += 2) // изменение младшего со старшим
    {
        values << unit[i + 1] << unit[i];
    }

    for(int i = 0; i < columnCount; i++)
    {
        int column_index = i + col_offset; // индекс столбца (запрос разбит на несколько частей, н-р: LED = 4 запроса
        int offset_pos   = i*24; // смещение в данных, т.е. 24 ячейки длина одного запроса

        CMatrix::RowArray& rows = matrix.rows();

        qDebug() << tr("Количество строк в таблице выходов: %1").arg(matrix.rowCount());

        for(CRow& row: rows)
        {
            int pos = row.data().position;

            if(pos == -1)
                continue;

            int word   = pos/16;
            int bit    = pos%16;
            bool state = (values[word + offset_pos]&(1 << bit));

            row[column_index].data().state = (state)?CHECKED:UNCHECKED;
        }
    }

    model->updateData();
}
//-------------------------------------------------------------------------------------------------------------------------------
void ConfiguratorWindow::displayPurposeDIResponse(const QVector<quint16>& input_list, const QVector<quint16>& input_inverse_list)
{
    if(input_list.count() != input_inverse_list.count())
    {
        qDebug() << tr("Количество привязок входов не соответствует количеству инверсных привязок: %1/%2").
                    arg(input_inverse_list.count()).arg(input_inverse_list.count());
        return;
    }

    QVector<QString> var_list = loadVaribleByType("DI");

    if(var_list.isEmpty())
        return;

    CMatrixPurposeModel* model  = static_cast<CMatrixPurposeModel*>(ui->tablewgtDiscreteInputPurpose->model());
    CMatrix&             matrix = model->matrix();

    if(matrix.rowCount() == 0 || matrix.columnCount() == 0)
        return;

    QVector<quint32> input_data;

    for(int i = 0; i < (input_list.count() - 1); i += 2) // переводим полуслова (16 бит) в слова (32 бита)
    {                                                    // каждые 32 бита хранят состояния входов для переменной
        quint32 value = ((input_list[i] << 16) | input_list[i + 1]);
        input_data << value;
    }

    QVector<quint32> input_inverse_data;

    for(int i = 0; i < (input_inverse_list.count() - 1); i += 2) // переводим полуслова (16 бит) в слова (32 бита)
    {                                                            // каждые 32 бита хранят состояния инверсий входов для переменной
        quint32 value = ((input_inverse_list[i] << 16) | input_inverse_list[i + 1]);
        input_inverse_data << value;
    }

    for(int i = 0; i < input_data.count(); i++)
    {
        QString key = var_list[i].toUpper();

        int row_index = -1;

        for(int k = 0; k < matrix.rowCount(); k++) // производим поиск позиции текущей переменной в строках, т.к. строки
        {                                          // идут не по порядку - разбиты на группы (позиция переменной в var_list
                                                   // определяет ее положение в полученных данных учитывая смещение)
            if(matrix[k].data().key.toUpper() == key)
            {
                row_index = k;
                break;
            }
        }

        if(row_index != -1)
        {
            for(int j = 0; j < matrix.columnCount(); j++)
            {
                bool input_state   = input_data[i]&(1 << j);
                bool inverse_state = input_inverse_data[i]&(1 << j);

                StateType state = UNCHECKED;

                if(input_state && !inverse_state)
                    state = CHECKED;
                else if(input_state && inverse_state)
                    state = INVERSE;

                matrix[row_index][j].data().state = state;
            }
        }
    }

    model->updateData();
}
//---------------------------------------------------------------------
void ConfiguratorWindow::displayJournalResponse(QVector<quint16>& data)
{
    if(!m_journal_read_current)
    {
        return;
    }

    m_journal_read_current->print(data);

    QString key = m_journal_read_current->property("TYPE").toString();

    if(m_journal_set.find(key) != m_journal_set.end())
    {
        int read  = m_journal_set[key].message.read_count;
        int total = m_journal_set[key].message.read_total;

        m_journal_read_current->header()->setTextDeviceCountMessages(read, total);
    }
}
//------------------------------------------------------------------------------
void ConfiguratorWindow::displayDeviceSerialNumber(const QVector<quint16>& data)
{
    if(data.count() == 4) // пришел серийный номер, иначе сообщение с ошибкой
    {
        m_status_bar->connectStateChanged(true); // обновляем состояние соединения с устройством

        quint16 value0 = data[0];
        quint16 value1 = data[1];
        quint16 value2 = data[2];
        quint16 value3 = data[3];

        quint8  device_code = quint8((value0 >> 8)&0xFF); // получаем код изделия РПА
        quint8 index_number = ((quint8((value0&0xFF)) >> 4)&0x0F)*1000 + (quint8((value0&0xFF))&0x0F)*100 +
                              (((quint8((value1 >> 8)&0xFF)) >> 4)&0x0F)*10 + ((quint8((value1 >> 8)&0xFF))&0x0F);
        quint8 party_number = (quint8(value1&0xFF) >> 4)*10 + quint8(value1&0x0F);
        quint8 firmware_var = ((quint8(value2 >> 8)&0xFF) >> 4)*10 + (quint8(value2 >> 8)&0x0F);
        quint8 year         = (quint8(value2&0xFF) >> 4)*10 + quint8(value2&0x0F);
        quint8 month        = ((quint8(value3 >> 8)&0xFF) >> 4)*10 + (quint8(value3 >> 8)&0x0F);
        quint8 day          = (quint8(value3&0xFF) >> 4)*10 + quint8(value3&0x0F);

        QString dev_code_str = m_device_code_list[device_code];

        if(dev_code_str.isEmpty())
            dev_code_str = "0";

        QString str;
        QString date = "0";

        if(year != 0 && month != 0 && day != 0 && month <= 12 && day <= 31)
            date = QDate(year, month, day).toString("yy-MM-dd");

        str = QString("S/n: %1-%2-%3-%4-%5").arg(dev_code_str).
                                             arg(QString("%1%2").arg(QString(4 - QString::number(index_number).count(), '0')).
                                                 arg(index_number)).
                                             arg(QString("%1%2").arg(QString(2 - QString::number(party_number).count(), '0')).
                                                 arg(party_number)).
                                             arg(QString("%1%2").arg(QString(2 - QString::number(firmware_var).count(), '0')).
                                                 arg(firmware_var)).
                                             arg(date);

        m_status_bar->setSerialNumber(str);
    }
}
//-------------------------------------------------------------------------------------
void ConfiguratorWindow::displayProtectReserveSignalStart(const QVector<quint16>& data)
{
    if(data.isEmpty())
    {
        qWarning() << tr("Защиты->Резервные->Сигналы пуска: нет данных.");
        return;
    }

//    QVector<QComboBox*> box_list = QVector<QComboBox*>() << ui->cboxN50 << ui->cboxN52 << ui->cboxN53 << ui->cboxN54 <<
//                                                            ui->cboxN55 << ui->cboxN56 << ui->cboxN57 << ui->cboxN58 <<
//                                                            ui->cboxN59 << ui->cboxV04 << ui->cboxV07 << ui->cboxV10 <<
//                                                            ui->cboxV13 << ui->cboxV16 << ui->cboxV19 << ui->cboxV22 <<
//                                                            ui->cboxV25 << ui->cboxV28 << ui->cboxV31 << ui->cboxV36 <<
//                                                            ui->cboxV39 << ui->cboxV44 << ui->cboxV50 << ui->cboxV62 <<
//                                                            ui->cboxV65 << ui->cboxV68 << ui->cboxV76 << ui->cboxV77 <<
//                                                            ui->cboxV81 << ui->cboxV86 << ui->cboxV90 << ui->cboxV95 <<
//                                                            ui->cboxV96;

//    QVector<quint16> tdata;

//    for(int i = 0; i < data.count() - 1; i += 2) // меняем местами старший и младший байт
//    {
//        tdata << data[i + 1] << data[i];
//    }

//    for(QComboBox* box: box_list)
//    {
//        QString key = box->objectName().remove("cbox");

//        if(key.isEmpty())
//            continue;

//        int bit     = m_variable_bits[key];
//        int val_pos = bit/16;
//        int bit_pos = bit%16;

//        if(val_pos < tdata.count())
//        {
//            int item_pos = (tdata[val_pos]&(1 << bit_pos))?1:0;

//            if(item_pos < box->count())
//                box->setCurrentIndex(item_pos);
//        }
//    }
}
//------------------------------------------------------------------------------------
void ConfiguratorWindow::displayAutomationAPVSignalStart(const QVector<quint16>& data)
{
//    QVector<QComboBox*> box_list = QVector<QComboBox*>() << ui->cboxN50_2 << ui->cboxN52_2 << ui->cboxN53_2 << ui->cboxN54_2 <<
//                                                            ui->cboxN55_2 << ui->cboxN56_2 << ui->cboxN57_2 << ui->cboxN58_2 <<
//                                                            ui->cboxN59_2 << ui->cboxV04_2 << ui->cboxV07_2 << ui->cboxV10_2 <<
//                                                            ui->cboxV13_2 << ui->cboxV16_2 << ui->cboxV19_2 << ui->cboxV22_2 <<
//                                                            ui->cboxV25_2 << ui->cboxV28_2 << ui->cboxV31_2 << ui->cboxV36_2 <<
//                                                            ui->cboxV39_2 << ui->cboxV44_2 << ui->cboxV50_2 << ui->cboxV62_2 <<
//                                                            ui->cboxV65_2 << ui->cboxV68_2 << ui->cboxV76_2 << ui->cboxV77_2 <<
//                                                            ui->cboxV81_2 << ui->cboxV86_2 << ui->cboxV90_2 << ui->cboxV95_2 <<
//                                                            ui->cboxV96_2;

//    QVector<quint16> tdata;

//    for(int i = 0; i < data.count() - 1; i += 2) // меняем местами старший и младший байт
//    {
//        tdata << data[i + 1] << data[i];
//    }

//    for(QComboBox* box: box_list)
//    {
//        QString key = (box->objectName().remove("cbox")).remove("_2");

//        if(key.isEmpty())
//            continue;

//        int bit     = m_variable_bits[key];
//        int val_pos = bit/16;
//        int bit_pos = bit%16;

//        if(val_pos < tdata.count())
//        {
//            int item_pos = (tdata[val_pos]&(1 << bit_pos))?1:0;

//            if(item_pos < box->count())
//                box->setCurrentIndex(item_pos);
//        }
//    }
}
//---------------------------------------------------------------------------------------
void ConfiguratorWindow::displayCommunicationTimeoutRequest(const QVector<quint16>& data)
{
    qDebug() << tr("Чтение таймаут запроса устройства: %1").arg(data[0]);
    if(data.count() > 0)
    {
        ui->spinBoxCommunicationRequestTimeout->setValue(data[0]);
    }
}
//-------------------------------------------------------------------------------------
void ConfiguratorWindow::displayCommunicationTimeoutSpeed(const QVector<quint16>& data)
{
    qDebug() << tr("Чтение таймаут скорости устройства: %1").arg(data[0]);
    if(data.count() > 0)
    {
        ui->spinBoxCommunicationTimeoutSpeed->setValue(data[0]);
    }
}
//--------------------------------------------------------------------------------
void ConfiguratorWindow::displayCommunicationAddress(const QVector<quint16>& data)
{
    qDebug() << tr("Чтение адреса устройства: %1").arg(data[0]);
    if(data.count() > 0)
    {
        ui->spinBoxCommunicationAddress->setValue(data[0]);
    }
}
//-----------------------------------------------------------------------
void ConfiguratorWindow::displayProtectionWorkMode(CModBusDataUnit& unit)
{
    if(unit.count() != 48)
        return;

    QString tprotect = unit.property("PROTECTION").toString();

    if(tprotect.isEmpty())
        return;

    DeviceMenuItemType      group = static_cast<DeviceMenuItemType>(unit.property("GROUP").toInt());
    CDeviceMenuTableWidget* table = groupMenuWidget(group);

    if(!table)
        return;

    QString    wgtName  = QString("comboBox%1").arg(tprotect);
    QComboBox* comboBox = qobject_cast<QComboBox*>(groupMenuCellWidget(table, wgtName, 1));

    if(!comboBox)
        return;

    RequestFunction function = RequestFunction(unit.property("REQUEST_FUNCTION").toInt());

    QString var;

    if(m_protections.find(tprotect) != m_protections.end())
        var = m_protections[tprotect].var_name;

    if(var.isEmpty())
        return;

    int bitNumber = -1;

    if(m_variable_bits.find(var) != m_variable_bits.end())
        bitNumber = m_variable_bits[var];

    if(bitNumber == -1)
        return;

    int k10_pos = bitNumber/16;
    int k11_pos = k10_pos + 24;
    int bit     = bitNumber%16;

    int k10_state = -1;
    int k11_state = -1;

    if(function == FUN_READ)
    {
        k10_state = ((unit[k10_pos]&(1 << bit)) >> bit);
        k11_state = ((unit[k11_pos]&(1 << bit)) >> bit);

        int row = k10_state << 1 | k11_state;

        if(row < comboBox->count())
            comboBox->setCurrentIndex(row);
    }
    else if(function == FUN_SAVE)
    {
        int state = comboBox->currentIndex();

        switch(state)
        {
            case 0:
                k10_state = 0;
                k11_state = 0;
            break;

            case 1:
                k10_state = 0;
                k11_state = 1;
            break;

            case 2:
                k10_state = 1;
                k11_state = 0;
            break;

            default: return;
        }

        if(k10_state == -1 || k11_state == -1)
            return;

        QVector<quint16> values = unit.values();

        values[k10_pos] &= (~quint16(1 << bit)); // очищаем бит k10
        values[k10_pos] |= quint16(k10_state << bit); // устанавливаем бит k10
        values[k11_pos] &= (~quint16(1 << bit)); // очищаем бит k11
        values[k11_pos] |= quint16(k11_state << bit); // устанавливаем бит k11

        int addr = addressSettingKey("K10");

        if(addr == -1)
            return;

        CModBusDataUnit new_unit(m_serialPortSettings_window->deviceID(), CModBusDataUnit::WriteMultipleRegisters,
                                 addr, values);

        m_modbus->sendData(new_unit);
    }
}
//-----------------------------------------------------------------
void ConfiguratorWindow::displayMonitorK10_K11(CModBusDataUnit& unit)
{
    if(unit.count() != 48)
        return;

    QTableView* table = m_monitor_purpose_window->table();

    if(!table)
        return;

    CMatrixPurposeModel* model = static_cast<CMatrixPurposeModel*>(table->model());

    if(!model)
        return;

    CMatrix& matrix = model->matrix();

    for(int col = 0; col < matrix.columnCount(); col++)
    {
        int offset = col*24;

        for(int row = 0; row < matrix.rowCount(); row++)
        {
            quint16 value = unit[row + offset];

            for(quint8 bit = 0; bit < sizeof(value)*8; bit++)
            {
                bool state = (value&(1 << bit));
                int  pos   = (sizeof(value)*8)*row + bit;

                if(pos >= matrix.rowCount())
                    break;

                matrix[pos][col].data().state = ((state)?CHECKED:UNCHECKED);
            }
        }
    }

    model->updateData();
}
//----------------------------------------------------------------
void ConfiguratorWindow::displayOutputAllRead(CModBusDataUnit& unit)
{
    if(unit.count() != 4)
        return;

    QVector<quint16> data = QVector<quint16>() << unit[1] << unit[0] << unit[3] << unit[2];

    m_outputall_window->setOutputStates(data);
    m_output_window->setOutputStates(data);
}
//----------------------------------------------------------------------
void ConfiguratorWindow::displayInputsRead(const QVector<quint16>& data)
{
    if(data.count() != 2)
        return;

    QVector<quint16> t;

    t << data[1] << data[0];

    m_inputs_window->setOutputStates(t);
}
//-------------------------------------------------------------------------------
void ConfiguratorWindow::displayBlockProtectionRead(const QVector<quint16>& data)
{
    if(data.count() < 24)
        return;

    static QVector<QVector<quint16> > data_buf;

    QVector<quint16> tdata;

    for(int i = 0; i < data.count() - 1; i += 2)
        tdata << data[i + 1] << data[i];

    data_buf << tdata;

    if(data_buf.count() < m_block_list.count())
        return;

    CMatrixPurposeModel* model = static_cast<CMatrixPurposeModel*>(ui->tablewgtProtectionCtrl->model());

    if(!model)
    {
        qInfo() << tr("Не удалось получить модель таблицы Управления защитами.");
        return;
    }

    CMatrix& matrix = model->matrix();

    for(int i = 0; i < matrix.rowCount(); i++)
    {
        QVector<quint16> row = data_buf[i];
        QVector<block_protection_purpose_t> purpose_list = m_block_list[i].purpose;

        for(int j = 0, pos = 0; j < matrix.columnCount(); j++, pos++)
        {
            if(i == j)
            {
                pos--;
                continue;
            }

            block_protection_purpose_t purpose = purpose_list[pos];
            int col = purpose.bit/16;
            int bit = purpose.bit%16;

            bool state = (row[col]&(1 << bit));

            matrix[i][j].data().state = ((state)?CHECKED:UNCHECKED);
        }
    }

    data_buf.clear();
    model->updateData();
}
//------------------------------------------------------------------
void ConfiguratorWindow::displayDebugInfo(const CModBusDataUnit& unit)
{
//    if(unit.valueCount() != 15)
//        return;

    int channel = unit.property("CHANNEL").toInt();

    m_debuginfo_window->setData(channel, unit.values());
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

    QVector<QPair<QString, QString> > ver;

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
                        version += ((temp.toFloat() < 10.0f)?tr("0") + temp:temp) + tr(" build ");
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
                ver << qMakePair(version, desc);

                fMajor = fMinor = fBuild = fDesc = false;

                version.clear();
                desc.clear();

                param = tr("MAJOR");
            }
        }
    }

    file.close();

    m_version_window->setText(ver);

    QString title = this->windowTitle();

    if(!ver.isEmpty())
        title += tr(" ") + ver.last().first;

    ui->widgetMenuBar->setWindowTitle(title);
}
//---------------------------------------------------------------------------------
int ConfiguratorWindow::sizeBlockSetting(const QString& first, const QString& last)
{
    int iFirst = -1, iSecond = -1;

    for(quint8 i = 0; i < m_cell_list.count(); i++)
    {
        QPair<QString, cell_t> pair = m_cell_list[i];

        if(pair.first == first)
            iFirst = i;

        if(pair.first == last)
            iSecond = i;
    }

    if(iFirst != -1 && iSecond != -1 && (iSecond >= iFirst))
        return ((iSecond - iFirst) + 1);

    return -1;
}
/*!
 * \brief ConfiguratorWindow::readDataFromExcel
 * \param doc Текущий документ Excel
 * \param is_next_group Следующая запись начинается с новой группы, т.е. имеет отдельный заголовок
 * \param grid Указатель на грид в котором находятся поля с данными
 */
void ConfiguratorWindow::readDataFromExcel(QXlsx::Document& doc, const QString& group, const QGridLayout* grid)
{
    int beg_offset = 0;

    if(!group.isEmpty())
    {
        beg_offset = groupPositionInExcel(doc, group);

        if(beg_offset == -1)
        {
            qWarning() << tr("Чтение данных из файла Excel: группа <%1> не найдена.").arg(group);
            return;
        }
    }

    static int offset = 0;

    if(!group.isEmpty())
        offset = beg_offset;

    int rows    = grid->rowCount();
    int columns = grid->columnCount();
    int count   = 0;

    for(int col = 0; col < columns - 1; col += 2)
    {
        int col_index = col;
        int row_index = col/2;

        if(columns%2 && col != 0)
            col_index++;

        for(int row = 0; row < rows; row++)
        {
            int cell_index = row + rows*row_index + offset;

            QLayoutItem* layout_item = grid->itemAtPosition(row, col_index + 1);

            if(layout_item)
            {
                QWidget* wgt = layout_item->widget();

                if(wgt)
                {
                    if(QString(wgt->metaObject()->className()).toUpper() == "QCOMBOBOX")
                    {
                        QComboBox* cb = qobject_cast<QComboBox*>(wgt);

                        if(cb)
                        {
                            int pos = doc.read(cell_index, 3).toInt();

                            if(pos >= 1)
                                cb->setCurrentIndex(pos - 1);
                        }
                    }
                    else if(QString(wgt->metaObject()->className()).toUpper() == "CLINEEDIT")
                    {
                        CLineEdit* le = qobject_cast<CLineEdit*>(wgt);

                        if(le)
                        {
                            bool    is_ok    = false;
                            QString cell_val = doc.read(cell_index, 3).toString();
                            float   number   = QLocale::system().toFloat(cell_val, &is_ok);

                            if(is_ok)
                            {
                                le->setText(QLocale::system().toString(number, 'f', 6));
                            }
                            else
                                qWarning() << tr("Чтение данных из файла Excel: невозможно преобразование в число <%1>.").arg(group);
                        }
                    }

                    count++;
                }
            }
        }
    }

    offset += count;

    m_progressbar->increment();
}
/*!
 * \brief ConfiguratorWindow::groupPositionInExcel
 * \param doc Документ Excel
 * \param group Имя группы позицию которой необходимо найти
 * \return Позиция начала данных группы, или -1 если не найдена такая группа
 */
int ConfiguratorWindow::groupPositionInExcel(QXlsx::Document& doc, const QString& group)
{
    int result = -1;

    for(int row = 1; row <= doc.dimension().lastRow(); row++)
    {
        QString cell_text = doc.read(row, 1).toString();

        if(cell_text.isEmpty())
            continue;

        if(cell_text.toUpper() == group.toUpper())
        {
            result = row + 1;
            break;
        }
    }

    return result;
}
//------------------------------------------------------------------------------------------
CDeviceMenuTableWidget::group_t ConfiguratorWindow::loadMenuGroup(const QString& group_name)
{
    QSqlQuery query(m_system_db);
    CDeviceMenuTableWidget::group_t group;
    int group_id = -1;

    if(!query.exec(QString("SELECT id FROM menu_group WHERE name=\"%1\";").arg(group_name)))
    {
        qWarning() << tr("Не удалось прочитать ID группы \"%1\", (%2)").arg(group_name).arg(query.lastError().text());
    }

    if(query.first())
        group_id = query.value("id").toInt();

    group.name = group_name;

    if(!query.exec(QString("SELECT * FROM iodevice WHERE sort_id=%1").arg(group_id)))
    {
        qWarning() << tr("Не удалось прочитать свойства группы с ID=%1 (%2)").arg(group_id).arg(query.lastError().text());
    }

    while(query.next())
    {
        CDeviceMenuTableWidget::item_t item;

        item.key      = query.value("key").toString();
        item.address  = query.value("address").toInt();
        item.unit     = CDeviceMenuTableWidget::measure_t({ query.value("limit_min").toFloat(),
                                                            query.value("limit_max").toFloat(),
                                                            query.value("unit_measure").toString() });
        item.type     = query.value("data_type").toString();
        item.row      = query.value("row").toInt();
        item.name     = query.value("description").toString();

        if(item.type.toUpper() == "LIST") // если тип - "СПИСОК", то читаем список подпунктов
        {
            QSqlQuery query_list(m_system_db);

            if(query_list.exec(QString("SELECT name FROM menu_item_choice WHERE key=\"%1\";").arg(item.key)))
            {
               CDeviceMenuTableWidget::item_list_t subitemlist;

               while(query_list.next())
               {
                   CDeviceMenuTableWidget::item_t subitem;
                   subitem.name = query_list.value("name").toString();
                   subitemlist << subitem;
               }

               item.subitems = subitemlist;
            }
        }

        group.items << item;
    }

    return group;
}
/*!
 * \brief ConfiguratorWindow::groupMenuWidget
 * \param type - Тип группы меню
 * \return Указатель на таблицу группы
 *
 * Метод для получения виджета меню группы
 */
CDeviceMenuTableWidget* ConfiguratorWindow::groupMenuWidget(DeviceMenuItemType type) const
{
    switch(type)
    {
        case DEVICE_MENU_PROTECT_ITEM_CURRENT:
            return ui->tableWidgetProtectionGroupMTZ;

        case DEVICE_MENU_PROTECT_ITEM_POWER:
            return ui->tableWidgetProtectionGroupPower;

        case DEVICE_MENU_PROTECT_ITEM_DIRECTED:
            return ui->tableWidgetProtectionGroupDirect;

        case DEVICE_MENU_PROTECT_ITEM_FREQUENCY:
            return ui->tableWidgetProtectionGroupFrequency;

        case DEVICE_MENU_PROTECT_ITEM_EXTERNAL:
            return ui->tableWidgetProtectionGroupExternal;

        case DEVICE_MENU_PROTECT_ITEM_MOTOR:
            return ui->tableWidgetProtectionGroupMotor;

        case DEVICE_MENU_PROTECT_ITEM_TEMPERATURE:
            return ui->tableWidgetProtectionGroupTemperature;

        case DEVICE_MENU_PROTECT_ITEM_RESERVE:
            return ui->tableWidgetProtectionGroupReserve;

        case DEVICE_MENU_PROTECT_ITEM_CONTROL:
            return ui->tableWidgetProtectionGroupControl;

        case DEVICE_MENU_ITEM_AUTOMATION_ROOT:
            return ui->tableWidgetAutomationGroup;

        case DEVICE_MENU_ITEM_SETTINGS_ITEM_IN_ANALOG:
            return ui->tableWidgetSettingsAnalogGroupGeneral;

        default: return nullptr;
    }
}
/*!
 * \brief ConfiguratorWindow::groupMenuCellWidget
 * \param table - указатель на таблицу
 * \param wgt_name - имя виджета
 * \param col - колонка таблицы
 * \return Указатель на виджет из ячейки
 *
 * Поиск виджета в указанно таблице по имени и номеру ячейки
 */
QWidget* ConfiguratorWindow::groupMenuCellWidget(CDeviceMenuTableWidget* table, const QString& wgt_name, int col) const
{
    if(!table)
        return nullptr;

    if(col > table->columnCount())
        return nullptr;

    for(int i = 0; i < table->rowCount(); i++)
    {
        QWidget* wgt = table->cellWidget(i, col);

        if(!wgt)
            continue;

        QObjectList obj_list = wgt->children();

        for(QObject* obj: obj_list)
        {
            if(obj->isWidgetType())
            {
                if(obj->objectName().toUpper() == wgt_name.toUpper())
                    return qobject_cast<QWidget*>(obj);
            }
        }
    }

    return nullptr;
}
//----------------------------------------------------------------------------------------
void ConfiguratorWindow::sendSettingReadRequest(const QString& first, const QString& last,
                                                CModBusDataUnit::FunctionType type, int size, DeviceMenuItemType index)
{
    if(size <= 0)
        return;

    int addr = addressSettingKey(first);

    CModBusDataUnit unit(m_serialPortSettings_window->deviceID(), type, addr, QVector<quint16>() << size);

    unit.setProperty("REQUEST", GENERAL_TYPE);
    unit.setProperty("FIRST", first);
    unit.setProperty("LAST", last);
    unit.setProperty("GROUP", index);

    m_modbus->sendData(unit);
}
//----------------------------------------------------------------------------------------------------------
void ConfiguratorWindow::sendSettingControlReadRequest(const QString& index, DeviceMenuItemType group_index)
{
    int addr = addressSettingKey(index);

    CModBusDataUnit unit(m_serialPortSettings_window->deviceID(), CModBusDataUnit::ReadHoldingRegisters, addr, QVector<quint16>() << 1);

    unit.setProperty("REQUEST", GENERAL_CONTROL_TYPE);
    unit.setProperty("REQUEST_FUNCTION", FUN_READ);
    unit.setProperty("INDEX", index);
    unit.setProperty("GROUP", group_index);

    m_modbus->sendData(unit);
}
//-----------------------------------------------------------------------------------------------------------
void ConfiguratorWindow::sendSettingControlWriteRequest(const QString& index, DeviceMenuItemType group_index)
{
    if(index.isEmpty())
        return;

    QPoint key = indexSettingKey(index, index);

    if(key.x() == -1 || key.x() >= m_cell_list.count())
        return;

    CDeviceMenuTableWidget* table = groupMenuWidget(group_index);

    if(!table)
        return;

    QString nameWgt = QString("comboBox%1").arg(m_cell_list[key.x()].first);

    if(nameWgt.isEmpty())
        return;

    QComboBox* comboBox = qobject_cast<QComboBox*>(groupMenuCellWidget(table, nameWgt, 1));

    if(!comboBox)
        return;

    int addr = addressSettingKey(index);

    if(addr == -1)
        return;

    quint16 value = comboBox->currentIndex();

    if(index.toUpper() != "TZ") // токозависимые характеристики учитывают и ноль, остальные с единицы
        value++;

    CModBusDataUnit unit(m_serialPortSettings_window->deviceID(), CModBusDataUnit::WriteSingleRegister, addr,
                                                 QVector<quint16>() << value);
    unit.setProperty("REQUEST", GENERAL_CONTROL_TYPE);
    unit.setProperty("REQUEST_FUNCTION", FUN_SAVE);
    unit.setProperty("INDEX", index);

    m_modbus->sendData(unit);
}
//-------------------------------------------------------------------------------------------------------------------------
void ConfiguratorWindow::sendSettingWriteRequest(const QString& first, const QString& last, DeviceMenuItemType group_index)
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

    CDeviceMenuTableWidget* table = groupMenuWidget(group_index);

    for(int i = index.x(); i <= index.y(); i++)
    {
        QString nameWgt = QString("lineEdit%1").arg(m_cell_list[i].first);

        if(nameWgt.isEmpty())
        {
            data.append(0.0f);
            data.append(0.0f);

            continue;
        }

        CLineEdit* lineEdit = qobject_cast<CLineEdit*>(groupMenuCellWidget(table, nameWgt, 1));

        if(!lineEdit)
            continue;

        value.f = QLocale::system().toFloat(lineEdit->text());

        data.append(value.w[1]);
        data.append(value.w[0]);
    }

    CModBusDataUnit::FunctionType funType = ((data.count() == 1)?CModBusDataUnit::WriteSingleRegister:
                                                               CModBusDataUnit::WriteMultipleRegisters);

    CModBusDataUnit unit(m_serialPortSettings_window->deviceID(), funType, addressSettingKey(first), data);

    unit.setProperty(tr("FIRST"), first);
    unit.setProperty(tr("LAST"), last);

    m_modbus->sendData(unit);
}
//----------------------------------------------------------------------------------------
void ConfiguratorWindow::sendPurposeReadRequest(const QString& first, const QString& last)
{
    int faddr = addressPurposeKey(first);
    int laddr = addressPurposeKey(last);

    if(faddr == -1 || laddr == -1)
        return;

    int size = laddr - faddr + 24; // получаем размер считываемого блока с учетом выравнивания в 48 байт (одна строка)

    CModBusDataUnit unit(m_serialPortSettings_window->deviceID(), CModBusDataUnit::ReadHoldingRegisters, faddr,
                                                                  QVector<quint16>() << size);

    unit.setProperty("REQUEST", PURPOSE_OUT_TYPE);
    unit.setProperty("FIRST", first);
    unit.setProperty("LAST", last);

    m_modbus->sendData(unit);
}
//-----------------------------------------------------------------------------------------
void ConfiguratorWindow::sendPurposeWriteRequest(const QString& first, const QString& last)
{
    QTableView* table = tableMatrixFromKeys(first, last);

    if(!table)
        return;

    CMatrix matrix = static_cast<CMatrixPurposeModel*>(table->model())->matrix();

    int bIndex = matrix.columnIndexByKey(first);
    int eIndex = matrix.columnIndexByKey(last);

    if(bIndex == -1 || eIndex == -1)
        return;

    int hword_size = (eIndex - bIndex + 1)*24;

    QVector<quint16> data = QVector<quint16>(hword_size, 0); // создаем вектор размерностью hword_size полуслов со значением 0

    for(int i = 0; i <= (eIndex - bIndex); i++) // обход колонок
    {
        int index      = i + bIndex;
        int offset_pos = i*24;

        for(int j = 1; j < matrix.rowCount(); j++)
        {
            quint16 hword = matrix[j].data().position/16;
            quint16 bit   = matrix[j].data().position%16;

            StateType state = matrix[j][index].data().state;

            if(state == CHECKED)
                data[hword + offset_pos] |= (1 << bit);
        }
    }

    QVector<quint16> values;

    for(int i = 0; i < (data.count() - 1); i += 2)
        values << data[i + 1] << data[i];

    CModBusDataUnit::FunctionType funType = ((values.count() == 1)?CModBusDataUnit::WriteSingleRegister:
                                                                   CModBusDataUnit::WriteMultipleRegisters);

    CModBusDataUnit unit(m_serialPortSettings_window->deviceID(), funType, addressPurposeKey(first), values);

    unit.setProperty(tr("FIRST"), first);
    unit.setProperty(tr("LAST"), last);

    m_modbus->sendData(unit);
}
//------------------------------------------------------------------------------
void ConfiguratorWindow::sendPurposeDIReadRequest(int first_addr, int last_addr)
{
    int size = last_addr - first_addr + 2;

    CModBusDataUnit unit(m_serialPortSettings_window->deviceID(), CModBusDataUnit::ReadHoldingRegisters, first_addr,
                                                                  QVector<quint16>() << size);

    unit.setProperty(tr("REQUEST"), ((first_addr < 768)?PURPOSE_INPUT_TYPE:PURPOSE_INPUT_INVERSE_TYPE));
    unit.setProperty(tr("FIRST_ADDRESS"), first_addr);
    unit.setProperty(tr("LAST_ADDRESS"), last_addr);

    m_modbus->sendData(unit);
}
//-------------------------------------------------------------------------------
void ConfiguratorWindow::sendPurposeDIWriteRequest(int first_addr, int last_addr)
{
    CMatrixPurposeModel* model = static_cast<CMatrixPurposeModel*>(ui->tablewgtDiscreteInputPurpose->model());

    if(!model)
        return;

    CMatrix matrix = model->matrix();

    if(matrix.rowCount() == 0 || matrix.columnCount() == 0)
        return;

    QVector<QString> var_list = loadVaribleByType("DI");

    int bIndex = (first_addr - 512)/2;
    int eIndex = (670 - last_addr)/2;

    QVector<quint16> values;

    for(int i = bIndex; i < var_list.count() - eIndex; i++)
    {
        QString key       = var_list[i].toUpper();
        int     row_index = -1;

        for(int j = 0; j < matrix.rowCount(); j++)
        {
            QString row_key = matrix[j].data().key.toUpper();

            if(key == row_key)
            {
                row_index = j;
                break;
            }
        }

        if(row_index != -1)
        {
            quint32 value = 0;

            for(int k = 0; k < matrix.columnCount(); k++)
            {
                bool state = ((matrix[row_index][k].data().state == CHECKED ||
                               matrix[row_index][k].data().state == INVERSE)?true:false);

                if(state)
                    value |= (1 << k);
            }

            values << quint16((value&0xFFFF0000) >> 16) << quint16(value&0x0000FFFF);
        }
    }

    CModBusDataUnit::FunctionType funType = ((values.count() == 1)?CModBusDataUnit::WriteSingleRegister:
                                                                 CModBusDataUnit::WriteMultipleRegisters);

    CModBusDataUnit unit(m_serialPortSettings_window->deviceID(), funType, first_addr, values);

    unit.setProperty(tr("FIRST_ADDRESS"), first_addr);
    unit.setProperty(tr("LAST_ADDRESS"), last_addr);

    m_modbus->sendData(unit);
}
//--------------------------------------------------------------------------------------
void ConfiguratorWindow::sendPurposeInverseDIWriteRequest(int first_addr, int last_addr)
{
    CMatrixPurposeModel* model = static_cast<CMatrixPurposeModel*>(ui->tablewgtDiscreteInputPurpose->model());

    if(!model)
        return;

    CMatrix matrix = model->matrix();

    if(matrix.rowCount() == 0 || matrix.columnCount() == 0)
        return;

    QVector<QString> var_list = loadVaribleByType("DI");

    int bIndex = (first_addr - 768)/2;
    int eIndex = (926 - last_addr)/2;

    QVector<quint16> values;

    for(int i = bIndex; i < var_list.count() - eIndex; i++)
    {
        QString key       = var_list[i].toUpper();
        int     row_index = -1;

        for(int j = 0; j < matrix.rowCount(); j++)
        {
            QString row_key = matrix[j].data().key.toUpper();

            if(key == row_key)
            {
                row_index = j;
                break;
            }
        }

        if(row_index != -1)
        {
            quint32 value = 0;

            for(int k = 0; k < matrix.columnCount(); k++)
            {
                bool state = ((matrix[row_index][k].data().state == INVERSE)?true:false);

                if(state)
                    value |= (1 << k);
            }

            values << quint16((value&0xFFFF0000) >> 16) << quint16(value&0x0000FFFF);
        }
    }

    CModBusDataUnit::FunctionType funType = ((values.count() == 1)?CModBusDataUnit::WriteSingleRegister:
                                                                   CModBusDataUnit::WriteMultipleRegisters);

    CModBusDataUnit unit(m_serialPortSettings_window->deviceID(), funType, first_addr, values);

    unit.setProperty(tr("FIRST_ADDRESS"), first_addr);
    unit.setProperty(tr("LAST_ADDRESS"), last_addr);

    m_modbus->sendData(unit);
}
//---------------------------------------------------------------------------------------------------------
void ConfiguratorWindow::sendProtectionWorkModeRequest(const QString& protection, RequestFunction function,
                                                       DeviceMenuItemType group_index)
{
    int firstAddr = addressSettingKey("K10");

    CModBusDataUnit unit(m_serialPortSettings_window->deviceID(), CModBusDataUnit::ReadHoldingRegisters, firstAddr,
                         QVector<quint16>() << 48);

    unit.setProperty("REQUEST", PROTECTION_WORK_MODE_TYPE);
    unit.setProperty("PROTECTION", protection);
    unit.setProperty("REQUEST_FUNCTION", function);
    unit.setProperty("GROUP", group_index);

    m_modbus->sendData(unit);
}
//---------------------------------------------------------
void ConfiguratorWindow::sendMonitorPurposeK10_K11Request()
{
    int firstAddr = addressSettingKey("K10");

    CModBusDataUnit unit(m_serialPortSettings_window->deviceID(), CModBusDataUnit::ReadHoldingRegisters, firstAddr,
                       QVector<quint16>() << 48);

    unit.setProperty("REQUEST", MONITONR_PURPOSE_K10_K11_TYPE);

    m_modbus->sendData(unit);
}
/*!
 * \brief ConfiguratorWindow::sendRequestRead
 * \param addr Адрес ячейки
 * \param size Количество ячеек для чтения
 */
void ConfiguratorWindow::sendRequestRead(int addr, int size, int request, CModBusDataUnit::FunctionType functionType)
{
    CModBusDataUnit unit(m_serialPortSettings_window->deviceID(), functionType, addr, QVector<quint16>() << size);

    unit.setProperty(tr("REQUEST"), request);

    m_modbus->sendData(unit);
}
/*!
 * \brief ConfiguratorWindow::sendRequestWrite
 * \param addr Адрес ячейки
 * \param data Данные для записи
 */
void ConfiguratorWindow::sendRequestWrite(int addr, QVector<quint16>& values, int request)
{
    CModBusDataUnit::FunctionType funType = ((values.count() == 1)?CModBusDataUnit::WriteSingleRegister:
                                                                 CModBusDataUnit::WriteMultipleRegisters);

    CModBusDataUnit unit(m_serialPortSettings_window->deviceID(), funType, addr, values);

    unit.setProperty("REQUST", request);

    m_modbus->sendData(unit);
}
/*!
 * \brief ConfiguratorWindow::sendDeviceCommand
 *
 * Отправка команды устройству
 */
void ConfiguratorWindow::sendDeviceCommand(int cmd)
{
    CModBusDataUnit unit(m_serialPortSettings_window->deviceID(), CModBusDataUnit::WriteSingleRegister, 0x3000, QVector<quint16>() << cmd);

    unit.setProperty("CMD", cmd);
    m_modbus->sendData(unit);
}
/*!
 * \brief ConfiguratorWindow::sendOutputAllRequest
 *
 * Чтение состояния всех выходов (Светодиоды, Реле и Модифецируемые переменные)
 */
void ConfiguratorWindow::sendOutputAllRequest()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());

    QString type = "";

    if(button)
        type = button->property("TYPE").toString();

    CModBusDataUnit unit(m_serialPortSettings_window->deviceID(), CModBusDataUnit::ReadInputRegisters, 196, QVector<quint16>() << 4);

    unit.setProperty("REQUEST", READ_OUTPUT_ALL);
    unit.setProperty("BUTTON_TYPE", type);

    m_modbus->sendData(unit);
}
/*!
 * \brief ConfiguratorWindow::sendInputStatesRequest
 *
 * Чтение состояний входов
 */
void ConfiguratorWindow::sendInputStatesRequest()
{
    CModBusDataUnit unit(m_serialPortSettings_window->deviceID(), CModBusDataUnit::ReadInputRegisters, 200, QVector<quint16>() << 2);

    unit.setProperty("REQUEST", READ_INPUTS);

    m_modbus->sendData(unit);
}
//-----------------------------------------------------
void ConfiguratorWindow::sendDebugInfoRead(int channel)
{
    CModBusDataUnit unit(m_serialPortSettings_window->deviceID(), CModBusDataUnit::ReadInputRegisters, 202 + channel*15,
                       QVector<quint16>() << 15);

    unit.setProperty("REQUEST", READ_DEBUG_INFO);
    unit.setProperty("CHANNEL", channel);

    m_modbus->sendData(unit);
}
//-------------------------------------
void ConfiguratorWindow::clearIOTable()
{
    QTableView*        table = nullptr;
    DeviceMenuItemType index = menuIndex();

    switch(index)
    {
        case DEVICE_MENU_ITEM_SETTINGS_ITEM_LEDS:
            table = ui->tablewgtLedPurpose;
        break;

        case DEVICE_MENU_ITEM_SETTINGS_ITEM_IO_MDVV01_INPUTS:
            table = ui->tablewgtDiscreteInputPurpose;
        break;

        case DEVICE_MENU_ITEM_SETTINGS_ITEM_IO_MDVV01_RELAY:
            table = ui->tablewgtRelayPurpose;
        break;

        case DEVICE_MENU_ITEM_SETTINGS_ITEM_KEYBOARD:

        break;

        case DEVICE_MENU_ITEM_SETTINGS_ITEM_IO_PROTECTION:
            table = ui->tablewgtProtectionCtrl;
        break;

        default: return;
    }

    if(!table)
        return;

    CMatrixPurposeModel* model = static_cast<CMatrixPurposeModel*>(table->model());

    if(!model)
        return;

    CMatrix& matrix = model->matrix();

    for(int i = 0; i < matrix.rowCount(); i++)
    {
        for(int j = 0; j < matrix.columnCount(); j++)
        {
            matrix[i][j].data().state = UNCHECKED;
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

    m_active_journal_current->journalClear();
    m_active_journal_current->headerClear();

    m_status_bar->setStatusMessage(tr("Очистка таблицы журнала %1").arg(journal_name), 2000);
}
//--------------------------------------
void ConfiguratorWindow::menuPanelCtrl()
{
    if(ui->menuDeviceDockPanel->isHidden())
        ui->menuDeviceDockPanel->show();
    else
        ui->menuDeviceDockPanel->hide();
}
//------------------------------------------
void ConfiguratorWindow::variablePanelCtrl()
{
    if(ui->variableDockPanel->isHidden())
        ui->variableDockPanel->show();
    else
        ui->variableDockPanel->hide();
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

    if(!dir.exists("outputs/reports"))
    {
        dir.mkdir("outputs/reports");
    }

    QString selectedFilter    = tr("PDF (*.pdf)");
    QString journal_full_name = tr("Журнал %1-%2").arg(journal_name).arg(m_status_bar->serialNumberText());
    QString journal_path      = QFileDialog::getSaveFileName(this, tr("Экспорт журнала %1 в PDF").arg(journal_name),
                                                             dir.absolutePath() + QString("/outputs/reports/%1.%2").
                                                             arg(journal_full_name).arg("pdf"),
                                                             tr("PDF (*.pdf);;Все файлы (*.*)"), &selectedFilter);

    if(journal_path.isEmpty())
        return;

    m_progressbar->setProgressTitle(tr("Экспорт в PDF"));
    m_progressbar->progressStart();

    connect(m_watcher, &QFutureWatcher<void>::finished, m_progressbar, &CProgressBarWidget::progressStop);

    QFuture<void> future = QtConcurrent::run(this, &exportToPDF, m_active_journal_current,
                                             QString(tr("Журнал %1")).arg(journal_name),
                                             sn_device, journal_path);
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

    int count = 0;

    if(m_journal_set.find(key) != m_journal_set.end())
    {
        count = m_journal_set[key].message.read_total;
    }

    CFilter::FilterIntervalType tinterval = { count, 0, count };
    CFilter::FilterDateType     tdate     = { dBegin, dEnd };

    m_filter[key] = CFilter(tinterval, tdate);

    CFilterDialog* filterDlg = new CFilterDialog(m_filter[key], this);

    if(filterDlg->exec() == QDialog::Accepted)
    {
        CFilter filter = filterDlg->filter();
        m_filter[key] = filter;

        if(filter.type() == CFilter::DATE)
            filterJournal(filter);
    }

    delete filterDlg;
}
//----------------------------------------------
void ConfiguratorWindow::deviceDefaultSettings()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, tr("Отправка команды"),
                                  tr("Вы действительно хотите сбросить настройки устройства по умолчанию?"),
                                  QMessageBox::Yes | QMessageBox::No);

    if(reply == QMessageBox::Yes)
    {
        sendDeviceCommand(0x0001); // отправка команды на сброс настроек по умолчанию
        readSetCurrent(); // читаем настройки после сброса
    }
}
//-----------------------------------------
void ConfiguratorWindow::noConnectMessage()
{
    QMessageBox::warning(this, tr("Отправка данных"), tr("Невозможно отправить запрос. Нет соединения с устройством"));
}
/*!
 * \brief ConfiguratorWindow::setNewBaudrate
 * \param baudrate_index индекс скорости из списка
 * Отправка команды устройству для установки новой скорости
 */
void ConfiguratorWindow::setNewBaudrate(int baudrate_index)
{
    sendDeviceCommand(baudrate_index + 6);
}
/*!
 * \brief ConfiguratorWindow::saveDeviceSettings
 *
 * Отправка команды устройству на сохранение настроек в энергонезависимой памяти
 */
void ConfiguratorWindow::saveDeviceSettings()
{
    sendDeviceCommand(2);
}
/*!
 * \brief ConfiguratorWindow::dateDeviceChanged
 * \param date Новая дата
 *
 * Изменение вывода текущего дня при исправлении даты (установка новой даты для устройства)
 */
void ConfiguratorWindow::dateDeviceChanged(const QDate& date)
{
    ui->lineEditWeekDay->setText(date.toString("dddd"));
}
/*!
 * \brief ConfiguratorWindow::autospeedStateChanged
 * \param state Текущее состояние чекбокса подбора скорости
 */
void ConfiguratorWindow::autospeedStateChanged(bool state)
{
    m_modbus->channel()->setAutochoicespeed(state);
}
/*!
 * \brief ConfiguratorWindow::panelMoved
 * \param pos    Позиция виджета
 * \param index  Индекса виджета
 */
void ConfiguratorWindow::panelMoved(int pos, int index)
{
    Q_UNUSED(pos);

    QSplitter* splitter = qobject_cast<QSplitter*>(sender());
    QWidget*   widget   = splitter->widget(index);

    panelVisibleCtrl(widget);

    // Изменение ширины колонок при ресайзе (медленно работает перемещение панелей из-за перерасчета ширины колонок
//    resizeColumns();
}
/*!
 * \brief ConfiguratorWindow::panelButtonCtrlPress
 *
 * Обработка нажатия кнопок управления панелью
 */
void ConfiguratorWindow::panelButtonCtrlPress()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());

    if(!button)
        return;

    QList<int> sizes;

    if(button == ui->pushButtonPanelMessage)
        sizes = ui->splitterPanelMessage->sizes();
    else
        sizes = ui->splitterCentralWidget->sizes();

    if(sizes.count() == 3)
    {
        if(button == ui->pushButtonMenuDeviceCtrl)
        {
            if(ui->menuDeviceDockPanel->width() > ui->pushButtonMenuDeviceCtrl->minimumWidth())
            {
                int w = sizes[0];

                ui->menuDeviceDockPanel->setProperty("WIDTH", w);

                sizes[0] = ui->pushButtonMenuDeviceCtrl->minimumWidth();
                sizes[1] = sizes[1] + (w - sizes[0]);

                ui->splitterCentralWidget->setSizes(sizes);

                emit ui->splitterCentralWidget->splitterMoved(sizes[1], 1);
            }
            else if(ui->menuDeviceDockPanel->width() == ui->pushButtonMenuDeviceCtrl->minimumWidth())
            {
                int w        = sizes[0];
                int newWidth = ui->menuDeviceDockPanel->property("WIDTH").toInt();

                if(newWidth <= 0)
                    newWidth = ui->menuDeviceDockPanel->sizeHint().width();

                sizes[0] = newWidth;
                sizes[1] = sizes[1] - (newWidth - w);

                ui->splitterCentralWidget->setSizes(sizes);

                emit ui->splitterCentralWidget->splitterMoved(sizes[1], 1);
            }
        }
        else if(button == ui->pushButtonVariableCtrl)
        {
            if(ui->variableDockPanel->width() > ui->pushButtonVariableCtrl->minimumWidth())
            {
                int w = sizes[2];

                ui->variableDockPanel->setProperty("WIDTH", w);

                sizes[2] = ui->pushButtonVariableCtrl->minimumWidth();
                sizes[1] = sizes[1] + (w - sizes[2]);

                ui->splitterCentralWidget->setSizes(sizes);

                emit ui->splitterCentralWidget->splitterMoved(sizes[2], 2);
            }
            else if(ui->variableDockPanel->width() == ui->pushButtonVariableCtrl->minimumWidth())
            {
                int w        = sizes[2];
                int newWidth = ui->variableDockPanel->property("WIDTH").toInt();

                if(newWidth <= 0)
                    newWidth = ui->variableDockPanel->sizeHint().width();

                sizes[2] = newWidth;
                sizes[1] = sizes[1] - (newWidth - w);

                ui->splitterCentralWidget->setSizes(sizes);

                emit ui->splitterCentralWidget->splitterMoved(sizes[2], 2);
            }
        }
    }
    else if(sizes.count() == 2)
    {
        if(button == ui->pushButtonPanelMessage)
        {
            if(ui->framePanelMessage->height() > ui->pushButtonPanelMessage->height())
            {
                int h = sizes[1];

                ui->framePanelMessage->setProperty("HEIGHT", h);

                sizes[1] = ui->framePanelMessage->minimumHeight();
                sizes[0] = sizes[0] + (h - sizes[1]);

                ui->splitterPanelMessage->setSizes(sizes);

                emit ui->splitterPanelMessage->splitterMoved(sizes[0], 0);
            }
            else if(ui->framePanelMessage->height() == ui->pushButtonPanelMessage->height())
            {
                int h         = sizes[1];
                int newHeight = ui->framePanelMessage->property("HEIGHT").toInt();

                if(newHeight <= 0)
                    newHeight = ui->framePanelMessage->sizeHint().height();

                sizes[1] = newHeight;
                sizes[0] = sizes[0] - (newHeight - h);

                ui->splitterPanelMessage->setSizes(sizes);

                emit ui->splitterPanelMessage->splitterMoved(0, 0);
            }
        }
    }

    // Изменение ширины колонок при ресайзе
    resizeColumns();
}
/*!
 * \brief ConfiguratorWindow::filterJournal
 * \param filter Настройки фильтра
 *
 * Фильтрация текущей таблицы
 */
void ConfiguratorWindow::filterJournal(const CFilter& filter)
{
    if(!m_active_journal_current || !filter.time().isValid())
        return;

    CJournalTable* table = m_active_journal_current->table();

    if(!table || table->rowCount() == 0 || table->columnCount() < 3)
        return;

    int count = 0;
    int beg_num = -1, end_num = -1;

    for(int i = 0; i < table->rowCount(); i++)
    {
        QTableWidgetItem* itemDate = table->item(i, 1);
        QTableWidgetItem* itemTime = table->item(i, 2);

        if(!itemDate || !itemTime)
            continue;

        QDate fdate = QDate::fromString(itemDate->text(), "dd.MM.yyyy");
        QTime ftime = QTime::fromString(itemTime->text(), "hh:mm:ss.zzz");

        if(!fdate.isValid() || !ftime.isValid())
            continue;

        if(fdate < filter.date().begin || fdate > filter.date().end)
        {
            table->setRowHidden(i, true);
        }
        else if(fdate == filter.date().begin && ftime < filter.time())
            table->setRowHidden(i, true);
        else
        {
            if(beg_num == -1)
                beg_num = i; // первая видимая строка в таблице

            end_num = i; // последняя видимая строка в таблице
            count++;
            table->setRowHidden(i, false);
        }
    }

    QString text = QString("%1").arg(count);

    if(beg_num != -1 && end_num != -1)
    {
        QTableWidgetItem* itemBeg = table->item(beg_num, 1);
        QTableWidgetItem* itemEnd = table->item(end_num, 1);

        if(itemBeg && itemEnd)
        {
            text = QString("%1 - %2/%3").arg(itemBeg->text()).arg(itemEnd->text()).arg(count);
        }
    }

    m_active_journal_current->header()->setTextTableCountMessages(text);
}
//----------------------------------------
void ConfiguratorWindow::stopProgressbar()
{
    m_progressbar->progressStop();
}
//-------------------------------------------
void ConfiguratorWindow::timeoutJournalRead()
{
    m_journal_timer->stop();

    if(!m_modbus->isConnected())
    {
        ui->pushButtonJournalRead->setChecked(false);
        m_progressbar->progressStop();

        QString nameJournal;

        if(m_journal_read_current)
            nameJournal = m_journal_read_current->property("NAME").toString();

        QMessageBox::warning(this, tr("Чтение журнала"), tr("Ошибка чтения журнала %1.").arg(nameJournal));
    }
}
//--------------------------------------------
void ConfiguratorWindow::testStyle(bool state)
{
    QString path_to_style = "";

    if(state)
    {
        path_to_style = "styles/custom_style.qss";
    }
    else
    {
        path_to_style = ":/styles/resource/styles/default_style.qss";
    }

    QFile styleFile(path_to_style);

    styleFile.open(QFile::ReadOnly);

    qApp->setStyleSheet(styleFile.readAll());

    styleFile.close();
}
/*!
 * \brief ConfiguratorWindow::readStatusInfo
 *
 * Запрос чтения информации о статусах
 */
void ConfiguratorWindow::readStatusInfo()
{
    sendRequestRead(16, 2, READ_STATUS_MCP_INFO, CModBusDataUnit::ReadInputRegisters);
    sendRequestRead(538, 24, READ_STATUS_MODULE_INFO, CModBusDataUnit::ReadInputRegisters);
}
/*!
 * \brief ConfiguratorWindow::updateSerialPortSettings
 *
 * Обнолвение параметров настроек последовательного порта и протокола Modbus при их изменении в окне настроек
 */
void ConfiguratorWindow::updateSerialPortSettings()
{
    m_modbus->setIntervalSilence(m_serialPortSettings_window->modbusIntervalSilence());
    m_modbus->setIntervalResponce(m_serialPortSettings_window->modbusTimeout());
    m_modbus->setTryCount(m_serialPortSettings_window->modbusTryCount());

    int index = ui->comboBoxCommunicationBaudrate->findText(QString("%1").arg(m_serialPortSettings_window->baudrate()));

    if(index != -1)
        ui->comboBoxCommunicationBaudrate->setCurrentIndex(index);
}
//--------------------------------------
void ConfiguratorWindow::setNewAddress()
{
    m_timer_new_address_set->stop();
    disconnect(m_timer_new_address_set, &QTimer::timeout, this, &ConfiguratorWindow::setNewAddress);
    sendDeviceCommand(ui->comboBoxCommunicationBaudrate->currentIndex() + 6); // новая скорость
    sendDeviceCommand(19); // установить новый адрес MODBUS
    sendDeviceCommand(2);
}
//---------------------------------------
void ConfiguratorWindow::expandedWindow()
{
    Qt::WindowStates states = windowState();

    if(!(states & Qt::WindowMaximized))
        showMaximized();
    else
        showNormal();
}
//--------------------------------------------
void ConfiguratorWindow::mouseMove(QPoint pos)
{
    if(!(windowState() & Qt::WindowMaximized))
    {
        QPoint mouse_pos = ui->widgetMenuBar->mousePosition();

        int x = pos.x() - mouse_pos.x() + geometry().left() - ui->widgetMenuBar->geometry().left();
        int y = pos.y() - mouse_pos.y() + geometry().top() - ui->widgetMenuBar->geometry().top();

        move(x,y);
    }
}
//-----------------------------------
void ConfiguratorWindow::newProject()
{
    m_popup->setPopupText(tr("Эта функция находится на стадии разработки!"));
    m_popup->show();
}
//------------------------------------
void ConfiguratorWindow::openProject()
{
    QDir dir;
    QString fileName = QFileDialog::getOpenFileName(this, tr("Открытие файла проекта"),
                                                    QString(dir.absolutePath() + "/%1/%2").arg("outputs/projects").
                                                    arg("project"), tr("Проекты (*.prj)"));

    if(fileName.isEmpty())
        return;

    QFile file(fileName);

    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox::warning(this, tr("Открытие файла проекта"), tr("Невозможно открыть файл проекта: ").
                                                                 arg(file.errorString()));

        return;
    }

    QJsonDocument jsonDocument = QJsonDocument::fromJson(file.readAll());

    file.close();

    if(!jsonDocument.isObject())
    {
        qWarning() << tr("Неопознаный формат файла проекта.");
        return;
    }

    QJsonObject objCalibration = jsonDocument.object();
    QJsonValue  valCalibration = objCalibration.value("calibration");

    if(!valCalibration.isObject())
    {
        qWarning() << tr("Не найден объект <calibration> в файле проекта.");
        return;
    }

    QJsonObject objCurrent = valCalibration.toObject();
    QJsonValue  valCurrent = objCurrent.value("current");

    if(!valCalibration.isObject())
    {
        qWarning() << tr("Не найден объект <current> в файле проекта.");
        return;
    }

    QJsonObject objStandard = valCurrent.toObject();
    QJsonValue  valStandard = objStandard.value("standard");

    if(!valStandard.isObject())
    {
        qWarning() << tr("Не найден объект <standard> в файле проекта.");
        return;
    }

    QJsonObject objStandardField      = valStandard.toObject();
    QJsonValue  valStandardFieldPhase = objStandardField.value("phase");
    QJsonValue  valStandardField3I0   = objStandardField.value("3I0");

    if(valStandardFieldPhase.isString())
        ui->widgetCalibrationOfCurrent->setCurrentStandardPhase(valStandardFieldPhase.toString().toFloat());
    if(valStandardField3I0.isString())
        ui->widgetCalibrationOfCurrent->setCurrentStandard3I0(valStandardField3I0.toString().toFloat());

    emit ui->widgetMenuBar->widgetMenu()->addDocument(fileName);
}
//------------------------------------
void ConfiguratorWindow::saveProject()
{
    QDir dir;

    if(!dir.exists("outputs/projects"))
        dir.mkdir("outputs/projects");

    QString fileName = QFileDialog::getSaveFileName(this, tr("Запись файла проекта"),
                                                    QString(dir.absolutePath() + "/%1/%2").
                                                    arg("outputs/projects").arg("project.prj"),
                                                    tr("Файлы проектов (*.prj)"));

    if(fileName.isEmpty())
        return;

    QFile file(fileName);

    if(!file.open(QFile::WriteOnly))
    {
        QMessageBox::warning(this, tr("Сохранение файла проекта"), tr("Невозможно создать файл для записи \"%1\"").
                                                                   arg(fileName));
        return;
    }

    QString textStandardPhase = QString::number(ui->widgetCalibrationOfCurrent->calibrationCurrentStandardPhase(), 'f', 6);
    QString textStandard3I0 = QString::number(ui->widgetCalibrationOfCurrent->calibrationCurrentStandard3I0(), 'f', 6);
    QString textStandard = QString("\"standard\":\n\t\t\t{\n\t\t\t\t\"phase\": \"%1\","
                                   "\n\t\t\t\t\"3I0\": \"%2\"\n\t\t\t}").arg(textStandardPhase).arg(textStandard3I0);
    QString textCurrent     = QString("\"current\":\n\t\t{\n\t\t\t%1\n\t\t}").arg(textStandard);
    QString textCalibration = QString("\"calibration\":\n\t{\n\t\t%1\n\t}").arg(textCurrent);
    QString text            = QString("{\n\t%1\n}").arg(textCalibration);

    file.write(text.toStdString().c_str());
    file.close();
}
//--------------------------------------
void ConfiguratorWindow::saveAsProject()
{
    m_popup->setPopupText(tr("Эта функция находится на стадии разработки!"));
    m_popup->show();
}
//-------------------------------------------
void ConfiguratorWindow::exportToPDFProject()
{
    m_popup->setPopupText(tr("Эта функция находится на стадии разработки!"));
    m_popup->show();
}
//---------------------------------------------
void ConfiguratorWindow::exportToExcelProject()
{
    qDebug() << tr("Экспорт уставок в Excel");
    QDir dir;

    if(!dir.exists("outputs/excel"))
        dir.mkdir("outputs/excel");

    QString fileName = QFileDialog::getSaveFileName(this, tr("Экспорт в Excel"),
                                                    QString(dir.absolutePath() + "/%1/%2").
                                                    arg("outputs/excel").arg("settings"),
                                                    tr("Excel (*.xlsx)"));

    if(fileName.isEmpty())
        return;

    m_progressbar->setProgressTitle(tr("Экспорт уставок в Excel"));
    m_progressbar->setSettings(0, 50);
    m_progressbar->progressStart();

    QXlsx::Document xlsx;

    xlsx.addSheet(tr("Защиты"));

    xlsx.write("A1", tr("Имя"));
    xlsx.write("B1", tr("Параметр"));
    xlsx.write("C1", tr("Значение"));
    xlsx.write("D1", tr("Диапазон"));

    QXlsx::Format format_header;
    format_header.setFontBold(true);
    format_header.setHorizontalAlignment(QXlsx::Format::AlignHCenter);
    format_header.setVerticalAlignment(QXlsx::Format::AlignBottom);

    xlsx.setColumnFormat("A1:D1", format_header);

    xlsx.setColumnWidth("A1", 30);
    xlsx.setColumnWidth("B1", 50);
    xlsx.setColumnWidth("C1", 20);
    xlsx.setColumnWidth("D1", 50);

//    writeDataToExcel(xlsx, tr("МТЗ1"), ui->gridLayoutMTZ1);
//    writeDataToExcel(xlsx, tr("МТЗ2"), ui->gridLayoutMTZ2);
//    writeDataToExcel(xlsx, tr("МТЗ3"), ui->gridLayoutMTZ3);
//    writeDataToExcel(xlsx, "", ui->gridLayoutMTZ3_Steep);
//    writeDataToExcel(xlsx, "", ui->gridLayoutMTZ3_Sloping);
//    writeDataToExcel(xlsx, "", ui->gridLayoutMTZ3_Inverse);
//    writeDataToExcel(xlsx, "", ui->gridLayoutMTZ3_DurInverse);
//    writeDataToExcel(xlsx, "", ui->gridLayoutMTZ3_Back);
//    writeDataToExcel(xlsx, "", ui->gridLayoutMTZ3_StrInverse);
//    writeDataToExcel(xlsx, "", ui->gridLayoutMTZ3_ExtInverse);
//    writeDataToExcel(xlsx, tr("МТЗ4"), ui->gridLayoutMTZ4);
//    writeDataToExcel(xlsx, tr("ОЗЗ1"), ui->gridLayoutOZZ1);
//    writeDataToExcel(xlsx, tr("ОЗЗ2"), ui->gridLayoutOZZ2);
//    writeDataToExcel(xlsx, tr("НЗЗ1"), ui->gridLayoutNZZ1);
//    writeDataToExcel(xlsx, tr("НЗЗ2"), ui->gridLayoutNZZ2);
//    writeDataToExcel(xlsx, tr("Umax1"), ui->gridLayoutUmax1);
//    writeDataToExcel(xlsx, tr("Umax2"), ui->gridLayoutUmax2);
//    writeDataToExcel(xlsx, tr("Umin1"), ui->gridLayoutUmin1);
//    writeDataToExcel(xlsx, "", ui->gridLayoutDisconnectors); // корректировка КЦУ (читаются настройки Автоматика->Выключатель)
//    writeDataToExcel(xlsx, tr("Umin2"), ui->gridLayoutUmin2);
//    writeDataToExcel(xlsx, "", ui->gridLayoutDisconnectors); // корректировка КЦУ (читаются настройки Автоматика->Выключатель)
//    writeDataToExcel(xlsx, tr("3U0"), ui->gridLayout3U0);
//    writeDataToExcel(xlsx, tr("АЧР1"), ui->gridLayoutACHR1);
//    writeDataToExcel(xlsx, tr("АЧР2"), ui->gridLayoutACHR2);
//    writeDataToExcel(xlsx, tr("АЧР3"), ui->gridLayoutACHR3);
//    writeDataToExcel(xlsx, tr("Дуговая"), ui->gridLayoutArc);
//    writeDataToExcel(xlsx, tr("Внешняя1"), ui->gridLayoutExt1);
//    writeDataToExcel(xlsx, tr("Внешняя2"), ui->gridLayoutExt2);
//    writeDataToExcel(xlsx, tr("Внешняя3"), ui->gridLayoutExt3);
//    writeDataToExcel(xlsx, tr("Пусковая"), ui->gridLayoutMotorStarting);
//    writeDataToExcel(xlsx, tr("Imin"), ui->gridLayoutMotorImin);
//    writeDataToExcel(xlsx, tr("Температурная1"), ui->gridLayoutTemp1);
//    writeDataToExcel(xlsx, tr("Температурная2"), ui->gridLayoutTemp2);
//    writeDataToExcel(xlsx, tr("Уровневая1"), ui->gridLayoutLevel1);
//    writeDataToExcel(xlsx, tr("Уровневая2"), ui->gridLayoutLevel2);
//    writeDataToExcel(xlsx, tr("Сигнал пуска"), ui->gridLayoutSignalStart);
//    writeDataToExcel(xlsx, tr("БРУ"), ui->gridLayoutBRU);
//    writeDataToExcel(xlsx, tr("Вакуум"), ui->gridLayoutVacuum);

//    xlsx.addSheet(tr("Автоматика"));

//    xlsx.write("A1", tr("Имя"));
//    xlsx.write("B1", tr("Параметр"));
//    xlsx.write("C1", tr("Значение"));
//    xlsx.write("D1", tr("Диапазон"));
//    xlsx.setColumnFormat("A1:D1", format_header);
//    xlsx.setColumnWidth("A1", 30);
//    xlsx.setColumnWidth("B1", 50);
//    xlsx.setColumnWidth("C1", 20);
//    xlsx.setColumnWidth("D1", 50);

//    writeDataToExcel(xlsx, tr("Выключатель"), ui->gridLayoutDisconnectors, -1);
//    writeDataToExcel(xlsx, tr("Тележка выключателя"), ui->gridLayoutDisconnectorTruck);
//    writeDataToExcel(xlsx, tr("Блокировки"), ui->gridLayoutBlock);
//    writeDataToExcel(xlsx, tr("Шинный разъединитель"), ui->gridLayoutDisconnectBus);
//    writeDataToExcel(xlsx, tr("Линейный разъединитель"), ui->gridLayoutDisconnectLine);
//    writeDataToExcel(xlsx, tr("Заземляющий разъединитель"), ui->gridLayoutDisconnectEarth);
//    writeDataToExcel(xlsx, tr("Контроль ТН"), ui->gridLayoutCtrlTN);
//    writeDataToExcel(xlsx, tr("АВР"), ui->gridLayoutAVR);
//    writeDataToExcel(xlsx, tr("АПВ"), ui->gridLayoutAPV);
//    writeDataToExcel(xlsx, "", ui->gridLayoutAPVSignalStart);

//    xlsx.addSheet(tr("Аналоговые входы"));

//    xlsx.write("A1", tr("Имя"));
//    xlsx.write("B1", tr("Параметр"));
//    xlsx.write("C1", tr("Значение"));
//    xlsx.write("D1", tr("Диапазон"));
//    xlsx.setColumnFormat("A1:D1", format_header);
//    xlsx.setColumnWidth("A1", 30);
//    xlsx.setColumnWidth("B1", 50);
//    xlsx.setColumnWidth("C1", 20);
//    xlsx.setColumnWidth("D1", 50);

//    writeDataToExcel(xlsx, tr("Основные"), ui->gridLayoutInAnalogMain, -1);
//    writeDataToExcel(xlsx, tr("Калибровки"), ui->gridLayoutInAnalogCalibration);

//    xlsx.selectSheet(tr("Защиты"));
//    xlsx.saveAs(fileName);

    m_progressbar->progressStop();
}
//-----------------------------------------------
void ConfiguratorWindow::importFromExcelProject()
{
    m_popup->setPopupText(tr("Эта функция находится на стадии разработки!"));
    m_popup->show();

    QDir dir;

    QString fileName = QFileDialog::getOpenFileName(this, tr("Импорт уставок из Excel"),
                                                    QString(dir.absolutePath() + "/%1/%2").arg("outputs/excel").
                                                    arg("settings"),
                                                    tr("Excel (*.xlsx)"));

    if(fileName.isEmpty())
        return;

    m_progressbar->setProgressTitle(tr("Импорт уставок из Excel"));
    m_progressbar->setSettings(0, 50);
    m_progressbar->progressStart();

    QXlsx::Document xlsx(fileName);

    if(!xlsx.selectSheet(tr("Защиты")))
    {
        qWarning() << tr("Импорт данных из файла Excel: лист <Защиты> не найден.");
        return;
    }

//    readDataFromExcel(xlsx, tr("МТЗ1"), ui->gridLayoutMTZ1);
//    readDataFromExcel(xlsx, tr("МТЗ2"), ui->gridLayoutMTZ2);
//    readDataFromExcel(xlsx, tr("МТЗ3"), ui->gridLayoutMTZ3);
//    readDataFromExcel(xlsx, "", ui->gridLayoutMTZ3_Steep);
//    readDataFromExcel(xlsx, "", ui->gridLayoutMTZ3_Sloping);
//    readDataFromExcel(xlsx, "", ui->gridLayoutMTZ3_Inverse);
//    readDataFromExcel(xlsx, "", ui->gridLayoutMTZ3_DurInverse);
//    readDataFromExcel(xlsx, "", ui->gridLayoutMTZ3_Back);
//    readDataFromExcel(xlsx, "", ui->gridLayoutMTZ3_StrInverse);
//    readDataFromExcel(xlsx, "", ui->gridLayoutMTZ3_ExtInverse);
//    readDataFromExcel(xlsx, tr("МТЗ4"), ui->gridLayoutMTZ4);
//    readDataFromExcel(xlsx, tr("ОЗЗ1"), ui->gridLayoutOZZ1);
//    readDataFromExcel(xlsx, tr("ОЗЗ2"), ui->gridLayoutOZZ2);
//    readDataFromExcel(xlsx, tr("НЗЗ1"), ui->gridLayoutNZZ1);
//    readDataFromExcel(xlsx, tr("НЗЗ2"), ui->gridLayoutNZZ2);
//    readDataFromExcel(xlsx, tr("Umax1"), ui->gridLayoutUmax1);
//    readDataFromExcel(xlsx, tr("Umax2"), ui->gridLayoutUmax2);
//    readDataFromExcel(xlsx, tr("Umin1"), ui->gridLayoutUmin1);
//    readDataFromExcel(xlsx, "", ui->gridLayoutDisconnectors); // корректировка КЦУ (читаются настройки Автоматика->Выключатель)
//    readDataFromExcel(xlsx, tr("Umin2"), ui->gridLayoutUmin2);
//    readDataFromExcel(xlsx, "", ui->gridLayoutDisconnectors); // корректировка КЦУ (читаются настройки Автоматика->Выключатель)
//    readDataFromExcel(xlsx, tr("3U0"), ui->gridLayout3U0);
//    readDataFromExcel(xlsx, tr("АЧР1"), ui->gridLayoutACHR1);
//    readDataFromExcel(xlsx, tr("АЧР2"), ui->gridLayoutACHR2);
//    readDataFromExcel(xlsx, tr("АЧР3"), ui->gridLayoutACHR3);
//    readDataFromExcel(xlsx, tr("Дуговая"), ui->gridLayoutArc);
//    readDataFromExcel(xlsx, tr("Внешняя1"), ui->gridLayoutExt1);
//    readDataFromExcel(xlsx, tr("Внешняя2"), ui->gridLayoutExt2);
//    readDataFromExcel(xlsx, tr("Внешняя3"), ui->gridLayoutExt3);
//    readDataFromExcel(xlsx, tr("Пусковая"), ui->gridLayoutMotorStarting);
//    readDataFromExcel(xlsx, tr("Imin"), ui->gridLayoutMotorImin);
//    readDataFromExcel(xlsx, tr("Температурная1"), ui->gridLayoutTemp1);
//    readDataFromExcel(xlsx, tr("Температурная2"), ui->gridLayoutTemp2);
//    readDataFromExcel(xlsx, tr("Уровневая1"), ui->gridLayoutLevel1);
//    readDataFromExcel(xlsx, tr("Уровневая2"), ui->gridLayoutLevel2);
//    readDataFromExcel(xlsx, tr("Сигнал пуска"), ui->gridLayoutSignalStart);
//    readDataFromExcel(xlsx, tr("БРУ"), ui->gridLayoutBRU);
//    readDataFromExcel(xlsx, tr("Вакуум"), ui->gridLayoutVacuum);

//    if(!xlsx.selectSheet(tr("Автоматика")))
//    {
//        qWarning() << tr("Импорт данных из файла Excel: лист <Автоматика> не найден.");
//        return;
//    }

//    readDataFromExcel(xlsx, tr("Выключатель"), ui->gridLayoutDisconnectors);
//    readDataFromExcel(xlsx, tr("Тележка выключателя"), ui->gridLayoutDisconnectorTruck);
//    readDataFromExcel(xlsx, tr("Блокировки"), ui->gridLayoutBlock);
//    readDataFromExcel(xlsx, tr("Шинный разъединитель"), ui->gridLayoutDisconnectBus);
//    readDataFromExcel(xlsx, tr("Линейный разъединитель"), ui->gridLayoutDisconnectLine);
//    readDataFromExcel(xlsx, tr("Заземляющий разъединитель"), ui->gridLayoutDisconnectEarth);
//    readDataFromExcel(xlsx, tr("Контроль ТН"), ui->gridLayoutCtrlTN);
//    readDataFromExcel(xlsx, tr("АВР"), ui->gridLayoutAVR);
//    readDataFromExcel(xlsx, tr("АПВ"), ui->gridLayoutAPV);
//    readDataFromExcel(xlsx, "", ui->gridLayoutAPVSignalStart);

//    if(!xlsx.selectSheet(tr("Аналоговые входы")))
//    {
//        qWarning() << tr("Импорт данных из файла Excel: лист <Аналоговые входы> не найден.");
//        return;
//    }

//    readDataFromExcel(xlsx, tr("Основные"), ui->gridLayoutInAnalogMain);
//    readDataFromExcel(xlsx, tr("Калибровки"), ui->gridLayoutInAnalogCalibration);

    m_progressbar->progressStop();
}
//-------------------------------------
void ConfiguratorWindow::closeProject()
{
    m_popup->setPopupText(tr("Эта функция находится на стадии разработки!"));
    m_popup->show();
}
//--------------------------------------------------
void ConfiguratorWindow::minimizeTabMenu(bool state)
{
    Q_UNUSED(state);
}
//--------------------------------------------------------
void ConfiguratorWindow::panelMessageVisiblity(bool state)
{
    if(state)
        ui->framePanelMessage->show();
    else
        ui->framePanelMessage->hide();
}
/*!
 * \brief ConfiguratorWindow::calibrationOfCurrent
 *
 * Слот для запуска калибровки токовых коэффициентов
 */
void ConfiguratorWindow::calibrationOfCurrent()
{
    m_calib_of_current = { 0, 0, 0, QVector<CModBusDataUnit>(0), nullptr };

    QCheckBox* checkBoxIa  = ui->widgetCalibrationOfCurrent->ctrlIa();
    QCheckBox* checkBoxIb  = ui->widgetCalibrationOfCurrent->ctrlIb();
    QCheckBox* checkBoxIc  = ui->widgetCalibrationOfCurrent->ctrlIc();
    QCheckBox* checkBox3I0 = ui->widgetCalibrationOfCurrent->ctrl3I0();

    if(!checkBoxIa->isChecked() && !checkBoxIb->isChecked() && !checkBoxIc->isChecked() &&
       !checkBox3I0->isChecked())
    {
        QMessageBox::warning(this, tr("Калибровка по току"), tr("Нет выбранных каналов для калибровки"));
        return;
    }

    CModBusDataUnit unit_Ia(m_serialPortSettings_window->deviceID(), CModBusDataUnit::ReadInputRegisters, 64, 2);
    CModBusDataUnit unit_Ib(m_serialPortSettings_window->deviceID(), CModBusDataUnit::ReadInputRegisters, 66, 2);
    CModBusDataUnit unit_Ic(m_serialPortSettings_window->deviceID(), CModBusDataUnit::ReadInputRegisters, 68, 2);
    CModBusDataUnit unit_3I0(m_serialPortSettings_window->deviceID(), CModBusDataUnit::ReadInputRegisters, 70, 2);

    unit_Ia.setProperty("REQUEST", CALIBRATION_CURRENT_IA);
    unit_Ib.setProperty("REQUEST", CALIBRATION_CURRENT_IB);
    unit_Ic.setProperty("REQUEST", CALIBRATION_CURRENT_IC);
    unit_3I0.setProperty("REQUEST", CALIBRATION_CURRENT_3I0);

    m_calib_of_current.request_all = ui->widgetCalibrationOfCurrent->dataSetCount();
    m_calib_of_current.pause       = ui->widgetCalibrationOfCurrent->timePauseRequest();
    m_calib_of_current.timer       = new QTimer;

    connect(m_calib_of_current.timer, &QTimer::timeout, this, &ConfiguratorWindow::calibrationOfCurrentRequest);

    if(checkBoxIa->isChecked())
        m_calib_of_current.units << unit_Ia;
    if(checkBoxIb->isChecked())
        m_calib_of_current.units << unit_Ib;
    if(checkBoxIc->isChecked())
        m_calib_of_current.units << unit_Ic;
    if(checkBox3I0->isChecked())
        m_calib_of_current.units << unit_3I0;

    inputAnalogCalibrateRead();
    calibrationOfCurrentRequest();
}
/*!
 * \brief ConfiguratorWindow::displayCalibrationOfCurrent
 *
 * Расчет и вывод новых калибровочных коэффициентов
 */
void ConfiguratorWindow::displayCalibrationOfCurrent()
{
    CCalibrationWidget::calibration_current_t calib = ui->widgetCalibrationOfCurrent->calibrationCurrent();

    qInfo() << tr("Калибровка по току:");
    if(!calib.Ia.isEmpty())
    {
        CLineEdit* lineEdit = qobject_cast<CLineEdit*>(groupMenuCellWidget(ui->tableWidgetSettingsAnalogGroupGeneral,
                                                                           "lineEditKIA", 1));

        float   standard   = ui->widgetCalibrationOfCurrent->calibrationCurrentStandardPhase();
        float   cur_factor = QLocale::system().toFloat(((lineEdit)?lineEdit->text():"0.0"));
        float   newFactor  = newCalibrationOfCurrentFactor(standard, cur_factor, calib.Ia);
        QPointF deviation  = standardDeviation(calib.Ia);

        ui->widgetCalibrationOfCurrent->setFactorIa(newFactor);
        ui->widgetCalibrationOfCurrent->setMeasureIa(deviation.x());
        ui->widgetCalibrationOfCurrent->setDeviationIa(deviation.y());
        qInfo() << tr("Калибровка тока фазы А");
        for(float value: calib.Ia)
            qInfo() << QString("value: %1").arg(QLocale::system().toString(value, 'f', 6));
        qInfo() << QString("Среднее арифметическое: %1 / Среднеквадратическое отклонение: %2").
                   arg(QLocale::system().toString(deviation.x(), 'f', 6)).
                   arg(QLocale::system().toString(deviation.y(), 'f', 6));
        qInfo() << tr("Старое калибровочное значение: %1").arg(cur_factor);
        qInfo() << tr("Новое калибровочное значение: %1").arg(QLocale::system().toString(newFactor, 'f', 6));
    }

    if(!calib.Ib.isEmpty())
    {
        CLineEdit* lineEdit = qobject_cast<CLineEdit*>(groupMenuCellWidget(ui->tableWidgetSettingsAnalogGroupGeneral,
                                                                           "lineEditKIB", 1));

        float   standard   = ui->widgetCalibrationOfCurrent->calibrationCurrentStandardPhase();
        float   cur_factor = QLocale::system().toFloat(((lineEdit)?lineEdit->text():"0.0"));
        float   newFactor  = newCalibrationOfCurrentFactor(standard, cur_factor, calib.Ib);
        QPointF deviation  = standardDeviation(calib.Ib);

        ui->widgetCalibrationOfCurrent->setFactorIb(newFactor);
        ui->widgetCalibrationOfCurrent->setMeasureIb(deviation.x());
        ui->widgetCalibrationOfCurrent->setDeviationIb(deviation.y());
        qInfo() << tr("Калибровка тока фазы B");
        for(float value: calib.Ib)
            qInfo() << QString("value: %1").arg(QLocale::system().toString(value, 'f', 6));
        qInfo() << QString("Среднее арифметическое: %1 / Среднеквадратическое отклонение: %2").
                   arg(QLocale::system().toString(deviation.x(), 'f', 6)).
                   arg(QLocale::system().toString(deviation.y(), 'f', 6));
        qInfo() << tr("Старое калибровочное значение: %1").arg(cur_factor);
        qInfo() << tr("Новое калибровочное значение: %1").arg(QLocale::system().toString(newFactor, 'f', 6));
    }

    if(!calib.Ic.isEmpty())
    {
        CLineEdit* lineEdit = qobject_cast<CLineEdit*>(groupMenuCellWidget(ui->tableWidgetSettingsAnalogGroupGeneral,
                                                                           "lineEditKIC", 1));

        float   standard   = ui->widgetCalibrationOfCurrent->calibrationCurrentStandardPhase();
        float   cur_factor = QLocale::system().toFloat(((lineEdit)?lineEdit->text():"0.0"));
        float   newFactor  = newCalibrationOfCurrentFactor(standard, cur_factor, calib.Ic);
        QPointF deviation  = standardDeviation(calib.Ic);

        ui->widgetCalibrationOfCurrent->setFactorIc(newFactor);
        ui->widgetCalibrationOfCurrent->setMeasureIc(deviation.x());
        ui->widgetCalibrationOfCurrent->setDeviationIc(deviation.y());
        qInfo() << tr("Калибровка тока фазы C");
        for(float value: calib.Ic)
            qInfo() << QString("Значение: %1").arg(QLocale::system().toString(value, 'f', 6));
        qInfo() << QString("Среднее арифметическое: %1 / Среднеквадратическое отклонение: %2").
                   arg(QLocale::system().toString(deviation.x(), 'f', 6)).
                   arg(QLocale::system().toString(deviation.y(), 'f', 6));
        qInfo() << tr("Старое калибровочное значение: %1").arg(cur_factor);
        qInfo() << tr("Новое калибровочное значение: %1").arg(QLocale::system().toString(newFactor, 'f', 6));
    }

    if(!calib._3I0.isEmpty())
    {
        CLineEdit* lineEdit = qobject_cast<CLineEdit*>(groupMenuCellWidget(ui->tableWidgetSettingsAnalogGroupGeneral,
                                                                           "lineEdit3I0", 1));

        float   standard   = ui->widgetCalibrationOfCurrent->calibrationCurrentStandard3I0();
        float   cur_factor = QLocale::system().toFloat(((lineEdit)?lineEdit->text():"0.0"));
        float   newFactor  = newCalibrationOfCurrentFactor(standard, cur_factor, calib._3I0);
        QPointF deviation  = standardDeviation(calib._3I0);

        ui->widgetCalibrationOfCurrent->setFactor3I0(newFactor);
        ui->widgetCalibrationOfCurrent->setMeasure3I0(deviation.x());
        ui->widgetCalibrationOfCurrent->setDeviation3I0(deviation.y());
        qInfo() << tr("Калибровка среднего тока 3I0");
        for(float value: calib._3I0)
            qInfo() << QString("value: %1").arg(QLocale::system().toString(value, 'f', 6));
        qInfo() << QString("Среднее арифметическое: %1 / Среднеквадратическое отклонение: %2").
                   arg(QLocale::system().toString(deviation.x(), 'f', 6)).
                   arg(QLocale::system().toString(deviation.y(), 'f', 6));
        qInfo() << tr("Старое калибровочное значение: %1").arg(cur_factor);
        qInfo() << tr("Новое калибровочное значение: %1").arg(QLocale::system().toString(newFactor, 'f', 6));
    }

    ui->widgetCalibrationOfCurrent->calibrationCurrentClear();
    emit ui->widgetCalibrationOfCurrent->calibrationEnd(false);
}
/*!
 * \brief ConfiguratorWindow::calibrationOfCurrentRequest
 *
 * Отправка запросов на калибровку по току
 */
void ConfiguratorWindow::calibrationOfCurrentRequest()
{
    m_calib_of_current.timer->stop();

    if(m_calib_of_current.request_all == 0)
        m_calib_of_current.request_all = 1;

    if(m_calib_of_current.request_count < m_calib_of_current.request_all)
    {
        for(CModBusDataUnit& unit: m_calib_of_current.units)
            m_modbus->sendData(unit);

        m_calib_of_current.request_count++;
        m_calib_of_current.timer->start(m_calib_of_current.pause);
    }
    else
    {
        disconnect(m_calib_of_current.timer, &QTimer::timeout, this,
                   &ConfiguratorWindow::calibrationOfCurrentRequest);
        delete m_calib_of_current.timer;
        displayCalibrationOfCurrent();
    }
}
//--------------------------------------
void ConfiguratorWindow::resizeColumns()
{
    DeviceMenuItemType index = menuIndex();

    if(index == -1)
        return;

    QTableView* table = nullptr;
    QString     str   = "";

    if(index == DEVICE_MENU_ITEM_SETTINGS_ITEM_LEDS)
    {
        table = ui->tablewgtLedPurpose;
        str   = tr("Светодиод");
    }
    else if(index == DEVICE_MENU_ITEM_SETTINGS_ITEM_IO_MDVV01_INPUTS)
    {
        table = ui->tablewgtDiscreteInputPurpose;
        str   = tr("Вход");
    }
    else if(index == DEVICE_MENU_ITEM_SETTINGS_ITEM_IO_MDVV01_RELAY)
    {
        table = ui->tablewgtRelayPurpose;
        str   = tr("Реле");
    }

    if(!table)
        return;

    QFontMetrics            fm_table = table->fontMetrics();
    HierarchicalHeaderView* header   = static_cast<HierarchicalHeaderView*>(table->horizontalHeader());

    int w_header = header->width()/table->model()->columnCount();

    for(int i = 0; i < table->model()->columnCount(); i++)
    {
        QString text = QString("%1 %2").arg(str).arg(header->model()->headerData(i, Qt::Horizontal).toString());

        int w_text = fm_table.width(text)*1.2f;

        header->resizeSection(i, ((w_header < w_text)?w_text:w_header));
    }
}
//------------------------------------------------------
void ConfiguratorWindow::keyPressEvent(QKeyEvent* event)
{
    if(event->key() == Qt::Key_F7)
    {
        testStyle(true);
    }
}
/*!
 * \brief ConfiguratorWindow::createJournalTable
 * \return Возвращает true, если таблица успешно создана
 */
bool ConfiguratorWindow::createJournalTable(QSqlDatabase* db, const QString& journal_type)
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

    if(journal_type == "EVENT")
    {
        // создание таблицы для хранения журналов событий
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
            QMessageBox::warning(this, tr("Создание таблицы"), tr("Невозможно создать таблицу журналов событий: ") +
                                       query.lastError().text());
            return false;
        }
    }
    else if(journal_type == "CRASH")
    {
        // создание таблицы для хранения журналов аварий
        db_str = "CREATE TABLE journals ("
                 "id_msg INTEGER NOT NULL, "
                 "date STRING(25) NOT NULL, "
                 "time STRING(25), "
                 "protection STRING(255), "
                 "id_journal INTEGER NOT NULL, " // идентификатор для привязки записей из таблицы свойств
                 "sn_device INTEGER NOT NULL, "
                 "CONSTRAINT new_pk PRIMARY KEY (id_msg, date, time, sn_device));";

        if(!query.exec(db_str))
        {
            QMessageBox::warning(this, tr("Создание таблицы"), tr("Невозможно создать таблицу журналов аварий: ") +
                                       query.lastError().text());
            return false;
        }

        // создание таблицы для хранения свойств журналов аварий
        db_str = "CREATE TABLE property ("
                 "name STRING(255) NOT NULL, "
                 "value STRING(255) NOT NULL, "
                 "id_journal INTEGER NOT NULL, "
                 "CONSTRAINT new_pk PRIMARY KEY (name, value, id_journal));";

        if(!query.exec(db_str))
        {
            QMessageBox::warning(this, tr("Создание таблицы"), tr("Невозможно создать таблицу свойств журналов аварий: ") +
                                       query.lastError().text());
            return false;
        }
    }
    else if(journal_type == "HALFHOUR")
    {
        // создание таблицы для хранения журналов аварий
        db_str = "CREATE TABLE journals ("
                 "id_msg INTEGER NOT NULL, "
                 "date STRING NOT NULL, "
                 "time STRING, "
                 "type STRING, "
                 "time_reset STRING, "
                 "id_journal INTEGER NOT NULL, " // идентификатор для привязки записей из таблицы свойств
                 "sn_device INTEGER NOT NULL, "
                 "CONSTRAINT new_pk PRIMARY KEY (id_msg, date, time, sn_device));";

        if(!query.exec(db_str))
        {
            QMessageBox::warning(this, tr("Создание таблицы"), tr("Невозможно создать таблицу журналов получасовок: ") +
                                       query.lastError().text());
            return false;
        }

        // создание таблицы для хранения свойств журналов аварий
        db_str = "CREATE TABLE property ("
                 "value DOUBLE, "
                 "id_journal INTEGER);"
                 /*"CONSTRAINT new_pk PRIMARY KEY (time, value, id_journal));"*/;

        if(!query.exec(db_str))
        {
            QMessageBox::warning(this, tr("Создание таблицы"), tr("Невозможно создать таблицу свойств журналов получасовок: ") +
                                       query.lastError().text());
            return false;
        }
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
    DeviceMenuItemType  index  = menuIndex();
    bool                result = false;

    if(index == DEVICE_MENU_ITEM_JOURNALS_CRASHES || index == DEVICE_MENU_ITEM_JOURNALS_EVENTS ||
       index == DEVICE_MENU_ITEM_JOURNALS_HALF_HOURS || index == DEVICE_MENU_ITEM_JOURNALS_ISOLATION)
    {
        QWidget* curWgt = ui->stwgtMain->currentWidget();

        for(const QObject* obj: curWgt->children())
        {
            if(obj->isWidgetType() && QString(obj->metaObject()->className()) == "CJournalWidget")
            {
                widget = qobject_cast<const CJournalWidget*>(obj);
                result = true;
            }
        }
    }

    return result;
}
//-------------------------------------
void ConfiguratorWindow::loadSettings()
{
    qInfo() << tr("Загрузка настроек программы...");

    int baudrate = -1;

    if(m_settings)
    {
        m_settings->beginGroup("serial-port");
            baudrate = m_settings->value("baudrate", QSerialPort::Baud115200).toInt();
            m_serialPortSettings_window->setBaudrate(QSerialPort::BaudRate(baudrate));
            m_serialPortSettings_window->setDataBits(QSerialPort::DataBits(m_settings->value("databits", QSerialPort::Data8).toInt()));
            m_serialPortSettings_window->setStopBits(QSerialPort::StopBits(m_settings->value("stopbits", QSerialPort::OneStop).toInt()));
            m_serialPortSettings_window->setParity(QSerialPort::Parity(m_settings->value("parity", QSerialPort::EvenParity).toInt()));
        m_settings->endGroup();

        m_settings->beginGroup("modbus");
            m_serialPortSettings_window->setModbusTimeout(m_settings->value("timeout", 500).toInt());
            m_serialPortSettings_window->setModbusTryCount(m_settings->value("trycount", 3).toInt());
            m_serialPortSettings_window->setModbusIntervalSilence(m_settings->value("interval_silence", 4).toInt());
        m_settings->endGroup();

        m_settings->beginGroup("device");
            ui->sboxTimeoutCalc->setValue(m_settings->value("timeoutcalculate", 1000).toInt());
            ui->checkboxCalibTimeout->setChecked(m_settings->value("timeoutcalculateenable", true).toBool());
            m_serialPortSettings_window->setDeviceSync(m_settings->value("synctime", 1000).toInt());
            m_serialPortSettings_window->setAutospeed(m_settings->value("autospeed", false).toBool());
        m_settings->endGroup();

        m_settings->beginGroup("mainwindow");
            restoreGeometry(m_settings->value("geometry").toByteArray());
            ui->splitterCentralWidget->restoreState(m_settings->value("panel").toByteArray());
            ui->splitterPanelMessage->restoreState(m_settings->value("downpanel").toByteArray());
        m_settings->endGroup();

        m_settings->beginGroup("settings");
            ui->checkBoxPanelMessage->setChecked(m_settings->value("downpanel_state").toBool());
            ui->menuDeviceDockPanel->setProperty("WIDTH", m_settings->value("panel_menu_width", 100).toInt());
            ui->variableDockPanel->setProperty("WIDTH", m_settings->value("panel_variable_width", 100).toInt());
            ui->framePanelMessage->setProperty("HEIGHT", m_settings->value("panel_message_height", 100).toInt());
        m_settings->endGroup();
    }

    if(baudrate != -1)
    {
        int index = ui->comboBoxCommunicationBaudrate->findText(QString::number(baudrate));

        if(index != -1)
            ui->comboBoxCommunicationBaudrate->setCurrentIndex(index);
    }

    m_modbus->channel()->setAutochoicespeed(m_serialPortSettings_window->autospeedState());
}
//-------------------------------------
void ConfiguratorWindow::saveSettings()
{
    qInfo() << tr("Сохранение настроек программы...");

    if(m_settings)
    {
        m_settings->beginGroup("serial-port");
            m_settings->setValue("baudrate", m_serialPortSettings_window->baudrate());
            m_settings->setValue("databits", m_serialPortSettings_window->dataBits());
            m_settings->setValue("stopbits", m_serialPortSettings_window->stopBits());
            m_settings->setValue("parity", m_serialPortSettings_window->parity());
        m_settings->endGroup();

        m_settings->beginGroup("modbus");
            m_settings->setValue("timeout", m_serialPortSettings_window->modbusTimeout());
            m_settings->setValue("trycount", m_serialPortSettings_window->modbusTryCount());
            m_settings->setValue("interval_silence", m_serialPortSettings_window->modbusIntervalSilence());
        m_settings->endGroup();

        m_settings->beginGroup("device");
            m_settings->setValue("timeoutcalculate", ui->sboxTimeoutCalc->value());
            m_settings->setValue("timeoutcalculateenable", ui->checkboxCalibTimeout->isChecked());
            m_settings->setValue("synctime", m_serialPortSettings_window->deviceSync());
            m_settings->setValue("autospeed", m_serialPortSettings_window->autospeedState());
        m_settings->endGroup();

        m_settings->beginGroup("mainwindow");
            m_settings->setValue("geometry", saveGeometry());
            m_settings->setValue("panel", ui->splitterCentralWidget->saveState());
            m_settings->setValue("downpanel", ui->splitterPanelMessage->saveState());
        m_settings->endGroup();

        m_settings->beginGroup("settings");
            m_settings->setValue("downpanel_state", ui->checkBoxPanelMessage->isChecked());
            m_settings->setValue("panel_menu_width", ui->menuDeviceDockPanel->property("WIDTH").toInt());
            m_settings->setValue("panel_variable_width", ui->variableDockPanel->property("WIDTH").toInt());
            m_settings->setValue("panel_message_height", ui->framePanelMessage->property("HEIGHT").toInt());
        m_settings->endGroup();
    }
}
//-------------------------------------------------------------------------------------------
void ConfiguratorWindow::exportToPDF(const CJournalWidget* widget, const QString& reportName,
                                     const QString& sn_device, const QString& filename)
{
    QPrinter* printer = new QPrinter(QPrinter::ScreenResolution);

    printer->setColorMode(QPrinter::GrayScale);
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
    int sub_rows = 0; // количество подстрок (свойства строки журнала)

    row_property_t list;

    if(key == "CRASH")
    {
        for(int i = pos.x(); i <= pos.y(); i++)
        {
            property_list_t property_list = qvariant_cast<property_list_t>(widget->table()->rowData(i));
            property_list_t sub_list;

            int index_calc_value = -1;

            for(int j = 1; j < property_list.count(); j++)
            {
                if(property_list[j].name == ';')
                {
                    index_calc_value = j + 1;
                    break;
                }

                sub_list << property_list[j];
            }

            if(index_calc_value != -1 && index_calc_value < property_list.count())
            {
                for(int k = index_calc_value; k < property_list.count(); k++)
                {
                    if(property_list[k].name == ';')
                    {
                        break;
                    }

                    sub_list << property_list[k];
                }
            }

            list << sub_list;
            sub_rows += sub_list.count();
        }
    }
    else if(key == "HALFHOUR")
    {
        halfhour_labels_t column_labels = qvariant_cast<halfhour_labels_t>(widget->property("COLUMNS"));
        halfhour_labels_t row_labels    = qvariant_cast<halfhour_labels_t>(widget->property("ROWS"));

        if(column_labels.isEmpty() || row_labels.isEmpty())
            return;

        for(int i = pos.x(); i <= pos.y(); i++)
        {
            halfhour_t halfhour = qvariant_cast<halfhour_t>(widget->table()->rowData(i));

            if(halfhour.values.isEmpty())
                list << property_list_t(QVector<property_data_item_t>(0));
            else
            {
                if((column_labels.count()*row_labels.count()) != halfhour.values.count())
                    continue;

                property_list_t list_property;

                for(int j = 0; j < column_labels.count(); j++)
                {
                    for(int k = 0; k < row_labels.count(); k++)
                    {
                        property_data_item_t item = property_data_item_t(
                        {
                            QString("%1 %2").arg(column_labels[j].name).arg(row_labels[k].name),
                            QLocale::system().toString(halfhour.values[j*4 + k], 'f', 1)
                        });

                        list_property << item;
                    }
                }

                list << list_property;
                sub_rows += column_labels.count()*row_labels.count();
            }
        }
    }

    QTextTable* textTable = cursor.insertTable(rows + sub_rows + 1, columnCount, tableFormat);

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

    int row_cur = 0;

    for(int i = 0; i < rows; i++)
    {
        qDebug() << tr("Начало экспорта в пдф, строка = %1").arg(i);
        for(int j = 0; j < columnCount; j++)
        {
            QTextTableCell cell = textTable->cellAt(i + row_cur + 1, j);
            Q_ASSERT(cell.isValid());
            QTextCursor cellCursor = cell.firstCursorPosition();
            qDebug() << tr("вставка текста, строка: %1, колонка: %2.").arg(pos.x() + i).arg(j);
            QTableWidgetItem* item = widget->table()->item(pos.x() + i, j);

            if(item)
                cellCursor.insertText(widget->table()->item(pos.x() + i, j)->text());
        }

        if(!list.isEmpty())
        {
            property_list_t property_list = list[i];

            if(!property_list.isEmpty())
            {
                for(int k = 0; k < property_list.count(); k++)
                {
                    QTextTableCell cell_name = textTable->cellAt(i + row_cur + 2 + k, columnCount - 2);
                    Q_ASSERT(cell_name.isValid());
                    QTextCursor cellCursorName = cell_name.firstCursorPosition();
                    cellCursorName.insertText(property_list[k].name);

                    QTextTableCell cell_value = textTable->cellAt(i + row_cur + 2 + k, columnCount - 1);
                    Q_ASSERT(cell_value.isValid());
                    QTextCursor cellCursorValue = cell_value.firstCursorPosition();
                    cellCursorValue.insertText(property_list[k].value);
                }

                row_cur += property_list.count();
                qDebug() << tr("Экспорт в пдф, текущая строка = %1 из %2").arg(row_cur).arg(rows);
            }
        }
        qDebug() << tr("Конец экспорта в пдф, строка = %1").arg(i);
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

        painter.drawImage(textRect, QImage(":/images/resource/images/background_report.jpg"));

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
            painter.setRenderHint(QPainter::TextAntialiasing,true);
            painter.setRenderHint(QPainter::HighQualityAntialiasing,false);
            const QRectF textPageRect(0, pageIndex*reportPDF->pageSize().height(), reportPDF->pageSize().width(),
                                                                                   reportPDF->pageSize().height());
            painter.setClipRect(textRect);
            painter.translate(0, -textPageRect.top());
            painter.translate(textRect.left(), textRect.top());
            reportPDF->drawContents(&painter, textPageRect);
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
    CMatrix    matrix;
    QString    fileNameDefault;
    QString    typeName;

    DeviceMenuItemType index = menuIndex();

    if(index == DEVICE_MENU_ITEM_SETTINGS_ITEM_LEDS)
    {
        matrix          = static_cast<CMatrixPurposeModel*>(ui->tablewgtLedPurpose->model())->matrix();
        typeName        = "LED";
        fileNameDefault = "led";
    }
    else if(index == DEVICE_MENU_ITEM_SETTINGS_ITEM_IO_MDVV01_INPUTS)
    {
        matrix          = static_cast<CMatrixPurposeModel*>(ui->tablewgtDiscreteInputPurpose->model())->matrix();
        typeName        = "INPUT";
        fileNameDefault = "input";
    }
    else if(index == DEVICE_MENU_ITEM_SETTINGS_ITEM_IO_MDVV01_RELAY)
    {
        matrix          = static_cast<CMatrixPurposeModel*>(ui->tablewgtRelayPurpose->model())->matrix();
        typeName        = "RELAY";
        fileNameDefault = "relay";
    }

    if(matrix.rowCount() == 0 || matrix.columnCount() == 0)
        return;

    QDir dir;

    if(!dir.exists("outputs/profiles"))
        dir.mkdir("outputs/profiles");

    QString fileName = QFileDialog::getSaveFileName(this, tr("Открытие профиля привязок"),
                                                    QString(dir.absolutePath() + "/%1/%2").
                                                    arg("outputs/profiles").arg(fileNameDefault),
                                                    tr("Профили привязок (*.prf)"));

    if(fileName.isEmpty())
        return;

    m_progressbar->setProgressTitle(tr("Экспорт профиля привязок %1").arg(((typeName == "LED")?tr("Светодиоды"):
                                                                           (typeName == "INPUT")?tr("Входы"):tr("Реле"))));
    m_progressbar->setSettings(0, matrix.rowCount(), tr("строк"));
    m_progressbar->progressStart();

    // Начало формирования файла JSON
    QJsonObject objJson;
    QJsonArray  arrRows;
    for(int irow = 0; irow < matrix.rowCount(); irow++)
    {
        CRow row = matrix[irow];

        QJsonObject objRow
        {
            { "key", row.data().key },
            { "name", row.data().name },
            { "description", row.data().description },
            { "position", row.data().position },
            { "state", ((row.data().state == UNCHECKED)?UNCHECKED:(row.data().state == CHECKED)?CHECKED:INVERSE) }
        };

        QJsonArray arrColumns;

        for(const CColumn& column: row.columns())
        {
            QJsonObject objColumn
            {
                { "key", column.data().key },
                { "name", column.data().name },
                { "description", column.data().description },
                { "position", column.data().position },
                { "state", ((column.data().state == UNCHECKED)?UNCHECKED:(column.data().state == CHECKED)?CHECKED:INVERSE) }
            };

            arrColumns += objColumn;
        }

        objRow["columns"] = arrColumns;
        arrRows += objRow;

        emit m_progressbar->increment();
    }

    objJson["type"] = typeName;
    objJson["rows"] = arrRows;

    QJsonDocument docJson(objJson);
    // Конец формирования файла JSON

    QFile file(fileName);

    if(!file.open(QFile::WriteOnly))
    {
        QMessageBox::warning(this, tr("Сохранение профиля привязок"), tr("Невозможно создать файл для записи \"%1\"").
                                                                      arg(fileName));

        return;
    }

    file.write(docJson.toJson());
    file.close();
    m_progressbar->progressStop();
}
//----------------------------------------------
void ConfiguratorWindow::importPurposeFromJSON()
{
    QString fileNameDefault;
    QString typeName;
    QString typeNameDescription;

    CMatrixPurposeModel* model = nullptr;
    DeviceMenuItemType   index = menuIndex();

    if(index == DEVICE_MENU_ITEM_SETTINGS_ITEM_LEDS)
    {
        fileNameDefault     = "led";
        typeName            = "LED";
        typeNameDescription = tr("Светодиоды");

        model = static_cast<CMatrixPurposeModel*>(ui->tablewgtLedPurpose->model());
        emit ui->pbtnClearLedOutput->clicked();
    }
    else if(index == DEVICE_MENU_ITEM_SETTINGS_ITEM_IO_MDVV01_INPUTS)
    {
        fileNameDefault     = "input";
        typeName            = "INPUT";
        typeNameDescription = tr("Дискретные входы");

        model = static_cast<CMatrixPurposeModel*>(ui->tablewgtDiscreteInputPurpose->model());
        emit ui->pbtnClearDiscreteInput->clicked();
    }
    else if(index == DEVICE_MENU_ITEM_SETTINGS_ITEM_IO_MDVV01_RELAY)
    {
        fileNameDefault     = "relay";
        typeName            = "RELAY";
        typeNameDescription = tr("Реле");

        model = static_cast<CMatrixPurposeModel*>(ui->tablewgtRelayPurpose->model());
        emit ui->pbtnClearRelayOutput->clicked();
    }

    CMatrix& matrix = model->matrix();

    if(matrix.rowCount() == 0 || matrix.columnCount() == 0)
        return;

    QDir dir;

    QString fileName = QFileDialog::getOpenFileName(this, tr("Открытие профиля привязок"),
                                                    QString(dir.absolutePath() + "/%1/%2").arg("outputs/profiles").
                                                    arg(fileNameDefault),
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

    if(!json.isObject())
    {
        QMessageBox::warning(this, tr("Импорт профиля привязок"), tr("Неизвестный формат файла привязок"));
        return;
    }

    QString t = json["type"].toString();

    if(t.toUpper() != typeName.toUpper())
    {
        QMessageBox::warning(this, tr("Импорт профиля привязок"), tr("Ошибка типа файла привязок.\nВыберите другой файл."));
        return;
    }

    QJsonArray arrRows = json["rows"].toArray();

    if(matrix.rowCount() != arrRows.count())
    {
        QMessageBox::warning(this, tr("Импорт профиля привязок"), tr("Количество привязок в файле не соответствует\n"
                                                                     "количеству привязок в таблице."));
        return;
    }

    m_progressbar->setProgressTitle(tr("Импорт профиля привязок %1").arg(((typeName == "LED")?tr("Светодиоды"):
                                                                          (typeName == "INPUT")?tr("Входы"):tr("Реле"))));
    m_progressbar->setSettings(0, matrix.rowCount(), tr("строк"));
    m_progressbar->progressStart();

    for(int i = 0; i < matrix.rowCount(); i++)
    {
        QJsonObject objRow = arrRows[i].toObject();
        CRow& row = matrix[i];

        row.data().state = static_cast<StateType>(objRow["state"].toInt());

        QJsonArray arrColumns = objRow["columns"].toArray();

        if(arrColumns.count() != matrix.columnCount())
        {
            qDebug() << tr("Количество колонок в строке %1 не соответствует количеству колонок в файле.").arg(i);
            continue;
        }

        for(int j  = 0; j < matrix.columnCount(); j++)
        {
            QJsonObject objColumn = arrColumns[j].toObject();
            CColumn& column = row[j];
            column.data().state = static_cast<StateType>(objColumn["state"].toInt());
        }

        m_progressbar->increment();
    }

    model->updateData();
    m_progressbar->progressStop();
}
//----------------------------------------------------------------
void ConfiguratorWindow::processReadJournal(CModBusDataUnit& unit)
{
    RequestType type = (RequestType)unit.property(tr("REQUEST")).toInt();

    QString key = unit.property(tr("JOURNAL")).toString();

    if(key.isEmpty() || m_journal_set.find(key) == m_journal_set.end())
        return;

    journal_set_t& set = m_journal_set[key];
    static int time_sum = 0; // текущее время выполнения чтения (для расчета скорости)

    switch(type)
    {
        case READ_JOURNAL_SHIFT_PTR:
            if(unit.error() != CModBusDataUnit::ERROR_NO)
            {
                QMessageBox::warning(this, tr("Чтение журнала"), tr("Ошибка перемещения указателя журнала:\n%1").
                                     arg(CModBusDataUnit::errorDescription(unit.error())));
            }
        break;

        case READ_JOURNAL_COUNT:
            if(unit.count() == 2)
            {
                long count = long(long(unit[0] << 16) | long(unit[1]));

                set.message.read_total = count;

                CHeaderJournal* header = nullptr;

                if(key == "CRASH")
                    header = ui->widgetJournalCrash->header();
                else if(key == "EVENT")
                    header = ui->widgetJournalEvent->header();
                else if(key == "HALFHOUR")
                    header = ui->widgetJournalHalfHour->header();

                if(header)
                    header->setTextDeviceCountMessages(0, count);
            }
        break;

        case READ_JOURNAL:
        {
            if(!m_journal_read_current)
                return;

            QVector<quint16> data = unit.values();

            if(set.message.read_count == 0) // обнуление счетчика времени передачи данных (если пришло первое сообщение)
                time_sum = 0;

            if(unit.count()*2 < set.message.size) // принятые данные меньше, чем размер одного сообщения
            {
                if(set.buffer.isEmpty()) // буфер сообщений пуст
                {
                    set.buffer.append(unit.values()); // сохраняем сообщения в буфер
                    m_journal_timer->stop();
                    journalRead(key);

                    return;
                }
                else
                {
                    if((unit.count()*2 + set.buffer.count()*2) == set.message.size) // данные приняты полностью
                    {
                        data = set.buffer;
                        data.append(unit.values());

                        set.buffer.clear(); // чистим буфер
                        set.msg_part = 0; // обнуляем счетчик частей
                    }
                }
            }

            long count = data.count()/(set.message.size/2);

            set.message.read_count   += count;
            set.message.read_current += count;

            if(!key.isEmpty())
                displayJournalResponse(data);

            m_progressbar->progressIncrement(count);
            m_journal_timer->stop();

            time_sum += m_time_process_speed.elapsed();

            if(time_sum > 0)
            {
                float speed_kb = float(set.message.size*set.message.read_count/1024.0f)/float(time_sum/1000.0f);

                m_journal_read_current->header()->setTextElapsedTime(QString("%1 КБ/сек.").arg(QLocale::system().toString(speed_kb, 'f', 3)));
            }

            // Проверка условий на окончание чтения журнала
            if(set.isStart && set.message.read_count >= set.message.read_limit)
            {
                QString text;
                QString journal_name = m_journal_read_current->property("NAME").toString();

                if(!set.isStop)
                {
                    text = tr("Чтение журнала %1 успешно завершено.\nПрочитано %2 из %3 сообщений.").
                           arg(journal_name).arg(set.message.read_count).arg(set.message.read_total);
                }
                else
                {
                    text = tr("Чтение журнала %1 прервано пользователем.\nПрочитано %2 из %3 сообщений.").
                           arg(journal_name).arg(set.message.read_count).arg(set.message.read_total);
                }


                set.isStart = set.isStop = false;
                endJournalReadProcess(text);

                return;
            }

            journalRead(key);
        }
        break;

        default: break;
    }
}
//---------------------------------------------------
void ConfiguratorWindow::widgetStackIndexChanged(int)
{
    ui->tabwgtMenu->setTabEnabled(TAB_READ_WRITE_INDEX, false);
    ui->tabwgtMenu->setTabEnabled(TAB_FILTER_INDEX, false);

    m_active_journal_current = nullptr;

    ui->pushButtonJournalRead->setVisible(false);
    ui->pushButtonJournalClear->setVisible(false);
    ui->pushButtonDefaultSettings->setVisible(false);

    if(ui->tabwgtMenu->currentIndex() == TAB_HELP_INDEX)
    {
        ui->tabwgtMenu->setCurrentIndex(TAB_SET_INDEX);
    }

    DeviceMenuItemType index = menuIndex();

    if(index >= DEVICE_MENU_ITEM_PROTECTION_ROOT && index <= DEVICE_MENU_ITEM_AUTOMATION_APV_SIGNAL_START)
    {
        ui->tabwgtMenu->setTabEnabled(TAB_READ_WRITE_INDEX, true);
        ui->tabwgtMenu->setCurrentIndex(TAB_READ_WRITE_INDEX);
    }
    else if(index >= DEVICE_MENU_ITEM_JOURNALS_CRASHES && index <= DEVICE_MENU_ITEM_JOURNALS_ISOLATION)
    {
        ui->tabwgtMenu->setTabEnabled(TAB_FILTER_INDEX, true);

        int width = ui->stwgtMain->width() - 760;

        ui->widgetJournalCrash->table()->setFixedWidth(475);
        ui->widgetJournalEvent->setTableColumnWidth(3, width);
        ui->widgetJournalHalfHour->setTableColumnWidth(3, width);
        ui->widgetJournalIsolation->setTableColumnWidth(3, width);

        if(!currentJournal(m_active_journal_current))
            m_active_journal_current = nullptr;

        ui->pushButtonJournalRead->setVisible(true);
        ui->pushButtonJournalClear->setVisible(true);

        ui->tabwgtMenu->setTabEnabled(TAB_READ_WRITE_INDEX, true);
        ui->tabwgtMenu->setCurrentIndex(TAB_READ_WRITE_INDEX);

        readJournalCount();
    }
    else if(index == DEVICE_MENU_ITEM_SETTINGS_ITEM_IN_ANALOG ||
            index == DEVICE_MENU_ITEM_SETTINGS_ITEM_DATETIME ||
            index == DEVICE_MENU_ITEM_SETTINGS_ITEM_COMMUNICATIONS ||
            index == DEVICE_MENU_ITEM_SETTINGS_ITEM_IO_PROTECTION)
    {
        ui->tabwgtMenu->setTabEnabled(TAB_READ_WRITE_INDEX, true);
        ui->tabwgtMenu->setCurrentIndex(TAB_READ_WRITE_INDEX);
    }
    else if(index == DEVICE_MENU_ITEM_SETTINGS_ITEM_LEDS ||
            index == DEVICE_MENU_ITEM_SETTINGS_ITEM_IO_MDVV01_INPUTS ||
            index == DEVICE_MENU_ITEM_SETTINGS_ITEM_IO_MDVV01_RELAY ||
            index == DEVICE_MENU_ITEM_SETTINGS_ITEM_KEYBOARD)
    {
        if(index == DEVICE_MENU_ITEM_SETTINGS_ITEM_IO_MDVV01_INPUTS)
        {
            ui->widgetToolTipState->show();
        }
        else
            ui->widgetToolTipState->hide();

        ui->tabwgtMenu->setTabEnabled(TAB_READ_WRITE_INDEX, true);
        ui->tabwgtMenu->setCurrentIndex(TAB_READ_WRITE_INDEX);
        ui->pushButtonDefaultSettings->setVisible(true);

        resizeColumns();
    }
}
//-----------------------------------------------------------------------
void ConfiguratorWindow::setJournalPtrShift(const QString& key, long pos)
{
    if(key.isEmpty() || m_journal_set.find(key) == m_journal_set.end())
        return;

    journal_set_t set = m_journal_set[key];

    QVector<quint16> values = QVector<quint16>() << (quint16)((pos >> 16)&0xFFFF) << (quint16)(pos&0xFFFF);

    CModBusDataUnit unit(m_serialPortSettings_window->deviceID(), CModBusDataUnit::WriteMultipleRegisters, set.address.set_shift, values);
    unit.setProperty(tr("REQUEST"), READ_JOURNAL_SHIFT_PTR);
    unit.setProperty(tr("JOURNAL"), key);

    m_modbus->sendData(unit);
}
//------------------------------------------------
void ConfiguratorWindow::timeoutSynchronization()
{
    CModBusDataUnit unit(m_serialPortSettings_window->deviceID(), CModBusDataUnit::ReadInputRegisters, 0x0001, QVector<quint16>() << 4);

    unit.setProperty("REQUEST", READ_SERIAL_NUMBER);

    m_modbus->sendData(unit);

    DeviceMenuItemType item = menuIndex();

    if(item != DEVICE_MENU_ITEM_NONE)
    {
        if(item >= DEVICE_MENU_ITEM_JOURNALS_CRASHES && item <=
                   DEVICE_MENU_ITEM_JOURNALS_ISOLATION &&
                   !ui->pushButtonJournalRead->isChecked())
        {
            readJournalCount(); // читаем количество сообщений в каждом журнале
        }
    }

    m_timer_synchronization->start(m_serialPortSettings_window->deviceSync());
}
//-----------------------------------------
void ConfiguratorWindow::timeoutDebugInfo()
{
    debugInfoRead();
}
//-----------------------------------------------------------
void ConfiguratorWindow::debugInfoCtrl(int timer, bool state)
{
    if(state)
    {
        if(m_tim_debug_info->isActive())
            m_tim_debug_info->setInterval(timer);
        else
            m_tim_debug_info->start(timer);
    }
    else
    {
        m_tim_debug_info->stop();
    }
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
    CJournalTable*  table  = m_active_journal_current->table();
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
                                                             dir.absolutePath() + QString("/outputs/db/%1.%2").
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

    m_progressbar->setProgressTitle(QString("Импорт журнала %1").arg(journal_name));
    m_progressbar->progressStart();
    m_progressbar->setSettings(0, max_read_msg, "");

    int msg_count = 0;

    table->setSortingEnabled(false);

    while(query.next()) // заносим данные в таблицу журналов
    {
        int row = table->rowCount();

        table->insertRow(row);

        QString id_msg = query.value("id_msg").toString();
        QString date   = QDate::fromString(query.value("date").toString(), Qt::ISODate).toString("dd.MM.yyyy");
        QString time   = query.value("time").toString();

        table->setItem(row, 0, new QTableWidgetItem(id_msg));
        table->setItem(row, 1, new CTableWidgetItem(date));
        table->setItem(row, 2, new QTableWidgetItem(time));

        table->item(row, 0)->setTextAlignment(Qt::AlignCenter);
        table->item(row, 1)->setTextAlignment(Qt::AlignCenter);
        table->item(row, 2)->setTextAlignment(Qt::AlignCenter);

        if(journal_type == "EVENT")
        {
            QString type      = query.value("type").toString();
            QString category  = query.value("category").toString();
            QString parameter = query.value("parameter").toString();

            table->setItem(row, 3, new QTableWidgetItem(type));
            table->setItem(row, 4, new QTableWidgetItem(category));
            table->setItem(row, 5, new QTableWidgetItem(parameter));
        }
        else if(journal_type == "CRASH")
        {
            QString protection = query.value("protection").toString();
            int     id_journal = query.value("id_journal").toInt();

            table->setItem(row, 3, new QTableWidgetItem(protection));

            // формирование запроса для получения свойств записи
            QSqlQuery query_property(*db);

            if(!query_property.exec(QString("SELECT name, value FROM property WHERE id_journal=%1;").arg(id_journal)))
            {
                QMessageBox::warning(this, tr("Импорт журнала"), tr("Не удалось прочитать свойства журнала аварий: %1").
                                                                 arg(query_property.lastError().text()));
                continue;
            }

            property_list_t property_list;

            while(query_property.next())
            {
                property_list << property_data_item_t({ query_property.value("name").toString(),
                                                        query_property.value("value").toString() });
            }

            if(!property_list.isEmpty())
                table->setRowData(row, QVariant::fromValue(property_list));
        }
        else if(journal_type == "HALFHOUR")
        {
            QString type       = query.value("type").toString();
            int     id_journal = query.value("id_journal").toInt();
            QString time_reset = query.value("time_reset").toString();

            table->setItem(row, 3, new QTableWidgetItem(type));
            table->setItem(row, 4, new QTableWidgetItem(time_reset));

            // формирование запроса для получения свойств записи
            QSqlQuery query_property(*db);

            if(!query_property.exec(QString("SELECT value FROM property WHERE id_journal=%1;").arg(id_journal)))
            {
                qWarning() <<  tr("Не удалось прочитать свойства журнала получасовок: %1").arg(query_property.lastError().text());
                continue;
            }

            halfhour_t halfhour;

            while(query_property.next())
            {
                halfhour.values << query_property.value("value").toFloat();
            }

            if(!halfhour.values.isEmpty() && (!type.isEmpty() && type.toUpper() == tr("ДАННЫЕ")))
                table->setRowData(row, QVariant::fromValue(halfhour));
        }

        m_progressbar->progressIncrement();

        msg_count++;
    }

    table->sortByColumn(1, Qt::AscendingOrder);
    table->setSortingEnabled(true);
    table->setSortingEnabled(false);

    if(m_filter.find(journal_type) != m_filter.end())
    {
        CFilter::FilterDateType d = { QDate::fromString(table->item(0, 1)->text(), "dd.MM.yyyy"),
                                      QDate::fromString(table->item(table->rowCount() - 1, 1)->text(), "dd.MM.yyyy")};
        m_filter[journal_type].setDate(d);
    }


    header->setTextTableCountMessages(msg_count);
    header->setTextElapsedTime(timer.elapsed());

    QTableWidgetItem* itemBeg = table->item(0, 1);
    QTableWidgetItem* itemEnd = table->item(table->rowCount() - 1, 1);

    if(itemBeg && itemEnd)
    {
        QString text = QString("%1 - %2/%3").arg(itemBeg->text()).arg(itemEnd->text()).arg(table->rowCount());
        m_active_journal_current->header()->setTextTableCountMessages(text);
    }

    m_progressbar->progressStop();

    table->resizeColumnsToContents();
    table->horizontalHeader()->setStretchLastSection(true);

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
    CJournalTable*  table  = m_active_journal_current->table();
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

    if(!dir.exists("outputs/db"))
        dir.mkdir("outputs/db");

    QString selectedFilter    = tr("Базы данных (*.db)");
    QString journal_full_name = tr("Журнал %1-%2").arg(journal_name).arg(m_status_bar->serialNumberText());
    QString journal_path      = QFileDialog::getSaveFileName(this, tr("Экспорт журнала %1 в базу данных").arg(journal_name),
                                                             dir.absolutePath() + QString("/outputs/db/%1.%2").
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

    int id_journal = 0; // id записи в журнале - используется для журнала аварий - привязка свойств к записи

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

        QString res = recordLastDb(db, "property", "id_journal"); // читаем id журнала для свойств

        if(!res.isEmpty())
            id_journal = res.toInt() + 1; // присваиваем начальный номер для свойств
    }
    else // создаем новую базу данных
    {
        if(!createJournalTable(db, journal_type)) // создаем таблицы
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

    for(i = pos.x(); i <= pos.y(); i++)
    {
        int     id_msg = table->item(i, 0)->text().toInt();
        QString date   = QDate::fromString(table->item(i, 1)->text(),
                                           "dd.MM.yyyy").toString(Qt::ISODate); // приведение строки к yyyy-MM-dd для sqlite
        QString time   = table->item(i, 2)->text();

        if(journal_type == "EVENT")
        {
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

            query.exec();
        }
        else if(journal_type == "CRASH")
        {
            QString protect_name = table->item(i, 3)->text();

            query.prepare("INSERT OR REPLACE INTO journals (id_msg, date, time, protection, id_journal, sn_device)"
                          "VALUES(:id_msg, :date, :time, :protection, :id_journal, :sn_device)");
            query.bindValue(":id_msg", id_msg);
            query.bindValue(":date", date);
            query.bindValue(":time", time);
            query.bindValue(":protection", protect_name);
            query.bindValue(":id_journal", id_journal);
            query.bindValue(":sn_device", id);

            query.exec();

            property_list_t property_list = qvariant_cast<property_list_t>(table->rowData(i));

            if(!property_list.isEmpty())
            {
                for(const property_data_item_t& item: property_list)
                {
                    QSqlQuery query_property(*db);

                    query_property.prepare("INSERT OR REPLACE INTO property (name, value, id_journal)"
                                           "VALUES(:name, :value, :id_journal)");
                    query_property.bindValue(":name", item.name);
                    query_property.bindValue(":value", item.value);
                    query_property.bindValue(":id_journal", id_journal);

                    query_property.exec();
                }

                id_journal++;
            }
        }
        else if(journal_type == "HALFHOUR")
        {
            QTableWidgetItem* item = table->item(i, 3);

            if(!item)
                continue;

            QString type = item->text();

            query.prepare("INSERT OR REPLACE INTO journals (id_msg, date, time, type, time_reset, id_journal, sn_device)"
                          "VALUES(:id_msg, :date, :time, :type, :time_reset, :id_journal, :sn_device)");
            query.bindValue(":id_msg", id_msg);
            query.bindValue(":date", date);
            query.bindValue(":time", time);
            query.bindValue(":type", type);

            QTableWidgetItem* itemTimeReset = table->item(i, 4);

            query.bindValue(":time_reset", ((itemTimeReset)?itemTimeReset->text():""));
            query.bindValue(":id_journal", id_journal);
            query.bindValue(":sn_device", id);

            if(!query.exec())
                qWarning() << tr("Ошбика экспорта журнала получасовок в базу данных: %1").arg(query.lastError().text());

            halfhour_t halfhour = qvariant_cast<halfhour_t>(table->rowData(i));

            if(!halfhour.values.isEmpty() && type.toUpper() == tr("ДАННЫЕ"))
            {
                for(const float& value: halfhour.values)
                {
                    QSqlQuery query_property(*db);

                    query_property.prepare("INSERT INTO property (value, id_journal)"
                                           "VALUES(:value, :id_journal)");
                    query_property.bindValue(":value", value);
                    query_property.bindValue(":id_journal", id_journal);

                    if(!query_property.exec())
                        qWarning() << tr("journal halfhour property error: %1").arg(query_property.lastError().text());
                }

                id_journal++;
            }
        }

        m_progressbar->progressIncrement();
    }

    db->commit();
    disconnectDb(db);

    m_progressbar->progressStop();
}
//-----------------------------------------------------------------
int ConfiguratorWindow::addressSettingKey(const QString& key) const
{
    for(QPair<QString, cell_t> pair: m_cell_list)
    {
        if(pair.first == key)
        {
            return pair.second.addr;
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
    if(m_journal_set.isEmpty())
        return;

    for(const QString key: m_journal_set.keys())
    {
        journal_set_t set = m_journal_set[key];

        CModBusDataUnit unit(m_serialPortSettings_window->deviceID(), CModBusDataUnit::ReadHoldingRegisters, set.address.set_shift,
                                                     QVector<quint16>() << 2);

        unit.setProperty(tr("REQUEST"), READ_JOURNAL_SHIFT_PTR);
        unit.setProperty(tr("JOURNAL"), key);

        m_modbus->sendData(unit);
    }
}
//-----------------------------------------
void ConfiguratorWindow::readJournalCount()
{
    if(m_journal_set.isEmpty())
        return;

    for(const QString key: m_journal_set.keys())
    {
        journal_set_t set = m_journal_set[key];

        CModBusDataUnit unit(m_serialPortSettings_window->deviceID(), CModBusDataUnit::ReadInputRegisters, set.address.msg_count,
                                                     QVector<quint16>() << 2);
        unit.setProperty(tr("REQUEST"), READ_JOURNAL_COUNT);
        unit.setProperty(tr("JOURNAL"), key);

        m_modbus->sendData(unit);
    }
}
//--------------------------------------------------
void ConfiguratorWindow::synchronization(bool state)
{
    if(state)
    {
        timeoutSynchronization();
    }
    else
    {
        if(m_timer_synchronization->isActive())
            m_timer_synchronization->stop();
    }
}
/*!
 * \brief ConfiguratorWindow::setLineEditValidator
 * \param object Объект к которому применяется валидатор (class CLineEdit)
 */
void ConfiguratorWindow::setLineEditValidator(QObject* object)
{
    CLineEdit* lineEdit = qobject_cast<CLineEdit*>(object);

    QString str = lineEdit->objectName().toUpper();
    QString key = str.remove("LE");

    float limit_min = 0;
    float limit_max = 0;

    if(m_limits.find(key) != m_limits.end())
    {
        limit_min = m_limits[key].limit_min;
        limit_max = m_limits[key].limit_max;
    }

    QWidget* widget = findChild<QWidget*>(QString("label%1").arg(key));

    if(widget)
    {
        QString classWgt = widget->metaObject()->className();

        if(classWgt == tr("QLabel") && limit_max != 0)
        {
            QLabel* label = qobject_cast<QLabel*>(widget);

            if(label)
                label->setText(QString("%1 (%2...%3)").arg(label->text()).
                                                       arg(QLocale::system().toString(limit_min)).
                                                       arg(QLocale::system().toString(limit_max)));
        }
    }

    if(limit_min == -180 && limit_max == 180)
    {
        lineEdit->setValidator(new QIntValidator(limit_min, limit_max));
        lineEdit->setText("0");
    }
    else if(limit_min == 0 && limit_max == 0)
    {
        lineEdit->setValidator(new QDoubleValidator(0, 100, 6));
        lineEdit->setText(QString("%1").arg(QLocale::system().toString(0.0f, 'f', 6)));
    }
    else
    {
        lineEdit->setValidator(new QDoubleValidator(limit_min, limit_max, 6));
        lineEdit->setText(QString("%1").arg(QLocale::system().toString(0.0f, 'f', 6)));
    }
}
//--------------------------------------------------------
void ConfiguratorWindow::panelVisibleCtrl(QWidget* widget)
{
    if(widget == ui->variableDockPanel)
    {
        if(ui->variableDockPanel->width() == ui->pushButtonVariableCtrl->minimumWidth())
        {
            if(!ui->variableWidget->isHidden())
            {
                ui->variableWidget->hide();
                ui->labelVariablePanel->hide();
                ui->pushButtonVariableCtrl->setState(CDockPanelItemCtrl::Close);
            }
        }
        else
        {
            if(ui->variableWidget->isHidden())
            {
                ui->variableWidget->show();
                ui->labelVariablePanel->show();
                ui->pushButtonVariableCtrl->setState(CDockPanelItemCtrl::Open);
            }
        }
    }

    if(widget == ui->centralWgt)
    {
        if(ui->menuDeviceDockPanel->width() == ui->pushButtonMenuDeviceCtrl->minimumWidth())
        {
            if(!ui->treewgtDeviceMenu->isHidden())
            {
                ui->treewgtDeviceMenu->hide();
                ui->labelMenuDevice->hide();
                ui->tbntExpandItems->hide();
                ui->pushButtonMenuDeviceCtrl->setState(CDockPanelItemCtrl::Close);
            }
        }
        else
        {
            if(ui->treewgtDeviceMenu->isHidden())
            {
                ui->treewgtDeviceMenu->show();
                ui->labelMenuDevice->show();
                ui->tbntExpandItems->show();
                ui->pushButtonMenuDeviceCtrl->setState(CDockPanelItemCtrl::Open);
            }
        }
    }

    if(widget == ui->framePanelMessage || widget == ui->stwgtMain)
    {
        if(ui->framePanelMessage->height() == ui->pushButtonPanelMessage->minimumHeight())
        {
            if(!ui->tabWidgetMessage->isHidden())
            {
                ui->tabWidgetMessage->hide();
                ui->pushButtonPanelMessage->setState(CDockPanelItemCtrl::Close);
            }
        }
        else
        {
            if(ui->tabWidgetMessage->isHidden())
            {
                ui->tabWidgetMessage->show();
                ui->pushButtonPanelMessage->setState(CDockPanelItemCtrl::Open);
            }
        }
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
//-------------------------------------------------------------------------------------------------------------
QString ConfiguratorWindow::recordLastDb(QSqlDatabase* db, const QString& table_name, const QString& parameter)
{
    QSqlQuery query(*db);

    QString str = QString("SELECT %1 FROM %2 ORDER BY %3 DESC LIMIT 1;").arg(parameter).arg(table_name).arg(parameter);

    if(!query.exec(str))
    {
        return "";
    }

    if(query.first())
    {
        return query.value(0).toString();
    }

    return "";
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

                if(first == last)
                {
                    index.setY(i);
                    break;
                }

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
//--------------------------------------------------------------------
ConfiguratorWindow::DeviceMenuItemType ConfiguratorWindow::menuIndex()
{
    QTreeWidgetItem* item = ui->treewgtDeviceMenu->currentItem();

    if(!item)
    {
        return DEVICE_MENU_ITEM_NONE;
    }

    return DeviceMenuItemType(item->type());
}
/*!
 * \brief  ConfiguratorWindow::unpackDateTime
 * \param  data Данные в кототых хранятся упакованные дата и время сообщения
 * \return Возвращает либо пустую дату/время, либо дату/время сообщения
 *
 *  Метод применяется для распаковки даты/времени сообщений от журналов, которые хранятся в упакованном виде
 */
QDateTime ConfiguratorWindow::unpackDateTime(QVector<quint8>& data)
{
    if(data.isEmpty() || data.count() != 5) // упакованная дата размещается в 5ти байтах
        return QDateTime(); // возвращаем пусту дату, если нет данных или их размер меньше или больше 5ти байт

    quint16 year = ((data[0]&0xFC) >> 2);

    year += ((year < 2000)?2000:0); // приводит дату к 'yyyy'

    quint8 month = ((data[0]&0x03) << 2) | ((data[1]&0xC0) >> 6);
    quint8 day   = ((data[1]&0x3E) >> 1);

    quint8  hour    = ((data[1]&0x01) << 4) | ((data[2]&0xF0) >> 4);
    quint8  minute  = ((data[2]&0x0F) << 2) | ((data[3]&0xC0) >> 6);
    quint8  second  = (data[3]&0x3F);
    quint16 msecond = quint16(data[4]*3.90625f); // перевод 256 долей секунды в мс, т.е. 1000/256 = 3.90625

    QDateTime dt(QDate(year, month, day), QTime(hour, minute, second, msecond));

    return dt;
}
//--------------------------------------------------------------------------------------------------------
void ConfiguratorWindow::convertDataHalfwordToBytes(const QVector<quint16>& source, QVector<quint8>& dest)
{
    for(int i = 0; i < source.count(); i++)
    {
        dest.append(quint8(((source[i] >> 8)&0x00FF)));
        dest.append(quint8((source[i]&0x00FF)));
    }
}
//--------------------------------------------------------------------
group_t ConfiguratorWindow::createVariableGroup(const QString& io_key)
{
    QSqlQuery query;
    group_t   group;

    if(query.exec("SELECT * FROM var_group")) // читаем из базы список групп
    {
        while(query.next())
        {
            QSqlQuery query_item(m_system_db);

            int     id                = query.value("id").toInt();
            QString group_name        = query.value("name").toString();
            QString group_description = query.value("description").toString();

            QVector<var_t> var_list;

            if(query_item.exec(QString("SELECT * FROM variable WHERE group_id=%1;").arg(id)))
            {
                while(query_item.next())
                {
                    QString key         = query_item.value("key").toString();
                    int     group_id    = query_item.value("group_id").toInt();
                    int     bit         = query_item.value("bit").toInt();
                    QString name        = query_item.value("name").toString();
                    QString description = query_item.value("description").toString();

                    QSqlQuery query_purpose(m_system_db);

                    bool is_ok = false;

                    if(query_purpose.exec(QString("SELECT io_key FROM purpose WHERE var_key = \'%1\';").arg(key)))
                    {
                        while(query_purpose.next())
                        {
                            QString val = query_purpose.value("io_key").toString();

                            if(val.contains(io_key))
                            {
                                is_ok = true;
                                break;
                            }
                        }
                    }

                    if(!is_ok)
                        continue;

                    var_list << var_t({ key, group_id, bit, name, description });
                }
            }

            if(!var_list.isEmpty())
                group[id] = group_item_t({ group_name, group_description, var_list });
        }
    }

    return group;
}
//-----------------------------------------------------------------------------------------
QVector<QPair<QString, QString> > ConfiguratorWindow::loadLabelColumns(const QString& type)
{
    QVector<QPair<QString, QString> > labels;

    QSqlQuery query(QString("SELECT key, description FROM iodevice WHERE type = \'%1\';").arg(type));

    if(query.exec())
    {
        while(query.next())
        {
            labels << qMakePair(query.value("key").toString(), query.value("description").toString());
        }
    }

    return labels;
}
//-------------------------------------------------------------------------
QVector<QString> ConfiguratorWindow::loadVaribleByType(const QString& type)
{
    QSqlQuery query(m_system_db);

    QVector<QString> var_list;

    if(query.exec(QString("SELECT var_key FROM purpose WHERE io_key LIKE \'%1%\';").arg(type)))
    {
        while(query.next())
        {
            QString var_key = query.value("var_key").toString();

            if(!var_list.contains(var_key))
                var_list << var_key;
        }
    }

    return var_list;
}
//----------------------------------------------------------------------------------
ConfiguratorWindow::block_protection_list_t ConfiguratorWindow::loadProtectionList()
{
    block_protection_list_t block_list;

    QSqlQuery query(m_system_db);

    if(query.exec("SELECT * FROM block_protection;"))
    {
        while(query.next())
        {
            int     id          = query.value("id").toInt();
            QString key         = query.value("key").toString();
            int     address     = query.value("address").toInt();
            QString name        = query.value("name").toString();
            QString description = query.value("description").toString();

            QSqlQuery query_purpose(m_system_db);

            QVector<block_protection_purpose_t> purpose;

            if(query_purpose.exec(QString("SELECT * FROM block_protection_purpose WHERE id_block_protection = %1;").
                                  arg(id)))
            {
                while(query_purpose.next())
                {
                    QString key_purpose = query_purpose.value("key").toString();
                    int     bit         = query_purpose.value("bit").toInt();

                    purpose << block_protection_purpose_t({ key_purpose, bit });
                }
            }

            block_list << block_protection_t({ id, key, address, name, description, purpose });
        }
    }

    return block_list;
}
//--------------------------------------
bool ConfiguratorWindow::deleteLogFile()
{
    QDir dir;

    if(dir.exists("outputs/log.txt"))
    {
        if(dir.remove("outputs/log.txt"))
            return true;
    }

    return false;
}
//------------------------------------------------------------------------------------
void ConfiguratorWindow::showErrorMessage(const QString& title, CModBusDataUnit& unit)
{
    if(unit.error() != CModBusDataUnit::ERROR_NO)
    {
        QMessageBox::warning(this, title, tr("Ошибка: %1.").
                             arg(CModBusDataUnit::errorDescription(unit.error())));
    }
}
//-----------------------------------------------------------------
void ConfiguratorWindow::endJournalReadProcess(const QString& text)
{
    m_journal_read_current->header()->setTextElapsedTime(m_time_process.elapsed());
    m_journal_read_current->header()->setTextTableCountMessages(m_journal_read_current->table()->rowCount());

    m_journal_read_current = nullptr;

    emit buttonReadJournalStateChanged(); // отключаем кнопку чтения журналов

    disconnect(ui->pushButtonJournalRead, &QPushButton::clicked, this, stopProgressbar);
    m_progressbar->progressStop();

    m_popup->setPopupText(text);
    m_popup->show();
}
//---------------------------------------------------------------------------------------------------------------------
float ConfiguratorWindow::newCalibrationOfCurrentFactor(float standard, float cur_factor, QVector<float>& measure_list)
{
    float measure = 0;

    if(measure_list.count() > 1)
        standard *= measure_list.count();

    for(float value: measure_list)
        measure += value;

    return (standard/measure)*cur_factor;
}
/*!
 * \brief ConfiguratorWindow::standardDeviation
 * \param list Набор данных
 * \return Среднее значение + среднеквадратичное отклонение ввиде QPointF(среднее значение, среднеквадратичное отклонение)
 */
QPointF ConfiguratorWindow::standardDeviation(QVector<float>& list)
{
    float average = 0;

    for(float value: list)
        average += value;

    average /= list.count();

    float deviation = 0;

    for(float value: list)
        deviation += (value - average)*(value - average);

    deviation = sqrt(deviation/(list.count() - 1));

    return QPointF(average, deviation);
}
/*!
 * \brief ConfiguratorWindow::writeDataToExcel
 * \param doc Текущий документ Excel
 * \param name_group Имя группы (н-р, МТЗ1)
 * \param grid Указатель на грид в котором находятся поля с данными
 * \return возвращает индекс последней строки
 *
 * Запись уставок из грида в excel
 */
int ConfiguratorWindow::writeDataToExcel(QXlsx::Document& doc, const QString& group, const QGridLayout* grid, int offset)
{
    if(!grid)
        return -1;

    static int row_offset = 2;

    if(offset == -1)
        row_offset = 2;
    else
        row_offset += offset;

    if(!group.isEmpty())
        doc.write(QString("A%1").arg(row_offset++), group);

    int pos_first  = row_offset; // позиция начала сворачивания группы строк
    int rows       = grid->rowCount();
    int columns    = grid->columnCount();
    int row_count  = 0; // счетчик вставленных строк в Excel

    for(int col = 0; col < columns - 1; col += 2)
    {
        int col_pos = col;

        if(columns%2 && col != 0)
            col_pos++;

        for(int row = 0; row < rows; row++)
        {
            QLayoutItem* layout_item_value = grid->itemAtPosition(row, col_pos + 1);
            QLayoutItem* layout_item_param = grid->itemAtPosition(row, col_pos);

            if(!layout_item_value || !layout_item_param)
                continue;

            QWidget* wgt_value = layout_item_value->widget();
            QWidget* wgt_param = layout_item_param->widget();

            if(!wgt_value || !wgt_param)
                continue;

            QString name_param  = qobject_cast<QLabel*>(wgt_param)->text();
            QString str_value   = "";
            QString value_range = "";

            if(QString(wgt_value->metaObject()->className()).toUpper() == "QCOMBOBOX")
            {
                QComboBox* cb = qobject_cast<QComboBox*>(wgt_value);

                if(cb)
                {
                    for(int i = 0; i < cb->count(); i++)
                    {
                        value_range += QString("%1 - %2").arg(i + 1).arg(cb->itemText(i));

                        if(i != cb->count() - 1)
                            value_range += "; ";
                    }

                    str_value = QString("%1").arg(cb->currentIndex() + 1);
                }
            }
            else if(QString(wgt_value->metaObject()->className()).toUpper() == "CLINEEDIT")
            {
                CLineEdit* le = qobject_cast<CLineEdit*>(wgt_value);
                str_value = le->text();
                int pos = name_param.indexOf('(');

                if(pos != -1)
                {
                    value_range = name_param.mid(pos, name_param.length() - 1);
                    name_param  = name_param.mid(0, pos - 1);
                }
            }

            int row_pos = row;

            if(col != 0)
                row_pos = row_count;

            doc.write(QString("B%1").arg(row_pos + row_offset), name_param);
            doc.write(QString("C%1").arg(row_pos + row_offset), str_value);
            doc.write(QString("D%1").arg(row_pos + row_offset), value_range);

            row_count++;
        }
    }

    row_offset += row_count;
    doc.groupRows(pos_first, row_offset - 1, true);

    emit m_progressbar->increment();

    return row_offset;
}
//------------------------------------
void ConfiguratorWindow::initConnect()
{
    connect(ui->toolButtonConnect, &QToolButton::clicked, this, &ConfiguratorWindow::serialPortCtrl);
    connect(m_modbus, &CModBus::stateChanged, this, &ConfiguratorWindow::stateChanged);
    connect(m_modbus, &CModBus::stateChanged, ui->pushButtonJournalRead, &QPushButton::setEnabled);
    connect(m_modbus, &CModBus::readyRead, this, &ConfiguratorWindow::readyReadData);

    connect(m_modbus, &CModBus::errorDevice, this, &ConfiguratorWindow::errorDevice);
    connect(m_modbus, &CModBus::stateChanged, ui->pushButtonDefaultSettings, &QPushButton::setEnabled);
    connect(m_modbus, &CModBus::baudrateChanged, m_serialPortSettings_window, &CSerialPortSetting::setBaudrate);
    connect(m_modbus, &CModBus::errorChannel, this, &ConfiguratorWindow::errorConnect);
    connect(ui->toolButtonConnect, &QPushButton::clicked, m_modbus, &CModBus::userStateCtrl);
    connect(m_serialPortSettings_window, &CSerialPortSetting::refreshSerialPort, this,
            &ConfiguratorWindow::refreshSerialPort);
    connect(m_tim_calculate, &QTimer::timeout, this, &ConfiguratorWindow::calculateRead);
    connect(ui->checkboxCalibTimeout, &QCheckBox::clicked, this, &ConfiguratorWindow::chboxCalculateTimeoutStateChanged);
    connect(ui->sboxTimeoutCalc, SIGNAL(valueChanged(int)), this, SLOT(timeCalculateChanged(int)));
    connect(ui->chboxTerminal, &QCheckBox::stateChanged, this, &ConfiguratorWindow::terminalVisiblity);
    connect(ui->pushButtonIndicatorStates, &QPushButton::clicked, this, &ConfiguratorWindow::indicatorVisiblity);

    connect(m_output_window, &CIndicatorState::closeWindow, ui->pushButtonIndicatorStates,
            &QPushButton::setChecked);
    connect(m_output_window, &CIndicatorState::closeWindow, this, &ConfiguratorWindow::indicatorVisiblity);
    connect(ui->pushButtonMonitorK10_K11, &QPushButton::clicked, this,
            &ConfiguratorWindow::monitorK10K11Visiblity);
    connect(m_monitor_purpose_window, &CMonitorPurpose::closeWindow, ui->pushButtonMonitorK10_K11,
            &QPushButton::setChecked);
    connect(ui->pushButtonOutputAll, &QPushButton::clicked, this, &ConfiguratorWindow::outputAllVisiblity);
    connect(m_outputall_window, &COutputAll::closeWindow, ui->pushButtonOutputAll, &QPushButton::setChecked);
    connect(ui->pushButtonInputs, &QPushButton::clicked, this, &ConfiguratorWindow::inputVisiblity);
    connect(m_inputs_window, &COutputAll::closeWindow, ui->pushButtonInputs, &QPushButton::setChecked);
    connect(ui->pushButtonStatusInfo, &QPushButton::clicked, this, &ConfiguratorWindow::statusInfoVisiblity);
    connect(m_status_window, &CStatusInfo::closeWindow, ui->pushButtonStatusInfo, &QPushButton::setChecked);
    connect(m_status_window, &CStatusInfo::buttonUpdate, this, &ConfiguratorWindow::readStatusInfo);
    connect(ui->treewgtDeviceMenu, &QTreeWidget::itemClicked, this, &ConfiguratorWindow::itemClicked);
    connect(ui->pbtnReadAllBlock, &QPushButton::clicked, this, &ConfiguratorWindow::readSettings);
    connect(ui->pbtnWriteAllBlock, &QPushButton::clicked, this, &ConfiguratorWindow::writeSettings);
    connect(ui->pbtnReadCurrentBlock, &QPushButton::clicked, this, &ConfiguratorWindow::readSetCurrent);
    connect(ui->pbtnWriteCurrentBlock, &QPushButton::clicked, this, &ConfiguratorWindow::writeSetCurrent);
    connect(ui->tbntExpandItems, &QToolButton::clicked, this, &ConfiguratorWindow::expandItemTree);
    connect(ui->pbtnVersionSoftware, &QPushButton::clicked, this, &ConfiguratorWindow::versionSowftware);
    connect(ui->toolButtonConnectSettings, &QPushButton::clicked, this, &ConfiguratorWindow::serialPortSettings);
    connect(ui->pbtnClearLedOutput, &QPushButton::clicked, this, &ConfiguratorWindow::clearIOTable);
    connect(ui->pbtnClearDiscreteInput, &QPushButton::clicked, this, &ConfiguratorWindow::clearIOTable);
    connect(ui->pbtnClearRelayOutput, &QPushButton::clicked, this, &ConfiguratorWindow::clearIOTable);
    connect(ui->pbtnClearKeyboardPurpose, &QPushButton::clicked, this, &ConfiguratorWindow::clearIOTable);
    connect(ui->pbtnClearKeyboardProtectionCtrl, &QPushButton::clicked, this, &ConfiguratorWindow::clearIOTable);
    connect(ui->pushButtonJournalRead, &QPushButton::clicked, this, &ConfiguratorWindow::processReadJournals);
    connect(ui->pushButtonJournalClear, &QPushButton::clicked, this, &ConfiguratorWindow::clearJournal);
    connect(this, &ConfiguratorWindow::buttonReadJournalStateChanged, ui->pushButtonJournalRead,
            &QPushButton::setChecked);
    connect(ui->pbtnMenuExit, &QPushButton::clicked, this, &ConfiguratorWindow::exitFromApp);
    connect(ui->pbtnMenuExportToPDF, &QPushButton::clicked, this, &ConfiguratorWindow::startExportToPDF);
    connect(ui->pushButtonExport, &QPushButton::clicked, this, &ConfiguratorWindow::processExport);
    connect(ui->pushButtonImport, &QPushButton::clicked, this, &ConfiguratorWindow::processImport);
    connect(ui->stwgtMain, &QStackedWidget::currentChanged, this, &ConfiguratorWindow::widgetStackIndexChanged);
    connect(m_timer_synchronization, &QTimer::timeout, this, &ConfiguratorWindow::timeoutSynchronization);
    connect(ui->pbtnFilter, &QPushButton::clicked, this, &ConfiguratorWindow::filterDialog);
    connect(ui->pushButtonDefaultSettings, &QPushButton::clicked, this,
            &ConfiguratorWindow::deviceDefaultSettings);
    connect(ui->dateEdit, &QDateEdit::dateChanged, this, &ConfiguratorWindow::dateDeviceChanged);
    connect(m_serialPortSettings_window, &CSerialPortSetting::autospeed, this,
            &ConfiguratorWindow::autospeedStateChanged);

    connect(ui->splitterCentralWidget, &QSplitter::splitterMoved, this, &ConfiguratorWindow::panelMoved);
    connect(ui->splitterPanelMessage, &QSplitter::splitterMoved, this, &ConfiguratorWindow::panelMoved);
    connect(ui->pushButtonMenuDeviceCtrl, &QPushButton::clicked, this, &ConfiguratorWindow::panelButtonCtrlPress);
    connect(ui->pushButtonPanelMessage, &QPushButton::clicked, this, &ConfiguratorWindow::panelButtonCtrlPress);
    connect(ui->pushButtonVariableCtrl, &QPushButton::clicked, this, &ConfiguratorWindow::panelButtonCtrlPress);
    connect(m_monitor_purpose_window, &CMonitorPurpose::buttonUpdate, this,
            &ConfiguratorWindow::sendMonitorPurposeK10_K11Request);
    connect(m_outputall_window, &COutputAll::buttonUpdate, this, &ConfiguratorWindow::sendOutputAllRequest);
    connect(m_inputs_window, &COutputAll::buttonUpdate, this, &ConfiguratorWindow::sendInputStatesRequest);
    connect(ui->pushButtonSyncDateTime, &QPushButton::clicked, this, &ConfiguratorWindow::synchronizationDateTime);
    connect(m_output_window, &CIndicatorState::buttonUpdate, this, &ConfiguratorWindow::sendOutputAllRequest);
    connect(m_debuginfo_window, &CDebugInfo::readInfo, this, &ConfiguratorWindow::debugInfoCtrl);
    connect(ui->pushButtonDebugInfo, &QPushButton::clicked, this, &ConfiguratorWindow::debugInfoRead);
    connect(ui->pushButtonDebugInfo, &QPushButton::clicked, this, &ConfiguratorWindow::debugInfoVisiblity);
    connect(m_debuginfo_window, &CDebugInfo::closeWindow, ui->pushButtonDebugInfo, &QPushButton::setChecked);
    connect(m_tim_debug_info, &QTimer::timeout, this, &ConfiguratorWindow::timeoutDebugInfo);
    connect(m_journal_timer, &QTimer::timeout, this, &ConfiguratorWindow::timeoutJournalRead);
    connect(ui->checkBoxTestStyle, &QCheckBox::clicked, this, &ConfiguratorWindow::testStyle);
    connect(m_serialPortSettings_window, &CSerialPortSetting::updateSettings, this,
            &ConfiguratorWindow::updateSerialPortSettings);
    connect(ui->widgetMenuBar, &CMenuBar::closeWindow, this, &ConfiguratorWindow::close);
    connect(ui->widgetMenuBar, &CMenuBar::expandedWindow, this, &ConfiguratorWindow::expandedWindow);
    connect(ui->widgetMenuBar, &CMenuBar::minimizeWindow, this, &ConfiguratorWindow::showMinimized);
    connect(ui->widgetMenuBar, &CMenuBar::menubarMouseUpdatePosition, this, &ConfiguratorWindow::mouseMove);

    connect(ui->pbtnMenuNewProject, &QPushButton::clicked, this, &ConfiguratorWindow::newProject);
    connect(ui->pbtnMenuOpenProject, &QPushButton::clicked, this, &ConfiguratorWindow::openProject);
    connect(ui->pbtnMenuSaveProject, &QPushButton::clicked, this, &ConfiguratorWindow::saveProject);
    connect(ui->pbtnMenuSaveAsProject, &QPushButton::clicked, this, &ConfiguratorWindow::saveAsProject);

    connect(ui->widgetMenuBar->widgetMenu(), &CWidgetMenu::closeWindow, this, &ConfiguratorWindow::close);
    connect(ui->widgetMenuBar->widgetMenu(), &CWidgetMenu::newProject, this, &ConfiguratorWindow::newProject);
    connect(ui->widgetMenuBar->widgetMenu(), &CWidgetMenu::openProject, this, &ConfiguratorWindow::openProject);
    connect(ui->widgetMenuBar->widgetMenu(), &CWidgetMenu::saveProject, this, &ConfiguratorWindow::saveProject);
    connect(ui->widgetMenuBar->widgetMenu(), &CWidgetMenu::saveAsProject, this,
            &ConfiguratorWindow::saveAsProject);
    connect(ui->widgetMenuBar->widgetMenu(), &CWidgetMenu::exportToPDFProject, this,
            &ConfiguratorWindow::exportToPDFProject);
    connect(ui->widgetMenuBar->widgetMenu(), &CWidgetMenu::exportToExcelProject, this,
            &ConfiguratorWindow::exportToExcelProject);
    connect(ui->widgetMenuBar->widgetMenu(), &CWidgetMenu::importFromExcelProject, this,
            &ConfiguratorWindow::importFromExcelProject);
    connect(ui->widgetMenuBar->widgetMenu(), &CWidgetMenu::closeProject, this, &ConfiguratorWindow::closeProject);
    connect(ui->widgetMenuBar, &CMenuBar::minimizeMenu, this, &ConfiguratorWindow::minimizeTabMenu);
    connect(ui->checkBoxPanelMessage, &QCheckBox::clicked, this, &ConfiguratorWindow::panelMessageVisiblity);
    connect(ui->widgetCalibrationOfCurrent, &CCalibrationWidget::calibration, this,
            &ConfiguratorWindow::calibrationOfCurrent);
    connect(ui->widgetCalibrationOfCurrent, &CCalibrationWidget::apply, this, &ConfiguratorWindow::calibrationOfCurrentWrite);
    connect(ui->widgetCalibrationOfCurrent, &CCalibrationWidget::saveToFlash, this, &ConfiguratorWindow::sendDeviceCommand);

    CTerminal* terminal = qobject_cast<CTerminal*>(m_terminal_window->widget());

    if(terminal)
    {
        connect(m_modbus, &CModBus::rawData, terminal, &CTerminal::appendData);
        connect(terminal, &CTerminal::sendDeviceCommand, this, &ConfiguratorWindow::sendDeviceCommand);
        connect(terminal, &CTerminal::closeTerminal, this, &ConfiguratorWindow::terminalVisiblity);
    }
}
