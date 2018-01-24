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
    #include "cmodbus.h"
    #include "qpanel.h"
    #include "cterminal.h"
    #include "qcustomplot.h"
    #include "cversionsoftware.h"
    #include "cmatrixpurposemodel.h"
    #include "cserialportsetting.h"
    #include "ccalendarwidget.h"
    #include "cstatusbar.h"
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
                READ_SERIAL_NUMBER // чтение серийного номера
            };
            //-------------
            enum WidgetType
            {
                INT,
                FLOAT, // QLineEdit (set validator: QDoubleValidator)
                LIST   // QComboBox
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
            //------------------------------------------------------------------------------------------
            //--------------------key, address, description, list variables purpose---------------------
            typedef QVector<QPair<QString, QPair<int, QPair<QString, QVector<QString> > > > > purpose_t;
            //---------------------------------key, addres, widget name------------------------------
            typedef QVector<QPair<QString, QPair<int, QString> > >                            cell_t;

        public:
            explicit ConfiguratorWindow(QWidget* parent = Q_NULLPTR);
            ~ConfiguratorWindow();
        
        public slots:
            void serialPortCtrl();
            void stateChanged(bool state);
            void refreshSerialPort();
            void serialPortSettings();
            void calculateRead(); // запрос расчетных величин
            void eventJournalRead();
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
            void automationSetRead();
            void automationSetWrite();
            void switchDeviceSetRead();
            void switchDeviceSetWrite();
            void responseRead(CDataUnitType& unit);
            void exitFromApp();
            void show();
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
            void clearEventJournal();
            void menuPanelCtrl();
            void variablePanelCtrl();
            void exportToPDF();
            void exportPurposeToJSON();
            void importPurposeFromJSON();
            void eventJournalActiveRange(bool state);
            void eventJournalTypeRange();
            void eventJournalCalendar();
            void eventJournalDateChanged();
            void processReadJournal(CDataUnitType& unit);
            void updateParameterEventJournal(); // обновление данных журнала событий - вычитка кол-ва событий и положение указателя
            void widgetStackIndexChanged(int index);
            void setEventJournalPtrShift();
            void valueEventJournalInternalChanged(int new_value);
            void timeoutSyncSerialNumber();
            void importEventJournalToTable();
            void exportEventJournalToDb();
            
        private:
            void initConnect();
            void initMenuPanel();
            void initButtonGroup();
            void initCellBind();
            void initPurposeBind();
            void initModelTables();
            void initEventJournal();
            void initDeviceCode();
            void connectSystemDb();
            bool connectEventsDb();
            void initTable(QTableView* table, CDataTable& data);
            void displayCalculateValues(QVector<quint16> values);
            void displaySettingResponse(CDataUnitType& unit);
            void displayPurposeResponse(CDataUnitType& unit);
            void displayPurposeDIResponse(CDataUnitType& unit);
            void displayEventJournalResponse(const QVector<quint16>& data_list);
            void displayDeviceSerialNumber(const QVector<quint16>& data);
            void versionParser();
            int  sizeBlockSetting(const QString& first, const QString& last);
            int  addressSettingKey(const QString& key) const;
            int  addressPurposeKey(const QString& key) const;
            void readShiftPrtEventJournal();
            void readEventJournalCount();
            void deviceSync(bool state = false);
            QPoint            indexSettingKey(const QString& first, const QString& last);
            QPoint            indexPurposeKey(const QString& first, const QString& last);
            QVector<int>      indexVariableFromKey(const QStringList& variables, const QString& key);
            QTableView*       tableMatrixFromKeys(const QString& first, const QString& last);
            CColumn::column_t columnFromKey(const QString& key);
            
        private:
            Ui::ConfiguratorWindow*    ui;
            CModbus*                   m_modbusDevice;
            CSerialPortSetting*        m_serialPortSettings;
            QPanel*                    m_calculateWidget;
            CTerminal*                 m_terminal;
            QFile*                     m_logFile;
            QTimer*                    m_tim_calculate;
            QButtonGroup*              m_protect_mtz_group;
            QButtonGroup*              m_protect_earthly_group;
            QButtonGroup*              m_protect_power_group;
            QButtonGroup*              m_protect_motor_group;
            QButtonGroup*              m_protect_frequency_group;
            QButtonGroup*              m_protect_external_group;
            QButtonGroup*              m_protect_temperature_group;
            QButtonGroup*              m_protect_level_group;
            QButtonGroup*              m_switch_device_group;
            QButtonGroup*              m_additional_group;
            CVersionSoftware*          m_versionWidget;
            QSqlDatabase               m_system_db;
            QSqlDatabase               m_event_journal_db;
            cell_t                     m_cell_list;
            purpose_t                  m_purpose_list;
            QVector<event_t>           m_event_list; // список событий (вычитаны из БД)
            event_journal_t            m_event_journal_parameter;
            QVector<CColumn::column_t> m_variables;
            CCalendarWidget*           m_calendar_wgt;
            QTime                      m_time_process;
            QTimer                     m_sync_timer;
            CStatusBar*                m_status_bar;
            QMap<int, QString>         m_device_code_list;

            QTreeWidgetItem* itemSettings;
            QTreeWidgetItem* itemJournals;
            QTreeWidgetItem* itemMeasures;
            QTreeWidgetItem* itemMonitoring;
            QTreeWidgetItem* itemSetInputAnalogs;
            QTreeWidgetItem* itemSetProtections;
            QTreeWidgetItem* itemSetDevConnections;
            QTreeWidgetItem* itemSetAutomation;
            QTreeWidgetItem* itemJournalCrashs;
            QTreeWidgetItem* itemJournalEvents;
            QTreeWidgetItem* itemJournalOscill;
            QTreeWidgetItem* itemMeasPrimaryValues;
            QTreeWidgetItem* itemMeasSecondaryValues;
            QTreeWidgetItem* itemMeasPowerElectric;
            QTreeWidgetItem* itemMonitorInputDiscrets;
            QTreeWidgetItem* itemMonitorOutputDiscrets;
            QTreeWidgetItem* itemInAnalogMain;
            QTreeWidgetItem* itemInAnalogCalibration;
            QTreeWidgetItem* itemProtectCurrentMax;
            QTreeWidgetItem* itemProtectEarthy;
            QTreeWidgetItem* itemProtectPower;
            QTreeWidgetItem* itemProtectMotor;
            QTreeWidgetItem* itemProtectFrequency;
            QTreeWidgetItem* itemProtectExternal;
            QTreeWidgetItem* itemProtectTemperature;
            QTreeWidgetItem* itemProtectLevel;
            QTreeWidgetItem* itemProtectBRU;
            QTreeWidgetItem* itemProtectVacuum;
            QTreeWidgetItem* itemSetLedPurpose;
            QTreeWidgetItem* itemSetDiscretInPurpose;
            QTreeWidgetItem* itemSetRelayPurpose;
            QTreeWidgetItem* itemSetKeyboardPurpose;
    };
#endif // CONFIGURATORWINDOW_H
