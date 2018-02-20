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
    #include "ctablewidgetitem.h"
    #include "ceventlistjournals.h"
    #include "cprogressbarwidget.h"
    #include "cfilterdialog.h"
    #include "cjournalwidget.h"
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
                CALCULATE_ADDRESS = 64
            };
            //--------------
            enum RequestType
            {
                CALCULATE_TYPE, // расчетные данные
                GENERAL_TYPE, // общие (настройки/уставки)
                PURPOSE_OUT_TYPE, // матрца привязок выходов
                PURPOSE_INPUT_TYPE, // матрица привязок входов
                READ_EVENT_JOURNAL, // чтение журнала событий
                READ_EVENT_COUNT, // чтение количества событий в журнале
                READ_EVENT_SHIFT_PTR, // чтение позиции указателя сдвига журнала событий
                READ_SERIAL_NUMBER, // чтение серийного номера
                READ_JOURNAL,
                READ_JOURNAL_COUNT,
                READ_JOURNAL_SHIFT_PTR
            };
            //-------------
            enum WidgetType
            {
                INT,
                FLOAT, // QLineEdit (set validator: QDoubleValidator)
                LIST   // QComboBox
            };
            //-------------------
            enum JournalIndexType
            {
                JOURNAL_INDEX_CRASH = 14,
                JOURNAL_INDEX_EVENT,
                JOURNAL_INDEX_HALFHOUR,
                JOURNAL_INDEX_ISOLATION,
                JOURNAL_INDEX_OSCILLOSCOPE
            };
            //----------------------
            enum DeviceMenuIndexType
            {
                DEVICE_MENU_INDEX_NONE = -1,
                DEVICE_MENU_INDEX_LED  = 4,
                DEVICE_MENU_INDEX_INPUT,
                DEVICE_MENU_INDEX_RELAY,
                DEVICE_MENU_INDEX_KEYBOARD,
                DEVICE_MENU_INDEX_CRASH = 0,
                DEVICE_MENU_INDEX_EVENT,
                DEVICE_MENU_INDEX_HALFHOUR,
                DEVICE_MENU_INDEX_ISOLATION
            };
            /*!
             * \brief The PurposeIndexType enum
             *
             *  Индексы таблиц привязок для определения выбранной таблицы
             */
            enum PurposeIndexType
            {
                PURPOSE_INDEX_LED = 24,
                PURPOSE_INDEX_INPUT,
                PURPOSE_INDEX_RELAY,
                PURPOSE_INDEX_KEYBOARD
            };
            //------------
            struct event_t
            {
                int              code;
                QString          name;
                QVector<event_t> sub_event;
            };
            //--------------------
            struct event_journal_t
            {
                int start; // событие с которого начинается чтение
                int count; // количество событий, которые будут прочитаны
                int read;  // количество прочитанных сообщений
                int total; // всего событий в устройстве
                int shift; // положение указателя сдвига
            };
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
            void journalRead();
            void journalReadNew(const QString& key);
            void inAnalogRead();
            void inAnalogWrite();
            void controlStateRead();
            void controlStateWrite();
            void protectionMTZSetRead();
            void protectionMTZSetWrite();
            void protectionEarthySetRead();
            void protectionEarthySetWrite();
            void protectionPowerSetRead();
            void protectionPowerSetWrite();
            void protectionMotorSetRead();
            void protectionMotorSetWrite();
            void protectionFrequencySetRead();
            void protectionFrequencySetWrite();
            void protectionExternalSetRead();
            void protectionExternalSetWrite();
            void protectionTemperatureSetRead();
            void protectionTemperatureSetWrite();
            void protectionLevelSetRead();
            void protectionLevelSetWrite();
            void protectionBruSetRead();
            void protectionBruSetWrite();
            void protectionVacuumSetRead();
            void protectionVacuumSetWrite();
            void processReadJournalEvent(bool checked);
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
            void protectMTZChangedID(int id);
            void protectEarthlyChangedID(int id);
            void protectPowerChangedID(int id);
            void protectMotorChangedID(int id);
            void protectFrequencyChangedID(int id);
            void protectExternalChangedID(int id);
            void protectTemperatureChangedID(int id);
            void protectLevelChangedID(int id);
            void switchDeviceChangedID(int id);
            void additionalChangedID(int id);
            void errorDevice(const QString& error);
            void terminalVisiblity(int state);
            void saveLog(const QString& info);
            void itemClicked(QTreeWidgetItem* item, int col);
            void readSettings();
            void readSetCurrent();
            void writeSettings();
            void writeSetCurrent();
            void expandItemTree(bool state);
            void versionSowftware();
            void sendSettingReadRequest(const QString& first, const QString& last,
                                        CDataUnitType::FunctionType type, int size);
            void sendSettingWriteRequest(const QString& first, const QString& last);
            void sendPurposeReadRequest(const QString& first, const QString& last);
            void sendPurposeWriteRequest(const QString& first, const QString& last);
            void sendPurposeDIReadRequest(int first_addr, int last_addr);
            void sendPurposeDIWriteRequest(int first_addr, int last_addr);
            void clearIOTable();
            void clearJournal();
            void menuPanelCtrl();
            void variablePanelCtrl();
            void exportToPDF(const CJournalWidget* widget, const QString& reportName, const QString& sn_device,
                                                                                      const QString& filename);
            void exportPurposeToJSON();
            void importPurposeFromJSON();
            void processReadJournal(CDataUnitType& unit);
            void updateParameterJournal(); // обновление данных журнала событий - вычитка кол-ва событий и положение указателя
            void widgetStackIndexChanged(int index);
            void setJournalPtrShift(const QString& key, long pos);
            void valueEventJournalInternalChanged(int new_value);
            void timeoutSyncSerialNumber();
            void importJournalToTable();
            void exportJournalToDb();
            void startExportToPDF();
            void filterDialog();
            
        private:
            bool createJournalTable(QSqlDatabase* db);
            bool currentJournal(const CJournalWidget*& widget);
            void loadSettings();
            void saveSattings();
            void initConnect();
            void initMenuPanel();
            void initButtonGroup();
            void initCellBind();
            void initPurposeBind();
            void initModelTables();
            void initEventJournal();
            void initDeviceCode();
            void initJournals();
            void connectSystemDb();
            bool connectDb(QSqlDatabase*& db, const QString& path);
            void disconnectDb(QSqlDatabase* db);
            void initTable(QTableView* table, CDataTable& data);
            void displayCalculateValues(QVector<quint16> values);
            void displaySettingResponse(CDataUnitType& unit);
            void displayPurposeResponse(CDataUnitType& unit);
            void displayPurposeDIResponse(CDataUnitType& unit);
            void displayJournalResponse(const QVector<quint16>& data_list);
            void displayDeviceSerialNumber(const QVector<quint16>& data);
            void versionParser();
            int  sizeBlockSetting(const QString& first, const QString& last);
            int  addressSettingKey(const QString& key) const;
            int  addressPurposeKey(const QString& key) const;
            void readShiftPrtEventJournal();
            void readEventJournalCount();
            void deviceSync(bool state = false);
            int  recordCountDb(QSqlDatabase *db, const QString& table_name, const QString& parameter, const QString& value,
                                                 const QString& subparamter = "", const QStringList& range = QStringList());
            QPoint              indexDateFilter(QTableWidget* table, const QDate& begin, const QDate& end);
            QPoint              indexSettingKey(const QString& first, const QString& last);
            QPoint              indexPurposeKey(const QString& first, const QString& last);
            QVector<int>        indexVariableFromKey(const QStringList& variables, const QString& key);
            QTableView*         tableMatrixFromKeys(const QString& first, const QString& last);
            CColumn::column_t   columnFromKey(const QString& key);
            DeviceMenuIndexType menuIndex();
            
        private:
            Ui::ConfiguratorWindow*      ui;
            CModbus*                     m_modbusDevice;
            CSerialPortSetting*          m_serialPortSettings;
            QPanel*                      m_calculateWidget;
            CTerminal*                   m_terminal;
            QFile*                       m_logFile;
            QTimer*                      m_tim_calculate;
            QButtonGroup*                m_protect_mtz_group;
            QButtonGroup*                m_protect_earthly_group;
            QButtonGroup*                m_protect_power_group;
            QButtonGroup*                m_protect_motor_group;
            QButtonGroup*                m_protect_frequency_group;
            QButtonGroup*                m_protect_external_group;
            QButtonGroup*                m_protect_temperature_group;
            QButtonGroup*                m_protect_level_group;
            QButtonGroup*                m_switch_device_group;
            QButtonGroup*                m_additional_group;
            CVersionSoftware*            m_versionWidget;
            QSqlDatabase                 m_system_db;
            cell_t                       m_cell_list;
            purpose_t                    m_purpose_list;
            QVector<event_t>             m_event_list; // список событий (вычитаны из БД)
            event_journal_t              m_journal_parameters;
            QVector<CColumn::column_t>   m_variables;
            QTime                        m_time_process;
            QTimer                       m_sync_timer;
            CStatusBar*                  m_status_bar;
            QMap<int, QString>           m_device_code_list;
            QFutureWatcher<void>*        m_watcher;
            CProgressBarWidget*          m_progressbar;
            QSettings*                   m_settings;
            QMap<QString, CFilter>       m_filter;
            const CJournalWidget*        m_active_journal_current; // текущий активный журнал
            const CJournalWidget*        m_journal_read_current; // текущий журнал чтения, т.е. журнал, который читают из устройства
            QMap<QString, journal_set_t> m_journal_set; // установки журналов
    };
#endif // CONFIGURATORWINDOW_H
