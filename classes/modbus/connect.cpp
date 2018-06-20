#include "connect.h"
//---------------------------------------------------
CConnect::baudrate_list_t CConnect::m_baudrate_list =
{
    QSerialPort::Baud1200, QSerialPort::Baud2400, QSerialPort::Baud4800, QSerialPort::Baud9600, QSerialPort::Baud19200,
    QSerialPort::Baud38400, QSerialPort::Baud57600, QSerialPort::Baud115200
};
//----------------------------------------
CConnect::error_t CConnect::m_error_list =
{
    { QSerialPort::NoError, QObject::tr("Ошибок нет.") },
    { QSerialPort::DeviceNotFoundError, QObject::tr("Произошла ошибка при попытке открыть несуществующее устройство.") },
    { QSerialPort::PermissionError, QObject::tr("Произошла ошибка при попытке открыть уже открытое устройство "
                                                "другим процессом.") },
    { QSerialPort::OpenError, QObject::tr("Произошла ошибка при попытке открыть уже открытое устройство в этом объекте.") },
    { QSerialPort::NotOpenError, QObject::tr("Эта ошибка возникает, когда выполняется операция, которая может быть успешно "
                                             "выполнена только в том случае, если устройство открыто.") },
    { QSerialPort::ParityError, QObject::tr("Ошибка четности, обнаруженная аппаратными средствами при чтении данных.") },
    { QSerialPort::FramingError, QObject::tr("Ошибка кадрирования, обнаруженная аппаратным обеспечением при чтении данных.") },
    { QSerialPort::BreakConditionError, QObject::tr("Условие прерывания, обнаруженное оборудованием на входной линии.") },
    { QSerialPort::WriteError, QObject::tr("При записи данных произошла ошибка ввода-вывода.") },
    { QSerialPort::ReadError, QObject::tr("При чтении данных произошла ошибка ввода-вывода.") },
    { QSerialPort::ResourceError, QObject::tr("Ошибка ввода-вывода произошла, когда ресурс становится недоступным, например, "
                                              "когда устройство неожиданно удалено из системы.") },
    { QSerialPort::UnsupportedOperationError, QObject::tr("Запрашиваемая операция устройства не поддерживается или запрещена "
                                                          "работающей операционной системой.") },
    { QSerialPort::TimeoutError, QObject::tr("Произошла ошибка тайм-аута.") },
    { QSerialPort::UnknownError, QObject::tr("Произошла неопознанная ошибка.") }
};
//----------------------------------
CConnect::CConnect(QObject *parent):
    QObject(parent),
    m_port(nullptr),
    m_settings({ "", QSerialPort::Baud9600, QSerialPort::Data8, QSerialPort::EvenParity, QSerialPort::OneStop }),
    m_autochoicespeed(false)
{
    m_port = new QSerialPort(this);

    connect(m_port, &QSerialPort::bytesWritten, this, &CConnect::endWrite);
    connect(m_port, &QSerialPort::errorOccurred, this, &CConnect::errorOccuredPort);
    connect(m_port, &QSerialPort::readyRead, this, &CConnect::readyReadData);
}
//-------------------
CConnect::~CConnect()
{
    if(m_port)
    {
        if(m_port->isOpen())
            m_port->close();

        delete m_port;
        m_port = nullptr;
    }
}
//------------------------------------
bool CConnect::autochoicespeed() const
{
    return m_autochoicespeed;
}
//--------------------------------------------------
QSerialPort::BaudRate CConnect::baudrateNext() const
{
    for(int i = 0; i < m_baudrate_list.count(); i++)
    {
        if(m_baudrate_list[i] == m_settings.baudrate)
        {
            if(i == (m_baudrate_list.count() - 1))
                return m_baudrate_list[0];

            return m_baudrate_list[i + 1];
        }
    }

    return m_settings.baudrate;
}
//---------------------------
bool CConnect::isOpen() const
{
    return m_port->isOpen();
}
//-------------------------------------------
void CConnect::setAutochoicespeed(bool state)
{
    m_autochoicespeed = state;
}
//--------------------
void CConnect::close()
{
    if(m_port->isOpen())
    {
        m_port->close();
        emit stateChanged();
    }
    else
        qDebug() << tr("Нельзя закрыть уже закрытый порт.");
}
//----------------------------------
void CConnect::endWrite(qint64 size)
{
    qDebug() << tr("В буфер последовательного порта загружены данные: %1 байт.").arg(size);
}
//-----------------------------------------------------------------
void CConnect::errorOccuredPort(QSerialPort::SerialPortError error)
{
    QString msg_str = tr("Ошибка последовательного порта: %1.").arg(m_error_list[error]);
    qWarning() << msg_str;
    emit portError(msg_str);
}
//-------------------
void CConnect::open()
{
    if(m_port->isOpen())
        m_port->close();

    m_port->setPortName(m_settings.name);
    m_port->setBaudRate(m_settings.baudrate);
    m_port->setDataBits(m_settings.databits);
    m_port->setStopBits(m_settings.stopbits);
    m_port->setParity(m_settings.parity);

    if(!m_port->open(QSerialPort::ReadWrite))
    {
        m_port->close();
        qCritical() << tr("Невозможно открыть последовательный порт: %1.").arg(m_port->errorString());
        return;
    }

    emit stateChanged(true);
}
//----------------------------
void CConnect::readyReadData()
{
    QByteArray ba = m_port->readAll();
    emit readyRead(ba);
}
//------------------------------------------------------------
void CConnect::setSettings(CConnect::SerialPortType& settings)
{
    m_settings = settings;
}
//--------------------------------------------------------
const CConnect::SerialPortType& CConnect::settings() const
{
    return m_settings;
}
