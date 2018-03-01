#include "cdataunittype.h"
//-----------------------------
CDataUnitType::CDataUnitType():
    m_error(ERROR_NO),
    m_ok(false)
{
    
}
//----------------------------------------------------------------------------
CDataUnitType::CDataUnitType(quint8 slaveID, CDataUnitType::FunctionType type, 
                             uint16_t address, QVector<quint16> values):
    m_type(type),
    m_slaveID(slaveID),
    m_address(address),
    m_values(values),
    m_error(ERROR_NO),
    m_ok(true)
{
    
}
//--------------------------------------------
void CDataUnitType::setSlaveID(quint8 slaveID)
{
    m_slaveID = slaveID;
}
//-------------------------------------------------------------------
void CDataUnitType::setFunctionType(CDataUnitType::FunctionType type)
{
    m_type = type;
}
//----------------------------------------------
void CDataUnitType::setAddress(uint16_t address)
{
    m_address = address;
}
//-----------------------------------------------------------
void CDataUnitType::setValues(const QVector<quint16>& values)
{
    m_values = values;
}
//-----------------------------------
quint8 CDataUnitType::slaveID() const
{
    return m_slaveID;
}
//-------------------------------------------------------------
CDataUnitType::FunctionType CDataUnitType::functionType() const
{
    return m_type;
}
//------------------------------------
quint16 CDataUnitType::address() const
{
    return m_address;
}
//----------------------------------
quint16 CDataUnitType::error() const
{
    return m_error;
}
//-----------------------------------------------
QString CDataUnitType::errorToString(quint8 code)
{
    QString str_error = "";

    switch(code)
    {
        case ERROR_CODE:
            str_error += QObject::tr("Принятый код функции не может быть обработан.\n");
        break;

        case ERROR_DATA_ADDRESS:
            str_error += QObject::tr("Адрес данных, указанный в запросе, недоступен.\n");
        break;

        case ERROR_VALUE_REQUEST:
            str_error += QObject::tr("Значение, содержащееся в поле данных запроса, является недопустимой величиной.\n");
        break;

        case ERROR_NO:
            str_error = "";
        break;

        default:
            str_error += QObject::tr("Не определена.\n");
        break;
    }

    return str_error;
}
//--------------------------------------
QString CDataUnitType::errorStringList()
{
    QString str = "";

    if(m_error != ERROR_NO)
    {
        for(int i = 0; i < 8; i++)
        {
            quint16 err = (m_error&(1 << i));

            if(err)
                str += errorToString(err);
        }
    }

    return str;
}
//---------------------------------------------------
const QVector<quint16>& CDataUnitType::values() const
{
    return m_values;
}
//-------------------------------------------
quint16 CDataUnitType::value(int index) const
{
    if(index < m_values.count())
        return m_values.at(index);
    
    return 0xFFFF;
}
//--------------------------------------
quint8 CDataUnitType::valueCount() const
{
    return m_values.count();
}
//----------------------------------
bool CDataUnitType::is_empty() const
{
    return !m_ok;
}
//----------------------------------
bool CDataUnitType::is_valid() const
{
    return m_ok;
}
//------------------------------------------------------------------
void CDataUnitType::setProperty(const QString& key, QVariant value)
{
    m_properties.insert(key, value);
}
//--------------------------------------------------------------------
void CDataUnitType::setProperties(QMap<QString, QVariant>& properties)
{
    m_properties = properties;
}
//--------------------------------------------
void CDataUnitType::serErrorCode(quint8 code)
{
    m_error = code;
}
//---------------------------------------------------------
QVariant CDataUnitType::property(const QString& key) const
{
    return m_properties.value(key);
}
//--------------------------------------------------
QMap<QString, QVariant>& CDataUnitType::properties()
{
    return m_properties;
}
