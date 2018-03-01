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
                ERROR_NO            = quint8(0x00), // Ошибок нет
                ERROR_CODE          = quint8(0x01), // Принятый код функции не может быть обработан
                ERROR_DATA_ADDRESS  = quint8(0x02), // Адрес данных, указанный в запросе, недоступен
                ERROR_VALUE_REQUEST = quint8(0x03), // Значение, содержащееся в поле данных запроса, является
                                                    // недопустимой величиной
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
            void                     serErrorCode(quint8 code);
            QVariant                 property(const QString& key) const;
            QMap<QString, QVariant>& properties();

            operator bool() const
            {
                return m_ok;
            }

        private:
            QString errorToString(quint8 code);
            
        private:
            FunctionType            m_type;
            quint8                  m_slaveID;
            quint16                 m_address;
            QVector<quint16>        m_values;
            QMap<QString, QVariant> m_properties;
            quint8                  m_error;
            bool                    m_ok; // true - unit является валидным
    };
#endif // CDATAUNITTYPE_H
