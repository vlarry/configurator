#include "modbus.h"
//--------------------------------
CModBus::CModBus(QObject* parent):
    QObject(parent),
    m_channel(nullptr)
{
    m_channel = new CConnect(this);

    connect(m_channel, &CConnect::readyRead, this, &CModBus::readyReadData);
    connect(this, &CModBus::open, m_channel, &CConnect::open);
    connect(this, &CModBus::close, m_channel, &CConnect::close);
    connect(m_channel, &CConnect::stateChanged, this, &CModBus::stateChanged);
}
//--------------------------
CConnect* CModBus::channel()
{
    return m_channel;
}
//-----------------------------------------------------
quint16 CModBus::crc16(QByteArray& data, size_t length)
{
    quint16 crc = 0xFFFF;

    for(quint8 i = 0; i < length; i++)
    {
        crc ^= quint8(data[i]);

        for(int j=0; j < 8; j++)
        {
            if(crc & 0x01)
                crc = (crc >> 1)^0xA001; // LSB(b0)=1
            else
                crc = crc >> 1;
        }
    }

    return crc;
}
//--------------------------------------------
void CModBus::readyReadData(QByteArray& bytes)
{
    qDebug() << tr("Получены данные: %1 байт.").arg(bytes.count());
}
//------------------------------------------
void CModBus::request(CModBusDataUnit& unit)
{
    QByteArray ba;

    ba.append(unit.id());
    ba.append(quint8(unit.function()));
    ba.append(quint8((unit.address() >> 8)&0xFF)); // MSB address
    ba.append(quint8(unit.address()&0xFF)); // LSB address

    switch(unit.function())
    {
        // ID, FUNCTION_CODE, ADDRESS REGISTER FIRST, REGISTER NUMBERS, CRC
        case CModBusDataUnit::ReadHoldingRegisters:
        case CModBusDataUnit::ReadInputRegisters:
        // ID, FUNCTION_CODE, ADDRESS REGISTER FIRST, VALUE, CRC
        case CModBusDataUnit::WriteSingleRegister:
            ba.append((unit[0] >> 8)&0xFF); // MSB register numbers
            ba.append(unit[0]&0xFF); // LSB register numbers
        break;

        // ID, FUNCTION_CODE, ADDRESS REGISTER FIRST, REGISTER NUMBERS, ...VALUES..., CRC
        case CModBusDataUnit::WriteMultipleRegisters:
            ba.append((unit.count() >> 8)&0xFF); // MSB register numbers
            ba.append(unit.count()&0xFF); // LSB register numbers

            for(int i = 0; i < unit.count(); i++)
            {
                ba.append((unit[i] >> 8)&0xFF); // MSB data value
                ba.append(unit[i]&0xFF); // LSB data value
            }
        break;

        default:
            qWarning() << tr("Неизвестный код функции: %1.").arg(quint8(unit.function()));
        return;
    }

    quint16 crc = crc16(ba, ba.count());

    ba.append(crc&0xFF); // LSB crc
    ba.append((crc >> 8)&0xFF); // MSB crc

    m_channel->write(ba);

    emit rawData(ba);
}
//-------------------------------------------
void CModBus::sendData(CModBusDataUnit& unit)
{
    if(unit.isValid())
        request(unit);
}
