#ifndef CMODBUSDATAUNIT_H
    #define CMODBUSDATAUNIT_H
    //----------------
    #include <QVector>
    #include <QDebug>
    //-------------------
    class CModBusDataUnit
    {
        public:
            enum FunctionType
            {
                ReadHoldingRegisters   = quint8(0x03),
                ReadInputRegisters     = quint8(0x04),
                WriteSingleRegister    = quint8(0x06),
                WriteMultipleRegisters = quint8(0x10),
                UnknownFunction        = quint8(0xFF)
            };
            //------------
            enum ErrorType
            {
                ERROR_NO            = quint8(0x00), // Ошибок не обнаружено
                ERROR_CODE          = quint8(0x01), // Принятый код функции не может быть обработан
                ERROR_DATA_ADDRESS  = quint8(0x02), // Адрес данных, указанный в запросе, недоступен
                ERROR_VALUE_REQUEST = quint8(0x03), // Значение, содержащееся в поле данных запроса, является
                                                    // недопустимой величиной
            };
            //-----------------------------------------
            typedef quint16                     cell_t;
            typedef QVector<cell_t>             vlist_t;
            typedef QMap<QString, QVariant>     property_t;
            typedef QMap<FunctionType, QString> function_desc_t;
            typedef QMap<ErrorType, QString>    error_t;

        public:
            CModBusDataUnit();
            CModBusDataUnit(const quint8 slaveId, const FunctionType function, const quint16 address, const vlist_t& values);
            CModBusDataUnit(const quint8 slaveId, const FunctionType function, const quint16 address, const cell_t value);

            quint16        address() const;
            quint8         count() const;
            ErrorType      error() const;
            static QString errorDescription(ErrorType error);
            FunctionType   function() const;
            quint8         id() const;
            bool           isEmpty() const;
            bool           isValid() const;
            quint16        operator [](const int index);
            quint16        operator [](const int index) const;
            QVariant       property(const QString& name_property) const;
            property_t&    properties();
            void           setAddress(quint16 address);
            void           setError(ErrorType error);
            void           setFunction(FunctionType function);
            void           setID(quint8 id);
            void           setProperty(const QString& name, QVariant value);
            void           setProperties(property_t& properties);
            void           setValues(vlist_t& values);
            QString        toString() const;
            const vlist_t& values() const;

        private:
            bool                   m_valid;
            quint8                 m_id;
            FunctionType           m_function;
            quint16                m_address;
            vlist_t                m_values;
            property_t             m_property;
            ErrorType              m_error;
            static function_desc_t m_function_description;
            static error_t         m_error_description;
    };
#endif // CMODBUSDATAUNIT_H
