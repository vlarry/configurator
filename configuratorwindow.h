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
    #include "cmodbus.h"
    #include "qpanel.h"
    #include "cterminal.h"
    #include "qcustomplot.h"
    #include "cversionsoftware.h"
    #include "cmatrixpurposemodel.h"
    #include "cserialportsetting.h"
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
                READ_EVENT_JOURNAL
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
                int              c_event; // количество доступных событий в журнале
                int              count; // счетчик прочитанных событий
                int              shitf; // смещение чтения событий, когда их больше 8192 байта
                QVector<event_t> event; // база событий (вычитаны из БД)
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
            void eventJournalRead(bool isShift = false); // isShift - признак сдвига окна чтения
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
            
        private:
            void initConnect();
            void initMenuPanel();
            void initButtonGroup();
            void initCellBind();
            void initPurposeBind();
            void initModelTables();
            void initEventJournal();
            void connectDb();
            void initTable(QTableView* table, CDataTable& data);
            void displayCalculateValues(QVector<quint16> values);
            void displaySettingResponse(CDataUnitType& unit);
            void displayPurposeResponse(CDataUnitType& unit);
            void displayPurposeDIResponse(CDataUnitType& unit);
            void displayEventJournalResponse(CDataUnitType& unit);
            void versionParser();
            int  sizeBlockSetting(const QString& first, const QString& last);
            int  addressSettingKey(const QString& key) const;
            int  addressPurposeKey(const QString& key) const;
            QPoint            indexSettingKey(const QString& first, const QString& last);
            QPoint            indexPurposeKey(const QString& first, const QString& last);
            QVector<int>      indexVariableFromKey(const QStringList& variables, const QString& key);
            QTableView*       tableMatrixFromKeys(const QString& first, const QString& last);
            
        private:
            Ui::ConfiguratorWindow* ui;
            CModbus*                m_modbusDevice;
            CSerialPortSetting*     m_serialPortSettings;
            QPanel*                 m_calculateWidget;
            CTerminal*              m_terminal;
            QFile*                  m_logFile;
            QTimer*                 m_tim_calculate;
            QButtonGroup*           m_protect_mtz_group;
            QButtonGroup*           m_protect_earthly_group;
            QButtonGroup*           m_protect_power_group;
            QButtonGroup*           m_protect_motor_group;
            QButtonGroup*           m_protect_frequency_group;
            QButtonGroup*           m_protect_external_group;
            QButtonGroup*           m_protect_temperature_group;
            QButtonGroup*           m_protect_level_group;
            QButtonGroup*           m_switch_device_group;
            QButtonGroup*           m_additional_group;
            CVersionSoftware*       m_versionWidget;
            QSqlDatabase            m_db;
            cell_t                  m_cell_list;
            purpose_t               m_purpose_list;
            event_journal_t         m_event_journal_list;

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
