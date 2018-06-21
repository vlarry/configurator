#include "modbusdataunit.h"
//------------------------------------------------------------------------
CModBusDataUnit::function_desc_t CModBusDataUnit::m_function_description =
{
    { CModBusDataUnit::ReadHoldingRegisters, "ReadHoldingRegisters" },
    { CModBusDataUnit::ReadInputRegisters, "ReadInputRegisters" },
    { CModBusDataUnit::WriteSingleRegister, "WriteSingleRegister" },
    { CModBusDataUnit::WriteMultipleRegisters, "WriteMultipleRegisters" },
    { CModBusDataUnit::UnknownFunction, "UnknownFunction" }
};
//-------------------------------------------------------------
CModBusDataUnit::error_t CModBusDataUnit::m_error_description =
{
    { CModBusDataUnit::ERROR_NO, QObject::tr("Ошибок не обнаружено") },
    { CModBusDataUnit::ERROR_CODE, QObject::tr("Принятый код функции не может быть обработан") },
    { CModBusDataUnit::ERROR_DATA_ADDRESS, QObject::tr("Адрес данных, указанный в запросе, недоступен") },
    { CModBusDataUnit::ERROR_VALUE_REQUEST, QObject::tr("Значение, содержащееся в поле данных запроса,"
                                                        "\nявляется недопустимой величиной") }
};
//---------------------------------
CModBusDataUnit::CModBusDataUnit():
    m_valid(false),
    m_id(0xFF),
    m_function(UnknownFunction),
    m_address(0),
    m_values(vlist_t(0)),
    m_error(ERROR_NO)
{

}
//--------------------------------------------------------------------------------------------------
CModBusDataUnit::CModBusDataUnit(const quint8 slaveId, const CModBusDataUnit::FunctionType function,
                                 const quint16 address, const CModBusDataUnit::vlist_t& values)
{
    if(slaveId == 0xFF || function == 0xFF)
    {
        qWarning() << QObject::tr("Некорректное значение id/функции: %1/%2").arg(slaveId).arg(function);
        return;
    }

    m_id       = slaveId;
    m_function = function;
    m_address  = address;
    m_values   = values;

    m_valid = true;
    m_error = ERROR_NO;
}
//--------------------------------------------------------------------------------------------------
CModBusDataUnit::CModBusDataUnit(const quint8 slaveId, const CModBusDataUnit::FunctionType function,
                                 const quint16 address, const CModBusDataUnit::cell_t value)
{
    if(slaveId == 0xFF || function == 0xFF)
    {
        qWarning() << QObject::tr("Некорректное значение id/функции: %1/%2").arg(slaveId).arg(function);
        return;
    }

    m_id       = slaveId;
    m_function = function;
    m_address  = address;
    m_values  << value;

    m_valid = true;
    m_error = ERROR_NO;
}
//--------------------------------------
quint16 CModBusDataUnit::address() const
{
    return m_address;
}
//-----------------------------------
quint8 CModBusDataUnit::count() const
{
    return m_values.count();
}
//-------------------------------------------------------
CModBusDataUnit::ErrorType CModBusDataUnit::error() const
{
    return m_error;
}
//-------------------------------------------------------------------------
QString CModBusDataUnit::errorDescription(CModBusDataUnit::ErrorType error)
{
    if(error >= ERROR_NO && error <= ERROR_VALUE_REQUEST)
        return m_error_description[error];

    return "";
}
//-------------------------------------------------------------
CModBusDataUnit::FunctionType CModBusDataUnit::function() const
{
    return m_function;
}
//--------------------------------
quint8 CModBusDataUnit::id() const
{
    return m_id;
}
//-----------------------------------
bool CModBusDataUnit::isEmpty() const
{
    return m_values.isEmpty();
}
//-----------------------------------
bool CModBusDataUnit::isValid() const
{
    return m_valid;
}
//---------------------------------------------------
quint16 CModBusDataUnit::operator [](const int index)
{
    if(index >= 0 && index < m_values.count())
        return m_values[index];

    return 0xFFFF;
}
//---------------------------------------------------------
quint16 CModBusDataUnit::operator [](const int index) const
{
    if(index >= 0 && index < m_values.count())
        return m_values[index];

    return 0xFFFF;
}
//--------------------------------------------------------------------
QVariant CModBusDataUnit::property(const QString& name_property) const
{
    if(m_property.find(name_property) != m_property.end())
        return m_property[name_property];

    return QVariant();
}
//--------------------------------------------------------
CModBusDataUnit::property_t& CModBusDataUnit::properties()
{
    return m_property;
}
//-----------------------------------------------
void CModBusDataUnit::setAddress(quint16 address)
{
    m_address = address;
    m_valid   = true;
}
//--------------------------------------------------------------
void CModBusDataUnit::setError(CModBusDataUnit::ErrorType error)
{
    if(error >= ERROR_NO && error <= ERROR_VALUE_REQUEST)
        m_error = error;
}
//-----------------------------------------------------------------------
void CModBusDataUnit::setFunction(CModBusDataUnit::FunctionType function)
{
    m_function = function;
    m_valid    = true;
}
//------------------------------------
void CModBusDataUnit::setID(quint8 id)
{
    m_id    = id;
    m_valid = true;
}
//--------------------------------------------------------------------
void CModBusDataUnit::setProperty(const QString& name, QVariant value)
{
    m_property[name] = value;
}
//--------------------------------------------------------------------------
void CModBusDataUnit::setProperties(CModBusDataUnit::property_t& properties)
{
    m_property = properties;
}
//---------------------------------------------------------------
void CModBusDataUnit::setValues(CModBusDataUnit::vlist_t& values)
{
    m_values = values;
    m_valid  = true;
}
//---------------------------------------
QString CModBusDataUnit::toString() const
{
    return QString("slaveID: %1, function: %2, address: %3, data size: %4").arg(m_id).
                                                                            arg(m_function_description[m_function]).
                                                                            arg(m_address).
                                                                            arg(m_values.count());
}
//-------------------------------------------------------------
const CModBusDataUnit::vlist_t& CModBusDataUnit::values() const
{
    return m_values;
}
