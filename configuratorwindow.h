#ifndef CONFIGURATORWINDOW_H
    #define CONFIGURATORWINDOW_H
    //--------------------
    #include <QMainWindow>
    #include <QSerialPort>
    #include <QSerialPortInfo>
    #include <QMessageBox>
    #include <QDebug>
    #include <QListWidget>
    #include <QTimer>
    #include <QSpinBox>
    #include <QButtonGroup>
    #include <QAbstractButton>
    #include <QFile>
    #include <QTreeWidget>
    #include <QTextStream>
    #include <QTableWidget>
    #include <QFont>
    #include <QSqlDatabase>
    #include <QSqlQuery>
    #include <QSqlError>
    #include <QSqlRecord>
    #include <QTextDocument>
    #include <QTextCursor>
    #include <QJsonObject>
    #include <QJsonDocument>
    #include <QJsonArray>
    #include <QProgressBar>
    #include <QThread>
    #include <QtConcurrent/QtConcurrent>
    #include <QFileDialog>
    #include <QShowEvent>
    #include <QSettings>
    #include <QVBoxLayout>
    #include <QHBoxLayout>
    #include <QKeyEvent>
    #include <cmath>
    #include <algorithm>
    #include "modbus.h"
    #include "menubar.h"
    #include "cterminal.h"
    #include "cindicatorstate.h"
    #include "qcustomplot.h"
    #include "cversionsoftware.h"
    #include "cmatrixpurposemodel.h"
    #include "cserialportsetting.h"
    #include "cstatusbar.h"
    #include "ceventlistjournals.h"
    #include "cprogressbarwidget.h"
    #include "cfilterdialog.h"
    #include "cjournalwidget.h"
    #include "clineedit.h"
    #include "HierarchicalHeaderView.h"
    #include "cvariablewidget.h"
    #include "cdockpanelitemctrl.h"
    #include "cmonitorpurpose.h"
    #include "coutputall.h"
    #include "cdebuginfo.h"
    #include "cstatusinfo.h"
    #include "popup.h"
    #include "tabwidget.h"
    #include "dragwidget.h"
    #include "xlsx/xlsxdocument.h"
    #include "devicemenumodel.h"
    //-------------------
//    #define DEBUG_REQUEST // отладка отправки/приема данных (отключение синхронизации)
//    #define DEBUG_JOURNAL // отладка чтение журналов
//    #define DEBUG_FUNCTION // отладка всего остального
    //-----------------------------------------------------
    const QString ORGANIZATION_NAME   = QObject::tr("РПА");
    const QString ORGANIZATION_DOMAIN = QObject::tr("http://www.rpa.ua/");
    //----------
    namespace Ui 
    {
        class ConfiguratorWindow;
    }
    //----------------------------------------------
    typedef QVector<property_list_t> row_property_t;
    //-------------------------------------------
    class ConfiguratorWindow: public QMainWindow
    {
        Q_OBJECT
        
        public:
            enum RegisterAddress
            {
                CALCULATE_ADDRESS_PART1 = 64,
                CALCULATE_ADDRESS_PART2 = 164
            };
            //--------------
            enum RequestType
            {
                CALCULATE_TYPE, // расчетные данные
                GENERAL_TYPE, // общие (настройки/уставки)
                GENERAL_CONTROL_TYPE, // общие настройки (выбор варианта, т.е. комбобокс) - отдельный запрос
                PURPOSE_OUT_TYPE, // матрца привязок выходов
                PURPOSE_OUT_MEMORY_LED_TYPE, // матрица привязок запоминания выходов светодиодов
                PURPOSE_OUT_MEMORY_RELAY_TYPE, // матрица привязок запоминания выходов реле
                PURPOSE_INPUT_TYPE, // матрица привязок входов
                PURPOSE_INPUT_INVERSE_TYPE, // матрица привязок инверсий входов
                PROTECTION_WORK_MODE_TYPE, // чтение режима работы защит
                MONITONR_PURPOSE_K10_K11_TYPE, // чтение привязок для внутренних переменных К10-К11
                READ_OUTPUT_ALL, // чтение состояний всех выходов
                READ_INPUTS, // чтение состояний входов
                READ_EVENT_JOURNAL, // чтение журнала событий
                READ_EVENT_COUNT, // чтение количества событий в журнале
                READ_EVENT_SHIFT_PTR, // чтение позиции указателя сдвига журнала событий
                READ_SERIAL_NUMBER, // чтение серийного номера
                READ_BLOCK_PROTECTION, // чтение блокировок защит
                READ_DEBUG_INFO, // чтение отладочной информации
                READ_JOURNAL,
                READ_JOURNAL_COUNT,
                READ_JOURNAL_SHIFT_PTR,
                READ_STATUS_MCP_INFO,
                READ_STATUS_MODULE_INFO,
                DATETIME_TYPE,
                PORTECT_RESERVE_SIGNAL_START,
                AUTOMATION_SIGNAL_START,
                COMMUNICATIONS_MODBUS_ADDRESS,
                COMMUNICATIONS_MODBUS_TIM_REQUEST,
                COMMUNICATIONS_MODBUS_TIM_SPEED,
                CALIBRATION_CURRENT_IA, // калибровка токовго коэффициента Ia
                CALIBRATION_CURRENT_IB, // калибровка токовго коэффициента Ib
                CALIBRATION_CURRENT_IC, // калибровка токовго коэффициента Ic
                CALIBRATION_CURRENT_3I0, // калибровка токовго коэффициента 3I0
                AMPLITUDE_READ_CH2, // амплитуда канала №2
                AMPLITUDE_READ_CH3, // амплитуда канала №3
                AMPLITUDE_READ_CH4, // амплитуда канала №4
                AMPLITUDE_READ_CH5, // амплитуда канала №5
            };
            //------------------
            enum RequestFunction
            {
                FUN_READ,
                FUN_SAVE
            };
            //-------------
            enum WidgetType
            {
                INT,
                FLOAT, // QLineEdit (set validator: QDoubleValidator)
                LIST   // QComboBox
            };
            /*!
             * \brief The DeviceMenuItemType enum
             * Описание перечислений пунктов меню устройства
             */
            enum DeviceMenuItemType
            {
                DEVICE_MENU_ITEM_NONE                            = 0,
                DEVICE_MENU_ITEM_PROTECTION_ROOT                 = 1000,
                DEVICE_MENU_ITEM_AUTOMATION_ROOT                 = 2000,
                DEVICE_MENU_ITEM_JOURNALS_ROOT                   = 3000,
                DEVICE_MENU_ITEM_MEASURES_ROOT                   = 4000,
                DEVICE_MENU_ITEM_SETTINGS_ROOT                   = 5000,
                DEVICE_MENU_PROTECT_ITEM_CURRENT                 = 1100,
                DEVICE_MENU_PROTECT_ITEM_POWER                   = 1200,
                DEVICE_MENU_PROTECT_ITEM_DIRECTED                = 1300,
                DEVICE_MENU_PROTECT_ITEM_FREQUENCY               = 1400,
                DEVICE_MENU_PROTECT_ITEM_EXTERNAL                = 1500,
                DEVICE_MENU_PROTECT_ITEM_MOTOR                   = 1600,
                DEVICE_MENU_PROTECT_ITEM_TEMPERATURE             = 1700,
                DEVICE_MENU_PROTECT_ITEM_RESERVE                 = 1800,
                DEVICE_MENU_PROTECT_ITEM_CONTROL                 = 1900,
                DEVICE_MENU_PROTECT_ITEM_CURRENT_MTZ1            = 1121,
                DEVICE_MENU_PROTECT_ITEM_CURRENT_MTZ2            = 1122,
                DEVICE_MENU_PROTECT_ITEM_CURRENT_MTZ3            = 1123,
                DEVICE_MENU_PROTECT_ITEM_CURRENT_MTZ3_SET_CHAR   = 1110, // Настройки характеристик
                DEVICE_MENU_PROTECT_ITEM_CURRENT_MTZ3_PROP_STEEP = 1101, // крутая
                DEVICE_MENU_PROTECT_ITEM_CURRENT_MTZ3_PROP_SLOP  = 1102, // пологая
                DEVICE_MENU_PROTECT_ITEM_CURRENT_MTZ3_PROP_INV   = 1103, // инверсная
                DEVICE_MENU_PROTECT_ITEM_CURRENT_MTZ3_PROP_DINV  = 1104, // длительно инверсная
                DEVICE_MENU_PROTECT_ITEM_CURRENT_MTZ3_PROP_BACK  = 1105, // обратно зависимая
                DEVICE_MENU_PROTECT_ITEM_CURRENT_MTZ3_PROP_SINV  = 1106, // сильно инверсная
                DEVICE_MENU_PROTECT_ITEM_CURRENT_MTZ3_PROP_EINV  = 1107, // экстремально инверсная
                DEVICE_MENU_PROTECT_ITEM_CURRENT_MTZ4            = 1114,
                DEVICE_MENU_PROTECT_ITEM_POWER_UMAX1             = 1201,
                DEVICE_MENU_PROTECT_ITEM_POWER_UMAX2             = 1202,
                DEVICE_MENU_PROTECT_ITEM_POWER_UMIN1             = 1203,
                DEVICE_MENU_PROTECT_ITEM_POWER_UMIN1_COREC_KCU   = 1213, // переход на коррекцию КЦУ (вкладка автоматика/выключатель)
                DEVICE_MENU_PROTECT_ITEM_POWER_UMIN2             = 1204,
                DEVICE_MENU_PROTECT_ITEM_POWER_UMIN2_COREC_KCU   = 1214, // переход на коррекцию КЦУ (вкладка автоматика/выключатель)
                DEVICE_MENU_PROTECT_ITEM_POWER_3U0               = 1205,
                DEVICE_MENU_PROTECT_ITEM_DIRECTED_OZZ1           = 1301,
                DEVICE_MENU_PROTECT_ITEM_DIRECTED_OZZ2           = 1302,
                DEVICE_MENU_PROTECT_ITEM_DIRECTED_NZZ1           = 1303,
                DEVICE_MENU_PROTECT_ITEM_DIRECTED_NZZ2           = 1304,
                DEVICE_MENU_PROTECT_ITEM_FREQUENCY_ACHR1         = 1401,
                DEVICE_MENU_PROTECT_ITEM_FREQUENCY_ACHR2         = 1402,
                DEVICE_MENU_PROTECT_ITEM_FREQUENCY_ACHR3         = 1403,
                DEVICE_MENU_PROTECT_ITEM_EXTERNAL_ARC            = 1501,
                DEVICE_MENU_PROTECT_ITEM_EXTERNAL_EXT1           = 1502,
                DEVICE_MENU_PROTECT_ITEM_EXTERNAL_EXT2           = 1503,
                DEVICE_MENU_PROTECT_ITEM_EXTERNAL_EXT3           = 1504,
                DEVICE_MENU_PROTECT_ITEM_MOTOR_STARTING          = 1601,
                DEVICE_MENU_PROTECT_ITEM_MOTOR_IMIN              = 1602,
                DEVICE_MENU_PROTECT_ITEM_TEMPERATURE_TEMP1       = 1701,
                DEVICE_MENU_PROTECT_ITEM_TEMPERATURE_TEMP2       = 1702,
                DEVICE_MENU_PROTECT_ITEM_RESERVE_LEVEL1          = 1801,
                DEVICE_MENU_PROTECT_ITEM_RESERVE_LEVEL2          = 1802,
                DEVICE_MENU_PROTECT_ITEM_RESERVE_SIG_START       = 1803,
                DEVICE_MENU_PROTECT_ITEM_CONTROL_BRU             = 1901,
                DEVICE_MENU_PROTECT_ITEM_CONTROL_VACUUM          = 1902,
                DEVICE_MENU_ITEM_AUTOMATION_SWITCH               = 2111,
                DEVICE_MENU_ITEM_AUTOMATION_SWITCH_TRUCK         = 2221,
                DEVICE_MENU_ITEM_AUTOMATION_BLOCKS               = 2331,
                DEVICE_MENU_ITEM_AUTOMATION_DISCONNECTORS        = 2440,
                DEVICE_MENU_ITEM_AUTOMATION_DISCONNECTORS_BUS    = 2441,
                DEVICE_MENU_ITEM_AUTOMATION_DISCONNECTORS_LINE   = 2442,
                DEVICE_MENU_ITEM_AUTOMATION_DISCONNECTORS_EARTH  = 2443,
                DEVICE_MENU_ITEM_AUTOMATION_CTRL_TN              = 2551,
                DEVICE_MENU_ITEM_AUTOMATION_AVR                  = 2661,
                DEVICE_MENU_ITEM_AUTOMATION_APV                  = 2770,
                DEVICE_MENU_ITEM_AUTOMATION_APV_SIGNAL_START     = 2771,
                DEVICE_MENU_ITEM_JOURNALS_CRASHES                = 3011,
                DEVICE_MENU_ITEM_JOURNALS_EVENTS                 = 3012,
                DEVICE_MENU_ITEM_JOURNALS_HALF_HOURS             = 3013,
                DEVICE_MENU_ITEM_JOURNALS_ISOLATION              = 3014,
                DEVICE_MENU_ITEM_MEASURES_INPUTS                 = 4011,
                DEVICE_MENU_ITEM_SETTINGS_ITEM_IN_ANALOG         = 5100,
                DEVICE_MENU_ITEM_SETTINGS_ITEM_COMMUNICATIONS    = 5200,
                DEVICE_MENU_ITEM_SETTINGS_ITEM_DATETIME          = 5300,
                DEVICE_MENU_ITEM_SETTINGS_ITEM_KEYBOARD          = 5400,
                DEVICE_MENU_ITEM_SETTINGS_ITEM_LEDS              = 5500,
                DEVICE_MENU_ITEM_SETTINGS_ITEM_IO                = 5600,
                DEVICE_MENU_ITEM_SETTINGS_ITEM_IN_ANALOG_GENERAL = 5111,
                DEVICE_MENU_ITEM_SETTINGS_ITEM_IN_ANALOG_CALIB   = 5112,
                DEVICE_MENU_ITEM_SETTINGS_ITEM_IO_MDVV01         = 5610,
                DEVICE_MENU_ITEM_SETTINGS_ITEM_IO_MDVV02         = 5620,
                DEVICE_MENU_ITEM_SETTINGS_ITEM_IO_MDVV01_RELAY   = 5611,
                DEVICE_MENU_ITEM_SETTINGS_ITEM_IO_MDVV01_INPUTS  = 5612,
                DEVICE_MENU_ITEM_SETTINGS_ITEM_IO_MDVV02_RELAY   = 5021,
                DEVICE_MENU_ITEM_SETTINGS_ITEM_IO_MDVV02_INPUTS  = 5022,
                DEVICE_MENU_ITEM_SETTINGS_ITEM_IO_PROTECTION     = 5023
            };
            //---------------
            enum TabMenuIndex
            {
                TAB_FILE_INDEX = 0,
                TAB_IMPORT_EXPORT_INDEX,
                TAB_VIEW_INDEX,
                TAB_SCREEN_INDEX,
                TAB_SET_INDEX,
                TAB_READ_WRITE_INDEX,
                TAB_FILTER_INDEX,
                TAB_HELP_INDEX
            };
            /*!
             * \brief variable_bit_t
             *
             * Список положений переменных в битовом массиве
             */
            typedef QMap<QString, int> variable_bit_t;
            /*!
             * \brief device_menu_item_key_t
             * Карта ключей QMap<номер пункта меню, номер в стеке виджетов>
             */
            typedef QMap<DeviceMenuItemType, int> device_menu_item_key_t;
            /*!
             * \brief The journal_address_t struct
             *
             *  Структура адресов журнала (используется при чтении)
             */
            struct journal_address_t
            {
                long msg_count;  // адрес чтения количества сообщений
                long set_shift;  // адрес установки указателя свдига (установка текущего окна для чтения)
                long start_page; // начальный адрес страницы
            };
            /*!
             * \brief The journal_message_t struct
             *
             *  Структура опсывающая сообщения (параметры)
             */
            struct journal_message_t
            {
                long read_number;  // количество сообщений читаемых в одном запросе
                long read_count;   // счетчик прочитанных сообщений
                long read_limit;   // сообщение до которого читаем (по умолчанию последнее, но изменяется фильром)
                long read_total;   // всего сообщений доступных для чтения
                long read_current; // текущее читаемое сообщение
                long read_start;   // сообщение с которого начинается чтение
                long size;         // размер одного сообщения
            };
            /*!
             * \brief The journal_set struct
             *
             *  Структура описывающая установки чтения журнала
             */
            struct journal_set_t
            {
                long              shift_ptr; // текущее положение указателя сдвига
                long              msg_part;
                bool              isStart;   // чтение первого сообщения
                bool              isStop;    // останвка чтения сообщений
                journal_address_t address;
                journal_message_t message; 
                QVector<quint16>  buffer;    // буфер сообщений
            };
            /*!
             * \brief The protection_t struct
             *
             * Структура определяющая тип защиты
             */
            struct protection_group_t
            {
                int     code;
                QString var_name;
                QString name;
            };
            /*!
             * \brief The cell_t struct
             *
             * Структура определяющая ячейку редактируемой величины
             */
            struct cell_t
            {
                int     addr;
                QString name;
                float   limit_min;
                float   limit_max;
                QString unit_measure;
                QString date_type;
                int     row;
            };
            /*!
             * \brief The block_protection_purpose_t struct
             *
             * Структура описывающая привязку блокировки защиты (таблица управления защитами)
             */
            struct block_protection_purpose_t
            {
                QString key;
                int     bit;
            };
            /*!
             * \brief The block_protection_t struct
             *
             * Структура описывающая блокировку защит (таблица управления защитами)
             */
            struct block_protection_t
            {
                int     id;
                QString key;
                int     address;
                QString name;
                QString description;
                QVector<block_protection_purpose_t> purpose;
            };
            /*!
             * \brief The calibration_current_t struct
             *
             * Структура описывающая переменные для выполнения калибровок по току
             */
            struct calibration_current_t
            {
                int                      request_all; // всего запросов
                int                      request_count; // количество отправленных запросов
                int                      pause; // пауза между запросами в мс
                QVector<CModBusDataUnit> units; // массив запросов
                QTimer*                  timer; // таймер отправки запросов
            };
            //------------------------------------------------------------------------------------------
            //--------------------key, address, description, list variables purpose---------------------
            typedef QVector<QPair<QString, QPair<int, QPair<QString, QVector<QString> > > > > purpose_t;
            //-------------key, addres, widget name--------------
            typedef QVector<QPair<QString, cell_t> > cell_list_t;
            //--------key, <addr, name, limit_min, limit_max, unit_measure>--------
            typedef QMap<QString, cell_t> limit_unit_t;
            //--------------------key <protection>----------------------
            typedef QMap<QString, protection_group_t> protection_list_t;
            //----------------------------------------------------------
            typedef QVector<block_protection_t> block_protection_list_t;

        public:
            explicit ConfiguratorWindow(QWidget* parent = Q_NULLPTR);
            ~ConfiguratorWindow();
        
        public slots:
            void serialPortCtrl();
            void stateChanged(bool state);
            void refreshSerialPort();
            void serialPortSettings();
            void blockProtectionCtrlRead(); // чтение состояний блокировок защит (таблица "Управление блокировками"
            void blockProtectionCtrlWrite(); // запись состояний блокировок защит (таблица "Управление блокировками"
            void calculateRead(); // запрос расчетных величин
            void debugInfoRead(); // чтение отладочной информации
            void journalRead(const QString& key);
            void inputAnalogGeneralRead();
            void inputAnalogCalibrateRead();
            void inputAnalogGroupRead();
            void protectionMTZ1Read();
            void protectionMTZ2Read();
            void protectionMTZ3Read();
            void protectionMTZ3SetCharRead();
            void protectionMTZ3PropertySteepRead();
            void protectionMTZ3PropertySlopRead();
            void protectionMTZ3PropertyInversionRead();
            void protectionMTZ3PropertyDInversionRead();
            void protectionMTZ3PropertyBackRead();
            void protectionMTZ3PropertyStrongInversionRead();
            void protectionMTZ3PropertyExtremalInversionRead();
            void protectionMTZ4Read();
            void protectionMTZGroupRead();
            void protectionUmax1Read();
            void protectionUmax2Read();
            void protectionUmin1Read();
            void protectionUmin2Read();
            void protection3U0Read();
            void protectionPowerGroupRead();
            void protectionOZZ1Read();
            void protectionOZZ2Read();
            void protectionNZZ1Read();
            void protectionNZZ2Read();
            void protectionDirectedGroupRead();
            void protectionAchr1Read();
            void protectionAchr2Read();
            void protectionAchr3Read();
            void protectionFrequencyGroupRead();
            void protectionArcRead();
            void protectionExt1Read();
            void protectionExt2Read();
            void protectionExt3Read();
            void protectionExternalGroupRead();
            void protectionStartingRead();
            void protectionIminRead();
            void protectionMotorGroupRead();
            void protectionTemp1Read();
            void protectionTemp2Read();
            void protectionTemperatureGroupRead();
            void protectionLevel1Read();
            void protectionLevel2Read();
            void protectionSignalStartRead();
            void protectionReserveGroupRead();
            void protectionBRURead();
            void protectionVacuumRead();
            void protectionControlGroupRead();
            void amplitudeReadOfCurrent();
            void automationSwitchRead();
            void automationSwitchTruckRead();
            void automationBlockRead();
            void automationBusRead();
            void automationLineRead();
            void automationEarthRead();
            void automationDisconnectorsGroupRead();
            void automationCtrlTNRead();
            void automationAVRRead();
            void automationAPVSignalStartRead();
            void automationAPVRead();
            void automationGroupRead();
            void automationGroupWrite();
            void calibrationOfCurrentWrite();
            void purposeLedsRead();
            void purposeInputRead();
            void purposeRelayRead();
            void purposeMemoryOutLedRead();
            void purposeMemoryOutRelayRead();
            void dateTimeRead();
            void settingCommunicationsRead();
            void inputAnalogGeneralWrite();
            void inputAnalogCalibrateWrite();
            void inputAnalogGroupWrite();
            void protectionMTZ1Write();
            void protectionMTZ2Write();
            void protectionMTZ3Write();
            void protectionMTZ3SetCharWrite();
            void protectionMTZ3PropertySteepWrite();
            void protectionMTZ3PropertySlopWrite();
            void protectionMTZ3PropertyInversionWrite();
            void protectionMTZ3PropertyDInversionWrite();
            void protectionMTZ3PropertyBackWrite();
            void protectionMTZ3PropertyStrongInversionWrite();
            void protectionMTZ3PropertyExtremalInversionWrite();
            void protectionMTZ4Write();
            void protectionMTZGroupWrite();
            void protectionUmax1Write();
            void protectionUmax2Write();
            void protectionUmin1Write();
            void protectionUmin2Write();
            void protection3U0Write();
            void protectionPowerGroupWrite();
            void protectionOZZ1Write();
            void protectionOZZ2Write();
            void protectionNZZ1Write();
            void protectionNZZ2Write();
            void protectionDirectedGroupWrite();
            void protectionAchr1Write();
            void protectionAchr2Write();
            void protectionAchr3Write();
            void protectionFrequencyGroupWrite();
            void protectionArcWrite();
            void protectionExt1Write();
            void protectionExt2Write();
            void protectionExt3Write();
            void protectionExternalGroupWrite();
            void protectionStartingWrite();
            void protectionIminWrite();
            void protectionMotorGroupWrite();
            void protectionTemp1Write();
            void protectionTemp2Write();
            void protectionTemperatureGroupWrite();
            void protectionLevel1Write();
            void protectionLevel2Write();
            void protectionSignalStartWrite();
            void protectionReserveGroupWrite();
            void protectionBRUWrite();
            void protectionVacuumWrite();
            void protectionControlGroupWrite();
            void automationSwitchWrite();
            void automationSwitchTruckWrite();
            void automationBlockWrite();
            void automationBusWrite();
            void automationLineWrite();
            void automationEarthWrite();
            void automationDisconnectorsGroupWrite();
            void automationCtrlTNWrite();
            void automationAVRWrite();
            void automationAPVWrite();
            void automationAPVSignalStartWrite();
            void purposeLedsWrite();
            void purposeInputWrite();
            void purposeRelayWrite();
            void purposeMemoryOutLedWrite();
            void purposeMemoryOutRelayWrite();
            void dateTimeWrite(const QDateTime& dateTime = QDateTime());
            void synchronizationDateTime();
            void settingCommunicationsWrite();
            void processReadJournals(bool state);
            void processExport();
            void processImport();
            void readyReadData(CModBusDataUnit& unit);
            void exitFromApp();
            void show();
            void resizeEvent(QResizeEvent* event);
            void chboxCalculateTimeoutStateChanged(bool state);
            void timeCalculateChanged(int newTime);
            void errorDevice(const QString& errorConnect);
            void errorConnect(const QString& errorConnect);
            void terminalVisiblity(int state);
            void indicatorVisiblity(bool state);
            void monitorK10K11Visiblity(bool state);
            void outputAllVisiblity(bool state);
            void inputVisiblity(bool state);
            void debugInfoVisiblity(bool state);
            void statusInfoVisiblity(bool state);
            void itemClicked(QTreeWidgetItem* item, int);
            void readSettings();
            void readSetCurrent();
            void writeSettings();
            void writeSetCurrent();
            void expandItemTree(bool state);
            void versionSowftware();
            void sendCalculateRead(CModBusDataUnit& unit);
            void sendSettingReadRequest(const QString& first, const QString& last,
                                        CModBusDataUnit::FunctionType type, int size, DeviceMenuItemType index);
            void sendSettingControlReadRequest(const QString& index, DeviceMenuItemType group_index);
            void sendSettingControlWriteRequest(const QString& index, DeviceMenuItemType group_index);
            void sendSettingWriteRequest(const QString& first, const QString& last, DeviceMenuItemType group_index);
            void sendPurposeReadRequest(const QString& first, const QString& last);
            void sendPurposeWriteRequest(const QString& first, const QString& last);
            void sendPurposeDIReadRequest(int first_addr, int last_addr);
            void sendPurposeDIWriteRequest(int first_addr, int last_addr);
            void sendPurposeInverseDIWriteRequest(int first_addr, int last_addr);
            void sendProtectionWorkModeRequest(const QString& protection, RequestFunction function,
                                               DeviceMenuItemType group_index);
            void sendMonitorPurposeK10_K11Request();
            void sendRequestRead(int addr, int size, int request,
                                 CModBusDataUnit::FunctionType functionType = CModBusDataUnit::ReadHoldingRegisters);
            void sendRequestWrite(int addr, QVector<quint16>& values, int request);
            void sendDeviceCommand(int cmd);
            void sendOutputAllRequest();
            void sendInputStatesRequest();
            void sendDebugInfoRead(int channel);
            void clearIOTable();
            void clearJournal();
            void menuPanelCtrl();
            void variablePanelCtrl();
            void exportToPDF(const CJournalWidget* widget, const QString& reportName, const QString& sn_device,
                                                                                      const QString& filename);
            void exportPurposeToJSON();
            void importPurposeFromJSON();
            void processReadJournal(CModBusDataUnit& unit);
            void widgetStackIndexChanged(int);
            void setJournalPtrShift(const QString& key, long pos);
            void timeoutSynchronization();
            void timeoutDebugInfo();
            void debugInfoCtrl(int timer, bool state = false);
            void importJournalToTable();
            void exportJournalToDb();
            void startExportToPDF();
            void filterDialog();
            void deviceDefaultSettings();
            void noConnectMessage();
            void setNewBaudrate(int baudrate_index);
            void saveDeviceSettings();
            void dateDeviceChanged(const QDate& date);
            void autospeedStateChanged(bool state);
            void panelMoved(int pos, int index);
            void panelButtonCtrlPress();
            void filterJournal(const CFilter& filter);
            void stopProgressbar();
            void timeoutJournalRead();
            void testStyle(bool state);
            void readStatusInfo();
            void updateSerialPortSettings();
            void setNewAddress(); // отправка команды на смену адреса предварительно установленного (срабатывает по таймеру);
            void expandedWindow(); // управление разворачиванием окна
            void mouseMove(QPoint pos);
            void newProject();
            void openProject();
            void saveProject();
            void saveAsProject();
            void exportToPDFProject();
            void exportToExcelProject();
            void importFromExcelProject();
            void closeProject();
            void minimizeTabMenu(bool state);
            void panelMessageVisiblity(bool state = false);
            void calibrationOfCurrent();
            void calibrationOfCurrentRequest();
            void resizeColumns();
            void processKCUUmin();

        protected:
            void keyPressEvent(QKeyEvent* event);

        private:
            bool createJournalTable(QSqlDatabase* db, const QString& journal_type);
            bool currentJournal(const CJournalWidget*& widget);
            void loadSettings();
            void saveSettings();
            void initConnect();
            void initMenuPanel();
            void initCellBind();
            void initPurposeBind();
            void initModelTables();
            void initEventJournal();
            void initCrashJournal();
            void initHalfhourJournal();
            void initDeviceCode();
            void initJournals();
            void initProtectionList();
            void initMonitorPurpose();
            void initOutputAll();
            void initInputs();
            void initDebugInfo();
            void initWordStatus();
            void connectSystemDb();
            bool connectDb(QSqlDatabase*& db, const QString& path);
            void disconnectDb(QSqlDatabase* db);
            void initTable(QTableView* table, QVector<QPair<QString, QString> >& column_labels, group_t& group_rows);
            void initTableProtection(QTableView* table, block_protection_list_t& labels);
            void initIndicatorStates();
            void displayCalculateValues(QVector<quint16> values);
            void displayDateTime(CModBusDataUnit& unit);
            void displaySettingResponse(CModBusDataUnit& unit);
            void displaySettingControlResponce(const CModBusDataUnit& unit);
            void displayPurposeOutput(CModBusDataUnit& unit);
            void displayPurposeDIResponse(const QVector<quint16>& input_list, const QVector<quint16>& input_inverse_list);
            void displayJournalResponse(QVector<quint16>& data);
            void displayDeviceSerialNumber(const QVector<quint16>& data);
            void displayProtectReserveSignalStart(const QVector<quint16>& data);
            void displayAutomationAPVSignalStart(const QVector<quint16>& data);
            void displayCommunicationTimeoutRequest(const QVector<quint16>& data);
            void displayCommunicationTimeoutSpeed(const QVector<quint16>& data);
            void displayCommunicationAddress(const QVector<quint16>& data);
            void displayProtectionWorkMode(CModBusDataUnit& unit);
            void displayMonitorK10_K11(CModBusDataUnit& unit);
            void displayOutputAllRead(CModBusDataUnit& unit);
            void displayInputsRead(const QVector<quint16>& data);
            void displayBlockProtectionRead(const QVector<quint16>& data);
            void displayDebugInfo(const CModBusDataUnit& unit);
            void displayStatusInfo(const CModBusDataUnit& unit);
            void displayMemoryOut(const CModBusDataUnit::vlist_t& values);
            void displayCalibrationOfCurrent();
            void versionParser();
            int  sizeBlockSetting(const QString& first, const QString& last);
            int  addressSettingKey(const QString& key) const;
            int  addressPurposeKey(const QString& key) const;
            void readShiftPrtEventJournal();
            void readJournalCount();
            void synchronization(bool state = false);
            void setLineEditValidator(QObject* object);
            void panelVisibleCtrl(QWidget* widget);
            int  recordCountDb(QSqlDatabase* db, const QString& table_name, const QString& parameter, const QString& value,
                                                 const QString& subparamter = "", const QStringList& range = QStringList());
            QString             recordLastDb(QSqlDatabase* db, const QString& table_name, const QString& parameter);
            QPoint              indexDateFilter(QTableWidget* table, const QDate& begin, const QDate& end);
            QPoint              indexSettingKey(const QString& first, const QString& last);
            QPoint              indexPurposeKey(const QString& first, const QString& last);
            QVector<int>        indexVariableFromKey(const QStringList& variables, const QString& key);
            QTableView*         tableMatrixFromKeys(const QString& first, const QString& last);
            DeviceMenuItemType  menuIndex();
            QDateTime           unpackDateTime(QVector<quint8>& data);
            void                convertDataHalfwordToBytes(const QVector<quint16>& source, QVector<quint8>& dest);
            group_t             createVariableGroup(const QString& io_key);
            QVector<QPair<QString, QString> > loadLabelColumns(const QString& type);
            QVector<QString>    loadVaribleByType(const QString& type);
            block_protection_list_t loadProtectionList();
            bool deleteLogFile();
            bool showErrorMessage(const QString& title, CModBusDataUnit& unit);
            void endJournalReadProcess(const QString& text);
            float newCalibrationOfCurrentFactor(float standard, float cur_factor, QVector<float>& measure_list);
            QPointF standardDeviation(QVector<float>& list);
            int writeDataToExcel(QXlsx::Document& doc, const CDeviceMenuTableWidget* table, int offset = 0);
            int readDataFromExcel(QXlsx::Document& doc, const CDeviceMenuTableWidget* table, int offset);
            int groupPositionInExcel(QXlsx::Document& doc, const QString& group);
            CDeviceMenuTableWidget::group_t loadMenuGroup(const QString& group_name);
            CDeviceMenuTableWidget::group_t loadMenuSubgroup(const QString& group_name);
            CDeviceMenuTableWidget::item_t loadIODeviceItem(const QString& k);
            CDeviceMenuTableWidget* groupMenuWidget(DeviceMenuItemType type) const;
            QWidget* groupMenuCellWidgetByName(CDeviceMenuTableWidget* table, const QString& wgt_name, int col) const;
            QWidget* groupMenuCellWidget(const CDeviceMenuTableWidget* table, int row, int col);
            int groupMenuPosition(const QString& name, const CDeviceMenuTableWidget* table);

        signals:
            void buttonReadJournalStateChanged(bool = false);

        private:
            Ui::ConfiguratorWindow*          ui;
            CModBus*                         m_modbus;
            CSerialPortSetting*              m_serialPortSettings_window;
            CDragWidget*                     m_terminal_window;
            CIndicatorState*                 m_output_window;
            CMonitorPurpose*                 m_monitor_purpose_window;
            COutputAll*                      m_outputall_window;
            COutputAll*                      m_inputs_window; // состояние входов
            CDebugInfo*                      m_debuginfo_window;
            CStatusInfo*                     m_status_window;
            PopUp*                           m_popup;
            QTimer*                          m_tim_calculate;
            QTimer*                          m_tim_debug_info;
            CVersionSoftware*                m_version_window;
            QSqlDatabase                     m_system_db;
            cell_list_t                      m_cell_list;
            purpose_t                        m_purpose_list;
            QTime                            m_time_process;
            QTime                            m_time_process_speed;
            QTimer*                          m_timer_synchronization;
            QTimer*                          m_timer_new_address_set;
            CStatusBar*                      m_status_bar;
            QMap<int, QString>               m_device_code_list;
            QFutureWatcher<void>*            m_watcher;
            CProgressBarWidget*              m_progressbar;
            QSettings*                       m_settings;
            QMap<QString, CFilter>           m_filter;
            const CJournalWidget*            m_active_journal_current; // текущий активный журнал
            const CJournalWidget*            m_journal_read_current; // текущий журнал чтения, т.е. журнал, который читают из устройства
            QMap<QString, journal_set_t>     m_journal_set; // установки журналов
            device_menu_item_key_t           m_menu_items; // карта пунктов меню устройства
            QVector<quint16>                 m_calculate_buffer; // буфер расчетных величи (два запроса, поэтому необходимо клеить)
            variable_bit_t                   m_variable_bits;
            protection_list_t                m_protections; // карта защит
            limit_unit_t                     m_limits; // лимиты редактируемых величин
            block_protection_list_t          m_block_list; // список блокировок для таблицы Управление защитами
            QTimer*                          m_journal_timer; // проверка на обрыв чтения журнала
            calibration_current_t            m_calib_of_current; // структура для калибровок по току
    };
    // Регистрация пользовательских типов
    Q_DECLARE_METATYPE(row_property_t)
#endif // CONFIGURATORWINDOW_H
