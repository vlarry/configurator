#include "cdataunittype.h"
//-----------------------------
CDataUnitType::CDataUnitType():
    m_error(NO_DEVICE_ERROR)
{
    
}
//----------------------------------------------------------------------------
CDataUnitType::CDataUnitType(quint8 slaveID, CDataUnitType::FunctionType type, 
                             uint16_t address, QVector<quint16> values):
    m_type(type),
    m_slaveID(slaveID),
    m_address(address),
    m_values(values),
    m_error(NO_DEVICE_ERROR)
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
//------------------------------------------------
QString CDataUnitType::errorToString(quint16 code)
{
    QString str_error = "Ошибка ";

    switch(code)
    {
        case BKPSRAM_ERROR:
            str_error += "Разрушена BKPSRAM.\n";
        break;

        case MAV_ERROR:
            str_error += "МАВ.\n";
        break;

        case MDVV1_ERROR:
            str_error += "МДВВ1.\n";
        break;

        case MDVV2_ERROR:
            str_error += "МДВВ2.\n";
        break;

        case MIK_ERROR:
            str_error += "МИК.\n";
        break;

        case FLASH_ERROR:
            str_error += "FLASH.\n";
        break;

        case EVENT_JOURNAL_ERROR:
            str_error += "журнала событий.\n";
        break;

        case CRASH_JOURNAL_ERROR:
            str_error += "журнала аварий.\n";
        break;

        case HALF_HOUR_JOURNAL_ERROR:
            str_error += "журнала получасовок.\n";
        break;

        case R_ISOLATOR_JOURNAL_ERROR:
            str_error += "журнала сопротивления изоляции.\n";
        break;

        case OSCILL_JOURNAL_ERROR:
            str_error += "журнала осциллограмм.\n";
        break;

        case SET_DEFAULT_ERROR:
            str_error += "установки умолчаний.\n";
        break;

        case SET_DEFAULT_INCORRECT_ERROR:
            str_error += "установки умолчаний из-за некорректности.\n";
        break;

        case NO_DEVICE_ERROR:
            str_error = "";
        break;

        default:
            str_error += "не определена.\n";
        break;
    }

    return str_error;
}
//--------------------------------------
QString CDataUnitType::errorStringList()
{
    QString str = "";

    if(m_error != NO_DEVICE_ERROR)
    {
        for(int i = 0; i < 16; i++)
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
//--------------------------------------------
void CDataUnitType::serErrorCode(quint16 code)
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
