#include "cmodbus.h"
//--------------------------------
CModbus::CModbus(QObject *parent):
    QObject(parent),
    m_port_name(tr("")),
    m_baudrate(115200),
    m_data_bits(QSerialPort::Data8),
    m_stop_bits(QSerialPort::OneStop),
    m_parity(QSerialPort::EvenParity),
    m_blocking_send(false),
    m_timeout_timer(nullptr),
    m_timeout_repeat(1000),
    m_request_count_repeat(3),
    m_counter_request_error(0),
    m_sizeQuery(0)
{
    m_device = new QSerialPort(this);
    m_timeout_timer = new QTimer(this);
    
    m_device->setPortName(m_port_name);
    m_device->setBaudRate(m_baudrate);
    m_device->setDataBits(m_data_bits);
    m_device->setStopBits(m_stop_bits);
    m_device->setParity(m_parity);
    
    connect(m_device, &QSerialPort::readyRead, this, &CModbus::readyRead);
    connect(m_device, &QSerialPort::errorOccurred, this, &CModbus::errorPort);
    connect(m_timeout_timer, &QTimer::timeout, this, &CModbus::timeout);
}
//-----------------
CModbus::~CModbus()
{
    if(m_timeout_timer->isActive())
        m_timeout_timer->stop();
    
    delete m_timeout_timer;
    m_timeout_timer = nullptr;
    
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
//----------------------------------------
void CModbus::setBaudrate(qint32 baudrate)
{
    m_baudrate = baudrate;
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
    
    emit infoLog(tr("Последовательный порт <") + m_device->portName() + tr("> открыт\n"));
}
//------------------------------
void CModbus::disconnectDevice()
{
    if(m_device->isOpen())
    {
        m_device->close();
        
        m_counter_request_error = 0;
        m_receive_buffer.clear();
        m_request_queue.clear();
        m_request_cur = CDataUnitType();
        m_timeout_timer->stop();
        unblock();
        
        emit connectDeviceState(false);
        emit infoLog(tr("Последовательный порт <") + m_device->portName() + tr("> закрыт\n"));
    }
}
//----------------------------------------
void CModbus::request(CDataUnitType& unit)
{
    if(unit.is_empty())
        return;
    
    quint16 size;
    
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
    
    if(is_bloking() && m_counter_request_error == 0) // передача блокированна и не было ошибок -> все запросы в очередь
    {
        m_request_queue.append(unit);
        m_sizeQuery++;
        
        return;
    }
    
    block(); // блокируем передачу
    
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
    m_device->write(ba);
    
    emit rawData(ba);
}
//-----------------------
void CModbus::readyRead()
{
    if(m_request_cur.is_empty())
        return;
    
    m_receive_buffer += m_device->readAll();
    
    quint16 count = 0;
    
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
        count = 8;
    
    if(count != m_receive_buffer.count() && count > m_receive_buffer.count()) 
    // сообщение передается в однобайтовых значениях + 
    // 5 байт накладные расхорды
    {
        return;
    }
    else if(count < m_receive_buffer.count())
    {
        emit infoLog(tr("Получено больше, чем ожидалось (") + QString::number(count) + " < " + 
                     QString::number(m_receive_buffer.count()) + tr("). Данные обрезаны до размера: ") + 
                     QString::number(count) + tr(" байт"));
        
        m_receive_buffer = m_receive_buffer.remove(count, (m_receive_buffer.count() - count));
    }
    
    m_timeout_timer->stop();
    m_counter_request_error = 0;
    
    // приняли все сообщение
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
        
        QString error = tr("Ошибка контрольной суммы->принято(") + str1 + tr("), рассчитано(") + str2 + tr(")");
        
        emit errorDevice(error);
        emit infoLog(error + QString("\n"));
        
        unblock();
        process_request_queue();
        
        return;
    }
    
    CDataUnitType unit;
    
    unit.setSlaveID(data.at(0));
    unit.setFunctionType((CDataUnitType::FunctionType)data.at(1));
    
    QVector<quint16> values;
    
    for(quint8 i = 3; i < data.count() - 1; i += 2)
    {
        quint8 mbs = data.at(i);
        quint8 lbs = data.at(i + 1);
        
        values.append((quint16)(mbs << 8) | lbs);
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
//---------------------
void CModbus::timeout()
{
    m_timeout_timer->stop();
    
    m_counter_request_error++;
    m_receive_buffer.clear();
    
    if(m_counter_request_error == m_request_count_repeat)
    {
        disconnectDevice();
    }
    else
    {
        unblock();
        emit errorDevice(tr("Ошибка: время ожидания ответа от устройства истекло -> попытка №") + 
                         QString::number(m_counter_request_error));
        request(m_request_cur);
    }
}
//-----------------------------------
void CModbus::process_request_queue()
{
    if(!m_request_queue.isEmpty() && !is_bloking()) // очередь не пуста и не передается очередной запрос
    {
        CDataUnitType unit = m_request_queue.takeAt(0); // получение из очереди очередного запроса
        
        if(!unit.is_empty()) // если запрос не пустой
        {
            request(unit);
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
    m_blocking_send = true;
}
//---------------------
void CModbus::unblock()
{
    m_blocking_send = false;
}
//------------------------------
bool CModbus::is_bloking() const
{
    return m_blocking_send;
}