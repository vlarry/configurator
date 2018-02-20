#ifndef CDATAUNITTYPE_H
    #define CDATAUNITTYPE_H
    //----------------
    #include <QVector>
    #include <QMap>
    #include <QVariant>
    //-----------------
    class CDataUnitType
    {
        public:
            enum FunctionType
            {
                ReadHoldingRegisters   = 0x03,
                ReadInputRegisters     = 0x04,
                WriteSingleRegister    = 0x06,
                WriteMultipleRegisters = 0x10
            };
            //------------
            enum ErrorType
            {
                NO_DEVICE_ERROR             = ((quint16)0x0000),
                BKPSRAM_ERROR               = ((quint16)0x0001),
                MAV_ERROR                   = ((quint16)0x0002),
                MDVV1_ERROR                 = ((quint16)0x0004),
                MDVV2_ERROR                 = ((quint16)0x0008),
                MIK_ERROR                   = ((quint16)0x0010),
                FLASH_ERROR                 = ((quint16)0x0020),
                EVENT_JOURNAL_ERROR         = ((quint16)0x0040),
                CRASH_JOURNAL_ERROR         = ((quint16)0x0080),
                HALF_HOUR_JOURNAL_ERROR     = ((quint16)0x0100),
                R_ISOLATOR_JOURNAL_ERROR    = ((quint16)0x0200),
                OSCILL_JOURNAL_ERROR        = ((quint16)0x0400),
                SET_DEFAULT_ERROR           = ((quint16)0x0800),
                SET_DEFAULT_INCORRECT_ERROR = ((quint16)0x1000)
            };
            
        public:
            CDataUnitType();
            CDataUnitType(quint8 slaveID, FunctionType type, uint16_t address, 
                          QVector<quint16> values);
            
            void setSlaveID(quint8 slaveID);
            void setFunctionType(FunctionType type);
            void setAddress(uint16_t address);
            void setValues(const QVector<quint16>& values);
            
            quint8                  slaveID() const;
            FunctionType            functionType() const;
            quint16                 address() const;
            quint16                 error() const;
            QString                 errorStringList();
            const QVector<quint16>& values() const;
            quint16                 value(int index) const;
            quint8                  valueCount() const;
            
            bool                    is_empty() const;
            bool                    is_valid() const;

            void                     setProperty(const QString& key, QVariant value);
            void                     setProperties(QMap<QString, QVariant> &properties);
            void                     serErrorCode(quint16 code);
            QVariant                 property(const QString& key) const;
            QMap<QString, QVariant>& properties();

            operator bool() const
            {
                return m_ok;
            }

        private:
            QString errorToString(quint16 code);
            
        private:
            FunctionType            m_type;
            quint8                  m_slaveID;
            quint16                 m_address;
            QVector<quint16>        m_values;
            QMap<QString, QVariant> m_properties;
            quint16                 m_error;
            bool                    m_ok; // true - unit является валидным
    };
#endif // CDATAUNITTYPE_H
