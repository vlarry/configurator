#include "cmodbus.h"
//--------------------------------
CModbus::CModbus(QObject *parent):
    QObject(parent),
    m_device(nullptr),
    m_port_name(tr("")),
    m_baudrate(QSerialPort::Baud115200),
    m_data_bits(QSerialPort::Data8),
    m_stop_bits(QSerialPort::OneStop),
    m_parity(QSerialPort::EvenParity),
    m_blocking_send(false),
    m_timeout_timer(nullptr),
    m_send_wait_timer(nullptr),
    m_counter_request_error(0),
    m_request_count_repeat(3),
    m_timeout_repeat(1000),
    m_connect({ false, false, m_baudrate, 0, 0, QVector<QSerialPort::BaudRate>(0) })
{
    m_device          = new QSerialPort(this);
    m_timeout_timer   = new QTimer(this);
    m_send_wait_timer = new QTimer(this);
    
    m_device->setPortName(m_port_name);
    m_device->setBaudRate(m_baudrate);
    m_device->setDataBits(m_data_bits);
    m_device->setStopBits(m_stop_bits);
    m_device->setParity(m_parity);
    
    connect(m_device, &QSerialPort::readyRead, this, &CModbus::readyRead);
    connect(m_device, &QSerialPort::errorOccurred, this, &CModbus::errorPort);
    connect(m_timeout_timer, &QTimer::timeout, this, &CModbus::timeoutReadWait, Qt::DirectConnection);
    connect(m_send_wait_timer, &QTimer::timeout, this, &CModbus::sendRequestWait);
}
//-----------------
CModbus::~CModbus()
{
    if(m_timeout_timer->isActive())
        m_timeout_timer->stop();
    
    delete m_timeout_timer;
    m_timeout_timer = nullptr;

    if(m_send_wait_timer->isActive())
        m_send_wait_timer->stop();

    delete m_send_wait_timer;
    m_send_wait_timer = nullptr;
    
    if(m_device)
    {
        if(m_device->isOpen())
            m_device->close();
    }
    
    delete m_device;
    m_device = nullptr;
}
//--------------------------------------------
void CModbus::setPortName(const QString& name)
{
    m_port_name = name;
}
//-------------------------------------------------------
void CModbus::setBaudrate(QSerialPort::BaudRate baudrate)
{
    m_baudrate = m_connect.baudrate_init = baudrate;
}
//-------------------------------------------------------
void CModbus::setDatabits(QSerialPort::DataBits databits)
{
    m_data_bits = databits;
}
//-------------------------------------------------------
void CModbus::setStopbits(QSerialPort::StopBits stopbits)
{
    m_stop_bits = stopbits;
}
//-------------------------------------------------
void CModbus::setParity(QSerialPort::Parity parity)
{
    m_parity = parity;
}
//--------------------------------------------------------------------------------
void CModbus::setBaudrateList(const QVector<QSerialPort::BaudRate>& baudrate_list)
{
    m_connect.baudrate_list = baudrate_list;
}
//--------------------------------------
const QString &CModbus::portName() const
{
    return m_port_name;
}
//-------------------------------
quint32 CModbus::baudrate() const
{
    return m_baudrate;
}
//---------------------------------------------
QSerialPort::DataBits CModbus::databits() const
{
    return m_data_bits;
}
//---------------------------------------------
QSerialPort::StopBits CModbus::stopbits() const
{
    return m_stop_bits;
}
//-----------------------------------------
QSerialPort::Parity CModbus::parity() const
{
    return m_parity;
}
/*!
 * \brief CModbus::autospeed
 * \return Состояние автоподбора скорости
 */
bool CModbus::autospeed() const
{
    return m_autospeed;
}
//-------------------------------
void CModbus::clearQueueRequest()
{
    m_request_cur = CDataUnitType();
    m_request_queue.clear();
}
//-------------------------------------------------------------------
void CModbus::removeRequest(const QString& key, const QString& value)
{
    block();

    if(m_request_queue.isEmpty())
        return;

    QMutableVectorIterator<CDataUnitType> it(m_request_queue);

    while(it.hasNext())
    {
        QString val = it.next().property(key).toString();

        if(val == value)
            it.remove();
    }

    unblock();
}
//----------------------------------------
quint32 CModbus::requestQueueCount() const
{
    return m_request_queue.count();
}
//---------------------------
bool CModbus::is_open() const
{
    return m_device->isOpen();
}
//------------------------------------
quint16 CModbus::timeoutRepeat() const
{
    return m_timeout_repeat;
}
//----------------------------------------
quint8 CModbus::requestCountRepeat() const
{
    return m_request_count_repeat;
}
//------------------------------------------------
void CModbus::setTimeoutRepeat(quint16 newTimeout)
{
    m_timeout_repeat = newTimeout;
}
//--------------------------------------------------
void CModbus::setRequestCountRepeat(quint8 newCount)
{
    m_request_count_repeat = newCount;
}

quint32 CModbus::sizeQueue() const
{
    return m_sizeQuery;
}
/*!
 * \brief CModbus::setAutospeed
 * \param state Установка состояния автоподбора скорости - истина, значит подбираем
 */
void CModbus::setAutospeed(bool state)
{
    m_autospeed = state;
}
//---------------------------
void CModbus::connectDevice()
{
    m_device->setPortName(m_port_name);
    m_device->setBaudRate(m_baudrate);
    m_device->setDataBits(m_data_bits);
    m_device->setStopBits(m_stop_bits);
    m_device->setParity(m_parity);
    
    if(!m_device->open(QIODevice::ReadWrite))
    {
        m_device->close();
        
        emit errorDevice(m_device->errorString());
    }
    else
        emit connectDeviceState(true);
    
    qInfo() << tr("Последовательный порт <%1> открыт").arg(m_device->portName());
}
//------------------------------------------
void CModbus::disconnectDevice(bool isClear)
{
    if(m_device->isOpen())
    {
        m_device->close();
        
        m_counter_request_error = 0;
        m_receive_buffer.clear();

        if(isClear)
            m_request_queue.clear();

        m_request_cur = CDataUnitType();
        m_timeout_timer->stop();
        unblock();
        
        emit connectDeviceState(false);

        QString str = tr("Последовательный порт <%1> закрыт.").arg(m_device->portName());

        qInfo() << str;
    }

    if(m_connect.baud_reconnect && m_autospeed)
    {
        m_connect.baud_reconnect = false;

        for(int i = 0; i < m_connect.baudrate_list.count(); i++)
        {
            if(m_baudrate == m_connect.baudrate_list[i])
                emit baudrateChanged(i);
        }

        connectDevice();

        emit saveSettings();
    }
}
//----------------------------------------
void CModbus::request(CDataUnitType& unit)
{
    quint16 size;
#ifdef DEBUG_MODBUS
    qDebug() << QString("Запрос. Адрес: %1, размер данных: %2, тип запроса: %3").arg(unit.address()).arg(unit.valueCount()).
                                                                                 arg(unit.property("REQUEST").toInt());
#endif
    if(unit.functionType() == CDataUnitType::ReadHoldingRegisters || 
       unit.functionType() == CDataUnitType::ReadInputRegisters)
    {
        size = unit.value(0)*2;
    }
    else
    {
        size = unit.valueCount()*2;
    }
    
    if(size > 247) // максимальный размер запроса 252 байта - накладные расходы 
    {
        return;
    }
    
    QVector<quint8> tvalues;
    
    tvalues.append(unit.slaveID());
    tvalues.append((quint8)unit.functionType());
    
    quint16 addr = unit.address();
    
    tvalues.append((quint8)(addr >> 8));
    tvalues.append((quint8)(addr&0x00FF));
    
    if(unit.functionType() == CDataUnitType::WriteMultipleRegisters)
    {
        quint16 value = unit.valueCount();
        
        tvalues.append((quint8)(value >> 8));   // количество изменяемых ячеек - байт 1
        tvalues.append((quint8)(value&0x00FF)); // количество изменяемых ячеек - байт 2
        tvalues.append(size); // количество передаваемых байт
    }
    
    for(quint8 i = 0; i < unit.valueCount(); i++)
    {
        quint16 value = unit.value(i);
        
        tvalues.append((quint8)(value >> 8));
        tvalues.append((quint8)(value&0x00FF));
    }
    
    quint16 crc = CRC16(tvalues, tvalues.count());
    
    QByteArray ba;
    
    for(quint8 value: tvalues)
    {
        ba.append(value);
    }
    
    ba.append(crc&0x00FF);
    ba.append(crc >> 8);
    
    m_request_cur = unit;
    m_timeout_timer->start(m_timeout_repeat);

#ifdef DEBUG_MODBUS
    qDebug() << QString("Запрос: %1, адрес: %2, размер данных: %3").arg(m_request_cur.property("REQUEST").toInt()).
                                                                    arg(m_request_cur.address()).
                                                                    arg(m_request_cur.values().count());
#endif

    m_time.start();

    m_device->write(ba);

    emit rawData(ba);
}
//--------------------------------------------
void CModbus::sendRequest(CDataUnitType& unit)
{
    if(!m_device->isOpen())
    {
        if(!m_device->portName().isEmpty())
            emit errorDevice(tr("Порт <") + m_device->portName() + tr("> закрыт."));
        else
            emit noConnect();

        return;
    }

    if(unit.is_empty())
        return;

    if(!m_connect.is_connect && !m_request_cur.is_empty())
        return;
    else if(!m_connect.is_connect && m_request_cur.is_empty())
    {
        request(unit);

        return;
    }

    if(is_blocking() && m_counter_request_error == 0) // передача блокированна и не было ошибок -> все запросы в очередь
    {
        m_request_queue.append(unit);

#ifdef DEBUG_MODBUS
        qDebug() << QString("Добавление запроса в очередь. Запрос: %1, размер очереди: %2").
                    arg(unit.property("REQUEST").toInt()).arg(m_request_queue.count());
#endif

        return;
    }

    block(); // блокируем передачу

    m_request_send_wait = unit;

    m_send_wait_timer->start(10); // пауза 10мс
}
//-----------------------
void CModbus::readyRead()
{
    if(m_request_cur.is_empty())
        return;
    
    m_receive_buffer += m_device->readAll();
    
    quint16 count = 0;

#ifdef DEBUG_MODBUS
    qDebug() << QString("Ответ. Размер данных: %1").arg(m_receive_buffer.count());
#endif
    
    // Структура запроса для ReadHoldingRegisters и ReadInputRegisters:
    // ID | КОД ФУНКЦИИ | 1 БАЙТ - КОЛИЧЕСТВО ДАННЫХ ИДУЩИХ СЛЕДОМ | ДАННЫЕ | 2 БАЙТА CRC
    
    // Структура запроса для WriteSingleRegister:
    // ID | КОД ФУНКЦИИ | 2 БАЙТА АДРЕСА РЕГИСТРА | 2 БАЙТА ЗНАЧЕНИE | 2 БАЙТА CRC
    
    // Структура запроса для WriteMultipleRegisters:
    // ID | КОД ФУНКЦИИ | 2 БАЙТА АДРЕСА РЕГИСТРА | 2 БАЙТА КОЛИЧЕСТВО ЗАПИСАННЫХ ДАННЫХ | 2 БАЙТА CRC
    
    if(m_request_cur.functionType() == CDataUnitType::ReadHoldingRegisters || 
       m_request_cur.functionType() == CDataUnitType::ReadInputRegisters)
    {
        count = m_request_cur.value(0)*2 + 5;   
    }
    else
    {
        if(m_receive_buffer.count() == 5 && m_receive_buffer.at(1)&0x80) // если размер буфера 5 байт (в случае ошибки) и старший бит
                                                                         // второго байта выставлен
        {
            count = 5;
        }
        else
            count = 8;
    }

#ifdef DEBUG_MODBUS
    qDebug() << QString("Ответ. Ожидаемый размер данных: %1").arg(count);
#endif

    if(count != m_receive_buffer.count() && count > m_receive_buffer.count()) 
    // сообщение передается в однобайтовых значениях + 
    // 5 байт накладные расхорды
    {
#ifdef DEBUG_MODBUS
        qDebug() << QString("Ответ. Размер не соответствует ожидаемому %1/%2").arg(count).arg(m_receive_buffer.count());
#endif
        return;
    }
    else if(count < m_receive_buffer.count())
    {
#ifdef DEBUG_MODBUS
        qDebug() << QString("Ответ. Размер сообщения больше, чем ожидалось: %1/%2").arg(count).arg(m_receive_buffer.count());
#endif
        m_receive_buffer = m_receive_buffer.remove(count, (m_receive_buffer.count() - count));
    }
    
    m_timeout_timer->stop();
    m_counter_request_error = 0;

    // приняли все сообщение
    if(!m_connect.is_connect && m_autospeed)
    {
        m_connect.is_connect = true;

        if(m_connect.baudrate_init != m_baudrate)
        {
            m_baudrate = m_connect.baudrate_init;

            for(int i = 0; i < m_connect.baudrate_list.count(); i++)
            {
                if(m_baudrate == m_connect.baudrate_list[i])
                {
                    m_connect.baud_reconnect = true;
                    emit newBaudrate(i);
                }
            }
        }
    }

#ifdef DEBUG_MODBUS
    qDebug() << QString("Ответ. Данные получены в полном объеме: %1").arg(m_receive_buffer.count());
#endif
    // расчет и проверка контрольной суммы
    quint8 mbs = m_receive_buffer.at(m_receive_buffer.count() - 2);
    quint8 lbs = m_receive_buffer.at(m_receive_buffer.count() - 1);
    
    quint16 crc_receive = ((quint16)lbs << 8) | mbs;
    
    QVector<quint8> data;
    
    for(quint8 i = 0; i < count - 2; i++) // получаем целочиесленные однобайтовые данные без учета CRC
    {
        data.append(m_receive_buffer.at(i));
    }
    
    emit rawData(m_receive_buffer, false);
    m_receive_buffer.clear();
    
    quint16 crc_calculate = CRC16(data, data.count());
    
    if(crc_receive != crc_calculate)
    {
        QString str1 = (crc_receive <= 0x0FFF)?tr("0x0") + QString::number(crc_receive, 16).toUpper():
                                               tr("0x") + QString::number(crc_receive, 16).toUpper();
        QString str2 = (crc_calculate <= 0x0FFF)?tr("0x0") + QString::number(crc_calculate, 16).toUpper():
                                                 tr("0x") + QString::number(crc_calculate, 16).toUpper();
        
        QString func_type_str =((m_request_cur.functionType() == CDataUnitType::ReadHoldingRegisters)?tr("ReadHoldingRegisters"):
                                (m_request_cur.functionType() == CDataUnitType::ReadInputRegisters)?tr("ReadInputRegisters"):
                                (m_request_cur.functionType() == CDataUnitType::WriteSingleRegister)?tr("WriteSingleRegister"):
                                (m_request_cur.functionType() == CDataUnitType::WriteMultipleRegisters)?tr("WriteMultipleRegisters"):
                                                                                                        tr("Unknown"));
        QString error = tr("Ошибка контрольной суммы -> принято(%1), расчитано (%2): тип функции (%3): размер ожидаемых данных: "
                           "(%4) байт, получили: (%5) байт").
                           arg(str1).arg(str2).arg(func_type_str).arg(m_request_cur.valueCount()).arg(m_receive_buffer.count());

//        emit errorDevice(error);

#ifdef DEBUG_MODBUS
        qDebug() << QString("Ответ. %1").arg(error);
#endif
        unblock();
        process_request_queue();
        
        return;
    }

    QVector<quint16> values;

    for(quint8 i = 3; i < data.count() - 1; i += 2)
    {
        quint8 mbs = data.at(i);
        quint8 lbs = data.at(i + 1);
        
        values.append((quint16)(mbs << 8) | lbs);
    }

    CDataUnitType unit = m_request_cur;

    if(data.count() >= 3) // в случае ошибки возвращается 5 байт (id, функциональный код со старшим битом, байт ошибки и crc)
    {
        if((data[1]&0x80) == 0x80) // если установлен старший бит в функции, то ведомый сообщает об ошибке
        {
            unit.serErrorCode(data[2]);
#ifdef DEBUG_MODBUS
        qDebug() << QString("Ответ. Ошибка в сообщении: %1").arg(data[2]);
#endif
        }
    }

    unit.setValues(values);

    emit dataReady(unit);
    
    unblock();
    
    process_request_queue();
}
//---------------------------------------------------------
void CModbus::errorPort(QSerialPort::SerialPortError error)
{
    if(error != QSerialPort::NoError)
    {
        emit errorDevice(m_device->errorString());
    }
}
//-----------------------------
void CModbus::timeoutReadWait()
{
    m_timeout_timer->stop();
    
    m_counter_request_error++;
    m_receive_buffer.clear();
    
    if(m_counter_request_error == m_request_count_repeat)
    {
        disconnectDevice(false);

        bool isRepeat = false;

        if(!m_connect.is_connect && m_connect.index_start == 0 &&
                                    m_connect.index_current == 0 &&
                                    m_autospeed) // инициализация структуры подбора скорости
        {
            int index = 0;

            for(int i = 0; i < m_connect.baudrate_list.count(); i++)
            {
                // Поиск индекса скорости в списке иначе он будет равен 0, т.е. началу списка
                int br = m_connect.baudrate_list[i];

                if(br == m_baudrate && i <= (m_connect.baudrate_list.count() - 2))
                {
                    index = i + 1;
                    break;
                }
            }

            m_connect.index_start = m_connect.index_current = index;

            isRepeat = true;
        }
        else if(!m_connect.is_connect && (m_connect.index_current != 0 || m_connect.index_start != 0) && m_autospeed)
        {
            if(m_connect.index_current == m_connect.baudrate_list.count())
                m_connect.index_current = 0;

            if(m_connect.index_current == m_connect.index_start - 1)
            {
                emit baudrateChanged(m_connect.index_start - 1);

                m_connect.index_current = m_connect.index_start = 0;

                emit error(tr("Не удалось произвести подключение к устройству.\nПроверьте соединение."));

                return;
            }

            isRepeat = true;
        }

        if(isRepeat)
        {
            m_baudrate = m_connect.baudrate_list[m_connect.index_current++];

            emit baudrateChanged(m_connect.index_current - 1);

            connectDevice();
        }
    }
    else
    {
        unblock();

        QString str;

        if(m_connect.is_connect || !m_autospeed) // если соединение было активным, то значит обрыв
        {
            str = tr("Ошибка: время ожидания ответа от устройства истекло (%1 мс) -> попытка №%2").
                     arg(m_time.elapsed()).
                     arg(m_counter_request_error);
        }
        else if(!m_connect.is_connect && m_autospeed)
        {
            str = tr("Идет автоматический подбор скорости. Текущая скорость %1.").arg(m_baudrate);
        }

        emit errorDevice(str);

#ifdef DEBUG_MODBUS
    qDebug() << QString("Таймаут ожидания ответа. Запрос: %1, адрес: %2, размер данных: %3").
                                 arg(m_request_cur.property("REQUEST").toInt()).arg(m_request_cur.address()).
                                 arg(m_request_cur.values().count());
    qDebug() << QString("Повторная отправка запроса: ");
#endif
        request(m_request_cur);
    }
}
//-----------------------------
void CModbus::sendRequestWait()
{
    m_send_wait_timer->stop();

//    if(is_bloking())
//    {
//        m_request_queue.append(m_request_send_wait);
//        return;
//    }

    if(!m_request_send_wait)
        return;

    request(m_request_send_wait);

    m_request_send_wait = CDataUnitType();
}
//-----------------------------------
void CModbus::process_request_queue()
{
    if(!m_request_queue.isEmpty() && !is_blocking()) // очередь не пуста и не передается очередной запрос
    {
        CDataUnitType unit = m_request_queue.takeAt(0); // получение из очереди очередного запроса
#ifdef DEBUG_MODBUS
        qDebug() << QString("Очередь запросов не пуста.");
#endif
        if(!unit.is_empty()) // если запрос не пустой
        {
#ifdef DEBUG_MODBUS
        qDebug() << QString("Очередь запросов. Запрос: %1, адрес: %2, размер данных: %3").
                    arg(m_request_cur.property("REQUEST").toInt()).arg(m_request_cur.address()).
                    arg(m_request_cur.values().count());
#endif
            sendRequest(unit);
        }
    }
}
//----------------------------------------------------------
quint16 CModbus::CRC16(QVector<quint8> &data, quint8 length)
{
    quint16 crc = 0xFFFF; 
    
    for(quint8 i = 0; i < length; i++)
    { 
        crc ^= data.at(i);
        
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
//-------------------
void CModbus::block()
{
#ifdef DEBUG_MODBUS
    qDebug() << QString("Блокирование канала передачи.");
#endif
    m_blocking_send = true;
}
//---------------------
void CModbus::unblock()
{
#ifdef DEBUG_MODBUS
    qDebug() << QString("Разблокирование канала передачи.");
#endif
    m_blocking_send = false;
}
//------------------------------
bool CModbus::is_blocking() const
{
    return m_blocking_send;
}
