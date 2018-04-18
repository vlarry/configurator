#include "configuratorwindow.h"
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
    m_versionWidget(nullptr),
    m_timer_synchronization(nullptr),
    m_status_bar(nullptr),
    m_watcher(nullptr),
    m_progressbar(nullptr),
    m_settings(nullptr),
    m_active_journal_current(nullptr),
    m_journal_read_current(nullptr)
{
    ui->setupUi(this);

    CModbus::baudrate_list_t baudrate_list;

    for(int i = 0; i < ui->cboxBaudrate->count(); i++)
    {
        baudrate_list << ui->cboxBaudrate->itemText(i).toInt();
    }

    m_modbusDevice              = new CModbus(baudrate_list, this);
    m_calculateWidget           = new QPanel(this);
    m_tim_calculate             = new QTimer(this);
    m_terminal                  = new CTerminal(this);
    m_logFile                   = new QFile("Log.txt");
    m_serialPortSettings        = new CSerialPortSetting;
    m_status_bar                = new CStatusBar(statusBar());
    m_watcher                   = new QFutureWatcher<void>(this);
    m_progressbar               = new CProgressBarWidget(this);
    m_settings                  = new QSettings(QSettings::IniFormat, QSettings::UserScope, ORGANIZATION_NAME, "configurator",
                                                this);
    m_timer_synchronization     = new QTimer(this);

    m_calculateWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    m_calculateWidget->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
    m_calculateWidget->setWindowTitle(tr("Расчетные величины"));
    addDockWidget(Qt::RightDockWidgetArea, m_calculateWidget);

    m_calculateWidget->hide();

    CVariableWidget* vwgt = new CVariableWidget;

    ui->frameDockPanel->setWidget(vwgt);
    ui->frameDockPanel->setHeaderText(tr("Расчетные величины"));

    m_status_bar->addWidget(m_progressbar);
    statusBar()->addPermanentWidget(m_status_bar, 100);

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
    initLineEditValidator();

    if(!m_logFile->open(QFile::ReadWrite))
    {
        m_status_bar->setStatusMessage(tr("Ошибка. Невозможно открыть log-файл"));
    }
    else
        saveLog(tr("Запуск программы..."));

    refreshSerialPort();
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

    if(m_logFile)
    {
        saveLog(tr("Завершение работы программы."));

        m_logFile->close();
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
        m_status_bar->clearSerialNumber(); // удаляем старый серийный номер
        synchronization(true); // запускаем синхронизацию
        readJournalCount(); // читаем количество сообщений в каждом журнале
    }
    else
    {
        saveLog(tr("Порт <") + m_modbusDevice->portName() + tr("> закрыт."));
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
    CDataUnitType unit_part1(ui->sboxSlaveID->value(), CDataUnitType::ReadInputRegisters,
                             CALCULATE_ADDRESS_PART1, QVector<quint16>() << 66);
    unit_part1.setProperty(tr("REQUEST"), CALCULATE_TYPE);
    unit_part1.setProperty("PART", CALCULATE_ADDRESS_PART1);

    sendCalculateRead(unit_part1);

    CDataUnitType unit_part2(ui->sboxSlaveID->value(), CDataUnitType::ReadInputRegisters,
                             CALCULATE_ADDRESS_PART2, QVector<quint16>() << 8);
    unit_part2.setProperty(tr("REQUEST"), CALCULATE_TYPE);
    unit_part2.setProperty("PART", CALCULATE_ADDRESS_PART2);

    sendCalculateRead(unit_part2);
}
//------------------------------------------------------
void ConfiguratorWindow::journalRead(const QString& key)
{
    if(key.isEmpty() || m_journal_set.find(key) == m_journal_set.end())
        return;

    journal_set_t& set = m_journal_set[key];
    int sector_size = 4096/set.message.size; // размер сектора в сообщениях, т.е. сколько можно сообщений считать без перехода

    if((set.isStart && set.message.read_limit == set.message.read_count) || (set.isStart && set.isStop))
    {
        m_journal_read_current->header()->setTextElapsedTime(m_time_process.elapsed());
        m_journal_read_current->header()->setTextTableCountMessages(m_journal_read_current->table()->rowCount());

        m_journal_read_current = nullptr;

        set.isStart = false;
        set.isStop  = false;

        emit buttonReadJournalStateChanged(); // отключаем кнопку чтения журналов

        return;
    }

    if(!set.isStart) // если это первый запрос сообщения
    {
        m_journal_read_current = m_active_journal_current; // сохраняем текущий виджет таблицы

        if(m_journal_read_current->table()->rowCount() > 0) // есть данные в таблице
        {
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
                    set.message.read_limit = pos_end - pos_beg; // устанавливаем сообщение до которого будем читать
                }
            }
        }

        if(set.message.read_start >= sector_size) // если начальное сообщение находится не в на первой странице
        {
            set.shift_ptr            = (set.message.read_start/sector_size)*4096; // получаем смещение указателя
            set.message.read_current = set.message.read_start%sector_size; // устанавливаем текущее сообщение (для определения перехода указатеяля)
        }

        setJournalPtrShift(key, set.shift_ptr);

        m_time_process.start();
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

    CDataUnitType unit(ui->sboxSlaveID->value(), CDataUnitType::ReadInputRegisters, address, QVector<quint16>() << msg_size);
    unit.setProperty(tr("REQUEST"), READ_JOURNAL);
    unit.setProperty(tr("JOURNAL"), key);

    m_modbusDevice->request(unit);
}
/*!
 * \brief ConfiguratorWindow::inputAnalogGeneralRead
 *
 * Чтение настроек "Настройки-аналоговые-основные"
 */
void ConfiguratorWindow::inputAnalogGeneralRead()
{
    sendSettingReadRequest(tr("M01"), tr("M03"), CDataUnitType::ReadHoldingRegisters, 6);
    sendSettingControlReadRequest("M04"); // чтение состояния настройки
}
/*!
 * \brief ConfiguratorWindow::inputAnalogCalibrateRead
 * Чтение настроек "Настройки-аналоговые-калибровки"
 */
void ConfiguratorWindow::inputAnalogCalibrateRead()
{
    sendSettingReadRequest(tr("KIA"), tr("KU0X_"), CDataUnitType::ReadHoldingRegisters, 36);
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
    sendSettingWriteRequest("M01", "M03");
    sendSettingControlWriteRequest("M04"); // запись состояния настройки
}
/*!
 * \brief ConfiguratorWindow::inputAnalogCalibrateWrite
 *
 * Запись настроек калибровки
 */
void ConfiguratorWindow::inputAnalogCalibrateWrite()
{
    sendSettingWriteRequest("KIA", "KU0X_");
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
    sendSettingControlWriteRequest("M05");
    sendSettingWriteRequest("M06", "X01");
}
/*!
 * \brief ConfiguratorWindow::protectionMTZ2Write
 *
 * Запись настроек МТЗ2
 */
void ConfiguratorWindow::protectionMTZ2Write()
{
    sendSettingControlWriteRequest("M09");
    sendSettingWriteRequest("M10", "X03");
}
/*!
 * \brief ConfiguratorWindow::protectionMTZ3Write
 *
 * Запись настроек МТЗ3
 */
void ConfiguratorWindow::protectionMTZ3Write()
{
    sendSettingControlWriteRequest("M13");
    sendSettingControlWriteRequest("TZ");
    sendSettingWriteRequest("M14", "K22");
    sendSettingWriteRequest("X04", "X04");
    sendSettingWriteRequest("TZ1", "TZ7");
}
/*!
 * \brief ConfiguratorWindow::protectionMTZ4Write
 *
 * Запись настроек МТЗ4
 */
void ConfiguratorWindow::protectionMTZ4Write()
{
    sendSettingControlWriteRequest("M16");
    sendSettingWriteRequest("M17", "X05a");
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
    sendSettingControlWriteRequest("M32");
    sendSettingWriteRequest("M33", "X11");
}
/*!
 * \brief ConfiguratorWindow::protectionUmax2Write
 *
 * Запись защиты Umax2
 */
void ConfiguratorWindow::protectionUmax2Write()
{
    sendSettingControlWriteRequest("M35");
    sendSettingWriteRequest("M36", "X12");
}
/*!
 * \brief ConfiguratorWindow::protectionUmin1Write
 *
 * Запись защиты Umin1
 */
void ConfiguratorWindow::protectionUmin1Write()
{
    sendSettingControlWriteRequest("M38");
    sendSettingControlWriteRequest("M39");
    sendSettingControlWriteRequest("M40");
    sendSettingWriteRequest("M41", "X13");
}
/*!
 * \brief ConfiguratorWindow::protectionUmin2Write
 *
 * Запись защиты Umin2
 */
void ConfiguratorWindow::protectionUmin2Write()
{
    sendSettingControlWriteRequest("M43");
    sendSettingControlWriteRequest("M44");
    sendSettingControlWriteRequest("M45");
    sendSettingWriteRequest("M46", "X14");
}
/*!
 * \brief ConfiguratorWindow::protection3UOWrite
 *
 * Запись защиты 3UO
 */
void ConfiguratorWindow::protection3UOWrite()
{
    sendSettingControlWriteRequest("M48");
    sendSettingWriteRequest("M49", "X15");
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
    protection3UOWrite();
}
/*!
 * \brief ConfiguratorWindow::protectionOZZ1Write
 *
 * Запись защиты ОЗЗ1
 */
void ConfiguratorWindow::protectionOZZ1Write()
{
    sendSettingControlWriteRequest("M22");
    sendSettingWriteRequest("M23", "X07");
}
/*!
 * \brief ConfiguratorWindow::protectionOZZ2Write
 *
 * Запись защиты ОЗЗ2
 */
void ConfiguratorWindow::protectionOZZ2Write()
{
    sendSettingControlWriteRequest("K23");
    sendSettingWriteRequest("K24", "X07a");
}
/*!
 * \brief ConfiguratorWindow::protectionNZZ1Write
 *
 * Запись защиты НЗЗ1
 */
void ConfiguratorWindow::protectionNZZ1Write()
{
    sendSettingControlWriteRequest("M25");
    sendSettingWriteRequest("M26", "X09");
}
/*!
 * \brief ConfiguratorWindow::protectionNZZ2Write
 *
 * Запись защиты НЗЗ2
 */
void ConfiguratorWindow::protectionNZZ2Write()
{
    sendSettingControlWriteRequest("K26");
    sendSettingWriteRequest("K27", "X09a");
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
    sendSettingControlWriteRequest("M51");
    sendSettingWriteRequest("M52", "X16");
}
/*!
 * \brief ConfiguratorWindow::protectionAchr2Write
 *
 * Запись защиты АЧР2
 */
void ConfiguratorWindow::protectionAchr2Write()
{
    sendSettingControlWriteRequest("M55");
    sendSettingWriteRequest("M56", "X17");
}
/*!
 * \brief ConfiguratorWindow::protectionAchr3Write
 *
 * Запись защиты АЧР3
 */
void ConfiguratorWindow::protectionAchr3Write()
{
    sendSettingControlWriteRequest("M59");
    sendSettingWriteRequest("M60", "X18");
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
    sendSettingControlWriteRequest("M63");
    sendSettingWriteRequest("M64", "X19");
}
/*!
 * \brief ConfiguratorWindow::protectionExt1Write
 *
 * Запись защиты Внешняя1
 */
void ConfiguratorWindow::protectionExt1Write()
{
    sendSettingControlWriteRequest("M71");
    sendSettingWriteRequest("M72", "M72");
}
/*!
 * \brief ConfiguratorWindow::protectionExt2Write
 *
 * Запись защиты Внешняя2
 */
void ConfiguratorWindow::protectionExt2Write()
{
    sendSettingControlWriteRequest("M73");
    sendSettingWriteRequest("M74", "M74");
}
/*!
 * \brief ConfiguratorWindow::protectionExt3Write
 *
 * Запись защиты Внешняя3
 */
void ConfiguratorWindow::protectionExt3Write()
{
    sendSettingControlWriteRequest("M75");
    sendSettingWriteRequest("M76", "M76");
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
    sendSettingControlWriteRequest("M19");
    sendSettingWriteRequest("M20", "X06");
}
/*!
 * \brief ConfiguratorWindow::protectionIminWrite
 *
 * Запись защиты Imin
 */
void ConfiguratorWindow::protectionIminWrite()
{
    sendSettingControlWriteRequest("M29");
    sendSettingWriteRequest("M30", "X10");
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
    sendSettingControlWriteRequest("M65");
    sendSettingControlWriteRequest("M66");
    sendSettingWriteRequest("M67", "X20");
}
/*!
 * \brief ConfiguratorWindow::protectionTemp2Write
 *
 * Запись защиты Температурная2
 */
void ConfiguratorWindow::protectionTemp2Write()
{
    sendSettingControlWriteRequest("M65");
    sendSettingControlWriteRequest("M66");
    sendSettingWriteRequest("M68", "X21");
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
    sendSettingControlWriteRequest("M77");
    sendSettingWriteRequest("M78", "M78");
}
/*!
 * \brief ConfiguratorWindow::protectionLevel2Write
 *
 * Запись защиты Уровневая2
 */
void ConfiguratorWindow::protectionLevel2Write()
{
    sendSettingControlWriteRequest("M77");
    sendSettingWriteRequest("M79", "M79");
}
/*!
 * \brief ConfiguratorWindow::protectionSignalStartWrite
 *
 * Запись защиты Сигнал Пуска
 */
void ConfiguratorWindow::protectionSignalStartWrite()
{
    QVector<QComboBox*> box_list = QVector<QComboBox*>() << ui->cboxN50 << ui->cboxN52 << ui->cboxN53 << ui->cboxN54 <<
                                                            ui->cboxN55 << ui->cboxN56 << ui->cboxN57 << ui->cboxN58 <<
                                                            ui->cboxN59 << ui->cboxV04 << ui->cboxV07 << ui->cboxV10 <<
                                                            ui->cboxV13 << ui->cboxV16 << ui->cboxV19 << ui->cboxV22 <<
                                                            ui->cboxV25 << ui->cboxV28 << ui->cboxV31 << ui->cboxV36 <<
                                                            ui->cboxV39 << ui->cboxV44 << ui->cboxV50 << ui->cboxV62 <<
                                                            ui->cboxV65 << ui->cboxV68 << ui->cboxV76 << ui->cboxV77 <<
                                                            ui->cboxV81 << ui->cboxV86 << ui->cboxV90 << ui->cboxV95 <<
                                                            ui->cboxV96;

    QVector<quint16> data(24, 0); // 24 ячейки со значением нуль

    for(QComboBox* box: box_list)
    {
        QString key = box->objectName().remove("cbox");

        if(key.isEmpty())
            continue;

        int bit     = m_variable_bits[key];
        int val_pos = bit/16;
        int bit_pos = bit%16;

        if(val_pos < data.count())
        {
            int item_pos = box->currentIndex();

            if(item_pos == 1)
                data[val_pos] |= (1 << bit_pos);
        }
    }

    QVector<quint16> tdata;

    for(int i = 0; i < data.count() - 1; i += 2) // меняем местами старший и младший байт
    {
        tdata << data[i + 1] << data[i];
    }

    int addr = addressSettingKey("M80");

    CDataUnitType unit(ui->sboxSlaveID->value(), CDataUnitType::WriteMultipleRegisters, addr, tdata);

    unit.setProperty("REQUEST", PORTECT_RESERVE_SIGNAL_START);

    m_modbusDevice->request(unit);
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
    sendSettingControlWriteRequest("M93");
    sendSettingControlWriteRequest("M95");
    sendSettingWriteRequest("M96", "M99");
}
/*!
 * \brief ConfiguratorWindow::protectionVacuumWrite
 *
 * Запись защиты Вакуум
 */
void ConfiguratorWindow::protectionVacuumWrite()
{
    sendSettingControlWriteRequest("M90");
    sendSettingWriteRequest("M91", "X23");
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
    sendSettingControlWriteRequest("K32");
    sendSettingControlWriteRequest("K01");
    sendSettingControlWriteRequest("K03");
    sendSettingControlWriteRequest("K06");
    sendSettingControlWriteRequest("K17");
    sendSettingControlWriteRequest("K07");
    sendSettingWriteRequest("K02", "X22");
}
/*!
 * \brief ConfiguratorWindow::automationSwitchTruckWrite
 *
 * Запись автоматик Тележка Выключателя
 */
void ConfiguratorWindow::automationSwitchTruckWrite()
{
    sendSettingControlWriteRequest("K37");
    sendSettingControlWriteRequest("K41");
    sendSettingWriteRequest("K45", "K49");
}
/*!
 * \brief ConfiguratorWindow::automationBlockWrite
 *
 * Запись автоматик Блокировок
 */
void ConfiguratorWindow::automationBlockWrite()
{
    sendSettingControlWriteRequest("K13");
    sendSettingControlWriteRequest("K14");
    sendSettingControlWriteRequest("K15");
}
/*!
 * \brief ConfiguratorWindow::automationBusWrite
 *
 * Запись автоматик Шинные разъединители
 */
void ConfiguratorWindow::automationBusWrite()
{
    sendSettingControlWriteRequest("K34");
    sendSettingControlWriteRequest("K38");
    sendSettingWriteRequest("K42", "K46");
}
/*!
 * \brief ConfiguratorWindow::automationLineWrite
 *
 * Запись автоматик Линейные разъединители
 */
void ConfiguratorWindow::automationLineWrite()
{
    sendSettingControlWriteRequest("K35");
    sendSettingControlWriteRequest("K39");
    sendSettingWriteRequest("K43", "K47");
}
/*!
 * \brief ConfiguratorWindow::automationEarthWrite
 *
 * Запись автоматик Землянные разъединители
 */
void ConfiguratorWindow::automationEarthWrite()
{
    sendSettingControlWriteRequest("K36");
    sendSettingControlWriteRequest("K40");
    sendSettingWriteRequest("K44", "K48");
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
    sendSettingControlWriteRequest("K18");
    sendSettingWriteRequest("K19", "K19");
}
/*!
 * \brief ConfiguratorWindow::automationAVRWrite
 *
 * Запись автоматик АВР
 */
void ConfiguratorWindow::automationAVRWrite()
{
    sendSettingControlWriteRequest("M81");
    sendSettingWriteRequest("M82", "M85");
}
/*!
 * \brief ConfiguratorWindow::automationAPVWrite
 *
 * Запись автоматик АПВ
 */
void ConfiguratorWindow::automationAPVWrite()
{
    sendSettingControlWriteRequest("M87");
    sendSettingWriteRequest("M88", "M89");
}
/*!
 * \brief ConfiguratorWindow::automationAPVSignalStartWrite
 *
 * Запись синалов пуска автоматики АПВ
 */
void ConfiguratorWindow::automationAPVSignalStartWrite()
{
    QVector<QComboBox*> box_list = QVector<QComboBox*>() << ui->cboxN50_2 << ui->cboxN52_2 << ui->cboxN53_2 << ui->cboxN54_2 <<
                                                            ui->cboxN55_2 << ui->cboxN56_2 << ui->cboxN57_2 << ui->cboxN58_2 <<
                                                            ui->cboxN59_2 << ui->cboxV04_2 << ui->cboxV07_2 << ui->cboxV10_2 <<
                                                            ui->cboxV13_2 << ui->cboxV16_2 << ui->cboxV19_2 << ui->cboxV22_2 <<
                                                            ui->cboxV25_2 << ui->cboxV28_2 << ui->cboxV31_2 << ui->cboxV36_2 <<
                                                            ui->cboxV39_2 << ui->cboxV44_2 << ui->cboxV50_2 << ui->cboxV62_2 <<
                                                            ui->cboxV65_2 << ui->cboxV68_2 << ui->cboxV76_2 << ui->cboxV77_2 <<
                                                            ui->cboxV81_2 << ui->cboxV86_2 << ui->cboxV90_2 << ui->cboxV95_2 <<
                                                            ui->cboxV96_2;

    QVector<quint16> data(24, 0); // 24 ячейки со значением нуль

    for(QComboBox* box: box_list)
    {
        QString key = (box->objectName().remove("cbox")).remove("_2");

        if(key.isEmpty())
            continue;

        int bit     = m_variable_bits[key];
        int val_pos = bit/16;
        int bit_pos = bit%16;

        if(val_pos < data.count())
        {
            int item_pos = box->currentIndex();

            if(item_pos == 1)
                data[val_pos] |= (1 << bit_pos);
        }
    }

    QVector<quint16> tdata;

    for(int i = 0; i < data.count() - 1; i += 2) // меняем местами старший и младший байт
    {
        tdata << data[i + 1] << data[i];
    }

    int addr = addressSettingKey("M86");

    CDataUnitType unit(ui->sboxSlaveID->value(), CDataUnitType::WriteMultipleRegisters, addr, tdata);

    unit.setProperty("REQUEST", AUTOMATION_SIGNAL_START);

    m_modbusDevice->request(unit);
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
/*!
 * \brief ConfiguratorWindow::dateTimeWrite
 *
 * Запись настроек дата/время
 */
void ConfiguratorWindow::dateTimeWrite()
{
    QDateTime dt(ui->dateEdit->date(), ui->timeEdit->time());

    quint16 year_month = ((((dt.date().year() - 2000) << 8)&0xFF00) | (dt.date().month()&0x00FF));
    quint16 date_wday  = (((dt.date().day() << 8)&0xFF00) | (dt.date().dayOfWeek()&0x00FF));
    quint16 hour       = (dt.time().hour()&0x00FF);
    quint16 min_second = (((dt.time().minute() << 8)&0xFF00) | (dt.time().second()&0x00FF));

    QVector<quint16> data = QVector<quint16>() << year_month << date_wday << hour << min_second;

    CDataUnitType unit(ui->sboxSlaveID->value(), CDataUnitType::WriteMultipleRegisters, 0x2000, data);

    unit.setProperty(tr("REQUEST"), DATETIME_TYPE);

    m_modbusDevice->request(unit);
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
    sendDeviceCommand(ui->comboBoxCommunicationBaudrate->currentIndex() + 6); // новая скорость
    sendRequestWrite(0x25, QVector<quint16>() << (quint16)ui->spinBoxCommunicationAddress->value(), 255);
    sendDeviceCommand(19); // установить новый адрес MODBUS
}
/*!
 * \brief ConfiguratorWindow::protectionMTZ1Read
 *
 * Чтение защиты МТЗ1
 */
void ConfiguratorWindow::protectionMTZ1Read()
{
    sendSettingControlReadRequest("M05");
    sendSettingReadRequest(tr("M06"), tr("X01"), CDataUnitType::ReadHoldingRegisters, 8);
}
/*!
 * \brief ConfiguratorWindow::protectionMTZ2Read
 *
 * Чтение защиты МТЗ2
 */
void ConfiguratorWindow::protectionMTZ2Read()
{
    sendSettingControlReadRequest("M09");
    sendSettingReadRequest(tr("M10"), tr("X03"), CDataUnitType::ReadHoldingRegisters, 8);
}
/*!
 * \brief ConfiguratorWindow::protectionMTZ3Read
 *
 * Чтение защиты МТЗ3
 */
void ConfiguratorWindow::protectionMTZ3Read()
{
    sendSettingControlReadRequest("M13");
    sendSettingControlReadRequest("TZ");
    sendSettingReadRequest(tr("M14"), tr("K22"), CDataUnitType::ReadHoldingRegisters, 4);
    sendSettingReadRequest(tr("X04"), tr("X04"), CDataUnitType::ReadHoldingRegisters, 2);
    sendSettingReadRequest(tr("TZ1"), tr("TZ7"), CDataUnitType::ReadHoldingRegisters, 14);
}
/*!
 * \brief ConfiguratorWindow::protectionMTZ4Read
 *
 * Чтение защиты МТЗ4
 */
void ConfiguratorWindow::protectionMTZ4Read()
{
    sendSettingControlReadRequest("M16");
    sendSettingReadRequest(tr("M17"), tr("X05a"), CDataUnitType::ReadHoldingRegisters, 12);
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
    sendSettingControlReadRequest("M32");
    sendSettingReadRequest(tr("M33"), tr("X11"), CDataUnitType::ReadHoldingRegisters, 6);
}
/*!
 * \brief ConfiguratorWindow::protectionUmax2Read
 *
 * Чтение защиты Umax2
 */
void ConfiguratorWindow::protectionUmax2Read()
{
    sendSettingControlReadRequest("M35");
    sendSettingReadRequest(tr("M36"), tr("X12"), CDataUnitType::ReadHoldingRegisters, 6);
}
/*!
 * \brief ConfiguratorWindow::protectionUmin1Read
 *
 * Чтение защиты Umin1
 */
void ConfiguratorWindow::protectionUmin1Read()
{
    sendSettingControlReadRequest("M38");
    sendSettingControlReadRequest("M39");
    sendSettingControlReadRequest("M40");
    sendSettingReadRequest(tr("M41"), tr("X13"), CDataUnitType::ReadHoldingRegisters, 6);
}
/*!
 * \brief ConfiguratorWindow::protectionUmin2Read
 *
 * Чтение защиты Umin2
 */
void ConfiguratorWindow::protectionUmin2Read()
{
    sendSettingControlReadRequest("M43");
    sendSettingControlReadRequest("M44");
    sendSettingControlReadRequest("M45");
    sendSettingReadRequest(tr("M46"), tr("X14"), CDataUnitType::ReadHoldingRegisters, 6);
}
/*!
 * \brief ConfiguratorWindow::protection3UORead
 *
 * Чтение защиты 3UO
 */
void ConfiguratorWindow::protection3UORead()
{
    sendSettingControlReadRequest("M48");
    sendSettingReadRequest(tr("M49"), tr("X15"), CDataUnitType::ReadHoldingRegisters, 6);
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
    protection3UORead();
}
/*!
 * \brief ConfiguratorWindow::protectionDirectedOZZ1Read
 *
 * Чтение защиты ОЗЗ1
 */
void ConfiguratorWindow::protectionOZZ1Read()
{
    sendSettingControlReadRequest("M22");
    sendSettingReadRequest(tr("M23"), tr("X07"), CDataUnitType::ReadHoldingRegisters, 6);
}
/*!
 * \brief ConfiguratorWindow::protectionDirectedOZZ2Read
 *
 * Чтение защиты ОЗЗ2
 */
void ConfiguratorWindow::protectionOZZ2Read()
{
    sendSettingControlReadRequest("K23");
    sendSettingReadRequest(tr("K24"), tr("X07a"), CDataUnitType::ReadHoldingRegisters, 6);
}
/*!
 * \brief ConfiguratorWindow::protectionDirectedNZZ1Read
 *
 * Чтение защиты НЗЗ1
 */
void ConfiguratorWindow::protectionNZZ1Read()
{
    sendSettingControlReadRequest("M25");
    sendSettingReadRequest(tr("M26"), tr("X09"), CDataUnitType::ReadHoldingRegisters, 12);
}
/*!
 * \brief ConfiguratorWindow::protectionDirectedNZZ2Read
 *
 * Чтение защиты НЗЗ2
 */
void ConfiguratorWindow::protectionNZZ2Read()
{
    sendSettingControlReadRequest("K26");
    sendSettingReadRequest(tr("K27"), tr("X09a"), CDataUnitType::ReadHoldingRegisters, 12);
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
    sendSettingControlReadRequest("M51");
    sendSettingReadRequest(tr("M52"), tr("X16"), CDataUnitType::ReadHoldingRegisters, 8);
}
/*!
 * \brief ConfiguratorWindow::protectionAchr2Read
 *
 * Чтение защиты АЧР2
 */
void ConfiguratorWindow::protectionAchr2Read()
{
    sendSettingControlReadRequest("M55");
    sendSettingReadRequest(tr("M56"), tr("X17"), CDataUnitType::ReadHoldingRegisters, 8);
}
/*!
 * \brief ConfiguratorWindow::protectionAchr3Read
 *
 * Чтение защиты АЧР3
 */
void ConfiguratorWindow::protectionAchr3Read()
{
    sendSettingControlReadRequest("M59");
    sendSettingReadRequest(tr("M60"), tr("X18"), CDataUnitType::ReadHoldingRegisters, 8);
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
    sendSettingControlReadRequest("M63");
    sendSettingReadRequest(tr("M64"), tr("X19"), CDataUnitType::ReadHoldingRegisters, 4);
}
/*!
 * \brief ConfiguratorWindow::protectionExt1
 *
 * Чтение защиты Внешняя1
 */
void ConfiguratorWindow::protectionExt1Read()
{
    sendSettingControlReadRequest("M71");
    sendSettingReadRequest(tr("M72"), tr("M72"), CDataUnitType::ReadHoldingRegisters, 2);
}
/*!
 * \brief ConfiguratorWindow::protectionExt2
 *
 * Чтение защыты Внешняя2
 */
void ConfiguratorWindow::protectionExt2Read()
{
    sendSettingControlReadRequest("M73");
    sendSettingReadRequest(tr("M74"), tr("M74"), CDataUnitType::ReadHoldingRegisters, 2);
}
/*!
 * \brief ConfiguratorWindow::protectionExt3
 *
 * Чтение защиты Внешняя3
 */
void ConfiguratorWindow::protectionExt3Read()
{
    sendSettingControlReadRequest("M75");
    sendSettingReadRequest(tr("M76"), tr("M76"), CDataUnitType::ReadHoldingRegisters, 2);
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
    sendSettingControlReadRequest("M19");
    sendSettingReadRequest(tr("M20"), tr("X06"), CDataUnitType::ReadHoldingRegisters, 6);
}
/*!
 * \brief ConfiguratorWindow::protectionIminRead
 *
 * Чтение защиты Imin
 */
void ConfiguratorWindow::protectionIminRead()
{
    sendSettingControlReadRequest("M29");
    sendSettingReadRequest(tr("M30"), tr("X10"), CDataUnitType::ReadHoldingRegisters, 6);
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
    sendSettingControlReadRequest("M65");
    sendSettingControlReadRequest("M66");
    sendSettingReadRequest(tr("M67"), tr("X20"), CDataUnitType::ReadHoldingRegisters, 6);
}
/*!
 * \brief ConfiguratorWindow::protectionTemp2Read
 *
 * Чтение защиты Температурная2
 */
void ConfiguratorWindow::protectionTemp2Read()
{
    sendSettingControlReadRequest("M65");
    sendSettingControlReadRequest("M66");
    sendSettingReadRequest(tr("M68"), tr("X21"), CDataUnitType::ReadHoldingRegisters, 6);
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
    sendSettingControlReadRequest("M77");
    sendSettingReadRequest(tr("M78"), tr("M78"), CDataUnitType::ReadHoldingRegisters, 2);
}
/*!
 * \brief ConfiguratorWindow::protectionLevel2Read
 *
 * Чтение защиты Уровень2
 */
void ConfiguratorWindow::protectionLevel2Read()
{
    sendSettingControlReadRequest("M77");
    sendSettingReadRequest(tr("M79"), tr("M79"), CDataUnitType::ReadHoldingRegisters, 2);
}
/*!
 * \brief ConfiguratorWindow::protectionSignalStartRead
 *
 * Чтение защиты Сигнала пуска
 */
void ConfiguratorWindow::protectionSignalStartRead()
{
    int addr = addressSettingKey("M80");

    CDataUnitType unit(ui->sboxSlaveID->value(), CDataUnitType::ReadHoldingRegisters, addr, QVector<quint16>() << 24);

    unit.setProperty("REQUEST", PORTECT_RESERVE_SIGNAL_START);

    m_modbusDevice->request(unit);
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
    sendSettingControlReadRequest("M93");
    sendSettingControlReadRequest("M95");
    sendSettingReadRequest(tr("M96"), tr("M99"), CDataUnitType::ReadHoldingRegisters, 8);
}
/*!
 * \brief ConfiguratorWindow::protectionVacuumRead
 *
 * Чтение защиты Вакуум
 */
void ConfiguratorWindow::protectionVacuumRead()
{
    sendSettingControlReadRequest("M90");
    sendSettingReadRequest(tr("M91"), tr("X23"), CDataUnitType::ReadHoldingRegisters, 6);
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
 * \brief ConfiguratorWindow::automationSwitchRead
 *
 * Чтение автоматика Выключатель
 */
void ConfiguratorWindow::automationSwitchRead()
{
    sendSettingControlReadRequest("K32");
    sendSettingControlReadRequest("K01");
    sendSettingControlReadRequest("K03");
    sendSettingControlReadRequest("K06");
    sendSettingControlReadRequest("K17");
    sendSettingControlReadRequest("K07");
    sendSettingReadRequest(tr("K02"), tr("X22"), CDataUnitType::ReadHoldingRegisters, 12);
}
/*!
 * \brief ConfiguratorWindow::automationSwitchTruckRead
 *
 * Чтение автоматика Тележка Выключателя
 */
void ConfiguratorWindow::automationSwitchTruckRead()
{
    sendSettingControlReadRequest("K37");
    sendSettingControlReadRequest("K41");
    sendSettingReadRequest(tr("K45"), tr("K49"), CDataUnitType::ReadHoldingRegisters, 4);
}
/*!
 * \brief ConfiguratorWindow::automationBlockRead
 *
 * Чтение автоматика Блокировки
 */
void ConfiguratorWindow::automationBlockRead()
{
    sendSettingControlReadRequest("K13");
    sendSettingControlReadRequest("K14");
    sendSettingControlReadRequest("K15");
}
/*!
 * \brief ConfiguratorWindow::automationBusRead
 *
 * Чтение автоматика Шинный разъединитель
 */
void ConfiguratorWindow::automationBusRead()
{
    sendSettingControlReadRequest("K34");
    sendSettingControlReadRequest("K38");
    sendSettingReadRequest(tr("K42"), tr("K46"), CDataUnitType::ReadHoldingRegisters, 4);
}
/*!
 * \brief ConfiguratorWindow::automationLineRead
 *
 * Чтение автоматика Линейные разъединители
 */
void ConfiguratorWindow::automationLineRead()
{
    sendSettingControlReadRequest("K35");
    sendSettingControlReadRequest("K39");
    sendSettingReadRequest(tr("K43"), tr("K47"), CDataUnitType::ReadHoldingRegisters, 4);
}
/*!
 * \brief ConfiguratorWindow::automationEarthRead
 *
 * Чтение автоматика Заземляющий разъединитель
 */
void ConfiguratorWindow::automationEarthRead()
{
    sendSettingControlReadRequest("K36");
    sendSettingControlReadRequest("K40");
    sendSettingReadRequest(tr("K44"), tr("K48"), CDataUnitType::ReadHoldingRegisters, 4);
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
    sendSettingControlReadRequest("K18");
    sendSettingReadRequest(tr("K19"), tr("K19"), CDataUnitType::ReadHoldingRegisters, 2);
}
/*!
 * \brief ConfiguratorWindow::automationAVRRead
 *
 * Чтение автоматика АВР
 */
void ConfiguratorWindow::automationAVRRead()
{
    sendSettingControlReadRequest("M81");
    sendSettingReadRequest(tr("M82"), tr("M85"), CDataUnitType::ReadHoldingRegisters, 8);
}
/*!
 * \brief ConfiguratorWindow::automationAPVSignalStartRead
 *
 * Чтение состояний автоматики сигнала пуска
 */
void ConfiguratorWindow::automationAPVSignalStartRead()
{
    int addr = addressSettingKey("M86");

    CDataUnitType unit(ui->sboxSlaveID->value(), CDataUnitType::ReadHoldingRegisters, addr, QVector<quint16>() << 24);

    unit.setProperty("REQUEST", AUTOMATION_SIGNAL_START);

    m_modbusDevice->request(unit);
}
/*!
 * \brief ConfiguratorWindow::automationAPVRead
 *
 * Чтение автоматика АПВ
 */
void ConfiguratorWindow::automationAPVRead()
{
    sendSettingControlReadRequest("M87");
    sendSettingReadRequest(tr("M88"), tr("M89"), CDataUnitType::ReadHoldingRegisters, 4);
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
//-------------------------------------
void ConfiguratorWindow::dateTimeRead()
{
    CDataUnitType unit(ui->sboxSlaveID->value(), CDataUnitType::ReadHoldingRegisters, 0x2000, QVector<quint16>() << 4);

    unit.setProperty(tr("REQUEST"), DATETIME_TYPE);

    m_modbusDevice->request(unit);
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
    Q_UNUSED(state);

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

        m_modbusDevice->clearQueueRequest();

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
//--------------------------------------------------------
void ConfiguratorWindow::responseRead(CDataUnitType& unit)
{
    if(unit.is_empty())
    {
        noConnectMessage();
        return;
    }

    quint8 error = unit.error();

    if(error != CDataUnitType::ERROR_NO) // если ошибка, то выводим ее
    {
        QMessageBox::warning(this, tr("Ответ устройства"),
                             tr("В ответе обнаружена ошибка:\n") + unit.errorStringList());
    }
    
    RequestType type = (RequestType)unit.property(tr("REQUEST")).toInt();

    if(type == CALCULATE_TYPE)
    {
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
        displaySettingResponse(unit);
    else if(type == GENERAL_CONTROL_TYPE)
        displaySettingControlResponce(unit);
    else if(type == PURPOSE_OUT_TYPE)
        displayPurposeResponse(unit);
    else if(type == PURPOSE_INPUT_TYPE)
        displayPurposeDIResponse(unit);
    else if(type == READ_JOURNAL || type == READ_JOURNAL_COUNT || type == READ_JOURNAL_SHIFT_PTR)
        processReadJournal(unit);
    else if(type == READ_SERIAL_NUMBER)
        displayDeviceSerialNumber(unit.values());
    else if(type == DATETIME_TYPE)
        displayDateTime(unit);
    else if(type == PORTECT_RESERVE_SIGNAL_START)
    {
        displayProtectReserveSignalStart(unit.values());
    }
    else if(type == AUTOMATION_SIGNAL_START)
    {
        displayAutomationAPVSignalStart(unit.values());
    }
    else if(type == COMMUNICATIONS_MODBUS_TIM_REQUEST)
    {
        displayCommunicationTimeoutRequest(unit.values());
    }
    else if(type == COMMUNICATIONS_MODBUS_TIM_SPEED)
    {
        displayCommunicationTimeoutSpeed(unit.values());
    }
    else if(type == COMMUNICATIONS_MODBUS_ADDRESS)
    {
        displayCommunicationAddress(unit.values());
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
    
    m_terminal->hide();
    ui->stwgtMain->setCurrentIndex(0);

    m_versionWidget = new CVersionSoftware(this);
    versionParser();

    ui->tabwgtMenu->setCurrentIndex(3);
    m_status_bar->connectStateChanged(false);

    ui->tabwgtMenu->setTabEnabled(4, false);

    ui->pushButtonJournalRead->setVisible(false);  // скрытие кнопки чтения журналов
    ui->pushButtonJournalClear->setVisible(false); // скрытие кнопки очистки журналов
    ui->pushButtonDefaultSettings->setVisible(false); // скрытие кнопки сброса настроек по умолчанию

    ui->comboBoxCommunicationParity->setCurrentIndex(1);

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
//--------------------------------------------------------
void ConfiguratorWindow::errorDevice(const QString& error)
{
    m_status_bar->setStatusMessage(error, 5000);
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
        QDateTime dt = QDateTime::currentDateTime();

        QString str = QString("[%1] - %3\n").arg(dt.toString("dd.MM.yyyy - hh.mm.ss")).arg(info);
        m_logFile->write(str.toStdString().c_str());
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
    }
}
//-------------------------------------
void ConfiguratorWindow::readSettings()
{
    if(!m_modbusDevice->is_open())
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
}
//---------------------------------------
void ConfiguratorWindow::readSetCurrent()
{
    if(!m_modbusDevice->is_open())
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
        break;

        case DEVICE_MENU_PROTECT_ITEM_POWER_UMIN2: // чтение защиты Umin2
            protectionUmin2Read();
        break;

        case DEVICE_MENU_PROTECT_ITEM_POWER_3UO: // чтение защиты 3UO
            protection3UORead();
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

        case DEVICE_MENU_ITEM_SETTINGS_ITEM_LEDS: // чтение настройки Светодиоды
            purposeLedsRead();
        break;

        case DEVICE_MENU_ITEM_SETTINGS_ITEM_IO_MDVV01_INPUTS:
            purposeInputRead();
        break;

        case DEVICE_MENU_ITEM_SETTINGS_ITEM_IO_MDVV01_RELAY:
            purposeRelayRead();
        break;

        case DEVICE_MENU_ITEM_SETTINGS_ITEM_DATETIME:
            dateTimeRead();
        break;

        case DEVICE_MENU_ITEM_SETTINGS_ITEM_COMMUNICATIONS:
            settingCommunicationsRead();
        break;

        default: return;
    }
}
//--------------------------------------
void ConfiguratorWindow::writeSettings()
{
    if(!m_modbusDevice->is_open())
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

    sendDeviceCommand(2);
}
//----------------------------------------
void ConfiguratorWindow::writeSetCurrent()
{
    if(!m_modbusDevice->is_open())
    {
        noConnectMessage();
        return;
    }

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
        break;

        case DEVICE_MENU_PROTECT_ITEM_POWER_UMIN2: // запись защиты Umin2
            protectionUmin2Write();
        break;

        case DEVICE_MENU_PROTECT_ITEM_POWER_3UO: // запись защиты 3UO
            protection3UOWrite();
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

        case DEVICE_MENU_ITEM_AUTOMATION_APV: // запись автоматики АПВ
            automationAPVWrite();
            automationAPVSignalStartWrite();
        break;

        case DEVICE_MENU_ITEM_AUTOMATION_APV_SIGNAL_START: // чтение автоматики АПВ сигналы пуска
            automationAPVSignalStartWrite();
        break;

        case DEVICE_MENU_ITEM_SETTINGS_ITEM_LEDS: // запись настройки Светодиоды
            purposeLedsWrite();
        break;

        case DEVICE_MENU_ITEM_SETTINGS_ITEM_IO_MDVV01_INPUTS: // запись настройки Дискретные входы
            purposeInputWrite();
        break;

        case DEVICE_MENU_ITEM_SETTINGS_ITEM_IO_MDVV01_RELAY: // запись настройки Реле
            purposeRelayWrite();
        break;

        case DEVICE_MENU_ITEM_SETTINGS_ITEM_DATETIME: // запись настройки дата/время
            dateTimeWrite();
        break;

        case DEVICE_MENU_ITEM_SETTINGS_ITEM_COMMUNICATIONS:
            settingCommunicationsWrite();
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
//-------------------------------------------------------------
void ConfiguratorWindow::sendCalculateRead(CDataUnitType& unit)
{
    m_modbusDevice->request(unit);
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
    QTreeWidgetItem* currentItemMTZ1 = new QTreeWidgetItem(protectItemCurrent, QStringList() << tr("МТЗ1"),
                                                           DEVICE_MENU_PROTECT_ITEM_CURRENT_MTZ1); // защита МТЗ1
    QTreeWidgetItem* currentItemMTZ2 = new QTreeWidgetItem(protectItemCurrent, QStringList() << tr("МТЗ2"),
                                                           DEVICE_MENU_PROTECT_ITEM_CURRENT_MTZ2); // защита МТЗ2
    QTreeWidgetItem* currentItemMTZ3 = new QTreeWidgetItem(protectItemCurrent, QStringList() << tr("МТЗ3"),
                                                           DEVICE_MENU_PROTECT_ITEM_CURRENT_MTZ3); // защита МТЗ3

    QTreeWidgetItem* currentItemMTZ3_set_character = new QTreeWidgetItem(currentItemMTZ3,
                                                                         QStringList() << tr("Настройка характеристик"),
                                                                         DEVICE_MENU_PROTECT_ITEM_CURRENT_MTZ3_SET_CHAR);

    currentItemMTZ3->addChild(currentItemMTZ3_set_character);

    QTreeWidgetItem* currentItemMTZ3_charact_steep   = new QTreeWidgetItem(currentItemMTZ3_set_character,
                                                                           QStringList() << tr("Крутая"),
                                                                           DEVICE_MENU_PROTECT_ITEM_CURRENT_MTZ3_PROP_STEEP);
    QTreeWidgetItem* currentItemMTZ3_charact_sloping = new QTreeWidgetItem(currentItemMTZ3_set_character,
                                                                           QStringList() << tr("Пологая"),
                                                                           DEVICE_MENU_PROTECT_ITEM_CURRENT_MTZ3_PROP_SLOP);
    QTreeWidgetItem* currentItemMTZ3_charact_inverse = new QTreeWidgetItem(currentItemMTZ3_set_character,
                                                                           QStringList() << tr("Инверсная"),
                                                                           DEVICE_MENU_PROTECT_ITEM_CURRENT_MTZ3_PROP_INV);
    QTreeWidgetItem* currentItemMTZ3_charact_dur_inv = new QTreeWidgetItem(currentItemMTZ3_set_character,
                                                                           QStringList() << tr("Длительно инверсная"),
                                                                           DEVICE_MENU_PROTECT_ITEM_CURRENT_MTZ3_PROP_DINV);
    QTreeWidgetItem* currentItemMTZ3_charact_back    = new QTreeWidgetItem(currentItemMTZ3_set_character,
                                                                           QStringList() << tr("Обратно зависимая"),
                                                                           DEVICE_MENU_PROTECT_ITEM_CURRENT_MTZ3_PROP_BACK);
    QTreeWidgetItem* currentItemMTZ3_charact_str_inv = new QTreeWidgetItem(currentItemMTZ3_set_character,
                                                                           QStringList() << tr("Сильно инверсная"),
                                                                           DEVICE_MENU_PROTECT_ITEM_CURRENT_MTZ3_PROP_SINV);
    QTreeWidgetItem* currentItemMTZ3_charact_ext_inv = new QTreeWidgetItem(currentItemMTZ3_set_character,
                                                                           QStringList() << tr("Экстремально инверсная"),
                                                                           DEVICE_MENU_PROTECT_ITEM_CURRENT_MTZ3_PROP_EINV);

    currentItemMTZ3_set_character->addChildren(QList<QTreeWidgetItem*>() << currentItemMTZ3_charact_steep <<
                                                                            currentItemMTZ3_charact_sloping <<
                                                                            currentItemMTZ3_charact_inverse <<
                                                                            currentItemMTZ3_charact_dur_inv <<
                                                                            currentItemMTZ3_charact_back <<
                                                                            currentItemMTZ3_charact_str_inv <<
                                                                            currentItemMTZ3_charact_ext_inv);

    QTreeWidgetItem* currentItemMTZ4 = new QTreeWidgetItem(protectItemCurrent, QStringList() << tr("МТЗ4"),
                                                           DEVICE_MENU_PROTECT_ITEM_CURRENT_MTZ4); // защита МТЗ4

    protectItemCurrent->addChildren(QList<QTreeWidgetItem*>() << currentItemMTZ1 << currentItemMTZ2 << currentItemMTZ3 <<
                                                                 currentItemMTZ4);

    // пункты защиты "по напряжению"
    QTreeWidgetItem* powerItemUmax1 = new QTreeWidgetItem(protectItemPower, QStringList() << tr("Umax1"),
                                                          DEVICE_MENU_PROTECT_ITEM_POWER_UMAX1); // защита Umax1
    QTreeWidgetItem* powerItemUmax2 = new QTreeWidgetItem(protectItemPower, QStringList() << tr("Umax2"),
                                                          DEVICE_MENU_PROTECT_ITEM_POWER_UMAX2); // защита Umax2
    QTreeWidgetItem* powerItemUmin1 = new QTreeWidgetItem(protectItemPower, QStringList() << tr("Umin1"),
                                                          DEVICE_MENU_PROTECT_ITEM_POWER_UMIN1); // защита Umin1
    QTreeWidgetItem* powerItemUmin2 = new QTreeWidgetItem(protectItemPower, QStringList() << tr("Umin2"),
                                                          DEVICE_MENU_PROTECT_ITEM_POWER_UMIN2); // защита Umin2
    QTreeWidgetItem* powerItem3UO   = new QTreeWidgetItem(protectItemPower, QStringList() << tr("3UO"),
                                                          DEVICE_MENU_PROTECT_ITEM_POWER_3UO); // защита 3UO

    protectItemPower->addChildren(QList<QTreeWidgetItem*>() << powerItemUmax1 << powerItemUmax2 << powerItemUmin1 <<
                                                               powerItemUmin2 << powerItem3UO);

    // пункты защиты "направленные"
    QTreeWidgetItem* directItemOZZ1 = new QTreeWidgetItem(protectItemDirected, QStringList() << tr("ОЗЗ1"),
                                                          DEVICE_MENU_PROTECT_ITEM_DIRECTED_OZZ1); // защита ОЗЗ1
    QTreeWidgetItem* directItemOZZ2 = new QTreeWidgetItem(protectItemDirected, QStringList() << tr("ОЗЗ2"),
                                                          DEVICE_MENU_PROTECT_ITEM_DIRECTED_OZZ2); // защита ОЗЗ2
    QTreeWidgetItem* directItemNZZ1 = new QTreeWidgetItem(protectItemDirected, QStringList() << tr("НЗЗ1"),
                                                          DEVICE_MENU_PROTECT_ITEM_DIRECTED_NZZ1); // защита НЗЗ1
    QTreeWidgetItem* directItemNZZ2 = new QTreeWidgetItem(protectItemDirected, QStringList() << tr("НЗЗ2"),
                                                          DEVICE_MENU_PROTECT_ITEM_DIRECTED_NZZ2); // защита НЗЗ2

    protectItemDirected->addChildren(QList<QTreeWidgetItem*>() << directItemOZZ1 << directItemOZZ2 << directItemNZZ1 <<
                                                                  directItemNZZ2);

    // пункты защиты "по частоте"
    QTreeWidgetItem* freqItemACHR1 = new QTreeWidgetItem(protectItemFrequency, QStringList() << tr("АЧР1"),
                                                         DEVICE_MENU_PROTECT_ITEM_FREQUENCY_ACHR1); // защита АЧР1
    QTreeWidgetItem* freqItemACHR2 = new QTreeWidgetItem(protectItemFrequency, QStringList() << tr("АЧР2"),
                                                         DEVICE_MENU_PROTECT_ITEM_FREQUENCY_ACHR2); // защита АЧР2
    QTreeWidgetItem* freqItemACHR3 = new QTreeWidgetItem(protectItemFrequency, QStringList() << tr("АЧР3"),
                                                         DEVICE_MENU_PROTECT_ITEM_FREQUENCY_ACHR3); // защита АЧР3

    protectItemFrequency->addChildren(QList<QTreeWidgetItem*>() << freqItemACHR1 << freqItemACHR2 << freqItemACHR3);

    // пункты защиты "внешние"
    QTreeWidgetItem* extItemARC       = new QTreeWidgetItem(protectItemExternal, QStringList() << tr("Дуговая"),
                                                            DEVICE_MENU_PROTECT_ITEM_EXTERNAL_ARC); // защита Дуговая
    QTreeWidgetItem* extItemExternal1 = new QTreeWidgetItem(protectItemExternal, QStringList() << tr("Внешняя1"),
                                                            DEVICE_MENU_PROTECT_ITEM_EXTERNAL_EXT1); // защита Внешняя1
    QTreeWidgetItem* extItemExternal2 = new QTreeWidgetItem(protectItemExternal, QStringList() << tr("Внешняя2"),
                                                            DEVICE_MENU_PROTECT_ITEM_EXTERNAL_EXT2); // защита Внешняя2
    QTreeWidgetItem* extItemExternal3 = new QTreeWidgetItem(protectItemExternal, QStringList() << tr("Внешняя3"),
                                                            DEVICE_MENU_PROTECT_ITEM_EXTERNAL_EXT3); // защита Внешняя3

    protectItemExternal->addChildren(QList<QTreeWidgetItem*>() << extItemARC << extItemExternal1 << extItemExternal2 <<
                                                                  extItemExternal3);

    // пункты защиты "для двигателя"
    QTreeWidgetItem* motorItemStarting = new QTreeWidgetItem(protectItemMotor, QStringList() << tr("Пусковая"),
                                                             DEVICE_MENU_PROTECT_ITEM_MOTOR_STARTING); // защита Пусковая
    QTreeWidgetItem* motorItemImin     = new QTreeWidgetItem(protectItemMotor, QStringList() << tr("Imin"),
                                                             DEVICE_MENU_PROTECT_ITEM_MOTOR_IMIN); // защита Imin

    protectItemMotor->addChildren(QList<QTreeWidgetItem*>() << motorItemStarting << motorItemImin);

    // пункты защиты "по температуре"
    QTreeWidgetItem* tempItemTemperature1 = new QTreeWidgetItem(protectItemTemperature, QStringList() << tr("Температурная1"),
                                                                DEVICE_MENU_PROTECT_ITEM_TEMPERATURE_TEMP1); // защита Температурная1
    QTreeWidgetItem* tempItemTemperature2 = new QTreeWidgetItem(protectItemTemperature, QStringList() << tr("Температурная2"),
                                                                DEVICE_MENU_PROTECT_ITEM_TEMPERATURE_TEMP2); // защита Температурная2

    protectItemTemperature->addChildren(QList<QTreeWidgetItem*>() << tempItemTemperature1 << tempItemTemperature2);

    // пункты защиты "резервные"
    QTreeWidgetItem* reserveItemLevel1      = new QTreeWidgetItem(protectItemReserve, QStringList() << tr("Уров1"),
                                                                  DEVICE_MENU_PROTECT_ITEM_RESERVE_LEVEL1); // защита Уров1
    QTreeWidgetItem* reserveItemLevel2      = new QTreeWidgetItem(protectItemReserve, QStringList() << tr("Уров2"),
                                                                  DEVICE_MENU_PROTECT_ITEM_RESERVE_LEVEL2); // защита Уров2
    QTreeWidgetItem* reserveItemSignalStart = new QTreeWidgetItem(protectItemReserve, QStringList() << tr("Сигнал пуска"),
                                                                  DEVICE_MENU_PROTECT_ITEM_RESERVE_SIG_START); // защита Сигнал пуска

    protectItemReserve->addChildren(QList<QTreeWidgetItem*>() << reserveItemLevel1 << reserveItemLevel2 << reserveItemSignalStart);

    // пункты защиты "предварительного контроля"
    QTreeWidgetItem* ctrlItemBRU    = new QTreeWidgetItem(protectItemControl, QStringList() << tr("БРУ"),
                                                          DEVICE_MENU_PROTECT_ITEM_CONTROL_BRU); // защита БРУ
    QTreeWidgetItem* ctrlItemVacuum = new QTreeWidgetItem(protectItemControl, QStringList() << tr("Вакууум"),
                                                          DEVICE_MENU_PROTECT_ITEM_CONTROL_VACUUM); // защита Вакуум

    protectItemControl->addChildren(QList<QTreeWidgetItem*>() << ctrlItemBRU << ctrlItemVacuum);

    // АВТОМАТИКА
    QTreeWidgetItem* automationSwitch        = new QTreeWidgetItem(itemAutomation, QStringList() << tr("Выключатель"),
                                                                   DEVICE_MENU_ITEM_AUTOMATION_SWITCH); // автоматика Выключатель
    QTreeWidgetItem* automationSwitchTruck   = new QTreeWidgetItem(itemAutomation, QStringList() << tr("Тележка выключателя"),
                                                                   DEVICE_MENU_ITEM_AUTOMATION_SWITCH_TRUCK); // автоматика Выключатель
    QTreeWidgetItem* automationBlocks        = new QTreeWidgetItem(itemAutomation, QStringList() << tr("Блокировки"),
                                                                   DEVICE_MENU_ITEM_AUTOMATION_BLOCKS); // автоматика Выключатель
    QTreeWidgetItem* automationDisconnectors = new QTreeWidgetItem(itemAutomation, QStringList() << tr("Разъединители"),
                                                                   DEVICE_MENU_ITEM_AUTOMATION_DISCONNECTORS); // автоматика Выключатель

    QTreeWidgetItem* automationDisconnectorsBus   = new QTreeWidgetItem(automationDisconnectors,
                                                                        QStringList() << tr("Шинный разъединитель"),
                                                                        DEVICE_MENU_ITEM_AUTOMATION_DISCONNECTORS_BUS);
    QTreeWidgetItem* automationDisconnectorsLine  = new QTreeWidgetItem(automationDisconnectors,
                                                                        QStringList() << tr("Линейный разъединитель"),
                                                                        DEVICE_MENU_ITEM_AUTOMATION_DISCONNECTORS_LINE);
    QTreeWidgetItem* automationDisconnectorsEarth = new QTreeWidgetItem(automationDisconnectors,
                                                                        QStringList() << tr("Заземляющий разъединитель"),
                                                                        DEVICE_MENU_ITEM_AUTOMATION_DISCONNECTORS_EARTH);

    protectItemControl->addChildren(QList<QTreeWidgetItem*>() << automationDisconnectorsBus << automationDisconnectorsLine <<
                                                                 automationDisconnectorsEarth);

    QTreeWidgetItem* automationCtrlTN = new QTreeWidgetItem(itemAutomation, QStringList() << tr("Контроль ТН"),
                                                                   DEVICE_MENU_ITEM_AUTOMATION_CTRL_TN); // автоматика Выключатель
    QTreeWidgetItem* automationAVR    = new QTreeWidgetItem(itemAutomation, QStringList() << tr("АВР"),
                                                                   DEVICE_MENU_ITEM_AUTOMATION_AVR); // автоматика Выключатель
    QTreeWidgetItem* automationAPV    = new QTreeWidgetItem(itemAutomation, QStringList() << tr("АПВ"),
                                                                   DEVICE_MENU_ITEM_AUTOMATION_APV); // автоматика Выключатель

    QTreeWidgetItem* automationAPVSignalStart = new QTreeWidgetItem(automationAPV, QStringList() << tr("АПВ сигналы пуска"),
                                                                    DEVICE_MENU_ITEM_AUTOMATION_APV_SIGNAL_START);

    automationAPV->addChild(automationAPVSignalStart);

    itemAutomation->addChildren(QList<QTreeWidgetItem*>() << automationSwitch << automationSwitchTruck << automationBlocks <<
                                                             automationDisconnectors << automationCtrlTN << automationAVR <<
                                                             automationAPV);

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
    QTreeWidgetItem* inputAnalogGeneral     = new QTreeWidgetItem(settingInputAnalog, QStringList() << tr("Основные"),
                                                                  DEVICE_MENU_ITEM_SETTINGS_ITEM_IN_ANALOG_GENERAL);
    QTreeWidgetItem* inputAnalogCalibration = new QTreeWidgetItem(settingInputAnalog, QStringList() << tr("Калибровка"),
                                                                  DEVICE_MENU_ITEM_SETTINGS_ITEM_IN_ANALOG_CALIB);

    settingInputAnalog->addChildren(QList<QTreeWidgetItem*>() << inputAnalogGeneral << inputAnalogCalibration);

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
    m_menu_items[DEVICE_MENU_ITEM_SETTINGS_ITEM_IN_ANALOG_GENERAL] = 0;
    m_menu_items[DEVICE_MENU_ITEM_SETTINGS_ITEM_IN_ANALOG_CALIB]   = 1;
    m_menu_items[DEVICE_MENU_PROTECT_ITEM_CURRENT_MTZ1]            = 2;
    m_menu_items[DEVICE_MENU_PROTECT_ITEM_CURRENT_MTZ2]            = 3;
    m_menu_items[DEVICE_MENU_PROTECT_ITEM_CURRENT_MTZ3]            = 4;
    m_menu_items[DEVICE_MENU_PROTECT_ITEM_CURRENT_MTZ3_PROP_STEEP] = 5;
    m_menu_items[DEVICE_MENU_PROTECT_ITEM_CURRENT_MTZ3_PROP_SLOP]  = 6;
    m_menu_items[DEVICE_MENU_PROTECT_ITEM_CURRENT_MTZ3_PROP_INV]   = 7;
    m_menu_items[DEVICE_MENU_PROTECT_ITEM_CURRENT_MTZ3_PROP_DINV]  = 8;
    m_menu_items[DEVICE_MENU_PROTECT_ITEM_CURRENT_MTZ3_PROP_BACK]  = 9;
    m_menu_items[DEVICE_MENU_PROTECT_ITEM_CURRENT_MTZ3_PROP_SINV]  = 10;
    m_menu_items[DEVICE_MENU_PROTECT_ITEM_CURRENT_MTZ3_PROP_EINV]  = 11;
    m_menu_items[DEVICE_MENU_PROTECT_ITEM_CURRENT_MTZ4]            = 12;
    m_menu_items[DEVICE_MENU_PROTECT_ITEM_DIRECTED_OZZ1]           = 13;
    m_menu_items[DEVICE_MENU_PROTECT_ITEM_DIRECTED_OZZ2]           = 14;
    m_menu_items[DEVICE_MENU_PROTECT_ITEM_DIRECTED_NZZ1]           = 15;
    m_menu_items[DEVICE_MENU_PROTECT_ITEM_DIRECTED_NZZ2]           = 16;
    m_menu_items[DEVICE_MENU_PROTECT_ITEM_POWER_UMAX1]             = 17;
    m_menu_items[DEVICE_MENU_PROTECT_ITEM_POWER_UMAX2]             = 18;
    m_menu_items[DEVICE_MENU_PROTECT_ITEM_POWER_UMIN1]             = 19;
    m_menu_items[DEVICE_MENU_PROTECT_ITEM_POWER_UMIN2]             = 20;
    m_menu_items[DEVICE_MENU_PROTECT_ITEM_POWER_3UO]               = 21;
    m_menu_items[DEVICE_MENU_PROTECT_ITEM_MOTOR_STARTING]          = 22;
    m_menu_items[DEVICE_MENU_PROTECT_ITEM_MOTOR_IMIN]              = 23;
    m_menu_items[DEVICE_MENU_PROTECT_ITEM_FREQUENCY_ACHR1]         = 24;
    m_menu_items[DEVICE_MENU_PROTECT_ITEM_FREQUENCY_ACHR2]         = 25;
    m_menu_items[DEVICE_MENU_PROTECT_ITEM_FREQUENCY_ACHR3]         = 26;
    m_menu_items[DEVICE_MENU_PROTECT_ITEM_EXTERNAL_ARC]            = 27;
    m_menu_items[DEVICE_MENU_PROTECT_ITEM_EXTERNAL_EXT1]           = 28;
    m_menu_items[DEVICE_MENU_PROTECT_ITEM_EXTERNAL_EXT2]           = 29;
    m_menu_items[DEVICE_MENU_PROTECT_ITEM_EXTERNAL_EXT3]           = 30;
    m_menu_items[DEVICE_MENU_PROTECT_ITEM_TEMPERATURE_TEMP1]       = 31;
    m_menu_items[DEVICE_MENU_PROTECT_ITEM_TEMPERATURE_TEMP2]       = 32;
    m_menu_items[DEVICE_MENU_PROTECT_ITEM_RESERVE_LEVEL1]          = 33;
    m_menu_items[DEVICE_MENU_PROTECT_ITEM_RESERVE_LEVEL2]          = 34;
    m_menu_items[DEVICE_MENU_PROTECT_ITEM_RESERVE_SIG_START]       = 35;
    m_menu_items[DEVICE_MENU_PROTECT_ITEM_CONTROL_BRU]             = 36;
    m_menu_items[DEVICE_MENU_PROTECT_ITEM_CONTROL_VACUUM]          = 37;
    m_menu_items[DEVICE_MENU_ITEM_AUTOMATION_SWITCH]               = 38;
    m_menu_items[DEVICE_MENU_ITEM_AUTOMATION_SWITCH_TRUCK]         = 39;
    m_menu_items[DEVICE_MENU_ITEM_AUTOMATION_BLOCKS]               = 40;
    m_menu_items[DEVICE_MENU_ITEM_AUTOMATION_DISCONNECTORS_BUS]    = 41;
    m_menu_items[DEVICE_MENU_ITEM_AUTOMATION_DISCONNECTORS_LINE]   = 42;
    m_menu_items[DEVICE_MENU_ITEM_AUTOMATION_DISCONNECTORS_EARTH]  = 43;
    m_menu_items[DEVICE_MENU_ITEM_AUTOMATION_CTRL_TN]              = 44;
    m_menu_items[DEVICE_MENU_ITEM_AUTOMATION_AVR]                  = 45;
    m_menu_items[DEVICE_MENU_ITEM_AUTOMATION_APV]                  = 46;
    m_menu_items[DEVICE_MENU_ITEM_AUTOMATION_APV_SIGNAL_START]     = 47;
    m_menu_items[DEVICE_MENU_ITEM_JOURNALS_CRASHES]                = 48;
    m_menu_items[DEVICE_MENU_ITEM_JOURNALS_EVENTS]                 = 49;
    m_menu_items[DEVICE_MENU_ITEM_JOURNALS_HALF_HOURS]             = 50;
    m_menu_items[DEVICE_MENU_ITEM_JOURNALS_ISOLATION]              = 51;
    m_menu_items[DEVICE_MENU_ITEM_MEASURES_INPUTS]                 = 52;
    m_menu_items[DEVICE_MENU_ITEM_SETTINGS_ITEM_COMMUNICATIONS]    = 53;
    m_menu_items[DEVICE_MENU_ITEM_SETTINGS_ITEM_DATETIME]          = 54;
    m_menu_items[DEVICE_MENU_ITEM_SETTINGS_ITEM_KEYBOARD]          = 55;
    m_menu_items[DEVICE_MENU_ITEM_SETTINGS_ITEM_LEDS]              = 56;
    m_menu_items[DEVICE_MENU_ITEM_SETTINGS_ITEM_IO_MDVV01_INPUTS]  = 57;
//    m_menu_items[DEVICE_MENU_ITEM_SETTINGS_ITEM_IO_MDVV02_INPUTS]  = 57;
    m_menu_items[DEVICE_MENU_ITEM_SETTINGS_ITEM_IO_MDVV01_RELAY]   = 58;
//    m_menu_items[DEVICE_MENU_ITEM_SETTINGS_ITEM_IO_MDVV02_RELAY]   = 58;
    m_menu_items[DEVICE_MENU_ITEM_SETTINGS_ITEM_IO_PROTECTION]     = 60;
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
        QVector<QPair<QString, QString> > labels = loadLabelRows("LED");
        initTable(ui->tablewgtLedPurpose, labels, group);
    }

    group = createVariableGroup("DO");

    if(!group.isEmpty())
    {
        QVector<QPair<QString, QString> > labels = loadLabelRows("RELAY");
        initTable(ui->tablewgtRelayPurpose, labels, group);
    }

    group = createVariableGroup("DI");

    if(!group.isEmpty())
    {
        QVector<QPair<QString, QString> > labels = loadLabelRows("INPUT");
        initTable(ui->tablewgtDiscreteInputPurpose, labels, group);
    }

    QVector<QPair<QString, int> > list = loadProtectionList();

    if(!list.isEmpty())
    {
        initTableProtection(ui->tablewgtProtectionCtrl, list);
    }
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
        qDebug() << "Ошибка чтения типов журнала событий: " << query.lastError().text();
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
            qDebug() << "Ошибка чтения категорий журнала событий: " << query.lastError().text();
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

    if(m_calculateWidget)
        m_calculateWidget->setVariableNames(calc_value_list); // добавляем в виджет имена переменных

    CVariableWidget* variableWidget = qobject_cast<CVariableWidget*>(ui->frameDockPanel->widget());

    if(variableWidget)
    {
        variableWidget->setVariableNames(calc_value_list);
    }

    protection_t protection = { list_item, list_set, variable_list, out_list, input_list, calc_value_list };

    ui->widgetJournalCrash->setJournalDescription(QVariant::fromValue(protection));
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
    QStringList eventJournalHeaders = QStringList() << tr("ID") << tr("Дата") << tr("Время") << tr("Тип") << tr("Категория") <<
                                                       tr("Параметр");

    QStringList crashJournalHeaders = QStringList() << tr("ID") << tr("Дата") << tr("Время") << tr("Защита");

    ui->widgetJournalCrash->setProperty("NAME", tr("аварий"));
    ui->widgetJournalEvent->setProperty("NAME", tr("событий"));
    ui->widgetJournalHalfHour->setProperty("NAME", tr("получасовок"));
    ui->widgetJournalIsolation->setProperty("NAME", tr("изоляций"));

    ui->widgetJournalCrash->setProperty("TYPE", tr("CRASH"));
    ui->widgetJournalEvent->setProperty("TYPE", tr("EVENT"));
    ui->widgetJournalHalfHour->setProperty("TYPE", tr("HALFHOUR"));
    ui->widgetJournalIsolation->setProperty("TYPE", tr("ISOLATION"));

    ui->widgetJournalCrash->setTableHeaders(crashJournalHeaders);
    ui->widgetJournalEvent->setTableHeaders(eventJournalHeaders);
    ui->widgetJournalHalfHour->setTableHeaders(eventJournalHeaders);
    ui->widgetJournalIsolation->setTableHeaders(eventJournalHeaders);

    QVector<int> length_list = QVector<int>() << 50 << 100 << 100 << 100 << 200 << 300;

    ui->widgetJournalCrash->setTableColumnWidth(QVector<int>() << 75 << 100 << 100 << 175);
    ui->widgetJournalEvent->setTableColumnWidth(length_list);
    ui->widgetJournalHalfHour->setTableColumnWidth(length_list);
    ui->widgetJournalIsolation->setTableColumnWidth(length_list);

    ui->widgetJournalCrash->setVisibleProperty(true);

    m_journal_set["CRASH"] = journal_set_t({ 0, 0, false, false, journal_address_t({ 0x26, 0x3011, 0x2000 }),
                                                                 journal_message_t({ 1, 0, 0, 0, 0, 0, 256 }), QVector<quint16>()});
    m_journal_set["EVENT"] = journal_set_t({ 0, 0, false, false, journal_address_t({ 0x22, 0x300C, 0x1000 }),
                                             journal_message_t({ 8, 0, 0, 0, 0, 0, 16 }), QVector<quint16>()});
}
/*!
 * \brief ConfiguratorWindow::initLineEditValidator
 * Применение валидатора для полей ввода CLineEdit
 */
void ConfiguratorWindow::initLineEditValidator()
{
    QObjectList root_obj_list = ui->stwgtMain->children();

    for(QObject* obj: root_obj_list)
    {
        if(obj->isWidgetType() && obj->metaObject()->className() == QString("QWidget"))
        {
            QWidget* root_wgt = qobject_cast<QWidget*>(obj);

            QObjectList root_wgt_list = root_wgt->children();

            for(QObject* child_obj: root_wgt_list)
            {
                if(child_obj->isWidgetType())
                {
                    if(child_obj->metaObject()->className() == QString("CLineEdit"))
                    {
                        setLineEditValidator(child_obj);
                    }
                    else if(child_obj->metaObject()->className() == QString("QGroupBox"))
                    {
                        QGroupBox* root_groupbox = qobject_cast<QGroupBox*>(child_obj);

                        QObjectList root_groupbox_list = root_groupbox->children();

                        for(QObject* child_groupbox_obj: root_groupbox_list)
                        {
                            if(child_groupbox_obj->isWidgetType())
                            {
                                if(child_groupbox_obj->metaObject()->className() == QString("CLineEdit"))
                                {
                                    setLineEditValidator(child_groupbox_obj);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
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
//------------------------------------------------------------------------------------------------------------------
void ConfiguratorWindow::initTable(QTableView* table, QVector<QPair<QString, QString> >& row_labels, group_t& group)
{
    HierarchicalHeaderView* hheader = new HierarchicalHeaderView(Qt::Horizontal, table);
    HierarchicalHeaderView* vheader = new HierarchicalHeaderView(Qt::Vertical, table);
    CMatrixPurposeModel*    model   = new CMatrixPurposeModel(row_labels, group);

    table->setItemDelegate(new CTableItemDelegate(CTableItemDelegate::PURPOSE_TYPE));
    table->setHorizontalHeader(hheader);
    table->setVerticalHeader(vheader);
    table->setModel(model);
    table->resizeColumnsToContents();
    table->resizeRowsToContents();
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    table->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);

    table->setColumnWidth(0, 100);

    if(table == ui->tablewgtLedPurpose || table == ui->tablewgtRelayPurpose)
    {
        table->setColumnWidth(1, 75);
        table->setColumnWidth(2, 75);
        table->setColumnWidth(50, 75);
        table->setColumnWidth(51, 75);
    }
    else if(table == ui->tablewgtDiscreteInputPurpose)
    {
        table->setColumnWidth(1, 150);
        table->setColumnWidth(26, 120);
        table->setColumnWidth(75, 100);
    }
}
//----------------------------------------------------------------------------------------------------
void ConfiguratorWindow::initTableProtection(QTableView* table, QVector<QPair<QString, int> >& labels)
{
    HierarchicalHeaderView* hheader = new HierarchicalHeaderView(Qt::Horizontal, table);
    HierarchicalHeaderView* vheader = new HierarchicalHeaderView(Qt::Vertical, table);
    CMatrixPurposeModel*    model   = new CMatrixPurposeModel(labels);

    table->setItemDelegate(new CTableItemDelegate(CTableItemDelegate::PROTECTION_TYPE));
    table->setHorizontalHeader(hheader);
    table->setVerticalHeader(vheader);
    table->setModel(model);
    table->resizeColumnsToContents();
    table->resizeRowsToContents();

    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    table->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
}
//----------------------------------------------------------------------
void ConfiguratorWindow::displayCalculateValues(QVector<quint16> values)
{
    if(values.size() == 74)
        m_calculateWidget->setData(values);

    m_calculate_buffer.clear();
}
//-----------------------------------------------------------
void ConfiguratorWindow::displayDateTime(CDataUnitType& unit)
{
    if(unit.valueCount() != 4)
        return;

    int day    = (unit.value(1) >> 8)&0x00FF;
    int month  = unit.value(0)&0x00FF;
    int year   = (unit.value(0) >> 8)&0x00FF;
    int hour   = unit.value(2)&0x00FF;
    int minute = (unit.value(3) >> 8)&0x00FF;
    int second = unit.value(3)&0x00FF;

    if(year < 2000)
        year += 2000;

    QDateTime dt(QDate(year, month, day), QTime(hour, minute, second));

    ui->dateEdit->setDate(dt.date());
    ui->timeEdit->setTime(dt.time());
    ui->lineEditWeekDay->setText(dt.date().toString("dddd"));
}
//------------------------------------------------------------------
void ConfiguratorWindow::displaySettingResponse(CDataUnitType& unit)
{
    if(unit.is_empty())
    {
        noConnectMessage();
        return;
    }

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

        if(classWgt == tr("CLineEdit"))
        {
            CLineEdit* edit = qobject_cast<CLineEdit*>(widget);

            if(edit)
            {
                quint16 val1 = unit.values().at(index + 1);
                quint16 val2 = unit.values().at(index);

                value.w[0] = val1;
                value.w[1] = val2;

                QString str = QLocale::system().toString(value.f, 'f', 6);

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
                quint16 i = unit.values().at(index++);

                if(i != 0)
                    box->setCurrentIndex(i - 1);
            }
        }
    }
}
//-------------------------------------------------------------------------------
void ConfiguratorWindow::displaySettingControlResponce(const CDataUnitType& unit)
{
    if(unit.is_empty() || unit.valueCount() > 1 || unit.valueCount() == 0)
    {
        noConnectMessage();
        return;
    }

    QString indexName = unit.property("INDEX").toString();

    QPoint index = indexSettingKey(indexName, indexName);

    if(index.x() == -1 || index.x() >= m_cell_list.count())
        return;

    QString nameWgt = m_cell_list[index.x()].second.second;

    if(nameWgt.isEmpty())
        return;

    QWidget* widget   = findChild<QWidget*>(nameWgt);
    QString  classWgt = widget->metaObject()->className();

    if(classWgt != "QComboBox")
        return;

    QComboBox* box = qobject_cast<QComboBox*>(widget);

    if(box)
    {
        quint16 i = unit.value(0);

        if(i != 0 && (i - 1) < box->count())
        {
            box->setCurrentIndex(i - 1);

            if(indexName == "M65")
                ui->cboxProtectionTemp2_Sensor1->setCurrentIndex(i - 1);
            else if(indexName == "M66")
                ui->cboxProtectionTemp2_Sensor2->setCurrentIndex(i - 1);
            else if(indexName == "M77")
                ui->cboxProtectionLeve2_Ctrl->setCurrentIndex(i - 1);
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

    if(first.isEmpty() || last.isEmpty())
        return;

    int faddr = addressPurposeKey(first);
    int eaddr = addressPurposeKey(last);

    int rowCount = (eaddr - faddr)/24 + 1; // 24 полуслова хранит до 384 переменных, т.е. при чтении одной
                                           // переменной мы получим 24 полуслова

    if(rowCount == 0)
        return;

    QTableView* table = tableMatrixFromKeys(first, last); // поиск таблицы по ключам

    if(!table)
        return;

    CMatrixPurposeModel* model = static_cast<CMatrixPurposeModel*>(table->model());

    if(!model)
        return;

    CMatrix& matrix = model->matrixTable();

    int offset = matrix.rowIndexByKey(first);

    QVector<quint16> values;

    for(int i = 0; i < unit.valueCount() - 1; i += 2) // изменение младшего со старшим
    {
        values << unit.value(i + 1) << unit.value(i);
    }

    for(int i = 0; i < rowCount; i++)
    {
        int row_index  = i + offset;
        int offset_pos = i*24;

        CRow::column_t& columns = matrix[row_index].columns();

        for(CColumn& col: columns)
        {
            int hword = col.bit()/16;
            int bit   = col.bit()%16;

            bool state = (values[hword + offset_pos]&(1 << bit));

            col.setState(state);
        }
    }

    model->updateData();
}
//--------------------------------------------------------------------
void ConfiguratorWindow::displayPurposeDIResponse(CDataUnitType& unit)
{
    if(unit.is_empty())
    {
        noConnectMessage();
        return;
    }

    QVector<QString> var_list = loadVaribleByType("DI");

    if(var_list.isEmpty())
        return;

    int first_addr = unit.property(tr("FIRST_ADDRESS")).toInt();
    int last_addr  = unit.property(tr("LAST_ADDRESS")).toInt();

    if((last_addr - first_addr + 2) != unit.valueCount())
        return;

    int column_offset = (first_addr - 512)/2;

    CMatrixPurposeModel* model  = static_cast<CMatrixPurposeModel*>(ui->tablewgtDiscreteInputPurpose->model());
    CMatrix&             matrix = model->matrixTable();

    if(matrix.rowCount() == 0 || matrix.columnCount() == 0)
        return;

    QVector<quint32> data;

    for(int i = 0; i < (unit.valueCount() - 1); i += 2) // переводим полуслова (16 бит) в слова (32 бита)
    {                                                   // каждые 32 бита хранят состояния входов для переменной
        quint32 value = ((unit.value(i) << 16) | unit.value(i + 1));
        data << value;
    }

    for(int i = 0; i < data.count(); i++)
    {
        QString key = var_list[i + column_offset].toUpper();

        int col_index = -1;

        for(int k = 0; k < matrix.columnCount(); k++) // производим поиск позиции текущей переменной в колонках, т.к. колонки
        {                                             // идут не по порядку - разбиты на группы (позиция переменной в var_list
                                                      // определяет ее положение в полученных данных учитывая смещение)
            if(matrix[0][k].key().toUpper() == key)
            {
                col_index = k;
                break;
            }
        }

        if(col_index != -1)
        {
            for(int j = 0; j < matrix.rowCount(); j++)
            {
                bool state = data[i]&(1 << j);
                matrix[j][col_index].setState(state);
            }
        }
    }

    model->updateData();
}
//--------------------------------------------------------------------------
void ConfiguratorWindow::displayEventJournalResponse(QVector<quint16>& data)
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
//--------------------------------------------------------------------------------
void ConfiguratorWindow::displayCrashJournalResponse(const QVector<quint16>& data)
{
    if(!m_journal_read_current)
        return;

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

        quint8  device_code  = quint8((value0 >> 8)&0xFF); // получаем код изделия РПА
        quint16 index_number = quint16(quint8(quint8(value0&0xFF) << 8) | quint8((value1 >> 8)&0xFF)); // получаем порядковый номер
        quint8  party_number = quint8(value1&0xFF); // получаем номер партии
        quint8  firmware_var = quint8((value2 >> 8)&0xFF); // получаем вариант прошивки
        quint8  year         = quint8(value2&0xFF); // получаем год
        quint8  month        = quint8((value3 >> 8)&0xFF); // получаем месяц
        quint8  day          = quint8(value3&0xFF); // получаем день

        QString dev_code_str = m_device_code_list[device_code];

        if(dev_code_str.isEmpty())
            dev_code_str = "0";

        QString str;
        QString date = "0";

        if(year != 0 && month != 0 && day != 0 && month <= 12 && day <= 31)
            date = QDate(year, month, day).toString("yy.MM.dd");

        str = QString("S/n: %1-%2-%3-%4-%5").arg(dev_code_str).
                                             arg(index_number).
                                             arg(party_number).
                                             arg(firmware_var).
                                             arg(date);

        m_status_bar->setSerialNumber(str);
    }
}
//-------------------------------------------------------------------------------------
void ConfiguratorWindow::displayProtectReserveSignalStart(const QVector<quint16>& data)
{
    QVector<QComboBox*> box_list = QVector<QComboBox*>() << ui->cboxN50 << ui->cboxN52 << ui->cboxN53 << ui->cboxN54 <<
                                                            ui->cboxN55 << ui->cboxN56 << ui->cboxN57 << ui->cboxN58 <<
                                                            ui->cboxN59 << ui->cboxV04 << ui->cboxV07 << ui->cboxV10 <<
                                                            ui->cboxV13 << ui->cboxV16 << ui->cboxV19 << ui->cboxV22 <<
                                                            ui->cboxV25 << ui->cboxV28 << ui->cboxV31 << ui->cboxV36 <<
                                                            ui->cboxV39 << ui->cboxV44 << ui->cboxV50 << ui->cboxV62 <<
                                                            ui->cboxV65 << ui->cboxV68 << ui->cboxV76 << ui->cboxV77 <<
                                                            ui->cboxV81 << ui->cboxV86 << ui->cboxV90 << ui->cboxV95 <<
                                                            ui->cboxV96;

    QVector<quint16> tdata;

    for(int i = 0; i < data.count() - 1; i += 2) // меняем местами старший и младший байт
    {
        tdata << data[i + 1] << data[i];
    }

    qDebug() << "read: " << tdata;

    for(QComboBox* box: box_list)
    {
        QString key = box->objectName().remove("cbox");

        if(key.isEmpty())
            continue;

        int bit     = m_variable_bits[key];
        int val_pos = bit/16;
        int bit_pos = bit%16;

        if(val_pos < tdata.count())
        {
            int item_pos = (tdata[val_pos]&(1 << bit_pos))?1:0;

            if(item_pos < box->count())
                box->setCurrentIndex(item_pos);
        }
    }
}
//------------------------------------------------------------------------------------
void ConfiguratorWindow::displayAutomationAPVSignalStart(const QVector<quint16>& data)
{
    QVector<QComboBox*> box_list = QVector<QComboBox*>() << ui->cboxN50_2 << ui->cboxN52_2 << ui->cboxN53_2 << ui->cboxN54_2 <<
                                                            ui->cboxN55_2 << ui->cboxN56_2 << ui->cboxN57_2 << ui->cboxN58_2 <<
                                                            ui->cboxN59_2 << ui->cboxV04_2 << ui->cboxV07_2 << ui->cboxV10_2 <<
                                                            ui->cboxV13_2 << ui->cboxV16_2 << ui->cboxV19_2 << ui->cboxV22_2 <<
                                                            ui->cboxV25_2 << ui->cboxV28_2 << ui->cboxV31_2 << ui->cboxV36_2 <<
                                                            ui->cboxV39_2 << ui->cboxV44_2 << ui->cboxV50_2 << ui->cboxV62_2 <<
                                                            ui->cboxV65_2 << ui->cboxV68_2 << ui->cboxV76_2 << ui->cboxV77_2 <<
                                                            ui->cboxV81_2 << ui->cboxV86_2 << ui->cboxV90_2 << ui->cboxV95_2 <<
                                                            ui->cboxV96_2;

    QVector<quint16> tdata;

    for(int i = 0; i < data.count() - 1; i += 2) // меняем местами старший и младший байт
    {
        tdata << data[i + 1] << data[i];
    }

    for(QComboBox* box: box_list)
    {
        QString key = (box->objectName().remove("cbox")).remove("_2");

        if(key.isEmpty())
            continue;

        int bit     = m_variable_bits[key];
        int val_pos = bit/16;
        int bit_pos = bit%16;

        if(val_pos < tdata.count())
        {
            int item_pos = (tdata[val_pos]&(1 << bit_pos))?1:0;

            if(item_pos < box->count())
                box->setCurrentIndex(item_pos);
        }
    }
}
//---------------------------------------------------------------------------------------
void ConfiguratorWindow::displayCommunicationTimeoutRequest(const QVector<quint16>& data)
{
    if(data.count() > 0)
    {
        ui->spinBoxCommunicationRequestTimeout->setValue(data[0]);
    }
}
//-------------------------------------------------------------------------------------
void ConfiguratorWindow::displayCommunicationTimeoutSpeed(const QVector<quint16>& data)
{
    if(data.count() > 0)
    {
        ui->spinBoxCommunicationTimeoutSpeed->setValue(data[0]);
    }
}
//--------------------------------------------------------------------------------
void ConfiguratorWindow::displayCommunicationAddress(const QVector<quint16>& data)
{
    if(data.count() > 0)
    {
        ui->spinBoxCommunicationAddress->setValue(data[0]);
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

    m_versionWidget->setText(ver);

    QString title = this->windowTitle();

    if(!ver.isEmpty())
        title += tr(" ") + ver.last().first;

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
//--------------------------------------------------------------------------
void ConfiguratorWindow::sendSettingControlReadRequest(const QString& index)
{
    int addr = addressSettingKey(index);

    CDataUnitType unit(ui->sboxSlaveID->value(), CDataUnitType::ReadHoldingRegisters, addr, QVector<quint16>() << 1);

    unit.setProperty("REQUEST", GENERAL_CONTROL_TYPE);
    unit.setProperty("INDEX", index);

    m_modbusDevice->request(unit);
}
//---------------------------------------------------------------------------
void ConfiguratorWindow::sendSettingControlWriteRequest(const QString& index)
{
    if(index.isEmpty())
        return;

    QPoint key = indexSettingKey(index, index);

    if(key.x() == -1 || key.x() >= m_cell_list.count())
        return;

    QString nameWgt = m_cell_list[key.x()].second.second;

    if(nameWgt.isEmpty())
        return;

    QWidget* widget   = findChild<QWidget*>(nameWgt);
    QString  classWgt = widget->metaObject()->className();

    if(classWgt != "QComboBox")
        return;

    QComboBox* box = qobject_cast<QComboBox*>(widget);

    if(box)
    {
        int addr = addressSettingKey(index);

        if(addr == -1)
            return;

        CDataUnitType unit(ui->sboxSlaveID->value(), CDataUnitType::WriteSingleRegister, addr,
                                                     QVector<quint16>() << (box->currentIndex() + 1));
        unit.setProperty("REQUEST", GENERAL_CONTROL_TYPE);
        unit.setProperty("INDEX", index);

        m_modbusDevice->request(unit);
    }
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

        if(classWgt == tr("CLineEdit"))
        {
            CLineEdit* edit = qobject_cast<CLineEdit*>(widget);

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

            data.append((quint16)box->currentIndex() + 1);
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

    CMatrix matrix = static_cast<CMatrixPurposeModel*>(table->model())->matrixTable();

    int bIndex = matrix.rowIndexByKey(first);
    int eIndex = matrix.rowIndexByKey(last);

    if(bIndex == -1 || eIndex == -1)
        return;

    int hword_size = (eIndex - bIndex + 1)*24;

    QVector<quint16> data = QVector<quint16>(hword_size, 0); // создаем вектор размерностью hword_size полуслов со значением 0

    for(int i = 0; i <= (eIndex - bIndex); i++)
    {
        int index      = i + bIndex;
        int offset_pos = i*24;

        for(CColumn& col: matrix[index].columns())
        {
            quint16 hword = col.bit()/16;
            quint16 bit   = col.bit()%16;

            if(col.state())
            {
                data[hword + offset_pos] |= (1 << bit);
            }
        }
    }

    QVector<quint16> values;

    for(int i = 0; i < (data.count() - 1); i += 2)
        values << data[i + 1] << data[i];

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

    CMatrix matrix = model->matrixTable();

    if(matrix.rowCount() == 0 || matrix.columnCount() == 0)
        return;

    QVector<QString> var_list = loadVaribleByType("DI");

    int bIndex = (first_addr - 512)/2;
    int eIndex = (670 - last_addr)/2;

    QVector<quint16> values;

    for(int i = bIndex; i < var_list.count() - eIndex; i++)
    {
        QString key       = var_list[i].toUpper();
        int     col_index = -1;

        for(int j = 0; j < matrix.columnCount(); j++)
        {
            QString col_key = matrix[0][j].key().toUpper();

            if(key == col_key)
            {
                col_index = j;
                break;
            }
        }

        if(col_index != -1)
        {
            quint32 value = 0;

            for(int k = 0; k < matrix.rowCount(); k++)
            {
                bool state = matrix[k][col_index].state();

                if(state)
                    value |= (1 << k);
            }

            values << quint16((value&0xFFFF0000) >> 16) << quint16(value&0x0000FFFF);
        }
    }

    CDataUnitType::FunctionType funType = ((values.count() == 1)?CDataUnitType::WriteSingleRegister:
                                                                 CDataUnitType::WriteMultipleRegisters);

    CDataUnitType unit(ui->sboxSlaveID->value(), funType, first_addr, values);

    unit.setProperty(tr("FIRST_ADDRESS"), first_addr);
    unit.setProperty(tr("LAST_ADDRESS"), last_addr);

    m_modbusDevice->request(unit);
}
/*!
 * \brief ConfiguratorWindow::sendRequestRead
 * \param addr Адрес ячейки
 * \param size Количество ячеек для чтения
 */
void ConfiguratorWindow::sendRequestRead(int addr, int size, int request)
{
    CDataUnitType unit(ui->sboxSlaveID->value(), CDataUnitType::ReadHoldingRegisters, addr, QVector<quint16>() << size);

    unit.setProperty(tr("REQUEST"), request);

    m_modbusDevice->request(unit);
}
/*!
 * \brief ConfiguratorWindow::sendRequestWrite
 * \param addr Адрес ячейки
 * \param data Данные для записи
 */
void ConfiguratorWindow::sendRequestWrite(int addr, QVector<quint16>& values, int request)
{
    CDataUnitType::FunctionType funType = ((values.count() == 1)?CDataUnitType::WriteSingleRegister:
                                                                 CDataUnitType::WriteMultipleRegisters);

    CDataUnitType unit(ui->sboxSlaveID->value(), funType, addr, values);

    unit.setProperty("REQUST", request);

    m_modbusDevice->request(unit);
}
/*!
 * \brief ConfiguratorWindow::sendDeviceCommand
 *
 * Отправка команды устройству
 */
void ConfiguratorWindow::sendDeviceCommand(int cmd)
{
    CDataUnitType unit(ui->sboxSlaveID->value(), CDataUnitType::WriteSingleRegister, 0x3000, QVector<quint16>() << cmd);

    unit.setProperty("CMD", cmd);

    m_modbusDevice->request(unit);
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

        default: return;
    }

    if(!table)
        return;

    CMatrixPurposeModel* model = static_cast<CMatrixPurposeModel*>(table->model());

    if(!model)
        return;

    CMatrix& matrix = model->matrixTable();

    for(int i = 0; i < matrix.rowCount(); i++)
    {
        for(int j = 0; j < matrix.columnCount(); j++)
        {
            matrix[i][j].setState(false);
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
        m_filter[key] = filterDlg->filter();
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
    m_modbusDevice->setAutospeed(state);
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
            m_serialPortSettings->setAutospeed(m_settings->value("autospeed", false).toBool());
        m_settings->endGroup();

        m_settings->beginGroup("mainwindow");
            restoreGeometry(m_settings->value("geometry").toByteArray());
        m_settings->endGroup();
    }

    ui->comboBoxCommunicationBaudrate->setCurrentIndex(ui->cboxBaudrate->currentIndex());
    m_modbusDevice->setAutospeed(m_serialPortSettings->autospeedState());
}
//-------------------------------------
void ConfiguratorWindow::saveSettings()
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
            m_settings->setValue("autospeed", m_serialPortSettings->autospeedState());
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
        matrix          = static_cast<CMatrixPurposeModel*>(ui->tablewgtLedPurpose->model())->matrixTable();
        typeName        = "LED";
        fileNameDefault = "led";
    }
    else if(index == DEVICE_MENU_ITEM_SETTINGS_ITEM_IO_MDVV01_INPUTS)
    {
        matrix          = static_cast<CMatrixPurposeModel*>(ui->tablewgtDiscreteInputPurpose->model())->matrixTable();
        typeName        = "INPUT";
        fileNameDefault = "input";
    }
    else if(index == DEVICE_MENU_ITEM_SETTINGS_ITEM_IO_MDVV01_RELAY)
    {
        matrix          = static_cast<CMatrixPurposeModel*>(ui->tablewgtRelayPurpose->model())->matrixTable();
        typeName        = "RELAY";
        fileNameDefault = "relay";
    }

    if(matrix.rowCount() == 0 || matrix.columnCount() == 0)
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

    json["type"] = typeName;

    for(int i = 0; i < matrix.rowCount(); i++)
    {
        QJsonObject trowCurObj;
        QJsonArray  columnArr;

        for(int j = 0; j < matrix.columnCount(); j++)
        {
            QJsonObject tcolumnObj;

            tcolumnObj["state"]       = matrix[i][j].state();
            tcolumnObj["bit"]         = matrix[i][j].bit();
            tcolumnObj["key"]         = matrix[i][j].key();
            tcolumnObj["name"]        = matrix[i][j].name();
            tcolumnObj["description"] = matrix[i][j].description();

            columnArr.append(tcolumnObj);
        }

        trowCurObj["key"]     = matrix[i].key();
        trowCurObj["name"]    = matrix[i].name();
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
    QString typeNameDescription;

    CMatrixPurposeModel* model = nullptr;
    DeviceMenuItemType   index = menuIndex();

    if(index == DEVICE_MENU_ITEM_SETTINGS_ITEM_LEDS)
    {
        fileNameDefault     = "led";
        typeName            = "LED";
        typeNameDescription = tr("Светодиоды");

        model = static_cast<CMatrixPurposeModel*>(ui->tablewgtLedPurpose->model());
    }
    else if(index == DEVICE_MENU_ITEM_SETTINGS_ITEM_IO_MDVV01_INPUTS)
    {
        fileNameDefault     = "input";
        typeName            = "INPUT";
        typeNameDescription = tr("Дискретные входы");

        model = static_cast<CMatrixPurposeModel*>(ui->tablewgtDiscreteInputPurpose->model());
    }
    else if(index == DEVICE_MENU_ITEM_SETTINGS_ITEM_IO_MDVV01_RELAY)
    {
        fileNameDefault     = "relay";
        typeName            = "RELAY";
        typeNameDescription = tr("Реле");

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
    {
        QMessageBox::warning(this, tr("Импорт профиля привязок"), tr("Неправильный тип привязок. Ожидаются: %1").
                                                                  arg(typeNameDescription));
        return;
    }

    QJsonArray dataArr = rootObj.value("data").toArray(); // получение массива данных - строки

    if(dataArr.isEmpty())
        return;

    CMatrix::row_t rows;
    int            columnCount = 0;

    for(int i = 0; i < dataArr.count(); i++)
    {
        QJsonObject rowObj = dataArr[i].toObject(); // получаем объект строки из массива

        if(rowObj.isEmpty())
            continue;

        QJsonArray colArr = rowObj["columns"].toArray(); // получение массива данных - колонки

        if(colArr.isEmpty())
            continue;

        CRow::column_t columns;

        for(int j = 0; j < colArr.count(); j++)
        {
            QJsonObject colObj = colArr[j].toObject(); // получаем колонку из массива

            columns << CColumn(colObj["bit"].toInt(), colObj["state"].toBool(), colObj["key"].toString(),
                                  colObj["name"].toString(), colObj["description"].toString());
        }

        rows << CRow(rowObj["key"].toString(), rowObj["name"].toString(), columns);

        columnCount = columns.count();
    }

    CMatrix matrix(rows, columnCount);

    if(model)
        model->setMatrixTable(matrix);
}
//--------------------------------------------------------------
void ConfiguratorWindow::processReadJournal(CDataUnitType& unit)
{
    RequestType type = (RequestType)unit.property(tr("REQUEST")).toInt();

    QString key = unit.property(tr("JOURNAL")).toString();

    if(key.isEmpty() || m_journal_set.find(key) == m_journal_set.end())
        return;

    journal_set_t& set = m_journal_set[key];

    switch(type)
    {
        case READ_JOURNAL_SHIFT_PTR:
        break;

        case READ_JOURNAL_COUNT:
            if(unit.valueCount() == 2)
            {
                set.message.read_total = long(long((unit.value(0) << 16)&0xFFFF) | long(unit.value(1)&0xFFFF));
            }
        break;

        case READ_JOURNAL:
        {
            QVector<quint16> data = unit.values();

            if(unit.valueCount()*2 < set.message.size) // принятые данные меньше, чем размер одного сообщения
            {
                if(set.buffer.isEmpty()) // буфер сообщений пуст
                {
                    set.buffer.append(unit.values()); // сохраняем сообщения в буфер
                    journalRead(key);

                    return;
                }
                else
                {
                    if((unit.valueCount()*2 + set.buffer.count()*2) == set.message.size) // данные приняты полностью
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

            if(key == "EVENT")
                displayEventJournalResponse(data);
            else if(key == "CRASH")
                displayCrashJournalResponse(data);

            journalRead(key);
        }
        break;

        default: break;
    }
}
//---------------------------------------------------
void ConfiguratorWindow::widgetStackIndexChanged(int)
{
    ui->tabwgtMenu->setTabEnabled(4, false);
    m_active_journal_current = nullptr;
    ui->pushButtonJournalRead->setVisible(false);
    ui->pushButtonJournalClear->setVisible(false);
    ui->pushButtonDefaultSettings->setVisible(false);

    DeviceMenuItemType index = menuIndex();

    if(index >= DEVICE_MENU_ITEM_JOURNALS_CRASHES && index <= DEVICE_MENU_ITEM_JOURNALS_ISOLATION)
    {
        ui->tabwgtMenu->setTabEnabled(4, true);

        int width = ui->stwgtMain->width() - 760;

        ui->widgetJournalCrash->table()->setFixedWidth(475);
        ui->widgetJournalEvent->setTableColumnWidth(3, width);
        ui->widgetJournalHalfHour->setTableColumnWidth(3, width);
        ui->widgetJournalIsolation->setTableColumnWidth(3, width);

        if(!currentJournal(m_active_journal_current))
            m_active_journal_current = nullptr;

        m_active_journal_current->header()->setTextDeviceCountMessages(0, 0);
        ui->pushButtonJournalRead->setVisible(true);
        ui->pushButtonJournalClear->setVisible(true);

        if(m_active_journal_current)
            readJournalCount();
    }
    else if(index == DEVICE_MENU_ITEM_SETTINGS_ITEM_LEDS || index == DEVICE_MENU_ITEM_SETTINGS_ITEM_IO_MDVV01_INPUTS ||
            index == DEVICE_MENU_ITEM_SETTINGS_ITEM_IO_MDVV01_RELAY || index == DEVICE_MENU_ITEM_SETTINGS_ITEM_KEYBOARD)
    {
        ui->pushButtonDefaultSettings->setVisible(true);
    }
}
//-----------------------------------------------------------------------
void ConfiguratorWindow::setJournalPtrShift(const QString& key, long pos)
{
    if(key.isEmpty() || m_journal_set.find(key) == m_journal_set.end())
        return;

    journal_set_t set = m_journal_set[key];

    QVector<quint16> values = QVector<quint16>() << (quint16)((pos >> 16)&0xFFFF) << (quint16)(pos&0xFFFF);

    CDataUnitType unit(ui->sboxSlaveID->value(), CDataUnitType::WriteMultipleRegisters, set.address.set_shift, values);
    unit.setProperty(tr("REQUEST"), READ_JOURNAL_SHIFT_PTR);
    unit.setProperty(tr("JOURNAL"), key);

    m_modbusDevice->request(unit);
}
//------------------------------------------------
void ConfiguratorWindow::timeoutSynchronization()
{
    CDataUnitType unit(ui->sboxSlaveID->value(), CDataUnitType::ReadInputRegisters, 0x0001, QVector<quint16>() << 4);

    unit.setProperty(tr("REQUEST"), READ_SERIAL_NUMBER);

    m_modbusDevice->request(unit);

    if(!m_timer_synchronization->isActive())
        m_timer_synchronization->start(ui->spinboxSyncTime->value());
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

        if(journal_type == "EVENT")
        {
            QString type      = query.value("type").toString();
            QString category  = query.value("category").toString();
            QString parameter = query.value("parameter").toString();

            table->setItem(row, 3, new QTableWidgetItem(type));
            table->setItem(row, 4, new QTableWidgetItem(category));
            table->setItem(row, 5, new QTableWidgetItem(parameter));

            table->item(row, 0)->setTextAlignment(Qt::AlignCenter);
            table->item(row, 1)->setTextAlignment(Qt::AlignCenter);
            table->item(row, 2)->setTextAlignment(Qt::AlignCenter);
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

        m_progressbar->progressIncrement();
    }

    db->commit();
    disconnectDb(db);

    m_progressbar->progressStop();
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
    if(m_journal_set.isEmpty())
        return;

    for(const QString key: m_journal_set.keys())
    {
        journal_set_t set = m_journal_set[key];

        CDataUnitType unit(ui->sboxSlaveID->value(), CDataUnitType::ReadHoldingRegisters, set.address.set_shift,
                                                     QVector<quint16>() << 2);

        unit.setProperty(tr("REQUEST"), READ_JOURNAL_SHIFT_PTR);
        unit.setProperty(tr("JOURNAL"), key);

        m_modbusDevice->request(unit);
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

        CDataUnitType unit(ui->sboxSlaveID->value(), CDataUnitType::ReadInputRegisters, set.address.msg_count,
                                                     QVector<quint16>() << 2);
        unit.setProperty(tr("REQUEST"), READ_JOURNAL_COUNT);
        unit.setProperty(tr("JOURNAL"), key);

        m_modbusDevice->request(unit);
    }
}
//--------------------------------------------------
void ConfiguratorWindow::synchronization(bool state)
{
    #ifdef DEBUG_REQUEST // проверка связи (отключение синхронизации)
        return;
    #endif

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

    if(str == "LEK20" || str == "LEK21" || str == "LEK29")
    {
        lineEdit->setValidator(new QIntValidator(0, 360));
        lineEdit->setText("0");
    }
    else
    {
        QDoubleValidator* validator = new QDoubleValidator(0, 100, 6);
        lineEdit->setValidator(validator);
        lineEdit->setText(QString("%1").arg(QLocale::system().toString(0.0f, 'f', 6)));
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

                    /*if(name.count() > 20)
                    {
                        int party = name.count()/20;

                        for(int i = 0; i < party; i++)
                        {
                            int pos = i*20;

                            if(name[pos] != ' ')
                            {
                                int from = name.indexOf(' ', pos - 5);
                                int to   = name.indexOf(' ', pos);

                                if(from != -1)
                                {
                                    name.insert(from, '\n');
                                }
                                else if(to != -1)
                                {
                                    name.insert(to, '\n');
                                }
                            }
                            else
                                name.insert(pos, '\n');
                        }
                    }*/

                    var_list << var_t({ key, group_id, bit, name, description });
                }
            }

            group[id] = group_item_t({ group_name, group_description, var_list });
        }
    }

    return group;
}
//--------------------------------------------------------------------------------------
QVector<QPair<QString, QString> > ConfiguratorWindow::loadLabelRows(const QString& type)
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
//--------------------------------------------------------------------
QVector<QPair<QString, int> > ConfiguratorWindow::loadProtectionList()
{
    QVector<QPair<QString, int> > list;

    QSqlQuery query(m_system_db);

    if(query.exec("SELECT code, name FROM protection;"))
    {
        while(query.next())
        {
            int     code = query.value("code").toInt();
            QString name = query.value("name").toString();

            list << qMakePair(name, code);
        }
    }

    return list;
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
    connect(ui->pushButtonJournalRead, &QPushButton::clicked, this, &ConfiguratorWindow::processReadJournals);
    connect(ui->pushButtonJournalClear, &QPushButton::clicked, this, &ConfiguratorWindow::clearJournal);
    connect(m_modbusDevice, &CModbus::connectDeviceState, ui->pushButtonJournalRead, &QPushButton::setEnabled);
    connect(this, &ConfiguratorWindow::buttonReadJournalStateChanged, ui->pushButtonJournalRead, &QPushButton::setChecked);
    connect(ui->pbtnMenuExit, &QPushButton::clicked, this, &ConfiguratorWindow::exitFromApp);
    connect(ui->pbtnMenuPanelMenuCtrl, &QPushButton::clicked, this, &ConfiguratorWindow::menuPanelCtrl);
    connect(ui->pbtnMenuPanelVariableCtrl, &QPushButton::clicked, this, &ConfiguratorWindow::variablePanelCtrl);
    connect(ui->pbtnMenuExportToPDF, &QPushButton::clicked, this, &ConfiguratorWindow::startExportToPDF);
    connect(ui->pushButtonExport, &QPushButton::clicked, this, &ConfiguratorWindow::processExport);
    connect(ui->pushButtonImport, &QPushButton::clicked, this, &ConfiguratorWindow::processImport);
    connect(ui->stwgtMain, &QStackedWidget::currentChanged, this, &ConfiguratorWindow::widgetStackIndexChanged);
    connect(m_timer_synchronization, &QTimer::timeout, this, &ConfiguratorWindow::timeoutSynchronization);
    connect(ui->pbtnFilter, &QPushButton::clicked, this, &ConfiguratorWindow::filterDialog);
    connect(ui->pushButtonDefaultSettings, &QPushButton::clicked, this, &ConfiguratorWindow::deviceDefaultSettings);
    connect(m_modbusDevice, &CModbus::connectDeviceState, ui->pushButtonDefaultSettings, &QPushButton::setEnabled);
    connect(m_modbusDevice, &CModbus::baudrateChanged, ui->cboxBaudrate, &QComboBox::setCurrentIndex);
    connect(m_modbusDevice, &CModbus::error, this, &ConfiguratorWindow::errorConnect);
    connect(m_modbusDevice, &CModbus::newBaudrate, this, &ConfiguratorWindow::setNewBaudrate);
    connect(m_modbusDevice, &CModbus::saveSettings, this, &ConfiguratorWindow::saveDeviceSettings);
    connect(ui->dateEdit, &QDateEdit::dateChanged, this, &ConfiguratorWindow::dateDeviceChanged);
    connect(m_serialPortSettings, &CSerialPortSetting::autospeed, this, &ConfiguratorWindow::autospeedStateChanged);
    connect(m_terminal, &CTerminal::sendDeviceCommand, this, &ConfiguratorWindow::sendDeviceCommand);
}
