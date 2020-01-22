#include "configuratorwindow.h"
#include "ui_configuratorwindow.h"
//------------------------------------------------------
ConfiguratorWindow::ConfiguratorWindow(QWidget* parent):
    QMainWindow(parent),
    ui(new Ui::ConfiguratorWindow),
    m_isInitWindow(false),
    m_isProject(false),
    m_modbus(nullptr),
    m_treeWidgetDeviceMenu(nullptr),
    m_variableWidget(nullptr),
    m_containerWidgetVariable(nullptr),
    m_containerWidgetDeviceMenu(nullptr),
    m_containerIndicatorState(nullptr),
    m_containerMonitorI11I17(nullptr),
    m_containerOutputAll(nullptr),
    m_containerInputs(nullptr),
    m_containerDebugInfo(nullptr),
    m_containerStatusInfo(nullptr),
    m_containerWidgetMessage(nullptr),
    m_serialPortSettings_window(nullptr),
    m_output_window(nullptr),
    m_monitor_purpose_window(nullptr),
    m_outputall_window(nullptr),
    m_inputs_window(nullptr),
    m_debuginfo_window(nullptr),
    m_debug_var_window(nullptr),
    m_event_window(nullptr),
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
    m_journal_timer(nullptr),
    m_journal_progress(nullptr),
    m_project_cur_path(""),
    m_serial_port_name(""),
    m_calibration_controller(nullptr),
    m_is_new_baudrate(false),
    m_is_set_change(false)
{
    ui->setupUi(this);
    setWindowFlag(Qt::FramelessWindowHint);

    // удаление вкладки Экран (также закоментирован индекс вкладки в хедере)
    ui->tabwgtMenu->removeTab(3); // индекс вкладки "Экран" - 3
}
//---------------------------------------
ConfiguratorWindow::~ConfiguratorWindow()
{
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
            QSerialPort::Data8,
            m_serialPortSettings_window->parity(),
            QSerialPort::OneStop
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

    QString text = ((state)?tr("Соединение с устройством установлено на скорости: %1 бод").
                    arg(m_modbus->channel()->settings().baudrate):
                    tr("Соединение с устройством разорвано"));
    m_status_bar->setStatusMessage(text, 5000);
    outLogMessage(text);
    
    if(ui->checkboxCalibTimeout->isChecked() && state)
        chboxCalculateTimeoutStateChanged(true);
    else
        m_tim_calculate->stop();

    if(state)
    {
        outLogMessage(tr("Порт <%1> открыт.").arg(m_modbus->channel()->settings().name));
        ui->toolButtonConnect->setText(tr("Отключиться"));
        ui->toolButtonConnect->setIconSize(QSize(24, 24));
        ui->toolButtonConnect->setIcon(QIcon(":/images/resource/images/disconnect_serial.png"));
        ui->toolButtonConnect->setToolTip(tr("Отключение от БЗУ"));
        m_status_bar->clearSerialNumber(); // удаляем старый серийный номер
        synchronization(true); // запускаем синхронизацию

        // установка одинаковых скоростей (скорости текущего соединения со скоростью настройки соединения - перезаписи)
        QString baudrate = QString::number(m_serialPortSettings_window->baudrate());

        if(!baudrate.isEmpty())
        {
            int index = ui->comboBoxCommunicationBaudrate->findText(baudrate);

            if(index != -1)
                ui->comboBoxCommunicationBaudrate->setCurrentIndex(index);
        }
        //...

        if(m_is_new_baudrate)
            saveDeviceSettings(); // сохранение настроек - скорости соединения

        m_is_new_baudrate = false;

        DeviceMenuItemType item = menuIndex();

        if(item == DEVICE_MENU_ITEM_SETTINGS_ITEM_IN_ANALOG ||
           item == DEVICE_MENU_ITEM_SETTINGS_ITEM_IN_ANALOG_CALIB ||
           item == DEVICE_MENU_ITEM_SETTINGS_ITEM_IN_ANALOG_GENERAL)
        {
            CCalibrationController::CalibrationType type = static_cast<CCalibrationController::CalibrationType>(ui->tabWidgetCalibration->currentIndex());
            m_calibration_controller->setCalculateState(true, type);
        }
    }
    else
    {
        outLogMessage(tr("Порт <%1> закрыт.").arg(m_modbus->channel()->settings().name));
        ui->toolButtonConnect->setText(tr("Подключиться"));
        ui->toolButtonConnect->setIconSize(QSize(24, 24));
        ui->toolButtonConnect->setIcon(QIcon(":/images/resource/images/flag.png"));
        ui->toolButtonConnect->setToolTip(tr("Подключение к БЗУ"));
        synchronization(); // отключение синхронизации
        m_status_bar->connectStateChanged(false);

        if(ui->pushButtonJournalRead->isChecked())
        {
            ui->pushButtonJournalRead->setChecked(false);
            m_progressbar->progressStop();
        }

        endJournalRead(m_journal_crash);
        endJournalRead(m_journal_event);
        endJournalRead(m_journal_halfhour);
        endJournalRead(m_journal_isolation);

        m_calibration_controller->setCalculateState(false, CCalibrationController::TYPE_NONE);

        if(m_is_new_baudrate)
        {
            QTimer::singleShot(500, [this]()
            {
                QSerialPort::BaudRate baudrate = static_cast<QSerialPort::BaudRate>(ui->comboBoxCommunicationBaudrate->currentText().toInt());

                if(baudrate != QSerialPort::UnknownBaud)
                {
                    m_serialPortSettings_window->setBaudrate(baudrate);
                    ui->toolButtonConnect->setChecked(true);
                    serialPortCtrl();
                }
            });
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
        showMessageBox(tr("Com-порт"), tr("Не удалось найти ни одного доступного\nпоследовательного порта на этом компьютере"), QMessageBox::Warning);
    }
    
    m_serialPortSettings_window->setSerialPortList(port_list);

    if(!m_serial_port_name.isEmpty())
    {
        m_serialPortSettings_window->setSerialPortCurrentName(m_serial_port_name);
    }
}
//-------------------------------------------
void ConfiguratorWindow::serialPortSettings()
{
    m_serialPortSettings_window->show();
}
//------------------------------------------------
void ConfiguratorWindow::blockProtectionCtrlRead()
{
    sendDeviceCommand(45); // установка ключа записи

    for(const block_protection_t& block: m_block_list)
    {
        qDebug() << QString("Чтение блокировок: имя->%1, адрес->%2, описание->%3").arg(block.name).arg(block.address).arg(block.description);
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

    for(int col = 0; col < matrix.columnCount(); col++)
    {
        QVector<block_protection_purpose_t> purpose_list = m_block_list[col].purpose;
        QVector<quint16> values = QVector<quint16>(24, 0);

        for(int row = 0, pos = 0; row < matrix.rowCount(); row++, pos++)
        {
            if(col == row)
            {
                pos--;
                continue;
            }

            block_protection_purpose_t purpose = purpose_list[pos];

            int r   = purpose.bit/16;
            int bit = purpose.bit%16;

            bool state = matrix[row][col].data().state;

            quint16 value = values[r];

            if(state)
                value |= (1 << bit);

            values[r] = value;
        }

        QVector<quint16> tvalues;

        for(int k = 0; k < values.count() - 1; k += 2)
            tvalues << values[k + 1] << values[k];

        sendRequestWrite(m_block_list[col].address, tvalues, -1);
    }
}
//--------------------------------------
void ConfiguratorWindow::calculateRead()
{
    CModBusDataUnit unit_part1(static_cast<quint8>(m_serialPortSettings_window->deviceID()), CModBusDataUnit::ReadInputRegisters,
                               CALCULATE_ADDRESS_PART1, QVector<quint16>() << 66);
    unit_part1.setProperty(tr("REQUEST"), CALCULATE_TYPE);
    unit_part1.setProperty("PART", CALCULATE_ADDRESS_PART1);

    sendCalculateRead(unit_part1);

    CModBusDataUnit unit_part2(static_cast<quint8>(m_serialPortSettings_window->deviceID()), CModBusDataUnit::ReadInputRegisters,
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
/*!
 * \brief ConfiguratorWindow::journalRead
 * \param journal указатель на открытый журнал
 */
void ConfiguratorWindow::journalRead(JournalPtr journal)
{
    if(!journal)
        return;

    if(journal->isReadState())
    {
        bool isShift;
        CModBusDataUnit unit = journal->read(m_serialPortSettings_window->deviceID(), READ_JOURNAL, &isShift);

        if(isShift)
            setJournalShiftPtr(journal);

        if(unit.isValid())
        {
            m_modbus->sendData(unit);
        }
    }
}
/*!
 * \brief ConfiguratorWindow::inputAnalogGeneralRead
 *
 * Чтение настроек "Настройки-аналоговые-основные"
 */
void ConfiguratorWindow::inputAnalogGeneralRead()
{
    QStringList list = QStringList() << "M01" << "M02" << "M03" << "K61" << "K62" << "K63" << "K64";
    sendSettingReadRequest(list, CModBusDataUnit::ReadHoldingRegisters, DEVICE_MENU_ITEM_SETTINGS_ITEM_IN_ANALOG);

    sendSettingControlReadRequest("K16", DEVICE_MENU_ITEM_SETTINGS_ITEM_IN_ANALOG);
    sendSettingControlReadRequest("K60", DEVICE_MENU_ITEM_SETTINGS_ITEM_IN_ANALOG);
}
/*!
 * \brief ConfiguratorWindow::inputAnalogCalibrateRead
 * Чтение настроек "Настройки-аналоговые-калибровки"
 */
void ConfiguratorWindow::inputAnalogCalibrateRead()
{
    QStringList list = QStringList() << "K3I0" << "KIA" << "KIB" << "KIC" << "KUA" << "KUB" << "KUC" << "KUAB" << "KUBC" <<
                                        "KUCA" << "K3U0R" << "K3U0S" << "K3U0T" << "KUADC" << "AUADC" << "KUBDC" << "AUBDC" <<
                                        "KUCDC" << "AUCDC" << "KUMDC" << "AUMDC" << "KRA" << "ARA" << "KRB" << "ARB" << "KRC" <<
                                        "ARC" << "KY01T" << "KY02T" << "KY03T" << "KY01R" << "KY02R" << "KY03R" << "KY04R" <<
                                        "KY04S" << "KY04T";
    sendSettingReadRequest(list, CModBusDataUnit::ReadHoldingRegisters, DEVICE_MENU_ITEM_SETTINGS_ITEM_IN_ANALOG, true);
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
    QStringList list = QStringList() << "M01" << "M02" << "M03" << "K61" << "K62" << "K63" << "K64";

    for(QString key: list)
        sendSettingWriteRequest(key, key, DEVICE_MENU_ITEM_SETTINGS_ITEM_IN_ANALOG);

    sendSettingControlWriteRequest("K16", DEVICE_MENU_ITEM_SETTINGS_ITEM_IN_ANALOG); // запись состояния настройки
    sendSettingControlWriteRequest("K60", DEVICE_MENU_ITEM_SETTINGS_ITEM_IN_ANALOG); // запись состояния настройки
}
/*!
 * \brief ConfiguratorWindow::inputAnalogCalibrateWrite
 *
 * Запись настроек калибровки
 */
void ConfiguratorWindow::inputAnalogCalibrateWrite()
{
    QStringList list = QStringList() << "K3I0" << "KIA" << "KIB" << "KIC" << "KUA" << "KUB" << "KUC" << "KUABT" << "KUBCT" <<
                                        "KUCAT" << "K3U0R" << "K3U0S" << "K3U0T" << "KUADC" << "AUADC" << "KUBDC" << "AUBDC" <<
                                        "KUCDC" << "AUCDC" << "KUMDC" << "AUMDC" << "KRA" << "ARA" << "KRB" << "ARB" << "KRC" <<
                                        "ARC" << "KY01T" << "KY02T" << "KY03T" << "KY01R" << "KY02R" << "KY03R" << "KY04R" <<
                                        "KY04S" << "KY04T";

    for(QString key: list)
        sendSettingWriteRequest(key, key, DEVICE_MENU_ITEM_SETTINGS_ITEM_IN_ANALOG);
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

    saveDeviceSettings();
}
/*!
 * \brief ConfiguratorWindow::protectionMTZ1Write
 *
 * Запись настроек МТЗ1
 */
void ConfiguratorWindow::protectionMTZ1Write()
{
    QStringList list = QStringList() << "M06" << "X01" << "M08" << "K31";

    for(QString key: list)
        sendSettingWriteRequest(key, key, DEVICE_MENU_PROTECT_ITEM_CURRENT);

    sendSettingControlWriteRequest("M05", DEVICE_MENU_PROTECT_ITEM_CURRENT);
}
/*!
 * \brief ConfiguratorWindow::protectionMTZ2Write
 *
 * Запись настроек МТЗ2
 */
void ConfiguratorWindow::protectionMTZ2Write()
{
    QStringList list = QStringList() << "M10" << "X03" << "M11" << "M12";

    for(QString key: list)
        sendSettingWriteRequest(key, key, DEVICE_MENU_PROTECT_ITEM_CURRENT);

    sendSettingControlWriteRequest("M09", DEVICE_MENU_PROTECT_ITEM_CURRENT);
}
/*!
 * \brief ConfiguratorWindow::protectionMTZ3Write
 *
 * Запись настроек МТЗ3
 */
void ConfiguratorWindow::protectionMTZ3Write()
{
    QStringList list = QStringList() << "M14" << "X04" << "K22" << "TZ1" << "TZ2" << "TZ3" << "TZ4" << "TZ5" << "TZ6" << "TZ7";

    for(QString key: list)
        sendSettingWriteRequest(key, key, DEVICE_MENU_PROTECT_ITEM_CURRENT);

    sendSettingControlWriteRequest("M13", DEVICE_MENU_PROTECT_ITEM_CURRENT);
    sendSettingControlWriteRequest("TZ", DEVICE_MENU_PROTECT_ITEM_CURRENT);
}
/*!
 * \brief ConfiguratorWindow::protectionMTZ3SetCharWrite
 *
 *  Запись токовых характеристик защиты МТЗ3
 */
void ConfiguratorWindow::protectionMTZ3SetCharWrite()
{
//    sendSettingWriteRequest("TZ1", "TZ7", DEVICE_MENU_PROTECT_ITEM_CURRENT);
}
/*!
 * \brief ConfiguratorWindow::protectionMTZ3ProperySteepWrite
 *
 *  Запись характеристики защиты МТЗ3 - Крутая
 */
void ConfiguratorWindow::protectionMTZ3PropertySteepWrite()
{
//    sendSettingWriteRequest("TZ1", "TZ1", DEVICE_MENU_PROTECT_ITEM_CURRENT);
}
/*!
 * \brief ConfiguratorWindow::protectionMTZ3ProperySlopWrite
 *
 * Запись характеристики защиты МТЗ3 - Пологая
 */
void ConfiguratorWindow::protectionMTZ3PropertySlopWrite()
{
//    sendSettingWriteRequest("TZ2", "TZ2", DEVICE_MENU_PROTECT_ITEM_CURRENT);
}
/*!
 * \brief ConfiguratorWindow::protectionMTZ3ProperyInversionWrite
 *
 * Запись характеристики защиты МТЗ3 - Инверсная
 */
void ConfiguratorWindow::protectionMTZ3PropertyInversionWrite()
{
//    sendSettingWriteRequest("TZ3", "TZ3", DEVICE_MENU_PROTECT_ITEM_CURRENT);
}
/*!
 * \brief ConfiguratorWindow::protectionMTZ3ProperyDInversionWrite
 *
 * Запись характеристики защиты МТЗ3 - Длительно инверсная
 */
void ConfiguratorWindow::protectionMTZ3PropertyDInversionWrite()
{
//    sendSettingWriteRequest("TZ4", "TZ4", DEVICE_MENU_PROTECT_ITEM_CURRENT);
}
/*!
 * \brief ConfiguratorWindow::protectionMTZ3ProperyBackWrite
 *
 * Запись характеристики защиты МТЗ3 - Возвратно зависимая
 */
void ConfiguratorWindow::protectionMTZ3PropertyBackWrite()
{
//    sendSettingWriteRequest("TZ5", "TZ5", DEVICE_MENU_PROTECT_ITEM_CURRENT);
}
/*!
 * \brief ConfiguratorWindow::protectionMTZ3ProperyStrongInversionWrite
 *
 * Запись характеристики защиты МТЗ3 - Сильно инверсная
 */
void ConfiguratorWindow::protectionMTZ3PropertyStrongInversionWrite()
{
//    sendSettingWriteRequest("TZ6", "TZ6", DEVICE_MENU_PROTECT_ITEM_CURRENT);
}
/*!
 * \brief ConfiguratorWindow::protectionMTZ3ProperyExtremalInversionWrite
 *
 * Запись характеристики защиты МТЗ3 - Экстремально инверсная
 */
void ConfiguratorWindow::protectionMTZ3PropertyExtremalInversionWrite()
{
//    sendSettingWriteRequest("TZ7", "TZ7", DEVICE_MENU_PROTECT_ITEM_CURRENT);
}
/*!
 * \brief ConfiguratorWindow::protectionMTZ4Write
 *
 * Запись настроек МТЗ4
 */
void ConfiguratorWindow::protectionMTZ4Write()
{
    QStringList list = QStringList() << "M17" << "X05" << "M07" << "X05a" << "M18";

    for(QString key: list)
        sendSettingWriteRequest(key, key, DEVICE_MENU_PROTECT_ITEM_CURRENT);

    sendSettingControlWriteRequest("M16", DEVICE_MENU_PROTECT_ITEM_CURRENT);
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
    protectionStartingWrite();
    protectionIminWrite();

    QString protection = "MTZ1,MTZ2,MTZ3,MTZ4,STARTING,IMIN";
    sendProtectionWorkModeRequest(protection, FUNCTION_SAVE, DEVICE_MENU_PROTECT_ITEM_CURRENT);

    saveDeviceSettings();
}
/*!
 * \brief ConfiguratorWindow::protectionUmax1Write
 *
 * Запись защиты Umax1
 */
void ConfiguratorWindow::protectionUmax1Write()
{
    QStringList list = QStringList() << "M33" << "X11" << "M34";

    for(QString key: list)
        sendSettingWriteRequest(key, key, DEVICE_MENU_PROTECT_ITEM_POWER);

    sendSettingControlWriteRequest("M32", DEVICE_MENU_PROTECT_ITEM_POWER);
}
/*!
 * \brief ConfiguratorWindow::protectionUmax2Write
 *
 * Запись защиты Umax2
 */
void ConfiguratorWindow::protectionUmax2Write()
{
    QStringList list = QStringList() << "M36" << "X12" << "M37";

    for(QString key: list)
        sendSettingWriteRequest(key, key, DEVICE_MENU_PROTECT_ITEM_POWER);

    sendSettingControlWriteRequest("M35", DEVICE_MENU_PROTECT_ITEM_POWER);
}
/*!
 * \brief ConfiguratorWindow::protectionUmin1Write
 *
 * Запись защиты Umin1
 */
void ConfiguratorWindow::protectionUmin1Write()
{
    QStringList list = QStringList() << "M41" << "X13" << "M42";

    for(QString key: list)
        sendSettingWriteRequest(key, key, DEVICE_MENU_PROTECT_ITEM_POWER);

    sendSettingControlWriteRequest("M38", DEVICE_MENU_PROTECT_ITEM_POWER);
    sendSettingControlWriteRequest("M39", DEVICE_MENU_PROTECT_ITEM_POWER);
    sendSettingReadRequestVariableState("N98", "V09,V15", "_1", DEVICE_MENU_PROTECT_ITEM_POWER, true);
}
/*!
 * \brief ConfiguratorWindow::protectionUmin2Write
 *
 * Запись защиты Umin2
 */
void ConfiguratorWindow::protectionUmin2Write()
{
    QStringList list = QStringList() << "M46" << "X14" << "M47";

    for(QString key: list)
        sendSettingWriteRequest(key, key, DEVICE_MENU_PROTECT_ITEM_POWER);

    sendSettingControlWriteRequest("M43", DEVICE_MENU_PROTECT_ITEM_POWER);
    sendSettingControlWriteRequest("M44", DEVICE_MENU_PROTECT_ITEM_POWER);
    sendSettingReadRequestVariableState("N99", "V09,V15", "_1_1", DEVICE_MENU_PROTECT_ITEM_POWER, true);
}
/*!
 * \brief ConfiguratorWindow::protection3U0Write
 *
 * Запись защиты 3U0
 */
void ConfiguratorWindow::protection3U0Write()
{
    QStringList list = QStringList() << "M49" << "X15" << "M50";

    for(QString key: list)
        sendSettingWriteRequest(key, key, DEVICE_MENU_PROTECT_ITEM_POWER);

    sendSettingControlWriteRequest("M48", DEVICE_MENU_PROTECT_ITEM_POWER);
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
    sendProtectionWorkModeRequest("UMAX1,UMAX2,UMIN1,UMIN2,3U0", FUNCTION_SAVE, DEVICE_MENU_PROTECT_ITEM_POWER);

    saveDeviceSettings();
}
/*!
 * \brief ConfiguratorWindow::protectionOZZ1Write
 *
 * Запись защиты ОЗЗ1
 */
void ConfiguratorWindow::protectionOZZ1Write()
{
    QStringList list = QStringList() << "M23" << "X07" << "M24";

    for(QString key: list)
        sendSettingWriteRequest(key, key, DEVICE_MENU_PROTECT_ITEM_LEAK);

    sendSettingControlWriteRequest("M22", DEVICE_MENU_PROTECT_ITEM_LEAK);
}
/*!
 * \brief ConfiguratorWindow::protectionOZZ2Write
 *
 * Запись защиты ОЗЗ2
 */
void ConfiguratorWindow::protectionOZZ2Write()
{
    QStringList list = QStringList() << "K24" << "X07a" << "K25";

    for(QString key: list)
        sendSettingWriteRequest(key, key, DEVICE_MENU_PROTECT_ITEM_LEAK);

    sendSettingControlWriteRequest("K23", DEVICE_MENU_PROTECT_ITEM_LEAK);
}
/*!
 * \brief ConfiguratorWindow::protectionNZZ1Write
 *
 * Запись защиты НЗЗ1
 */
void ConfiguratorWindow::protectionNZZ1Write()
{
    QStringList list = QStringList() << "M26" << "X08" << "M27" << "X09" << "K21" << "M54" << "M28" << "M26C" << "M27C" << "K21C" << "X16" << "M28C";

    for(QString key: list)
        sendSettingWriteRequest(key, key, DEVICE_MENU_PROTECT_ITEM_LEAK);

    sendSettingControlWriteRequest("M25", DEVICE_MENU_PROTECT_ITEM_LEAK);
}
/*!
 * \brief ConfiguratorWindow::protectionNZZ2Write
 *
 * Запись защиты НЗЗ2
 */
void ConfiguratorWindow::protectionNZZ2Write()
{
    QStringList list = QStringList() << "K27" << "X08a" << "K28" << "X09a" << "K29" << "M58" << "K30" << "K27C" << "K28C" << "K29C" << "X17" << "K30C";

    for(QString key: list)
        sendSettingWriteRequest(key, key, DEVICE_MENU_PROTECT_ITEM_LEAK);

    sendSettingControlWriteRequest("K26", DEVICE_MENU_PROTECT_ITEM_LEAK);
}
/*!
 * \brief ConfiguratorWindow::protectionDirectedGroupWrite
 *
 * Запись группы защит Направленные
 */
void ConfiguratorWindow::protectionLeakGroupWrite()
{
    protectionOZZ1Write();
    protectionOZZ2Write();
    protectionNZZ1Write();
    protectionNZZ2Write();
    protectionBRUWrite();
    protectionVacuumWrite();

    sendProtectionWorkModeRequest("OZZ1,OZZ2,NZZ1,NZZ2,BRU,VACUUM", FUNCTION_SAVE, DEVICE_MENU_PROTECT_ITEM_LEAK);

    saveDeviceSettings();
}
/*!
 * \brief ConfiguratorWindow::protectionAchr1Write
 *
 * Запись защиты АЧР1
 */
void ConfiguratorWindow::protectionAchr1Write()
{
    QStringList list = QStringList() << "M52" << "M52V" << "M53";

    for(QString key: list)
        sendSettingWriteRequest(key, key, DEVICE_MENU_PROTECT_ITEM_FREQUENCY);

    sendSettingControlWriteRequest("M51", DEVICE_MENU_PROTECT_ITEM_FREQUENCY);
}
/*!
 * \brief ConfiguratorWindow::protectionAchr2Write
 *
 * Запись защиты АЧР2
 */
void ConfiguratorWindow::protectionAchr2Write()
{
    QStringList list = QStringList() << "M56" << "M56V" << "M57";

    for(QString key: list)
        sendSettingWriteRequest(key, key, DEVICE_MENU_PROTECT_ITEM_FREQUENCY);

    sendSettingControlWriteRequest("M55", DEVICE_MENU_PROTECT_ITEM_FREQUENCY);
}
/*!
 * \brief ConfiguratorWindow::protectionAchr3Write
 *
 * Запись защиты АЧР3
 */
void ConfiguratorWindow::protectionAchr3Write()
{
    QStringList list = QStringList() << "M60" << "M60V" << "M61";

    for(QString key: list)
        sendSettingWriteRequest(key, key, DEVICE_MENU_PROTECT_ITEM_FREQUENCY);

    sendSettingControlWriteRequest("M59", DEVICE_MENU_PROTECT_ITEM_FREQUENCY);
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

    sendProtectionWorkModeRequest("ACHR1,ACHR2,ACHR3", FUNCTION_SAVE, DEVICE_MENU_PROTECT_ITEM_FREQUENCY);

    saveDeviceSettings();
}
/*!
 * \brief ConfiguratorWindow::protectionArcWrite
 *
 * Запись защиты Дуговая
 */
void ConfiguratorWindow::protectionArcWrite()
{
    sendSettingWriteRequest("M64", "M64", DEVICE_MENU_PROTECT_ITEM_EXTERNAL);
    sendSettingWriteRequest("X19", "X19", DEVICE_MENU_PROTECT_ITEM_EXTERNAL);

    sendSettingControlWriteRequest("M63", DEVICE_MENU_PROTECT_ITEM_EXTERNAL);
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
    sendSettingWriteRequest("K07", "K07", DEVICE_MENU_PROTECT_ITEM_EXTERNAL);
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
    sendSettingWriteRequest("M04", "M04", DEVICE_MENU_PROTECT_ITEM_EXTERNAL);
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
    sendSettingWriteRequest("M45", "M45", DEVICE_MENU_PROTECT_ITEM_EXTERNAL);
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

    sendProtectionWorkModeRequest("ITEMARC,EXT1,EXT2,EXT3", FUNCTION_SAVE, DEVICE_MENU_PROTECT_ITEM_EXTERNAL);

    saveDeviceSettings();
}
/*!
 * \brief ConfiguratorWindow::protectionStartingWrite
 *
 * Запись защиты Пусковая
 */
void ConfiguratorWindow::protectionStartingWrite()
{
    QStringList list = QStringList() << "M20" << "X06" << "M21";

    for(QString key: list)
        sendSettingWriteRequest(key, key, DEVICE_MENU_PROTECT_ITEM_CURRENT);

    sendSettingControlWriteRequest("M19", DEVICE_MENU_PROTECT_ITEM_CURRENT);
    sendProtectionWorkModeRequest("STARTING", FUNCTION_SAVE, DEVICE_MENU_PROTECT_ITEM_CURRENT);
}
/*!
 * \brief ConfiguratorWindow::protectionIminWrite
 *
 * Запись защиты Imin
 */
void ConfiguratorWindow::protectionIminWrite()
{
    QStringList list = QStringList() << "M30" << "X10" << "M31";

    for(QString key: list)
        sendSettingWriteRequest(key, key, DEVICE_MENU_PROTECT_ITEM_CURRENT);

    sendSettingControlWriteRequest("M29", DEVICE_MENU_PROTECT_ITEM_CURRENT);
    sendProtectionWorkModeRequest("IMIN", FUNCTION_SAVE, DEVICE_MENU_PROTECT_ITEM_CURRENT);
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

    saveDeviceSettings();
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
    sendSettingWriteRequest("M67", "M67", DEVICE_MENU_PROTECT_ITEM_TEMPERATURE);
    sendSettingWriteRequest("X20", "X20", DEVICE_MENU_PROTECT_ITEM_TEMPERATURE);
    sendSettingWriteRequest("M69", "M69", DEVICE_MENU_PROTECT_ITEM_TEMPERATURE);
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
    sendSettingWriteRequest("M68", "M68", DEVICE_MENU_PROTECT_ITEM_TEMPERATURE);
    sendSettingWriteRequest("X21", "X21", DEVICE_MENU_PROTECT_ITEM_TEMPERATURE);
    sendSettingWriteRequest("M70", "M70", DEVICE_MENU_PROTECT_ITEM_TEMPERATURE);
    sendProtectionWorkModeRequest("TEMP2", FUNCTION_SAVE, DEVICE_MENU_PROTECT_ITEM_TEMPERATURE);
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
    sendProtectionWorkModeRequest("TEMP1,TEMP2", FUNCTION_SAVE, DEVICE_MENU_PROTECT_ITEM_TEMPERATURE);

    saveDeviceSettings();
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

    sendSettingReadRequestVariableState("I67", "I50,I15,I66,N55", "_1", DEVICE_MENU_PROTECT_ITEM_RESERVE, true);
}
/*!
 * \brief ConfiguratorWindow::protectionLevel2Write
 *
 * Запись защиты Уровневая2
 */
void ConfiguratorWindow::protectionLevel2Write()
{
    sendSettingControlWriteRequest("K11", DEVICE_MENU_PROTECT_ITEM_RESERVE);
    sendSettingWriteRequest("M79", "M79", DEVICE_MENU_PROTECT_ITEM_RESERVE);

    sendSettingReadRequestVariableState("I68", "I50,I15,I66,N55", "_1_1", DEVICE_MENU_PROTECT_ITEM_RESERVE, true);
}
/*!
 * \brief ConfiguratorWindow::protectionSignalStartWrite
 *
 * Запись защиты Сигнал Пуска
 */
void ConfiguratorWindow::protectionSignalStartWrite()
{           
    QVector<quint16> data(24, 0); // 24 ячейки со значением нуль
    int pos = groupMenuPosition(tr("Сигнал пуска"), ui->tableWidgetProtectionGroupReserve);

    for(int row = pos; row < ui->tableWidgetProtectionGroupReserve->rowCount(); row++)
    {
        QWidget* widget = groupMenuCellWidget(ui->tableWidgetProtectionGroupReserve, row, 1);

        if(!widget)
            continue;

        if(QString(widget->metaObject()->className()).toUpper() != "CMENUCOMBOBOX")
            continue;

        CMenuComboBox* combobox = qobject_cast<CMenuComboBox*>(widget);

        if(!combobox)
            continue;

        QString key = combobox->objectName().remove("comboBox");

        if(key.isEmpty())
            continue;

        int bit     = m_variable_bits[key];
        int val_pos = bit/16;
        int bit_pos = bit%16;

        if(val_pos < data.count())
        {
            int item_pos = combobox->currentIndex();

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

    CModBusDataUnit unit(static_cast<quint8>(m_serialPortSettings_window->deviceID()), CModBusDataUnit::WriteMultipleRegisters,
                         static_cast<quint16>(addr), tdata);

    unit.setProperty("REQUEST", PORTECT_RESERVE_SIGNAL_START);

    m_modbus->sendData(unit);

    saveDeviceSettings();
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

    saveDeviceSettings();
}
/*!
 * \brief ConfiguratorWindow::protectionBRUWrite
 *
 * Запись защиты БРУ
 */
void ConfiguratorWindow::protectionBRUWrite()
{
    QStringList list = QStringList() << "M97" << "M98" << "M99f" << "M99" << "M99c" << "M99d" << "M99e" << "M96";

    for(QString key: list)
        sendSettingWriteRequest(key, key, DEVICE_MENU_PROTECT_ITEM_LEAK);

    sendSettingControlWriteRequest("M93", DEVICE_MENU_PROTECT_ITEM_LEAK);
    sendSettingControlWriteRequest("M95", DEVICE_MENU_PROTECT_ITEM_LEAK);
    sendSettingControlWriteRequest("M99a", DEVICE_MENU_PROTECT_ITEM_LEAK);
}
/*!
 * \brief ConfiguratorWindow::protectionVacuumWrite
 *
 * Запись защиты Вакуум
 */
void ConfiguratorWindow::protectionVacuumWrite()
{
    QStringList list = QStringList() << "M91" << "X23" << "M92";

    for(QString key: list)
        sendSettingWriteRequest(key, key, DEVICE_MENU_PROTECT_ITEM_LEAK);

    sendSettingControlWriteRequest("M90", DEVICE_MENU_PROTECT_ITEM_LEAK);
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

    saveDeviceSettings();
}
/*!
 * \brief ConfiguratorWindow::automationSwitchWrite
 *
 * Запись автоматик Выключатель
 */
void ConfiguratorWindow::automationSwitchWrite()
{
    sendSettingControlWriteRequest("K32", DEVICE_MENU_ITEM_AUTOMATION_SWITCH);
    sendSettingControlWriteRequest("K06", DEVICE_MENU_ITEM_AUTOMATION_SWITCH);
    sendSettingControlWriteRequest("K12", DEVICE_MENU_ITEM_AUTOMATION_SWITCH);
    sendSettingControlWriteRequest("K03", DEVICE_MENU_ITEM_AUTOMATION_SWITCH);
    sendSettingControlWriteRequest("K13", DEVICE_MENU_ITEM_AUTOMATION_SWITCH);
    sendSettingControlWriteRequest("K14", DEVICE_MENU_ITEM_AUTOMATION_SWITCH);
    sendSettingControlWriteRequest("K15", DEVICE_MENU_ITEM_AUTOMATION_SWITCH);
    sendSettingControlWriteRequest("SPRK_CNF", DEVICE_MENU_ITEM_AUTOMATION_SWITCH);

    sendSettingWriteRequest("K02", "K02", DEVICE_MENU_ITEM_AUTOMATION_SWITCH);
    sendSettingWriteRequest("K04", "K04", DEVICE_MENU_ITEM_AUTOMATION_SWITCH);
    sendSettingWriteRequest("K10", "K10", DEVICE_MENU_ITEM_AUTOMATION_SWITCH);
    sendSettingWriteRequest("K01", "K01", DEVICE_MENU_ITEM_AUTOMATION_SWITCH, 1);
    sendSettingWriteRequest("K09", "K09", DEVICE_MENU_ITEM_AUTOMATION_SWITCH);
    sendSettingWriteRequest("X22", "X22", DEVICE_MENU_ITEM_AUTOMATION_SWITCH);
    sendSettingWriteRequest("K08", "K08", DEVICE_MENU_ITEM_AUTOMATION_SWITCH);
    sendSettingWriteRequest("K05", "K05", DEVICE_MENU_ITEM_AUTOMATION_SWITCH);

    saveDeviceSettings();
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

    saveDeviceSettings();
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

    saveDeviceSettings();
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

    saveDeviceSettings();
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

    saveDeviceSettings();
}
/*!
 * \brief ConfiguratorWindow::automationAVRWrite
 *
 * Запись автоматик АВР
 */
void ConfiguratorWindow::automationAVRWrite()
{
    sendSettingControlWriteRequest("M81", DEVICE_MENU_ITEM_AUTOMATION_AVR);
    sendSettingWriteRequest("M84", "M84", DEVICE_MENU_ITEM_AUTOMATION_AVR);
}
/*!
 * \brief ConfiguratorWindow::automationAPVWrite
 *
 * Запись автоматик АПВ
 */
void ConfiguratorWindow::automationAPVWrite()
{
    sendSettingControlWriteRequest("M87", DEVICE_MENU_ITEM_AUTOMATION_APV);
    sendSettingWriteRequest("K17", "K17", DEVICE_MENU_ITEM_AUTOMATION_APV, 1);
    sendSettingWriteRequest("M88", "M88", DEVICE_MENU_ITEM_AUTOMATION_APV);
    sendSettingWriteRequest("M89", "M89", DEVICE_MENU_ITEM_AUTOMATION_APV);

    QString list = "V04,V07,V10,V13,V19,N64,V22,N67,V44,V62,V65,V68,V81,V86,V90";
    sendSettingReadRequestVariableState("I69", list, "_1", DEVICE_MENU_ITEM_AUTOMATION_APV, true);

    sendDeviceCommand(2);
}
/*!
 * \brief ConfiguratorWindow::automationKCNWrite
 *
 * Запись автоматик КЦН
 */
void ConfiguratorWindow::automationKCNWrite()
{
    sendSettingWriteRequest("K19", "K19", DEVICE_MENU_ITEM_AUTOMATION_KCN);
}
/*!
 * \brief ConfiguratorWindow::automationAPVSignalStartWrite
 *
 * Запись синалов пуска автоматики АПВ
 */
void ConfiguratorWindow::automationAPVSignalStartWrite()
{
//    QVector<quint16> data(24, 0); // 24 ячейки со значением нуль
//    int pos = groupMenuPosition(tr("АПВ сигналы пуска"), ui->tableWidgetAutomationGroup);

//    for(int row = pos; row < ui->tableWidgetAutomationGroup->rowCount(); row++)
//    {
//        QWidget* widget = groupMenuCellWidget(ui->tableWidgetAutomationGroup, row, 1);

//        if(!widget)
//            continue;

//        if(QString(widget->metaObject()->className()).toUpper() != "QCOMBOBOX")
//            continue;

//        QComboBox* combobox = qobject_cast<QComboBox*>(widget);

//        if(!combobox)
//            continue;

//        QString key = (combobox->objectName().remove("comboBox")).remove("_1");

//        if(key.isEmpty())
//            continue;

//        int bit     = m_variable_bits[key];
//        int val_pos = bit/16;
//        int bit_pos = bit%16;

//        if(val_pos < data.count())
//        {
//            int item_pos = combobox->currentIndex();

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

//    CModBusDataUnit unit(static_cast<quint8>(m_serialPortSettings_window->deviceID()), CModBusDataUnit::WriteMultipleRegisters,
//                         static_cast<quint16>(addr), tdata);

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

    saveDeviceSettings();
}
/*!
 * \brief ConfiguratorWindow::purposeInputWrite
 *
 * Запись настроек привязок Дискретных входов
 */
void ConfiguratorWindow::purposeInputWrite()
{
    sendPurposeDIWriteRequest(512, 592);
    sendPurposeDIWriteRequest(594, 676);
    sendPurposeInverseDIWriteRequest(768, 848);
    sendPurposeInverseDIWriteRequest(850, 932);

    saveDeviceSettings();
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

    saveDeviceSettings();
}
//-------------------------------------------------
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

    saveDeviceSettings();
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

    saveDeviceSettings();
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

    CModBusDataUnit unit(static_cast<quint8>(m_serialPortSettings_window->deviceID()), CModBusDataUnit::WriteMultipleRegisters, 0x2000, data);

    unit.setProperty(tr("REQUEST"), DATETIME_TYPE);

    m_modbus->sendData(unit);

    saveDeviceSettings();
}
//------------------------------------------------
void ConfiguratorWindow::synchronizationDateTime()
{
    dateTimeWrite(QDateTime::currentDateTime());
}
/*!
 * \brief ConfiguratorWindow::settingCommunicationsWrite
 *
 * Запись настроек связи - скорость передачи данных по com-порту
 */
void ConfiguratorWindow::settingCommunicationsWrite()
{
    m_is_new_baudrate = true; // взводим флаг о изменение скорости порта
    sendDeviceCommand(ui->comboBoxCommunicationBaudrate->currentIndex() + 6); // новая скорость
    saveDeviceSettings();
}
/*!
 * \brief ConfiguratorWindow::protectionMTZ1Read
 *
 * Чтение защиты МТЗ1
 */
void ConfiguratorWindow::protectionMTZ1Read()
{
    QStringList list = QStringList() << "M06" << "X01" << "M08" << "K31";

    for(QString key: list)
        sendSettingReadRequest(key, key, CModBusDataUnit::ReadHoldingRegisters, 2, DEVICE_MENU_PROTECT_ITEM_CURRENT);

    sendSettingControlReadRequest("M05", DEVICE_MENU_PROTECT_ITEM_CURRENT);
}
/*!
 * \brief ConfiguratorWindow::protectionMTZ2Read
 *
 * Чтение защиты МТЗ2
 */
void ConfiguratorWindow::protectionMTZ2Read()
{
    QStringList list = QStringList() << "M10" << "X03" << "M11" << "M12";

    for(QString key: list)
        sendSettingReadRequest(key, key, CModBusDataUnit::ReadHoldingRegisters, 2, DEVICE_MENU_PROTECT_ITEM_CURRENT);

    sendSettingControlReadRequest("M09", DEVICE_MENU_PROTECT_ITEM_CURRENT);
}
/*!
 * \brief ConfiguratorWindow::protectionMTZ3Read
 *
 * Чтение защиты МТЗ3
 */
void ConfiguratorWindow::protectionMTZ3Read()
{
    QStringList list = QStringList() << "M14" << "X04" << "K22" << "TZ1" << "TZ2" << "TZ3" << "TZ4" << "TZ5" << "TZ6" << "TZ7";

    for(QString key: list)
        sendSettingReadRequest(key, key, CModBusDataUnit::ReadHoldingRegisters, 2, DEVICE_MENU_PROTECT_ITEM_CURRENT);

    sendSettingControlReadRequest("M13", DEVICE_MENU_PROTECT_ITEM_CURRENT);
    sendSettingControlReadRequest("TZ", DEVICE_MENU_PROTECT_ITEM_CURRENT);
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
//    sendSettingReadRequest("TZ1", "TZ1", CModBusDataUnit::ReadHoldingRegisters, 2, DEVICE_MENU_PROTECT_ITEM_CURRENT);
}
/*!
 * \brief ConfiguratorWindow::protectionMTZ3ProperySlopRead
 *
 * Чтение характеристики защиты МТЗ3 - Пологая
 */
void ConfiguratorWindow::protectionMTZ3PropertySlopRead()
{
//    sendSettingReadRequest("TZ2", "TZ2", CModBusDataUnit::ReadHoldingRegisters, 2, DEVICE_MENU_PROTECT_ITEM_CURRENT);
}
/*!
 * \brief ConfiguratorWindow::protectionMTZ3ProperyInversionRead
 *
 * Чтение характеристики защиты МТЗ3 - Инверсная
 */
void ConfiguratorWindow::protectionMTZ3PropertyInversionRead()
{
//    sendSettingReadRequest("TZ3", "TZ3", CModBusDataUnit::ReadHoldingRegisters, 2, DEVICE_MENU_PROTECT_ITEM_CURRENT);
}
/*!
 * \brief ConfiguratorWindow::protectionMTZ3ProperyDInversionRead
 *
 * Чтение характеристики защиты МТЗ3 - Длительно инверсная
 */
void ConfiguratorWindow::protectionMTZ3PropertyDInversionRead()
{
//    sendSettingReadRequest("TZ4", "TZ4", CModBusDataUnit::ReadHoldingRegisters, 2, DEVICE_MENU_PROTECT_ITEM_CURRENT);
}
/*!
 * \brief ConfiguratorWindow::protectionMTZ3ProperyBackRead
 *
 * Чтение характеристики защиты МТЗ3 - Обратно зависимая
 */
void ConfiguratorWindow::protectionMTZ3PropertyBackRead()
{
//    sendSettingReadRequest("TZ5", "TZ5", CModBusDataUnit::ReadHoldingRegisters, 2, DEVICE_MENU_PROTECT_ITEM_CURRENT);
}
/*!
 * \brief ConfiguratorWindow::protectionMTZ3ProperyStrongInversionRead
 *
 * Чтение характеристики защиты МТЗ3 - Сильно инверсная
 */
void ConfiguratorWindow::protectionMTZ3PropertyStrongInversionRead()
{
//    sendSettingReadRequest("TZ6", "TZ6", CModBusDataUnit::ReadHoldingRegisters, 2, DEVICE_MENU_PROTECT_ITEM_CURRENT);
}
/*!
 * \brief ConfiguratorWindow::protectionMTZ3ProperyExtremalInversionRead
 *
 * Чтение характеристики защиты МТЗ3 - Экстремально инверсная
 */
void ConfiguratorWindow::protectionMTZ3PropertyExtremalInversionRead()
{
//    sendSettingReadRequest("TZ7", "TZ7", CModBusDataUnit::ReadHoldingRegisters, 2, DEVICE_MENU_PROTECT_ITEM_CURRENT);
}
/*!
 * \brief ConfiguratorWindow::protectionMTZ4Read
 *
 * Чтение защиты МТЗ4
 */
void ConfiguratorWindow::protectionMTZ4Read()
{
    QStringList list = QStringList() << "M17" << "X05" << "M07" << "X05a" << "M18";

    for(QString key: list)
        sendSettingReadRequest(key, key, CModBusDataUnit::ReadHoldingRegisters, 2, DEVICE_MENU_PROTECT_ITEM_CURRENT);

    sendSettingControlReadRequest("M16", DEVICE_MENU_PROTECT_ITEM_CURRENT);
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
    protectionStartingRead();
    protectionIminRead();

    sendProtectionWorkModeRequest("MTZ1,MTZ2,MTZ3,MTZ4,STARTING,IMIN", FUNCTION_READ, DEVICE_MENU_PROTECT_ITEM_CURRENT);
}
/*!
 * \brief ConfiguratorWindow::protectionUmax1Read
 *
 * Чтение защиты Umax1
 */
void ConfiguratorWindow::protectionUmax1Read()
{
    QStringList list = QStringList() << "M33" << "X11" << "M34";

    for(QString key: list)
        sendSettingReadRequest(key, key, CModBusDataUnit::ReadHoldingRegisters, 2, DEVICE_MENU_PROTECT_ITEM_POWER);

    sendSettingControlReadRequest("M32", DEVICE_MENU_PROTECT_ITEM_POWER);
}
/*!
 * \brief ConfiguratorWindow::protectionUmax2Read
 *
 * Чтение защиты Umax2
 */
void ConfiguratorWindow::protectionUmax2Read()
{
    QStringList list = QStringList() << "M36" << "X12" << "M37";

    for(QString key: list)
        sendSettingReadRequest(key, key, CModBusDataUnit::ReadHoldingRegisters, 2, DEVICE_MENU_PROTECT_ITEM_POWER);

    sendSettingControlReadRequest("M35", DEVICE_MENU_PROTECT_ITEM_POWER);
}
/*!
 * \brief ConfiguratorWindow::protectionUmin1Read
 *
 * Чтение защиты Umin1
 */
void ConfiguratorWindow::protectionUmin1Read()
{
    QStringList list = QStringList() << "M41" << "X13" << "M42";

    for(QString key: list)
        sendSettingReadRequest(key, key, CModBusDataUnit::ReadHoldingRegisters, 2, DEVICE_MENU_PROTECT_ITEM_POWER);

    sendSettingControlReadRequest("M38", DEVICE_MENU_PROTECT_ITEM_POWER);
    sendSettingControlReadRequest("M39", DEVICE_MENU_PROTECT_ITEM_POWER);
    sendSettingReadRequestVariableState("N98", "V09,V15", "_1", DEVICE_MENU_PROTECT_ITEM_POWER);
}
/*!
 * \brief ConfiguratorWindow::protectionUmin2Read
 *
 * Чтение защиты Umin2
 */
void ConfiguratorWindow::protectionUmin2Read()
{
    QStringList list = QStringList() << "M46" << "X14" << "M47";

    for(QString key: list)
        sendSettingReadRequest(key, key, CModBusDataUnit::ReadHoldingRegisters, 2, DEVICE_MENU_PROTECT_ITEM_POWER);

    sendSettingControlReadRequest("M43", DEVICE_MENU_PROTECT_ITEM_POWER);
    sendSettingControlReadRequest("M44", DEVICE_MENU_PROTECT_ITEM_POWER);
    sendSettingReadRequestVariableState("N99", "V09,V15", "_1_1", DEVICE_MENU_PROTECT_ITEM_POWER);
}
/*!
 * \brief ConfiguratorWindow::protection3U0Read
 *
 * Чтение защиты 3U0
 */
void ConfiguratorWindow::protection3U0Read()
{
    QStringList list = QStringList() << "M49" << "X15" << "M50";

    for(QString key: list)
        sendSettingReadRequest(key, key, CModBusDataUnit::ReadHoldingRegisters, 2, DEVICE_MENU_PROTECT_ITEM_POWER);

    sendSettingControlReadRequest("M48", DEVICE_MENU_PROTECT_ITEM_POWER);
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

    sendProtectionWorkModeRequest("UMAX1,UMAX2,UMIN1,UMIN2,3U0", FUNCTION_READ, DEVICE_MENU_PROTECT_ITEM_POWER);
}
/*!
 * \brief ConfiguratorWindow::protectionDirectedOZZ1Read
 *
 * Чтение защиты ОЗЗ1
 */
void ConfiguratorWindow::protectionOZZ1Read()
{
    QStringList list = QStringList() << "M23" << "X07" << "M24";

    for(QString key: list)
        sendSettingReadRequest(key, key, CModBusDataUnit::ReadHoldingRegisters, 2, DEVICE_MENU_PROTECT_ITEM_LEAK);

    sendSettingControlReadRequest("M22", DEVICE_MENU_PROTECT_ITEM_LEAK);
    sendProtectionWorkModeRequest("OZZ1", FUNCTION_READ, DEVICE_MENU_PROTECT_ITEM_LEAK);
}
/*!
 * \brief ConfiguratorWindow::protectionDirectedOZZ2Read
 *
 * Чтение защиты ОЗЗ2
 */
void ConfiguratorWindow::protectionOZZ2Read()
{
    QStringList list = QStringList() << "K24" << "X07a" << "K25";

    for(QString key: list)
        sendSettingReadRequest(key, key, CModBusDataUnit::ReadHoldingRegisters, 2, DEVICE_MENU_PROTECT_ITEM_LEAK);

    sendSettingControlReadRequest("K23", DEVICE_MENU_PROTECT_ITEM_LEAK);
    sendProtectionWorkModeRequest("OZZ2", FUNCTION_READ, DEVICE_MENU_PROTECT_ITEM_LEAK);
}
/*!
 * \brief ConfiguratorWindow::protectionDirectedNZZ1Read
 *
 * Чтение защиты НЗЗ1
 */
void ConfiguratorWindow::protectionNZZ1Read()
{
    QStringList list = QStringList() << "M26" << "X08" << "M27" << "X09" << "K21" << "M54" << "M28" << "M26C" << "M27C" << "K21C" << "X16" << "M28C";

    for(QString key: list)
        sendSettingReadRequest(key, key, CModBusDataUnit::ReadHoldingRegisters, 2, DEVICE_MENU_PROTECT_ITEM_LEAK);

    sendSettingControlReadRequest("M25", DEVICE_MENU_PROTECT_ITEM_LEAK);
    sendProtectionWorkModeRequest("NZZ1", FUNCTION_READ, DEVICE_MENU_PROTECT_ITEM_LEAK);
}
/*!
 * \brief ConfiguratorWindow::protectionDirectedNZZ2Read
 *
 * Чтение защиты НЗЗ2
 */
void ConfiguratorWindow::protectionNZZ2Read()
{
    QStringList list = QStringList() << "K27" << "X08a" << "K28" << "X09a" << "K29" << "M58" << "K30" << "K27C" << "K28C" << "K29C" << "X17" << "K30C";

    for(QString key: list)
        sendSettingReadRequest(key, key, CModBusDataUnit::ReadHoldingRegisters, 2, DEVICE_MENU_PROTECT_ITEM_LEAK);

    sendSettingControlReadRequest("K26", DEVICE_MENU_PROTECT_ITEM_LEAK);
    sendProtectionWorkModeRequest("NZZ2", FUNCTION_READ, DEVICE_MENU_PROTECT_ITEM_LEAK);
}
/*!
 * \brief ConfiguratorWindow::protectionDirectedGroupRead
 *
 * Чтение группы направленных защит
 */
void ConfiguratorWindow::protectionLeakGroupRead()
{
    protectionOZZ1Read();
    protectionOZZ2Read();
    protectionNZZ1Read();
    protectionNZZ2Read();
    protectionBRURead();
    protectionVacuumRead();
}
/*!
 * \brief ConfiguratorWindow::protectionAchr1Read
 *
 * Чтение защиты АЧР1
 */
void ConfiguratorWindow::protectionAchr1Read()
{
    QStringList list = QStringList() << "M52" << "M52V" << "M53";

    for(QString key: list)
        sendSettingReadRequest(key, key, CModBusDataUnit::ReadHoldingRegisters, 2, DEVICE_MENU_PROTECT_ITEM_FREQUENCY);

    sendSettingControlReadRequest("M51", DEVICE_MENU_PROTECT_ITEM_FREQUENCY);
}
/*!
 * \brief ConfiguratorWindow::protectionAchr2Read
 *
 * Чтение защиты АЧР2
 */
void ConfiguratorWindow::protectionAchr2Read()
{
    QStringList list = QStringList() << "M56" << "M56V" << "M57";

    for(QString key: list)
        sendSettingReadRequest(key, key, CModBusDataUnit::ReadHoldingRegisters, 2, DEVICE_MENU_PROTECT_ITEM_FREQUENCY);

    sendSettingControlReadRequest("M55", DEVICE_MENU_PROTECT_ITEM_FREQUENCY);
}
/*!
 * \brief ConfiguratorWindow::protectionAchr3Read
 *
 * Чтение защиты АЧР3
 */
void ConfiguratorWindow::protectionAchr3Read()
{
    QStringList list = QStringList() << "M60" << "M60V" << "M61";

    for(QString key: list)
        sendSettingReadRequest(key, key, CModBusDataUnit::ReadHoldingRegisters, 2, DEVICE_MENU_PROTECT_ITEM_FREQUENCY);

    sendSettingControlReadRequest("M59", DEVICE_MENU_PROTECT_ITEM_FREQUENCY);
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

    sendProtectionWorkModeRequest("ACHR1,ACHR2,ACHR3", FUNCTION_READ, DEVICE_MENU_PROTECT_ITEM_FREQUENCY);
}
/*!
 * \brief ConfiguratorWindow::protectionArc
 *
 * Чтение защиты Дуговая
 */
void ConfiguratorWindow::protectionArcRead()
{
    QStringList list = QStringList() << "M64" << "X19";

    for(QString key: list)
        sendSettingReadRequest(key, key, CModBusDataUnit::ReadHoldingRegisters, 2, DEVICE_MENU_PROTECT_ITEM_EXTERNAL);

    sendSettingControlReadRequest("M63", DEVICE_MENU_PROTECT_ITEM_EXTERNAL);
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
    sendSettingReadRequest("K07", "K07", CModBusDataUnit::ReadHoldingRegisters, 2, DEVICE_MENU_PROTECT_ITEM_EXTERNAL);
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
    sendSettingReadRequest("M04", "M04", CModBusDataUnit::ReadHoldingRegisters, 2, DEVICE_MENU_PROTECT_ITEM_EXTERNAL);
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
    sendSettingReadRequest("M45", "M45", CModBusDataUnit::ReadHoldingRegisters, 2, DEVICE_MENU_PROTECT_ITEM_EXTERNAL);
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

    sendProtectionWorkModeRequest("ITEMARC,EXT1,EXT2,EXT3", FUNCTION_READ, DEVICE_MENU_PROTECT_ITEM_EXTERNAL);
}
/*!
 * \brief ConfiguratorWindow::protectionStartingRead
 *
 * Чтение защиты Пусковая
 */
void ConfiguratorWindow::protectionStartingRead()
{
    QStringList list = QStringList() << "M20" << "X06" << "M21";

    for(QString key: list)
        sendSettingReadRequest(key, key, CModBusDataUnit::ReadHoldingRegisters, 2, DEVICE_MENU_PROTECT_ITEM_CURRENT);

    sendSettingControlReadRequest("M19", DEVICE_MENU_PROTECT_ITEM_CURRENT);
}
/*!
 * \brief ConfiguratorWindow::protectionIminRead
 *
 * Чтение защиты Imin
 */
void ConfiguratorWindow::protectionIminRead()
{
    QStringList list = QStringList() << "M30" << "X10" << "M31";

    for(QString key: list)
        sendSettingReadRequest(key, key, CModBusDataUnit::ReadHoldingRegisters, 2, DEVICE_MENU_PROTECT_ITEM_CURRENT);

    sendSettingControlReadRequest("M29", DEVICE_MENU_PROTECT_ITEM_CURRENT);
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
    sendSettingReadRequest("M67", "M67", CModBusDataUnit::ReadHoldingRegisters, 2, DEVICE_MENU_PROTECT_ITEM_TEMPERATURE);
    sendSettingReadRequest("X20", "X20", CModBusDataUnit::ReadHoldingRegisters, 2, DEVICE_MENU_PROTECT_ITEM_TEMPERATURE);
    sendSettingReadRequest("M69", "M69", CModBusDataUnit::ReadHoldingRegisters, 2, DEVICE_MENU_PROTECT_ITEM_TEMPERATURE);
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
    sendSettingReadRequest("M68", "M68", CModBusDataUnit::ReadHoldingRegisters, 2, DEVICE_MENU_PROTECT_ITEM_TEMPERATURE);
    sendSettingReadRequest("X21", "X21", CModBusDataUnit::ReadHoldingRegisters, 2, DEVICE_MENU_PROTECT_ITEM_TEMPERATURE);
    sendSettingReadRequest("M70", "M70", CModBusDataUnit::ReadHoldingRegisters, 2, DEVICE_MENU_PROTECT_ITEM_TEMPERATURE);
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

    sendProtectionWorkModeRequest("TEMP1,TEMP2", FUNCTION_READ, DEVICE_MENU_PROTECT_ITEM_TEMPERATURE);
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
    sendSettingReadRequestVariableState("I67", "I50,I15,I66,N55", "_1", DEVICE_MENU_PROTECT_ITEM_RESERVE);
}
/*!
 * \brief ConfiguratorWindow::protectionLevel2Read
 *
 * Чтение защиты Уровень2
 */
void ConfiguratorWindow::protectionLevel2Read()
{
    sendSettingControlReadRequest("K11", DEVICE_MENU_PROTECT_ITEM_RESERVE);
    sendSettingReadRequest("M79", "M79", CModBusDataUnit::ReadHoldingRegisters, 2, DEVICE_MENU_PROTECT_ITEM_RESERVE);
    sendSettingReadRequestVariableState("I68", "I50,I15,I66,N55", "_1_1", DEVICE_MENU_PROTECT_ITEM_RESERVE);
}
/*!
 * \brief ConfiguratorWindow::protectionSignalStartRead
 *
 * Чтение защиты Сигнала пуска
 */
void ConfiguratorWindow::protectionSignalStartRead()
{
    int addr = addressSettingKey("M80");

    CModBusDataUnit unit(static_cast<quint8>(m_serialPortSettings_window->deviceID()), CModBusDataUnit::ReadHoldingRegisters,
                         static_cast<quint16>(addr), QVector<quint16>() << 24);

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
}
/*!
 * \brief ConfiguratorWindow::protectionBRURead
 *
 * Чтение защиты БРУ
 */
void ConfiguratorWindow::protectionBRURead()
{
    QStringList list = QStringList() << "M97" << "M98" << "M99f" << "M99" << "M99c" << "M99d" << "M99e" << "M96";

    for(QString key: list)
        sendSettingReadRequest(key, key, CModBusDataUnit::ReadHoldingRegisters, 2, DEVICE_MENU_PROTECT_ITEM_LEAK);

    sendSettingControlReadRequest("M93", DEVICE_MENU_PROTECT_ITEM_LEAK);
    sendSettingControlReadRequest("M95", DEVICE_MENU_PROTECT_ITEM_LEAK);
    sendSettingControlReadRequest("M99a", DEVICE_MENU_PROTECT_ITEM_LEAK);
    sendProtectionWorkModeRequest("BRU", FUNCTION_READ, DEVICE_MENU_PROTECT_ITEM_LEAK);
}
/*!
 * \brief ConfiguratorWindow::protectionVacuumRead
 *
 * Чтение защиты Вакуум
 */
void ConfiguratorWindow::protectionVacuumRead()
{
    QStringList list = QStringList() << "M91" << "X23" << "M92";

    for(QString key: list)
        sendSettingReadRequest(key, key, CModBusDataUnit::ReadHoldingRegisters, 2, DEVICE_MENU_PROTECT_ITEM_LEAK);

    sendSettingControlReadRequest("M90", DEVICE_MENU_PROTECT_ITEM_LEAK);
    sendProtectionWorkModeRequest("VACUUM", FUNCTION_READ, DEVICE_MENU_PROTECT_ITEM_LEAK);
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
    sendSettingControlReadRequest("K32", DEVICE_MENU_ITEM_AUTOMATION_SWITCH);
    sendSettingControlReadRequest("K06", DEVICE_MENU_ITEM_AUTOMATION_SWITCH);
    sendSettingControlReadRequest("K12", DEVICE_MENU_ITEM_AUTOMATION_SWITCH);
    sendSettingControlReadRequest("K03", DEVICE_MENU_ITEM_AUTOMATION_SWITCH);
    sendSettingControlReadRequest("K13", DEVICE_MENU_ITEM_AUTOMATION_SWITCH);
    sendSettingControlReadRequest("K14", DEVICE_MENU_ITEM_AUTOMATION_SWITCH);
    sendSettingControlReadRequest("K15", DEVICE_MENU_ITEM_AUTOMATION_SWITCH);
    sendSettingControlReadRequest("SPRK_CNF", DEVICE_MENU_ITEM_AUTOMATION_SWITCH);

    sendSettingReadRequest("K02", "K02", CModBusDataUnit::ReadHoldingRegisters, 2, DEVICE_MENU_ITEM_AUTOMATION_SWITCH);
    sendSettingReadRequest("K04", "K04", CModBusDataUnit::ReadHoldingRegisters, 2, DEVICE_MENU_ITEM_AUTOMATION_SWITCH);
    sendSettingReadRequest("K10", "K10", CModBusDataUnit::ReadHoldingRegisters, 2, DEVICE_MENU_ITEM_AUTOMATION_SWITCH);
    sendSettingReadRequest("K01", "K01", CModBusDataUnit::ReadHoldingRegisters, 1, DEVICE_MENU_ITEM_AUTOMATION_SWITCH);
    sendSettingReadRequest("K09", "K09", CModBusDataUnit::ReadHoldingRegisters, 2, DEVICE_MENU_ITEM_AUTOMATION_SWITCH);
    sendSettingReadRequest("X22", "X22", CModBusDataUnit::ReadHoldingRegisters, 2, DEVICE_MENU_ITEM_AUTOMATION_SWITCH);
    sendSettingReadRequest("K08", "K08", CModBusDataUnit::ReadHoldingRegisters, 2, DEVICE_MENU_ITEM_AUTOMATION_SWITCH);
    sendSettingReadRequest("K05", "K05", CModBusDataUnit::ReadHoldingRegisters, 2, DEVICE_MENU_ITEM_AUTOMATION_SWITCH);
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

}
/*!
 * \brief ConfiguratorWindow::automationAVRRead
 *
 * Чтение автоматика АВР
 */
void ConfiguratorWindow::automationAVRRead()
{
    sendSettingControlReadRequest("M81", DEVICE_MENU_ITEM_AUTOMATION_AVR);
    sendSettingReadRequest("M84", "M84", CModBusDataUnit::ReadHoldingRegisters, 2, DEVICE_MENU_ITEM_AUTOMATION_AVR);
}
/*!
 * \brief ConfiguratorWindow::automationKCNRead
 *
 *  Чтение автоматика КЦН
 */
void ConfiguratorWindow::automationKCNRead()
{
    sendSettingReadRequest("K19", "K19", CModBusDataUnit::ReadHoldingRegisters, 2, DEVICE_MENU_ITEM_AUTOMATION_KCN);
}
/*!
 * \brief ConfiguratorWindow::automationAPVSignalStartRead
 *
 * Чтение состояний автоматики сигнала пуска
 */
void ConfiguratorWindow::automationAPVSignalStartRead()
{
    int addr = addressSettingKey("M86");

    CModBusDataUnit unit(static_cast<quint8>(m_serialPortSettings_window->deviceID()), CModBusDataUnit::ReadHoldingRegisters,
                         static_cast<quint16>(addr), QVector<quint16>() << 24);

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
    sendSettingControlReadRequest("M87", DEVICE_MENU_ITEM_AUTOMATION_APV);

    sendSettingReadRequest("K17", "K17", CModBusDataUnit::ReadHoldingRegisters, 1, DEVICE_MENU_ITEM_AUTOMATION_APV);
    sendSettingReadRequest("M88", "M88", CModBusDataUnit::ReadHoldingRegisters, 2, DEVICE_MENU_ITEM_AUTOMATION_APV);
    sendSettingReadRequest("M89", "M89", CModBusDataUnit::ReadHoldingRegisters, 2, DEVICE_MENU_ITEM_AUTOMATION_APV);

    QString list = "V04,V07,V10,V13,V19,N64,V22,N67,V44,V62,V65,V68,V81,V86,V90";

    sendSettingReadRequestVariableState("I69", list, "_1", DEVICE_MENU_ITEM_AUTOMATION_APV);
}
/*!
 * \brief ConfiguratorWindow::automationGroupRead
 *
 * Чтение группы автоматика
 */
void ConfiguratorWindow::automationGroupRead()
{
    automationSwitchRead(); // автоматики защиты Выключатель
    automationAPVRead();
    automationAVRRead();
    automationKCNRead();
}
/*!
 * \brief ConfiguratorWindow::automationGroupWrite
 *
 * Запись группы автоматика
 */
void ConfiguratorWindow::automationGroupWrite()
{
    automationSwitchWrite(); // запись автоматика Выключатель
    automationAPVWrite();
    automationAVRWrite();
    automationKCNWrite();

    saveDeviceSettings();
}
//----------------------------------------
void ConfiguratorWindow::purposeLedsRead()
{
    sendPurposeReadRequest("LED1", "LED2");
    sendPurposeReadRequest("LED3", "LED4");
    sendPurposeReadRequest("LED5", "LED6");
    sendPurposeReadRequest("LED7", "LED8");
}
//-----------------------------------------
void ConfiguratorWindow::purposeInputRead()
{
    sendPurposeDIReadRequest(512, 592);
    sendPurposeDIReadRequest(594, 676);
    sendPurposeDIReadRequest(768, 848);
    sendPurposeDIReadRequest(850, 932);
}
//-----------------------------------------
void ConfiguratorWindow::purposeRelayRead()
{
    sendPurposeReadRequest("DO1", "DO2");
    sendPurposeReadRequest("DO4", "DO5");
    sendPurposeReadRequest("DO6", "DO7");
    sendPurposeReadRequest("DO8", "DO9");
    sendPurposeReadRequest("DO10", "DO11");
    sendPurposeReadRequest("DO12", "DO13");
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
    CModBusDataUnit unit(static_cast<quint8>(m_serialPortSettings_window->deviceID()), CModBusDataUnit::ReadHoldingRegisters, 0x2000,
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
/*!
 * \brief ConfiguratorWindow::internalVariableRead
 *
 * Чтение состояния внутренних переменных
 */
void ConfiguratorWindow::internalVariableRead()
{
    if(!m_modbus->isConnected())
    {
        return;
    }

    // Чтение двумя запросами, т.к. для того чтобы вместить состояния всех переменных (439 в новой матрице) необходимо было
    // растянуть адреса, но они идут не линейно всего получается 14 32-битных переменных, т.е. 28 16-битных и могут вместить
    // до 448 переменных
    sendRequestRead(172, 24, INTERNAL_VARIABLES_READ, CModBusDataUnit::ReadInputRegisters); // читаем адреса 172-195
    sendRequestRead(562, 4, INTERNAL_VARIABLES_READ, CModBusDataUnit::ReadInputRegisters); // читаем адреса 562-565
}
//------------------------------------------------------
void ConfiguratorWindow::processReadJournals(bool state)
{
    JournalPtr journal = nullptr;

    journal = currentJournalWidget();

    if(journal)
    {
        if(state)
        {
            if(journal->filter().type() == CFilter::FilterDateType)
            {
                searchDateJournals(journal);
                return;
            }

            if(journal->filter().limitUpperValue() == 0)
            {
                m_popup->setPopupText(tr("Записей в устройстве для чтения нет"));
                m_popup->show();

                ui->pushButtonJournalRead->setChecked(false);

                return;
            }

            if(m_modbus->isConnected())
            {
                journal->initRead();
                setJournalShiftPtr(journal);

                startReadJournal(journal);
            }
        }
        else
        {
            QString journal_name = journalName(journal);
            int result = QMessageBox::question(this, tr("Отмена вычитки журнала %1").arg(journal_name),
                                                     tr("Прекратить чтение журнала %1").arg(journal_name));

            if(result == QMessageBox::Yes)
            {
//                journal->reset(); // отменяем чтение журнала
                journal->setReadState(false);
            }
        }
    }
}
//-------------------------------------------------------------
void ConfiguratorWindow::searchDateJournals(JournalPtr journal)
{
    if(!journal)
        return;

    uint8_t index_storage = 0xFF;

    if(journal == m_journal_event)
        index_storage = 1;
    else if(journal == m_journal_crash)
        index_storage = 2;
    else if(journal == m_journal_halfhour)
        index_storage = 3;
    else if(journal == m_journal_isolation)
        index_storage = 4;

    if(index_storage == 2 || index_storage == 3 || index_storage == 4) // пока не реализовано в прошивке
    {
        outLogMessage(tr("Поиск по дате в этих журналах не реализован!!!"));
    }

    if(index_storage == 0xFF) // если журнал не соответствует ни одному из заданных
    {
        journal->reset(); // отменяем чтение журнала

        return;
    }

    CModBusDataUnit::vlist_t val;

    QDate date = journal->filter().dateFrom();
    quint8 year = date.year() - 2000;
    quint8 month = date.month();
    quint8 day = date.day();

qInfo() << QString("Дата выборки: %1.%2.%3").arg(day).arg(month).arg(year);

    val << (quint16(index_storage << 8) | year) << quint16((month << 8) | day);

qInfo() << QString("Данные запроса: %1, %2, %3, %4").arg(index_storage).arg(year).arg(month).arg(day);

    CModBusDataUnit unit(static_cast<quint8>(m_serialPortSettings_window->deviceID()),
                         CModBusDataUnit::WriteMultipleRegisters, 0x302D, val);

    unit.setProperty(tr("REQUEST"), WRITE_JOURNAL_DATE_SEEK_SET_REQUEST);

    m_modbus->sendData(unit);
}
//-----------------------------------------------------------
void ConfiguratorWindow::startReadJournal(JournalPtr journal)
{
    m_timer_synchronization->stop(); // отключаем синхронизацию

    if(ui->checkboxCalibTimeout->isChecked()) // отключаем опрос расчетных величин, если было запущено
        chboxCalculateTimeoutStateChanged(false);

    m_time_process.start();

    m_journal_progress = new JournalProgress(journal->msgRead(), tr("Чтение журнала %1").arg(journalName(journal)), this);

    if(m_journal_progress)
    {
        connect(m_journal_progress, &JournalProgress::cancel, this, &ConfiguratorWindow::processReadJournals);
        m_journal_progress->show();
    }

    journalRead(journal);
}
/*!
 * \brief ConfiguratorWindow::processJournalDateSetControl
 *
 * Чтение результата поиска даты начала чтения в журнале
 */
void ConfiguratorWindow::processJournalDateSetControl()
{
    CModBusDataUnit unit(static_cast<quint8>(m_serialPortSettings_window->deviceID()),
                         CModBusDataUnit::ReadHoldingRegisters, 0x302D, 1);

    unit.setProperty(tr("REQUEST"), READ_JOURNAL_DATE_SEEK_SET_CONTROL);

    m_modbus->sendData(unit);
}
//--------------------------------------
void ConfiguratorWindow::processExport()
{
    DeviceMenuItemType index = menuIndex();

    if(index >= DEVICE_MENU_ITEM_JOURNALS_CRASHES && index <= DEVICE_MENU_ITEM_JOURNALS_ISOLATION)
        exportJournalToDb();
    else if(index == DEVICE_MENU_ITEM_SETTINGS_ITEM_WELLCOME_SCREEN || index == DEVICE_MENU_ITEM_SETTINGS_ITEM_LEDS ||
            index == DEVICE_MENU_ITEM_SETTINGS_ITEM_IO_MDVV01_INPUTS || index == DEVICE_MENU_ITEM_SETTINGS_ITEM_IO_MDVV01_RELAY ||
            index == DEVICE_MENU_ITEM_SETTINGS_ITEM_IO_MDVV02_INPUTS || index == DEVICE_MENU_ITEM_SETTINGS_ITEM_IO_MDVV02_RELAY)
    {
        exportPurposeToJSON();
    }
    else
    {
        showMessageBox(tr("Экспорт"), tr("Не выбран допустимый пункт меню"), QMessageBox::Warning);
    }
}
//--------------------------------------
void ConfiguratorWindow::processImport()
{
    DeviceMenuItemType index = menuIndex();

    if(index >= DEVICE_MENU_ITEM_JOURNALS_CRASHES && index <= DEVICE_MENU_ITEM_JOURNALS_ISOLATION)
        importJournalToTable();
    else if(index == DEVICE_MENU_ITEM_SETTINGS_ITEM_WELLCOME_SCREEN || index == DEVICE_MENU_ITEM_SETTINGS_ITEM_LEDS ||
            index == DEVICE_MENU_ITEM_SETTINGS_ITEM_IO_MDVV01_INPUTS || index == DEVICE_MENU_ITEM_SETTINGS_ITEM_IO_MDVV01_RELAY ||
            index == DEVICE_MENU_ITEM_SETTINGS_ITEM_IO_MDVV02_INPUTS || index == DEVICE_MENU_ITEM_SETTINGS_ITEM_IO_MDVV02_RELAY)
    {
        importPurposeFromJSON();
    }
    else
    {
        showMessageBox(tr("Импорт"), tr("Не выбран допустимый пункт меню"), QMessageBox::Warning);
    }
}
//-----------------------------------------------------------
void ConfiguratorWindow::readyReadData(CModBusDataUnit& unit)
{
    if(!unit.isValid())
    {
        noConnectMessage();
        return;
    }

    RequestType type = static_cast<RequestType>(unit.property(tr("REQUEST")).toInt());

    if(type == CALCULATE_TYPE)
    {
        if(showErrorMessage(tr("Чтение расчетных величин"), unit))
            return;

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
        if(!showErrorMessage(tr("Чтение уставок"), unit))
        {
            if(unit.property("VARIABLE").toString().isEmpty())
                displaySettingResponse(unit); // если не существует свойство VARIABLE, то значит стандартная обработка
            else
                displaySettingVariableResponse(unit); // иначе вывод состояния внутренней переменной
        }
    }
    else if(type == GENERAL_CONTROL_TYPE)
    {
        if(!showErrorMessage(tr("Чтение уставок"), unit))
            displaySettingControlResponce(unit);
    }
    else if(type == PURPOSE_OUT_TYPE)
    {
        if(!showErrorMessage(tr("Матрица привязок выходов"), unit))
            displayPurposeOutput(unit);
    }
    else if(type == PURPOSE_INPUT_TYPE || type == PURPOSE_INPUT_INVERSE_TYPE)
    {
        static QVector<quint16> input_list;
        static QVector<quint16> input_inverse_list;

        if(type == PURPOSE_INPUT_TYPE)
        {
            if(showErrorMessage(tr("Матрица привязок входов"), unit))
                return;

            if(input_list.isEmpty())
                input_list = unit.values();
            else
                input_list += unit.values();
        }
        else if(type == PURPOSE_INPUT_INVERSE_TYPE)
        {
            if(showErrorMessage(tr("Матрица привязок инверсий входов"), unit))
                return;

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
        JournalPtr journal = unit.property(tr("JOURNAL")).value<JournalPtr>();

        if(!journal)
            return;

        if(type == READ_JOURNAL_SHIFT_PTR)
        {

        }

        if(type == READ_JOURNAL_COUNT)
        {
            int count = static_cast<int>(static_cast<int>(unit[0] << 16) | static_cast<int>(unit[1]));

            CFilter &filter = journal->filter();
            filter.setRange(1, count);
            filter.setLimit(1, count);

            journal->widget()->header()->setTextDeviceCountMessages(0, filter.rangeMaxValue());
        }

        if(!showErrorMessage(tr("Чтение журнала"), unit) && type == READ_JOURNAL)
        {
            journal->receiver(unit.values());

            if(journal->isReadState())
            {
                float speed_kb = float((unit.values().count()*2)/1024.0f)/float(unit.elapsed()/1000.0f);
                journal->widget()->header()->setTextElapsedTime(QString("%1 КБ/сек.").
                                                                arg(QLocale::system().toString(speed_kb, 'f', 3)));
                if(m_journal_progress)
                {
                    m_journal_progress->setProgressValue(journal->msgCount());
                }

                journalRead(journal);
            }
            else
            {
                qDebug() << QString("Завершение чтения журнала.");
                journal->print();
                endJournalRead(journal);
            }
        }
        else if(type == READ_JOURNAL)
            endJournalRead(journal);
    }
    else if(type == WRITE_JOURNAL_DATE_SEEK_SET_REQUEST || type == READ_JOURNAL_DATE_SEEK_SET_CONTROL ||
            type == READ_JOURNAL_DATE_SEEK_COUNT || type == READ_JOURNAL_DATE_SHIFT_PTR)
    {
        static quint32 msg_count = 0xFFFFFFFF; // промежуточная переменная для хранения количества сообщений для чтения

        if(type == WRITE_JOURNAL_DATE_SEEK_SET_REQUEST)
        {
            msg_count = 0xFFFFFFFF;

            if(!showErrorMessage(tr("Установка даты чтения журнала"), unit))
            {
                QTimer::singleShot(250, this, processJournalDateSetControl);
            }
        }
        else if(type == READ_JOURNAL_DATE_SEEK_SET_CONTROL)
        {
            if(!showErrorMessage(tr("Контроль поиска даты чтения журнала"), unit))
            {
                quint8 res = quint8(unit[0] >> 8);

                if(res == 0x00) // поиск даты чтения журнала завершен успешно
                {
                    CModBusDataUnit unit(static_cast<quint8>(m_serialPortSettings_window->deviceID()),
                                         CModBusDataUnit::ReadInputRegisters, 52, 2);

                    unit.setProperty(tr("REQUEST"), READ_JOURNAL_DATE_SEEK_COUNT);

                    m_modbus->sendData(unit);
                }
                else if(res == 0x10) // поиск даты чтения журнала завершился ошибкой
                {
                    JournalPtr journal = currentJournalWidget();

                    if(journal)
                    {
                        endJournalRead(journal);
                    }

                    showMessageBox(tr("Поиск даты в журнале"), tr("Поиск даты начала записи в журнале завершен ошибкой!"), QMessageBox::Warning);

                    return;
                }
                else
                {
                    processJournalDateSetControl();
                }
            }
        }
        else if(type == READ_JOURNAL_DATE_SEEK_COUNT)
        {
            if(!showErrorMessage(tr("Чтение количества доступных сообщений по дате журнала"), unit))
            {
                if(unit.count() == 2)
                {
                    quint32 value = static_cast<quint32>(unit[1]) | static_cast<quint32>((unit[0] << 16));

                    msg_count = value;

                    JournalPtr journal = currentJournalWidget();

                    if(journal)
                    {
                        if(msg_count == 0)
                        {
                            showMessageBox(tr("Поиск даты в журнале"), tr("По выбранной дате в журнале ничего не найдено!"), QMessageBox::Information);
                            endJournalRead(journal);
                        }
                        else
                        {
                            CModBusDataUnit unit(quint8(m_serialPortSettings_window->deviceID()),
                                                 CModBusDataUnit::ReadHoldingRegisters, quint16(journal->addrShiftPtr()), 2);

                            unit.setProperty(tr("REQUEST"), READ_JOURNAL_DATE_SHIFT_PTR);

                            m_modbus->sendData(unit);
                        }
                    }
                }
            }
        }
        else if(type == READ_JOURNAL_DATE_SHIFT_PTR)
        {
            if(unit.count() == 2)
            {
                quint32 value = static_cast<quint32>(unit[1]) | static_cast<quint32>((unit[0] << 16));

                JournalPtr journal = currentJournalWidget();

                if(journal && msg_count != 0 && msg_count != 0xFFFFFFFF)
                {
                    setJournalShiftPtr(journal->addrShiftPtr(), value);
                    journal->initRead(msg_count, value);
                    startReadJournal(journal);
                }
            }
        }
    }
    else if(type == READ_SERIAL_NUMBER)
    {
        if(!showErrorMessage(tr("Чтение серийного номера"), unit))
            displayDeviceSerialNumber(unit.values());
    }
    else if(type == DATETIME_TYPE)
    {
        if(!showErrorMessage(tr("Чтение настроек Дата/Время"), unit))
            displayDateTime(unit);
    }
    else if(type == PORTECT_RESERVE_SIGNAL_START)
    {
        if(!showErrorMessage(tr("Чтение Защиты/Резервные/Сигналы Пуска"), unit))
            displayProtectReserveSignalStart(unit.values());
    }
    else if(type == PROTECTION_WORK_MODE_TYPE)
    {
        displayProtectionWorkMode(unit);
    }
    else if(type == MONITOR_PURPOSE_I11_I17_TYPE)
    {
        if(!showErrorMessage(tr("Мониторинг привязок К10/К11"), unit))
            displayMonitorI11_I17(unit);
    }
    else if(type == READ_OUTPUT_ALL)
    {
        if(!showErrorMessage(tr("Чтение состояний все выходы"), unit))
        displayOutputAllRead(unit);
    }
    else if(type == READ_INPUTS)
    {
        if(!showErrorMessage(tr("Чтение состояний входов"), unit))
            displayInputsRead(unit.values());
    }
    else if(type == READ_BLOCK_PROTECTION)
    {
        if(!showErrorMessage(tr("Чтение блокировок защит"), unit))
            displayBlockProtectionRead(unit.values());
    }
    else if(type == READ_DEBUG_INFO)
    {
        if(!showErrorMessage(tr("Чтение отладочной информации"), unit))
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
    else if(type == CALIBRATION_PARAMETER)
    {
        if(showErrorMessage(tr("Чтение параметра калибровок"), unit))
            return;

        emit calibrationDataIsReady(unit);
    }
    else if(type == CALIBRATION_CALCULATE_VALUE)
    {
        if(showErrorMessage(tr("Чтение расчетного значения калибровок"), unit))
            return;

        emit calibrationCalculateValue(unit);
    }
    else if(type == CALIBRATION_BRU_RESISTANCE_STATE_VARIABLE)
    {
        if(showErrorMessage(tr("Чтение состояния переменной БРУ по сопротивлению"), unit))
            return;

//        if(unit.address() == 173) // Только для теста
//            unit.setValues(QVector<quint16>() << 0);

        emit bruResistanceStateVariableRead(unit);
    }
    else if(type >= AMPLITUDE_READ_CH2 && type <= AMPLITUDE_READ_CH5)
    {
        if(showErrorMessage(tr("Чтение амплитуд по каналам"), unit))
            return;

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
    else if(type == INTERNAL_VARIABLES_READ)
    {
        static CModBusDataUnit::vlist_t variable_list = CModBusDataUnit::vlist_t();

        if(!showErrorMessage(tr("Чтение состояний внутренних переменных"), unit))
        {
            if(unit.count() == 24)
                variable_list = unit.values();
            else if(unit.count() == 4)
            {
                variable_list += unit.values();
                displayInternalVariables(variable_list);
                variable_list.clear();
            }
        }
    }
}
//------------------------------------
void ConfiguratorWindow::exitFromApp()
{
    if(m_isProject && m_is_set_change)
        saveProject();

    close();
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
    outApplicationEvent(error);
}
//---------------------------------------------------------
void ConfiguratorWindow::errorConnect(const QString& error)
{
    showMessageBox(tr("Ошибка подлкючения"), error, QMessageBox::Warning);
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
        m_containerIndicatorState->show();
    }
    else
    {
        m_containerIndicatorState->hide();

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
void ConfiguratorWindow::monitorI11I17Visiblity(bool state)
{
    if(state)
    {
        sendMonitorPurposeI11_I17Request();
        m_containerMonitorI11I17->show();
    }
    else
        m_containerMonitorI11I17->hide();

    ui->pushButtonMonitorI11_I17->setChecked(state);
}
//-----------------------------------------------------
void ConfiguratorWindow::outputAllVisiblity(bool state)
{
    if(state)
    {
        sendOutputAllRequest();
        m_containerOutputAll->show();
    }
    else
        m_containerOutputAll->hide();
}
//-------------------------------------------------
void ConfiguratorWindow::inputVisiblity(bool state)
{
    if(state)
    {
        sendInputStatesRequest();
        m_containerInputs->show();
    }
    else
        m_containerInputs->hide();
}
//-----------------------------------------------------
void ConfiguratorWindow::debugInfoVisiblity(bool state)
{
    if(state)
    {
        debugInfoRead();
        m_containerDebugInfo->show();
    }
    else
    {
        m_containerDebugInfo->hide();
    }
}
//------------------------------------------------------
void ConfiguratorWindow::statusInfoVisiblity(bool state)
{
    if(state)
    {
        readStatusInfo();
        m_containerStatusInfo->show();
    }
    else
    {
        m_containerStatusInfo->hide();
    }
}
//--------------------------------------------------------------
void ConfiguratorWindow::itemClicked(QTreeWidgetItem* item, int)
{
    if(!item)
        return;

    DeviceMenuItemType type = DeviceMenuItemType(item->type());

    int menu_item = m_menu_items.value(type, -1);

    if(type == DEVICE_MENU_ITEM_JOURNALS_ROOT) // выбрана вкладка Журналы
    {
        readJournalCount(m_journal_event);
        readJournalCount(m_journal_crash);
        readJournalCount(m_journal_halfhour);
        readJournalCount(m_journal_isolation);
    }

    if(type == DEVICE_MENU_ITEM_SETTINGS_ITEM_IN_ANALOG) // выбраны калибровки
    {
        if(accessCalibration())
        {
            ui->tabwgtMenu->setTabEnabled(TAB_READ_WRITE_INDEX, true);
            ui->tabwgtMenu->setCurrentIndex(TAB_READ_WRITE_INDEX);
            // открываем доступ к калибровкам
//            ui->tableWidgetSettingsAnalogGroupGeneral->setEnabled(true);
//            ui->pushButtonCalibrationRoll->setEnabled(true);
//            ui->tabWidgetCalibration->setEnabled(true);
            //...
        }
        else
        {
            int i = m_menu_items[DEVICE_MENU_ITEM_SETTINGS_ITEM_WELLCOME_SCREEN];
            ui->stwgtMain->setCurrentIndex(i);
            return;
        }
    }

    if(type == DEVICE_MENU_ITEM_SETTINGS_ITEM_IN_ANALOG ||
       type == DEVICE_MENU_ITEM_SETTINGS_ITEM_IN_ANALOG_CALIB ||
       type == DEVICE_MENU_ITEM_SETTINGS_ITEM_IN_ANALOG_GENERAL) // управление чтением расчетных величин для калибровок
    {
        CCalibrationController::CalibrationType type = static_cast<CCalibrationController::CalibrationType>(ui->tabWidgetCalibration->currentIndex());

        if(type == CCalibrationController::TYPE_BRU) // если выбраны калибровки БРУ
            type = static_cast<CCalibrationController::CalibrationType>(ui->tabWidgetCalibrationBRU->currentIndex() + 3);

        m_calibration_controller->setCalculateState(true, type);
    }
    else
        m_calibration_controller->setCalculateState(false, CCalibrationController::TYPE_NONE);

    if(menu_item != -1)
    {
        widgetStackIndexChanged(menu_item);
        ui->stwgtMain->setCurrentIndex(menu_item);
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
    protectionLeakGroupRead();
    protectionFrequencyGroupRead();
    protectionExternalGroupRead();
    protectionMotorGroupRead();
    protectionTemperatureGroupRead();
    protectionReserveGroupRead();
    protectionControlGroupRead();
    automationGroupRead();
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
        case DEVICE_MENU_ITEM_SETTINGS_ITEM_IN_ANALOG:
            inputAnalogGroupRead(); // чтение группы настроек "Аналоговые"
        break;

        case DEVICE_MENU_PROTECT_ITEM_CURRENT: // чтение группы защит МТЗ
            protectionMTZGroupRead();
        break;

        case DEVICE_MENU_PROTECT_ITEM_POWER: // чтение группы защит По напряжению
            protectionPowerGroupRead();
        break;

        case DEVICE_MENU_PROTECT_ITEM_LEAK: // чтение направленных защит
            protectionLeakGroupRead();
        break;

        case DEVICE_MENU_PROTECT_ITEM_FREQUENCY: // чтение защит по частоте
            protectionFrequencyGroupRead();
        break;

        case DEVICE_MENU_PROTECT_ITEM_EXTERNAL: // чтение Внешних защит
            protectionExternalGroupRead();
        break;

        case DEVICE_MENU_PROTECT_ITEM_MOTOR: // чтение защит для двигателя
            protectionMotorGroupRead();
        break;

        case DEVICE_MENU_PROTECT_ITEM_TEMPERATURE: // чтение защит по Температуре
            protectionTemperatureGroupRead();
        break;

        case DEVICE_MENU_PROTECT_ITEM_RESERVE: // чтение резервных защит
            protectionReserveGroupRead();
        break;

        case DEVICE_MENU_PROTECT_ITEM_CONTROL: // чтение группы защит Предварительного контроля
            protectionControlGroupRead();
        break;

        case DEVICE_MENU_ITEM_AUTOMATION_ROOT:
            automationGroupRead();
        break;

        case DEVICE_MENU_ITEM_AUTOMATION_SWITCH:
            automationSwitchRead();
        break;

        case DEVICE_MENU_ITEM_AUTOMATION_APV:
            automationAPVRead();
        break;

        case DEVICE_MENU_ITEM_AUTOMATION_AVR:
            automationAVRRead();
        break;

        case DEVICE_MENU_ITEM_AUTOMATION_KCN:
            automationKCNRead();
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

    int answer = showMessageBox(tr("Сохранение настроек утройства"), tr("Вы действительно хотите перезаписать настройки?"),
                                QMessageBox::Question);

    if(answer == QMessageBox::No)
        return;

    // запись всех настроек
    inputAnalogGroupWrite();
    protectionMTZGroupWrite();
    protectionPowerGroupWrite();
    protectionLeakGroupWrite();
    protectionFrequencyGroupWrite();
    protectionExternalGroupWrite();
    protectionMotorGroupWrite();
    protectionTemperatureGroupWrite();
    protectionReserveGroupWrite();
    protectionControlGroupWrite();
    automationGroupWrite();
}
//----------------------------------------
void ConfiguratorWindow::writeSetCurrent()
{
    if(!m_modbus->channel()->isOpen())
    {
        noConnectMessage();
        return;
    }

    DeviceMenuItemType index = menuIndex();

    switch(index)
    {
        case DEVICE_MENU_ITEM_SETTINGS_ITEM_IN_ANALOG:
            inputAnalogGroupWrite(); // запись группы настроек "Аналоговые"
        break;

        case DEVICE_MENU_PROTECT_ITEM_CURRENT: // запись группы защит МТЗ
            protectionMTZGroupWrite();
        break;

        case DEVICE_MENU_PROTECT_ITEM_POWER: // запись группы защит По напряжению
            protectionPowerGroupWrite();
        break;

        case DEVICE_MENU_PROTECT_ITEM_LEAK: // запись направленных защит
            protectionLeakGroupWrite();
        break;

        case DEVICE_MENU_PROTECT_ITEM_FREQUENCY: // запись защит по частоте
            protectionFrequencyGroupWrite();
        break;

        case DEVICE_MENU_PROTECT_ITEM_EXTERNAL: // запись Внешних защит
            protectionExternalGroupWrite();
        break;

        case DEVICE_MENU_PROTECT_ITEM_MOTOR: // запись защит для двигателя
            protectionMotorGroupWrite();
        break;

        case DEVICE_MENU_PROTECT_ITEM_TEMPERATURE: // запись защит по Температуре
            protectionTemperatureGroupWrite();
        break;

        case DEVICE_MENU_PROTECT_ITEM_RESERVE: // запись резервных защит
            protectionReserveGroupWrite();
        break;

        case DEVICE_MENU_PROTECT_ITEM_CONTROL: // запись группы защит Предварительного контроля
            protectionControlGroupWrite();
        break;

        case DEVICE_MENU_ITEM_AUTOMATION_ROOT:
            automationGroupWrite();
        break;

        case DEVICE_MENU_ITEM_AUTOMATION_SWITCH:
            automationSwitchWrite();
        break;

        case DEVICE_MENU_ITEM_AUTOMATION_APV:
            automationAPVWrite();
        break;

        case DEVICE_MENU_ITEM_AUTOMATION_AVR:
            automationAVRWrite();
        break;

        case DEVICE_MENU_ITEM_AUTOMATION_KCN:
            automationKCNWrite();
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
}
/*!
 * \brief ConfiguratorWindow::writeSetEditItem
 *
 * Записывает только настройки измененных пунктов меню
 */
void ConfiguratorWindow::writeSetEditItem()
{
    DeviceMenuItemType group = menuIndex();

    if(group == DEVICE_MENU_ITEM_NONE)
        return;

    CDeviceMenuTableWidget *table = groupMenuWidget(group);

    bool isEdit = false;

    QString workmode_current = "MTZ1,MTZ2,MTZ3,MTZ4,STARTING,IMIN"; // по току
    QString workmode_power = "UMAX1,UMAX2,UMIN1,UMIN2,3U0"; // по напряжению
    QString workmode_leak = "OZZ1,OZZ2,NZZ1,NZZ2,BRU,VACUUM"; // утечка
    QString workmode_frequency = "ACHR1,ACHR2,ACHR3"; // по частоте
    QString workmode_external = "ITEMARC,EXT1,EXT2,EXT3"; // внешние
    QString workmode_temperature = "TEMP1,TEMP2"; // по температуре
    QString workmode_reserve = "LEVEL1,LEVEL2"; // резервные

    for(int i = 0; i < table->rowCount(); i++)
    {
        QWidget* wgt = table->cellWidget(i, 1);

        if(!wgt)
            continue;

        QObjectList obj_list = wgt->children();

        for(QObject* obj: obj_list)
        {
            QString key;

            if(obj->isWidgetType())
            {
                if(obj->objectName().toUpper().contains("COMBOBOX"))
                {
                    CMenuComboBox *combobox = qobject_cast<CMenuComboBox*>(obj);

                    if(combobox && combobox->isEdit())
                    {
                        key = combobox->property("ITEM_KEY").toString();

                        QString workmode_list;

                        // проверка на редактирование режимов работы защит
                        if(workmode_current.contains(key))
                            workmode_list = workmode_current;
                        else if(workmode_power.contains(key))
                            workmode_list = workmode_power;
                        else if(workmode_leak.contains(key))
                            workmode_list = workmode_leak;
                        else if(workmode_frequency.contains(key))
                            workmode_list = workmode_frequency;
                        else if(workmode_external.contains(key))
                            workmode_list = workmode_external;
                        else if(workmode_temperature.contains(key))
                            workmode_list = workmode_temperature;
                        else if(workmode_reserve.contains(key))
                            workmode_list = workmode_reserve;

                        if(!workmode_list.isEmpty()) // если ключ относится к режимам работы защит
                            sendProtectionWorkModeRequest(workmode_list, FUNCTION_SAVE, group); // записываем режимы работ защит
                        else // иначе читаем уставку "Управление"
                            sendSettingControlWriteRequest(key, group);

                        combobox->resetIsEdit();
                        isEdit = true;
                    }
                }
                else if(obj->objectName().toUpper().contains("LINEEDIT"))
                {
                    CLineEdit *lineedit = qobject_cast<CLineEdit*>(obj);

                    if(lineedit && lineedit->isEdit())
                    {
                        key = lineedit->property("ITEM_KEY").toString();
                        isEdit = true;
                        sendSettingWriteRequest(key, key, group);
                        lineedit->resetIsEdit();
                    }
                }
            }
        }
    }

    if(!isEdit)
    {
        QMessageBox::information(this, tr("Запись уставок"), tr("Ни одна из настроек не была изменена!\n"
                                                                "Отмена записи"));
    }
    else
        saveDeviceSettings();
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
        m_treeWidgetDeviceMenu->expandAll();
    else
        m_treeWidgetDeviceMenu->collapseAll();
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
    QFont itemFont(m_treeWidgetDeviceMenu->font());
    itemFont.setPointSize(12);
    m_treeWidgetDeviceMenu->setFont(itemFont);
    m_treeWidgetDeviceMenu->header()->hide();

    QTreeWidgetItem* itemProtections = new QTreeWidgetItem(m_treeWidgetDeviceMenu, DEVICE_MENU_ITEM_PROTECTION_ROOT); // Защиты
    QTreeWidgetItem* itemAutomation  = new QTreeWidgetItem(m_treeWidgetDeviceMenu, DEVICE_MENU_ITEM_AUTOMATION_ROOT); // Автоматика
    QTreeWidgetItem* itemJournals    = new QTreeWidgetItem(m_treeWidgetDeviceMenu, DEVICE_MENU_ITEM_JOURNALS_ROOT); // Журналы
    QTreeWidgetItem* itemSettings    = new QTreeWidgetItem(m_treeWidgetDeviceMenu, DEVICE_MENU_ITEM_SETTINGS_ROOT); // Настройки

    itemProtections->setText(0, tr("Защиты"));
    itemAutomation->setText(0, tr("Автоматика"));
    itemJournals->setText(0, tr("Журналы"));
    itemSettings->setText(0, tr("Настройки"));

    // ЗАЩИТЫ
    QTreeWidgetItem* protectItemCurrent     = new QTreeWidgetItem(itemProtections, QStringList() << tr("По току"),
                                                                  DEVICE_MENU_PROTECT_ITEM_CURRENT); // по току
    QTreeWidgetItem* protectItemPower       = new QTreeWidgetItem(itemProtections, QStringList() << tr("По напряжению"),
                                                                  DEVICE_MENU_PROTECT_ITEM_POWER); // по напряжению
    QTreeWidgetItem* protectItemDirected    = new QTreeWidgetItem(itemProtections, QStringList() << tr("Утечка"),
                                                                  DEVICE_MENU_PROTECT_ITEM_LEAK); // направленные
    QTreeWidgetItem* protectItemFrequency   = new QTreeWidgetItem(itemProtections, QStringList() << tr("По частоте"),
                                                                  DEVICE_MENU_PROTECT_ITEM_FREQUENCY); // по частоте
    QTreeWidgetItem* protectItemExternal    = new QTreeWidgetItem(itemProtections, QStringList() << tr("Внешние"),
                                                                  DEVICE_MENU_PROTECT_ITEM_EXTERNAL); // внешние
    QTreeWidgetItem* protectItemTemperature = new QTreeWidgetItem(itemProtections, QStringList() << tr("По температуре"),
                                                                  DEVICE_MENU_PROTECT_ITEM_TEMPERATURE); // по температуре
    QTreeWidgetItem* protectItemReserve     = new QTreeWidgetItem(itemProtections, QStringList() << tr("Резервные"),
                                                                  DEVICE_MENU_PROTECT_ITEM_RESERVE); // резервные

    itemProtections->addChildren(QList<QTreeWidgetItem*>() << protectItemCurrent << protectItemPower << protectItemDirected <<
                                                              protectItemFrequency << protectItemExternal << protectItemTemperature <<
                                                              protectItemReserve);

    QTreeWidgetItem* automationItemSwitch = new QTreeWidgetItem(itemAutomation, QStringList() << tr("Выключатель"),
                                                                DEVICE_MENU_ITEM_AUTOMATION_SWITCH); // Автоматика->Выключатель
    QTreeWidgetItem* automationItemApv = new QTreeWidgetItem(itemAutomation, QStringList() << tr("АПВ"),
                                                             DEVICE_MENU_ITEM_AUTOMATION_APV); // Автоматика->АПВ
    QTreeWidgetItem* automationItemAvr = new QTreeWidgetItem(itemAutomation, QStringList() << tr("АВР"),
                                                             DEVICE_MENU_ITEM_AUTOMATION_AVR); // Автоматика->АВР
    QTreeWidgetItem* automationItemKcn = new QTreeWidgetItem(itemAutomation, QStringList() << tr("КЦН"),
                                                             DEVICE_MENU_ITEM_AUTOMATION_KCN); // Автоматика->КЦН

    itemAutomation->addChildren(QList<QTreeWidgetItem*>() << automationItemSwitch << automationItemApv << automationItemAvr <<
                                                             automationItemKcn);

    // ЖУРНАЛЫ
    QTreeWidgetItem* journalCrash     = new QTreeWidgetItem(itemJournals, QStringList() << tr("Аварий"),
                                                            DEVICE_MENU_ITEM_JOURNALS_CRASHES);
    QTreeWidgetItem* journalEvents    = new QTreeWidgetItem(itemJournals, QStringList() << tr("Событий"),
                                                            DEVICE_MENU_ITEM_JOURNALS_EVENTS);
    QTreeWidgetItem* journalHalfHour  = new QTreeWidgetItem(itemJournals, QStringList() << tr("Получасовок"),
                                                            DEVICE_MENU_ITEM_JOURNALS_HALF_HOURS);
    QTreeWidgetItem* journalIsolation = new QTreeWidgetItem(itemJournals, QStringList() << tr("Изоляции"),
                                                            DEVICE_MENU_ITEM_JOURNALS_ISOLATION);

    itemJournals->addChildren(QList<QTreeWidgetItem*>() << journalCrash << journalEvents << journalHalfHour << journalIsolation);

    // НАСТРОЙКИ
    QTreeWidgetItem* settingInputAnalog    = new QTreeWidgetItem(itemSettings, QStringList() << tr("Аналоговые входы"),
                                                                 DEVICE_MENU_ITEM_SETTINGS_ITEM_IN_ANALOG);
    QTreeWidgetItem* ioDSInputMDVV01       = new QTreeWidgetItem(itemSettings, QStringList() << tr("Дискретные входы"),
                                                                 DEVICE_MENU_ITEM_SETTINGS_ITEM_IO_MDVV01_INPUTS);
    QTreeWidgetItem* settingLeds           = new QTreeWidgetItem(itemSettings, QStringList() << tr("Светодиоды"),
                                                                 DEVICE_MENU_ITEM_SETTINGS_ITEM_LEDS);
    QTreeWidgetItem* ioRelayMDVV01         = new QTreeWidgetItem(itemSettings, QStringList() << tr("Реле"),
                                                                 DEVICE_MENU_ITEM_SETTINGS_ITEM_IO_MDVV01_RELAY);
    QTreeWidgetItem* ioProtectionCtrl      = new QTreeWidgetItem(itemSettings, QStringList() << tr("Блокировка защит"),
                                                                 DEVICE_MENU_ITEM_SETTINGS_ITEM_IO_PROTECTION);
    QTreeWidgetItem* settingCommunications = new QTreeWidgetItem(itemSettings, QStringList() << tr("Связь"),
                                                                 DEVICE_MENU_ITEM_SETTINGS_ITEM_COMMUNICATIONS);
    QTreeWidgetItem* settingDateTime       = new QTreeWidgetItem(itemSettings, QStringList() << tr("Дата и время"),
                                                                 DEVICE_MENU_ITEM_SETTINGS_ITEM_DATETIME);

    itemSettings->addChildren(QList<QTreeWidgetItem*>() << settingInputAnalog << ioDSInputMDVV01 << settingLeds << ioRelayMDVV01 <<
                                                           ioProtectionCtrl << settingCommunications << settingDateTime);

    m_treeWidgetDeviceMenu->addTopLevelItem(itemProtections);
    m_treeWidgetDeviceMenu->addTopLevelItem(itemAutomation);
    m_treeWidgetDeviceMenu->addTopLevelItem(itemJournals);
    m_treeWidgetDeviceMenu->addTopLevelItem(itemSettings);

    // заполнение карты меню устройства для доступа к настройкам при клике по пункту
    m_menu_items[DEVICE_MENU_ITEM_SETTINGS_ITEM_IN_ANALOG]         = 0;
    m_menu_items[DEVICE_MENU_PROTECT_ITEM_CURRENT]                 = 1;
    m_menu_items[DEVICE_MENU_PROTECT_ITEM_POWER]                   = 2;
    m_menu_items[DEVICE_MENU_PROTECT_ITEM_LEAK]                = 3;
    m_menu_items[DEVICE_MENU_PROTECT_ITEM_FREQUENCY]               = 4;
    m_menu_items[DEVICE_MENU_PROTECT_ITEM_EXTERNAL]                = 5;
    m_menu_items[DEVICE_MENU_PROTECT_ITEM_TEMPERATURE]             = 6;
    m_menu_items[DEVICE_MENU_PROTECT_ITEM_RESERVE]                 = 7;
    m_menu_items[DEVICE_MENU_ITEM_AUTOMATION_SWITCH]               = 8;
    m_menu_items[DEVICE_MENU_ITEM_AUTOMATION_APV]                  = 9;
    m_menu_items[DEVICE_MENU_ITEM_AUTOMATION_AVR]                  = 10;
    m_menu_items[DEVICE_MENU_ITEM_AUTOMATION_KCN]                  = 11;
    m_menu_items[DEVICE_MENU_ITEM_JOURNALS_CRASHES]                = 12;
    m_menu_items[DEVICE_MENU_ITEM_JOURNALS_EVENTS]                 = 13;
    m_menu_items[DEVICE_MENU_ITEM_JOURNALS_HALF_HOURS]             = 14;
    m_menu_items[DEVICE_MENU_ITEM_JOURNALS_ISOLATION]              = 15;
    m_menu_items[DEVICE_MENU_ITEM_MEASURES_INPUTS]                 = 16;
    m_menu_items[DEVICE_MENU_ITEM_SETTINGS_ITEM_COMMUNICATIONS]    = 17;
    m_menu_items[DEVICE_MENU_ITEM_SETTINGS_ITEM_DATETIME]          = 18;
    m_menu_items[DEVICE_MENU_ITEM_SETTINGS_ITEM_LEDS]              = 20;
    m_menu_items[DEVICE_MENU_ITEM_SETTINGS_ITEM_IO_MDVV01_INPUTS]  = 21;
    m_menu_items[DEVICE_MENU_ITEM_SETTINGS_ITEM_IO_MDVV01_RELAY]   = 22;
    m_menu_items[DEVICE_MENU_ITEM_SETTINGS_ITEM_WELLCOME_SCREEN]   = 23; // вывод окна приветствия
    m_menu_items[DEVICE_MENU_ITEM_PROTECTION_ROOT]                 = 23; // вывод окна приветствия
    m_menu_items[DEVICE_MENU_ITEM_AUTOMATION_ROOT]                 = 23; // вывод окна приветствия
    m_menu_items[DEVICE_MENU_ITEM_JOURNALS_ROOT]                   = 23; // вывод окна приветствия
    m_menu_items[DEVICE_MENU_ITEM_MEASURES_ROOT]                   = 23; // вывод окна приветствия
    m_menu_items[DEVICE_MENU_ITEM_SETTINGS_ROOT]                   = 23; // вывод окна приветствия
    m_menu_items[DEVICE_MENU_ITEM_SETTINGS_ITEM_IO_PROTECTION]     = 24;

    QStringList columns = QStringList() << tr("Имя") << tr("Параметр") << tr("Предел") << tr("Ед. изм.");

    ui->tableWidgetSettingsAnalogGroupGeneral->setColumns(columns);
    ui->tableWidgetProtectionGroupMTZ->setColumns(columns);
    ui->tableWidgetProtectionGroupPower->setColumns(columns);
    ui->tableWidgetProtectionGroupDirect->setColumns(columns);
    ui->tableWidgetProtectionGroupFrequency->setColumns(columns);
    ui->tableWidgetProtectionGroupExternal->setColumns(columns);
    ui->tableWidgetProtectionGroupTemperature->setColumns(columns);
    ui->tableWidgetProtectionGroupReserve->setColumns(columns);
    ui->tableWidgetAutomationSwitch->setColumns(columns);
    ui->tableWidgetAutomationAPV->setColumns(columns);
    ui->tableWidgetAutomationAVR->setColumns(columns);
    ui->tableWidgetAutomationKCN->setColumns(columns);

    CDeviceMenuTableWidget::group_t group;

    // группа аналоговые основные
    group = loadMenuGroup(tr("Основные"));
    ui->tableWidgetSettingsAnalogGroupGeneral->addGroup(group, true);

    // группа аналоговые калибровки "Ток"
    group = loadMenuGroup(tr("Ток"));
    ui->tableWidgetSettingsAnalogGroupGeneral->addGroup(group);

    // группа аналоговые калибровки "Напряжение АС"
    group = loadMenuGroup(tr("Напряжение AC"));
    ui->tableWidgetSettingsAnalogGroupGeneral->addGroup(group);

    // группа аналоговые калибровки "Напряжение DC"
    group = loadMenuGroup(tr("Напряжение DC"));
    ui->tableWidgetSettingsAnalogGroupGeneral->addGroup(group);

    // группа аналоговые калибровки "Сопротивление"
    group = loadMenuGroup(tr("Сопротивление"));
    ui->tableWidgetSettingsAnalogGroupGeneral->addGroup(group);

    // группа аналоговые калибровки "Угол сдвига фазы"
    group = loadMenuGroup(tr("Угол сдвига фазы"));
    ui->tableWidgetSettingsAnalogGroupGeneral->addGroup(group);

    connect(ui->tableWidgetSettingsAnalogGroupGeneral, &CDeviceMenuTableWidget::itemEdit, this, &ConfiguratorWindow::setChanged);

    // группа по току
    group = loadMenuGroup(tr("МТЗ1"));
    ui->tableWidgetProtectionGroupMTZ->addGroup(group, true);
    group = loadMenuGroup(tr("МТЗ2"));
    ui->tableWidgetProtectionGroupMTZ->addGroup(group);
    group = loadMenuGroup(tr("МТЗ3"));
    ui->tableWidgetProtectionGroupMTZ->addGroup(group);
    group = loadMenuGroup(tr("МТЗ4"));
    ui->tableWidgetProtectionGroupMTZ->addGroup(group);
    group = loadMenuGroup(tr("Пусковая"));
    ui->tableWidgetProtectionGroupMTZ->addGroup(group);
    group = loadMenuGroup(tr("Imin"));
    ui->tableWidgetProtectionGroupMTZ->addGroup(group);

    connect(ui->tableWidgetProtectionGroupMTZ, &CDeviceMenuTableWidget::itemEdit, this, &ConfiguratorWindow::setChanged);

    // группа по напряжению
    group = loadMenuGroup(tr("Umax1"));
    ui->tableWidgetProtectionGroupPower->addGroup(group, true);
    group = loadMenuGroup(tr("Umax2"));
    ui->tableWidgetProtectionGroupPower->addGroup(group);
    group = loadMenuGroup(tr("Umin1"));
    ui->tableWidgetProtectionGroupPower->addGroup(group);
    group = loadMenuGroup(tr("Umin2"));
    ui->tableWidgetProtectionGroupPower->addGroup(group);
    group = loadMenuGroup(tr("3U0"));
    ui->tableWidgetProtectionGroupPower->addGroup(group);

    connect(ui->tableWidgetProtectionGroupPower, &CDeviceMenuTableWidget::itemEdit, this, &ConfiguratorWindow::setChanged);

    // группа направленные - новое название "Утечка"
    group = loadMenuGroup(tr("ОЗЗ1"));
    ui->tableWidgetProtectionGroupDirect->addGroup(group, true);
    group = loadMenuGroup(tr("ОЗЗ2"));
    ui->tableWidgetProtectionGroupDirect->addGroup(group);
    group = loadMenuGroup(tr("НЗЗ1"));
    ui->tableWidgetProtectionGroupDirect->addGroup(group);
    group = loadMenuGroup(tr("НЗЗ2"));
    ui->tableWidgetProtectionGroupDirect->addGroup(group);
    group = loadMenuGroup(tr("БРУ"));
    ui->tableWidgetProtectionGroupDirect->addGroup(group);
    group = loadMenuGroup(tr("Вакуум"));
    ui->tableWidgetProtectionGroupDirect->addGroup(group);

    connect(ui->tableWidgetProtectionGroupDirect, &CDeviceMenuTableWidget::itemEdit, this, &ConfiguratorWindow::setChanged);

    // группа по частоте
    group = loadMenuGroup(tr("АЧР1"));
    ui->tableWidgetProtectionGroupFrequency->addGroup(group, true);
    group = loadMenuGroup(tr("АЧР2"));
    ui->tableWidgetProtectionGroupFrequency->addGroup(group);
    group = loadMenuGroup(tr("АЧР3"));
    ui->tableWidgetProtectionGroupFrequency->addGroup(group);

    connect(ui->tableWidgetProtectionGroupFrequency, &CDeviceMenuTableWidget::itemEdit, this, &ConfiguratorWindow::setChanged);

    // группа внешние
    group = loadMenuGroup(tr("Дуговая"));
    ui->tableWidgetProtectionGroupExternal->addGroup(group, true);
    group = loadMenuGroup(tr("Внешняя1"));
    ui->tableWidgetProtectionGroupExternal->addGroup(group);
    group = loadMenuGroup(tr("Внешняя2"));
    ui->tableWidgetProtectionGroupExternal->addGroup(group);
    group = loadMenuGroup(tr("Внешняя3"));
    ui->tableWidgetProtectionGroupExternal->addGroup(group);

    connect(ui->tableWidgetProtectionGroupExternal, &CDeviceMenuTableWidget::itemEdit, this, &ConfiguratorWindow::setChanged);

    // группа по температуре
    group = loadMenuGroup(tr("Температурная1"));
    ui->tableWidgetProtectionGroupTemperature->addGroup(group, true);
    group = loadMenuGroup(tr("Температурная2"));
    ui->tableWidgetProtectionGroupTemperature->addGroup(group);

    connect(ui->tableWidgetProtectionGroupTemperature, &CDeviceMenuTableWidget::itemEdit, this, &ConfiguratorWindow::setChanged);

    // группа резервные
    group = loadMenuGroup(tr("Уров1"));
    ui->tableWidgetProtectionGroupReserve->addGroup(group, true);
    group = loadMenuGroup(tr("Уров2"));
    ui->tableWidgetProtectionGroupReserve->addGroup(group);

    connect(ui->tableWidgetProtectionGroupReserve, &CDeviceMenuTableWidget::itemEdit, this, &ConfiguratorWindow::setChanged);

    // группа автоматики "Выключатель"
    group = loadMenuGroup(tr("Параметры ВКЛ"));
    ui->tableWidgetAutomationSwitch->addGroup(group, true);
    group = loadMenuGroup(tr("Параметры ОТКЛ"));
    ui->tableWidgetAutomationSwitch->addGroup(group);
    group = loadMenuGroup(tr("Контроль состояний"));
    ui->tableWidgetAutomationSwitch->addGroup(group);
    group = loadMenuGroup(tr("Блокировки"));
    ui->tableWidgetAutomationSwitch->addGroup(group);
    group = loadMenuGroup(tr("Схема внешнего управления"));
    ui->tableWidgetAutomationSwitch->addGroup(group);

    connect(ui->tableWidgetAutomationSwitch, &CDeviceMenuTableWidget::itemEdit, this, &ConfiguratorWindow::setChanged);

    // группа автоматики "АПВ"
    group = loadMenuGroup(tr("АПВ"));
    ui->tableWidgetAutomationAPV->addGroup(group, true);

    connect(ui->tableWidgetAutomationAPV, &CDeviceMenuTableWidget::itemEdit, this, &ConfiguratorWindow::setChanged);

    // группа автоматики "АВР"
    group = loadMenuGroup(tr("АВР"));
    ui->tableWidgetAutomationAVR->addGroup(group, true);

    connect(ui->tableWidgetAutomationAVR, &CDeviceMenuTableWidget::itemEdit, this, &ConfiguratorWindow::setChanged);

    // группа автоматики "КЦН"
    group = loadMenuGroup(tr("КЦН"));
    ui->tableWidgetAutomationKCN->addGroup(group, true);

    connect(ui->tableWidgetAutomationKCN, &CDeviceMenuTableWidget::itemEdit, this, &ConfiguratorWindow::setChanged);

    // формирование связей между отдельными ячейками
    // Объединение ячеек Датчик1 и Датчик2 Температуры1 с датчиками Температуры2
    CMenuComboBox* cboxTemp1 = qobject_cast<CMenuComboBox*>(groupMenuCellWidgetByName(ui->tableWidgetProtectionGroupTemperature,
                                                            QString("comboBoxM65"), 1));
    CMenuComboBox* cboxTemp2 = qobject_cast<CMenuComboBox*>(groupMenuCellWidgetByName(ui->tableWidgetProtectionGroupTemperature,
                                                            QString("comboBoxM66"), 1));
    CMenuComboBox* cboxTemp1_1 = qobject_cast<CMenuComboBox*>(groupMenuCellWidgetByName(ui->tableWidgetProtectionGroupTemperature,
                                                              QString("comboBoxM65_1"), 1));
    CMenuComboBox* cboxTemp2_1 = qobject_cast<CMenuComboBox*>(groupMenuCellWidgetByName(ui->tableWidgetProtectionGroupTemperature,
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
}
//-------------------------------------
void ConfiguratorWindow::initCellBind()
{
    QSqlQuery query(m_system_db);

    if(!query.exec("SELECT * FROM menu_item WHERE type = 'SET' OR type = 'MODIFY';"))
        return;

    while(query.next())
    {
        QString key          = query.value("key").toString();
        QString name         = query.value("description").toString();
        int     addr         = query.value("address").toInt();
        float   limit_min    = query.value("limit_min").toFloat();
        float   limit_max    = query.value("limit_max").toFloat();
        QString unit_meadure = query.value("unit_measure").toString();
        QString date_type    = query.value("data_type").toString();
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
        if(query.exec(QString("SELECT key, address, description FROM io_list WHERE type = '%1';").arg(type)))
        {
            while(query.next())
            {
                QString          key  = query.value(tr("key")).toString();
                int              addr = query.value(tr("address")).toInt();
                QString          desc = query.value(tr("description")).toString();
                QVector<QString> var_list;

                QSqlQuery query_var(QString("SELECT var_key FROM purpose WHERE io_key = '%1';").arg(key));

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
        ui->tablewgtLedPurpose->setProperty("NAME", tr("Светодиоды"));
        ui->tablewgtLedPurpose->setProperty("TYPE", "LED");
        QVector<QPair<QString, QString> > labels = loadLabelColumns("LED");
        initTable(ui->tablewgtLedPurpose, labels, group);
    }

    group = createVariableGroup("DO");

    if(!group.isEmpty())
    {
        ui->tablewgtRelayPurpose->setProperty("NAME", tr("Реле"));
        ui->tablewgtRelayPurpose->setProperty("TYPE", "RELAY");
        QVector<QPair<QString, QString> > labels = loadLabelColumns("RELAY");
        initTable(ui->tablewgtRelayPurpose, labels, group);
    }

    group = createVariableGroup("DI");

    if(!group.isEmpty())
    {
        ui->tablewgtDiscreteInputPurpose->setProperty("NAME", tr("Дискретные входы"));
        ui->tablewgtDiscreteInputPurpose->setProperty("TYPE", "INPUT");
        QVector<QPair<QString, QString> > labels = loadLabelColumns("INPUT");
        initTable(ui->tablewgtDiscreteInputPurpose, labels, group);
    }

    m_block_list = loadProtectionList();

    if(!m_block_list.isEmpty())
    {
        ui->tablewgtProtectionCtrl->setProperty("NAME", tr("Блокировка защит"));
        ui->tablewgtProtectionCtrl->setProperty("TYPE", "PROTECTION");
        initTableProtection(ui->tablewgtProtectionCtrl, m_block_list);
    }
    else
    {
        outLogMessage(tr("Ошибка чтения привязок для таблицы Управления Защитами"));
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
        outLogMessage(tr("Ошибка чтения типов журнала событий: %1").arg(query.lastError().text()));
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
            outLogMessage(tr("Ошибка чтения категорий журнала событий: %1").arg(query.lastError().text()));
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
                outLogMessage(tr("Ошибка чтения параметров журнала событий: %1").arg(query.lastError().text()));
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
        showMessageBox(title_msg, tr("Не удалось загрузить список доступных защит: %1").arg(query.lastError().text()), QMessageBox::Warning);
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
            showMessageBox(title_msg, tr("Не удалось загрузить переменные защит: %1").arg(query_item.lastError().text()), QMessageBox::Warning);
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
                    showMessageBox(title_msg, tr("Не удалось загрузить перечень списов вариантов переменных типа LIST: %1").arg(query.lastError().text()),
                                   QMessageBox::Warning);
                    break;
                }

                while(query.next())
                {
                    int number = query.value("number").toInt();

                    QSqlQuery query_set(m_system_db);

                    QVector<QString> set_list;

                    if(!query_set.exec(QString("SELECT name FROM protection_set_list WHERE set_number=%1;").arg(number)))
                    {
                        showMessageBox(title_msg, tr("Не удалось загрузить варианты переменных типа LIST: %1").arg(query_set.lastError().text()),
                                       QMessageBox::Warning);
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
        outApplicationEvent(tr("Не удалось загрузить список внутренних переменных: %1").arg(query.lastError().text()));
        return;
    }

    variable_list_t variable_list;

    while(query.next())
    {
        variable_list << variable_t({ query.value("key").toString(), query.value("name").toString(),
                                                                     query.value("description").toString() });
    }

    // загружаем выходы (светодиоды, реле и модифицируемые переменные)
    if(!query.exec("SELECT key, description FROM io_list WHERE type=\"RELAY\" OR type=\"LED\" OR type=\"EMPTY\";"))
    {
        outApplicationEvent(tr("Не удалось загрузить список выходов и модифицируемых переменных: %1").arg(query.lastError().text()));
        return;
    }

    io_list_t out_list;

    while(query.next())
    {
        out_list << io_t({ query.value("key").toString(), query.value("description").toString() });
    }

    // загружаем входы
    if(!query.exec("SELECT key, description FROM io_list WHERE type=\"INPUT\";"))
    {
        outApplicationEvent(tr("Не удалось загрузить список входов: %1").arg(query.lastError().text()));
        return;
    }

    io_list_t input_list;

    while(query.next())
    {
        input_list << io_t({ query.value("key").toString(), query.value("description").toString() });
    }

    // загружаем список расчетных величин из БД
    if(!query.exec("SELECT * FROM calc_value;"))
    {
        outApplicationEvent(tr("Не удалось загрузить список расчетных величин: %1").arg(query.lastError().text()));
        return;
    }

    calc_value_list_t calc_value_list;

    while(query.next())
    {
        calc_value_list << calc_value_t({ query.value("id").toInt(), query.value("name").toString(), query.value("sort_id").toInt(),
                                          query.value("first").toInt(), query.value("description").toString() });
    }

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
            QString name = query_halfhour.value("name").toString();
            QString meas = query_halfhour.value("measure").toString();

            columns << halfhour_item_t({ name, meas });
        }
    }
    else
    {
        outLogMessage(tr("Ошибка чтения переменных halfhour: %1").arg(query_halfhour.lastError().text()));
    }

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
        outLogMessage(tr("Ошибка чтения значений halfhour_val: %1").arg(query_halfhour.lastError().text()));

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
        outLogMessage(tr("Ошибка чтения кодов устройств: %1").arg(query.lastError().text()));
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
    QStringList eventJournalHeaders     = QStringList() << tr("№") << tr("ID") << tr("Дата") << tr("Время") << tr("Параметр") << tr("Категория") <<
                                                           tr("Тип");
    QStringList crashJournalHeaders     = QStringList() << tr("№") << tr("ID") << tr("Дата") << tr("Время") << tr("Защита");
    QStringList halfHourJournalHeaders  = QStringList() << tr("№") << tr("ID") << tr("Дата") << tr("Время") << tr("Тип записи") << tr("Время учета, сек");
    QStringList isolationJournalHeaders = QStringList() << tr("№") << tr("ID") << tr("Дата") << tr("Время") << tr("Тип измерения") << tr("Ra, кОм") <<
                                                           tr("Rb, кОм") << tr("Rc, кОм");

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

    ui->widgetJournalCrash->setTableColumnWidth(QVector<int>() << 75 << 100 << 100 << 175);
    ui->widgetJournalEvent->setTableColumnWidth(QVector<int>() << 50 << 100 << 100 << 100 << 200 << 300 << 100);
    ui->widgetJournalHalfHour->setTableColumnWidth(QVector<int>() << 50 << 100 << 100 << 100 << 200 << 300);
//    ui->widgetJournalIsolation->setTableColumnWidth(QVector<int>() << 50 << 100 << 100 << 200 << 200 << 200 << 200 << 200);

    ui->widgetJournalCrash->setVisibleProperty(CJournalWidget::CRASH_PROPERTY, true);
    ui->widgetJournalHalfHour->setVisibleProperty(CJournalWidget::HALFHOUR_PROPERTY , true);

    m_journal_event = new CJournal(0x08, 0x22, 0x300C, 0x1000, ui->widgetJournalEvent);
    m_journal_crash = new CJournal(0x80, 0x26, 0x3011, 0x2000, ui->widgetJournalCrash);
    m_journal_halfhour = new CJournal(0x20, 0x2A, 0x3016, 0x5000, ui->widgetJournalHalfHour);
    m_journal_isolation = new CJournal(0x10, 0x2E, 0x301B, 0x6000, ui->widgetJournalIsolation);
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

    if(query.exec("SELECT * FROM variable WHERE type_function=\"OFF\";"))
    {
        while(query.next())
        {
            int type_sort = query.value("type_sort").toInt();

            unit_t unit =
            {
                query.value("key").toString(),
                query.value("name").toString(),
                query.value("description").toString(),
                query.value("bit").toInt(),
                UNCHECKED
            };

            m_monitor_I11_I17_field[type_sort] = unit;
        }
    }
qDebug() << QString("Загрузка привязок по I11, I17: %1").arg(m_monitor_I11_I17_field.count());
    if(m_monitor_I11_I17_field.isEmpty())
    {
        outLogMessage(tr("Невозможно создать список привязок для переменных I11 и I17."));
        return;
    }

    QStringList rows, columns;

    for(int key: m_monitor_I11_I17_field.keys())
    {
        unit_t  unit = m_monitor_I11_I17_field[key];
        QString str  = QString("%1 (%2)").arg(unit.key).arg(unit.name);

        if(unit.key.toUpper() == "I11" || unit.key.toUpper() == "I17")
            rows << str;

        if(str.count() > 22)
        {
            str  = str.remove(22, str.count() - 22);
            str += "...)";
        }

        columns << str;
    }

    m_monitor_purpose_window->setHeaders(rows, columns);
}
//--------------------------------------
void ConfiguratorWindow::initOutputAll()
{
    ui->pushButtonOutputAll->setProperty("TYPE", "OUTPUTALL");

    QSqlQuery query(m_system_db);

    QStringList list;

    if(query.exec("SELECT description FROM io_list WHERE type=\'RELAY\';"))
    {
        while(query.next())
        {
            list << query.value("description").toString();
        }
    }

    if(query.exec("SELECT description FROM io_list WHERE type=\'LED\';"))
    {
        while(query.next())
        {
            list << query.value("description").toString();
        }
    }

    if(query.exec("SELECT key, description FROM io_list WHERE type=\'MODIFY\';"))
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

    if(query.exec("SELECT description FROM io_list WHERE type=\'INPUT\';"))
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
        outLogMessage(tr("Ошибка чтения базы данных \"Статус\": %1").arg(query.lastError().text()));
}
/*!
 * \brief ConfiguratorWindow::initDebugVariables
 *
 * Инициализация вывода отладочной информации о внутренних переменных
 * (формирование чекбоксов для каждой переменной по группам)
 */
void ConfiguratorWindow::initDebugVariables()
{
    if(m_debug_var_window)
    {
        if(m_debug_var_window->isHidden())
            m_debug_var_window->show();

        internalVariableRead();

        return;
    }

    m_debug_var_window = new CWidget(this);
    m_debug_var_window->setWindowTitle(tr("Состояния внутренних переменных"));
    m_debug_var_window->setWindowFlag(Qt::Window);
    m_debug_var_window->setWindowState(Qt::WindowMaximized);
    m_debug_var_window->setMinimumSize(minimumSizeHint());

    connect(m_debug_var_window, &CWidget::read, this, &ConfiguratorWindow::internalVariableRead);
    connect(m_debug_var_window, &CWidget::pressKey, this, &ConfiguratorWindow::internalVariablePressKey);

    group_t group = createVariableGroup("");

    if(group.isEmpty())
        return;

    const int MAX_CHECKBOX_COUNT = 40;
    const int SPAISING = 3;

    QHBoxLayout* hlayout  = new QHBoxLayout;
    QVBoxLayout* vlayout  = new QVBoxLayout;
    hlayout->setAlignment(Qt::AlignRight|Qt::AlignTop);
    vlayout->setAlignment(Qt::AlignHCenter|Qt::AlignTop);
    vlayout->setSpacing(SPAISING*3);

    int var_group_count = 0;
    int group_count = 0;

    for(const auto& group_item: group)
    {
        QGroupBox*   groupBox        = new QGroupBox(group_item.name.toUpper(), this);
        QVBoxLayout* vlayoutCheckbox = new QVBoxLayout;
        groupBox->setStyleSheet("QGroupBox { font-weight: bold; }");
        groupBox->setAlignment(Qt::AlignCenter);
        groupBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        vlayoutCheckbox->setSpacing(SPAISING);

        int checkbox_count = 0;
        bool isGroup = true;

        group_count++;

        for(const auto& var: group_item.var_list)
        {
            CCheckBoxInternalVariable* checkBox = new CCheckBoxInternalVariable(QString("%1 (%2)").arg(var.key).arg(var.name), this);
            QFont tfont = checkBox->font();
            tfont.setPointSize(8);
            checkBox->setObjectName(QString("checkBox%1").arg(var.key));
            checkBox->setToolTip(((var.description.isEmpty())?var.name:var.description));
            checkBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
            checkBox->setFont(tfont);

            vlayoutCheckbox->addWidget(checkBox);
            var_group_count++;
            checkbox_count++;

            m_internal_variable_list[var.bit] = checkBox;

            int count = MAX_CHECKBOX_COUNT;

            if(vlayout && vlayout->count() > 1)
            {
                count = MAX_CHECKBOX_COUNT - vlayout->count()*2;
            }

            if(checkbox_count >= count || var_group_count >= count)
            {
                groupBox->setLayout(vlayoutCheckbox);
                vlayout->addWidget(groupBox);
                hlayout->addLayout(vlayout);

                if(checkbox_count == group_item.var_list.count())
                    isGroup = false;

                groupBox = new QGroupBox(group_item.name.toUpper(), this);
                groupBox->setStyleSheet("QGroupBox { font-weight: bold; }");
                groupBox->setAlignment(Qt::AlignCenter);
                groupBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

                vlayoutCheckbox = new QVBoxLayout;
                vlayoutCheckbox->setSpacing(SPAISING);

                vlayout = new QVBoxLayout;
                vlayout->setAlignment(Qt::AlignHCenter|Qt::AlignTop);
                vlayout->setSpacing(SPAISING*3);

                checkbox_count = var_group_count = 0;
            }
        }

        if(isGroup)
        {
            groupBox->setLayout(vlayoutCheckbox);
            vlayout->addWidget(groupBox);
            hlayout->addLayout(vlayout);
        }
    }

    QScrollArea* scrollArea       = new QScrollArea(m_debug_var_window);
    QWidget*     widgetContents   = new QWidget(scrollArea);
    QVBoxLayout* scrollAreaLayout = new QVBoxLayout;

    widgetContents->setLayout(hlayout);
    scrollArea->setWidget(widgetContents);
    scrollAreaLayout->addWidget(scrollArea);
    m_debug_var_window->setLayout(scrollAreaLayout);
    m_debug_var_window->show();

    internalVariableRead();
}
//--------------------------------------
void ConfiguratorWindow::authorization()
{
    if(m_debug_var_window)
    {
        if(m_debug_var_window->isHidden())
            initDebugVariables();

        return;
    }

    QStringList logins = loadLoginList();

    if(logins.isEmpty())
        return;

    CUserDialog* userDialog = new CUserDialog(logins, this);

    int answer = userDialog->exec();

    if(answer == QDialog::Accepted)
    {
        CUserDialog::user_t usr = userDialog->user();

        if(!usr.password.isEmpty())
        {
            QString pass = loadUserPassword(usr.login);

            if(usr.password.toUpper() == pass.toUpper())
                initDebugVariables();
            else
            {
                m_popup->setPopupText(tr("Ошибка: пароль неправильный!"));
                outLogMessage(tr("Ошибка: пароль неправильный!"));
                m_popup->show();
            }
        }
        else
        {
            m_popup->setPopupText(tr("Ошибка: пароль не может быть пустым."));
            outLogMessage(tr("Ошибка: пароль не может быть пустым."));
            m_popup->show();
        }
    }

    delete userDialog;
    userDialog = nullptr;
}
/*!
 * \brief ConfiguratorWindow::internalVariableSetInterval
 *
 * Настйройка интервала опроса состояний внутренних входов
 */
void ConfiguratorWindow::internalVariableSetInterval()
{

}
/*!
 * \brief ConfiguratorWindow::internalVariablePressKey
 * \param isAlt Состояние модификатора Alt
 * \param isCtrl Состояние модификатора Ctrl
 * \param key Нажатая кнопка
 *
 * Обработчик нажатия клавиш в окне состояния внутренних переменных
 */
void ConfiguratorWindow::internalVariablePressKey(bool isAlt, bool isCtrl, int key)
{
    if(isAlt && isCtrl && (key & Qt::Key_Backspace))
    {
        QStringList logins = loadLoginList();

        if(!logins.isEmpty())
        {
            CUserDialog* editPassDialog = new CUserDialog(logins, this);
            editPassDialog->setPasswordMode(CUserDialog::EDIT_MODE);

            if(editPassDialog->exec() == QDialog::Accepted)
            {
                CUserDialog::user_t currentUser = editPassDialog->currentUser();
                CUserDialog::user_t newUser     = editPassDialog->user();

                if(!currentUser.password.isEmpty() && !newUser.password.isEmpty() &&
                    currentUser.password.toUpper() != newUser.password.toUpper())
                {
                    QString currentPass = loadUserPassword(currentUser.login);

                    if(!currentPass.isEmpty())
                    {
                        if(currentPass.toUpper() == currentUser.password.toUpper())
                        {
                            QSqlQuery query(m_system_db);

                            if(!query.exec(QString("UPDATE user SET pass=\"%1\" WHERE login=\"%2\";").arg(newUser.password).
                                           arg(newUser.login)))
                            {
                                m_popup->setPopupText(tr("Ошибка: не удалось изменить пароль в БД!"));
                                outApplicationEvent(tr("Ошибка: не удалось изменить пароль в БД!"));
                                m_popup->show();
                            }
                            else
                            {
                                m_popup->setPopupText(tr("Пароль успешно изменен!"));
                                outApplicationEvent(tr("Пароль успешно изменен!"));
                                m_popup->show();
                            }
                        }
                        else
                        {
                            m_popup->setPopupText(tr("Ошибка: текущий пароль неправильный!"));
                            outApplicationEvent(tr("Ошибка: текущий пароль неправильный!"));
                            m_popup->show();
                        }
                    }
                }
            }
        }
    }
    else if(isAlt && key == Qt::Key_U)
    {
        internalVariableRead();
    }
}
/*!
 * \brief ConfiguratorWindow::calibrationRoll
 * \param state состояния виджета калибровок
 *
 * Разворачивание/Сворачивание виджета калибровок
 */
void ConfiguratorWindow::calibrationRoll(bool state)
{
    ui->tabWidgetCalibration->setVisible(state);
    ui->pushButtonCalibrationRoll->setChecked(state);
    ui->tableWidgetSettingsAnalogGroupGeneral->setVisible(!state);
    ui->pushButtonCalibrationRoll->setText((state)?tr("Свернуть калибровки"):tr("Развернуть калибровки"));
}
//------------------------------------------------
void ConfiguratorWindow::applicationCloseProcess()
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

    outLogMessage(tr("Завершение работы программы."));

    if(m_system_db.isOpen())
        m_system_db.close();

    if(m_journal_crash)
    {
        delete m_journal_crash;
        m_journal_crash = nullptr;
    }

    if(m_journal_event)
    {
        delete m_journal_event;
        m_journal_event = nullptr;
    }

    if(m_journal_halfhour)
    {
        delete m_journal_halfhour;
        m_journal_halfhour = nullptr;
    }

    if(m_journal_isolation)
    {
        delete m_journal_isolation;
        m_journal_isolation = nullptr;
    }

    delete m_modbus;
    m_modbus = nullptr;

    close();
}
//--------------------------------------------------------
void ConfiguratorWindow::calibrationTypeChanged(int index)
{
    QTabWidget *tabWidget = qobject_cast<QTabWidget*>(sender());

    if(tabWidget)
    {
        CCalibrationController::CalibrationType type = CCalibrationController::TYPE_NONE;

        if(tabWidget == ui->tabWidgetCalibration)
        {
            type = static_cast<CCalibrationController::CalibrationType>(index);

            if(type == CCalibrationController::TYPE_BRU)
                type = static_cast<CCalibrationController::CalibrationType>(ui->tabWidgetCalibrationBRU->currentIndex() + 3);
        }
        else if(tabWidget == ui->tabWidgetCalibrationBRU)
        {
            type = static_cast<CCalibrationController::CalibrationType>(ui->tabWidgetCalibrationBRU->currentIndex() + 3);
        }

        if(type != CCalibrationController::TYPE_NONE)
            m_calibration_controller->setCalculateState(true, type);
    }
}
//----------------------------------------------------------------------------
void ConfiguratorWindow::bruResistanceStateVariableSend(CModBusDataUnit &unit)
{
    unit.setID(quint8(m_serialPortSettings_window->deviceID()));
    unit.setProperty("REQUEST", CALIBRATION_BRU_RESISTANCE_STATE_VARIABLE);

    if(m_modbus->isConnected())
        m_modbus->sendData(unit);
}
//----------------------------------------------
void ConfiguratorWindow::containerVisibleState()
{
    ui->pushButtonDeviceMenu->setChecked(m_containerWidgetDeviceMenu->isVisible());
    ui->pushButtonCalculateVarible->setChecked(m_containerWidgetVariable->isVisible());
    ui->pushButtonDebugInfo->setChecked(m_containerDebugInfo->isVisible());
    ui->pushButtonInputs->setChecked(m_containerInputs->isVisible());
    ui->pushButtonIndicatorStates->setChecked(m_containerIndicatorState->isVisible());
    ui->pushButtonOutputAll->setChecked(m_containerOutputAll->isVisible());
    ui->pushButtonMonitorI11_I17->setChecked(m_containerMonitorI11I17->isVisible());
    ui->pushButtonStatusInfo->setChecked(m_containerStatusInfo->isVisible());
    ui->pushButtonTerminal->setChecked(m_containerTerminalModbus->isVisible());
    ui->pushButtonMessageEvent->setChecked(m_containerWidgetMessage->isVisible());
}
//-----------------------------------
void ConfiguratorWindow::setChanged()
{
    if(!m_is_set_change)
        m_is_set_change = true;
}
//----------------------------------------
void ConfiguratorWindow::connectSystemDb()
{
    m_system_db = QSqlDatabase::addDatabase("QSQLITE");
    m_system_db.setDatabaseName("db/system.db");

    if(!m_system_db.open())
    {
        showMessageBox(tr("Системная база данных"), tr("Невозможно открыть системную базу данных: %1").arg(m_system_db.lastError().text()),
                       QMessageBox::Critical);
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
        showMessageBox(tr("База данных журнала событий"), tr("Невозможно открыть базу данных журнала событий: %1").arg(db->lastError().text()),
                       QMessageBox::Critical);
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

    connect(model, &CMatrixPurposeModel::dataIsChanged, this, &ConfiguratorWindow::setChanged);

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
        int text_width = int(fm_table.width(item.name)*1.2f);

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

    m_is_set_change = false;
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

    connect(model, &CMatrixPurposeModel::dataIsChanged, this, &ConfiguratorWindow::setChanged);

    m_is_set_change = false;
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
//------------------------------------
void ConfiguratorWindow::initWindows()
{
    m_treeWidgetDeviceMenu   = new QTreeWidget(this);
    m_variableWidget         = new CVariableWidget(this);
    m_event_window           = new CTerminalWindow(this);
    m_terminal_modbus        = new CTerminal(this);
    m_output_window          = new CIndicatorState(this);
    m_monitor_purpose_window = new CMonitorPurpose(tr("Монитор привязок по I11/I17"), this);
    m_outputall_window       = new COutputAll(tr("Все выходы"), this);
    m_inputs_window          = new COutputAll(tr("Входы"), this);
    m_debuginfo_window       = new CDebugInfo(tr("Отладочная информация"), this);
    m_status_window          = new CStatusInfo(this);

    // инициализация панели расчетных величин
    m_variableWidget->init(m_system_db);
    m_variableWidget->setProperty("TYPE", "VARIABLE");
    m_variableWidget->setObjectName("panelVariable");
    m_containerWidgetVariable = new CContainerWidget(tr("Панель измерений"), m_variableWidget,
                                                     CContainerWidget::AnchorType::AnchorDockWidget, this);
    m_containerWidgetVariable->setSuperParent(this);
    m_containerWidgetVariable->setHeaderBackground(QColor(190, 190, 190));
    m_containerWidgetVariable->setSide(CDockPanelItemCtrl::DirRight);
    m_containerWidgetVariable->setName("VARIABLE");
    m_containerWidgetVariable->setDefaultSize(QSize(600, 720));

    // инициализация панели меню
    m_treeWidgetDeviceMenu->setProperty("TYPE", "DEVICE_MENU");
    m_treeWidgetDeviceMenu->setObjectName("panelDeviceMenu");
    m_containerWidgetDeviceMenu = new CContainerWidget(tr("Меню устройства"), m_treeWidgetDeviceMenu,
                                                       CContainerWidget::AnchorType::AnchorDockWidget, this);
    m_containerWidgetDeviceMenu->setSuperParent(this);
    m_containerWidgetDeviceMenu->setButtonFunctionState(true);
    m_containerWidgetDeviceMenu->setHeaderBackground(QColor(190, 190, 190));
    m_containerWidgetDeviceMenu->setSide(CDockPanelItemCtrl::DirLeft);
    m_containerWidgetDeviceMenu->setName("DEVICE_MENU");
    m_containerWidgetDeviceMenu->setDefaultSize(QSize(500, 720));
    connect(m_containerWidgetDeviceMenu->buttonFunction(), &QToolButton::clicked, this, &ConfiguratorWindow::expandItemTree);

    // инициализация панели сообщений
    m_event_window->setObjectName("terminalWindowEvent");
    m_containerWidgetMessage = new CContainerWidget(tr("События"), m_event_window, CContainerWidget::AnchorType::AnchorDockWidget, this);
    m_containerWidgetMessage->setHeaderBackground(QColor(190, 190, 190));
    m_containerWidgetMessage->setSuperParent(this);
    m_containerWidgetMessage->setSide(CDockPanelItemCtrl::DirBottom);
    m_containerWidgetMessage->setName("EVENT_MESSAGE");
    m_containerWidgetMessage->setDefaultSize(QSize(700, 500));

    // инициализация терминала MODBUS
    m_terminal_modbus->setObjectName("terminalModbus");
    m_containerTerminalModbus = new CContainerWidget(tr("Терминал"), m_terminal_modbus, CContainerWidget::AnchorType::AnchorDockWidget, this);
    m_containerTerminalModbus->setHeaderBackground(QColor(190, 190, 190));
    m_containerTerminalModbus->setSuperParent(this);
    m_containerTerminalModbus->setSide(CDockPanelItemCtrl::DirBottom);
    m_containerTerminalModbus->setName("TERMINAL");
    m_containerTerminalModbus->setDefaultSize(QSize(700, 500));

    // инициализация окна простмотра состояний светодиодов и реле
    m_containerIndicatorState = new CContainerWidget(tr("Состояние выходов"), m_output_window, CContainerWidget::AnchorType::AnchorFree,
                                                     this);
    m_containerIndicatorState->setObjectName("InputIndicatorStates");
    m_containerIndicatorState->setName("INDICATOR_STATE");
    m_containerIndicatorState->setSuperParent(this);
    m_containerIndicatorState->setHeaderBackground(QColor(190, 190, 190));
    m_containerIndicatorState->setDefaultSize(QSize(550, 650));
    m_containerIndicatorState->hide();

    // инициализация окна просмотра привязок I11 и I17
    m_containerMonitorI11I17 = new CContainerWidget(tr("Монитор привязок по I11/I17"), m_monitor_purpose_window,
                                                    CContainerWidget::AnchorType::AnchorFree, this);
    m_containerMonitorI11I17->setObjectName("Monitor_I11_I17");
    m_containerMonitorI11I17->setName("MONITOR_I11_I17");
    m_containerMonitorI11I17->setSuperParent(this);
    m_containerMonitorI11I17->setHeaderBackground(QColor(190, 190, 190));
    m_containerMonitorI11I17->setDefaultSize(QSize(400, 720));
    m_containerMonitorI11I17->hide();

    // инициализация окна просмотра состояний всех выходов
    m_containerOutputAll = new CContainerWidget(tr("Все выходы"), m_outputall_window, CContainerWidget::AnchorType::AnchorFree, this);
    m_containerOutputAll->setObjectName("OutputAll");
    m_containerOutputAll->setName("OUTPUT_ALL");
    m_containerOutputAll->setSuperParent(this);
    m_containerOutputAll->setHeaderBackground(QColor(190, 190, 190));
    m_containerOutputAll->setDefaultSize(QSize(500, 550));
    m_containerOutputAll->hide();

    // инициализация окна просмотра состояний выходов
    m_containerInputs = new CContainerWidget(tr("Входы"), m_inputs_window, CContainerWidget::AnchorType::AnchorFree, this);
    m_containerInputs->setObjectName("Inputs");
    m_containerInputs->setName("INPUTS");
    m_containerInputs->setSuperParent(this);
    m_containerInputs->setHeaderBackground(QColor(190, 190, 190));
    m_containerInputs->setDefaultSize(QSize(500, 550));
    m_containerInputs->hide();

    // инициализация окна отладочной информации
    m_containerDebugInfo = new CContainerWidget(tr("Отладочная информация"), m_debuginfo_window, CContainerWidget::AnchorType::AnchorFree, this);
    m_containerDebugInfo->setObjectName("DebugInfo");
    m_containerDebugInfo->setName("DEBUG_INFO");
    m_containerDebugInfo->setSuperParent(this);
    m_containerDebugInfo->setHeaderBackground(QColor(190, 190, 190));
    m_containerDebugInfo->setDefaultSize(QSize(800, 650));
    m_containerDebugInfo->hide();

    // инициализация окна информации о статусах
    m_containerStatusInfo = new CContainerWidget(tr("Информация о статусах"), m_status_window, CContainerWidget::AnchorType::AnchorFree, this);
    m_containerStatusInfo->setObjectName("StatusInfo");
    m_containerStatusInfo->setName("STATUS_INFO");
    m_containerStatusInfo->setSuperParent(this);
    m_containerStatusInfo->setHeaderBackground(QColor(190, 190, 190));
    m_containerStatusInfo->setDefaultSize(QSize(950, 400));
    m_containerStatusInfo->hide();
}
//----------------------------------------------------------------------
void ConfiguratorWindow::displayCalculateValues(QVector<quint16> values)
{
    if(!m_containerWidgetVariable)
        return;

    CVariableWidget* varWidget = qobject_cast<CVariableWidget*>(m_containerWidgetVariable->widget());

    if(varWidget)
    {
        varWidget->setData(values);
    }

    m_calculate_buffer.clear();
}
//-------------------------------------------------------------
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
    bool                    isCalibration = unit.property("CALIBRATION").toBool();

    QStringList listCalibrationKey = QStringList() << "K3I0" << "KIA" << "KIB" << "KIC" << "KUA" << "KUB" << "KUC" <<
                                                      "KUAB" << "KUBC" << "KUCA" << "K3U0R" << "K3U0S" << "K3U0T" <<
                                                      "KUADC" << "AUADC" << "KUBDC" << "AUBDC" << "KUCDC" << "AUCDC" <<
                                                      "KUMDC" << "AUMDC" << "KRA" << "ARA" << "KRB" << "ARB" <<
                                                      "KRC" << "ARC" << "KY01T" << "KY02T" << "KY03T" << "KY01R" <<
                                                      "KY02R" << "KY03R" << "KY04R" << "KY04S" << "KY04T";

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
        CLineEdit* lineEdit = qobject_cast<CLineEdit*>(groupMenuCellWidgetByName(table, nameWgt, 1));

        if(!lineEdit)
        {
            nameWgt += "_1";
            lineEdit = qobject_cast<CLineEdit*>(groupMenuCellWidgetByName(table, nameWgt, 1));

            if(!lineEdit)
            {
                index += 2;
                continue;
            }
        }

        QString str;

        if(unit.count() > 1)
        {
            quint16 val1 = unit.values().at(index + 1);
            quint16 val2 = unit.values().at(index);

            value.w[0] = val1;
            value.w[1] = val2;

            int zero_num = 2;

            if(listCalibrationKey.contains(first))
                zero_num = 6;

            str = QLocale::system().toString(value.f, 'f', zero_num);
        }
        else
            str = QString::number(unit[0]);

        if(!str.isEmpty())
        {
            lineEdit->setText(str);
            lineEdit->resetIsEdit();

            if(isCalibration)
                emit calibrationFactorIsReady(first, value.f);
        }

        qDebug() << QString("Отображение уставки: переменная->%1, значение = %2").arg(first).arg(str);

        if(unit.count() == 1)
            break;

        index += 2;
    }
}
/*!
 * \brief ConfiguratorWindow::displaySettingVariableResponse
 * \param unit ответ с данными от устройства
 */
void ConfiguratorWindow::displaySettingVariableResponse(CModBusDataUnit &unit)
{
    if(unit.count() != 20)
        return;

    QString key = unit.property("KEY").toString();
    QString var = unit.property("VARIABLE").toString();

    if(key.isEmpty() || var.isEmpty())
        return;

    int addr = addressSettingKey(key);

    if(addr == -1)
        return;

    DeviceMenuItemType group = static_cast<DeviceMenuItemType>(unit.property("GROUP").toInt());
    CDeviceMenuTableWidget* table = groupMenuWidget(group);

    if(!table)
    {
        return;
    }

    QStringList var_list = var.split(',');

    if(var_list.isEmpty())
        return;

    QVector<quint32> values;

    for(int i = 0; i < unit.count() - 1; i += 2)
    {
        values << ((unit[i] << 16) | unit[i + 1]);
    }

    bool is_save = unit.property("READ_STATE").toBool();

    if(is_save)
    {
        sendSettingWriteRequestVariableState(addr, var_list, unit.property("SUFFIX").toString(), values, table);
        return;
    }

    for(const QString &v: var_list)
    {
        int var_bit = bitByVariableName(v);

        if(var_bit == -1)
            return;

        QString nameWgt = QString("comboBox%1").arg(v);
        QString nameSuffix = unit.property("SUFFIX").toString();

        if(!nameSuffix.isEmpty())
            nameWgt += nameSuffix;

        if(nameWgt.isEmpty())
            return;

        CMenuComboBox* comboBox = qobject_cast<CMenuComboBox*>(groupMenuCellWidgetByName(table, nameWgt, 1));

        if(!comboBox)
            return;

        int var_pos = var_bit/32;
        int bit_pos = var_bit%32;

        quint32 value = values[var_pos];

        int state = (value >> bit_pos) & 0x00000001;
        qDebug() << QString("Отображение уставки (внутренняя переменная): переменная->%1, значение = %2 (номер бита = %3)").
                    arg(v).arg(state).arg(bit_pos);
        comboBox->setCurrentIndex(state);
        comboBox->resetIsEdit();
    }
}
/*!
 * \brief ConfiguratorWindow::statusInfo
 *
 * Отображение информации о статусах
 */
void ConfiguratorWindow::displayStatusInfo(const CModBusDataUnit& unit)
{
    RequestType type = static_cast<RequestType>(unit.property("REQUEST").toInt());

    quint32 value = 0;

    switch (type)
    {
        case READ_STATUS_MCP_INFO:
            value = static_cast<quint32>(unit[1]) | static_cast<quint32>((unit[0] << 16));

            if(value == 0)
            {
                showMessageBox(tr("Информация о статусах"), tr("Нет ни одного активного статуса"), QMessageBox::Information);
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
/*!
 * \brief ConfiguratorWindow::displayInternalVariables
 * \param data Вектор состояний внутренних переменных
 *
 * Отображение состояний внутренних переменных
 */
void ConfiguratorWindow::displayInternalVariables(const QVector<quint16>& data)
{
    if(data.count() != 28 || !m_debug_var_window || m_internal_variable_list.isEmpty())
        return;

    QVector<quint16> values;

    for(int i = 0; i < data.count() - 1; i += 2) // меняем местами младший со старшим
    {
        values << data[i + 1] << data[i];
    }

    for(int i = 0; i < m_internal_variable_list.count(); i++)
    {
        if(m_internal_variable_list.find(i) != m_internal_variable_list.end())
        {
            CCheckBoxInternalVariable* checkbox = m_internal_variable_list[i];

            if(checkbox)
            {
                if(checkbox->text().toUpper().contains("RESERVE"))
                {
                    checkbox->setDisabled(true);
                    continue;
                }

                int var = i/16;
                int bit = i%16;

                if(i < m_internal_variable_list.count())
                {
                    bool new_state = ((values[var]&(1 << bit))?true:false);
                    bool cur_state = checkbox->isChecked();

                    if(new_state != cur_state)
                    {
                        checkbox->setChecked(new_state);
                        checkbox->setChangedState(true);
                    }
                }
                else
                {
                    qDebug() << QString("Конец вывода отладочной информации: бит %1 выходит за пределы 0 - %2").arg(i).
                                       arg(data.count()*16 - 1);
                    break;
                }
            }
        }
    }
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

    if(requestFuncton == FUNCTION_SAVE) // ответ на запись - выйти
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

    CMenuComboBox* comboBox = qobject_cast<CMenuComboBox*>(groupMenuCellWidgetByName(table, nameWgt, 1));

    if(!comboBox)
        return;

    quint16 i = unit[0];

    if(indexName.toUpper() != "TZ")
        i--;
qDebug() << QString("Отображение уставки: переменная->%1, значение = %2").arg(indexName).arg(i);
    comboBox->setCurrentIndex(i);
    comboBox->resetIsEdit();
}
//------------------------------------------------------------------
void ConfiguratorWindow::displayPurposeOutput(CModBusDataUnit& unit)
{
    if(unit.count() == 0)
        return;

    QString first = unit.property(tr("FIRST")).toString(); // получаем первый ключ
    QString last  = unit.property(tr("LAST")).toString();  // получаем второй ключ

    if(first.isEmpty() || last.isEmpty())
        return;

    int faddr = addressPurposeKey(first);
    int eaddr = addressPurposeKey(last);

    int columnCount = (eaddr - faddr)/20 + 1; // 20 полуслов хранит до 320 переменных

    if(columnCount == 0)
        return;

    QTableView* table = tableMatrixFromKeys(first, last); // поиск таблицы по ключам

    if(!table)
        return;

    CMatrixPurposeModel* model = static_cast<CMatrixPurposeModel*>(table->model());

    if(!model)
        return;

    CMatrix& matrix = model->matrix();

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
        int offset_pos   = i*20; // смещение в данных, т.е. 20 ячеек длина одного запроса

        CMatrix::RowArray& rows = matrix.rows();

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
        outLogMessage(tr("Количество привязок входов не соответствует количеству инверсных привязок: %1/%2").
                      arg(input_inverse_list.count()).arg(input_inverse_list.count()));
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
        quint32 value = static_cast<quint32>(((input_list[i] << 16) | input_list[i + 1]));
        input_data << value;
    }

    QVector<quint32> input_inverse_data;

    for(int i = 0; i < (input_inverse_list.count() - 1); i += 2) // переводим полуслова (16 бит) в слова (32 бита)
    {                                                            // каждые 32 бита хранят состояния инверсий входов для переменной
        quint32 value = static_cast<quint32>(((input_inverse_list[i] << 16) | input_inverse_list[i + 1]));
        input_inverse_data << value;
    }

    if(input_data.count() != var_list.count() || input_inverse_data.count() != var_list.count())
    {
        qWarning() << QString("Не совпадает количество принятых переменных с прочитанными из таблицы привязок базы данных!");
        return;
    }

    int columns = matrix.columnCount();
qDebug() << QString("Количество столбцов: %1").arg(columns);

    for(int i = 0; i < var_list.count(); i++)
    {
        QString key = var_list[i].toUpper();

        int row_index = -1;

        for(int k = 0; k < matrix.rowCount(); k++) // производим поиск позиции текущей переменной в строках, т.к. строки
        {                                          // идут не по порядку - разбиты на группы (позиция переменной в var_list
                                                   // определяет ее положение в полученных данных учитывая смещение)

            QString matrix_key = matrix[k].data().key.toUpper();

            if(matrix_key == key)
            {
                row_index = k;
                break;
            }
        }

        if(row_index != -1)
        {
            for(int j = 0; j < columns; j++)
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
        else
        {
            qWarning() << QString("Привязка входов: Нет переменной в базе данных -> %1").arg(key);
        }
    }

    model->updateData();
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

        quint8 device_code  = quint8((value0 >> 8)&0xFF); // получаем код изделия РПА
        quint8 index_number = static_cast<quint8>((((value0&0xFF) >> 4)&0x0F)*1000 + ((value0&0xFF)&0x0F)*100 +
                              (((((value1 >> 8)&0xFF)) >> 4)&0x0F)*10 + ((((value1 >> 8)&0xFF))&0x0F));
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
       outLogMessage(tr("Защиты->Резервные->Сигналы пуска: нет данных."));
        return;
    }

    QVector<quint16> tdata;

    for(int i = 0; i < data.count() - 1; i += 2) // меняем местами старший и младший байт
    {
        tdata << data[i + 1] << data[i];
    }

    int pos = groupMenuPosition(tr("Сигнал пуска"), ui->tableWidgetProtectionGroupReserve);

    for(int row = pos; row < ui->tableWidgetProtectionGroupReserve->rowCount(); row++)
    {
        QWidget* widget = groupMenuCellWidget(ui->tableWidgetProtectionGroupReserve, row, 1);

        if(!widget)
            continue;

        if(QString(widget->metaObject()->className()).toUpper() != "CMENUCOMBOBOX")
            continue;

        CMenuComboBox* combobox = qobject_cast<CMenuComboBox*>(widget);

        if(!combobox)
            continue;

        QString key = combobox->objectName().remove("comboBox");

        if(key.isEmpty())
            continue;

        int bit     = m_variable_bits[key];
        int val_pos = bit/16;
        int bit_pos = bit%16;

        if(val_pos < tdata.count())
        {
            int item_pos = (tdata[val_pos]&(1 << bit_pos))?1:0;

            if(item_pos < combobox->count())
                combobox->setCurrentIndex(item_pos);

            combobox->resetIsEdit();
        }
    }
}
//-----------------------------------------------------------------------
void ConfiguratorWindow::displayProtectionWorkMode(CModBusDataUnit& unit)
{
    if(unit.count() != 40) // 20 32-битных ячеек - это 2 строки, т.к. ширина матрицы 10 32-битных ячеек
        return;

    RequestFunction function = RequestFunction(unit.property("REQUEST_FUNCTION").toInt());

    if(function == FUNCTION_SAVE)
    {
        protectionWorkModeWrite(unit);
        return;
    }

    QString tprotect = unit.property("PROTECTION").toString();

    if(tprotect.isEmpty())
        return;

    DeviceMenuItemType      group = static_cast<DeviceMenuItemType>(unit.property("GROUP").toInt());
    CDeviceMenuTableWidget* table = groupMenuWidget(group);

    if(!table)
        return;

    QStringList protectList = tprotect.split(',');

    if(protectList.isEmpty())
        return;

    QVector<quint32> values;

    // перевод данных в из 16-битного в 32-битный формат
    for(int i = 0; i < unit.count() - 1; i += 2)
    {
        values << quint32((unit[i] << 16) | unit[i + 1]);
    }

    for(const QString &protect_name: protectList)
    {
        QString wgtName = QString("comboBox%1").arg(protect_name);
        CMenuComboBox* comboBox = qobject_cast<CMenuComboBox*>(groupMenuCellWidgetByName(table, wgtName, 1));

        if(!comboBox)
            return;

        QString var;

        if(m_protections.find(protect_name) != m_protections.end())
            var = m_protections[protect_name].var_name;

        if(var.isEmpty())
            return;

        int bitNumber = -1;

        if(m_variable_bits.find(var) != m_variable_bits.end())
            bitNumber = m_variable_bits[var];

        if(bitNumber == -1)
            return;

        int i11_pos = bitNumber/32;
        int i17_pos = i11_pos + 10;
        int bit     = bitNumber%32;

        bool i11_state = (values[i11_pos]&(1 << bit));
        bool i17_state = (values[i17_pos]&(1 << bit));

        int row = -1;

        if(!i11_state && !i17_state) // сигнализация
            row = 0;
        else if(i11_state && !i17_state) // отключение без блокировки
            row = 2;
        else if(i11_state && i17_state) // отключение с блокировкой
            row = 1;

        if(row >= 0 && row < comboBox->count())
        {
            comboBox->setCurrentIndex(row);
            comboBox->resetIsEdit();
        }
        else
            qWarning() << QString("Установка рабочего режима защиты: недопустимое значение I11(%1) и I17(%2).").arg(i11_state).arg(i17_state);
    }

    qDebug() << "Отображение режима работы защиты " << tprotect << ": данные-> " << values;
}
//---------------------------------------------------------------------
void ConfiguratorWindow::protectionWorkModeWrite(CModBusDataUnit &unit)
{
    QString tprotect = unit.property("PROTECTION").toString();

    if(tprotect.isEmpty())
        return;

    DeviceMenuItemType      group = static_cast<DeviceMenuItemType>(unit.property("GROUP").toInt());
    CDeviceMenuTableWidget* table = groupMenuWidget(group);

    if(!table)
        return;

    QStringList protectList = tprotect.split(',');

    QVector<quint32> values;

    // перевод данных из 16-битного в 32-битный формат
    for(int i = 0; i < unit.count() - 1; i += 2)
    {
        values << quint32((unit[i] << 16) | unit[i + 1]);
    }

    qDebug() << "Запись режима работы защит " << tprotect << " (принятые данные): " << values;

    for(const QString &protect_name: protectList)
    {
        QString wgtName = QString("comboBox%1").arg(protect_name);
        CMenuComboBox* comboBox = qobject_cast<CMenuComboBox*>(groupMenuCellWidgetByName(table, wgtName, 1));

        if(!comboBox)
            return;

        QString var;

        if(m_protections.find(protect_name) != m_protections.end())
            var = m_protections[protect_name].var_name;

        if(var.isEmpty())
            return;

        int bitNumber = -1;

        if(m_variable_bits.find(var) != m_variable_bits.end())
            bitNumber = m_variable_bits[var];

        if(bitNumber == -1)
            return;

        int i11_pos = bitNumber/32;
        int i17_pos = i11_pos + 10;
        int bit     = bitNumber%32;

        int i11_state = -1;
        int i17_state = -1;

        int state = comboBox->currentIndex();

        switch(state)
        {
            case 0: // сигнализация
                i11_state = 0;
                i17_state = 0;
            break;

            case 1: // отключение с блокировкой
                i11_state = 1;
                i17_state = 1;
            break;

            case 2: // отключение без блокировки
                i11_state = 1;
                i17_state = 0;
            break;

            default: return;
        }

        if(i11_state == -1 || i17_state == -1)
        {
            qWarning() << QString("Запись рабочего режима защиты: недопустимое значение I11(%1) и I17(%1).").arg(i11_state).arg(i17_state);
            return;
        }

        values[i11_pos] &= (~quint32(1 << bit)); // очищаем бит I11
        values[i11_pos] |= quint32(i11_state << bit); // устанавливаем бит I11
        values[i17_pos] &= (~quint32(1 << bit)); // очищаем бит I17
        values[i17_pos] |= quint32(i17_state << bit); // устанавливаем бит I17
    }

    qDebug() << "Запись режима работы защит " << tprotect << " (модифицированные данные): " << values;

    if(m_modbus->isConnected())
    {
        int addr = addressSettingKey("I11");

        if(addr == -1)
            return;

        QVector<quint16> data;

        // перевод 32 битных данных к 16 битным и переворот страшего и младшего полуслова
        for(int i = 0; i < values.count(); i++)
            data << quint16((values[i] >> 16)&0x0000FFFF) << quint16(values[i]&0x0000FFFF);

        qDebug() << QString("Запись режима работы защит %1 по адресу %2").arg(addr);

        sendDeviceCommand(45); // отправка команды на снятие ключа блокировки записи привязок

        CModBusDataUnit new_unit(static_cast<quint8>(m_serialPortSettings_window->deviceID()), CModBusDataUnit::WriteMultipleRegisters,
                                 static_cast<quint16>(addr), data);

        m_modbus->sendData(new_unit);
    }
}
//-------------------------------------------------------------------
void ConfiguratorWindow::displayMonitorI11_I17(CModBusDataUnit& unit)
{
    if(unit.count() != 40)
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
        int offset = col*20;

        for(int row = 0; row < matrix.rowCount(); row++)
        {
            if(m_monitor_I11_I17_field.find(row) == m_monitor_I11_I17_field.end())
                continue;

            unit_t unit_row = m_monitor_I11_I17_field[row];
            int    val      = unit_row.position/16;
            int    bit      = unit_row.position%16;
            int    index    = offset + val;

            bool state = (unit.values()[index]&(1 << bit));

            matrix[row][col].data().state = ((state)?CHECKED:UNCHECKED);
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
        outLogMessage(tr("Не удалось получить модель таблицы Управления защитами."));
        return;
    }

    CMatrix& matrix = model->matrix();

    for(int col = 0; col < matrix.columnCount(); col++)
    {
        QVector<quint16> col_data = data_buf[col];
        QVector<block_protection_purpose_t> purpose_list = m_block_list[col].purpose;

        for(int row = 0, pos = 0; row < matrix.rowCount(); row++, pos++)
        {
            if(col == row)
            {
                pos--;
                continue;
            }

            block_protection_purpose_t purpose = purpose_list[pos];
            int val_pos = purpose.bit/16;
            int bit     = purpose.bit%16;

            bool state = (col_data[val_pos]&(1 << bit));

            matrix[row][col].data().state = ((state)?CHECKED:UNCHECKED);
        }
    }

    data_buf.clear();
    model->updateData();
}
//--------------------------------------------------------------------
void ConfiguratorWindow::displayDebugInfo(const CModBusDataUnit& unit)
{
    int channel = unit.property("CHANNEL").toInt();

    m_debuginfo_window->setData(channel, unit.values());
}
//--------------------------------------
void ConfiguratorWindow::versionParser()
{
    QFile file(tr(":/files/resource/files/version.txt"));

    if(!file.open(QFile::ReadOnly))
    {
        QString text = tr("Нет файла версии или он поврежден...");
        m_status_bar->setStatusMessage(text, 2000);
        outApplicationEvent(text);
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
int ConfiguratorWindow::readDataFromExcel(QXlsx::Document& doc, const CDeviceMenuTableWidget* table, int offset)
{
    if(!table)
        return -1;

    for(int row = 0; row < table->rowCount(); row++)
    {
        QTableWidgetItem* item = table->item(row, 0);

        if(item)
        {
            continue;
        }

        QWidget* wgt = groupMenuCellWidget(table, row, 1);

        if(!wgt)
            continue;

        QString wgt_name = wgt->metaObject()->className();

        if(wgt_name.toUpper() == "CMENUCOMBOBOX")
        {
            CMenuComboBox* combobox = qobject_cast<CMenuComboBox*>(wgt);

            if(combobox)
            {
                bool isOk  = false;
                int  index = doc.read(row + offset + 1, 2).toInt(&isOk);

                if(isOk)
                    combobox->setCurrentIndex(index - 1);

                combobox->resetIsEdit();
            }
        }
        else if(wgt_name.toUpper() == "CLINEEDIT")
        {
            CLineEdit* lineedit = qobject_cast<CLineEdit*>(wgt);

            if(lineedit)
            {
                QString text = doc.read(row + offset + 1, 2).toString();

                if(!text.isEmpty())
                    lineedit->setText(text);

                lineedit->resetIsEdit();
            }
        }
    }

    m_progressbar->increment();

    return table->rowCount();
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
        outLogMessage(tr("Не удалось прочитать ID группы \"%1\", (%2)").arg(group_name).arg(query.lastError().text()));
    }

    if(query.first())
        group_id = query.value("id").toInt();

    group.name = group_name;

    if(!query.exec(QString("SELECT * FROM menu_item WHERE sort_id=%1").arg(group_id)))
    {
        outLogMessage(tr("Не удалось прочитать свойства группы с ID=%1 (%2)").arg(group_id).arg(query.lastError().text()));
    }

    while(query.next())
    {
        CDeviceMenuTableWidget::item_t item;

        item.key      = query.value("key").toString();
        item.address  = query.value("address").toInt();
        item.unit     = CDeviceMenuTableWidget::measure_t({ query.value("limit_min").toFloat(),
                                                            query.value("limit_max").toFloat(),
                                                            query.value("default_value").toFloat(),
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
        else if(item.type.toUpper() == "SUBGROUP") // если тип - "ПОДГРУППА", то читаем список подпунктов
        {
            group.subgroup << loadMenuSubgroup(item.key);
        }

        group.items << item;
    }

    return group;
}
/*!
 * \brief ConfiguratorWindow::loadMenuSubgroup
 * \param group_name Имя группы (родитель подгруппы - может быть имя другой подгруппы)
 * \return список групп
 */
CDeviceMenuTableWidget::group_t ConfiguratorWindow::loadMenuSubgroup(const QString& group_name)
{
    QSqlQuery query(m_system_db);
    CDeviceMenuTableWidget::group_t group;
    QString query_str = QString("SELECT * FROM subgroup WHERE group_name=\"%1\";").arg(group_name);

    if(query.exec(query_str))
    {
        CDeviceMenuTableWidget::item_list_t list_item;

        while(query.next())
        {
            QString name = query.value("name").toString();
            QString key = query.value("key").toString();
            CDeviceMenuTableWidget::item_t item = loadIODeviceItem(key);

            if(!name.isEmpty()) // еще одна вложенная группа
            {
                QString bind_name = QString("%1, %2").arg(name).arg(item.name);
                item.name = bind_name;
            }

            list_item << item;
        }

        group.items = list_item;
    }

    return group;
}
/*!
 * \brief ConfiguratorWindow::loadIODeviceItem
 * \param k - ключ итема
 * \return итем
 */
CDeviceMenuTableWidget::item_t ConfiguratorWindow::loadIODeviceItem(const QString& k)
{
    QSqlQuery query(m_system_db);
    CDeviceMenuTableWidget::item_t item;

    if(query.exec(QString("SELECT * FROM menu_item WHERE key=\"%1\";").arg(k)))
    {
        if(query.next())
        {
            int     address     = query.value("address").toInt();
            float   min         = query.value("limit_min").toFloat();
            float   max         = query.value("limit_max").toFloat();
            float   val_default = query.value("default_value").toFloat();
            QString unit        = query.value("unit_measure").toString();
            QString type        = query.value("data_type").toString();
            QString name        = query.value("description").toString();
            int     row         = query.value("row").toInt();

            item.key              = k;
            item.address          = address;
            item.unit.min         = min;
            item.unit.max         = max;
            item.unit.val_default = val_default;
            item.unit.unit        = unit;
            item.type             = type;
            item.name             = name;
            item.row              = row;

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
        }
    }

    return item;
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

        case DEVICE_MENU_PROTECT_ITEM_LEAK:
            return ui->tableWidgetProtectionGroupDirect;

        case DEVICE_MENU_PROTECT_ITEM_FREQUENCY:
            return ui->tableWidgetProtectionGroupFrequency;

        case DEVICE_MENU_PROTECT_ITEM_EXTERNAL:
            return ui->tableWidgetProtectionGroupExternal;

        case DEVICE_MENU_PROTECT_ITEM_TEMPERATURE:
            return ui->tableWidgetProtectionGroupTemperature;

        case DEVICE_MENU_PROTECT_ITEM_RESERVE:
            return ui->tableWidgetProtectionGroupReserve;

        case DEVICE_MENU_ITEM_AUTOMATION_ROOT:
        case DEVICE_MENU_ITEM_AUTOMATION_SWITCH:
            return ui->tableWidgetAutomationSwitch;

        case DEVICE_MENU_ITEM_AUTOMATION_APV:
            return ui->tableWidgetAutomationAPV;

        case DEVICE_MENU_ITEM_AUTOMATION_AVR:
            return ui->tableWidgetAutomationAVR;

        case DEVICE_MENU_ITEM_AUTOMATION_KCN:
            return ui->tableWidgetAutomationKCN;

        case DEVICE_MENU_ITEM_SETTINGS_ITEM_IN_ANALOG:
            return ui->tableWidgetSettingsAnalogGroupGeneral;

        default: return nullptr;
    }
}
/*!
 * \brief ConfiguratorWindow::groupMenuCellWidgetByName
 * \param table - указатель на таблицу
 * \param wgt_name - имя виджета
 * \param col - колонка таблицы
 * \return Указатель на виджет из ячейки
 *
 * Поиск виджета в указанно таблице по имени и номеру ячейки
 */
QWidget* ConfiguratorWindow::groupMenuCellWidgetByName(CDeviceMenuTableWidget* table, const QString& wgt_name, int col) const
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
/*!
 * \brief ConfiguratorWindow::groupMenuCellWidget
 * \param table - указатель на таблицу
 * \param row - строка таблицы
 * \param col - колонка таблицы
 * \return Указатель на виджет из ячейки
 *
 * Поиск виджета в указанно таблице по имени и номеру ячейки
 */
QWidget* ConfiguratorWindow::groupMenuCellWidget(const CDeviceMenuTableWidget* table, int row, int col)
{
    if(!table)
        return nullptr;

    if(col > table->columnCount() || row > table->rowCount())
        return nullptr;

    QWidget* wgt = table->cellWidget(row, col);

    if(!wgt)
        return nullptr;

    QObjectList obj_list = wgt->children();

    for(QObject* obj: obj_list)
    {
        if(obj->isWidgetType())
        {
            return qobject_cast<QWidget*>(obj);
        }
    }

    return nullptr;
}
/*!
 * \brief ConfiguratorWindow::groupMenuPosition
 * \param name Имя группы
 * \param table Таблица с группами
 * \return Начальную позицию первого виджета в группе name
 */
int ConfiguratorWindow::groupMenuPosition(const QString& name, const CDeviceMenuTableWidget* table)
{
    if(!table)
        return -1;

    for(int row = 0; row < table->rowCount(); row++)
    {
        QTableWidgetItem* item = table->item(row, 0);

        if(!item)
            continue;

        if(name.toUpper() == item->text().toUpper())
        {
            return row + 1;
        }
    }

    return -1;
}
//---------------------------------------------------
QStringList ConfiguratorWindow::loadLoginList() const
{
    QSqlQuery query(m_system_db);
    QStringList logins;

    if(query.exec(QString("SELECT login FROM user;")))
    {
        while(query.next())
        {
            logins << query.value("login").toString();
        }
    }

    return logins;
}
//----------------------------------------------------------------
QString ConfiguratorWindow::loadUserPassword(const QString& login)
{
    QSqlQuery query(m_system_db);
    QString pass;

    if(query.exec(QString("SELECT pass FROM user WHERE login=\"%1\";").arg(login)))
    {
        if(query.first())
        {
            pass = query.value("pass").toString();
        }
    }

    return pass;
}
//---------------------------------------------------------------
void ConfiguratorWindow::outApplicationEvent(const QString& text)
{
    QString dateTimeText = QString("%1 - %2.").arg(QDateTime::currentDateTime().toString("[dd.mm.yyyy - HH:mm:ss.zzz]")).arg(text);
//    m_event_window->appendPlainText(dateTimeText);
}
//-------------------------------------------------------------------------------------
bool ConfiguratorWindow::createTablePurpose(const QString& tableType, QSqlDatabase *db)
{
    if(!db || (db && !db->isOpen()) || tableType.isEmpty())
        return false;

    QVector<QPair<QString, QString> > labels = loadLabelColumns(tableType);

    if(labels.isEmpty())
        return false;

    QString str_db = QString("CREATE TABLE purpose%1 ("
                             "id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE, ").arg(tableType);

    for(int col = 0; col < labels.count(); col++)
    {
        str_db += QString("col%1 INTEGER").arg(col);
        if(col != labels.count() - 1)
            str_db += ", ";
    }

    str_db += ");";

    QSqlQuery query(*db);

    if(!query.exec(str_db))
    {
        outLogMessage(tr("Ошибка создания таблицы привязок <%1> (%2): %3").arg(tableType).arg(query.lastError().text()).arg(str_db));
        return false;
    }

    return true;
}
//----------------------------------------------------------------------------------
bool ConfiguratorWindow::createProjectTableProtection(int columns, QSqlDatabase *db)
{
    if((db && !db->isOpen()) || columns == 0)
        return false;

    QString str_db = QString("CREATE TABLE purposeProtection ("
                             "id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE, ");
    for(int col = 0; col < columns; col++)
    {
        str_db += QString("col%1 INTEGER").arg(col);
        if(col != columns - 1)
            str_db += ", ";
    }

    str_db += ");";

    QSqlQuery query(*db);

    if(!query.exec(str_db))
    {
        outLogMessage(tr("Ошибка создания таблицы привязок защит (%1): %2").arg(query.lastError().text()).arg(str_db));
        return false;
    }

    return true;
}
/*!
 * \brief ConfiguratorWindow::createProjectTableSet
 * \param tableName Имя таблицы
 * \return Истина в случае успешного создания таблицы
 */
bool ConfiguratorWindow::createProjectTableSet(const QString& tableName, QSqlDatabase *db)
{
    if((db && !db->isOpen()) || tableName.isEmpty())
        return false;

    QSqlQuery query(*db);

    if(!query.exec(QString("CREATE TABLE deviceSet%1 ("
                           "id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE, "
                           "val STRING, "
                           "type STRING);").arg(tableName)))
    {
        outLogMessage(tr("Ошибка создания таблицы уставок устройства (%1): %2").arg(tableName).arg(query.lastError().text()));
        return false;
    }

    return true;
}
/*!
 * \brief ConfiguratorWindow::createProjectTableCommunication
 * \return Истина если таблица создана успешно
 *
 * Создание таблицы в проектном файле для хранения настроек связи
 */
bool ConfiguratorWindow::createProjectTableCommunication(QSqlDatabase *db)
{
    if((db && !db->isOpen()))
        return false;

    QSqlQuery query(*db);

    if(!query.exec(QString("CREATE TABLE deviceCommunication ("
                           "id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE, "
                           "address INTEGER, "
                           "speed INTEGER, "
                           "parity INTEGER, "
                           "Trequest INTEGER, "
                           "Tspeed INTEGER);")))
    {
        outLogMessage(tr("Ошибка создания таблицы настроек связи устройства: %1").arg(query.lastError().text()));
        return false;
    }

    return true;
}
/*!
 * \brief ConfiguratorWindow::createProjectTableCalibration
 * \return Истина, если таблица успешно создана
 *
 * Сохранение данных калибровок (по умолчанию)
 */
bool ConfiguratorWindow::createProjectTableCalibrationCurrent(QSqlDatabase *db)
{
    if((db && !db->isOpen()))
        return false;

    QSqlQuery query(*db);

    if(!query.exec(QString("CREATE TABLE deviceCalibrationCurrent ("
                           "id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE, "
                           "standardPhase STRING, "
                           "standard3I0 STRING, "
                           "stateIa INTEGER, "
                           "stateIb INTEGER, "
                           "stateIc INTEGER,"
                           "state3I0 INTEGER, "
                           "dataCount INTEGER, "
                           "pauseRequest INTEGER);")))
    {
        outLogMessage(tr("Ошибка создания таблицы калибровок по току: %1").arg(query.lastError().text()));
        return false;
    }

    return true;
}
/*!
 * \brief ConfiguratorWindow::createProjectTableCalibrationPowerAC
 * \param db Указатель на текущую базу файла проекта
 * \return Истина, если таблица успешно создана
 *
 * Создание таблицы калибровок по напряжению AC
 */
bool ConfiguratorWindow::createProjectTableCalibrationPowerAC(QSqlDatabase *db)
{
    if((db && !db->isOpen()))
        return false;

    QSqlQuery query(*db);

    if(!query.exec(QString("CREATE TABLE deviceCalibrationPowerAC ("
                           "id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE, "
                           "standardPhase STRING, "
                           "standardInterPhase STRING, "
                           "standardMultiplier STRING, "
                           "stateUa INTEGER, "
                           "stateUb INTEGER, "
                           "stateUc INTEGER,"
                           "stateUab INTEGER, "
                           "stateUbc INTEGER, "
                           "stateUca INTEGER, "
                           "state3U0S INTEGER, "
                           "state3US INTEGER, "
                           "state3U0 INTEGER, "
                           "dataCount INTEGER, "
                           "pauseRequest INTEGER);")))
    {
        outLogMessage(tr("Ошибка создания таблицы калибровок по напряжению AC: %1").arg(query.lastError().text()));
        return false;
    }

    return true;
}
/*!
 * \brief ConfiguratorWindow::createProjectTableCalibrationBruResistance
 * \param db Указатель на текущую базу файла проекта
 * \return Истина, если таблица успешно создана
 *
 * Создание таблицы калибровок БРУ по сопротивлению
 */
bool ConfiguratorWindow::createProjectTableCalibrationBruResistance(QSqlDatabase *db)
{
    if((db && !db->isOpen()))
        return false;

    QSqlQuery query(*db);

    if(!query.exec(QString("CREATE TABLE deviceCalibrationBruResistance ("
                           "id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE, "
                           "Rmin STRING, "
                           "Rmax STRING, "
                           "stateRa INTEGER, "
                           "stateRb INTEGER, "
                           "stateRc INTEGER);")))
    {
        outLogMessage(tr("Ошибка создания таблицы калибровок БРУ по сопротивлению: %1").arg(query.lastError().text()));
        return false;
    }

    return true;
}
/*!
 * \brief ConfiguratorWindow::createProjectTableCalibrationBruPowerDC
 * \param db Указатель на текущую базу файла проекта
 * \return Истина, если таблица успешно создана
 *
 * Создание таблицы калибровок БРУ по постоянному напряжению
 */
bool ConfiguratorWindow::createProjectTableCalibrationBruPowerDC(QSqlDatabase *db)
{
    if((db && !db->isOpen()))
        return false;

    QSqlQuery query(*db);

    if(!query.exec(QString("CREATE TABLE deviceCalibrationBruPowerDC ("
                           "id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE, "
                           "PhaseMin STRING, "
                           "PhaseMax STRING, "
                           "MultiplierMin STRING, "
                           "MultiplierMax STRING, "
                           "stateUa INTEGER, "
                           "stateUb INTEGER, "
                           "stateUc INTEGER, "
                           "stateUU INTEGER"
                           "dataCount INTEGER, "
                           "pauseRequest INTEGER);")))
    {
        outLogMessage(tr("Ошибка создания таблицы калибровок БРУ по постоянноиу напряжению: %1").arg(query.lastError().text()));
        return false;
    }

    return true;
}
/*!
 * \brief ConfiguratorWindow::createProjectTableContainer
 * \return Истина, в случае успешного создания таблицы
 *
 * Создание таблицы настроек положения контейнеров панелей
 */
bool ConfiguratorWindow::createProjectTableContainer(QSqlDatabase *db)
{
    if((db && !db->isOpen()))
        return false;

    QSqlQuery query(*db);

    if(!query.exec(QString("CREATE TABLE containerSettings ("
                           "id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE, "
                           "name STRING, "
                           "side STRING, "
                           "visible INTEGER, "
                           "pos INTEGER, "
                           "geometry BLOB);")))
    {
        outLogMessage(tr("Ошибка создания таблицы настроек положения контейнеров: %1").arg(query.lastError().text()));
        return false;
    }

    return true;
}
/*!
 * \brief ConfiguratorWindow::saveDIToProject
 * \param matrix Матрица привязок дискретных входов
 *
 * Вставка привязок дискретных входов в таблицу проектного файла
 */
void ConfiguratorWindow::savePurposeToProject(CPurposeTableView* table, const QString& type, QSqlDatabase *db)
{
    if(!table || (db && !db->isOpen()) || type.isEmpty())
    {
        outLogMessage(tr("Запись привязок: Файл проекта не создан, либо закрыт"));
        return;
    }

    CMatrixPurposeModel* model = static_cast<CMatrixPurposeModel*>(table->model());

    if(!model)
    {
        outLogMessage(tr("Запись привязок: Невозможно обратиться к модели представления"));
        return;
    }

    CMatrix matrix = model->matrix();

    if(matrix.rowCount() == 0)
    {
        outLogMessage(tr("Запись привязок: Нет данных в матрице").arg(type));
        return;
    }

    QSqlQuery query(*db);
    // очистка таблицы привязок дискретных входов
    if(!query.exec(QString("DELETE FROM purpose%1;").arg(type)))
    {
        outLogMessage(tr("Запись привязок <%1>: не удалось очисть таблицу привязок (%2)").arg(type).arg(query.lastError().text()));
        return;
    }

    // вставка данных в таблицу
    QString colListBind;
    QString colList;
    for(int col = 0; col < matrix.columnCount(); col++)
    {
        QString colName = QString("col%1").arg(col);
        colList += colName;
        colListBind += QString(":%1").arg(colName);

        if(col != matrix.columnCount() - 1)
        {
            colList += ", ";
            colListBind += ", ";
        }
    }

    db->transaction();
    query.prepare(QString("INSERT INTO purpose%1 (%2) VALUES(%3)").arg(type).arg(colList).arg(colListBind));
    for(int row = 0; row < matrix.rowCount(); row++)
    {
        CRow::ColumnArray columns = matrix[row].columns();
        for(int col = 0; col < columns.count(); col++)
        {
            CColumn column = columns[col];
            int state = static_cast<int>(column.data().state);

            query.bindValue(QString(":col%1").arg(col), state);
        }

        if(!query.exec())
        {
            outLogMessage(tr("Запись привязок <%1>: не удалось вставить строку %2 в таблицу привязок (%3)").arg(type).arg(row).
                                                                                                            arg(query.lastError().text()));
        }
    }
    db->commit();
    m_progressbar->increment(5);
}
//--------------------------------------------------------------------------------------------------
void ConfiguratorWindow::saveJournalToProject(const CJournalWidget* widgetJournal, QSqlDatabase *db)
{
    if(!widgetJournal || !db || !db->isOpen())
        return;

    CJournalTable* journal = widgetJournal->table();

    if(!journal || (journal && journal->rowCount() == 0))
        return;

    QString type = widgetJournal->property("TYPE").toString();
    QSqlQuery query (*db);

    if(!query.exec(QString("DELETE FROM journal%1;").arg(type)))
    {
        outLogMessage(tr("Сохранение журнала <%1>: ошибка удаления данных журнала (%2)").arg(type).arg(db->lastError().text()));
        return;
    }

    query.clear();

    if(type.toUpper() == "CRASH" || type.toUpper() == "HALFHOUR")
    {
        if(!query.exec(QString("DELETE FROM propertyJournal%1;").arg(type)))
        {
            outLogMessage(tr("Сохранение журнала <%1>: возникли ошибки в процессе стирания свойств журнала (%2)").arg(type).
                          arg(query.lastError().text()));
        }

        query.clear();
    }

    // вставка данных в таблицу журналов
    QString colStrList;
    QStringList colListBind;

    if(type.toUpper() == "CRASH")
    {
        colStrList = "id_msg, date, time, protection, id_journal, sn_device";
        colListBind << ":id_msg" << ":date" << ":time" << ":protection";
    }
    else if(type.toUpper() == "EVENT")
    {
        colStrList = "id_msg, date, time, type, category, parameter, sn_device";
        colListBind << ":id_msg" << ":date" << ":time" << ":type" << ":category" << ":parameter";
    }
    else if(type.toUpper() == "HALFHOUR")
    {
        colStrList = "id_msg, date, time, type, time_reset, id_journal, sn_device";
        colListBind << ":id_msg" << ":date" << ":time" << ":type" << ":time_reset";
    }

    db->transaction();

    bool isError = false;
    QString lastError;

    for(int row = 0; row < journal->rowCount(); row++)
    {
        QString bindValue;
        QString type_data;

        for(int col = 1; col < journal->columnCount(); col++)
        {
            QString value = journal->rowColumnData(row, col).toString();
            QString field = colListBind.at(col - 1);

            if(field.toUpper() == ":TYPE")
                type_data = value;

            if(field.toUpper() == ":ID_MSG")
                bindValue += value;
            else
                bindValue += QString("\'%1\'").arg(value);

            if(col < journal->columnCount() - 1)
                bindValue += ", ";
        }

        if(type.toUpper() == "CRASH" || type.toUpper() == "HALFHOUR") // для журнлов аварий и получасовок добавляется id_journal
            bindValue += QString(", %1").arg(row);
        bindValue += QString(", %1").arg(0); // sn_device

        QString query_str = QString("INSERT INTO journal%1 (%2) VALUES(%3);").arg(type).arg(colStrList).arg(bindValue);

        if(!query.exec(query_str))
        {
            lastError = query.lastError().text();
            isError = true;
        }

        // вставка свойств журналов Аварий и Получасовок
        if(type.toUpper() == "CRASH")
        {
            property_list_t propertyList;
            QVariant val = journal->rowData(row);

            if(val.canConvert<property_list_t>())
                propertyList = val.value<property_list_t>();

           if(!propertyList.isEmpty())
           {
               QSqlQuery query_property(*db);
               query_property.prepare("INSERT INTO propertyJournalCRASH (name, value, id_journal) VALUES(:name, :value, :id_journal);");

               for(const property_data_item_t& item: propertyList)
               {
                    query_property.bindValue(":name", item.name);
                    query_property.bindValue(":value", item.value);
                    query_property.bindValue(":id_journal", row);

                    if(!query_property.exec())
                    {
                        outLogMessage(tr("Сохранение журнала <%1>: возникли ошибки в процессе сохранения свойств журнала (%2)").arg(type).
                                      arg(query_property.lastError().text()));
                    }
               }
           }
        }
        else if(type.toUpper() == "HALFHOUR")
        {
            halfhour_t halfhour;
            QVariant val = journal->rowData(row);

            if(val.canConvert<halfhour_t>())
                halfhour = val.value<halfhour_t>();

            if(!halfhour.values.isEmpty() && type_data.toUpper() == tr("ДАННЫЕ"))
            {
                QSqlQuery query_property(*db);

                for(const float& value: halfhour.values)
                {
                    query_property.prepare("INSERT INTO propertyJournalHALFHOUR (value, id_journal)"
                                           "VALUES(:value, :id_journal)");
                    query_property.bindValue(":value", value);
                    query_property.bindValue(":id_journal", row);

                    if(!query_property.exec())
                    {
                        outLogMessage(tr("Ошибка сохранения свойств данных журнала получасовок в БД: %1").arg(query_property.lastError().text()));
                    }
                }
            }
        }
    }

    db->commit();

    if(isError)
    {
        outLogMessage(tr("Сохранение журнала <%1>: возникли ошибки в процессе записи журнала (%2)").arg(type).arg(lastError));
    }

    m_progressbar->increment(3);
}
//---------------------------------------------------------------------------------------------------------------------------------------
void ConfiguratorWindow::saveDeviceSetToProject(ConfiguratorWindow::DeviceMenuItemType index, const QString& tableName, QSqlDatabase *db)
{
    if(!db || (db && !db->isOpen()) || index == DEVICE_MENU_ITEM_NONE || tableName.isEmpty())
        return;

    QSqlQuery query (*db);

    if(!query.exec(QString("DELETE FROM deviceSet%1;").arg(tableName)))
    {
        outLogMessage(tr("Сохранение уставок устройства для группы <%1>: ошибка удаления данных из таблицы уставок (%2)").
                      arg(tableName).arg(db->lastError().text()));
        return;
    }

    query.clear();

    CDeviceMenuTableWidget* table = groupMenuWidget(index);

    if(!table)
        return;

    db->transaction();

    for(int row = 0; row < table->rowCount(); row++)
    {
        QWidget* widget = groupMenuCellWidget(table, row, 1);

        if(!widget)
            continue;

        QString value;
        QString type;

        if(QString(widget->metaObject()->className()).toUpper() == "CMENUCOMBOBOX")
        {
            CMenuComboBox* comboBox = static_cast<CMenuComboBox*>(widget);

            if(comboBox)
            {
                value = QString::number(comboBox->currentIndex());
                type  = "COMBOBOX";
            }
        }
        else if(QString(widget->metaObject()->className()).toUpper() == "CLINEEDIT")
        {
            CLineEdit* lineEdit = static_cast<CLineEdit*>(widget);

            if(lineEdit)
            {
                value = lineEdit->text();
                type  = "LINEEDIT";
            }
        }

        if(!value.isEmpty() && !type.isEmpty())
        {
            QSqlQuery query(*db);

            if(!query.exec(QString("INSERT INTO deviceSet%1 (val, type) VALUES(\'%2\', \'%3\');").arg(tableName).arg(value).arg(type)))
            {
                outLogMessage(tr("Ошибка сохранения уставок группы %1: %2").arg(tableName).arg(db->lastError().text()));
            }
        }
    }

    db->commit();
    m_progressbar->increment(3);
}
/*!
 * \brief ConfiguratorWindow::saveDeviceCommunication
 *
 * Сохранение настроек связи устройства в таблицу проекта
 */
void ConfiguratorWindow::saveDeviceCommunication(QSqlDatabase *db)
{
    if(!db || (db && !db->isOpen()))
        return;

    QSqlQuery query(*db);

    if(!query.exec(QString("DELETE FROM deviceCommunication;")))
    {
        outLogMessage(tr("Сохранение настроек связи устройства: ошибка удаления данных из таблицы (%1)").arg(db->lastError().text()));
        return;
    }

    query.clear();

    if(!query.exec(QString("INSERT INTO deviceCommunication (address, speed, parity, Trequest, Tspeed) VALUES("
                           "%1, %2, %3, %4, %5);").arg(0).arg(ui->comboBoxCommunicationBaudrate->currentIndex()).
                                                   arg(0).arg(0).arg(0)))
    {
        outLogMessage(tr("Ошибка сохранения настроек связи устройства: %1").arg(query.lastError().text()));
    }

    m_progressbar->increment(3);
}
/*!
 * \brief ConfiguratorWindow::saveDeviceCalibrationCurrent
 *
 * Сохранение калибровок по току (эталонных значений) в проектном файле
 */
void ConfiguratorWindow::saveDeviceCalibrationCurrent(QSqlDatabase *db)
{
    if(!db || (db && !db->isOpen()))
        return;

    QSqlQuery query(*db);

    if(!query.exec(QString("DELETE FROM deviceCalibrationCurrent;")))
    {
        outLogMessage(tr("Сохранение эталонных значений калибровок по току: ошибка удаления данных из таблицы (%1)").arg(query.lastError().text()));
        return;
    }

    query.clear();

    if(!query.exec(QString("INSERT INTO deviceCalibrationCurrent (standardPhase, standard3I0, stateIa, stateIb, stateIc, state3I0, dataCount, pauseRequest)"
                           " VALUES(%1, %2, %3, %4, %5, %6, %7, %8);").arg(static_cast<double>(ui->widgetCalibrationOfCurrent->standardPhase())).
                                                                       arg(static_cast<double>(ui->widgetCalibrationOfCurrent->standard3I0())).
                                                                       arg(ui->widgetCalibrationOfCurrent->stateIa()).
                                                                       arg(ui->widgetCalibrationOfCurrent->stateIb()).
                                                                       arg(ui->widgetCalibrationOfCurrent->stateIc()).
                                                                       arg(ui->widgetCalibrationOfCurrent->state3I0()).
                                                                       arg(ui->widgetCalibrationOfCurrent->dataCount()).
                                                                       arg(ui->widgetCalibrationOfCurrent->pauseRequest())))
    {
        outLogMessage(tr("Ошибка сохранения эталонных значений калибровок по току устройства: %1").arg(query.lastError().text()));
    }

    m_progressbar->increment(3);
}
/*!
 * \brief ConfiguratorWindow::saveDeviceCalibrationPowerAC
 *
 * Сохранение калибровок по напряжению AC (эталонных значений) в проектном файле
 */
void ConfiguratorWindow::saveDeviceCalibrationPowerAC(QSqlDatabase *db)
{
    if(!db || (db && !db->isOpen()))
        return;

    QSqlQuery query(*db);

    if(!query.exec(QString("DELETE FROM deviceCalibrationPowerAC;")))
    {
        outLogMessage(tr("Сохранение эталонных значений калибровок по напряжению AC: ошибка удаления данных из таблицы (%1)").arg(query.lastError().text()));
        return;
    }

    query.clear();

    if(!query.exec(QString("INSERT INTO deviceCalibrationPowerAC (standardPhase, standardInterPhase, standardMultiplier, "
                           "stateUa, stateUb, stateUc, stateUab, stateUbc, stateUca, state3U0S, state3US, state3U0, "
                           "dataCount, pauseRequest)"
                           " VALUES(%1, %2, %3, %4, %5, %6, %7, %8, %9, %10, %11, %12, %13, %14);").
                   arg(static_cast<double>(ui->widgetCalibrationPower->standardPhase())).
                   arg(static_cast<double>(ui->widgetCalibrationPower->standardPhaseLinear())).
                   arg(static_cast<double>(ui->widgetCalibrationPower->standard3U())).
                   arg(ui->widgetCalibrationPower->stateUa()).
                   arg(ui->widgetCalibrationPower->stateUb()).
                   arg(ui->widgetCalibrationPower->stateUc()).
                   arg(ui->widgetCalibrationPower->stateUab()).
                   arg(ui->widgetCalibrationPower->stateUbc()).
                   arg(ui->widgetCalibrationPower->stateUca()).
                   arg(ui->widgetCalibrationPower->state3U0S()).
                   arg(ui->widgetCalibrationPower->state3US()).
                   arg(ui->widgetCalibrationPower->state3U0()).
                   arg(ui->widgetCalibrationPower->dataCount()).
                   arg(ui->widgetCalibrationPower->pauseRequest())))
    {
        outLogMessage(tr("Ошибка сохранения эталонных значений калибровок по напряжению AC устройства: %1").arg(query.lastError().text()));
    }

    m_progressbar->increment(3);
}
/*!
 * \brief ConfiguratorWindow::saveDeviceCalibrationBruResistance
 *
 * Сохранение калибровок БРУ по сопротивлению (эталонных значений) в проектном файле
 */
void ConfiguratorWindow::saveDeviceCalibrationBruResistance(QSqlDatabase *db)
{
    if(!db || (db && !db->isOpen()))
        return;

    QSqlQuery query(*db);

    if(!query.exec(QString("DELETE FROM deviceCalibrationBruResistance;")))
    {
        outLogMessage(tr("Сохранение эталонных значений калибровок БРУ по сопротивлению: ошибка удаления данных из таблицы (%1)").arg(query.lastError().text()));
        return;
    }

    query.clear();

    if(!query.exec(QString("INSERT INTO deviceCalibrationBruResistance (Rmin, Rmax, stateRa, stateRb, stateRc)"
                           " VALUES(%1, %2, %3, %4, %5);").
                   arg(static_cast<double>(ui->widgetCalibrationBRUResistance->standardPhaseMin())).
                   arg(static_cast<double>(ui->widgetCalibrationBRUResistance->standardPhaseMax())).
                   arg(ui->widgetCalibrationBRUResistance->stateRa()).
                   arg(ui->widgetCalibrationBRUResistance->stateRb()).
                   arg(ui->widgetCalibrationBRUResistance->stateRc())))
    {
        outLogMessage(tr("Ошибка сохранения эталонных значений калибровок БРУ по сопротивлению устройства: %1").arg(query.lastError().text()));
    }

    m_progressbar->increment(3);
}
/*!
 * \brief ConfiguratorWindow::saveDeviceCalibrationBruPowerDC
 *
 * Сохранение калибровок БРУ по постоянному напряжению (эталонных значений) в проектном файле
 */
void ConfiguratorWindow::saveDeviceCalibrationBruPowerDC(QSqlDatabase *db)
{
    if(!db || (db && !db->isOpen()))
        return;

    QSqlQuery query(*db);

    if(!query.exec(QString("DELETE FROM deviceCalibrationBruPowerDC;")))
    {
        outLogMessage(tr("Сохранение эталонных значений калибровок БРУ по напряжению DC: ошибка удаления данных из таблицы (%1)").arg(query.lastError().text()));
        return;
    }

    query.clear();

    if(!query.exec(QString("INSERT INTO deviceCalibrationBruPowerDC (PhaseMin, PhaseMax, MultiplierMin, MultiplierMax, "
                           "stateUa, stateUb, stateUc, stateUU, dataCount, pauseRequest)"
                           " VALUES(%1, %2, %3, %4, %5, %6, %7, %8);").
                   arg(static_cast<double>(ui->widgetCalibrationBRUPowerDC->standardPhaseMin())).
                   arg(static_cast<double>(ui->widgetCalibrationBRUPowerDC->standardPhaseMax())).
                   arg(static_cast<double>(ui->widgetCalibrationBRUPowerDC->standardPhaseMultiplierMin())).
                   arg(static_cast<double>(ui->widgetCalibrationBRUPowerDC->standardPhaseMultiplierMax())).
                   arg(ui->widgetCalibrationBRUPowerDC->stateUa()).
                   arg(ui->widgetCalibrationBRUPowerDC->stateUb()).
                   arg(ui->widgetCalibrationBRUPowerDC->stateUc()).
                   arg(ui->widgetCalibrationBRUPowerDC->stateUMultiplier()).
                   arg(ui->widgetCalibrationBRUPowerDC->dataCount()).
                   arg(ui->widgetCalibrationBRUPowerDC->pauseRequest())))
    {
        outLogMessage(tr("Ошибка сохранения эталонных значений калибровок БРУ по напряжению DC устройства: %1").arg(query.lastError().text()));
    }

    m_progressbar->increment(3);
}
/*!
 * \brief ConfiguratorWindow::saveContainerSettings
 * \param Указатель на контейнер
 *
 * Сохранение настроек контейнеров
 */
void ConfiguratorWindow::saveContainerSettings(const CContainerWidget* container, QSqlDatabase *db)
{
    if(!container || !db || (db && !db->isOpen()))
        return;

    QSqlQuery query(*db);
    QString side = QString("%1").arg((container->side() == CDockPanelItemCtrl::DirLeft)?"LEFT":
                                     (container->side() == CDockPanelItemCtrl::DirRight)?"RIGHT":
                                     (container->side() == CDockPanelItemCtrl::DirBottom)?"BOTTOM":
                                     (container->side() == CDockPanelItemCtrl::DirTop)?"TOP":"NONE");
    QString query_str = QString("INSERT INTO containerSettings (name, side, visible, pos, geometry) VALUES (:name, :side, :visible, :pos, :geometry);");

    query.prepare(query_str);
    query.bindValue(":name", container->name());
    query.bindValue(":side", side);
    query.bindValue(":visible", !container->isHidden());
    query.bindValue(":pos", container->position());
    query.bindValue(":geometry", container->saveGeometry());

    if(!query.exec())
    {
        outLogMessage(tr("Загрузка настроек контейнера: не удалось сохранить настройки контейнера <%1> в файле проекта (%2)").arg(container->name()).
                      arg(query.lastError().text()));
        return;
    }

    m_progressbar->increment(4);
}
/*!
 * \brief ConfiguratorWindow::loadJournalFromProject
 * \param widgetJournal Виджет журнала
 *
 * Загружает журнал из проектного файла
 */
bool ConfiguratorWindow::loadJournalFromProject(const CJournalWidget* widgetJournal, QSqlDatabase *db)
{
    if(!widgetJournal || !db || (db && !db->isOpen()))
        return false;

    CJournalTable* journal = widgetJournal->table();

    if(!journal)
        return false;

    QString journal_type = widgetJournal->property("TYPE").toString();

    if(journal_type.isEmpty())
        return false;

    widgetJournal->journalClear();

    QString query_str = QString("SELECT * FROM journal%1;").arg(journal_type);
    QSqlQuery query(*db);

    if(!query.exec(query_str))
        return false;

    while(query.next())
    {
        int row = journal->rowCount();
        journal->insertRow(row);

        QString id_msg = query.value("id_msg").toString();
        QString date   = query.value("date").toString();
        QString time   = query.value("time").toString();

        journal->setItem(row, 0, new CTableWidgetItem(QString::number(row + 1)));
        journal->setItem(row, 1, new CTableWidgetItem(id_msg));
        journal->setItem(row, 2, new CTableWidgetItem(date));
        journal->setItem(row, 3, new CTableWidgetItem(time));

        journal->item(row, 0)->setTextAlignment(Qt::AlignCenter);
        journal->item(row, 1)->setTextAlignment(Qt::AlignCenter);
        journal->item(row, 2)->setTextAlignment(Qt::AlignCenter);
        journal->item(row, 3)->setTextAlignment(Qt::AlignCenter);

        if(journal_type == "EVENT")
        {
            QString type      = query.value("type").toString();
            QString category  = query.value("category").toString();
            QString parameter = query.value("parameter").toString();

            journal->setItem(row, 6, new CTableWidgetItem(type));
            journal->setItem(row, 5, new CTableWidgetItem(category));
            journal->setItem(row, 4, new CTableWidgetItem(parameter));
        }
        else if(journal_type == "CRASH")
        {
            QString protection = query.value("protection").toString();
            int     id_journal = query.value("id_journal").toInt();

            journal->setItem(row, 4, new CTableWidgetItem(protection));

            // формирование запроса для получения свойств записи
            QSqlQuery query_property(*db);

            if(!query_property.exec(QString("SELECT name, value FROM propertyJournalCRASH WHERE id_journal=%1;").arg(id_journal)))
            {
                outLogMessage(tr("Импорт журнала: Не удалось прочитать свойства журнала аварий: %1").arg(query_property.lastError().text()));
                continue;
            }

            property_list_t property_list;

            while(query_property.next())
            {
                property_data_item_t property_data_item = { query_property.value("name").toString(), query_property.value("value").toString() };
                property_list << property_data_item;
            }

            if(!property_list.isEmpty())
                journal->setRowData(row, QVariant::fromValue(property_list));
        }
        else if(journal_type == "HALFHOUR")
        {
            QString type       = query.value("type").toString();
            int     id_journal = query.value("id_journal").toInt();
            QString time_reset = query.value("time_reset").toString();

            journal->setItem(row, 4, new CTableWidgetItem(type));
            journal->setItem(row, 5, new CTableWidgetItem(time_reset));

            // формирование запроса для получения свойств записи
            QSqlQuery query_property(*db);

            if(!query_property.exec(QString("SELECT value FROM propertyJournalHALFHOUR WHERE id_journal=%1;").arg(id_journal)))
            {
                outLogMessage(tr("Не удалось прочитать свойства журнала получасовок: %1").arg(query_property.lastError().text()));
                continue;
            }

            halfhour_t halfhour;

            while(query_property.next())
            {
                halfhour.values << query_property.value("value").toFloat();
            }

            if(!halfhour.values.isEmpty() && (!type.isEmpty() && type.toUpper() == tr("ДАННЫЕ")))
                journal->setRowData(row, QVariant::fromValue(halfhour));
        }
    }

    journal->resizeColumnsToContents();
    journal->horizontalHeader()->setStretchLastSection(true);
    m_progressbar->increment(3);

    return true;
}
/*!
 * \brief ConfiguratorWindow::loadPurposeToProject
 * \param table Таблица привязок
 * \param type Тип таблицы привязок
 */
void ConfiguratorWindow::loadPurposeToProject(CPurposeTableView* table, const QString& type, QSqlDatabase *db)
{
    if(!table || !db || (db && !db->isOpen()) || type.isEmpty())
    {
        outLogMessage(tr("Загрузка привязок: Файл проекта не создан, либо закрыт"));
        return;
    }

    CMatrixPurposeModel* model = static_cast<CMatrixPurposeModel*>(table->model());

    if(!model)
    {
        outLogMessage(tr("Загрузка привязок: Невозможно обратиться к модели представления"));
        return;
    }

    CMatrix& matrix = model->matrix();
    QSqlQuery query(*db);
    QString query_str = QString("SELECT * FROM purpose%1;").arg(type);

    if(!query.exec(query_str))
    {
        outLogMessage(tr("Загрузка привязок: не удалось прочитать привязки и БД: %1").arg(query.lastError().text()));
    }

    int row = 0;

    while(query.next())
    {
        for(int col = 0; col < matrix.columnCount(); col++)
        {
            QString colName = QString("col%1").arg(col);
            StateType state = static_cast<StateType>(query.value(colName).toInt());

            matrix[row][col].data().state = state;
        }

        row++;
    }

    model->updateData();
    m_progressbar->increment(3);
}
/*!
 * \brief ConfiguratorWindow::loadDeviceSetToProject
 * \param index Индекс пункта меню устройства
 * \param tableName Имя таблицы в БД
 *
 * Загрузка уставок из БД
 */
void ConfiguratorWindow::loadDeviceSetToProject(ConfiguratorWindow::DeviceMenuItemType index, const QString& tableName, QSqlDatabase *db)
{
    if(!db || (db && !db->isOpen()) || index == DEVICE_MENU_ITEM_NONE || tableName.isEmpty())
        return;

    CDeviceMenuTableWidget* table = groupMenuWidget(index);

    if(!table)
        return;

    QSqlQuery query (*db);
    QString query_str = QString("SELECT * FROM deviceSet%1;").arg(tableName);

    if(!query.exec(query_str))
    {
        outLogMessage(tr("Загрузка уставок: не удалось загрузить уставки из файла проекта для таблицы %1: %2").arg(tableName).arg(query.lastError().text()));
        return;
    }

    bool isLoad = false;

    for(int row = 0; row < table->rowCount(); row++)
    {
        QWidget* widget = groupMenuCellWidget(table, row, 1);

        if(!widget)
            continue;

        if(!query.next())
            continue;

        QString val = query.value("val").toString();
        QString type = query.value("type").toString();

        if(val.isEmpty() || type.isEmpty())
            continue;

        if(!isLoad)
            isLoad = true;

        if(type.toUpper() == "COMBOBOX" && widget->objectName().toUpper().contains("COMBOBOX"))
        {
            CMenuComboBox* comboBox = static_cast<CMenuComboBox*>(widget);

            if(comboBox)
            {
                int i = val.toInt();

                if(i < comboBox->count())
                    comboBox->setCurrentIndex(i);

                comboBox->resetIsEdit();
            }
        }
        else if(type.toUpper() == "LINEEDIT" && widget->objectName().toUpper().contains("LINEEDIT"))
        {
            CLineEdit* lineEdit = static_cast<CLineEdit*>(widget);

            if(lineEdit)
                lineEdit->setText(val);

            lineEdit->resetIsEdit();
        }
    }

    if(!isLoad)
    {
        outLogMessage(tr("Загрузка уставок: не удалось загрузить уставки из файла проекта для таблицы %1").arg(tableName));
    }

    m_progressbar->increment(3);
}
/*!
 * \brief ConfiguratorWindow::loadDeviceCommunication
 *
 * Загрузка настроек связи из файла проекта
 */
void ConfiguratorWindow::loadDeviceCommunication(QSqlDatabase *db)
{
    if(!db || (db && !db->isOpen()))
        return;

    QSqlQuery query(*db);
    QString query_str = QString("SELECT * FROM deviceCommunication;");

    if(!query.exec(query_str))
    {
        outLogMessage(tr("Загрузка настроек связи: не удалось загрузить из БД (%1)").arg(query.lastError().text()));
        return;
    }

    if(!query.first())
    {
        outLogMessage(tr("Загрузка настроек связи: не удалось загрузить из БД (%1)").arg(query.lastError().text()));
        return;
    }

//    int address = query.value("address").toInt();
    int speed = query.value("speed").toInt();
//    int parity = query.value("parity").toInt();
//    int Trequest = query.value("Trequest").toInt();
//    int Tspeed = query.value("Tspeed").toInt();

    ui->comboBoxCommunicationBaudrate->setCurrentIndex(speed);

    m_progressbar->increment(3);
}
/*!
 * \brief ConfiguratorWindow::loadDeviceCalibrationCurrent
 *
 * Загрузка настроек калибровки из файла проекта
 */
void ConfiguratorWindow::loadDeviceCalibrationCurrent(QSqlDatabase *db)
{
    if(!db || (db && !db->isOpen()))
        return;

    QSqlQuery query(*db);
    QString query_str = QString("SELECT * FROM deviceCalibrationCurrent;");

    if(!query.exec(query_str))
    {
        outLogMessage(tr("Загрузка настроек калиброки по току: не удалось загрузить из БД (%1)").arg(query.lastError().text()));
        return;
    }

    if(!query.first())
    {
        outLogMessage(tr("Загрузка настроек калиброки по току: не удалось загрузить из БД (%1)").arg(query.lastError().text()));
        return;
    }

    QString standardPhase = query.value("standardPhase").toString();
    QString standard3I0 = query.value("standard3I0").toString();
    bool stateIa = query.value("stateIa").toBool();
    bool stateIb = query.value("stateIb").toBool();
    bool stateIc = query.value("stateIc").toBool();
    bool state3I0 = query.value("state3I0").toBool();
    int dataCount = query.value("dataCount").toInt();
    int pauseRequest = query.value("pauseRequest").toInt();

    ui->widgetCalibrationOfCurrent->setStandardPhase(QLocale::system().toFloat(standardPhase));
    ui->widgetCalibrationOfCurrent->setStandard3I0(QLocale::system().toFloat(standard3I0));
    ui->widgetCalibrationOfCurrent->setIaState(stateIa);
    ui->widgetCalibrationOfCurrent->setIbState(stateIb);
    ui->widgetCalibrationOfCurrent->setIcState(stateIc);
    ui->widgetCalibrationOfCurrent->set3I0State(state3I0);
    ui->widgetCalibrationOfCurrent->setDataCount(dataCount);
    ui->widgetCalibrationOfCurrent->setPauseRequest(pauseRequest);

    m_progressbar->increment(3);
}
/*!
 * \brief ConfiguratorWindow::loadDeviceCalibrationPowerAC
 *
 * Загрузка настроек калибровки по переменному напряжению из файла проекта
 */
void ConfiguratorWindow::loadDeviceCalibrationPowerAC(QSqlDatabase *db)
{
    if(!db || (db && !db->isOpen()))
        return;

    QSqlQuery query(*db);
    QString query_str = QString("SELECT * FROM deviceCalibrationPowerAC;");

    if(!query.exec(query_str))
    {
        outLogMessage(tr("Загрузка настроек калиброки по напряжению AC: не удалось загрузить из БД (%1)").arg(query.lastError().text()));
        return;
    }

    if(!query.first())
    {
        outLogMessage(tr("Загрузка настроек калиброки по напряжению AC: не удалось загрузить из БД (%1)").arg(query.lastError().text()));
        return;
    }

    QString standardPhase = query.value("standardPhase").toString();
    QString standardInterPhase = query.value("standardInterPhase").toString();
    QString standardMultiplier = query.value("standardMultiplier").toString();
    bool stateUa = query.value("stateUa").toBool();
    bool stateUb = query.value("stateUb").toBool();
    bool stateUc = query.value("stateUc").toBool();
    bool stateUab = query.value("stateUab").toBool();
    bool stateUbc = query.value("stateUbc").toBool();
    bool stateUca = query.value("stateUca").toBool();
    bool state3U0S = query.value("state3U0S").toBool();
    bool state3US = query.value("state3US").toBool();
    bool state3U0 = query.value("state3U0").toBool();
    int dataCount = query.value("dataCount").toInt();
    int pauseRequest = query.value("pauseRequest").toInt();

    ui->widgetCalibrationPower->setStandardPhase(QLocale::system().toFloat(standardPhase));
    ui->widgetCalibrationPower->setStandardPhaseLinear(QLocale::system().toFloat(standardInterPhase));
    ui->widgetCalibrationPower->setStandard3U(QLocale::system().toFloat(standardMultiplier));
    ui->widgetCalibrationPower->setStateUa(stateUa);
    ui->widgetCalibrationPower->setStateUb(stateUb);
    ui->widgetCalibrationPower->setStateUc(stateUc);
    ui->widgetCalibrationPower->setStateUab(stateUab);
    ui->widgetCalibrationPower->setStateUbc(stateUbc);
    ui->widgetCalibrationPower->setStateUca(stateUca);
    ui->widgetCalibrationPower->setState3U0S(state3U0S);
    ui->widgetCalibrationPower->setState3US(state3US);
    ui->widgetCalibrationPower->setState3U0(state3U0);
    ui->widgetCalibrationPower->setDataCount(dataCount);
    ui->widgetCalibrationPower->setPauseRequest(pauseRequest);

    m_progressbar->increment(3);
}
/*!
 * \brief ConfiguratorWindow::loadDeviceCalibrationCurrent
 *
 * Загрузка настроек калибровки БРУ по сопротивлению из файла проекта
 */
void ConfiguratorWindow::loadDeviceCalibrationBruResistance(QSqlDatabase *db)
{
    if(!db || (db && !db->isOpen()))
        return;

    QSqlQuery query(*db);
    QString query_str = QString("SELECT * FROM deviceCalibrationBruResistance;");

    if(!query.exec(query_str))
    {
        outLogMessage(tr("Загрузка настроек калиброки БРУ по сопротивлению: не удалось загрузить из БД (%1)").arg(query.lastError().text()));
        return;
    }

    if(!query.first())
    {
        outLogMessage(tr("Загрузка настроек калиброки БРУ по сопротивлению: не удалось загрузить из БД (%1)").arg(query.lastError().text()));
        return;
    }

    QString Rmin = query.value("Rmin").toString();
    QString Rmax = query.value("Rmax").toString();
    bool stateRa = query.value("stateRa").toBool();
    bool stateRb = query.value("stateRb").toBool();
    bool stateRc = query.value("stateRc").toBool();

    ui->widgetCalibrationBRUResistance->setStandardPhaseMin(QLocale::system().toFloat(Rmin));
    ui->widgetCalibrationBRUResistance->setStandardPhaseMax(QLocale::system().toFloat(Rmax));
    ui->widgetCalibrationBRUResistance->setStateRa(stateRa);
    ui->widgetCalibrationBRUResistance->setStateRa(stateRb);
    ui->widgetCalibrationBRUResistance->setStateRa(stateRc);

    m_progressbar->increment(3);
}
/*!
 * \brief ConfiguratorWindow::loadDeviceCalibrationCurrent
 *
 * Загрузка настроек калибровки БРУ по постоянному напряжению из файла проекта
 */
void ConfiguratorWindow::loadDeviceCalibrationBruPowerDC(QSqlDatabase *db)
{
    if(!db || (db && !db->isOpen()))
        return;

    QSqlQuery query(*db);
    QString query_str = QString("SELECT * FROM deviceCalibrationBruPowerDC;");

    if(!query.exec(query_str))
    {
        outLogMessage(tr("Загрузка настроек калиброки БРУ по напряжению DC: не удалось загрузить из БД (%1)").arg(query.lastError().text()));
        return;
    }

    if(!query.first())
    {
        outLogMessage(tr("Загрузка настроек калиброки БРУ по напряжению DC: не удалось загрузить из БД (%1)").arg(query.lastError().text()));
        return;
    }

    QString phaseMin = query.value("PhaseMin").toString();
    QString phaseMax = query.value("PhaseMax").toString();
    QString multiplierMin = query.value("MultiplierMin").toString();
    QString multiplierMax = query.value("MultiplierMax").toString();
    bool stateUa = query.value("stateUa").toBool();
    bool stateUb = query.value("stateUb").toBool();
    bool stateUc = query.value("stateUc").toBool();
    bool stateUU = query.value("stateUab").toBool();
    int dataCount = query.value("dataCount").toInt();
    int pauseRequest = query.value("pauseRequest").toInt();

    ui->widgetCalibrationBRUPowerDC->setPhaseMin(QLocale::system().toFloat(phaseMin));
    ui->widgetCalibrationBRUPowerDC->setPhaseMax(QLocale::system().toFloat(phaseMax));
    ui->widgetCalibrationBRUPowerDC->setPhaseMultiplierMin(QLocale::system().toFloat(multiplierMin));
    ui->widgetCalibrationBRUPowerDC->setPhaseMultiplierMax(QLocale::system().toFloat(multiplierMax));
    ui->widgetCalibrationBRUPowerDC->setStateUa(stateUa);
    ui->widgetCalibrationBRUPowerDC->setStateUb(stateUb);
    ui->widgetCalibrationBRUPowerDC->setStateUc(stateUc);
    ui->widgetCalibrationBRUPowerDC->setStateMultiplier(stateUU);
    ui->widgetCalibrationPower->setDataCount(dataCount);
    ui->widgetCalibrationPower->setPauseRequest(pauseRequest);

    m_progressbar->increment(3);
}
/*!
 * \brief ConfiguratorWindow::loadContainerSettings
 * \param Указатель на контейнер
 * Загрузка настроек контейнеров
 */
void ConfiguratorWindow::loadContainerSettings(CContainerWidget* container, QSqlDatabase *db)
{
    if(!container || !db || (db && !db->isOpen()))
        return;

    QSqlQuery query(*db);
    QString query_str = QString("SELECT * FROM containerSettings WHERE name=\'%1\';").arg(container->name());

    if(!query.exec(query_str))
    {
        outLogMessage(tr("Загрузка настроек контейнера: не удалось загрузить настройки из БД (%1)").arg(query.lastError().text()));
        return;
    }

    if(!query.first())
    {
        outLogMessage(tr("Загрузка настроек контейнера: не удалось загрузить настройки из БД (%1)").arg(query.lastError().text()));
        return;
    }

    QString side = query.value("side").toString();
    bool visible = query.value("visible").toBool();
    int position = query.value("pos").toInt();
    QByteArray ba = query.value("geometry").toByteArray();

    container->setVisible(visible);
    container->setPosition(position);
    container->restoreGeometry(ba);

    if(side.toUpper() == "LEFT")
        ui->dockWidgetMenuDevice->addContainer(container);
    else if(side.toUpper() == "RIGHT")
        ui->dockWidgetVariable->addContainer(container);
    else if(side.toUpper() == "BOTTOM")
    {
        ui->tabWidgetMessage->addContainer(container);
    }

    m_progressbar->increment(4);
}
/*!
 * \brief ConfiguratorWindow::blockInterface
 *
 * Блокировка интерфейса
 */
void ConfiguratorWindow::blockInterface()
{
    m_isProject = false;

//    ui->splitterCentralWidget->setDisabled(true);
    ui->tabwgtMenu->setCurrentIndex(TAB_FILE_INDEX);
    ui->tabwgtMenu->setTabEnabled(TAB_IMPORT_EXPORT_INDEX, false);
    ui->tabwgtMenu->setTabEnabled(TAB_VIEW_INDEX, false);
//    ui->tabwgtMenu->setTabEnabled(TAB_SCREEN_INDEX, false);
    ui->tabwgtMenu->setTabEnabled(TAB_SET_INDEX, false);
    ui->tabwgtMenu->setTabEnabled(TAB_READ_WRITE_INDEX, false);
    ui->tabwgtMenu->setTabEnabled(TAB_FILTER_INDEX, false);
    ui->pbtnMenuSaveProject->setDisabled(true);
    ui->pbtnMenuSaveAsProject->setDisabled(true);
    ui->dockWidgetMenuDevice->hide();
    ui->dockWidgetVariable->hide();
    ui->framePanelMessage->hide();
    emit ui->widgetMenuBar->deactivateButtons();
    m_project_cur_path = "";

    int index_screen = m_menu_items[DEVICE_MENU_ITEM_SETTINGS_ITEM_WELLCOME_SCREEN];
    ui->stwgtMain->setCurrentIndex(index_screen); // вызываем экран приветствия
}
/*!
 * \brief ConfiguratorWindow::unblockInterface
 *
 * Разблокировка интерфеса программы
 */
void ConfiguratorWindow::unblockInterface()
{
    m_isProject = true;

//    ui->splitterCentralWidget->setEnabled(true);
    ui->tabwgtMenu->setTabEnabled(TAB_IMPORT_EXPORT_INDEX, true);
    ui->tabwgtMenu->setTabEnabled(TAB_VIEW_INDEX, true);
//    ui->tabwgtMenu->setTabEnabled(TAB_SCREEN_INDEX, true);
    ui->tabwgtMenu->setTabEnabled(TAB_SET_INDEX, true);
    ui->tabwgtMenu->setTabEnabled(TAB_READ_WRITE_INDEX, true);
    ui->tabwgtMenu->setTabEnabled(TAB_FILTER_INDEX, true);
    ui->tabwgtMenu->setCurrentIndex(TAB_SET_INDEX);
    ui->pbtnMenuSaveProject->setEnabled(true);
    ui->pbtnMenuSaveAsProject->setEnabled(true);
    ui->dockWidgetMenuDevice->show();
    ui->dockWidgetVariable->show();
    ui->framePanelMessage->show();
    emit ui->widgetMenuBar->activateButtons();
    containerVisibleState();
}
/*!
 * \brief ConfiguratorWindow::clearTableDB
 * \param db Указатель на базу данных
 * \param tableName Название таблицы
 * \return Истина, если таблица успешно очищена
 *
 * Очистка таблицы в базе данных
 */
bool ConfiguratorWindow::clearTableDB(const QSqlDatabase* db, const QString& tableName)
{
    if(!db || (db && !db->isOpen()) || tableName.isEmpty())
        return false;

    QSqlQuery query(*db);

    if(!query.exec(QString("DELETE FROM %1;").arg(tableName)))
        return false;

    return true;
}
/*!
 * \brief ConfiguratorWindow::showMessageBox
 * \param title Заголовок сообщения
 * \param text Текст сообщения
 * \param type Тип сообщения
 * \return Выбор пользователя
 *
 * Вывод сообщения о каком-либо событии или вопрос для реакции пользователя
 */
int ConfiguratorWindow::showMessageBox(const QString& title, const QString& text, const QMessageBox::Icon type)
{
    QMessageBox msgbox;
    msgbox.setWindowTitle(title);
    msgbox.setWindowIcon(QIcon(QPixmap(":/images/resource/images/configurator.png")));
    msgbox.setText(text);
    msgbox.setIcon(type);

    if(type == QMessageBox::Question)
    {
        msgbox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgbox.setButtonText(QMessageBox::Yes, tr("Да"));
        msgbox.setButtonText(QMessageBox::No, tr("Нет"));
    }
    else
    {
        outLogMessage(msgbox.text());
    }

    return msgbox.exec();
}
//---------------------------------------------------
JournalPtr ConfiguratorWindow::currentJournalWidget()
{
    DeviceMenuItemType item = menuIndex();
    JournalPtr journal = nullptr;

    switch(item)
    {
        case DEVICE_MENU_ITEM_JOURNALS_CRASHES:
            journal = m_journal_crash;
        break;

        case DEVICE_MENU_ITEM_JOURNALS_EVENTS:
            journal = m_journal_event;
        break;

        case DEVICE_MENU_ITEM_JOURNALS_HALF_HOURS:
            journal = m_journal_halfhour;
        break;

        case DEVICE_MENU_ITEM_JOURNALS_ISOLATION:
            journal = m_journal_isolation;
        break;

        default:
            journal = nullptr;
        break;
    }

    return journal;
}
//------------------------------------------------------------
void ConfiguratorWindow::outLogMessage(const QString& message)
{
    qInfo() << message;
    outApplicationEvent(message);
}
//-----------------------------------------------------------
int ConfiguratorWindow::dialogJournalRead(JournalPtr journal)
{
    if(!journal)
        return -1;

    QMessageBox msgbox;
    msgbox.setWindowIcon(QIcon(QPixmap(":/images/resource/images/configurator.png")));
    msgbox.setWindowTitle(tr("Чтение журнала"));
    msgbox.setInformativeText(tr("В таблице уже присутствуют данные!\nВыберите необходимое действие."));
    msgbox.setIcon(QMessageBox::Question);

    QPushButton* next   = msgbox.addButton(tr("Продолжить"), QMessageBox::ActionRole);
    QPushButton* begin  = msgbox.addButton(tr("Сначала"), QMessageBox::ActionRole);
    QPushButton* cancel = msgbox.addButton(tr("Отмена"), QMessageBox::ActionRole);

    msgbox.setDefaultButton(next);
    msgbox.exec();

    outApplicationEvent(msgbox.text());

    if(msgbox.clickedButton() == next)
    {
        int row_next = journal->widget()->table()->rowCount();
        CTableWidgetItem* item = static_cast<CTableWidgetItem*>(journal->widget()->table()->item(journal->widget()->table()->rowCount() - 1, 0));

        if(item)
        {
            row_next = item->data(Qt::DisplayRole).toInt();
        }

        row_next++;

        CFilter &filter = journal->filter();
        filter.setLimitLow(row_next);

        return row_next;
    }
    else if(msgbox.clickedButton() == begin)
    {
        clearJournal();
    }
    else if(msgbox.clickedButton() == cancel)
    {
        ui->pushButtonJournalRead->setChecked(false);

        return - 1;
    }

    return -1;
}
//---------------------------------------------------------
void ConfiguratorWindow::endJournalRead(JournalPtr journal)
{
    ui->pushButtonJournalRead->setChecked(false);

    if(!journal)
        return;

    if(m_modbus->isConnected())
    {
        int msg_count = journal->msgCount(); // прочитано сообщений
        int msg_read = journal->msgRead(); // сколько нужно было прочитать

        QString msg;
        QString journal_type = journal->widget()->property("TYPE").toString();
        QString journal_name = (journal_type == "CRASH")?tr("Аварий"):(journal_type == "EVENT")?tr("Событий"):(journal_type == "HALFHOUR")?
                                                         tr("Получасовок"):(journal_type == "ISOLATION")?tr("Изоляций"):tr("Неизвестный");

        if(m_journal_progress)
        {
            disconnect(m_journal_progress, &JournalProgress::cancel, this, &ConfiguratorWindow::processReadJournals);
            delete m_journal_progress;
            m_journal_progress = nullptr;
        }

        if(msg_read == msg_count && msg_read > 0 && msg_count > 0) // прочитаны все сообщения
        {
            msg = tr("Чтение журнала %1 успешно завершено.\nПрочитано %2 из %3 сообщений.").
                    arg(journal_name).arg(msg_count).arg(msg_read);

            float read_time = static_cast<float>(m_time_process.elapsed())/1000.0f;

            journal->widget()->header()->setTextDeviceCountMessages(msg_count, journal->filter().rangeMaxValue());
            journal->widget()->header()->setTextTableCountMessages(journal->widget()->table()->rowCount());
            journal->widget()->header()->setTextElapsedTime(tr("%1 сек").arg(QLocale::system().toString(read_time, 'f', 1)));

            DialogJournalReadResult dialog(tr("Результаты чтения журнала %1").arg(journal_name), this);
            dialog.setValues(journal->filter().rangeMaxValue(), journal->msgRead(), read_time);
            dialog.exec();
        }
        else if(msg_count != 0 && msg_read != 0)
        {
            msg = tr("Чтение журнала %1 прервано пользователем.\nПрочитано %2 из %3 сообщений.").arg(journal_name).arg(msg_count).
                    arg(msg_read);
        }

        if(msg_count != 0 && msg_read != 0)
            outLogMessage(msg);

        timeoutSynchronization(); // включаем синхронизацию

        if(ui->checkboxCalibTimeout->isChecked()) // включаем чтение расчетных величин, если было запущено
            chboxCalculateTimeoutStateChanged(true);

        readJournalCount(journal);
    }

    journal->reset();
}
//---------------------------------------------------------
QString ConfiguratorWindow::journalName(JournalPtr journal)
{
    if(!journal)
        return "";

    QString journal_type = journal->widget()->property("TYPE").toString();
    QString journal_name = (journal_type == "CRASH")?tr("Аварий"):(journal_type == "EVENT")?tr("Событий"):(journal_type == "HALFHOUR")?tr("Получасовок"):
                           (journal_type == "ISOLATION")?tr("Изоляций"):tr("Неизвестный");

    return journal_name;
}
//---------------------------------------------------------------------
JournalPtr ConfiguratorWindow::journalWidgetByType(const QString &type)
{
    if(type.isEmpty())
        return nullptr;

    JournalPtr journal = nullptr;

    if(type == "CRASH")
        journal = m_journal_crash;
    else if(type == "EVENT")
        journal = m_journal_event;
    else if(type == "HALFHOUR")
        journal = m_journal_halfhour;
    else if(type == "ISOLATION")
        journal = m_journal_isolation;
    else
        journal = nullptr;

    return journal;
}
//----------------------------------------------------------------------------
CPurposeTableView *ConfiguratorWindow::purposeTableByType(const QString &type)
{
    if(type.isEmpty())
        return nullptr;

    CPurposeTableView *table = nullptr;

    if(type == "INPUT")
        table = ui->tablewgtDiscreteInputPurpose;
    else if(type == "RELAY")
        table = ui->tablewgtRelayPurpose;
    else if(type == "LED")
        table = ui->tablewgtLedPurpose;
    else if(type == "PROTECTION")
        table = ui->tablewgtProtectionCtrl;
    else
        table = nullptr;

    return table;
}
//------------------------------------------------------------------
void ConfiguratorWindow::openProject(const QString &projectPathName)
{
    if(projectPathName.isEmpty())
        return;

    QSqlDatabase *db = nullptr;

    if(!connectDb(db, projectPathName))
    {
        QMessageBox::warning(this, tr("Открытие проекта"), tr("Не удалось открыть проект: %1.\n Попробуйте еще раз.").
                             arg(projectPathName));
        disconnectDb(db);
        m_progressbar->progressStop();
        return;
    }

    m_progressbar->setProgressTitle(tr("Открытие проекта"));
    m_progressbar->setSettings(0, 100, "%");
    m_progressbar->progressStart();

    loadJournalFromProject(ui->widgetJournalEvent, db); // Загрузка журнала событий
    loadJournalFromProject(ui->widgetJournalCrash, db); // Загрузка журнала аварий
    loadJournalFromProject(ui->widgetJournalHalfHour, db); // Загрузка журнала получасовок
    loadPurposeToProject(ui->tablewgtLedPurpose, "LED", db);
    loadPurposeToProject(ui->tablewgtRelayPurpose, "RELAY", db);
    loadPurposeToProject(ui->tablewgtDiscreteInputPurpose, "INPUT", db);
    loadPurposeToProject(ui->tablewgtProtectionCtrl, "PROTECTION", db);
    loadDeviceSetToProject(DEVICE_MENU_ITEM_SETTINGS_ITEM_IN_ANALOG, "ANALOG", db);
    loadDeviceSetToProject(DEVICE_MENU_PROTECT_ITEM_CURRENT, "MTZ", db);
    loadDeviceSetToProject(DEVICE_MENU_PROTECT_ITEM_POWER, "PWR", db);
    loadDeviceSetToProject(DEVICE_MENU_PROTECT_ITEM_LEAK, "DIR", db);
    loadDeviceSetToProject(DEVICE_MENU_PROTECT_ITEM_FREQUENCY, "FREQ", db);
    loadDeviceSetToProject(DEVICE_MENU_PROTECT_ITEM_EXTERNAL, "EXT", db);
    loadDeviceSetToProject(DEVICE_MENU_PROTECT_ITEM_TEMPERATURE, "TEMP", db);
    loadDeviceSetToProject(DEVICE_MENU_PROTECT_ITEM_RESERVE, "RESERVE", db);
    loadDeviceSetToProject(DEVICE_MENU_PROTECT_ITEM_CONTROL, "CTRL", db);
    loadDeviceSetToProject(DEVICE_MENU_ITEM_AUTOMATION_SWITCH, "AUTO_SWITCH", db);
    loadDeviceSetToProject(DEVICE_MENU_ITEM_AUTOMATION_APV, "AUTO_APV", db);
    loadDeviceSetToProject(DEVICE_MENU_ITEM_AUTOMATION_AVR, "AUTO_AVR", db);
    loadDeviceSetToProject(DEVICE_MENU_ITEM_AUTOMATION_KCN, "AUTO_KCN", db);
    loadDeviceCommunication(db);
    loadDeviceCalibrationCurrent(db);
    loadDeviceCalibrationPowerAC(db);
    loadDeviceCalibrationBruResistance(db);
    loadDeviceCalibrationBruPowerDC(db);
    loadContainerSettings(m_containerWidgetVariable, db);
    loadContainerSettings(m_containerWidgetDeviceMenu, db);
    loadContainerSettings(m_containerIndicatorState, db);
    loadContainerSettings(m_containerMonitorI11I17, db);
    loadContainerSettings(m_containerOutputAll, db);
    loadContainerSettings(m_containerInputs, db);
    loadContainerSettings(m_containerDebugInfo, db);
    loadContainerSettings(m_containerStatusInfo, db);
    loadContainerSettings(m_containerWidgetMessage, db);
    loadContainerSettings(m_containerTerminalModbus, db);

    disconnectDb(db);

    unblockInterface();
    outLogMessage(tr("Файл проекта успешно загружен: %1").arg(projectPathName));
    m_progressbar->progressStop();

    m_project_cur_path = projectPathName;
    emit ui->widgetMenuBar->widgetMenu()->addOpenDocument(projectPathName);

    m_is_set_change = false;
}
//--------------------------------------------------------------------------------
int ConfiguratorWindow::rowSheetExcel(QXlsx::Document &xlsx, QStringList &columns)
{
    for(int col = 1; col <= columns.count(); col++)
    {
        QString col_name = columns.at(col - 1);

        if(xlsx.read(1, col).toString().toUpper() != col_name.toUpper())
        {
            QString text = tr("Ошибка импорта матрицы привязок:\nКолонка (1, %1) не содержит имя \"%2\"(страница \'%3\')!").arg(col).
                    arg(col_name).arg(xlsx.currentWorksheet()->sheetName());
            m_popup->setPopupText(text);
            outLogMessage(text);
            return -1;
        }
    }

    int row = 2;
    int row_count = 0;

    while(!xlsx.read(row++, 1).toString().isEmpty()) { row_count++; }

    return row_count;
}
/*!
 * \brief ConfiguratorWindow::bitByVariableName
 * \param key Имя переменной для которой производится поиск номера бита
 * \return номер бита или -1, если не найден
 */
int ConfiguratorWindow::bitByVariableName(const QString &key)
{
    if(key.isEmpty())
        return -1;

    if(m_variable_bits.find(key) == m_variable_bits.end())
        return -1;

    return m_variable_bits[key];
}
//--------------------------------------------------------------------------------------------------------------------------------------
void ConfiguratorWindow::sendSettingReadRequest(const QString& first, const QString& last, CModBusDataUnit::FunctionType type, int size,
                                                DeviceMenuItemType index)
{
    if(size <= 0)
        return;

    int addr = addressSettingKey(first);

    if(addr == -1)
    {
        qWarning() << tr("Переменной \'%1\' не назначен адрес.").arg(first);
        return;
    }

    CModBusDataUnit unit(quint8(m_serialPortSettings_window->deviceID()), type, quint16(addr), QVector<quint16>() << quint16(size));

    unit.setProperty("REQUEST", GENERAL_TYPE);
    unit.setProperty("FIRST", first);
    unit.setProperty("LAST", last);
    unit.setProperty("GROUP", index);
qDebug() << QString("Чтение уставки по ключу: %1 (адрес: %2)").arg(first).arg(addr);
    m_modbus->sendData(unit);
}
//--------------------------------------------------------------------------------------------------------------
void ConfiguratorWindow::sendSettingReadRequest(const QStringList &key_list, CModBusDataUnit::FunctionType type,
                                                ConfiguratorWindow::DeviceMenuItemType index, bool isCalibration)
{
    if(key_list.isEmpty())
        return;

    for(QString key: key_list)
    {
        int addr = addressSettingKey(key);

        if(addr == -1)
        {
            qWarning() << tr("Переменной \'%1\' не назначен адрес.").arg(key);
            continue;
        }

        CModBusDataUnit unit(quint8(m_serialPortSettings_window->deviceID()), type, quint16(addr), 2);
        unit.setProperty("REQUEST", GENERAL_TYPE);
        unit.setProperty("GROUP", index);
        unit.setProperty("FIRST", key);
        unit.setProperty("LAST", key);
        unit.setProperty("CALIBRATION", isCalibration); // калибровочный коэффициент для передачи его в модуль калибровок (тока, напряжения и т.д. и т.п.)
qDebug() << QString("Чтение уставки по ключу: %1 (адрес: %2)").arg(key).arg(addr);
        m_modbus->sendData(unit);
    }
}
//----------------------------------------------------------------------------------------------------------
void ConfiguratorWindow::sendSettingControlReadRequest(const QString& index, DeviceMenuItemType group_index)
{
    int addr = addressSettingKey(index);

    if(addr == -1)
    {
        qWarning() << tr("Переменной \'%1\' не назначен адрес.").arg(index);
        return;
    }

    CModBusDataUnit unit(quint8(m_serialPortSettings_window->deviceID()), CModBusDataUnit::ReadHoldingRegisters,
                         quint16(addr), QVector<quint16>() << 1);

    unit.setProperty("REQUEST", GENERAL_CONTROL_TYPE);
    unit.setProperty("REQUEST_FUNCTION", FUNCTION_READ);
    unit.setProperty("INDEX", index);
    unit.setProperty("GROUP", group_index);
qDebug() << QString("Чтение уставки по ключу: %1 (адрес: %2)").arg(index).arg(addr);
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

    CMenuComboBox* comboBox = qobject_cast<CMenuComboBox*>(groupMenuCellWidgetByName(table, nameWgt, 1));

    if(!comboBox)
        return;

    int addr = addressSettingKey(index);

    if(addr == -1)
    {
        qWarning() << tr("Переменной \'%1\' не назначен адрес.").arg(index);
        return;
    }

    quint16 value = quint16(comboBox->currentIndex());

    if(index.toUpper() != "TZ") // токозависимые характеристики учитывают и ноль, остальные с единицы
        value++;

//    sendDeviceCommand(45); // отправка команды на снятие ключа блокировки записи привязок

    CModBusDataUnit unit(quint8(m_serialPortSettings_window->deviceID()), CModBusDataUnit::WriteSingleRegister, quint16(addr),
                         QVector<quint16>() << value);
    unit.setProperty("REQUEST", GENERAL_CONTROL_TYPE);
    unit.setProperty("REQUEST_FUNCTION", FUNCTION_SAVE);
    unit.setProperty("INDEX", index);
qDebug() << QString("Запись уставки по ключу: %1 (адрес: %2)").arg(index).arg(addr);
    m_modbus->sendData(unit);
}
//-----------------------------------------------------------------------------------------------------------------------------------
void ConfiguratorWindow::sendSettingWriteRequest(const QString& first, const QString& last, DeviceMenuItemType group_index, int size)
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

        CLineEdit* lineEdit = qobject_cast<CLineEdit*>(groupMenuCellWidgetByName(table, nameWgt, 1));

        if(!lineEdit)
        {
            nameWgt += "_1";
            lineEdit = qobject_cast<CLineEdit*>(groupMenuCellWidgetByName(table, nameWgt, 1));

            if(!lineEdit)
                continue;
        }

        value.f = QLocale::system().toFloat(lineEdit->text());

        if(size == 2)
        {
            data.append(value.w[1]);
            data.append(value.w[0]);
        }
        else
        {
            data.append(lineEdit->text().toInt());
        }
    }

    if(data.isEmpty())
        return;

    CModBusDataUnit::FunctionType funType = ((data.count() == 1)?CModBusDataUnit::WriteSingleRegister:
                                                               CModBusDataUnit::WriteMultipleRegisters);

    int addr = addressSettingKey(first);

    if(addr == -1)
    {
        qWarning() << tr("Переменной \'%1\' не назначен адрес.").arg(first);
        return;
    }

//    sendDeviceCommand(45); // установка ключа для записи уставок

    CModBusDataUnit unit(quint8(m_serialPortSettings_window->deviceID()), funType, quint16(addr), data);

    unit.setProperty(tr("FIRST"), first);
    unit.setProperty(tr("LAST"), last);

    QString val;

    for(quint16 v: data)
        val += QString("%1 ").arg(v);

qDebug() << QString("Запись уставки: %1, значение: { %2 }, адрес: { %3 }").arg(first).arg(val).arg(addr);

    m_modbus->sendData(unit);
}
//----------------------------------------------------------------------------------------
void ConfiguratorWindow::sendPurposeReadRequest(const QString& first, const QString& last)
{
    int faddr = addressPurposeKey(first);
    int laddr = addressPurposeKey(last);

    if(faddr == -1 || laddr == -1)
        return;

    int size = laddr - faddr + 20; // получаем размер считываемого блока с учетом выравнивания в 40 байт (одна строка)

    CModBusDataUnit unit(quint8(m_serialPortSettings_window->deviceID()), CModBusDataUnit::ReadHoldingRegisters, quint16(faddr),
                         QVector<quint16>() << quint16(size));

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

    int hword_size = (eIndex - bIndex + 1)*20;

    QVector<quint16> data = QVector<quint16>(hword_size, 0); // создаем вектор размерностью hword_size полуслов со значением 0

    for(int i = 0; i <= (eIndex - bIndex); i++) // обход колонок
    {
        int index      = i + bIndex;
        int offset_pos = i*20;

        for(int j = 1; j < matrix.rowCount(); j++)
        {
            quint16 hword = quint16(matrix[j].data().position/16);
            quint16 bit   = quint16(matrix[j].data().position%16);

            StateType state = matrix[j][index].data().state;

            if(state == CHECKED)
                data[hword + offset_pos] |= (1 << bit);
        }
    }

    QVector<quint16> values;

    for(int i = 0; i < (data.count() - 1); i += 2)
        values << data[i + 1] << data[i];

    sendDeviceCommand(45); // отправка команды на снятие ключа блокировки записи привязок

    CModBusDataUnit::FunctionType funType = ((values.count() == 1)?CModBusDataUnit::WriteSingleRegister:
                                                                   CModBusDataUnit::WriteMultipleRegisters);

    CModBusDataUnit unit(quint8(m_serialPortSettings_window->deviceID()), funType, quint16(addressPurposeKey(first)), values);

    unit.setProperty(tr("FIRST"), first);
    unit.setProperty(tr("LAST"), last);

    m_modbus->sendData(unit);
}
//------------------------------------------------------------------------------
void ConfiguratorWindow::sendPurposeDIReadRequest(int first_addr, int last_addr)
{
    int size = last_addr - first_addr + 2;

    CModBusDataUnit unit(quint8(m_serialPortSettings_window->deviceID()), CModBusDataUnit::ReadHoldingRegisters, quint16(first_addr),
                         QVector<quint16>() << quint16(size));

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
    int eIndex = (676 - last_addr)/2;

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
                {
                    value |= (1 << k);
                }
            }

            values << static_cast<quint16>((value&0xFFFF0000) >> 16) << static_cast<quint16>(value&0x0000FFFF);
        }
        else if(key.contains("RESERVE") || row_index == -1)
        // если имя переменной содержит слово "RESERVE", т.е. переменная является резервной или переменная не найдена в таблице,
        // то заполяем значение состояниями привязок нулями
        {
            values << static_cast<quint16>(0) << static_cast<quint16>(0);
        }
    }

    sendDeviceCommand(45); // отправка команды на снятие ключа блокировки записи привязок

    CModBusDataUnit::FunctionType funType = ((values.count() == 1)?CModBusDataUnit::WriteSingleRegister:
                                                                 CModBusDataUnit::WriteMultipleRegisters);

    CModBusDataUnit unit(quint8(m_serialPortSettings_window->deviceID()), funType, quint16(first_addr), values);

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
    int eIndex = (932 - last_addr)/2;

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

            values << static_cast<quint16>((value&0xFFFF0000) >> 16) << static_cast<quint16>(value&0x0000FFFF);
        }
        else if(key.contains("_R")) // если имя переменной оканчивается на "_R", т.е. переменная является резервной, то заполяем значение состояниями привязок нулями
        {
            values << static_cast<quint16>(0) << static_cast<quint16>(0);
        }
    }

    sendDeviceCommand(45); // отправка команды на снятие ключа блокировки записи привязок

    CModBusDataUnit::FunctionType funType = ((values.count() == 1)?CModBusDataUnit::WriteSingleRegister:
                                                                   CModBusDataUnit::WriteMultipleRegisters);

    CModBusDataUnit unit(quint8(m_serialPortSettings_window->deviceID()), funType, quint16(first_addr), values);

    unit.setProperty(tr("FIRST_ADDRESS"), first_addr);
    unit.setProperty(tr("LAST_ADDRESS"), last_addr);

    m_modbus->sendData(unit);
}
//---------------------------------------------------------------------------------------------------------
void ConfiguratorWindow::sendProtectionWorkModeRequest(const QString& protection, RequestFunction function,
                                                       DeviceMenuItemType group_index)
{
    int addr = addressSettingKey("I11");

    if(addr == -1)
        qWarning() << tr("Переменной \'I11\' не назначен адрес.");

//    sendDeviceCommand(45); // отправка команды на установку ключа блокировки записи уставок
    CModBusDataUnit unit(quint8(m_serialPortSettings_window->deviceID()), CModBusDataUnit::ReadHoldingRegisters, quint16(addr),
                         QVector<quint16>() << 40); // запрашиваем 2 32-битных строки сразу, т.к. за I11 идет I17, из которой нам тоже необходим бит состояния

    unit.setProperty("REQUEST", PROTECTION_WORK_MODE_TYPE);
    unit.setProperty("PROTECTION", protection);
    unit.setProperty("REQUEST_FUNCTION", function);
    unit.setProperty("GROUP", group_index);

    if(function == RequestFunction::FUNCTION_SAVE)
        qDebug() << QString("Запись режима работы блокировки защиты %1 по адресу %2").arg(protection).arg(addr);

    m_modbus->sendData(unit);
}
//---------------------------------------------------------
void ConfiguratorWindow::sendMonitorPurposeI11_I17Request()
{
    int addr = addressSettingKey("I11");

    CModBusDataUnit unit(quint8(m_serialPortSettings_window->deviceID()), CModBusDataUnit::ReadHoldingRegisters, quint16(addr),
                         QVector<quint16>() << 40);

    unit.setProperty("REQUEST", MONITOR_PURPOSE_I11_I17_TYPE);

    m_modbus->sendData(unit);
}
/*!
 * \brief ConfiguratorWindow::sendRequestRead
 * \param addr Адрес ячейки
 * \param size Количество ячеек для чтения
 */
void ConfiguratorWindow::sendRequestRead(int addr, int size, int request, CModBusDataUnit::FunctionType functionType)
{
    if(addr == -1)
    {
        qWarning() << tr("Переменной не назначен адрес.");
        return;
    }

    CModBusDataUnit unit(quint8(m_serialPortSettings_window->deviceID()), functionType, quint16(addr), QVector<quint16>() << quint16(size));

    unit.setProperty("REQUEST", request);

    m_modbus->sendData(unit);
}
/*!
 * \brief ConfiguratorWindow::sendRequestWrite
 * \param addr Адрес ячейки
 * \param data Данные для записи
 */
void ConfiguratorWindow::sendRequestWrite(int addr, QVector<quint16>& values, int request)
{
    if(addr == -1)
    {
        qWarning() << tr("Переменной не назначен адрес.");
        return;
    }

    CModBusDataUnit::FunctionType funType = ((values.count() == 1)?CModBusDataUnit::WriteSingleRegister:
                                                                 CModBusDataUnit::WriteMultipleRegisters);

    CModBusDataUnit unit(quint8(m_serialPortSettings_window->deviceID()), funType, quint16(addr), values);

    unit.setProperty("REQUST", request);

    m_modbus->sendData(unit);
}
/*!
 * \brief ConfiguratorWindow::sendSettingReadRequestVariableState
 * \param key Ключ - имя переменной для получения адреса строки (40 ячеек - 16бит) с состояниями внутренних переменных
 * \param var Имя внутренней переменной для получения бита позиции состояния этой переменной
 * \param suffix - суффикс в названии виджета (может быть кроме переменной "var" еще и суффикс ввиде "_1" или "_1_1" в зависимости от вложенности)
 * \param is_save - флаг извещающий о чтении состояний перед записью (по умолчанию false, т.е. просто чтение)
 * \param request_type итем меню для которого считываются данные
 */
void ConfiguratorWindow::sendSettingReadRequestVariableState(const QString &key, const QString var, const QString &suffix,
                                                             DeviceMenuItemType group_item, bool is_save)
{
    if(key.isEmpty() || var.isEmpty())
        return;

    int addr = addressSettingKey(key);

    if(addr == -1)
        return;

    // Запрашиваем строку матрицы (10 столбцов по 32 бита) по ключу key
    CModBusDataUnit unit(quint8(m_serialPortSettings_window->deviceID()), CModBusDataUnit::ReadHoldingRegisters, addr, 20);

    unit.setProperty("REQUEST", GENERAL_TYPE);
    unit.setProperty("GROUP", group_item);
    unit.setProperty("KEY", key);
    unit.setProperty("VARIABLE", var);
    unit.setProperty("READ_STATE", is_save);

    if(!suffix.isEmpty())
        unit.setProperty("SUFFIX", suffix);

    m_modbus->sendData(unit);
}
/*!
 * \brief ConfiguratorWindow::sendSettingWriteRequestVariableState
 * \param unit исходный запрос с прочитанными данными по набору переменных
 */
void ConfiguratorWindow::sendSettingWriteRequestVariableState(int addr, const QStringList &var_list, const QString &var_suffix, QVector<quint32> &values, CDeviceMenuTableWidget *table)
{
    if(values.count() != 10 || !table)
        return;

    for(const QString &v: var_list)
    {
        int var_bit = bitByVariableName(v);

        if(var_bit == -1)
            return;

        QString nameWgt = QString("comboBox%1").arg(v);

        if(!var_suffix.isEmpty())
            nameWgt += var_suffix;

        if(nameWgt.isEmpty())
            return;

        CMenuComboBox* comboBox = qobject_cast<CMenuComboBox*>(groupMenuCellWidgetByName(table, nameWgt, 1));

        if(!comboBox)
            return;

        int var_state = comboBox->currentIndex();
        int var_pos = var_bit/32;
        int bit_pos = var_bit%32;

        quint32& value = values[var_pos];

        value &= ~(1 << bit_pos); // очищаем бит состояния переменной
        value |= var_state << bit_pos; // устанавливаем новое состояние переменной

        qDebug() << QString("Запись состояния внутренной переменной <%1>: значение = %2, позиция переменной: %3, позиция бита: %4").arg(v).arg(value).
                    arg(var_pos).arg(bit_pos);
    }

    sendDeviceCommand(45); // установка ключа записи

    CModBusDataUnit::vlist_t data;

    // перевод 32 битных данных к 16 битным и переворот страшего и младшего полуслова
    for(int i = 0; i < values.count(); i++)
        data << quint16((values[i] >> 16)&0x0000FFFF) << quint16(values[i]&0x0000FFFF);

    CModBusDataUnit unit(quint8(m_serialPortSettings_window->deviceID()), CModBusDataUnit::WriteMultipleRegisters, addr, data);
    m_modbus->sendData(unit);

    saveDeviceSettings();
}
/*!
 * \brief ConfiguratorWindow::sendRequestCalibration
 * \param unit Сформированный объект запроса
 *
 * Отправка запроса на чтение данных по параметрам калибровки
 */
void ConfiguratorWindow::sendRequestCalibration(CModBusDataUnit &unit)
{
    unit.setID(quint8(m_serialPortSettings_window->deviceID()));
    unit.setProperty("REQUEST", CALIBRATION_PARAMETER);

    if(m_modbus->isConnected())
        m_modbus->sendData(unit);
}
/*!
 * \brief ConfiguratorWindow::sendRequestCalibrationWrite
 * \param units Список калибровок, которые выбраны для записи
 *
 * Отправка запроса на запись калибровок
 */
void ConfiguratorWindow::sendRequestCalibrationWrite(QVector<CModBusDataUnit> &units)
{
    if(!m_modbus->isConnected())
    {
        outLogMessage(tr("Невозможно отправить запрос на запись калибровок. Нет соединения с устройством."));
        return;
    }

    for(CModBusDataUnit &unit: units)
    {
        QString key = unit.property("KEY").toString();

        if(key.isEmpty())
            continue;

        int addr = addressSettingKey(key);

        if(addr == -1)
            continue;

        unit.setID(quint8(m_serialPortSettings_window->deviceID()));
        unit.setAddress(addr);
qDebug() << QString("Запись калибровочного коэффициента \"%1\": адрес = %2").arg(key).arg(addr);
        m_modbus->sendData(unit);
    }
}
/*!
 * \brief ConfiguratorWindow::sendCalibrationCalculateValues
 * \param units - список запросов расчетных величин для калибровок
 */
void ConfiguratorWindow::sendCalibrationCalculateValues(QVector<CModBusDataUnit> &units)
{
    if(units.isEmpty() || !m_modbus->isConnected())
        return;

    for(CModBusDataUnit &unit: units)
    {
        unit.setID(quint8(m_serialPortSettings_window->deviceID()));
        unit.setProperty("REQUEST", CALIBRATION_CALCULATE_VALUE);
        m_modbus->sendData(unit);
    }
}
/*!
 * \brief ConfiguratorWindow::sendDeviceCommand
 *
 * Отправка команды устройству
 */
void ConfiguratorWindow::sendDeviceCommand(int cmd)
{
    CModBusDataUnit unit(quint8(m_serialPortSettings_window->deviceID()), CModBusDataUnit::WriteSingleRegister, 0x3000,
                         QVector<quint16>() << quint16(cmd));

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

    CModBusDataUnit unit(quint8(m_serialPortSettings_window->deviceID()), CModBusDataUnit::ReadInputRegisters, 196, QVector<quint16>() << 4);

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
    CModBusDataUnit unit(quint8(m_serialPortSettings_window->deviceID()), CModBusDataUnit::ReadInputRegisters, 200, QVector<quint16>() << 2);

    unit.setProperty("REQUEST", READ_INPUTS);

    m_modbus->sendData(unit);
}
//-----------------------------------------------------
void ConfiguratorWindow::sendDebugInfoRead(int channel)
{
    CModBusDataUnit unit(quint8(m_serialPortSettings_window->deviceID()), CModBusDataUnit::ReadInputRegisters, 202 + quint16(channel)*15,
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

        case DEVICE_MENU_ITEM_SETTINGS_ITEM_WELLCOME_SCREEN:

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
    JournalPtr journal = currentJournalWidget();

    if(!journal)
        return;


    QString journal_type = journal->widget()->property("TYPE").toString();
    QString journal_name = (journal_type == "CRASH")?tr("Аварий"):(journal_type == "EVENT")?tr("Событий"):(journal_type == "HALFHOUR")?tr("Получасовок"):
                                                     tr("Неизвестный");

    journal->widget()->journalClear();
    journal->widget()->headerClear();
    journal->filter().reset();

    QString text = tr("Очистка таблицы журнала %1").arg(journal_name);
    m_status_bar->setStatusMessage(text, 2000);
    outApplicationEvent(text);

    readJournalCount(journal);
}
//-----------------------------------------------------------
void ConfiguratorWindow::startExportToPDF(JournalPtr journal)
{
    if(!journal)
    {
        showMessageBox(tr("Эксорт в PDF"), tr("Выберите текущий журнал для экспорта"), QMessageBox::Warning);
        return;
    }

    if(journal->widget()->journalIsEmpty())
    {
        showMessageBox(tr("Эксорт в PDF"), tr("Текущий журнал пуст. Экспорт невозможен."), QMessageBox::Warning);
        return;
    }

    QString journal_name = journalName(journal);
    QString sn_device    = "s/n: " + m_status_bar->serialNumberText();

    // выбираем файл для экспорта
    QDir dir;

    if(!dir.exists("outputs/reports"))
    {
        dir.mkdir("outputs/reports");
    }

    QString selectedFilter    = tr("PDF (*.pdf)");
    QString journal_full_name = tr("Журнал %1-%2 (%3 - %4)").arg(journal_name).arg(m_status_bar->serialNumberText()).
                                                             arg(QDate::currentDate().toString("dd_MM_yyyy")).
                                                             arg(QTime::currentTime().toString("HH_mm_ss"));;
    QString journal_path      = QFileDialog::getSaveFileName(this, tr("Экспорт журнала %1 в PDF").arg(journal_name),
                                                             dir.absolutePath() + QString("/outputs/reports/%1.%2").
                                                             arg(journal_full_name).arg("pdf"),
                                                             tr("PDF (*.pdf);;Все файлы (*.*)"), &selectedFilter);

    if(journal_path.isEmpty())
        return;

    m_progressbar->setProgressTitle(tr("Экспорт в PDF"));
    m_progressbar->progressStart();

    connect(m_watcher, &QFutureWatcher<void>::finished, m_progressbar, &CProgressBarWidget::progressStop);

    QFuture<void> future = QtConcurrent::run(this, &ConfiguratorWindow::exportToPDF, journal, QString(tr("Журнал %1")).arg(journal_name),
                                             sn_device, journal_path);
    m_watcher->setFuture(future);
}
//-----------------------------------------------------------------------
void ConfiguratorWindow::startMenuJournalExportToPDF(const QString &type)
{
    if(type.isEmpty())
        return;

    JournalPtr journal = journalWidgetByType(type);
    startExportToPDF(journal);
}
//-------------------------------------------------------
void ConfiguratorWindow::startCurrentJournalExportToPDF()
{
    JournalPtr journal = currentJournalWidget();
    startExportToPDF(journal);
}
//----------------------------------------------------------------------
void ConfiguratorWindow::startMenuJournalExportToDB(const QString &type)
{
    if(type.isEmpty())
        return;

    JournalPtr journal = journalWidgetByType(type);
    exportJournalToDb(journal);
}
//------------------------------------------------------------------------
void ConfiguratorWindow::startMenuJournalImportFromDB(const QString &type)
{
    if(type.isEmpty())
        return;

    JournalPtr journal = journalWidgetByType(type);

    int index = -1;

    if(type == "CRASH")
        index = 12;
    else if(type == "EVENT")
        index = 13;
    else if(type == "HALFHOUR")
        index = 14;
    else if(type == "ISOLATION")
        index = 15;

    if(index != -1)
    {
        ui->stwgtMain->setCurrentIndex(index);
        importJournalToTable(journal);
    }
}
//-------------------------------------
void ConfiguratorWindow::filterDialog()
{
    JournalPtr journal = currentJournalWidget();

    if(!journal)
        return;

    CFilter &filter = journal->filter();
    CFilterDialog* filterDlg = new CFilterDialog(filter, this);

    if(filterDlg->exec() == QDialog::Accepted)
    {
        filter = filterDlg->filter();

        if(filter.type() == CFilter::FilterDateType)
            filterJournal(journal);
    }

    delete filterDlg;
}
//----------------------------------------------
void ConfiguratorWindow::deviceDefaultSettings()
{
    int answer = showMessageBox(tr("Отправка команды"), tr("Вы действительно хотите сбросить настройки устройства по умолчанию?"), QMessageBox::Question);
    if(answer == QMessageBox::Yes)
    {
        sendDeviceCommand(0x0001); // отправка команды на сброс настроек по умолчанию

        QTimer::singleShot(1000, this, readSetCurrent); // чтение настроек после сброса после паузы 1сек
    }
}
//-----------------------------------------
void ConfiguratorWindow::noConnectMessage()
{
    showMessageBox(tr("Отправка данных"), tr("Невозможно отправить запрос. Нет соединения с устройством"), QMessageBox::Warning);
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
    resizeColumns();
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
        if(button == ui->dockWidgetMenuDevice->control())
        {
            if(ui->dockWidgetMenuDevice->width() > ui->dockWidgetMenuDevice->control()->minimumWidth())
            {
                int w = sizes[0];

                ui->dockWidgetMenuDevice->setProperty("WIDTH", w);

                sizes[0] = ui->dockWidgetMenuDevice->control()->minimumWidth();
                sizes[1] = sizes[1] + (w - sizes[0]);

                ui->splitterCentralWidget->setSizes(sizes);
                ui->dockWidgetMenuDevice->showContent();
                emit ui->splitterCentralWidget->splitterMoved(sizes[1], 1);
            }
            else if(ui->dockWidgetMenuDevice->width() == ui->dockWidgetMenuDevice->control()->minimumWidth())
            {
                int w        = sizes[0];
                int newWidth = ui->dockWidgetMenuDevice->property("WIDTH").toInt();

                if(newWidth <= 0)
                    newWidth = ui->dockWidgetMenuDevice->sizeHint().width();

                sizes[0] = newWidth;
                sizes[1] = sizes[1] - (newWidth - w);

                ui->splitterCentralWidget->setSizes(sizes);
                ui->dockWidgetMenuDevice->hideContent();
                emit ui->splitterCentralWidget->splitterMoved(sizes[1], 1);
            }
        }
        else if(button == ui->dockWidgetVariable->control())
        {
            if(ui->dockWidgetVariable->width() > ui->dockWidgetVariable->control()->minimumWidth())
            {
                int w = sizes[2];

                ui->dockWidgetVariable->setProperty("WIDTH", w);

                sizes[2] = ui->dockWidgetVariable->control()->minimumWidth();
                sizes[1] = sizes[1] + (w - sizes[2]);

                ui->splitterCentralWidget->setSizes(sizes);
                ui->dockWidgetVariable->showContent();
                emit ui->splitterCentralWidget->splitterMoved(sizes[2], 2);
            }
            else if(ui->dockWidgetVariable->width() == ui->dockWidgetVariable->control()->minimumWidth())
            {
                int w        = sizes[2];
                int newWidth = ui->dockWidgetVariable->property("WIDTH").toInt();

                if(newWidth <= 0)
                    newWidth = ui->dockWidgetVariable->sizeHint().width();

                sizes[2] = newWidth;
                sizes[1] = sizes[1] - (newWidth - w);

                ui->splitterCentralWidget->setSizes(sizes);
                ui->dockWidgetVariable->hideContent();
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

                emit ui->splitterPanelMessage->splitterMoved(sizes[1], 0);
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
void ConfiguratorWindow::filterJournal(JournalPtr journal)
{
    if(!journal)
        return;

    CJournalTable* table = journal->widget()->table();

    if(!table || table->rowCount() == 0 || table->columnCount() < 3)
        return;

    int count = 0;
    int beg_num = -1, end_num = -1;

    CFilter filter = journal->filter();

    QDate filter_date_from = filter.dateFrom();
    QDate filter_date_to = filter.dateTo();
    QTime filter_time = filter.time();

    for(int i = 0; i < table->rowCount(); i++)
    {
        QTableWidgetItem* itemDate = table->item(i, 1);
        QTableWidgetItem* itemTime = table->item(i, 2);

        if(!itemDate || !itemTime)
            continue;

        QDate row_date = QDate::fromString(itemDate->text(), "dd.MM.yyyy");
        QTime row_time = QTime::fromString(itemTime->text(), "hh:mm:ss.zzz");

        if(!row_date.isValid() || !row_time.isValid())
            continue;

        if(row_date < filter_date_from || row_date > filter_date_to)
        {
            table->setRowHidden(i, true);
        }
        else if((row_date >= filter_date_from && row_date <= filter_date_to) && row_time < filter_time)
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

    journal->widget()->header()->setTextTableCountMessages(text);
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

        showMessageBox(tr("Чтение журнала"), tr("Ошибка чтения журнала %1.").arg(nameJournal), QMessageBox::Warning);
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
    sendRequestRead(538, 20, READ_STATUS_MODULE_INFO, CModBusDataUnit::ReadInputRegisters);
}
/*!
 * \brief ConfiguratorWindow::updateSerialPortSettings
 *
 * Обнолвение параметров настроек последовательного порта и протокола Modbus при их изменении в окне настроек
 */
void ConfiguratorWindow::updateSerialPortSettings()
{
    int index = ui->comboBoxCommunicationBaudrate->findText(QString("%1").arg(m_serialPortSettings_window->baudrate()));

    if(index != -1)
        ui->comboBoxCommunicationBaudrate->setCurrentIndex(index);
}
//---------------------------------------
void ConfiguratorWindow::expandedWindow()
{
    Qt::WindowStates states = windowState();

    if(!(states & Qt::WindowMaximized))
    {
        showMaximized();
    }
    else
    {
        QRect r = normalGeometry();
        SetCursorPos(r.left(), r.top());
        showNormal();
    }
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
    // Создание файла БД нового проекта
    QDir dir;

    if(!dir.exists("outputs/projects"))
        dir.mkdir("outputs/projects");

    QString selectedFilter  = tr("Файлы проектов (*.project)");
    QString projectPathName = QFileDialog::getSaveFileName(this, tr("Создание файла проекта"), QString(dir.absolutePath() + "/%1/%2").
                                                           arg("outputs/projects").arg("newProject.project"),
                                                           tr("Файлы проектов (*.project);;Все файлы (*.*)"), &selectedFilter,
                                                           QFileDialog::DontConfirmOverwrite);

    if(projectPathName.isEmpty())
    {
        outApplicationEvent(tr("Отказ пользователя от создания файла проекта"));
        return;
    }

    QFileInfo fi;
    QString   baseNameFile = QFileInfo(projectPathName).baseName();

    if(fi.exists(projectPathName)) // Файл уже существует
    {
        outApplicationEvent(tr("Перезапись существующего файла проекта: %1").arg(baseNameFile));
        int reply = showMessageBox(tr("Экспорт журнала"), tr("Файл проекта с таким именем уже существует.\nПерезаписать его?"), QMessageBox::Question);

        if(reply == QMessageBox::Yes) // удаляем старый файл базы данных
        {
            QFile file(projectPathName);

            m_project_cur_path = "";

            if(!file.remove())
            {
                showMessageBox(tr("Удаление файла проекта"), tr("Невозможно удалить файл проекта %1! Возможно уже используется или у Вас нет прав").
                               arg(baseNameFile), QMessageBox::Warning);
                return;
            }
        }
        else
        {
            return;
        }
    }

    m_progressbar->setProgressTitle(tr("Создание нового проекта"));
    m_progressbar->progressStart();
    m_progressbar->setSettings(0, 100, "%");

    QSqlDatabase *project_db;

    if(!connectDb(project_db, projectPathName))
    {
        QMessageBox::warning(this, tr("Создание нового проекта"), tr("Не удалось создать новый проект: %1.\n Попробуйте еще раз.").
                             arg(projectPathName));
        disconnectDb(project_db);
        m_progressbar->progressStop();
        return;
    }

    // Создание таблиц для хранения настроек пользователя и хранения данных
    // Создание таблиц журналов
    if(!createJournalTable(project_db, "EVENT", false)) // журнал событий
    {
        disconnectDb(project_db);
        m_progressbar->progressStop();
        return;
    }

    m_progressbar->increment(5);

    if(!createJournalTable(project_db, "CRASH", false)) // журнал аварий
    {
        disconnectDb(project_db);
        m_progressbar->progressStop();
        return;
    }

    m_progressbar->increment(5);

    if(!createJournalTable(project_db, "HALFHOUR", false)) // журнал получасовок
    {
        disconnectDb(project_db);
        m_progressbar->progressStop();
        return;
    }

    m_progressbar->increment(5);

    // Создание таблиц для хранения состояний выходов
    if(!createTablePurpose("LED", project_db)) // светодиоды
    {
        showMessageBox(tr("Создание таблицы привязок светодиодов"), tr("Невозможно создать таблицу привязок светодиодов в файле проекта"),
                       QMessageBox::Warning);
        disconnectDb(project_db);
        m_progressbar->progressStop();
        return;
    }

    m_progressbar->increment(5);

    if(!createTablePurpose("RELAY", project_db)) // реле
    {
        showMessageBox(tr("Создание таблицы привязок реле"), tr("Невозможно создать таблицу привязок реле в файле проекта"), QMessageBox::Warning);
        disconnectDb(project_db);
        m_progressbar->progressStop();
        return;
    }

    m_progressbar->increment(5);

    if(!createTablePurpose("INPUT", project_db)) // дискретные входы
    {
        showMessageBox(tr("Создание таблицы привязок дискретных входов"), tr("Невозможно создать таблицу привязок дискретных входов в файле проекта"),
                       QMessageBox::Warning);
        disconnectDb(project_db);
        m_progressbar->progressStop();
        return;
    }

    m_progressbar->increment(5);

    // Создание таблицы привязок блокировок защит
    if(!createProjectTableProtection(loadProtectionList().count(), project_db))
    {
        showMessageBox(tr("Создание таблицы привязок защит"), tr("Невозможно создать таблицу привязок защит в файле проекта"), QMessageBox::Warning);
        disconnectDb(project_db);
        m_progressbar->progressStop();
        return;
    }

    m_progressbar->increment(5);

    // Создание таблицы уставок Аналоговые входы
    if(!createProjectTableSet("ANALOG", project_db))
    {
        showMessageBox(tr("Создание таблицы уставок группы Аналоговые входы"),
                       tr("Невозможно создать таблицу уставок группы Аналоговые входы в файле проекта"), QMessageBox::Warning);
        disconnectDb(project_db);
        m_progressbar->progressStop();
        return;
    }

    m_progressbar->increment(5);

    // Создание таблицы уставок группы "Защиты по току"
    if(!createProjectTableSet("MTZ", project_db))
    {
        showMessageBox(tr("Создание таблицы уставок группы МТЗ"), tr("Невозможно создать таблицу уставок группы МТЗ в файле проекта"),
                       QMessageBox::Warning);
        disconnectDb(project_db);
        m_progressbar->progressStop();
        return;
    }

    m_progressbar->increment(5);

    // Создание таблицы уставок группы "Защиты по напряжению"
    if(!createProjectTableSet("PWR", project_db))
    {
        showMessageBox(tr("Создание таблицы уставок группы Защиты по напряжению"),
                       tr("Невозможно создать таблицу уставок группы Защиты по напряжнию в файле проекта"), QMessageBox::Warning);
        disconnectDb(project_db);
        m_progressbar->progressStop();
        return;
    }

    m_progressbar->increment(5);

    // Создание таблицы уставок группы "Направленные"
    if(!createProjectTableSet("DIR", project_db))
    {
        showMessageBox(tr("Создание таблицы уставок группы Направленные"), tr("Невозможно создать таблицу уставок группы Направленные в файле проекта"),
                       QMessageBox::Warning);
        disconnectDb(project_db);
        m_progressbar->progressStop();
        return;
    }

    m_progressbar->increment(5);

    // Создание таблицы уставок группы "Защиты по частоте"
    if(!createProjectTableSet("FREQ", project_db))
    {
        showMessageBox(tr("Создание таблицы уставок группы Защиты по частоте"),
                       tr("Невозможно создать таблицу уставок группы Защиты по частоте в файле проекта"), QMessageBox::Warning);
        disconnectDb(project_db);
        m_progressbar->progressStop();
        return;
    }

    m_progressbar->increment(5);

    // Создание таблицы уставок группы "Внешние защиты"
    if(!createProjectTableSet("EXT", project_db))
    {
        showMessageBox(tr("Создание таблицы уставок группы Внешние защиты"),
                       tr("Невозможно создать таблицу уставок группы Внешние защиты в файле проекта"), QMessageBox::Warning);
        disconnectDb(project_db);
        m_progressbar->progressStop();
        return;
    }

    m_progressbar->increment(4);

    // Создание таблицы уставок группы "Для двигателя"
    if(!createProjectTableSet("MOTOR", project_db))
    {
        showMessageBox(tr("Создание таблицы уставок группы Защиты для двигателя"),
                       tr("Невозможно создать таблицу уставок группы Защиты для двигателя в файле проекта"), QMessageBox::Warning);
        disconnectDb(project_db);
        m_progressbar->progressStop();
        return;
    }

    m_progressbar->increment(3);

    // Создание таблицы уставок группы "Защиты по температуре"
    if(!createProjectTableSet("TEMP", project_db))
    {
        showMessageBox(tr("Создание таблицы уставок группы Защиты по температуре"),
                       tr("Невозможно создать таблицу уставок группы Защиты по температуре в файле проекта"), QMessageBox::Warning);
        disconnectDb(project_db);
        m_progressbar->progressStop();
        return;
    }

    m_progressbar->increment(3);

    // Создание таблицы уставок группы "Резервные защиты"
    if(!createProjectTableSet("RESERVE", project_db))
    {
        showMessageBox(tr("Создание таблицы уставок группы Резервные защиты"),
                       tr("Невозможно создать таблицу уставок группы Резервные защиты в файле проекта"), QMessageBox::Warning);
        disconnectDb(project_db);
        m_progressbar->progressStop();
        return;
    }

    m_progressbar->increment(3);

    // Создание таблицы уставок группы "Предварительного контроля"
    if(!createProjectTableSet("CTRL", project_db))
    {
        showMessageBox(tr("Создание таблицы уставок группы Предварительного контроля"),
                       tr("Невозможно создать таблицу уставок группы Предварительного контроля в файле проекта"), QMessageBox::Warning);
        disconnectDb(project_db);
        m_progressbar->progressStop();
        return;
    }

    m_progressbar->increment(3);

    // Создание таблицы уставок группы "Автоматика->Выключатель"
    if(!createProjectTableSet("AUTO_SWITCH", project_db))
    {
        showMessageBox(tr("Создание таблицы уставок группы Автоматика"), tr("Невозможно создать таблицу уставок группы Автоматика в файле проекта"),
                       QMessageBox::Warning);
        disconnectDb(project_db);
        m_progressbar->progressStop();
        return;
    }

    // Создание таблицы уставок группы "Автоматика->АПВ"
    if(!createProjectTableSet("AUTO_APV", project_db))
    {
        showMessageBox(tr("Создание таблицы уставок группы Автоматика"), tr("Невозможно создать таблицу уставок группы Автоматика в файле проекта"),
                       QMessageBox::Warning);
        disconnectDb(project_db);
        m_progressbar->progressStop();
        return;
    }

    // Создание таблицы уставок группы "Автоматика->АВР"
    if(!createProjectTableSet("AUTO_AVR", project_db))
    {
        showMessageBox(tr("Создание таблицы уставок группы Автоматика"), tr("Невозможно создать таблицу уставок группы Автоматика в файле проекта"),
                       QMessageBox::Warning);
        disconnectDb(project_db);
        m_progressbar->progressStop();
        return;
    }

    // Создание таблицы уставок группы "Автоматика->КЦН"
    if(!createProjectTableSet("AUTO_KCN", project_db))
    {
        showMessageBox(tr("Создание таблицы уставок группы Автоматика"), tr("Невозможно создать таблицу уставок группы Автоматика в файле проекта"),
                       QMessageBox::Warning);
        disconnectDb(project_db);
        m_progressbar->progressStop();
        return;
    }

    m_progressbar->increment(5);

    // Создание таблицы настроек связи
    if(!createProjectTableCommunication(project_db))
    {
        showMessageBox(tr("Создание таблицы настроек связи"), tr("Невозможно создать таблицу настроек связи в файле проекта"), QMessageBox::Warning);
        disconnectDb(project_db);
        m_progressbar->progressStop();
        return;
    }

    m_progressbar->increment(5);

    // Создание таблицы калибровок по току
    if(!createProjectTableCalibrationCurrent(project_db))
    {
        showMessageBox(tr("Создание таблицы калибровок по току"), tr("Невозможно создать таблицу калибровок по току в файле проекта"),
                       QMessageBox::Warning);
        disconnectDb(project_db);
        m_progressbar->progressStop();
        return;
    }

    m_progressbar->increment(3);

    // Создание таблицы калибровок по напряжению AC
    if(!createProjectTableCalibrationPowerAC(project_db))
    {
        showMessageBox(tr("Создание таблицы калибровок по напряжению AC"), tr("Невозможно создать таблицу калибровок по напряжению AC в файле проекта"),
                       QMessageBox::Warning);
        disconnectDb(project_db);
        m_progressbar->progressStop();
        return;
    }

    m_progressbar->increment(3);

    // Создание таблицы калибровок БРУ по сопротивлению
    if(!createProjectTableCalibrationBruResistance(project_db))
    {
        showMessageBox(tr("Создание таблицы калибровок БРУ по сопротивлению"), tr("Невозможно создать таблицу калибровок БРУ по сопротивлению в файле проекта"),
                       QMessageBox::Warning);
        disconnectDb(project_db);
        m_progressbar->progressStop();
        return;
    }

    m_progressbar->increment(3);

    // Создание таблицы калибровок БРУ по постоянному напряжению
    if(!createProjectTableCalibrationBruPowerDC(project_db))
    {
        showMessageBox(tr("Создание таблицы калибровок БРУ по напряжению"), tr("Невозможно создать таблицу калибровок БРУ по постоянному напряжению в файле проекта"),
                       QMessageBox::Warning);
        disconnectDb(project_db);
        m_progressbar->progressStop();
        return;
    }

    m_progressbar->increment(3);

    // Создание таблицы настроек контейнеров
    if(!createProjectTableContainer(project_db))
    {
        showMessageBox(tr("Создание таблицы настройки положения контейнеров"), tr("Невозможно создать таблицу настроек положения контейнеров в файле проекта"),
                       QMessageBox::Warning);
        disconnectDb(project_db);
        m_progressbar->progressStop();
        return;
    }

    disconnectDb(project_db);

    m_progressbar->increment(2);

    unblockInterface();
    outApplicationEvent(tr("Создание нового файла проекта: %1").arg(projectPathName));
    m_progressbar->progressStop();

    m_project_cur_path = projectPathName;
    m_is_set_change = true; // создан новый проект - есть возможность сохранения его в файл
    emit ui->widgetMenuBar->widgetMenu()->addOpenDocument(m_project_cur_path);
}
//----------------------------------------
void ConfiguratorWindow::openFileProject()
{
    if(!m_project_cur_path.isEmpty())
    {
        int answer = showMessageBox(tr("Открыть проект"), tr("Вы хотите сохранить изменения в текущем проекте?"),
                                    QMessageBox::Question);

        if(answer == QMessageBox::Yes)
        {
            saveProject();
            m_project_cur_path = "";
        }
    }

    QDir dir;
    QString projectPathName = QFileDialog::getOpenFileName(this, tr("Открытие файла проекта"),
                                                           QString(dir.absolutePath() + "/%1/%2").arg("outputs/projects").
                                                           arg("newProject.project"), tr("Проекты (*.project)"));
    openProject(projectPathName);
}
//--------------------------------------------------------------------
void ConfiguratorWindow::openExistsProject(const QString &projectPath)
{
    if(projectPath.isEmpty() || (!m_project_cur_path.isEmpty() && projectPath == m_project_cur_path))
        return;

    if(!m_project_cur_path.isEmpty())
    {
        int answer = showMessageBox(tr("Открыть проект"), tr("Вы хотите сохранить изменения в текущем проекте?"),
                                    QMessageBox::Question);

        if(answer == QMessageBox::Yes)
        {
            saveProject();
            m_project_cur_path = "";
        }
    }

    openProject(projectPath);
}
//------------------------------------
bool ConfiguratorWindow::saveProject()
{
    if(m_project_cur_path.isEmpty())
        return false;

    int answer = QMessageBox::question(this, tr("Сохранение проекта"), tr("Вы хотите сохранить текущие настройки в проект?"));

    if(answer == QMessageBox::No)
        return false;

    QSqlDatabase *db = nullptr;

    if(!connectDb(db, m_project_cur_path))
    {
        QMessageBox::warning(this, tr("Сохранение проекта"), tr("Не удалось сохранить проект: %1.\n Попробуйте еще раз.").
                             arg(m_project_cur_path));
        disconnectDb(db);
        m_progressbar->progressStop();

        return false;
    }

    m_progressbar->setProgressTitle(tr("Сохранение проекта"));
    m_progressbar->setSettings(0, 100, "%");
    m_progressbar->progressStart();

    saveJournalToProject(ui->widgetJournalEvent, db);
    saveJournalToProject(ui->widgetJournalCrash, db);
    saveJournalToProject(ui->widgetJournalHalfHour, db);
    savePurposeToProject(ui->tablewgtLedPurpose, "LED", db);
    savePurposeToProject(ui->tablewgtRelayPurpose, "RELAY", db);
    savePurposeToProject(ui->tablewgtDiscreteInputPurpose, "INPUT", db);
    savePurposeToProject(ui->tablewgtProtectionCtrl, "PROTECTION", db);

    saveDeviceSetToProject(DEVICE_MENU_ITEM_SETTINGS_ITEM_IN_ANALOG, "ANALOG", db);
    saveDeviceSetToProject(DEVICE_MENU_PROTECT_ITEM_CURRENT, "MTZ", db);
    saveDeviceSetToProject(DEVICE_MENU_PROTECT_ITEM_POWER, "PWR", db);
    saveDeviceSetToProject(DEVICE_MENU_PROTECT_ITEM_LEAK, "DIR", db);
    saveDeviceSetToProject(DEVICE_MENU_PROTECT_ITEM_FREQUENCY, "FREQ", db);
    saveDeviceSetToProject(DEVICE_MENU_PROTECT_ITEM_EXTERNAL, "EXT", db);
    saveDeviceSetToProject(DEVICE_MENU_PROTECT_ITEM_TEMPERATURE, "TEMP", db);
    saveDeviceSetToProject(DEVICE_MENU_PROTECT_ITEM_RESERVE, "RESERVE", db);
    saveDeviceSetToProject(DEVICE_MENU_PROTECT_ITEM_CONTROL, "CTRL", db);
    saveDeviceSetToProject(DEVICE_MENU_ITEM_AUTOMATION_SWITCH, "AUTO_SWITCH", db);
    saveDeviceSetToProject(DEVICE_MENU_ITEM_AUTOMATION_APV, "AUTO_APV", db);
    saveDeviceSetToProject(DEVICE_MENU_ITEM_AUTOMATION_AVR, "AUTO_AVR", db);
    saveDeviceSetToProject(DEVICE_MENU_ITEM_AUTOMATION_KCN, "AUTO_KCN", db);
    saveDeviceCommunication(db);
    saveDeviceCalibrationCurrent(db);
    saveDeviceCalibrationPowerAC(db);
    saveDeviceCalibrationBruResistance(db);
    saveDeviceCalibrationBruPowerDC(db);

    if(!clearTableDB(db, "containerSettings"))
    {
        QString text = tr("Загрузка настроек контейнера: не удалось удалить старые данные из таблицы");
        outLogMessage(text);
        m_popup->setPopupText(tr("Ошибка сохранения проекта!\n%1").arg(text));
        m_popup->show();
        return false;
    }

    saveContainerSettings(m_containerWidgetVariable, db);
    saveContainerSettings(m_containerWidgetDeviceMenu, db);
    saveContainerSettings(m_containerIndicatorState, db);
    saveContainerSettings(m_containerMonitorI11I17, db);
    saveContainerSettings(m_containerOutputAll, db);
    saveContainerSettings(m_containerInputs, db);
    saveContainerSettings(m_containerDebugInfo, db);
    saveContainerSettings(m_containerStatusInfo, db);
    saveContainerSettings(m_containerWidgetMessage, db);
    saveContainerSettings(m_containerTerminalModbus, db);

    outLogMessage(tr("Данные проекта успешно сохранены"));
    m_progressbar->progressStop();
    disconnectDb(db);
    m_is_set_change = false; // очищаем флаг изменения настроек при записи

    m_popup->setPopupText(tr("Проект успешно сохранен"));
    m_popup->show();

    return true;
}
//--------------------------------------
void ConfiguratorWindow::saveAsProject()
{
    //Сохранение файла проекта
    QDir dir;

    if(!dir.exists("outputs/projects"))
        dir.mkdir("outputs/projects");

    QString selectedFilter  = tr("Файлы проектов (*.project)");
    QString projectPathName = QFileDialog::getSaveFileName(this, tr("Сохранить файл проекта как..."), QString(m_project_cur_path),
                                                           tr("Файлы проектов (*.project);;Все файлы (*.*)"), &selectedFilter,
                                                           QFileDialog::DontConfirmOverwrite);

    if(projectPathName.isEmpty())
    {
        outLogMessage(tr("Отказ пользователя от сохранения файла проекта"));
        return;
    }

    if(m_project_cur_path == projectPathName)
        return;

    QFileInfo fi;
    QString   baseNameFile = QFileInfo(projectPathName).baseName();

    if(fi.exists(projectPathName)) // Файл уже существует
    {
        outApplicationEvent(tr("Перезапись существующего файла проекта: %1").arg(baseNameFile));
        int reply = showMessageBox(tr("Сохранить проект"), tr("Файл проекта с таким именем уже существует.\nПерезаписать его?"),
                                   QMessageBox::Question);

        if(reply == QMessageBox::Yes) // удаляем старый файл базы данных
        {
            QFile file(projectPathName);

            if(!file.remove())
            {
                showMessageBox(tr("Удаление файла проекта"), tr("Невозможно удалить файл проекта %1! Возможно уже используется или у Вас нет прав").
                               arg(baseNameFile), QMessageBox::Warning);
                return;
            }
        }
        else
        {
            return;
        }
    }

    if(!QFile::copy(m_project_cur_path, projectPathName))
    {
        showMessageBox(tr("Сохранить проект как"), tr("Ошибка сохранения проекта!\nВозможно этот файл уже используется."),
                       QMessageBox::Warning);
        outLogMessage(tr("Ошибка сохранения проекта!\nВозможно этот файл уже используется."));
        return;
    }

    m_project_cur_path = projectPathName;
    m_is_set_change = true;

    if(saveProject())
    {
        emit ui->widgetMenuBar->widgetMenu()->addOpenDocument(m_project_cur_path);
    }
}
//---------------------------------------------
void ConfiguratorWindow::exportToExcelProject()
{
    outLogMessage(tr("Экспорт уставок в Excel"));
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

    xlsx.currentWorksheet()->setGridLinesVisible(true);
    xlsx.addSheet(tr("Защиты"));

    QXlsx::Format headerFormat;
    headerFormat.setFontBold(true);
    headerFormat.setHorizontalAlignment(QXlsx::Format::AlignHCenter);
    headerFormat.setVerticalAlignment(QXlsx::Format::AlignBottom);
    headerFormat.setPatternBackgroundColor(Qt::yellow);
    headerFormat.setBorderStyle(QXlsx::Format::BorderThick);

    xlsx.setColumnWidth("A1", 50);
    xlsx.setColumnWidth("B1", 20);
    xlsx.setColumnWidth("C1", 70);

    xlsx.write("A1", tr("Параметр"), headerFormat);
    xlsx.write("B1", tr("Значение"), headerFormat);
    xlsx.write("C1", tr("Диапазон"), headerFormat);

    int pos;

    pos = writeDataToExcel(xlsx, ui->tableWidgetProtectionGroupMTZ);
    pos = writeDataToExcel(xlsx, ui->tableWidgetProtectionGroupPower, pos - 1);
    pos = writeDataToExcel(xlsx, ui->tableWidgetProtectionGroupDirect, pos - 1);
    pos = writeDataToExcel(xlsx, ui->tableWidgetProtectionGroupFrequency, pos - 1);
    pos = writeDataToExcel(xlsx, ui->tableWidgetProtectionGroupExternal, pos - 1);
    pos = writeDataToExcel(xlsx, ui->tableWidgetProtectionGroupTemperature, pos - 1);
    pos = writeDataToExcel(xlsx, ui->tableWidgetProtectionGroupReserve, pos - 1);

    xlsx.addSheet(tr("Автоматика"));

    xlsx.setColumnWidth("A1", 50);
    xlsx.setColumnWidth("B1", 20);
    xlsx.setColumnWidth("C1", 70);

    xlsx.write("A1", tr("Параметр"), headerFormat);
    xlsx.write("B1", tr("Значение"), headerFormat);
    xlsx.write("C1", tr("Диапазон"), headerFormat);

    pos = writeDataToExcel(xlsx, ui->tableWidgetAutomationSwitch);
    pos = writeDataToExcel(xlsx, ui->tableWidgetAutomationAPV, pos - 1);
    pos = writeDataToExcel(xlsx, ui->tableWidgetAutomationAVR, pos - 1);
    pos = writeDataToExcel(xlsx, ui->tableWidgetAutomationKCN, pos - 1);

    xlsx.addSheet(tr("Аналоговые входы"));

    xlsx.setColumnWidth("A1", 50);
    xlsx.setColumnWidth("B1", 20);
    xlsx.setColumnWidth("C1", 50);

    xlsx.write("A1", tr("Параметр"), headerFormat);
    xlsx.write("B1", tr("Значение"), headerFormat);
    xlsx.write("C1", tr("Диапазон"), headerFormat);

    writeDataToExcel(xlsx, ui->tableWidgetSettingsAnalogGroupGeneral);

    xlsx.deleteSheet("Sheet1");
    xlsx.selectSheet(tr("Защиты"));
    xlsx.saveAs(fileName);

    m_progressbar->progressStop();
}
//-----------------------------------------------
void ConfiguratorWindow::importFromExcelProject()
{
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
        outLogMessage(tr("Импорт данных из файла Excel: лист <Защиты> не найден"));
        return;
    }

    int offset = 0;

    offset += readDataFromExcel(xlsx, ui->tableWidgetProtectionGroupMTZ, offset);
    offset += readDataFromExcel(xlsx, ui->tableWidgetProtectionGroupPower, offset - 1);
    offset += readDataFromExcel(xlsx, ui->tableWidgetProtectionGroupDirect, offset - 2);
    offset += readDataFromExcel(xlsx, ui->tableWidgetProtectionGroupFrequency, offset - 3);
    offset += readDataFromExcel(xlsx, ui->tableWidgetProtectionGroupExternal, offset - 4);
    offset += readDataFromExcel(xlsx, ui->tableWidgetProtectionGroupTemperature, offset - 5);
    offset += readDataFromExcel(xlsx, ui->tableWidgetProtectionGroupReserve, offset - 6);

    if(!xlsx.selectSheet(tr("Автоматика")))
    {
        outLogMessage(tr("Импорт данных из файла Excel: лист <Автоматика> не найден"));
        return;
    }

    offset = 0;

    offset += readDataFromExcel(xlsx, ui->tableWidgetAutomationSwitch, offset);
    offset += readDataFromExcel(xlsx, ui->tableWidgetAutomationAPV, offset - 1);
    offset += readDataFromExcel(xlsx, ui->tableWidgetAutomationAVR, offset - 2);
    offset += readDataFromExcel(xlsx, ui->tableWidgetAutomationKCN, offset - 3);

    if(!xlsx.selectSheet(tr("Аналоговые входы")))
    {
        outLogMessage(tr("Импорт данных из файла Excel: лист <Аналоговые входы> не найден"));
        return;
    }

    readDataFromExcel(xlsx, ui->tableWidgetSettingsAnalogGroupGeneral, 0);

    m_progressbar->progressStop();
}
//-------------------------------------
void ConfiguratorWindow::closeProject()
{
    if(m_isProject)
        blockInterface();
}
//--------------------------------------------------
void ConfiguratorWindow::minimizeTabMenu(bool state)
{
    Q_UNUSED(state);
}
//--------------------------------------
void ConfiguratorWindow::resizeColumns()
{
    DeviceMenuItemType index = menuIndex();

    if(index == DEVICE_MENU_ITEM_NONE)
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

        int w_text = int(fm_table.width(text)*1.2f);

        header->resizeSection(i, ((w_header < w_text)?w_text:w_header));
    }
}
//---------------------------------------
void ConfiguratorWindow::processKCUUmin()
{
//    int index = groupMenuPosition(tr("Выключатель"), ui->tableWidgetAutomationGroup);

//    if(index != -1)
//    {
//        QTableWidgetItem* item = ui->tableWidgetAutomationGroup->item(index - 1, 0);

//        if(item)
//        {
//            CDeviceMenuTableWidget::RowType    rowType    = static_cast<CDeviceMenuTableWidget::RowType>(item->data(Qt::UserRole + 100).toInt());
//            CDeviceMenuTableWidget::GroupState groupState = static_cast<CDeviceMenuTableWidget::GroupState>(item->data(Qt::UserRole + 101).toInt());

//            if(rowType == CDeviceMenuTableWidget::HEADER || rowType == CDeviceMenuTableWidget::SUBHEADER)
//            {
//                if(groupState == CDeviceMenuTableWidget::CLOSE)
//                    emit ui->tableWidgetAutomationGroup->itemClicked(item);

//                ui->tableWidgetAutomationGroup->scrollToItem(item);
//                ui->stwgtMain->setCurrentIndex(m_menu_items[DEVICE_MENU_ITEM_AUTOMATION_ROOT]);
//            }
//        }
//    }
}
//------------------------------------------------------
void ConfiguratorWindow::keyPressEvent(QKeyEvent* event)
{
    if(event->key() == Qt::Key_F7)
    {
        testStyle(true);
    }
    else if(event->key() == Qt::Key_F3 && (event->modifiers() & Qt::AltModifier))
    {
        importPurposetToTableFromExcelStart();
    }
}
//---------------------------------------------------
void ConfiguratorWindow::showEvent(QShowEvent* event)
{
    QMainWindow::showEvent(event);
    initApplication();
}
//------------------------------------------------------------------
bool ConfiguratorWindow::eventFilter(QObject* object, QEvent* event)
{
    Q_UNUSED(object);
    Q_UNUSED(event);

    return false;
}
//------------------------------------------
bool ConfiguratorWindow::accessCalibration()
{
    QStringList logins = loadLoginList();

    if(logins.isEmpty())
        return false;

    CUserDialog* userDialog = new CUserDialog(logins, this);

    userDialog->setWindowTitle(tr("Доступ к калибровкам"));

    int answer = userDialog->exec();

    if(answer == QDialog::Accepted)
    {
        CUserDialog::user_t usr = userDialog->user();

        if(!usr.password.isEmpty())
        {
            QString pass = loadUserPassword(usr.login);

            if(usr.password.toUpper() == pass.toUpper())
            {
                return true;
            }
            else
            {
                m_popup->setPopupText(tr("Ошибка: пароль неправильный!"));
                outLogMessage(tr("Ошибка: пароль неправильный!"));
                m_popup->show();
            }
        }
        else
        {
            m_popup->setPopupText(tr("Ошибка: пароль не может быть пустым."));
            outLogMessage(tr("Ошибка: пароль не может быть пустым."));
            m_popup->show();
        }
    }

    delete userDialog;
    userDialog = nullptr;

    return false;
}
/*!
 * \brief ConfiguratorWindow::createJournalTable
 * \param isFull Создание полной карты таблиц (служебная информация - необходима только для экспорта/импорта в отдельный файл)
 * \return Возвращает true, если таблица успешно создана
 */
bool ConfiguratorWindow::createJournalTable(QSqlDatabase* db, const QString& journal_type, bool isFull)
{
    QSqlQuery query(*db);

    QString db_str;
    QString tableName = QString("journal%1").arg(journal_type);

    if(isFull)
    {
        // создание cлужебной таблицы для хранения скрытых от пользователя данных
        db_str = "CREATE TABLE table_info ("
                 "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                 "journal_type STRING(255) NOT NULL);";

        if(!query.exec(db_str))
        {
            showMessageBox(tr("Создание таблицы"), tr("Невозможно создать служебную таблицу журналов: ").arg(query.lastError().text()),
                           QMessageBox::Warning);
            return false;
        }

        // создание таблицы для хранения имен списка журналов
        db_str = "CREATE TABLE journal_list ("
                 "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                 "name STRING(255), "
                 "sn_device STRING(255));";

        if(!query.exec(db_str))
        {
            showMessageBox(tr("Создание таблицы"), tr("Невозможно создать таблицу списка журналов: ").arg(query.lastError().text()),
                           QMessageBox::Warning);
            return false;
        }

        tableName = "journals";
    }

    if(journal_type == "EVENT")
    {
        // создание таблицы для хранения журналов событий
        db_str = QString("CREATE TABLE %1 ("
                 "id_msg INTEGER NOT NULL, "
                 "date STRING(25) NOT NULL, "
                 "time STRING(25), "
                 "type STRING(255), "
                 "category STRING(255), "
                 "parameter STRING(255), "
                 "sn_device INTEGER NOT NULL, "
                 "CONSTRAINT new_pk PRIMARY KEY (id_msg, date, time, sn_device));").arg(tableName);

        if(!query.exec(db_str))
        {
            showMessageBox(tr("Создание таблицы"), tr("Невозможно создать таблицу журналов событий: ").arg(query.lastError().text()), QMessageBox::Warning);
            return false;
        }
    }
    else if(journal_type == "CRASH")
    {
        // создание таблицы для хранения журналов аварий
        db_str = QString("CREATE TABLE %1 ("
                 "id_msg INTEGER NOT NULL, "
                 "date STRING(25) NOT NULL, "
                 "time STRING(25), "
                 "protection STRING(255), "
                 "id_journal INTEGER NOT NULL, " // идентификатор для привязки записей из таблицы свойств
                 "sn_device INTEGER NOT NULL);"
                 /*"CONSTRAINT new_pk PRIMARY KEY (id_msg, date, time, sn_device));"*/).arg(tableName);

        if(!query.exec(db_str))
        {
            showMessageBox(tr("Создание таблицы"), tr("Невозможно создать таблицу журналов аварий: ").arg(query.lastError().text()), QMessageBox::Warning);
            return false;
        }

        // создание таблицы для хранения свойств журналов аварий
        db_str = QString("CREATE TABLE propertyJournal%1 ("
                 "name STRING(255) NOT NULL, "
                 "value STRING(255) NOT NULL, "
                 "id_journal INTEGER NOT NULL);"
                 /*"CONSTRAINT new_pk PRIMARY KEY (name, value, id_journal));"*/).arg(journal_type);

        if(!query.exec(db_str))
        {
            showMessageBox(tr("Создание таблицы"), tr("Невозможно создать таблицу свойств журналов аварий: ").arg(query.lastError().text()),
                           QMessageBox::Warning);
            return false;
        }
    }
    else if(journal_type == "HALFHOUR")
    {
        // создание таблицы для хранения журналов получасовок
        db_str = QString("CREATE TABLE %1 ("
                 "id_msg INTEGER NOT NULL, "
                 "date STRING NOT NULL, "
                 "time STRING, "
                 "type STRING, "
                 "time_reset STRING, "
                 "id_journal INTEGER NOT NULL, " // идентификатор для привязки записей из таблицы свойств
                 "sn_device INTEGER NOT NULL, "
                 "CONSTRAINT new_pk PRIMARY KEY (id_msg, date, time, sn_device));").arg(tableName);

        if(!query.exec(db_str))
        {
            showMessageBox(tr("Создание таблицы"), tr("Невозможно создать таблицу журналов получасовок: ").arg(query.lastError().text()),
                           QMessageBox::Warning);
            return false;
        }

        // создание таблицы для хранения свойств журналов получасовок
        db_str = QString("CREATE TABLE propertyJournal%1 ("
                 "value DOUBLE, "
                 "id_journal INTEGER);"
                 /*"CONSTRAINT new_pk PRIMARY KEY (time, value, id_journal));"*/).arg(journal_type);

        if(!query.exec(db_str))
        {
            showMessageBox(tr("Создание таблицы"), tr("Невозможно создать таблицу свойств журналов получасовок: ").arg(query.lastError().text()),
                           QMessageBox::Warning);
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
    outLogMessage(tr("Загрузка настроек программы..."));

    int baudrate = -1;

    if(m_settings)
    {
        m_settings->beginGroup("serial-port");
            m_serial_port_name = m_settings->value("port_name", "").toString();
            baudrate = m_settings->value("baudrate", QSerialPort::Baud115200).toInt();
            m_serialPortSettings_window->setBaudrate(QSerialPort::BaudRate(baudrate));
            m_serialPortSettings_window->setParity(QSerialPort::Parity(m_settings->value("parity", QSerialPort::EvenParity).toInt()));
        m_settings->endGroup();

        m_settings->beginGroup("device");
            ui->sboxTimeoutCalc->setValue(m_settings->value("timeoutcalculate", 1000).toInt());
            ui->checkboxCalibTimeout->setChecked(m_settings->value("timeoutcalculateenable", true).toBool());
            m_serialPortSettings_window->setAutospeed(m_settings->value("autospeed", false).toBool());
        m_settings->endGroup();

        m_settings->beginGroup("mainwindow");
            restoreGeometry(m_settings->value("geometry").toByteArray());
            ui->splitterCentralWidget->restoreState(m_settings->value("panel").toByteArray());
            ui->splitterPanelMessage->restoreState(m_settings->value("downpanel").toByteArray());
        m_settings->endGroup();

        m_settings->beginGroup("settings");
            m_settings->beginGroup("menu_device_widget");
                ui->dockWidgetMenuDevice->setProperty("WIDTH", m_settings->value("width", 100).toInt());
                ui->dockWidgetMenuDevice->setVisibleContent(m_settings->value("visible").toBool());
            m_settings->endGroup();
            m_settings->beginGroup("variable_widget");
                ui->dockWidgetVariable->setProperty("WIDTH", m_settings->value("width", 100).toInt());
                ui->dockWidgetVariable->setVisibleContent(m_settings->value("visible").toBool());
            m_settings->endGroup();
            ui->framePanelMessage->setProperty("HEIGHT", m_settings->value("panel_message_height", 100).toInt());
            m_settings->beginGroup("central_widget");
//                calibrationRoll(m_settings->value("calibration_roll", false).toBool());
                ui->checkboxCalibTimeout->setChecked(m_settings->value("calculate_check", false).toBool());
                ui->tabWidgetCalibration->setCurrentIndex(m_settings->value("calibration_actual_index", 0).toInt());
            m_settings->endGroup();
        m_settings->endGroup();
    }

    ui->dockWidgetVariable->control()->setState((ui->dockWidgetVariable->isContentHidden()?CDockPanelItemCtrl::Close:
                                                                                           CDockPanelItemCtrl::Open));
    ui->dockWidgetMenuDevice->control()->setState((ui->dockWidgetMenuDevice->isContentHidden()?CDockPanelItemCtrl::Close:
                                                                                               CDockPanelItemCtrl::Open));

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
    outLogMessage(tr("Сохранение настроек программы..."));

    if(m_settings)
    {
        m_settings->beginGroup("serial-port");
            m_settings->setValue("port_name", m_serialPortSettings_window->serialPortName());
            m_settings->setValue("baudrate", m_serialPortSettings_window->baudrate());
            m_settings->setValue("parity", m_serialPortSettings_window->parity());
        m_settings->endGroup();

        m_settings->beginGroup("device");
            m_settings->setValue("timeoutcalculate", ui->sboxTimeoutCalc->value());
            m_settings->setValue("timeoutcalculateenable", ui->checkboxCalibTimeout->isChecked());
            m_settings->setValue("autospeed", m_serialPortSettings_window->autospeedState());
        m_settings->endGroup();

        m_settings->beginGroup("mainwindow");
            m_settings->setValue("geometry", saveGeometry());
            m_settings->setValue("panel", ui->splitterCentralWidget->saveState());
            m_settings->setValue("downpanel", ui->splitterPanelMessage->saveState());
        m_settings->endGroup();
        m_settings->beginGroup("settings");
            m_settings->beginGroup("menu_device_widget");
                m_settings->setValue("width", ui->dockWidgetMenuDevice->property("WIDTH").toInt());
                m_settings->setValue("visible", !ui->dockWidgetMenuDevice->isContentHidden());
            m_settings->endGroup();
            m_settings->beginGroup("variable_widget");
                m_settings->setValue("width", ui->dockWidgetVariable->property("WIDTH").toInt());
                m_settings->setValue("visible", !ui->dockWidgetVariable->isContentHidden());
            m_settings->endGroup();
            m_settings->setValue("panel_message_height", ui->framePanelMessage->property("HEIGHT").toInt());
            m_settings->beginGroup("central_widget");
//                m_settings->setValue("calibration_roll", ui->pushButtonCalibrationRoll->isChecked());
                m_settings->setValue("calculate_check", ui->checkboxCalibTimeout->isChecked());
                m_settings->setValue("calibration_actual_index", ui->tabWidgetCalibration->currentIndex());
            m_settings->endGroup();
        m_settings->endGroup();
    }
}
/*!
 * \brief ConfiguratorWindow::initApplication
 *
 * Инициализация интерфейса программы
 */
void ConfiguratorWindow::initApplication()
{
    if(!m_isInitWindow) // если окно выводится впервые - производим инициализацию
    {
        setWindowState(Qt::WindowMaximized);

        m_modbus                    = new CModBus(this);
        m_serialPortSettings_window = new CSerialPortSetting;
        m_status_bar                = new CStatusBar(statusBar());
        m_popup                     = new PopUp(this);
        m_watcher                   = new QFutureWatcher<void>(this);
        m_progressbar               = new CProgressBarWidget(m_status_bar);
        m_settings                  = new QSettings(QSettings::IniFormat, QSettings::UserScope, ORGANIZATION_NAME, "configurator", this);
        m_tim_calculate             = new QTimer(this);
        m_timer_new_address_set     = new QTimer(this);
        m_tim_debug_info            = new QTimer(this);
        m_timer_synchronization     = new QTimer(this);
        m_journal_timer             = new QTimer(this);
        m_calibration_controller    = new CCalibrationController(ui->widgetCalibrationOfCurrent, ui->widgetCalibrationPower, ui->widgetCalibrationBRUPowerDC,
                                                                 ui->widgetCalibrationBRUResistance);

        statusBar()->addPermanentWidget(m_status_bar, 100);

        connectSystemDb();
        initWindows(); // инициализация окон
        initJournals();
        initMenuPanel();
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

        ui->tabWidgetMessage->setCurrentIndex(0);

        m_version_window = new CVersionSoftware(this);
        versionParser();

        ui->tabwgtMenu->setCurrentIndex(TAB_SET_INDEX);
        m_status_bar->connectStateChanged(false);

        ui->tabwgtMenu->setTabEnabled(TAB_READ_WRITE_INDEX, false);
        ui->tabwgtMenu->setTabEnabled(TAB_FILTER_INDEX, false);

        ui->pushButtonJournalRead->setVisible(false);  // скрытие кнопки чтения журналов
        ui->pushButtonJournalClear->setVisible(false); // скрытие кнопки очистки журналов

        QDateTime dt(QDateTime::currentDateTime());

        ui->dateEdit->setDate(dt.date());
        ui->timeEdit->setTime(dt.time());
        ui->lineEditWeekDay->setText(dt.date().toString("dddd"));

        ui->splitterCentralWidget->setCollapsible(ui->splitterCentralWidget->indexOf(ui->dockWidgetMenuDevice), false);
        ui->splitterCentralWidget->setCollapsible(ui->splitterCentralWidget->indexOf(ui->dockWidgetVariable), false);
        ui->splitterPanelMessage->setCollapsible(ui->splitterPanelMessage->indexOf(ui->framePanelMessage), false);

        ui->pushButtonPanelMessage->setText(tr("Панель сообщений"));
        ui->pushButtonPanelMessage->setDir(CDockPanelItemCtrl::DirBottom);
        ui->dockWidgetMenuDevice->setControlItemDir(CDockPanelItemCtrl::DirLeft);
        ui->dockWidgetVariable->setControlItemDir(CDockPanelItemCtrl::DirRight);

        if(ui->dockWidgetVariable->control())
            ui->dockWidgetVariable->setMinimumWidth(ui->dockWidgetVariable->control()->minimumWidth());
        if(ui->dockWidgetMenuDevice->control())
            ui->dockWidgetMenuDevice->setMinimumWidth(ui->dockWidgetMenuDevice->control()->minimumWidth());
        ui->framePanelMessage->setMinimumHeight(ui->pushButtonPanelMessage->minimumHeight());

        // шрифт для панели меню
        QFont treeFont = m_treeWidgetDeviceMenu->font();
        treeFont.setPointSize(12);
        m_treeWidgetDeviceMenu->setFont(treeFont);

        if(ui->dockWidgetMenuDevice->control() &&
           ui->dockWidgetMenuDevice->width() == ui->dockWidgetMenuDevice->control()->minimumWidth())
        {
            ui->dockWidgetMenuDevice->control()->setState(CDockPanelItemCtrl::Close);
        }
        else
        {
            ui->dockWidgetMenuDevice->control()->setState(CDockPanelItemCtrl::Open);
        }

        if(ui->dockWidgetVariable->control() &&
           ui->dockWidgetVariable->width() == ui->dockWidgetVariable->control()->minimumWidth())
        {
            ui->dockWidgetVariable->control()->setState(CDockPanelItemCtrl::Close);
        }
        else
        {
            ui->dockWidgetVariable->control()->setState(CDockPanelItemCtrl::Open);
        }

        if(ui->framePanelMessage->height() == ui->pushButtonPanelMessage->minimumHeight())
        {
            ui->pushButtonPanelMessage->setState(CDockPanelItemCtrl::Close);
        }
        else
        {
            ui->pushButtonPanelMessage->setState(CDockPanelItemCtrl::Open);
        }

        ui->pbtnMenuNewProject->setShortcut(QKeySequence("CTRL+N"));
        ui->pbtnMenuOpenProject->setShortcut(QKeySequence("CTRL+O"));
        ui->pbtnMenuSaveProject->setShortcut(QKeySequence("CTRL+S"));

        // Расположение контейнеров по умолчанию
        ui->dockWidgetVariable->addContainer(m_containerWidgetVariable);
        ui->dockWidgetMenuDevice->addContainer(m_containerWidgetDeviceMenu);
        ui->tabWidgetMessage->addContainer(m_containerWidgetMessage);
        ui->tabWidgetMessage->addContainer(m_containerTerminalModbus);

        bool is_remove = deleteLogFile();

        outLogMessage(tr("Запуск программы..."));

        if(is_remove)
            outLogMessage(tr("Файл логирования <log.txt> удален."));

        loadSettings();
        initConnect();

        // управление отображением панелей
        panelVisibleCtrl(ui->centralWgt);
        panelVisibleCtrl(ui->dockWidgetVariable);
        panelVisibleCtrl(ui->framePanelMessage);

        ui->tabWidgetMessage->setSuperParent(this);

        // сворачивание панели калибровок
        calibrationRoll(false);

        int index_screen = m_menu_items[DEVICE_MENU_ITEM_SETTINGS_ITEM_WELLCOME_SCREEN];
        ui->stwgtMain->setCurrentIndex(index_screen); // экран приветствия

        refreshSerialPort();

        m_isInitWindow = true;
    }

    if(!m_isProject)
    {
        blockInterface();
    }
}
//------------------------------------------------------------------------------------------------------------------------------------------
void ConfiguratorWindow::exportToPDF(const JournalPtr journal, const QString& reportName, const QString& sn_device, const QString& filename)
{
    QPrinter* printer = new QPrinter(QPrinter::ScreenResolution);

    printer->setColorMode(QPrinter::GrayScale);
    printer->setOutputFormat(QPrinter::PdfFormat);
    printer->setPaperSize(QPrinter::A4);
    printer->setPageMargins(15, 10, 10, 10, QPrinter::Millimeter);
    printer->setOutputFileName(filename);

    QTextDocument* reportPDF = new QTextDocument;
    reportPDF->setPageSize(printer->pageRect().size());

    QString key = journal->widget()->property("TYPE").toString();

    QTextTableFormat tableFormat;
    QVector<QTextLength> columnLength;

    columnLength << QTextLength(QTextLength::PercentageLength, 1);
    columnLength << QTextLength(QTextLength::PercentageLength, 1);
    columnLength << QTextLength(QTextLength::PercentageLength, 1);
    columnLength << QTextLength(QTextLength::PercentageLength, 1);
    columnLength << QTextLength(QTextLength::PercentageLength, 90);
    columnLength << QTextLength(QTextLength::PercentageLength, 1);

    if(key == "EVENT")
        columnLength << QTextLength(QTextLength::PercentageLength, 1);

    tableFormat.setColumnWidthConstraints(columnLength);
    tableFormat.setCellPadding(5);
    tableFormat.setCellSpacing(0);
    tableFormat.setHeaderRowCount(1);
    tableFormat.setBorderBrush(Qt::SolidPattern);
    tableFormat.setBorderStyle(QTextFrameFormat::BorderStyle_Ridge);
    tableFormat.setBorder(1);
    tableFormat.setAlignment(Qt::AlignCenter);

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
    cursor.insertText(tr("Блок БЗУ-2-11.01"));

    cursor.insertBlock();
    cursor.setCharFormat(charHeaderNextFormat);
    cursor.insertText(sn_device);

    cursor.insertBlock(blockFormat);

    int columnCount = journal->widget()->table()->columnCount();

    QPoint pos = QPoint(0, journal->widget()->table()->rowCount() - 1);

    CFilter filter = journal->filter();

    if(filter && filter.type() == CFilter::FilterDateType) // если фильтр активен
    {
        pos = indexDateFilter(journal->widget()->table(), filter.dateFrom(), filter.dateTo());
    }

    int rows = pos.y() - pos.x() + 1; // считаем количество строк для экспорта
    int sub_rows = 0; // количество подстрок (свойства строки журнала)

    row_property_t list;

    if(key == "CRASH")
    {
        for(int i = pos.x(); i <= pos.y(); i++)
        {
            property_list_t property_list = qvariant_cast<property_list_t>(journal->widget()->table()->rowData(i));
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
        halfhour_labels_t column_labels = qvariant_cast<halfhour_labels_t>(journal->widget()->property("COLUMNS"));
        halfhour_labels_t row_labels    = qvariant_cast<halfhour_labels_t>(journal->widget()->property("ROWS"));

        if(column_labels.isEmpty() || row_labels.isEmpty())
            return;

        for(int i = pos.x(); i <= pos.y(); i++)
        {
            halfhour_t halfhour = qvariant_cast<halfhour_t>(journal->widget()->table()->rowData(i));

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
        cellCursor.insertText(journal->widget()->table()->horizontalHeaderItem(i)->text());
    }

    int row_cur = 0;

    for(int i = 0; i < rows; i++)
    {
        for(int j = 0; j < columnCount; j++)
        {
            QTextTableCell cell = textTable->cellAt(i + row_cur + 1, j);
            Q_ASSERT(cell.isValid());
            QTextCursor cellCursor = cell.firstCursorPosition();
            QTableWidgetItem* item = journal->widget()->table()->item(pos.x() + i, j);

            if(item)
            {
                QString text = journal->widget()->table()->item(pos.x() + i, j)->text();

                if(key == "EVENT" && j == columnCount - 1)
                {
                    QStringList l = text.split("(");

                    if(l.count() > 1)
                    {
                        l = l.at(1).split(')');

                        if(l.count() > 0)
                            text = l.at(0);
                    }
                }

                cellCursor.insertText(text);
            }
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
            }
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

            painter.drawText(headerRect, Qt::AlignVCenter|Qt::AlignLeft, journal->widget()->table()->item(pos.x(), 1)->text() +
                                         " - " + journal->widget()->table()->item(pos.y(), 1)->text());

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
                                                    tr("Профили привязок (*.jprf)"));

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
        showMessageBox(tr("Сохранение профиля привязок"), tr("Невозможно создать файл для записи \"%1\"").arg(fileName), QMessageBox::Warning);
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
                                                    tr("Профили привязок (*.jprf)"));

    if(fileName.isEmpty())
        return;

    QFile file(fileName);

    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        showMessageBox(tr("Импорт профиля привязок"), tr("Невозможно открыть файл профиля привязок: %1").arg(file.errorString()), QMessageBox::Warning);
        return;
    }

    QJsonDocument json = QJsonDocument::fromJson(file.readAll());

    file.close();

    if(!json.isObject())
    {
        showMessageBox(tr("Импорт профиля привязок"), tr("Неизвестный формат файла привязок"), QMessageBox::Warning);
        return;
    }

    QString t = json["type"].toString();

    if(t.toUpper() != typeName.toUpper())
    {
        showMessageBox(tr("Импорт профиля привязок"), tr("Ошибка типа файла привязок.\nВыберите другой файл"), QMessageBox::Warning);
        return;
    }

    QJsonArray arrRows = json["rows"].toArray();

    if(matrix.rowCount() != arrRows.count())
    {
        showMessageBox(tr("Импорт профиля привязок"), tr("Количество привязок в файле не соответствует\nколичеству привязок в таблице"),
                       QMessageBox::Warning);
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
//---------------------------------------------------
void ConfiguratorWindow::widgetStackIndexChanged(int)
{
    ui->tabwgtMenu->setTabEnabled(TAB_READ_WRITE_INDEX, false);
    ui->tabwgtMenu->setTabEnabled(TAB_FILTER_INDEX, false);

    ui->pushButtonJournalRead->setVisible(false);
    ui->pushButtonJournalClear->setVisible(false);

    ui->pbtnReadCurrentBlock->setVisible(true);
    ui->pbtnReadAllBlock->setVisible(true);
    ui->pbtnWriteCurrentBlock->setVisible(true);
    ui->pbtnWriteAllBlock->setVisible(true);
    ui->pushButtonWriteEditItem->setVisible(true);

    // перекрываем доступ к калибровкам
//    ui->tableWidgetSettingsAnalogGroupGeneral->setDisabled(true);
//    ui->pushButtonCalibrationRoll->setDisabled(true);
//    ui->tabWidgetCalibration->setDisabled(true);
    //...

    if(ui->tabwgtMenu->currentIndex() == TAB_HELP_INDEX)
    {
        ui->tabwgtMenu->setCurrentIndex(TAB_SET_INDEX);
    }

    DeviceMenuItemType index = menuIndex();

    if(index >= DEVICE_MENU_ITEM_JOURNALS_CRASHES && index <= DEVICE_MENU_ITEM_JOURNALS_ISOLATION)
    {
        switch(index)
        {
            case DEVICE_MENU_ITEM_JOURNALS_CRASHES:
                readJournalCount(m_journal_crash);
            break;

            case DEVICE_MENU_ITEM_JOURNALS_EVENTS:
                readJournalCount(m_journal_event);
            break;

            case DEVICE_MENU_ITEM_JOURNALS_HALF_HOURS:
                readJournalCount(m_journal_halfhour);
            break;

            case DEVICE_MENU_ITEM_JOURNALS_ISOLATION:
                readJournalCount(m_journal_isolation);
            break;

            default: break;
        }

        ui->tabwgtMenu->setTabEnabled(TAB_FILTER_INDEX, true);

        int width = ui->stwgtMain->width() - 760;

        ui->widgetJournalCrash->table()->setFixedWidth(475);
        ui->widgetJournalEvent->setTableColumnWidth(3, width);
        ui->widgetJournalHalfHour->setTableColumnWidth(3, width);
        ui->widgetJournalIsolation->setTableColumnWidth(3, width);

        ui->pushButtonJournalRead->setVisible(true);
        ui->pushButtonJournalClear->setVisible(true);

        ui->pbtnReadCurrentBlock->setVisible(false);
        ui->pbtnReadAllBlock->setVisible(false);
        ui->pbtnWriteCurrentBlock->setVisible(false);
        ui->pbtnWriteAllBlock->setVisible(false);
        ui->pushButtonWriteEditItem->setVisible(false);

        ui->tabwgtMenu->setTabEnabled(TAB_READ_WRITE_INDEX, true);
        ui->tabwgtMenu->setCurrentIndex(TAB_READ_WRITE_INDEX);
    }
    else if(index == DEVICE_MENU_ITEM_SETTINGS_ITEM_DATETIME ||
            index == DEVICE_MENU_ITEM_SETTINGS_ITEM_COMMUNICATIONS ||
            index == DEVICE_MENU_ITEM_SETTINGS_ITEM_IO_PROTECTION)
    {
        ui->tabwgtMenu->setTabEnabled(TAB_READ_WRITE_INDEX, true);
        ui->tabwgtMenu->setCurrentIndex(TAB_READ_WRITE_INDEX);

        ui->pbtnReadAllBlock->hide();
        ui->pbtnWriteAllBlock->hide();
        ui->pushButtonWriteEditItem->hide();
    }
    else if(index == DEVICE_MENU_ITEM_SETTINGS_ITEM_LEDS ||
            index == DEVICE_MENU_ITEM_SETTINGS_ITEM_IO_MDVV01_INPUTS ||
            index == DEVICE_MENU_ITEM_SETTINGS_ITEM_IO_MDVV01_RELAY)
    {
        if(index == DEVICE_MENU_ITEM_SETTINGS_ITEM_IO_MDVV01_INPUTS)
        {
            ui->widgetToolTipState->show();
        }
        else
            ui->widgetToolTipState->hide();

        ui->tabwgtMenu->setTabEnabled(TAB_READ_WRITE_INDEX, true);
        ui->tabwgtMenu->setCurrentIndex(TAB_READ_WRITE_INDEX);

        ui->pbtnReadAllBlock->hide();
        ui->pbtnWriteAllBlock->hide();
        ui->pushButtonWriteEditItem->hide();

        resizeColumns();
    }
    else if((index >= DEVICE_MENU_PROTECT_ITEM_CURRENT && index <= DEVICE_MENU_ITEM_AUTOMATION_APV) && (
                index != DEVICE_MENU_ITEM_PROTECTION_ROOT &&
                index != DEVICE_MENU_ITEM_AUTOMATION_ROOT &&
                index != DEVICE_MENU_ITEM_JOURNALS_ROOT &&
                index != DEVICE_MENU_ITEM_MEASURES_ROOT &&
                index != DEVICE_MENU_ITEM_SETTINGS_ROOT &&
                index != DEVICE_MENU_ITEM_NONE))
    {
        ui->tabwgtMenu->setTabEnabled(TAB_READ_WRITE_INDEX, true);
        ui->tabwgtMenu->setCurrentIndex(TAB_READ_WRITE_INDEX);
    }
    else if(index == DEVICE_MENU_ITEM_SETTINGS_ITEM_IN_ANALOG)
    {
        ui->tabwgtMenu->setTabEnabled(TAB_READ_WRITE_INDEX, true);
        ui->tabwgtMenu->setCurrentIndex(TAB_READ_WRITE_INDEX);
    }
}
//-------------------------------------------------------------
void ConfiguratorWindow::setJournalShiftPtr(JournalPtr journal)
{
    if(!journal)
        return;

    int shift_ptr = journal->shiftPtrCur();

    QVector<quint16> values = QVector<quint16>() << static_cast<quint16>((shift_ptr >> 16)&0xFFFF) << static_cast<quint16>(shift_ptr&0xFFFF);

    CModBusDataUnit unit(static_cast<quint8>(m_serialPortSettings_window->deviceID()), CModBusDataUnit::WriteMultipleRegisters,
                         static_cast<quint16>(journal->addrShiftPtr()), values);
    unit.setProperty(tr("REQUEST"), READ_JOURNAL_SHIFT_PTR);
    QVariant var;
    var.setValue<JournalPtr>(journal);
    unit.setProperty(tr("JOURNAL"), var);
qDebug() << QString("SHIFT_PTR: перемещение указателя на адрес->%1").arg(shift_ptr);
    m_modbus->sendData(unit);
}
//--------------------------------------------------------------------------
void ConfiguratorWindow::setJournalShiftPtr(quint16 addr, quint32 shift_ptr)
{
    QVector<quint16> values = QVector<quint16>() << static_cast<quint16>((shift_ptr >> 16)&0xFFFF) << static_cast<quint16>(shift_ptr&0xFFFF);

    CModBusDataUnit unit(static_cast<quint8>(m_serialPortSettings_window->deviceID()),
                         CModBusDataUnit::WriteMultipleRegisters, addr, values);
qDebug() << QString("SHIFT_PTR: перемещение указателя на адрес->%1").arg(shift_ptr);
    m_modbus->sendData(unit);
}
//------------------------------------------------
void ConfiguratorWindow::timeoutSynchronization()
{
    CModBusDataUnit unit(quint8(m_serialPortSettings_window->deviceID()), CModBusDataUnit::ReadInputRegisters, 0x0001, QVector<quint16>() << 4);
    unit.setProperty("REQUEST", READ_SERIAL_NUMBER);
    m_modbus->sendData(unit);

    m_timer_synchronization->start(5000);
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
//-------------------------------------------------------------------
void ConfiguratorWindow::importJournalToTable(JournalPtr journal_ptr)
{
    JournalPtr journal = currentJournalWidget();

    if(journal_ptr)
        journal = journal_ptr;

    if(!journal) // не выбран текущий журнал
    {
        showMessageBox(tr("Импорт журнала"), tr("Выберите пожалуйста журнал в который необходимо произвести импорт"), QMessageBox::Warning);
        return;
    }

    // Получение таблицы и заголовка текущего журнала
    CJournalTable*  table  = journal->widget()->table();
    CHeaderJournal* header = journal->widget()->header();

    if(!table || !header)
        return;

    if(table->rowCount() > 0) // таблица пуста
    {
        clearJournal();
    }

    QString journal_name = journalName(journal);
    QString journal_type = journal->widget()->property("TYPE").toString();

    if(journal_name.isEmpty() || journal_type.isEmpty())
    {
        showMessageBox(tr("Экспорт журнала"), tr("Не удалось определить имя или тип журнала"), QMessageBox::Warning);
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
        showMessageBox(tr("Импорт журнала"), tr("Невозможно открыть базу данных журналов \"%1\"").arg(journal_name), QMessageBox::Warning);
        disconnectDb(db);
        return;
    }

    // проверяем служебную информацию таблицы
    int count = recordCountDb(db, "table_info", "journal_type", QString("\"%1\"").arg(journal_type));

    if(count == 0) // такой записи в служебной таблице нет - тип журнала не совпадает с текущим
    {
        showMessageBox(tr("Импорт журнала"), tr("Импортируемы журнал не является журналом %1.\n%2.").arg(journal_name).arg("Выберите другой журнал"),
                       QMessageBox::Warning);
    }
    else if(count == -1) // открытая база не является базой журналов
    {
        showMessageBox(tr("Импорт журнала"), tr("Импортируемый журнал не является каким-либо журналом.\nВыберите другой файл"), QMessageBox::Warning);
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
        showMessageBox(tr("Импорт журнала"), tr("Невозможно получить список журналов из базы %1:\n%2").arg(journal_name).arg(query.lastError().text()),
                       QMessageBox::Warning);
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
        showMessageBox(tr("Импорт журнала"), tr("Список журналов %1 пуст.").arg(journal_name), QMessageBox::Warning);
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
            int reply = showMessageBox(tr("Импорт журнала"), tr("Этот журнал не от этого устройства. Все равно загрузить?"), QMessageBox::Question);
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
        showMessageBox(tr("Импорт журнала"), tr("База журнала %1 пуста.").arg(journal_name), QMessageBox::Warning);
        disconnectDb(db);
        return;
    }

    QTime timer;
    timer.start();

    QString str = QString("SELECT * FROM journals WHERE sn_device=%1").arg(id);

    int max_read_msg = rows; // максимальное значение считываемых записей (используется для прогрессбара)

    CFilter &filter = journal->filter();

    if(filter && filter.type() == CFilter::FilterDateType)
    {
        str += QString(" AND date BETWEEN \"%1\" AND \"%2\"").arg(filter.dateFrom().toString(Qt::ISODate)).
                                                              arg(filter.dateTo().toString(Qt::ISODate));

        max_read_msg = recordCountDb(db, "journals", "sn_device", QString::number(id), "date", QStringList() <<
                                     QString("\'%1\'").arg(filter.dateFrom().toString(Qt::ISODate)) <<
                                     QString("\'%1\'").arg(filter.dateTo().toString(Qt::ISODate)));
    }

    str += ";";

    if(!query.exec(str))
    {
        showMessageBox(tr("Импорт журнала"), tr("Не возможно получить записи журнала %1: %2").arg(journal_name).arg(query.lastError().text()),
                       QMessageBox::Warning);
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

        table->setItem(row, 0, new CTableWidgetItem(QString("%1").arg(row + 1)));
        table->setItem(row, 1, new CTableWidgetItem(id_msg));
        table->setItem(row, 2, new CTableWidgetItem(date));
        table->setItem(row, 3, new CTableWidgetItem(time));

        table->item(row, 0)->setTextAlignment(Qt::AlignCenter);
        table->item(row, 1)->setTextAlignment(Qt::AlignCenter);
        table->item(row, 2)->setTextAlignment(Qt::AlignCenter);
        table->item(row, 3)->setTextAlignment(Qt::AlignCenter);

        if(journal_type == "EVENT")
        {
            QString type      = query.value("type").toString();
            QString category  = query.value("category").toString();
            QString parameter = query.value("parameter").toString();

            table->setItem(row, 4, new CTableWidgetItem(type));
            table->setItem(row, 5, new CTableWidgetItem(category));
            table->setItem(row, 6, new CTableWidgetItem(parameter));
        }
        else if(journal_type == "CRASH")
        {
            QString protection = query.value("protection").toString();
            int     id_journal = query.value("id_journal").toInt();

            table->setItem(row, 4, new CTableWidgetItem(protection));

            // формирование запроса для получения свойств записи
            QSqlQuery query_property(*db);

            if(!query_property.exec(QString("SELECT name, value FROM propertyJournalCRASH WHERE id_journal=%1;").arg(id_journal)))
            {
                showMessageBox(tr("Импорт журнала"), tr("Не удалось прочитать свойства журнала аварий: %1").arg(query_property.lastError().text()),
                               QMessageBox::Warning);
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

            table->setItem(row, 4, new CTableWidgetItem(type));
            table->setItem(row, 5, new CTableWidgetItem(time_reset));

            // формирование запроса для получения свойств записи
            QSqlQuery query_property(*db);

            if(!query_property.exec(QString("SELECT value FROM propertyJournalHALFHOUR WHERE id_journal=%1;").arg(id_journal)))
            {
                outLogMessage(tr("Не удалось прочитать свойства журнала получасовок: %1").arg(query_property.lastError().text()));
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

    table->setSortingEnabled(true);

    CFilter::DateType d = { QDate::fromString(table->item(0, 1)->text(), "dd.MM.yyyy"),
                            QDate::fromString(table->item(table->rowCount() - 1, 1)->text(), "dd.MM.yyyy") };
    filter.setDate(d);

    header->setTextTableCountMessages(msg_count);
    header->setTextElapsedTime(timer.elapsed());

    QTableWidgetItem* itemBeg = table->item(0, 1);
    QTableWidgetItem* itemEnd = table->item(table->rowCount() - 1, 1);

    if(itemBeg && itemEnd)
    {
        QString text = QString("%1 - %2/%3").arg(itemBeg->text()).arg(itemEnd->text()).arg(table->rowCount());
        header->setTextTableCountMessages(text);
    }

    m_progressbar->progressStop();

    table->resizeColumnsToContents();
    table->horizontalHeader()->setStretchLastSection(true);

    disconnectDb(db);
}
//----------------------------------------------------------------
void ConfiguratorWindow::exportJournalToDb(JournalPtr journal_ptr)
{
    JournalPtr journal = currentJournalWidget();

    if(journal_ptr)
        journal = journal_ptr;

    if(!journal) // не выбран текущий журнал
    {
        showMessageBox(tr("Экспорт журнала"), tr("Выберите пожалуйста журнал из которого необходимо произвести экспорт"), QMessageBox::Warning);
        return;
    }

    // Получение таблицы и заголовка текущего журнала
    CJournalTable*  table  = journal->widget()->table();
    CHeaderJournal* header = journal->widget()->header();

    if(!table || !header)
        return;

    if(journal->widget()->table()->rowCount() == 0) // таблица пуста
    {
        showMessageBox(tr("Экспорт журнала"), tr("Текущий журнал пуст"), QMessageBox::Warning);
        return;
    }

    QString journal_name = journalName(journal);
    QString journal_type = journal->widget()->property("TYPE").toString();

    if(journal_name.isEmpty() || journal_type.isEmpty())
    {
        showMessageBox(tr("Экспорт журнала"), tr("Не удалось определить имя или тип журнала"), QMessageBox::Warning);
        return;
    }

    // выбираем файл для экспорта
    QDir dir;

    if(!dir.exists("outputs/db"))
        dir.mkdir("outputs/db");

    QString selectedFilter    = tr("Базы данных (*.db)");
    QString journal_full_name = tr("Журнал %1-%2 (%3 - %4)").arg(journal_name).arg(m_status_bar->serialNumberText()).
                                                             arg(QDate::currentDate().toString("dd_MM_yyyy")).
                                                             arg(QTime::currentTime().toString("HH_mm_ss"));
    QString journal_path      = QFileDialog::getSaveFileName(this, tr("Экспорт журнала %1 в базу данных").arg(journal_name),
                                                             dir.absolutePath() + QString("/outputs/db/%1.%2").
                                                             arg(journal_full_name).arg("db"),
                                                             tr("Базы данных (*.db);;Все файлы (*.*)"), &selectedFilter,
                                                             QFileDialog::DontConfirmOverwrite);
    outApplicationEvent(tr("Экспорт журнала в БД: %1").arg(journal_full_name));

    if(journal_path.isEmpty())
        return;

    QSqlDatabase* db           = nullptr;
    QString       baseNameFile = QFileInfo(journal_path).baseName();

    QFileInfo fi;
    bool      isNewBase = true;

    if(fi.exists(journal_path)) // Файл уже существует
    {
        int reply = showMessageBox(tr("Экспорт журнала"), tr("Такая база уже существует. Перезаписать данные?"), QMessageBox::Question);
        if(reply == QMessageBox::Yes) // удаляемы старый файл базы данных
        {
            QFile file(journal_path);

            if(!file.remove())
            {
                showMessageBox(tr("Удаление базы журналов"),
                               tr("Невозможно удалить базу %1!\nВозможно уже используется или у Вас нет прав").arg(baseNameFile), QMessageBox::Warning);
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
        showMessageBox(tr("Экспорт журнала"), tr("Невозможно открыть/создать файл \"%1\"").arg(baseNameFile), QMessageBox::Warning);
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
            showMessageBox(tr("Экспорт журнала"), tr("Невозможно открыть базу \"%1\" для дозаписи новых данных").arg(baseNameFile), QMessageBox::Warning);
            disconnectDb(db);
            return;
        }

        if(query.first())
        {
            QString type = query.value("journal_type").toString();

            if(type.toUpper() != journal_type.toUpper()) // типы журналов не совпадают
            {
                showMessageBox(tr("Экспорт журнала"), tr("Невозможно выполнить запись журнала в эту базу. "
                                                         "Требуется тип журнала \"%1\", а текущий имеет тип \"%2\"").
                                                         arg(type.toLower()).arg(journal_type.toLower()), QMessageBox::Warning);
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
            showMessageBox(tr("Экспорт журнала"), tr("Невозможно вставить служебную информацию в базу данных: %1").arg(query.lastError().text()),
                           QMessageBox::Warning);
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
            showMessageBox(tr("Экспорт журнала"), tr("Невозможно записать новый журнал в базу данных: %1").arg(query.lastError().text()),
                           QMessageBox::Warning);
            disconnectDb(db);
            return;
        }
    }

    // получаем id записи из базы данных с текущим именем
    query_str = QString("SELECT id FROM journal_list WHERE name=\"%1\";").arg(name_db);

    if(!query.exec(query_str))
    {
        showMessageBox(tr("Экспорт журнала"), tr("Невозможно прочитать id записи \"%1\"").arg(query_str), QMessageBox::Warning);
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


    CFilter filter = journal->filter(); // получаем фильтр

    if(filter && filter.type() == CFilter::FilterDateType) // фильтр активен
    {
        pos = indexDateFilter(journal->widget()->table(), filter.dateFrom(), filter.dateTo()); // получаем новые позиции начала/конца записей (фильтруем)
    }

    m_progressbar->setProgressTitle(tr("Экспорт журнала %1").arg(journal_name));
    m_progressbar->progressStart();
    m_progressbar->setSettings(0, (pos.y() - pos.x()) + 1, "");

    db->transaction();

    int i = 0;

    for(i = pos.x(); i <= pos.y(); i++)
    {
        int     id_msg = journal->widget()->table()->item(i, 1)->text().toInt();
        QString date   = QDate::fromString(journal->widget()->table()->item(i, 2)->text(),
                                           "dd.MM.yyyy").toString(Qt::ISODate); // приведение строки к yyyy-MM-dd для sqlite
        QString time   = journal->widget()->table()->item(i, 3)->text();

        if(journal_type == "EVENT")
        {
            QString type      = journal->widget()->table()->item(i, 4)->text();
            QString category  = journal->widget()->table()->item(i, 5)->text();
            QString parameter = journal->widget()->table()->item(i, 6)->text();

            query.prepare(QString("INSERT OR REPLACE INTO journals (id_msg, date, time, type, category, parameter, sn_device)"
                                  "VALUES(:id_msg, :date, :time, :type, :category, :parameter, :sn_device)"));
            query.bindValue(":id_msg", id_msg);
            query.bindValue(":date", date);
            query.bindValue(":time", time);
            query.bindValue(":type", type);
            query.bindValue(":category", category);
            query.bindValue(":parameter", parameter);
            query.bindValue(":sn_device", id);

            if(!query.exec())
            {
                outLogMessage(tr("Ошибка вставки данных журнала событий в БД: %1").arg(query.lastError().text()));
            }
        }
        else if(journal_type == "CRASH")
        {
            QString protect_name = journal->widget()->table()->item(i, 4)->text();

            query.prepare("INSERT OR REPLACE INTO journals (id_msg, date, time, protection, id_journal, sn_device)"
                          "VALUES(:id_msg, :date, :time, :protection, :id_journal, :sn_device)");
            query.bindValue(":id_msg", id_msg);
            query.bindValue(":date", date);
            query.bindValue(":time", time);
            query.bindValue(":protection", protect_name);
            query.bindValue(":id_journal", id_journal);
            query.bindValue(":sn_device", id);

            if(!query.exec())
            {
                outLogMessage(tr("Ошибка вставки данных журнала аварий в БД: %1").arg(query.lastError().text()));
            }

            property_list_t property_list = qvariant_cast<property_list_t>(table->rowData(i));

            if(!property_list.isEmpty())
            {
                for(const property_data_item_t& item: property_list)
                {
                    QSqlQuery query_property(*db);

                    query_property.prepare("INSERT OR REPLACE INTO propertyJournalCRASH (name, value, id_journal) "
                                           "VALUES(:name, :value, :id_journal);");
                    query_property.bindValue(":name", item.name);
                    query_property.bindValue(":value", item.value);
                    query_property.bindValue(":id_journal", id_journal);

                    if(!query_property.exec())
                    {
                        outLogMessage(tr("Ошибка вставки свойств данных журнала аварий в БД: %1").arg(query_property.lastError().text()));
                    }
                }

                id_journal++;
            }
        }
        else if(journal_type == "HALFHOUR")
        {
            QTableWidgetItem* item = journal->widget()->table()->item(i, 4);

            if(!item)
                continue;

            QString type = item->text();

            query.prepare("INSERT OR REPLACE INTO journals (id_msg, date, time, type, time_reset, id_journal, sn_device)"
                          "VALUES(:id_msg, :date, :time, :type, :time_reset, :id_journal, :sn_device)");
            query.bindValue(":id_msg", id_msg);
            query.bindValue(":date", date);
            query.bindValue(":time", time);
            query.bindValue(":type", type);

            QTableWidgetItem* itemTimeReset = journal->widget()->table()->item(i, 5);

            query.bindValue(":time_reset", ((itemTimeReset)?itemTimeReset->text():""));
            query.bindValue(":id_journal", id_journal);
            query.bindValue(":sn_device", id);

            if(!query.exec())
            {
                outLogMessage(tr("Ошибка вставки данных журнала получасовок в БД: %1").arg(query.lastError().text()));
            }

            halfhour_t halfhour = qvariant_cast<halfhour_t>(journal->widget()->table()->rowData(i));

            if(!halfhour.values.isEmpty() && type.toUpper() == tr("ДАННЫЕ"))
            {
                for(const float& value: halfhour.values)
                {
                    QSqlQuery query_property(*db);

                    query_property.prepare("INSERT INTO propertyJournalHALFHOUR (value, id_journal)"
                                           "VALUES(:value, :id_journal)");
                    query_property.bindValue(":value", value);
                    query_property.bindValue(":id_journal", id_journal);

                    if(!query_property.exec())
                    {
                        outLogMessage(tr("Ошибка вставки свойств данных журнала получасовок в БД: %1").arg(query_property.lastError().text()));
                    }
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
//-------------------------------------------------------------
void ConfiguratorWindow::exportPurposeToDb(const QString &type)
{
    CPurposeTableView *table = purposeTableByType(type);

    if(!table || type.isEmpty())
    {
        outApplicationEvent(tr("Невозможно получить доступ к таблице привязок."));
        return;
    }

    // выбираем файл для экспорта
    QDir dir;

    if(!dir.exists("outputs/profiles"))
        dir.mkdir("outputs/profiles");

    QString purpose_name = table->property("NAME").toString();
    QString selectedFilter = tr("Файлы привязок (*.prf)");
    QString purpose_full_name = tr("Профиль привязок %1-%2 (%3 - %4)").arg(purpose_name).arg(m_status_bar->serialNumberText()).
                                                                       arg(QDate::currentDate().toString("dd_MM_yyyy")).
                                                                       arg(QTime::currentTime().toString("HH_mm_ss"));
    QString purpose_path = QFileDialog::getSaveFileName(this, tr("Экспорт привязок %1 в базу данных").arg(purpose_name),
                                                        dir.absolutePath() + QString("/outputs/profiles/%1.%2").
                                                        arg(purpose_full_name).arg("prf"),
                                                        tr("Файлы привязок (*.prf);;Все файлы (*.*)"), &selectedFilter,
                                                        QFileDialog::DontConfirmOverwrite);
    outApplicationEvent(tr("Экспорт привязок в БД: %1").arg(purpose_full_name));

    if(purpose_path.isEmpty())
        return;

    QString baseNameFile = QFileInfo(purpose_path).baseName();

    QFileInfo fi;

    if(fi.exists(purpose_path)) // Файл уже существует
    {
        int result = QMessageBox::question(this, tr("Экспорт привязок"), tr("Вы действительно хотите перезаписать данные?"));

        if(result == QMessageBox::No)
            return;

        QFile file(purpose_path);

        if(!file.remove())
        {
            showMessageBox(tr("Удаление базы привязок"),
                           tr("Невозможно удалить базу %1!\nВозможно уже используется или у Вас нет прав").arg(baseNameFile), QMessageBox::Warning);
            return;
        }
    }

    CMatrixPurposeModel* model = static_cast<CMatrixPurposeModel*>(table->model());

    if(!model)
    {
        outLogMessage(tr("Запись привязок: Невозможно обратиться к модели представления"));
        return;
    }

    CMatrix matrix = model->matrix();

    if(matrix.rowCount() == 0)
    {
        outLogMessage(tr("Запись привязок: Нет данных в матрице").arg(type));
        return;
    }

    // вставка данных в таблицу
    QString colListBind;
    QString colList;
    for(int col = 0; col < matrix.columnCount(); col++)
    {
        QString colName = QString("col%1").arg(col);
        colList += colName;
        colListBind += QString(":%1").arg(colName);

        if(col != matrix.columnCount() - 1)
        {
            colList += ", ";
            colListBind += ", ";
        }
    }

    QSqlDatabase* db = nullptr;

    if(!connectDb(db, purpose_path))
    {
        showMessageBox(tr("Экспорт привязок"), tr("Невозможно открыть/создать файл \"%1\"").arg(baseNameFile), QMessageBox::Warning);
        disconnectDb(db);
        return;
    }

    if(type != "PROTECTION")
    {
        if(!createTablePurpose(type, db))
        {
            showMessageBox(tr("Экспорт привязок"), tr("Не удалось создать таблицу привязок для %1").arg(purpose_name), QMessageBox::Warning);
            disconnectDb(db);
            return;
        }
    }
    else
    {
        if(!createProjectTableProtection(loadProtectionList().count(), db))
        {
            showMessageBox(tr("Экспорт привязок"), tr("Не удалось создать таблицу привязок для %1").arg(purpose_name), QMessageBox::Warning);
            disconnectDb(db);
            return;
        }
    }

    QSqlQuery query(*db);

    db->transaction();
    query.prepare(QString("INSERT INTO purpose%1 (%2) VALUES(%3)").arg(type).arg(colList).arg(colListBind));

    m_progressbar->setProgressTitle(tr("Экспорт привязок %1").arg(purpose_name));
    m_progressbar->progressStart();
    m_progressbar->setSettings(0, matrix.rowCount(), "привязок");

    bool isError = false;

    for(int row = 0; row < matrix.rowCount(); row++)
    {
        CRow::ColumnArray columns = matrix[row].columns();
        for(int col = 0; col < columns.count(); col++)
        {
            CColumn column = columns[col];
            int state = static_cast<int>(column.data().state);

            QString bindVal = QString(":col%1").arg(col);
            query.bindValue(bindVal, state);
        }

        if(!query.exec())
        {
            outLogMessage(tr("Запись привязок <%1>: не удалось вставить строку %2 в таблицу привязок (%3)").arg(type).arg(row).
                                                                                                            arg(query.lastError().text()));
            isError = true;
        }

        m_progressbar->progressIncrement();
    }

    db->commit();
    m_progressbar->progressStop();
    disconnectDb(db);

    QString text = QString("Привязки \"%1\" успешно экспортированны в базу данных!").arg(purpose_name);

    if(isError)
        text = QString("Привязки \"%1\" экспортированны в базу данных с ошибками!").arg(purpose_name);

    m_popup->setPopupText(text);
    outLogMessage(text);
    m_popup->show();
}
//------------------------------------------------------
void ConfiguratorWindow::exportProtectionAutomaticToDB()
{
    // выбираем файл для экспорта
    QDir dir;

    if(!dir.exists("outputs/profiles"))
        dir.mkdir("outputs/profiles");

    QString name = "Уставки устройства";
    QString selectedFilter = tr("Файлы уставок (*.set)");
    QString set_full_name = tr("%1-%2 (%3 - %4)").arg(name).arg(m_status_bar->serialNumberText()).
                                                      arg(QDate::currentDate().toString("dd_MM_yyyy")).
                                                      arg(QTime::currentTime().toString("HH_mm_ss"));
    QString set_path = QFileDialog::getSaveFileName(this, tr("Экспорт уставок устройства в базу данных"),
                                                        dir.absolutePath() + QString("/outputs/profiles/%1.%2").
                                                        arg(set_full_name).arg("set"),
                                                        tr("Файлы уставок (*.set);;Все файлы (*.*)"), &selectedFilter,
                                                        QFileDialog::DontConfirmOverwrite);
    outLogMessage(tr("Экспорт уставок устройства в БД: %1"));

    QString baseNameFile = QFileInfo(set_path).baseName();

    QFileInfo fi;

    if(fi.exists(set_path)) // Файл уже существует
    {
        int result = QMessageBox::question(this, tr("Экспорт уставок устройства"), tr("Вы действительно хотите перезаписать данные?"));

        if(result == QMessageBox::No)
            return;

        QFile file(set_path);

        if(!file.remove())
        {
            showMessageBox(tr("Удаление базы уставок"),
                           tr("Невозможно удалить базу %1!\nВозможно уже используется или у Вас нет прав").arg(baseNameFile), QMessageBox::Warning);
            return;
        }
    }

    QSqlDatabase *db = nullptr;

    m_progressbar->setProgressTitle(tr("Экспорт уставок %1").arg(name));
    m_progressbar->progressStart();
    m_progressbar->setSettings(0, 100, "%");

    if(!connectDb(db, set_path))
    {
        showMessageBox(tr("Экспорт уставок устройства"), tr("Невозможно открыть/создать файл \"%1\"").arg(baseNameFile), QMessageBox::Warning);
        disconnectDb(db);
        return;
    }

    m_progressbar->progressIncrement(5);

    // Создание таблицы уставок Аналоговые входы
    if(!createProjectTableSet("ANALOG", db))
    {
        showMessageBox(tr("Создание таблицы уставок группы Аналоговые входы"),
                       tr("Невозможно создать таблицу уставок группы Аналоговые входы в файле проекта"), QMessageBox::Warning);
        disconnectDb(db);
        return;
    }

    m_progressbar->progressIncrement(5);

    // Создание таблицы уставок группы "Защиты по току"
    if(!createProjectTableSet("MTZ", db))
    {
        showMessageBox(tr("Создание таблицы уставок группы МТЗ"), tr("Невозможно создать таблицу уставок группы МТЗ в файле проекта"),
                       QMessageBox::Warning);
        disconnectDb(db);
        return;
    }

    m_progressbar->progressIncrement(5);

    // Создание таблицы уставок группы "Защиты по напряжению"
    if(!createProjectTableSet("PWR", db))
    {
        showMessageBox(tr("Создание таблицы уставок группы Защиты по напряжению"),
                       tr("Невозможно создать таблицу уставок группы Защиты по напряжнию в файле проекта"), QMessageBox::Warning);
        disconnectDb(db);
        return;
    }

    m_progressbar->progressIncrement(5);

    // Создание таблицы уставок группы "Направленные"
    if(!createProjectTableSet("DIR", db))
    {
        showMessageBox(tr("Создание таблицы уставок группы Направленные"), tr("Невозможно создать таблицу уставок группы Направленные в файле проекта"),
                       QMessageBox::Warning);
        disconnectDb(db);
        return;
    }

    m_progressbar->progressIncrement(5);

    // Создание таблицы уставок группы "Защиты по частоте"
    if(!createProjectTableSet("FREQ", db))
    {
        showMessageBox(tr("Создание таблицы уставок группы Защиты по частоте"),
                       tr("Невозможно создать таблицу уставок группы Защиты по частоте в файле проекта"), QMessageBox::Warning);
        disconnectDb(db);
        return;
    }

    m_progressbar->progressIncrement(5);

    // Создание таблицы уставок группы "Внешние защиты"
    if(!createProjectTableSet("EXT", db))
    {
        showMessageBox(tr("Создание таблицы уставок группы Внешние защиты"),
                       tr("Невозможно создать таблицу уставок группы Внешние защиты в файле проекта"), QMessageBox::Warning);
        disconnectDb(db);
        return;
    }

    m_progressbar->progressIncrement(5);

    // Создание таблицы уставок группы "Для двигателя"
    if(!createProjectTableSet("MOTOR", db))
    {
        showMessageBox(tr("Создание таблицы уставок группы Защиты для двигателя"),
                       tr("Невозможно создать таблицу уставок группы Защиты для двигателя в файле проекта"), QMessageBox::Warning);
        disconnectDb(db);
        return;
    }

    m_progressbar->progressIncrement(5);

    // Создание таблицы уставок группы "Защиты по температуре"
    if(!createProjectTableSet("TEMP", db))
    {
        showMessageBox(tr("Создание таблицы уставок группы Защиты по температуре"),
                       tr("Невозможно создать таблицу уставок группы Защиты по температуре в файле проекта"), QMessageBox::Warning);
        disconnectDb(db);
        return;
    }

    m_progressbar->progressIncrement(5);

    // Создание таблицы уставок группы "Резервные защиты"
    if(!createProjectTableSet("RESERVE", db))
    {
        showMessageBox(tr("Создание таблицы уставок группы Резервные защиты"),
                       tr("Невозможно создать таблицу уставок группы Резервные защиты в файле проекта"), QMessageBox::Warning);
        disconnectDb(db);
        return;
    }

    m_progressbar->progressIncrement(5);

    // Создание таблицы уставок группы "Предварительного контроля"
    if(!createProjectTableSet("CTRL", db))
    {
        showMessageBox(tr("Создание таблицы уставок группы Предварительного контроля"),
                       tr("Невозможно создать таблицу уставок группы Предварительного контроля в файле проекта"), QMessageBox::Warning);
        disconnectDb(db);
        return;
    }

    m_progressbar->progressIncrement(5);

    // Создание таблицы уставок группы "Автоматика->Выключатель"
    if(!createProjectTableSet("AUTO_SWITCH", db))
    {
        showMessageBox(tr("Создание таблицы уставок группы Автоматика"), tr("Невозможно создать таблицу уставок группы Автоматика в файле проекта"),
                       QMessageBox::Warning);
        disconnectDb(db);
        return;
    }

    // Создание таблицы уставок группы "Автоматика->АПВ"
    if(!createProjectTableSet("AUTO_APV", db))
    {
        showMessageBox(tr("Создание таблицы уставок группы Автоматика"), tr("Невозможно создать таблицу уставок группы Автоматика в файле проекта"),
                       QMessageBox::Warning);
        disconnectDb(db);
        return;
    }

    // Создание таблицы уставок группы "Автоматика->АВР"
    if(!createProjectTableSet("AUTO_AVR", db))
    {
        showMessageBox(tr("Создание таблицы уставок группы Автоматика"), tr("Невозможно создать таблицу уставок группы Автоматика в файле проекта"),
                       QMessageBox::Warning);
        disconnectDb(db);
        return;
    }

    // Создание таблицы уставок группы "Автоматика->КЦН"
    if(!createProjectTableSet("AUTO_KCN", db))
    {
        showMessageBox(tr("Создание таблицы уставок группы Автоматика"), tr("Невозможно создать таблицу уставок группы Автоматика в файле проекта"),
                       QMessageBox::Warning);
        disconnectDb(db);
        return;
    }

    m_progressbar->progressIncrement(5);

    saveDeviceSetToProject(DEVICE_MENU_ITEM_SETTINGS_ITEM_IN_ANALOG, "ANALOG", db);
    m_progressbar->progressIncrement(5);
    saveDeviceSetToProject(DEVICE_MENU_PROTECT_ITEM_CURRENT, "MTZ", db);
    m_progressbar->progressIncrement(5);
    saveDeviceSetToProject(DEVICE_MENU_PROTECT_ITEM_POWER, "PWR", db);
    m_progressbar->progressIncrement(5);
    saveDeviceSetToProject(DEVICE_MENU_PROTECT_ITEM_LEAK, "DIR", db);
    m_progressbar->progressIncrement(5);
    saveDeviceSetToProject(DEVICE_MENU_PROTECT_ITEM_FREQUENCY, "FREQ", db);
    m_progressbar->progressIncrement(5);
    saveDeviceSetToProject(DEVICE_MENU_PROTECT_ITEM_EXTERNAL, "EXT", db);
    m_progressbar->progressIncrement(5);
    saveDeviceSetToProject(DEVICE_MENU_PROTECT_ITEM_MOTOR, "MOTOR", db);
    m_progressbar->progressIncrement(5);
    saveDeviceSetToProject(DEVICE_MENU_PROTECT_ITEM_TEMPERATURE, "TEMP", db);
    m_progressbar->progressIncrement(5);
    saveDeviceSetToProject(DEVICE_MENU_PROTECT_ITEM_RESERVE, "RESERVE", db);
    m_progressbar->progressIncrement(5);
    saveDeviceSetToProject(DEVICE_MENU_PROTECT_ITEM_CONTROL, "CTRL", db);
    m_progressbar->progressIncrement(5);
    saveDeviceSetToProject(DEVICE_MENU_ITEM_AUTOMATION_SWITCH, "AUTO_SWITCH", db);
    saveDeviceSetToProject(DEVICE_MENU_ITEM_AUTOMATION_APV, "AUTO_APV", db);
    saveDeviceSetToProject(DEVICE_MENU_ITEM_AUTOMATION_AVR, "AUTO_AVR", db);
    saveDeviceSetToProject(DEVICE_MENU_ITEM_AUTOMATION_KCN, "AUTO_KCN", db);
    m_progressbar->progressIncrement(5);

    disconnectDb(db);
    m_progressbar->progressStop();

    QString text = tr("Защита и автоматика (уставки) успешно экспортированы в БД");
    m_popup->setPopupText(text);
    m_popup->show();
    outLogMessage(text);
}
//--------------------------------------------------------
void ConfiguratorWindow::importProtectionAutomaticFromDB()
{
    // выбираем файл для импорта
    QDir dir;
    QString selectedFilter = tr("Файлы уставок (*.set)");
    QString set_full_name = tr("Уставки устройства - %1").arg(m_status_bar->serialNumberText());
    QString set_path = QFileDialog::getOpenFileName(this, tr("Импорт уставок устройства из базы данных"),
                                                    dir.absolutePath() + QString("/outputs/profiles/%1.%2").
                                                    arg(set_full_name).arg("set"),
                                                    tr("Файлы привязок (*.set);;Все файлы (*.*)"), &selectedFilter);

    if(set_path.isEmpty())
        return;

    QSqlDatabase* db = nullptr;

    if(!connectDb(db, set_path)) // открываем базу данных
    {
        showMessageBox(tr("Импорт уставок устройства"), tr("Невозможно открыть базу данных уставок устройства"),
                       QMessageBox::Warning);
        disconnectDb(db);
        return;
    }

    if(!db || (db && !db->isOpen()))
    {
        outLogMessage(tr("Загрузка уставок устройства: Файл проекта не создан, либо закрыт"));
        return;
    }

    m_progressbar->setProgressTitle(tr("Импорт уставок устройства"));
    m_progressbar->progressStart();
    m_progressbar->setSettings(0, 100, "%");

    loadDeviceSetToProject(DEVICE_MENU_ITEM_SETTINGS_ITEM_IN_ANALOG, "ANALOG", db);
    m_progressbar->progressIncrement(10);
    loadDeviceSetToProject(DEVICE_MENU_PROTECT_ITEM_CURRENT, "MTZ", db);
    m_progressbar->progressIncrement(10);
    loadDeviceSetToProject(DEVICE_MENU_PROTECT_ITEM_POWER, "PWR", db);
    m_progressbar->progressIncrement(10);
    loadDeviceSetToProject(DEVICE_MENU_PROTECT_ITEM_LEAK, "DIR", db);
    m_progressbar->progressIncrement(10);
    loadDeviceSetToProject(DEVICE_MENU_PROTECT_ITEM_FREQUENCY, "FREQ", db);
    m_progressbar->progressIncrement(10);
    loadDeviceSetToProject(DEVICE_MENU_PROTECT_ITEM_EXTERNAL, "EXT", db);
    m_progressbar->progressIncrement(10);
    loadDeviceSetToProject(DEVICE_MENU_PROTECT_ITEM_MOTOR, "MOTOR", db);
    m_progressbar->progressIncrement(10);
    loadDeviceSetToProject(DEVICE_MENU_PROTECT_ITEM_TEMPERATURE, "TEMP", db);
    m_progressbar->progressIncrement(10);
    loadDeviceSetToProject(DEVICE_MENU_PROTECT_ITEM_RESERVE, "RESERVE", db);
    m_progressbar->progressIncrement(10);
    loadDeviceSetToProject(DEVICE_MENU_PROTECT_ITEM_CONTROL, "CTRL", db);
    m_progressbar->progressIncrement(10);
    loadDeviceSetToProject(DEVICE_MENU_ITEM_AUTOMATION_SWITCH, "AUTO_SWITCH", db);
    loadDeviceSetToProject(DEVICE_MENU_ITEM_AUTOMATION_APV, "AUTO_APV", db);
    loadDeviceSetToProject(DEVICE_MENU_ITEM_AUTOMATION_AVR, "AUTO_AVR", db);
    loadDeviceSetToProject(DEVICE_MENU_ITEM_AUTOMATION_KCN, "AUTO_KCN", db);

    disconnectDb(db);
    m_progressbar->progressStop();

    QString text = tr("Защита и автоматика (уставки) успешно импортированы из БД");
    m_popup->setPopupText(text);
    m_popup->show();
    outLogMessage(text);
}
//---------------------------------------------------------------
void ConfiguratorWindow::importPurposeFromDb(const QString &type)
{
    CPurposeTableView *table = purposeTableByType(type);

    if(!table)
    {
        showMessageBox(tr("Импорт привязок"), tr("Не удалось получить доступ к таблице журнала"), QMessageBox::Warning);
        return;
    }

    QString purpose_name = table->property("NAME").toString();

    if(purpose_name.isEmpty())
    {
        showMessageBox(tr("Импорт привязок"), tr("Не удалось определить имя привязок"), QMessageBox::Warning);
        return;
    }

    // выбираем файл для импорта
    QDir dir;
    QString selectedFilter    = tr("Файлы привязок (*.prf)");
    QString purpose_full_name = tr("Привязки %1-%2").arg(purpose_name).arg(m_status_bar->serialNumberText());
    QString purpose_path      = QFileDialog::getOpenFileName(this, tr("Импорт привязок %1 из базы данных").arg(purpose_name),
                                                             dir.absolutePath() + QString("/outputs/profiles/%1.%2").
                                                             arg(purpose_full_name).arg("prf"),
                                                             tr("Файлы привязок (*.prf);;Все файлы (*.*)"), &selectedFilter);

    if(purpose_path.isEmpty())
        return;

    QSqlDatabase* db = nullptr;

    if(!connectDb(db, purpose_path)) // открываем базу данных
    {
        showMessageBox(tr("Импорт привязок"), tr("Невозможно открыть базу данных привязок \"%1\"").arg(purpose_name), QMessageBox::Warning);
        disconnectDb(db);
        return;
    }

    if(!db || (db && !db->isOpen()))
    {
        outLogMessage(tr("Загрузка привязок: Файл проекта не создан, либо закрыт"));
        return;
    }

    CMatrixPurposeModel* model = static_cast<CMatrixPurposeModel*>(table->model());

    if(!model)
    {
        outLogMessage(tr("Загрузка привязок: Невозможно обратиться к модели представления"));
        return;
    }

    CMatrix& matrix = model->matrix();
    QSqlQuery query(*db);
    QString query_str = QString("SELECT * FROM purpose%1;").arg(type);

    if(!query.exec(query_str))
    {
        outLogMessage(tr("Загрузка привязок: не удалось прочитать привязки из БД: %1").arg(query.lastError().text()));
        disconnectDb(db);
        return;
    }

    int row = 0;

    while(query.next())
    {
        for(int col = 0; col < matrix.columnCount(); col++)
        {
            QString colName = QString("col%1").arg(col);
            StateType state = static_cast<StateType>(query.value(colName).toInt());

            matrix[row][col].data().state = state;
        }

        row++;
    }

    model->updateData();
    disconnectDb(db);

    QString text = QString("Привязки \"%1\" успешно импортированны из базы данных!").arg(purpose_name);

    m_popup->setPopupText(text);
    outLogMessage(text);
    m_popup->show();
}
/*!
 * \brief ConfiguratorWindow::importPurposetToTableFromExcelStart
 *
 * Импорт матрицы привязок из excel
 */
void ConfiguratorWindow::importPurposetToTableFromExcelStart()
{
    if(m_isProject)
    {
        m_popup->setPopupText(tr("Невозможно импортировать привязки при созданном проекте"));
        m_popup->show();
        return;
    }

    QStringList logins = loadLoginList();

    if(logins.isEmpty())
        return;

    CUserDialog* userDialog = new CUserDialog(logins, this);

    int answer = userDialog->exec();

    if(answer == QDialog::Accepted)
    {
        CUserDialog::user_t usr = userDialog->user();

        if(!usr.password.isEmpty())
        {
            QString pass = loadUserPassword(usr.login);

            if(usr.password.toUpper() == pass.toUpper())
            {
                QDir dir;

                QString fileName = QFileDialog::getOpenFileName(this, tr("Импорт матрицы привязок из Excel"),
                                                                dir.absolutePath(), "Excel (*.xlsx)");

                if(fileName.isEmpty())
                    return;

                QXlsx::Document xlsx(fileName);

                QStringList sheet_list = xlsx.sheetNames();

                if(!sheet_list.isEmpty())
                {
                    if(sheet_list.contains("VAR", Qt::CaseInsensitive) &&
                       sheet_list.contains("VAR_GROUP", Qt::CaseInsensitive) &&
                       sheet_list.contains("INPUT", Qt::CaseInsensitive) &&
                       sheet_list.contains("RELAY", Qt::CaseInsensitive) &&
                       sheet_list.contains("LED", Qt::CaseInsensitive))
                    {
                        importPurposetToTableFromExcel(xlsx);
                    }
                }
            }
            else
            {
                m_popup->setPopupText(tr("Ошибка: пароль неправильный!"));
                outLogMessage(tr("Ошибка: пароль неправильный!"));
                m_popup->show();
            }
        }
        else
        {
            m_popup->setPopupText(tr("Ошибка: пароль не может быть пустым."));
            outLogMessage(tr("Ошибка: пароль не может быть пустым."));
            m_popup->show();
        }
    }

    delete userDialog;
    userDialog = nullptr;
}
//----------------------------------------------------------------------------
void ConfiguratorWindow::importPurposetToTableFromExcel(QXlsx::Document &xlsx)
{
    if(!m_system_db.isOpen())
    {
        m_popup->setPopupText(tr("Ошибка импорта матрицы привязок:\nНет доступа к системной базе данных!"));
        m_popup->show();
        return;
    }

    // Загрузка переменных в таблицу variable
    if(!xlsx.selectSheet("var"))
    {
        m_popup->setPopupText(tr("Ошибка импорта матрицы привязок:\nНет доступа к странице \"var\"!"));
        m_popup->show();
        return;
    }

    m_progressbar->setProgressTitle(tr("Импорт матрицы привязок"));
    m_progressbar->progressStart();
    m_progressbar->setSettings(0, 100, "%");


    QStringList column_list =  QStringList() << "key" << "group_id" << "sort_id" << "type_func" << "type_sort" << "bit" << "name" <<
                                                "description";

    int row_count = rowSheetExcel(xlsx, column_list);

    if(row_count == -1)
    {
        m_progressbar->progressStop();
        m_popup->setPopupText(tr("Ошибка импорта матрицы привязок:\nНевозможно прочитать список переменных!"));
        m_popup->show();
        return;
    }

    QVector<import_variable_t> variables;

    for(int row = 0, reserve = 0; row < row_count; row++)
    {
        import_variable_t var;

        var.key = xlsx.read(row + 2, 1).toString();
        if(var.key.toUpper() == "RESERVE")
            var.key += QString("_%1").arg(reserve++);
        var.group_id = xlsx.read(row + 2, 2).toInt();
        var.sort_id = xlsx.read(row + 2, 3).toInt();
        var.type_func = xlsx.read(row + 2, 4).toString();
        var.type_sort = xlsx.read(row + 2, 5).toInt();
        var.bit = xlsx.read(row + 2, 6).toInt();
        var.name = xlsx.read(row + 2, 7).toString();
        var.description = xlsx.read(row + 2, 8).toString();

        variables << var;
    }

    exportVariableToDbFromExcel(variables);
    m_progressbar->setProgressValue(20);

    // Загрузка групп в таблицу var_group
    if(!xlsx.selectSheet("var_group"))
    {
        m_progressbar->progressStop();
        m_popup->setPopupText(tr("Ошибка импорта матрицы привязок:\nНет доступа к странице \"var_group\"!"));
        m_popup->show();
        return;
    }

    column_list.clear();
    column_list = QStringList() << "id" << "name";

    row_count = rowSheetExcel(xlsx, column_list);

    if(row_count == -1)
    {
        m_progressbar->progressStop();
        m_popup->setPopupText(tr("Ошибка импорта матрицы привязок:\nНевозможно прочитать список групп переменных!"));
        m_popup->show();
        return;
    }

    QVector<QPair<int, QString>> groups;

    for(int row = 0; row < row_count; row++)
    {
        int group_id = xlsx.read(row + 2, 1).toInt();
        QString group_name = xlsx.read(row + 2, 2).toString();

        QPair<int, QString> group = qMakePair(group_id, group_name);
        groups << group;
    }

    exportGroupToDbFromExcel(groups);
    m_progressbar->setProgressValue(40);

    // Загрузка входов/выходов в таблицу purpose
    QStringList io_type = QStringList() << "input" << "relay" << "led";

    QSqlQuery query(m_system_db);

    if(!query.exec("DELETE FROM io_list;"))
    {
        m_progressbar->progressStop();
        m_popup->setPopupText(tr("Ошибка импорта матрицы привязок:\nНевозможно очистить таблицу списка входов/выходов\n(%1)!").
                              arg(query.lastError().text()));
        m_popup->show();
        return;
    }

    query.clear();

    if(!query.exec("DELETE FROM purpose;"))
    {
        m_popup->setPopupText(tr("Ошибка импорта матрицы привязок:\nНевозможно очистить таблицу привязок "
                                 "входов/выходов к переменным\n(%1)!").arg(query.lastError().text()));
        m_popup->show();
        return;
    }

    for(const QString &type: io_type)
    {
        if(!xlsx.selectSheet(type))
        {
            m_progressbar->progressStop();
            m_popup->setPopupText(tr("Ошибка импорта матрицы привязок:\nНет доступа к странице \"%1\"!").arg(type));
            m_popup->show();

            return;
        }

        column_list.clear();
        column_list = QStringList() << "io" << "address" << "description";

        row_count = rowSheetExcel(xlsx, column_list);

        if(row_count == -1)
        {
            m_popup->setPopupText(tr("Ошибка импорта матрицы привязок:\nНевозможно прочитать список входов/выходов!"));
            m_popup->show();
            return;
        }

        QVector<import_io_t> io_list;

        for(int row = 0; row < row_count; row++)
        {
            QString key = xlsx.read(row + 2, 1).toString();
            int address = xlsx.read(row + 2, 2).toInt();
            QString description = xlsx.read(row + 2, 3).toString();
            QString vars = xlsx.read(row + 2, 4).toString();

            if(!vars.isEmpty())
            {
                QStringList var_list = vars.split(";");

                if(!var_list.isEmpty())
                {
                    for(QString &var: var_list)
                        var = var.trimmed();

                    exportPurposeVaribleToIO(key, var_list);
                }
            }

            import_io_t io = import_io_t({ key, address, description });

            io_list << io;
        }

        exportIOToDbFromExcel(io_list, type);
        m_progressbar->setProgressValue(m_progressbar->progressValue() + 20);
    }

    m_progressbar->progressStop();

    m_popup->setPopupText(tr("Матрица привязок успешно импортирована!"));
    m_popup->show();
}
//-------------------------------------------------------------------------------------------------------------------
void ConfiguratorWindow::exportVariableToDbFromExcel(const QVector<ConfiguratorWindow::import_variable_t> &variables)
{
    if(variables.isEmpty())
        return;

    QSqlQuery query(m_system_db);

    if(!query.exec("DELETE FROM variable;"))
    {
        m_popup->setPopupText(tr("Ошибка импорта матрицы привязок:\nНевозможно очистить таблицу переменных\n(%1)!").
                              arg(query.lastError().text()));
        m_popup->show();
        return;
    }

    query.clear();

    m_system_db.transaction();

    query.prepare("INSERT INTO variable (key, group_id, sort_id, type_function, type_sort, bit, name, description) "
                  "VALUES (:key, :group_id, :sort_id, :type_function, :type_sort, :bit, :name, :description);");

    for(int i = 0; i < variables.count(); i++)
    {
        query.bindValue(":key", variables[i].key);
        query.bindValue(":group_id", variables[i].group_id);
        query.bindValue(":sort_id", variables[i].sort_id);
        query.bindValue(":type_function", variables[i].type_func);
        query.bindValue(":type_sort", variables[i].type_sort);
        query.bindValue(":bit", variables[i].bit);
        query.bindValue(":name", variables[i].name);
        query.bindValue(":description", variables[i].description);

        if(!query.exec())
        {
            QString text = tr("Ошибка импорта матрицы привязок:\nНевозможно вставить переменную \'%1\' в базу данных\n(%2)!").
                           arg(variables[i].key).arg(query.lastError().text());
            m_popup->setPopupText(text);
            m_popup->show();
            outLogMessage(text);
        }
    }

    m_system_db.commit();
}
//--------------------------------------------------------------------------------------------
void ConfiguratorWindow::exportGroupToDbFromExcel(const QVector<QPair<int, QString> > &groups)
{
    if(groups.isEmpty())
        return;

    QSqlQuery query(m_system_db);

    if(!query.exec("DELETE FROM var_group;"))
    {
        m_popup->setPopupText(tr("Ошибка импорта матрицы привязок:\nНевозможно очистить таблицу групп переменных\n(%1)!").
                              arg(query.lastError().text()));
        m_popup->show();
        return;
    }

    query.clear();

    m_system_db.transaction();

    query.prepare("INSERT INTO var_group (id, name) VALUES (:id, :name);");

    for(int i = 0; i < groups.count(); i++)
    {
        query.bindValue(":id", groups[i].first);
        query.bindValue(":name", groups[i].second);

        if(!query.exec())
        {
            QString text = tr("Ошибка импорта матрицы привязок:\nНевозможно вставить группу в базу данных\n(%1)!").
                           arg(query.lastError().text());
            m_popup->setPopupText(text);
            m_popup->show();
            outLogMessage(text);
        }
    }

    m_system_db.commit();
}
//--------------------------------------------------------------------------------------------------------------------------
void ConfiguratorWindow::exportIOToDbFromExcel(const QVector<ConfiguratorWindow::import_io_t> &io_list, const QString &type)
{
    if(io_list.isEmpty() || type.isEmpty())
        return;

    QSqlQuery query(m_system_db);
    m_system_db.transaction();

    query.prepare("INSERT INTO io_list (key, address, type, description) VALUES (:key, :address, :type, :description);");

    for(int i = 0; i < io_list.count(); i++)
    {
        query.bindValue(":key", io_list[i].key);
        query.bindValue(":address", io_list[i].address);
        query.bindValue(":type", type.toUpper());
        query.bindValue(":description", io_list[i].description);

        if(!query.exec())
        {
            QString text = tr("Ошибка импорта матрицы привязок:\nНевозможно вставить вход/выход в базу данных\n(%1)!").
                           arg(query.lastError().text());
            m_popup->setPopupText(text);
            m_popup->show();
            outLogMessage(text);
        }
    }

    m_system_db.commit();
}
//-----------------------------------------------------------------------------------------------
void ConfiguratorWindow::exportPurposeVaribleToIO(const QString &key, const QStringList &io_list)
{
    if(key.isEmpty() || io_list.isEmpty())
        return;

    QSqlQuery query(m_system_db);
    m_system_db.transaction();

    query.prepare("INSERT INTO purpose (io_key, var_key) VALUES (:io_key, :var_key);");

    for(int i = 0; i < io_list.count(); i++)
    {
        query.bindValue(":io_key", key);
        query.bindValue(":var_key", io_list[i]);

        if(!query.exec())
        {
            QString text = tr("Ошибка импорта матрицы привязок:\nНевозможно вставить привязку входа/выхода в базу данных\n(%1)!").
                           arg(query.lastError().text());
            m_popup->setPopupText(text);
            m_popup->show();
            outLogMessage(text);
        }
    }

    m_system_db.commit();
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
//--------------------------------------------------------------
void ConfiguratorWindow::readJournalShiftPtr(JournalPtr journal)
{
    if(!journal)
        return;

//    CModBusDataUnit unit(quint8(m_serialPortSettings_window->deviceID()), CModBusDataUnit::ReadHoldingRegisters,
//                         quint16(journal->addrShiftPtr())), QVector<quint16>() << 2);

//    unit.setProperty(tr("REQUEST"), READ_JOURNAL_SHIFT_PTR);
//    QVariant var;
//    var.setValue<JournalPtr>(journal);
//    unit.setProperty(tr("JOURNAL"), var);

//    m_modbus->sendData(unit);
}
//---------------------------------------------------
void ConfiguratorWindow::processReadJournalShiftPtr()
{
    JournalPtr journal = currentJournalWidget();

    if(journal)
        readJournalShiftPtr(journal);
}
//-----------------------------------------------------------
void ConfiguratorWindow::readJournalCount(JournalPtr journal)
{
        CModBusDataUnit unit(quint8(m_serialPortSettings_window->deviceID()), CModBusDataUnit::ReadInputRegisters,
                             quint16(journal->addrMsgCount()), 2);
        unit.setProperty(tr("REQUEST"), READ_JOURNAL_COUNT);
        QVariant var;
        var.setValue<JournalPtr>(journal);
        unit.setProperty(tr("JOURNAL"), var);

        m_modbus->sendData(unit); 
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
 *
 * Этот метод устаревший и нигде не используется - на удаление
 */
void ConfiguratorWindow::setLineEditValidator(QObject* object)
{
    CLineEdit* lineEdit = qobject_cast<CLineEdit*>(object);

    QString str = lineEdit->objectName().toUpper();
    QString key = str.remove("LE");

    double limit_min = 0.0;
    double limit_max = 0.0;

    if(m_limits.find(key) != m_limits.end())
    {
        limit_min = double(m_limits[key].limit_min);
        limit_max = double(m_limits[key].limit_max);
    }

    QWidget* widget = findChild<QWidget*>(QString("label%1").arg(key));

    if(widget)
    {
        QString classWgt = widget->metaObject()->className();

        if(classWgt == tr("QLabel") && limit_max != 0.0)
        {
            QLabel* label = qobject_cast<QLabel*>(widget);

            if(label)
                label->setText(QString("%1 (%2...%3)").arg(label->text()).
                                                       arg(QLocale::system().toString(limit_min)).
                                                       arg(QLocale::system().toString(limit_max)));
        }
    }

    if(static_cast<int>(limit_min) == -180 && static_cast<int>(limit_max) == 180)
    {
        lineEdit->setValidator(new QIntValidator(static_cast<int>(limit_min), static_cast<int>(limit_max)));
        lineEdit->setText("0");
    }
    else if(limit_min == 0.0 && limit_max == 0.0)
    {
        QDoubleValidator* doubleVal = new QDoubleValidator(0, 100, 6, lineEdit);
        doubleVal->setNotation(QDoubleValidator::StandardNotation);
        doubleVal->setLocale(QLocale::C);
        lineEdit->setValidator(doubleVal);
        lineEdit->setText(QString("%1").arg(QLocale::system().toString(0.0f, 'f', 2)));
    }
    else
    {
        QDoubleValidator* doubleVal = new QDoubleValidator(limit_min, limit_max, 6, lineEdit);
        doubleVal->setNotation(QDoubleValidator::StandardNotation);
        doubleVal->setLocale(QLocale::C);
        lineEdit->setValidator(doubleVal);
        lineEdit->setText(QString("%1").arg(QLocale::system().toString(0.0f, 'f', 2)));
    }
}
//--------------------------------------------------------
void ConfiguratorWindow::panelVisibleCtrl(QWidget* widget)
{
    if(widget == ui->dockWidgetVariable)
    {
        if(ui->dockWidgetVariable->control() &&
           ui->dockWidgetVariable->width() == ui->dockWidgetVariable->control()->minimumWidth())
        {
            if(!ui->dockWidgetVariable->isContentHidden())
            {
                ui->dockWidgetVariable->hideContent();
                ui->dockWidgetVariable->control()->setState(CDockPanelItemCtrl::Close);
            }
        }
        else
        {
            if(ui->dockWidgetVariable->control() && ui->dockWidgetVariable->isContentHidden())
            {
                ui->dockWidgetVariable->showContent();
                ui->dockWidgetVariable->control()->setState(CDockPanelItemCtrl::Open);
            }
        }
    }

    if(widget == ui->centralWgt)
    {
        if(ui->dockWidgetMenuDevice->control() &&
           ui->dockWidgetMenuDevice->width() == ui->dockWidgetMenuDevice->control()->minimumWidth())
        {
            if(!ui->dockWidgetMenuDevice->isContentHidden())
            {
                ui->dockWidgetMenuDevice->hideContent();
                ui->dockWidgetMenuDevice->control()->setState(CDockPanelItemCtrl::Close);
            }
        }
        else
        {
            if(ui->dockWidgetMenuDevice->control() && ui->dockWidgetMenuDevice->isContentHidden())
            {
                ui->dockWidgetMenuDevice->showContent();
                ui->dockWidgetMenuDevice->control()->setState(CDockPanelItemCtrl::Open);
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
//---------------------------------------------
void ConfiguratorWindow::panelVisibleTerminal()
{
    if(m_containerTerminalModbus->isHidden())
    {
        if(m_containerTerminalModbus->anchor() == CContainerWidget::AnchorType::AnchorDockWidget)
        {
            ui->tabWidgetMessage->addContainer(m_containerTerminalModbus);
        }

        m_containerTerminalModbus->show();
    }
    else
    {
        m_containerTerminalModbus->hide();

        if(m_containerTerminalModbus->anchor() == CContainerWidget::AnchorType::AnchorDockWidget)
            emit ui->tabWidgetMessage->removeContainer(tr("Терминал"));
    }
}
//--------------------------------------------
void ConfiguratorWindow::panelVisibleMessage()
{
    if(m_containerWidgetMessage->isHidden())
    {
        if(m_containerWidgetMessage->anchor() == CContainerWidget::AnchorType::AnchorDockWidget)
        {
            ui->tabWidgetMessage->addContainer(m_containerWidgetMessage);
        }

        m_containerWidgetMessage->show();
    }
    else
    {
        m_containerWidgetMessage->hide();

        if(m_containerWidgetMessage->anchor() == CContainerWidget::AnchorType::AnchorDockWidget)
            emit ui->tabWidgetMessage->removeContainer(tr("События"));
    }
}
//------------------------------------------------------
void ConfiguratorWindow::panelVisibleCalculateVariable()
{
    if(m_containerWidgetVariable->isHidden())
        m_containerWidgetVariable->show();
    else
        m_containerWidgetVariable->hide();
}
//-----------------------------------------------
void ConfiguratorWindow::panelVisibleDeviceMenu()
{
    if(m_containerWidgetDeviceMenu)
    {
        if(m_containerWidgetDeviceMenu->isHidden())
            m_containerWidgetDeviceMenu->show();
        else
            m_containerWidgetDeviceMenu->hide();
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

    if(m_purpose_list[indexes.x()].first.contains("DI", Qt::CaseInsensitive) &&
       m_purpose_list[indexes.y()].first.contains("DI", Qt::CaseInsensitive)) // входы
    {
        return ui->tablewgtDiscreteInputPurpose;
    }
    else if(m_purpose_list[indexes.x()].first.contains("DO", Qt::CaseInsensitive) &&
            m_purpose_list[indexes.y()].first.contains("DO", Qt::CaseInsensitive)) // выходы: реле
    {
        return ui->tablewgtRelayPurpose;
    }
    else if(m_purpose_list[indexes.x()].first.contains("LED", Qt::CaseInsensitive) &&
            m_purpose_list[indexes.y()].first.contains("LED", Qt::CaseInsensitive)) // выходы: светодиоды
    {
        return ui->tablewgtLedPurpose;
    }

    return nullptr;
}
//--------------------------------------------------------------------
ConfiguratorWindow::DeviceMenuItemType ConfiguratorWindow::menuIndex()
{
    QTreeWidgetItem* item = m_treeWidgetDeviceMenu->currentItem();

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

    quint8 month = quint8(((data[0]&0x03) << 2) | ((data[1]&0xC0) >> 6));
    quint8 day   = quint8(((data[1]&0x3E) >> 1));

    quint8  hour    = quint8(((data[1]&0x01) << 4) | ((data[2]&0xF0) >> 4));
    quint8  minute  = quint8(((data[2]&0x0F) << 2) | ((data[3]&0xC0) >> 6));
    quint8  second  = quint8(data[3]&0x3F);
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
                    int     sort_id     = query_item.value("sort_id").toInt();
                    int     bit         = query_item.value("bit").toInt();
                    QString name        = query_item.value("name").toString();
                    QString description = query_item.value("description").toString();

                    QSqlQuery query_purpose(m_system_db);

                    if(!io_key.isEmpty())
                    {
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
                    }

                    var_list << var_t({ key, group_id, sort_id, bit, name, description });
                }
            }

            if(!var_list.isEmpty())
            {
                // сортировка переменных в пределах группы по полю sort_id
                std::sort(var_list.begin(), var_list.end(), [](var_t& var1, var_t& var2) { return var1.sort_id < var2.sort_id; } );
                group[id] = group_item_t({ group_name, group_description, var_list });
            }
        }
    }

    return group;
}
//-----------------------------------------------------------------------------------------
QVector<QPair<QString, QString> > ConfiguratorWindow::loadLabelColumns(const QString& type)
{
    QVector<QPair<QString, QString> > labels;

    if(!m_system_db.isOpen())
        return labels;

    QSqlQuery query(m_system_db);
    QString str_db = QString("SELECT key, description FROM io_list WHERE type = \'%1\';").arg(type);

    if(query.exec(str_db))
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
bool ConfiguratorWindow::showErrorMessage(const QString& title, CModBusDataUnit& unit)
{
    if(unit.error() != CModBusDataUnit::ERROR_NO)
    {
        showMessageBox(title, tr("Ошибка: %1(%2).").arg(CModBusDataUnit::errorDescription(unit.error())).arg(QString::number(unit.address())),
                       QMessageBox::Warning);
        return true;
    }

    return false;
}
/*!
 * \brief ConfiguratorWindow::writeDataToExcel
 * \param doc Текущий документ Excel
 * \param name_group Имя группы (н-р, МТЗ1)
 * \param table Указатель на таблицу в которой находятся поля с данными
 * \return возвращает индекс последней строки
 *
 * Запись уставок из таблицы в excel
 */
int ConfiguratorWindow::writeDataToExcel(QXlsx::Document& doc, const CDeviceMenuTableWidget* table, int offset)
{
    if(!table)
        return -1;

    int row_count = 1 + offset; // счетчик вставленных строк в Excel (строки начинаются с индекса 1)
    int pos_group = -1;

    QXlsx::Format headerFormat;
    headerFormat.setHorizontalAlignment(QXlsx::Format::AlignHCenter);
    headerFormat.setPatternBackgroundColor(Qt::lightGray);
    headerFormat.setPatternForegroundColor(Qt::black);
    headerFormat.setBorderColor(Qt::black);
    headerFormat.setBorderStyle(QXlsx::Format::BorderThick);
    headerFormat.setFontBold(true);

    QXlsx::Format dataFormat;
    dataFormat.setHorizontalAlignment(QXlsx::Format::AlignHCenter);
    dataFormat.setBorderColor(Qt::black);
    dataFormat.setBorderStyle(QXlsx::Format::BorderMedium);

    for(int row = 0; row < table->rowCount(); row++)
    {
        QTableWidgetItem* item = table->item(row, 0);

        if(item)
        {
            CDeviceMenuTableWidget::RowType rowType = static_cast<CDeviceMenuTableWidget::RowType>(item->data(Qt::UserRole + 100).toInt());
            if(rowType == CDeviceMenuTableWidget::HEADER || rowType == CDeviceMenuTableWidget::SUBHEADER)
            {
                if(pos_group != -1)
                {
                    doc.groupRows(pos_group, row_count, true);
                    pos_group = -1;
                }

                doc.write(QString("A%1").arg(++row_count), item->text());
                doc.mergeCells(QString("A%1:C%2").arg(row_count).arg(row_count), headerFormat);
                pos_group = row_count + 1;
                continue;
            }
        }

        QWidget* wgt_value = groupMenuCellWidget(table, row, 1);
        QWidget* wgt_param = groupMenuCellWidget(table, row, 0);
        QWidget* wgt_unit  = groupMenuCellWidget(table, row, 2);

        if(!wgt_value || !wgt_param)
            continue;

        QString name_param  = qobject_cast<QLabel*>(wgt_param)->text();
        QString str_value   = "";
        QString value_range = "";

        if(wgt_value->objectName().toUpper().contains("COMBOBOX"))
        {
            CMenuComboBox* cb = qobject_cast<CMenuComboBox*>(wgt_value);

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
        else if(wgt_value->objectName().toUpper().contains("LINEEDIT"))
        {
            CLineEdit* le = qobject_cast<CLineEdit*>(wgt_value);
            str_value = le->text();

            if(wgt_unit)
            {
                QLabel* label_unit = qobject_cast<QLabel*>(wgt_unit);

                if(label_unit)
                    value_range = label_unit->text();
            }
        }

        row_count++;

        doc.write(QString("A%1").arg(row_count), name_param, dataFormat);
        doc.write(QString("B%1").arg(row_count), str_value, dataFormat);
        doc.write(QString("C%1").arg(row_count), value_range, dataFormat);
    }

    if(pos_group != -1)
    {
        doc.groupRows(pos_group, row_count, true);
    }

    emit m_progressbar->increment();

    return row_count;
}
//------------------------------------
void ConfiguratorWindow::initConnect()
{
    connect(ui->toolButtonConnect, &QToolButton::clicked, this, &ConfiguratorWindow::serialPortCtrl);
    connect(m_modbus, &CModBus::stateChanged, this, &ConfiguratorWindow::stateChanged);
    connect(m_modbus, &CModBus::stateChanged, ui->pushButtonJournalRead, &QPushButton::setEnabled);
    connect(m_modbus, &CModBus::readyRead, this, &ConfiguratorWindow::readyReadData);
    connect(m_modbus, &CModBus::errorDevice, this, &ConfiguratorWindow::errorDevice);
    connect(m_modbus, &CModBus::baudrateChanged, m_serialPortSettings_window, &CSerialPortSetting::setBaudrate);
    connect(m_modbus, &CModBus::errorChannel, this, &ConfiguratorWindow::errorConnect);
    connect(ui->toolButtonConnect, &QPushButton::clicked, m_modbus, &CModBus::userStateCtrl);
    connect(m_serialPortSettings_window, &CSerialPortSetting::refreshSerialPort, this, &ConfiguratorWindow::refreshSerialPort);
    connect(m_tim_calculate, &QTimer::timeout, this, &ConfiguratorWindow::calculateRead);
    connect(ui->checkboxCalibTimeout, &QCheckBox::clicked, this, &ConfiguratorWindow::chboxCalculateTimeoutStateChanged);
    connect(ui->sboxTimeoutCalc, SIGNAL(valueChanged(int)), this, SLOT(timeCalculateChanged(int)));

    // Управление контейнерами панелей
    connect(m_containerIndicatorState, &CContainerWidget::containerClose, ui->pushButtonIndicatorStates, &QPushButton::setChecked);
    connect(m_containerIndicatorState, &CContainerWidget::containerClose, this, &ConfiguratorWindow::indicatorVisiblity);
    connect(m_containerMonitorI11I17, &CContainerWidget::containerClose, ui->pushButtonMonitorI11_I17, &QPushButton::setChecked);
    connect(m_containerMonitorI11I17, &CContainerWidget::containerClose, this, &ConfiguratorWindow::monitorI11I17Visiblity);
    connect(m_containerOutputAll, &CContainerWidget::containerClose, ui->pushButtonOutputAll, &QPushButton::setChecked);
    connect(m_containerOutputAll, &CContainerWidget::containerClose, this, &ConfiguratorWindow::outputAllVisiblity);
    connect(m_containerInputs, &CContainerWidget::containerClose, ui->pushButtonInputs, &QPushButton::setChecked);
    connect(m_containerInputs, &CContainerWidget::containerClose, this, &ConfiguratorWindow::inputVisiblity);
    connect(m_containerStatusInfo, &CContainerWidget::containerClose, ui->pushButtonStatusInfo, &QPushButton::setChecked);
    connect(m_containerStatusInfo, &CContainerWidget::containerClose, this, &ConfiguratorWindow::statusInfoVisiblity);
    connect(m_containerDebugInfo, &CContainerWidget::containerClose, ui->pushButtonDebugInfo, &QPushButton::setChecked);
    connect(m_containerDebugInfo, &CContainerWidget::containerClose, this, &ConfiguratorWindow::debugInfoVisiblity);
    connect(m_containerTerminalModbus, &CContainerWidget::containerClose, ui->pushButtonTerminal, &QPushButton::setChecked);
    connect(m_containerTerminalModbus, &CContainerWidget::containerClose, this, &ConfiguratorWindow::panelVisibleTerminal);
    connect(m_containerWidgetDeviceMenu, &CContainerWidget::containerClose, ui->pushButtonDeviceMenu, &QPushButton::setChecked);
    connect(m_containerWidgetDeviceMenu, &CContainerWidget::containerClose, this, &ConfiguratorWindow::panelVisibleDeviceMenu);
    connect(m_containerWidgetMessage, &CContainerWidget::containerClose, ui->pushButtonMessageEvent, &QPushButton::setChecked);
    connect(m_containerWidgetMessage, &CContainerWidget::containerClose, this, &ConfiguratorWindow::panelVisibleMessage);
    connect(m_containerWidgetVariable, &CContainerWidget::containerClose, ui->pushButtonCalculateVarible, &QPushButton::setChecked);
    connect(m_containerWidgetVariable, &CContainerWidget::containerClose, this, &ConfiguratorWindow::panelVisibleCalculateVariable);

    connect(ui->pushButtonIndicatorStates, &QPushButton::clicked, this, &ConfiguratorWindow::indicatorVisiblity);
    connect(ui->pushButtonMonitorI11_I17, &QPushButton::clicked, this, &ConfiguratorWindow::monitorI11I17Visiblity);
    connect(ui->pushButtonOutputAll, &QPushButton::clicked, this, &ConfiguratorWindow::outputAllVisiblity);
    connect(ui->pushButtonInputs, &QPushButton::clicked, this, &ConfiguratorWindow::inputVisiblity);
    connect(ui->pushButtonStatusInfo, &QPushButton::clicked, this, &ConfiguratorWindow::statusInfoVisiblity);
    connect(ui->pushButtonDebugInfo, &QPushButton::clicked, this, &ConfiguratorWindow::debugInfoVisiblity);
    connect(ui->pushButtonTerminal, &QPushButton::clicked, this, &ConfiguratorWindow::panelVisibleTerminal);
    connect(ui->pushButtonDeviceMenu, &QPushButton::clicked, this, &ConfiguratorWindow::panelVisibleDeviceMenu);
    connect(ui->pushButtonMessageEvent, &QPushButton::clicked, this, &ConfiguratorWindow::panelVisibleMessage);
    connect(ui->pushButtonCalculateVarible, &QPushButton::clicked, this, &ConfiguratorWindow::panelVisibleCalculateVariable);
    //...

    connect(m_status_window, &CStatusInfo::buttonUpdate, this, &ConfiguratorWindow::readStatusInfo);
    connect(m_treeWidgetDeviceMenu, &QTreeWidget::itemClicked, this, &ConfiguratorWindow::itemClicked);
    connect(ui->pbtnReadAllBlock, &QPushButton::clicked, this, &ConfiguratorWindow::readSettings);
    connect(ui->pbtnWriteAllBlock, &QPushButton::clicked, this, &ConfiguratorWindow::writeSettings);
    connect(ui->pbtnReadCurrentBlock, &QPushButton::clicked, this, &ConfiguratorWindow::readSetCurrent);
    connect(ui->pbtnWriteCurrentBlock, &QPushButton::clicked, this, &ConfiguratorWindow::writeSetCurrent);
    connect(ui->pushButtonWriteEditItem, &QPushButton::clicked, this, &ConfiguratorWindow::writeSetEditItem);
    connect(ui->pbtnVersionSoftware, &QPushButton::clicked, this, &ConfiguratorWindow::versionSowftware);
    connect(ui->toolButtonConnectSettings, &QPushButton::clicked, this, &ConfiguratorWindow::serialPortSettings);
    connect(ui->pbtnClearLedOutput, &QPushButton::clicked, this, &ConfiguratorWindow::clearIOTable);
    connect(ui->pbtnClearDiscreteInput, &QPushButton::clicked, this, &ConfiguratorWindow::clearIOTable);
    connect(ui->pbtnClearRelayOutput, &QPushButton::clicked, this, &ConfiguratorWindow::clearIOTable);
    connect(ui->pbtnClearKeyboardProtectionCtrl, &QPushButton::clicked, this, &ConfiguratorWindow::clearIOTable);
    connect(ui->pushButtonJournalRead, &QPushButton::clicked, this, &ConfiguratorWindow::processReadJournals);
    connect(ui->pushButtonJournalClear, &QPushButton::clicked, this, &ConfiguratorWindow::clearJournal);
    connect(this, &ConfiguratorWindow::buttonReadJournalStateChanged, ui->pushButtonJournalRead, &QPushButton::setChecked);
    connect(ui->pbtnMenuExit, &QPushButton::clicked, this, &ConfiguratorWindow::exitFromApp);
    connect(ui->widgetMenuBar, &CMenuBar::closeWindow, this, &ConfiguratorWindow::exitFromApp);
    connect(ui->pbtnMenuExportToPDF, &QPushButton::clicked, this, &ConfiguratorWindow::startCurrentJournalExportToPDF);
    connect(ui->pushButtonExport, &QPushButton::clicked, this, &ConfiguratorWindow::processExport);
    connect(ui->pushButtonImport, &QPushButton::clicked, this, &ConfiguratorWindow::processImport);
    connect(ui->widgetMenuBar, &CMenuBar::exportToDataBaseAction, this, &ConfiguratorWindow::processExport);
    connect(ui->widgetMenuBar, &CMenuBar::importFromDataBaseAction, this, &ConfiguratorWindow::processImport);
//    connect(ui->stwgtMain, &QStackedWidget::currentChanged, this, &ConfiguratorWindow::widgetStackIndexChanged);
    connect(m_timer_synchronization, &QTimer::timeout, this, &ConfiguratorWindow::timeoutSynchronization);
    connect(ui->pbtnFilter, &QPushButton::clicked, this, &ConfiguratorWindow::filterDialog);
//    connect(ui->pushButtonDefaultSettings, &QPushButton::clicked, this, &ConfiguratorWindow::deviceDefaultSettings);
    connect(ui->dateEdit, &QDateEdit::dateChanged, this, &ConfiguratorWindow::dateDeviceChanged);
    connect(m_serialPortSettings_window, &CSerialPortSetting::autospeed, this, &ConfiguratorWindow::autospeedStateChanged);
    connect(ui->splitterCentralWidget, &QSplitter::splitterMoved, this, &ConfiguratorWindow::panelMoved);
    connect(ui->splitterPanelMessage, &QSplitter::splitterMoved, this, &ConfiguratorWindow::panelMoved);
    if(ui->dockWidgetMenuDevice->control())
        connect(ui->dockWidgetMenuDevice->control(), &QPushButton::clicked, this, &ConfiguratorWindow::panelButtonCtrlPress);
    if(ui->dockWidgetVariable->control())
        connect(ui->dockWidgetVariable->control(), &CDockPanelItemCtrl::clicked, this, &ConfiguratorWindow::panelButtonCtrlPress);
    connect(ui->pushButtonPanelMessage, &QPushButton::clicked, this, &ConfiguratorWindow::panelButtonCtrlPress);
    connect(m_monitor_purpose_window, &CMonitorPurpose::buttonUpdate, this, &ConfiguratorWindow::sendMonitorPurposeI11_I17Request);
    connect(m_outputall_window, &COutputAll::buttonUpdate, this, &ConfiguratorWindow::sendOutputAllRequest);
    connect(m_inputs_window, &COutputAll::buttonUpdate, this, &ConfiguratorWindow::sendInputStatesRequest);
    connect(ui->pushButtonSyncDateTime, &QPushButton::clicked, this, &ConfiguratorWindow::synchronizationDateTime);
    connect(m_output_window, &CIndicatorState::buttonUpdate, this, &ConfiguratorWindow::sendOutputAllRequest);
    connect(m_debuginfo_window, &CDebugInfo::readInfo, this, &ConfiguratorWindow::debugInfoCtrl);
    connect(ui->pushButtonDebugInfo, &QPushButton::clicked, this, &ConfiguratorWindow::debugInfoRead);

    connect(m_tim_debug_info, &QTimer::timeout, this, &ConfiguratorWindow::timeoutDebugInfo);
    connect(m_journal_timer, &QTimer::timeout, this, &ConfiguratorWindow::timeoutJournalRead);
    connect(ui->checkBoxTestStyle, &QCheckBox::clicked, this, &ConfiguratorWindow::testStyle);
    connect(m_serialPortSettings_window, &CSerialPortSetting::updateSettings, this, &ConfiguratorWindow::updateSerialPortSettings);
    connect(ui->widgetMenuBar, &CMenuBar::closeWindow, this, &ConfiguratorWindow::applicationCloseProcess);
    connect(ui->widgetMenuBar, &CMenuBar::expandedWindow, this, &ConfiguratorWindow::expandedWindow);
    connect(ui->widgetMenuBar, &CMenuBar::minimizeWindow, this, &ConfiguratorWindow::showMinimized);
    connect(ui->widgetMenuBar, &CMenuBar::menubarMouseUpdatePosition, this, &ConfiguratorWindow::mouseMove);
    connect(ui->pbtnMenuNewProject, &QPushButton::clicked, this, &ConfiguratorWindow::newProject);
    connect(ui->pbtnMenuOpenProject, &QPushButton::clicked, this, &ConfiguratorWindow::openFileProject);
    connect(ui->pbtnMenuSaveProject, &QPushButton::clicked, this, &ConfiguratorWindow::saveProject);
    connect(ui->pbtnMenuSaveAsProject, &QPushButton::clicked, this, &ConfiguratorWindow::saveAsProject);
    connect(ui->widgetMenuBar->widgetMenu(), &CWidgetMenu::closeWindow, this, &ConfiguratorWindow::close);
    connect(ui->widgetMenuBar->widgetMenu(), &CWidgetMenu::newProject, this, &ConfiguratorWindow::newProject);
    connect(ui->widgetMenuBar->widgetMenu(), &CWidgetMenu::openProject, this, &ConfiguratorWindow::openFileProject);
    connect(ui->widgetMenuBar->widgetMenu(), &CWidgetMenu::openExistsProject, this, &ConfiguratorWindow::openExistsProject);
    connect(ui->widgetMenuBar->widgetMenu(), &CWidgetMenu::saveProject, this, &ConfiguratorWindow::saveProject);
    connect(ui->widgetMenuBar->widgetMenu(), &CWidgetMenu::saveAsProject, this, &ConfiguratorWindow::saveAsProject);

    connect(ui->widgetMenuBar->widgetMenu(), &CWidgetMenu::exportProtectionAutomaticToExcel, this, &ConfiguratorWindow::exportToExcelProject);
    connect(ui->widgetMenuBar->widgetMenu(), &CWidgetMenu::importProtectionAutomaticFromExcel, this, &ConfiguratorWindow::importFromExcelProject);
    connect(ui->widgetMenuBar->widgetMenu(), &CWidgetMenu::exportProtectionAutomaticToDatabase, this, &ConfiguratorWindow::exportProtectionAutomaticToDB);
    connect(ui->widgetMenuBar->widgetMenu(), &CWidgetMenu::importProtectionAutomaticFromDatabase, this, &ConfiguratorWindow::importProtectionAutomaticFromDB);

    connect(ui->widgetMenuBar->widgetMenu(), &CWidgetMenu::exportJournalToPDF, this, &ConfiguratorWindow::startMenuJournalExportToPDF);
    connect(ui->widgetMenuBar->widgetMenu(), &CWidgetMenu::exportJournalToDatabase, this, &ConfiguratorWindow::startMenuJournalExportToDB);
    connect(ui->widgetMenuBar->widgetMenu(), &CWidgetMenu::importJournalFromDatabase, this, &ConfiguratorWindow::startMenuJournalImportFromDB);

    connect(ui->widgetMenuBar->widgetMenu(), &CWidgetMenu::exportSettingsToDatabase, this, &ConfiguratorWindow::exportPurposeToDb);
    connect(ui->widgetMenuBar->widgetMenu(), &CWidgetMenu::importSettingsFromDatabase, this, &ConfiguratorWindow::importPurposeFromDb);

    connect(ui->widgetMenuBar->widgetMenu(), &CWidgetMenu::closeProject, this, &ConfiguratorWindow::closeProject);
    connect(ui->widgetMenuBar, &CMenuBar::minimizeMenu, this, &ConfiguratorWindow::minimizeTabMenu);
    connect(ui->widgetMenuBar->widgetMenu(), &CWidgetMenu::settings, this, &ConfiguratorWindow::authorization);
    connect(ui->widgetMenuBar->widgetMenu(), &CWidgetMenu::defaltSet, this, &ConfiguratorWindow::deviceDefaultSettings);

    connect(m_calibration_controller, &CCalibrationController::calibrationSaveToFlash, this, &ConfiguratorWindow::saveDeviceSettings);
    connect(m_calibration_controller, &CCalibrationController::calibration, this, &ConfiguratorWindow::sendRequestCalibration);
    connect(this, &ConfiguratorWindow::calibrationDataIsReady, m_calibration_controller, &CCalibrationController::dataIsReady);
    connect(m_calibration_controller, &CCalibrationController::calibrationFactorAllRead, this, &ConfiguratorWindow::inputAnalogCalibrateRead);
    connect(m_calibration_controller, &CCalibrationController::calibrationWrite, this, &ConfiguratorWindow::sendRequestCalibrationWrite);
    connect(this, &ConfiguratorWindow::calibrationFactorIsReady, m_calibration_controller, &CCalibrationController::calibrationFactorActual);
    connect(m_calibration_controller, &CCalibrationController::calculate, this, &ConfiguratorWindow::sendCalibrationCalculateValues);
    connect(this, &ConfiguratorWindow::calibrationCalculateValue, m_calibration_controller, &CCalibrationController::calculateResponse);

    connect(m_calibration_controller, &CCalibrationController::bruResistanceStateVariable, this, &ConfiguratorWindow::bruResistanceStateVariableSend);
    connect(this, &ConfiguratorWindow::bruResistanceStateVariableRead, m_calibration_controller, &CCalibrationController::bruResistanceStateVariableIsReady);
    connect(m_calibration_controller, &CCalibrationController::bruResistanceSendCommand, this, &ConfiguratorWindow::sendDeviceCommand);

    connect(ui->tabWidgetCalibration, &QTabWidget::currentChanged, this, &ConfiguratorWindow::calibrationTypeChanged);
    connect(ui->tabWidgetCalibrationBRU, &QTabWidget::currentChanged, this, &ConfiguratorWindow::calibrationTypeChanged);

    connect(m_modbus, &CModBus::rawData, m_terminal_modbus, &CTerminal::appendData);
    connect(m_terminal_modbus, &CTerminal::sendDeviceCommand, this, &ConfiguratorWindow::sendDeviceCommand);
    connect(ui->pushButtonCalibrationRoll, &QPushButton::clicked, this, &ConfiguratorWindow::calibrationRoll);
}
