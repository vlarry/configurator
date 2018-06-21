#include "modbus.h"
//--------------------------------
CModBus::CModBus(QObject* parent):
    QObject(parent),
    m_channel(nullptr),
    m_request(CModBusDataUnit()),
    m_queue(queue_t(0)),
    m_connect(false),
    m_block(false),
    m_interval_timeout_response(1000),
    m_interval_timeout_silence(4),
    m_trycount(3),
    m_timer_timeout_response(nullptr),
    m_timer_timeout_silence(nullptr)
{
    m_channel                = new CConnect(this);
    m_timer_timeout_response = new QTimer(this);
    m_timer_timeout_silence  = new QTimer(this);

    connect(m_channel, &CConnect::readyRead, this, &CModBus::readyReadData);
    connect(this, &CModBus::open, m_channel, &CConnect::open);
    connect(this, &CModBus::close, m_channel, &CConnect::close);
    connect(m_channel, &CConnect::stateChanged, this, &CModBus::stateChanged);
    connect(this, &CModBus::close, this, &CModBus::disconnected);
    connect(m_timer_timeout_response, &QTimer::timeout, this, &CModBus::timeoutResponce);
    connect(m_timer_timeout_silence, &QTimer::timeout, this, &CModBus::timeoutSilencce);
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
//---------------------------
bool CModBus::isBlock() const
{
    return m_block;
}
//--------------------------------------------
void CModBus::readyReadData(QByteArray& bytes)
{
    if(!m_request.isValid())
        return;

    m_buffer += bytes;

    if(m_buffer.count() < 2)
        return;

    int size, offset;

    CModBusDataUnit::FunctionType code_function = CModBusDataUnit::FunctionType(quint8(m_buffer[1]));

    switch(code_function)
    {
        // ID, FUNCTION_CODE, BYTE NUMBERS, ...VALUES..., CRC(2 bytes)
        case CModBusDataUnit::ReadHoldingRegisters:
        case CModBusDataUnit::ReadInputRegisters:
            size   = quint8(m_request[0])*2 + 5;
            offset = 3;
        break;

        // ID, FUNCTION_CODE, ADDRESS REGISTER FIRST, WRITE BYTE NUMBERS MSB, WRITE BYTE NUMBERS LSB, CRC(2 bytes)
        case CModBusDataUnit::WriteMultipleRegisters:
        // ID, FUNCTION_CODE, ADDRESS REGISTER FIRST, VALUE MSB, VALUE LSB, CRC(2 bytes)
        case CModBusDataUnit::WriteSingleRegister:
            size   = 8;
            offset = 4;
        break;

        default:
            // ID, FUNCTION_CODE WITH BIT 0x80, CODE ERROR, CRC(2 bytes)
            if(code_function & 0x80) // в ответе устройства обнаружена ошибка
            {
                size = 5;
            }
            else
            {
                qDebug() << tr("Неизвестный код функции: %1.").arg(m_request.toString());
                return;
            }
        break;
    }

    if(m_buffer.count() < size)
    {
        qDebug() << tr("Принята часть данных: %1").arg(m_buffer.count());
        return;
    }
    else if(m_buffer.count() > size)
    {
        qDebug() << tr("Принятые данные больше по размеру, чем ожидается: %1/%2.").arg(m_buffer.count()).arg(size);
        emit rawData(m_buffer, false);
    }
    else // прияты все данные
    {
        if(!m_connect) // синхронизация по первому принятому сообщению
            m_connect = true;

        qDebug() << tr("Данные приняты в полном объеме %1 байт за %2мс.").arg(m_buffer.count()).arg(m_time_process.elapsed());

        // расчет и проверка контрольной суммы
        quint8 mbs = m_buffer[m_buffer.count() - 2];
        quint8 lbs = m_buffer[m_buffer.count() - 1];

        quint16 crc_receive = ((quint16)lbs << 8) | mbs;
        quint16 crc_calculate = crc16(m_buffer, m_buffer.count() - 2);

        if(crc_receive == crc_calculate)
        {
            quint8 id                                   = quint8(m_buffer[0]);
            CModBusDataUnit::ErrorType error            = CModBusDataUnit::ERROR_NO;

            if(code_function & 0x80)
            {
                code_function = CModBusDataUnit::FunctionType(quint8(code_function)^0x80);
                error         = CModBusDataUnit::ErrorType(quint8(m_buffer[2]));
            }

            if(id == m_request.id() && code_function == m_request.function())
            {
                CModBusDataUnit::vlist_t values;

                for(int i = offset; i < size - 3; i += 2)
                {
                    quint8 mbs = quint8(m_buffer[i]);
                    quint8 lbs = quint8(m_buffer[i + 1]);

                    values << CModBusDataUnit::cell_t((mbs << 8) | lbs);
                }

                CModBusDataUnit unit(id, code_function, m_request.address(), values);

                unit.setProperties(m_request.properties()); // наследуем свойства от запроса

                if(error != CModBusDataUnit::ERROR_NO)
                    unit.setError(error);

                emit readyRead(unit);
                emit rawData(m_buffer, false);
            }
            else
                qWarning() << tr("Принятые данные не соответствуют запрошенным.");
        }
        else
        {
            qWarning() << tr("Не совпадают контрольные суммы (расчитанная и принятая): %1/%2.").arg(crc_calculate).
                                                                                                arg(crc_receive);
        }
    }

    m_buffer.clear();
    m_request = CModBusDataUnit();
    m_timer_timeout_response->stop();
    m_timer_timeout_silence->start(m_interval_timeout_silence);
}
//-------------------
void CModBus::block()
{
    m_block = true;
}
//------------------------------------------
void CModBus::request(CModBusDataUnit& unit)
{
    if(!m_connect) // соединение неактивно
    {
        if(isBlock() || m_request.isValid()) // если передача заблокированна или запрос валидный, то на выход -
                                             // в очередь не ставим, т.к. нет смысла, пока не прошла синхронизация
            return;
    }

    if(isBlock())
    {
        qDebug() << tr("Добавление в очередь запроса: %1").arg(unit.toString());
        m_queue << unit;
        return;
    }

    block();

    QByteArray ba;

    ba.append(unit.id());
    ba.append(quint8(unit.function()));
    ba.append(quint8((unit.address() >> 8)&0xFF)); // MSB address
    ba.append(quint8(unit.address()&0xFF)); // LSB address

    switch(unit.function())
    {
        // ID, FUNCTION_CODE, ADDRESS REGISTER FIRST, REGISTER NUMBERS, CRC(2 bytes)
        case CModBusDataUnit::ReadHoldingRegisters:
        case CModBusDataUnit::ReadInputRegisters:
        // ID, FUNCTION_CODE, ADDRESS REGISTER FIRST, VALUE, CRC(2 bytes)
        case CModBusDataUnit::WriteSingleRegister:
            ba.append((unit[0] >> 8)&0xFF); // MSB register numbers
            ba.append(unit[0]&0xFF); // LSB register numbers
        break;

        // ID, FUNCTION_CODE, ADDRESS REGISTER FIRST, REGISTER NUMBERS, ...VALUES..., CRC(2 bytes)
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
            qWarning() << tr("Неизвестный код функции: %1.").arg(unit.toString());
            unblock();
        return;
    }

    quint16 crc = crc16(ba, ba.count());

    ba.append(crc&0xFF); // LSB crc
    ba.append((crc >> 8)&0xFF); // MSB crc

    m_channel->write(ba);
    m_request = unit;
    m_timer_timeout_response->start(m_interval_timeout_response);
    m_time_process.start();

    emit rawData(ba);
}
//---------------------
void CModBus::unblock()
{
    m_block = false;
}
//-------------------------------------------
void CModBus::sendData(CModBusDataUnit& unit)
{
    if(unit.isValid())
        request(unit);
}
//---------------------------------------------
void CModBus::setIntervalResponce(int interval)
{
    m_interval_timeout_response = interval;
}
//--------------------------------------------
void CModBus::setIntervalSilence(int interval)
{
    m_interval_timeout_silence = interval;
}
//----------------------------------
void CModBus::setTryCount(int count)
{
    m_trycount = count;
}
//--------------------------
void CModBus::disconnected()
{
    m_timer_timeout_response->stop();
    m_timer_timeout_silence->stop();
    m_request = CModBusDataUnit();
    m_queue.clear();
    m_connect = false;
    m_buffer.clear();
    unblock();
}
//-----------------------------
void CModBus::timeoutResponce()
{
    m_timer_timeout_response->stop();
    qDebug() << tr("Таймаут ответа: %1мс").arg(m_time_process.elapsed());
}
//-----------------------------
void CModBus::timeoutSilencce()
{
    m_timer_timeout_silence->stop();
    unblock();

    if(!isBlock() && !m_queue.isEmpty()) // опрос очереди
    {
        CModBusDataUnit unit(m_queue.takeFirst());
        qDebug() << tr("Извлечение из очереди запроса: %1.").arg(unit.toString());
        request(unit); // отправка первого запроса из очереди
    }
}
