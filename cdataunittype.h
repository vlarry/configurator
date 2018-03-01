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
                ERROR_FUNCTION_NO_SUPPORT    = quint8(0x00), // функция не поддерживается
                ERROR_REGISTER_NUMBER        = quint8(0x01), // кол-во регистров превышает 255
                ERROR_PACKET_LENGTH          = quint8(0x02), // ошибка длины пакета
                ERROR_BYTES_COUNT            = quint8(0x04), // счетчик байтов не бьется с заявленным кол-вом регистров
                ERROR_CRC                    = quint8(0x08), // ошибка CRC
                ERROR_REGISTER_ADDRESS_RANGE = quint8(0x10), // адреса регистров вне диапазона
                ERROR_FORMAT_DATA_WRITE      = quint8(0x20), // ошибка формата записываемых данных
                ERROR_DATA                   = quint8(0x40), // неверный код активации (ошибка данных)
                ERROR_NO                     = quint8(0xFF)  // нет ошибок
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
