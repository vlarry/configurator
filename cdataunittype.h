#ifndef CDATAUNITTYPE_H
    #define CDATAUNITTYPE_H
    //----------------
    #include <QVector>
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
            
        public:
            CDataUnitType();
            CDataUnitType(quint8 slaveID);
            CDataUnitType(FunctionType type);
            CDataUnitType(quint8 slaveID, FunctionType type, uint16_t address, 
                          QVector<quint16> values);
            
            void setSlaveID(quint8 slaveID);
            void setFunctionType(FunctionType type);
            void setAddress(uint16_t address);
            void setValues(const QVector<quint16>& values);
            
            quint8                  slaveID() const;
            FunctionType            functionType() const;
            quint16                 address() const;
            const QVector<quint16>& values() const;
            quint16                 value(int index) const;
            quint8                  valueCount() const;
            
            bool                    is_empty() const;
            
        private:
            FunctionType     m_type;
            quint8           m_slaveID;
            quint16          m_address;
            QVector<quint16> m_values;
    };
#endif // CDATAUNITTYPE_H
