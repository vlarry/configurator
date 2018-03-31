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
    #include "cmodbus.h"
    #include "qpanel.h"
    #include "cterminal.h"
    #include "qcustomplot.h"
    #include "cversionsoftware.h"
    #include "cmatrixpurposemodel.h"
    #include "cserialportsetting.h"
    #include "cstatusbar.h"
    #include "ceventlistjournals.h"
    #include "cprogressbarwidget.h"
    #include "cfilterdialog.h"
    #include "cjournalwidget.h"
    //-------------------
//    #define DEBUG_REQUEST // отладка отправки/приема данных (отключение синхронизации)
    //-----------------------------------------------------
    const QString ORGANIZATION_NAME   = QObject::tr("РПА");
    const QString ORGANIZATION_DOMAIN = QObject::tr("http://www.rpa.ua/");
    //----------
    namespace Ui 
    {
        class ConfiguratorWindow;
    }
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
                PURPOSE_INPUT_TYPE, // матрица привязок входов
                READ_EVENT_JOURNAL, // чтение журнала событий
                READ_EVENT_COUNT, // чтение количества событий в журнале
                READ_EVENT_SHIFT_PTR, // чтение позиции указателя сдвига журнала событий
                READ_SERIAL_NUMBER, // чтение серийного номера
                READ_JOURNAL,
                READ_JOURNAL_COUNT,
                READ_JOURNAL_SHIFT_PTR,
                DATETIME_TYPE
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
                DEVICE_MENU_PROTECT_ITEM_CURRENT_MTZ1            = 1111,
                DEVICE_MENU_PROTECT_ITEM_CURRENT_MTZ2            = 1112,
                DEVICE_MENU_PROTECT_ITEM_CURRENT_MTZ3            = 1113,
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
                DEVICE_MENU_PROTECT_ITEM_POWER_UMIN2             = 1204,
                DEVICE_MENU_PROTECT_ITEM_POWER_3UO               = 1205,
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
                DEVICE_MENU_ITEM_SETTINGS_ITEM_IO_MDVV02_INPUTS  = 5022
            };
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
            //------------------------------------------------------------------------------------------
            //--------------------key, address, description, list variables purpose---------------------
            typedef QVector<QPair<QString, QPair<int, QPair<QString, QVector<QString> > > > > purpose_t;
            //-------------------key, addres, widget name-----------------
            typedef QVector<QPair<QString, QPair<int, QString> > > cell_t;

        public:
            explicit ConfiguratorWindow(QWidget* parent = Q_NULLPTR);
            ~ConfiguratorWindow();
        
        public slots:
            void serialPortCtrl();
            void stateChanged(bool state);
            void refreshSerialPort();
            void serialPortSettings();
            void calculateRead(); // запрос расчетных величин
            void journalRead(const QString& key);
            void inputAnalogGeneralRead();
            void inputAnalogCalibrateRead();
            void inputAnalogGroupRead();
            void protectionMTZ1Read();
            void protectionMTZ2Read();
            void protectionMTZ3Read();
            void protectionMTZ4Read();
            void protectionMTZGroupRead();
            void protectionMTZSetWrite();
            void protectionUmax1Read();
            void protectionUmax2Read();
            void protectionUmin1Read();
            void protectionUmin2Read();
            void protection3UORead();
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
            void automationSwitchRead();
            void automationSwitchTruckRead();
            void automationBlockRead();
            void automationBusRead();
            void automationLineRead();
            void automationEarthRead();
            void automationDisconnectorsGroupRead();
            void automationCtrlTNRead();
            void automationAVRRead();
            void automationAPVRead();
            void purposeLedsRead();
            void purposeInputRead();
            void purposeRelayRead();
            void dateTimeRead();
            void inputAnalogGeneralWrite();
            void inputAnalogCalibrateWrite();
            void inputAnalogGroupWrite();
            void protectionMTZ1Write();
            void protectionMTZ2Write();
            void protectionMTZ3Write();
            void protectionMTZ4Write();
            void protectionMTZGroupWrite();
            void protectionUmax1Write();
            void protectionUmax2Write();
            void protectionUmin1Write();
            void protectionUmin2Write();
            void protection3UOWrite();
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
            void purposeLedsWrite();
            void purposeInputWrite();
            void purposeRelayWrite();
            void dateTimeWrite();

            void protectionEarthySetWrite();
            void protectionPowerSetWrite();
            void protectionMotorSetWrite();
            void protectionFrequencySetWrite();
            void protectionExternalSetWrite();
            void protectionTemperatureSetWrite();
            void protectionLevelSetWrite();
            void protectionBruSetWrite();
            void protectionVacuumSetWrite();
            void processReadJournals(bool state);
            void processExport();
            void processImport();
            void automationSetRead();
            void automationSetWrite();
            void switchDeviceSetRead();
            void switchDeviceSetWrite();
            void responseRead(CDataUnitType& unit);
            void exitFromApp();
            void show();
            void resizeEvent(QResizeEvent* event);
            void chboxCalculateTimeoutStateChanged(bool state);
            void timeCalculateChanged(int newTime);
            void timeoutValueChanged(int newTime);
            void numberRepeatChanged(int number);
            void errorDevice(const QString& errorConnect);
            void errorConnect(const QString& errorConnect);
            void terminalVisiblity(int state);
            void saveLog(const QString& info);
            void itemClicked(QTreeWidgetItem* item, int);
            void readSettings();
            void readSetCurrent();
            void writeSettings();
            void writeSetCurrent();
            void expandItemTree(bool state);
            void versionSowftware();
            void sendCalculateRead(CDataUnitType& unit);
            void sendSettingReadRequest(const QString& first, const QString& last,
                                        CDataUnitType::FunctionType type, int size);
            void sendSettingControlReadRequest(const QString& index);
            void sendSettingControlWriteRequest(const QString& index);
            void sendSettingWriteRequest(const QString& first, const QString& last);
            void sendPurposeReadRequest(const QString& first, const QString& last);
            void sendPurposeWriteRequest(const QString& first, const QString& last);
            void sendPurposeDIReadRequest(int first_addr, int last_addr);
            void sendPurposeDIWriteRequest(int first_addr, int last_addr);
            void sendDeviceCommand(int cmd);
            void clearIOTable();
            void clearJournal();
            void menuPanelCtrl();
            void variablePanelCtrl();
            void exportToPDF(const CJournalWidget* widget, const QString& reportName, const QString& sn_device,
                                                                                      const QString& filename);
            void exportPurposeToJSON();
            void importPurposeFromJSON();
            void processReadJournal(CDataUnitType& unit);
            void widgetStackIndexChanged(int);
            void setJournalPtrShift(const QString& key, long pos);
            void timeoutSynchronization();
            void importJournalToTable();
            void exportJournalToDb();
            void startExportToPDF();
            void filterDialog();
            void deviceDefaultSettings();
            void noConnectMessage();
            void setNewBaudrate(int baudrate_index);
            void saveDeviceSettings();
            
        private:
            bool createJournalTable(QSqlDatabase* db, const QString& journal_type);
            bool currentJournal(const CJournalWidget*& widget);
            void loadSettings();
            void saveSattings();
            void initConnect();
            void initMenuPanel();
            void initCellBind();
            void initPurposeBind();
            void initModelTables();
            void initEventJournal();
            void initCrashJournal();
            void initDeviceCode();
            void initJournals();
            void connectSystemDb();
            bool connectDb(QSqlDatabase*& db, const QString& path);
            void disconnectDb(QSqlDatabase* db);
            void initTable(QTableView* table, CDataTable& data);
            void displayCalculateValues(QVector<quint16> values);
            void displayDateTime(CDataUnitType& unit);
            void displaySettingResponse(CDataUnitType& unit);
            void displaySettingControlResponce(const CDataUnitType& unit);
            void displayPurposeResponse(CDataUnitType& unit);
            void displayPurposeDIResponse(CDataUnitType& unit);
            void displayEventJournalResponse(QVector<quint16>& data);
            void displayCrashJournalResponse(const QVector<quint16>& data);
            void displayDeviceSerialNumber(const QVector<quint16>& data);
            void versionParser();
            int  sizeBlockSetting(const QString& first, const QString& last);
            int  addressSettingKey(const QString& key) const;
            int  addressPurposeKey(const QString& key) const;
            void readShiftPrtEventJournal();
            void readJournalCount();
            void synchronization(bool state = false);
            int  recordCountDb(QSqlDatabase* db, const QString& table_name, const QString& parameter, const QString& value,
                                                 const QString& subparamter = "", const QStringList& range = QStringList());
            QString             recordLastDb(QSqlDatabase* db, const QString& table_name, const QString& parameter);
            QPoint              indexDateFilter(QTableWidget* table, const QDate& begin, const QDate& end);
            QPoint              indexSettingKey(const QString& first, const QString& last);
            QPoint              indexPurposeKey(const QString& first, const QString& last);
            QVector<int>        indexVariableFromKey(const QStringList& variables, const QString& key);
            QTableView*         tableMatrixFromKeys(const QString& first, const QString& last);
            CColumn::column_t   columnFromKey(const QString& key);
            int                 indexColumnFromKey(const QString& key);
            DeviceMenuItemType  menuIndex();
            QDateTime           unpackDateTime(QVector<quint8>& data);
            void                convertDataHalfwordToBytes(const QVector<quint16>& source, QVector<quint8>& dest);

        signals:
            void buttonReadJournalStateChanged(bool = false);

        private:
            Ui::ConfiguratorWindow*          ui;
            CModbus*                         m_modbusDevice;
            CSerialPortSetting*              m_serialPortSettings;
            QPanel*                          m_calculateWidget;
            CTerminal*                       m_terminal;
            QFile*                           m_logFile;
            QTimer*                          m_tim_calculate;
            CVersionSoftware*                m_versionWidget;
            QSqlDatabase                     m_system_db;
            cell_t                           m_cell_list;
            purpose_t                        m_purpose_list;
            QVector<CColumn::column_t>       m_variables;
            QTime                            m_time_process;
            QTimer*                          m_timer_synchronization;
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
    };
#endif // CONFIGURATORWINDOW_H
