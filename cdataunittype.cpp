#include "cdataunittype.h"
//----------------------------
CDataUnitType::CDataUnitType()
{
    
}
//-------------------------------------------
CDataUnitType::CDataUnitType(quint8 slaveID):
    m_slaveID(slaveID)
{
    m_slaveID = slaveID;
}
//----------------------------------------------
CDataUnitType::CDataUnitType(FunctionType type):
    m_type(type)
{
    m_type = type;
}
//----------------------------------------------------------------------------
CDataUnitType::CDataUnitType(quint8 slaveID, CDataUnitType::FunctionType type, 
                             uint16_t address, QVector<quint16> values):
    m_type(type),
    m_slaveID(slaveID),
    m_address(address),
    m_values(values)
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
    return (m_slaveID == 0xFF);
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
