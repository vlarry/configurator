#include "modbus.h"
//--------------------------------
CModBus::CModBus(QObject* parent):
    QObject(parent),
    m_channel(nullptr),
    m_request(CModBusDataUnit()),
    m_queue(queue_t(0)),
    m_connect(false),
    m_block(false),
    m_is_autochoicespeed(false),
    m_interval_timeout_response(1000),
    m_interval_timeout_silence(4),
    m_trycount(3),
    m_try_counter(0),
    m_timer_timeout_response(nullptr),
    m_timer_timeout_silence(nullptr)
{
    m_channel                = new CConnect(this);
    m_timer_timeout_response = new QTimer(this);
    m_timer_timeout_silence  = new QTimer(this);

    connect(m_channel, &CConnect::readyRead, this, &CModBus::readyReadData);
    connect(m_channel, &CConnect::stateChanged, this, &CModBus::stateChanged);
    connect(m_channel, &CConnect::portError, this, &CModBus::errorChannel);
    connect(this, &CModBus::open, m_channel, &CConnect::open);
    connect(this, &CModBus::close, m_channel, &CConnect::close);
    connect(this, &CModBus::close, this, &CModBus::disconnected);
    connect(this, &CModBus::autochoicespeed, this, &CModBus::autochoicespeedProcess);
    connect(m_timer_timeout_response, &QTimer::timeout, this, &CModBus::timeoutResponce);
    connect(m_timer_timeout_silence, &QTimer::timeout, this, &CModBus::timeoutSilencce);
}
//--------------------------
CConnect* CModBus::channel()
{
    return m_channel;
}
//-------------------------------
bool CModBus::isConnected() const
{
    return m_connect;
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

    int size = 0, offset = 0;

    QString str = bytes.toHex(' ').toUpper().data();
    qDebug() << tr("Принятые данные: %1").arg(str);

    CModBusDataUnit::FunctionType code_function = CModBusDataUnit::FunctionType(quint8(m_buffer[1]));

    switch(code_function)
    {
        // ID, FUNCTION_CODE, BYTE NUMBERS, ...VALUES..., CRC(2 bytes)
        case CModBusDataUnit::ReadHoldingRegisters:
        case CModBusDataUnit::ReadInputRegisters:
            size   = static_cast<quint8>(m_request[0])*2 + 5;
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
            if(static_cast<int>(code_function) & 0x80) // в ответе устройства обнаружена ошибка
            {
                size = 5;
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
        {
            m_connect = true;

            if(m_baudrate_init != m_channel->settings().baudrate)
            {
                m_baudrate_init = m_channel->settings().baudrate;
                emit baudrateChanged(m_baudrate_init);
            }
        }

        qDebug() << tr("Данные приняты в полном объеме %1 байт за %2мс.").arg(m_buffer.count()).arg(m_time_process.elapsed());

        // расчет и проверка контрольной суммы
        quint8 mbs = static_cast<quint8>(m_buffer[m_buffer.count() - 2]);
        quint8 lbs = static_cast<quint8>(m_buffer[m_buffer.count() - 1]);

        quint16 crc_receive = (static_cast<quint16>(lbs << 8)) | mbs;
        quint16 crc_calculate = crc16(m_buffer, static_cast<size_t>(m_buffer.count() - 2));

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
        emit noConnection();

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

    ba.append(static_cast<char>(unit.id()));
    ba.append(static_cast<char>(unit.function()));
    ba.append(static_cast<char>((unit.address() >> 8)&0xFF)); // MSB address
    ba.append(static_cast<char>(unit.address()&0xFF)); // LSB address

    switch(unit.function())
    {
        // ID, FUNCTION_CODE, ADDRESS REGISTER FIRST, REGISTER NUMBERS, CRC(2 bytes)
        case CModBusDataUnit::ReadHoldingRegisters:
        case CModBusDataUnit::ReadInputRegisters:
        // ID, FUNCTION_CODE, ADDRESS REGISTER FIRST, VALUE, CRC(2 bytes)
        case CModBusDataUnit::WriteSingleRegister:
            ba.append(static_cast<char>((unit[0] >> 8)&0xFF)); // MSB register numbers
            ba.append(static_cast<char>(unit[0]&0xFF)); // LSB register numbers
        break;

        // ID, FUNCTION_CODE, ADDRESS REGISTER FIRST, REGISTER NUMBERS, BYTE NUMBERS, ...VALUES..., CRC(2 bytes)
        case CModBusDataUnit::WriteMultipleRegisters:
            ba.append(static_cast<char>((unit.count() >> 8)&0xFF)); // MSB register numbers
            ba.append(static_cast<char>(unit.count()&0xFF)); // LSB register numbers

            ba.append(static_cast<char>(unit.count()*2)); // byte numbers

            for(int i = 0; i < unit.count(); i++)
            {
                ba.append(static_cast<char>((unit[i] >> 8)&0xFF)); // MSB data value
                ba.append(static_cast<char>(unit[i]&0xFF)); // LSB data value
            }
        break;

        default:
            qWarning() << tr("Неизвестный код функции: %1.").arg(unit.toString());
            unblock();
        return;
    }

    quint16 crc = crc16(ba, static_cast<size_t>(ba.count()));

    ba.append(static_cast<char>(crc&0xFF)); // LSB crc
    ba.append(static_cast<char>((crc >> 8)&0xFF)); // MSB crc

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
/*!
 * \brief CModBus::sendData
 * \param unit Запрос
 * \param count Количество повторений запроса - по умолчанию один
 */
void CModBus::sendData(CModBusDataUnit& unit, int count)
{
    if(unit.isValid())
    {
        for(int i = 0; i < count; i++)
            request(unit); // отправляем запрос заданное количество раз
    }
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
//------------------------------------
void CModBus::autochoicespeedProcess()
{
    if(!m_is_autochoicespeed)
    {
        m_baudrate_init = m_channel->settings().baudrate;
        m_is_autochoicespeed = true;
    }

    QSerialPort::BaudRate baudrate = m_channel->baudrateNext();

    if(baudrate == m_baudrate_init)
    {
        disconnected();
        m_is_autochoicespeed = false;

        return;
    }

    CConnect::SerialPortType& settings = m_channel->settings();

    settings.baudrate = baudrate;

    emit open();
}
//--------------------------
void CModBus::disconnected()
{
    m_timer_timeout_response->stop();
    m_timer_timeout_silence->stop();
    m_request = CModBusDataUnit();
    m_queue.clear();
    m_try_counter = 0;
    m_buffer.clear();
    unblock();
}
//-----------------------------
void CModBus::timeoutResponce()
{
    m_timer_timeout_response->stop();
    qDebug() << tr("Таймаут ответа: %1мс").arg(m_time_process.elapsed());

    if(!m_channel->isOpen())
        return;

    if(m_try_counter < m_trycount)
    {
        m_try_counter++;

        QString msg_str;

        if(!m_is_autochoicespeed)
        {
            msg_str = tr("Время ожидания ответа от устройства %1мс истекло. Попытка №%2.").arg(m_time_process.elapsed()).
                                                                                          arg(m_try_counter);
        }
        else
        {
            msg_str = tr("Идет автоматический подбор скорости. Текущая скорость: %1 бод.").arg(m_channel->settings().baudrate);
        }

        emit errorDevice(msg_str);

        CModBusDataUnit unit(m_request);
        m_request = CModBusDataUnit();
        unblock();
        request(unit);
    }
    else
    {
        emit close();

        if(m_channel->autochoicespeed() && !m_connect)
            emit autochoicespeed();

        m_connect = false;
    }
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
//-------------------------------------
void CModBus::userStateCtrl(bool state)
{
    if(state)
        return;

    disconnected();
    m_is_autochoicespeed = false;
    m_connect            = false;
}
