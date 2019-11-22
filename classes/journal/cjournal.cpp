#include "cjournal.h"
//--------------------------------------------------------------------------------------------------------------------
CJournal::CJournal(int msg_size, int addr_msg_count, int addr_shift_ptr, int page_addr_start, CJournalWidget *widget):
    m_msg_size(msg_size),
    m_addr_msg_count(addr_msg_count),
    m_addr_shift_ptr(addr_shift_ptr),
    m_page_addr_start(page_addr_start),
    m_msg_on_page(PAGE_SIZE/m_msg_size),
    m_default_reques_size((m_msg_size < 127)?127/m_msg_size:1), // 127 ячеек делим на размер сообщения в ячейках
    m_shift_ptr_cur(0),
    m_msg_count(0),
    m_msg_to_shift(0),
    m_msg_read(0),
    m_filter(CFilter()),
    m_widget(widget),
    m_buffer(CModBusDataUnit::vlist_t()),
    m_isRead(false),
    m_isShiftPrt(false)
{

}
//---------------------------
int CJournal::msgSize() const
{
    return m_msg_size;
}
//--------------------------------
int CJournal::addrMsgCount() const
{
    return m_addr_msg_count;
}
//--------------------------------
int CJournal::addrShiftPtr() const
{
    return m_addr_shift_ptr;
}
//---------------------------------
int CJournal::pageAddrStart() const
{
    return m_page_addr_start;
}
//-----------------------------
int CJournal::msgOnPage() const
{
    return m_msg_on_page;
}
//-------------------------------------
int CJournal::defaultRequesSize() const
{
    return m_default_reques_size;
}
//-------------------------------
int CJournal::shiftPtrCur() const
{
    return m_shift_ptr_cur;
}
//----------------------------
int CJournal::msgCount() const
{
    return m_msg_count;
}
//------------------------------
int CJournal::msgToShift() const
{
    return m_msg_to_shift;
}
//---------------------------
int CJournal::msgRead() const
{
    return m_msg_read;
}
//-------------------------
CFilter &CJournal::filter()
{
    return m_filter;
}
//--------------------------------
CJournalWidget *CJournal::widget()
{
    return m_widget;
}
/*!
 * \brief CJournal::initRead
 *
 * Инициализация чтениия журнала
 */
void CJournal::initRead()
{
    reset(); // сбос настроек журнала

    int msg_start = m_filter.limitLowValue();

    m_msg_to_shift = m_msg_on_page - msg_start;
    m_msg_read = m_filter.limitUpperValue() - msg_start;

    if(msg_start != 0) // если сообщения не читаются сначала - расчет смещения указателя окна
    {
        if(msg_start >= m_msg_on_page) // сообщение с которого начинается чтение находится не на первой странице
        {
            m_shift_ptr_cur = msg_start/m_msg_on_page*(PAGE_SIZE*2); // заносим адрес в указатель смещения окна чтения
            m_msg_to_shift = m_msg_on_page - (msg_start%m_msg_on_page); // заносим количество сообщений до конца страницы
        }

        m_msg_to_shift++; // сообщения читаются не с начала, значит смещаемся на одно сообщение к началу, чтобы захватить начальное
        m_msg_read++; // увеличиваем количество сообщений на одно, чтобы завхатить верхний предел
    }

    m_isRead = true; // устанавливаем флаг чтения активным
}
/*!
 * \brief CJournal::read
 * \param id адрес устройства
 * \param type тип запроса
 * \return unit запроса
 *
 * Возвращает запрос на чтение сообщений
 */
CModBusDataUnit CJournal::read(int id, int type, bool *isShift)
{
    *isShift = false;

    if(m_msg_to_shift == 0) // дочитали до конца страницы - переводим указатель
    {
        m_shift_ptr_cur += PAGE_SIZE*2;
        m_msg_to_shift = m_msg_on_page;

        *isShift = true;
    }

    int msg_size = m_msg_size;

    if(msg_size == 128) // размер сообщения равен максимальному размеру запроса в протоколе (128 ячеек = 256 байт),
        msg_size = 64; // то разбиваем на 2 части
    else
    {
        int request_size = m_default_reques_size;

        // количество сообщений в запросе по умолчанию больше, чем осталось прочитать на текущей странице
        if(m_default_reques_size > m_msg_to_shift)
        {
            request_size = m_msg_to_shift; // обрезаем размер запроса до максимального количества
        }

        if(request_size > m_msg_read - m_msg_count)
        {
            request_size = m_msg_read - m_msg_count;
        }

        msg_size = request_size*m_msg_size;
    }

    int page_addr = m_page_addr_start + (m_msg_on_page - m_msg_to_shift)*m_msg_size;

    // буфер сообщений не пустой и не кратный размеру сообщения и размер сообщения равен максимальному размеру
    if(!m_buffer.isEmpty() && m_buffer.count()%m_msg_size != 0 && m_msg_size == 128)
        page_addr += msg_size; // добавляем смещене к адресу в половину размера

    CModBusDataUnit unit(id, CModBusDataUnit::ReadInputRegisters, page_addr, msg_size);

    unit.setProperty("REQUEST", type);

    QVariant var;
    var.setValue<JournalPtr>(this);
    unit.setProperty("JOURNAL", var);

    return unit;
}
/*!
 * \brief CJournal::receiver
 * \param data данные сообщения
 *
 * Приемник сообщений
 */
void CJournal::receiver(const CModBusDataUnit::vlist_t &data)
{
    m_buffer += data;

    int count = data.count()/m_msg_size; // количество сообщений в полученных данных

    if(m_buffer.count()%m_msg_size == 0)
    {
        if(m_msg_size == 128 && count == 0)
            count = 1;

        m_msg_count += count;
        m_msg_to_shift -= count;

        m_widget->header()->setTextDeviceCountMessages(m_msg_count, m_filter.rangeMaxValue());
    }

    if(m_msg_count == m_msg_read) // дочитали до конца
        m_isRead = false;
}
/*!
 * \brief CJournal::isReadState
 * \return значение состояния чтения журнала
 *
 * Возвращает значение флага чтения журнала (очищается или устанавливается вручную)
 */
bool CJournal::isReadState() const
{
    return m_isRead;
}
/*!
 * \brief CJournal::isShiftPtr
 * \return значение флага сдвига окна чтения (true - необходимо сдвинуть окно)
 *
 * Возвращает значение флага сдвига окна чтения (очищается автоматически при сдвиге)
 */
bool CJournal::isShiftPtr() const
{
    return m_isShiftPrt;
}
/*!
 * \brief CJournal::setReadState
 * \param state значение состояния чтения
 *
 * Установка состояния чтения
 */
void CJournal::setReadState(bool state)
{
    m_isRead = state;
}
/*!
 * \brief CJournal::reset
 *
 * Сброс настроек журнала по умолчаниию
 */
void CJournal::reset()
{
    m_shift_ptr_cur = 0;
    m_msg_count = 0;
    m_msg_to_shift = 0;
    m_msg_read = 0;
    m_buffer.clear();
    m_isRead = false;
    m_isShiftPrt = false;
    m_filter.setType(CFilter::FilterLimitType);
}
//--------------------
void CJournal::print()
{
    if(!m_buffer.isEmpty())
        m_widget->print(m_buffer);
}
