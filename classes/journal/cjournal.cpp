#include "cjournal.h"
//-------------------
CJournal::CJournal():
    m_msg_size(-1),
    m_request_size(-1),
    m_request_last_count(-1),
    m_msg_read_on_page(-1),
    m_msg_read_count(-1),
    m_addr_page_start(-1),
    m_page_addr_cur(-1),
    m_addr_msg_num_r(-1),
    m_addr_page_ptr_rw(-1),
    m_msg_total_num(-1),
    m_msg_start_ptr(-1),
    m_msg_limit(-1),
    m_request_time(-1),
    m_page_limit(-1),
    m_is_clear(true),
    m_is_msg_read_state(false),
    m_is_msg_part(false),
    m_is_msg_print(true),
    m_msg_buffer(CModBusDataUnit::vlist_t(0)),
    m_data_buffer(CModBusDataUnit::vlist_t(0)),
    m_part_buffer(CModBusDataUnit::vlist_t(0)),
    m_widget(nullptr)
{

}
//-----------------------------------------------------------------------------------------------------------------------------------
CJournal::CJournal(int addr_page_start, int msg_size, int request_size, int addr_msg_num, int addr_page_ptr, CJournalWidget *widget):
    m_msg_size(msg_size),
    m_request_size(request_size),
    m_request_last_count(0),
    m_msg_read_on_page(0),
    m_msg_read_count(0),
    m_addr_page_start(addr_page_start),
    m_page_addr_cur(PAGE_SIZE),
    m_addr_msg_num_r(addr_msg_num),
    m_addr_page_ptr_rw(addr_page_ptr),
    m_msg_total_num(0),
    m_msg_start_ptr(0),
    m_msg_limit(0),
    m_request_time(0),
    m_page_limit(0),
    m_is_clear(true),
    m_is_msg_read_state(false),
    m_is_msg_part(false),
    m_is_msg_print(true),
    m_msg_buffer(CModBusDataUnit::vlist_t(0)),
    m_data_buffer(CModBusDataUnit::vlist_t(0)),
    m_part_buffer(CModBusDataUnit::vlist_t(0)),
    m_widget(widget)
{
    m_page_limit = m_addr_page_start + PAGE_SIZE;
}
/*!
 * \brief CJournal::addrMsgNum
 * \return значение адреса чтения доступных сообщений в устройстве
 */
int CJournal::addrMsgNum() const
{
    return m_addr_msg_num_r;
}
/*!
 * \brief CJournal::addrPagePtr
 * \return значение адреса чтения/записи указателя на текущую страницу (окно чтения)
 */
int CJournal::addrPagePtr() const
{
    return m_addr_page_ptr_rw;
}
/*!
 * \brief CJournal::addrPageStart
 * \return значение адреса начальной страницы
 */
int CJournal::addrPageStart() const
{
    return m_addr_page_start;
}
/*!
 * \brief CJournal::clear
 *
 * Очистка журнала
 */
void CJournal::clear()
{
    m_request_last_count = 0;
    m_msg_read_on_page = 0;
    m_msg_read_count = 0;
    m_page_addr_cur = PAGE_SIZE;
    m_msg_start_ptr = 0;
    m_request_time = 0;
    m_is_clear = true;
    m_is_msg_read_state = false;
    m_is_msg_part = false;
    m_msg_buffer = CModBusDataUnit::vlist_t(0);
    m_data_buffer = CModBusDataUnit::vlist_t(0);
    m_part_buffer = CModBusDataUnit::vlist_t(0);
}
/*!
 * \brief CJournal::filter
 * \return ссылка на фильтр
 *
 * Фильтр чтения данных
 */
CFilter &CJournal::filter()
{
    return m_filter;
}
/*!
 * \brief CJournal::isMsgPart
 * \return состояние флага использования части запроса
 */
bool CJournal::isMsgPart() const
{
    return m_is_msg_part;
}
/*!
 * \brief CJournal::isClear
 * \return флаг очистки журнала
 */
bool CJournal::isClear() const
{
    return m_is_clear;
}
/*!
 * \brief CJournal::msgBuffer
 * \return ссылка на буфер сообщений
 */
CModBusDataUnit::vlist_t &CJournal::msgBuffer()
{
    return m_msg_buffer;
}
/*!
 * \brief CJournal::msgReadOnPage
 * \return значение текущего сообщения на странице (локальный счетчик прочитанных сообщений на странице)
 */
int CJournal::msgReadOnPage() const
{
    return m_msg_read_on_page;
}
/*!
 * \brief CJournal::msgLimit
 * \return значение количества сообщений которое необходимо прочитать
 */
int CJournal::msgLimit() const
{
    return m_msg_limit;
}
/*!
 * \brief CJournal::msgReadCount
 * \return значение количества прочитанных сообщений (глобальный счетчик прочитанных сообщений)
 */
int CJournal::msgReadCount() const
{
    return m_msg_read_count;
}
/*!
 * \brief CJournal::msgSize
 * \return значение размера сообщения (количество ячеек, которое занимает сообщение)
 */
int CJournal::msgSize() const
{
    return m_msg_size;
}
/*!
 * \brief CJournal::msgStartPtr
 * \return значение указателя на сообщение с которого начинается чтение
 */
int CJournal::msgStartPtr() const
{
    return m_msg_start_ptr;
}
/*!
 * \brief CJournal::msgTotalNum
 * \return значение количества сообщений доступных для чтения
 */
int CJournal::msgTotalNum() const
{
    return m_msg_total_num;
}
/*!
 * \brief CJournal::nextPageAddr
 * \param флаг успешного вычисления адреса (возвращает ложь, если при расчете адреса необходимо сдигать окно чтения)
 * \return значение следующего адреса запроса
 *
 * Расчитывается адрес следующего запроса исходя из заданных параметров
 */
int CJournal::nextPageAddr(bool *isShift)
{
    if(m_addr_page_start == -1 || m_msg_read_on_page == -1)
        return -1;

    int page_addr = m_addr_page_start + m_msg_read_on_page*(m_msg_size/2);

    if(typeJournal() == "CRASH" && m_is_msg_part)
    {
        page_addr += 64;
    }

    if(isShift)
        *isShift = false;

    if(page_addr == m_page_limit)
    {
        page_addr = m_addr_page_start;
        m_msg_read_on_page = 0;
        m_page_addr_cur += 4096;
        *isShift = true;
    }

    return page_addr;
}
/*!
 * \brief CJournal::nextRequestSize
 * \return значение следующего размера запроса
 *
 * Расчитывается размер следующего запроса исходя из заданных параметров
 */
int CJournal::nextRequestSize()
{
    if(m_msg_limit == -1 || m_msg_read_count == -1 || (m_msg_limit < m_msg_read_count))
        return -1;

    int read_count = (m_msg_limit - m_msg_start_ptr) - m_msg_read_count;

    if(read_count > m_request_size) // если количество оставшихся сообщений больше максимального количества в одном запросе
        read_count = m_request_size; // то запрашиваем максимальное допустимое количество в одном запросе

    int msg_on_page_max = PAGE_SIZE/(m_msg_size/2); // максимальное количество сообщений на странице
    int msg_read_limit_on_page = msg_on_page_max - m_msg_read_on_page; // количество доступных сообщений на странице

    if(read_count > msg_read_limit_on_page) // количество сообщений доступных для чтения на одной странице меньше, чем расчетное
        read_count = msg_read_limit_on_page; // значит запрашиваем оставшиеся доступные сообщения на этой странице

    m_request_last_count = read_count; // последний запрос измеряемый в количестве запрашиваемых сообщений
    read_count *= m_msg_size/2; // получаем размер запроса в ячейках

    if(typeJournal() == "CRASH")
    {
        if(!m_is_msg_part)
        {
            read_count = 64;
            m_is_msg_part = true;
            m_request_last_count = 1;
        }
        else
        {
            read_count = 64;
            m_is_msg_part = false;
            m_request_last_count = 1;
        }
    }

    return read_count;
}
/*!
 * \brief CJournal::pageAddrCur
 * \return значение адреса текущей страницы
 */
int CJournal::pageAddrCur() const
{
    return m_page_addr_cur;
}
/*!
 * \brief CJournal::PAGE_LIMIT
 * \return предел (граница) окна чтения
 */
int CJournal::pageLimit() const
{
    return m_page_limit;
}
/*!
 * \brief CJournal::print
 *
 * Вывод данных в таблицу
 */
void CJournal::print(const CModBusDataUnit &unit)
{
    CModBusDataUnit::vlist_t data = unit.values();

    if(data.isEmpty() || m_request_last_count <= 0)
        return;

    if(typeJournal() == "CRASH")
    {
        if(m_is_msg_part)
        {
            m_part_buffer = data;
            return;
        }
        else
        {
            m_msg_buffer = m_part_buffer;
            m_part_buffer.clear();
        }
    }

    m_msg_buffer += data;

    int read_count = (m_msg_buffer.count()/(m_msg_size/2));

    if(read_count != m_request_last_count)
    {
        return;
    }

    m_request_time += unit.elapsed();
    m_msg_read_count += m_request_last_count;
    m_msg_read_on_page += m_request_last_count;

    if(m_msg_read_count == m_msg_limit - m_msg_start_ptr) // чтение окончено
    {
        if(!m_is_msg_print && !m_data_buffer.isEmpty()) // если данные сохранялись в буфере, то выводим их
        {
            m_data_buffer += m_msg_buffer;

            if(m_widget)
            {
                m_widget->print(m_data_buffer);
            }

            m_data_buffer.clear();
        }

        m_is_msg_read_state = false;
    }

    if(m_widget)
    {
        if(m_is_msg_print)
            m_widget->print(m_msg_buffer);
        else
            m_data_buffer += m_msg_buffer;

        float speed_kb = float(m_request_last_count*m_msg_size/1024.0f)/float(unit.elapsed()/1000.0f);
        m_widget->header()->setTextElapsedTime(QString("%1 КБ/сек.").arg(QLocale::system().toString(speed_kb, 'f', 3)));
    }

    m_msg_buffer.clear();
}
/*!
 * \brief CJournal::requestSize
 * \return значение размера запроса по умолчанию (содержит количество сообщений в одном запросе)
 */
int CJournal::requestSize() const
{
    return m_request_size;
}
/*!
 * \brief CJournal::dataBuffer
 * \return ссылка на буфер сообщений
 */
const CModBusDataUnit::vlist_t &CJournal::dataBuffer() const
{
    return m_data_buffer;
}
/*!
 * \brief CJournal::readState
 * \return состояние чтения
 */
bool CJournal::isMsgReadState() const
{
    return m_is_msg_read_state;
}
/*!
 * \brief CJournal::isMsgPrint
 * \return флаг вывода сообщений в таблицу при чтении
 */
bool CJournal::isMsgPrint() const
{
    return m_is_msg_print;
}
/*!
 * \brief CJournal::typeJournal
 * \return тип журнала (CRASH, EVENT or HALFHOUR)
 */
QString CJournal::typeJournal()
{
    return m_widget->property("TYPE").toString();
}
/*!
 * \brief CJournal::widget
 * \return указатель на виджет журнала
 */
CJournalWidget *CJournal::widget() const
{
    return m_widget;
}
/*!
 * \brief CJournal::appendMsgToBuffer
 * \param msg_buf данные сообщения для добавления в буфер
 */
void CJournal::appendMsgToBuffer(CModBusDataUnit::vlist_t &msg_buf)
{
    m_msg_buffer += msg_buf;
}
/*!
 * \brief CJournal::setAddrMsgNum
 * \param value значение адреса чтения доступных сообщений в устройстве
 */
void CJournal::setAddrMsgNum(int value)
{
    m_addr_msg_num_r = value;
}
/*!
 * \brief CJournal::setAddrPagePtr
 * \param value значение адреса чтения/записи указателя на текущую страницу (окно чтения)
 */
void CJournal::setAddrPagePtr(int value)
{
    m_addr_page_ptr_rw = value;
}
/*!
 * \brief CJournal::setAddrPageStart
 * \param value значение адреса начальной страницы
 */
void CJournal::setAddrPageStart(int value)
{
    m_addr_page_start = value;
}
/*!
 * \brief CJournal::setFilter
 * \param filter Фильтр чтения данных
 */
void CJournal::setFilter(CFilter &filter)
{
    m_filter = filter;
}
/*!
 * \brief CJournal::setMsgOnPage
 * \param value значение текущего сообщения на странице (локальный счетчик прочитанных сообщений на странице)
 */
void CJournal::setMsgOnPage(int value)
{
    m_msg_read_on_page = value;
}
/*!
 * \brief CJournal::setMsgLimit
 * \param value значение количества сообщений которое необходимо прочитать
 */
void CJournal::setMsgLimit(int value)
{
    m_msg_limit = value;
}
/*!
 * \brief CJournal::setMsgPart
 * \param state состояние флага части запроса
 */
void CJournal::setMsgPart(bool state)
{
    m_is_msg_part = state;
}
/*!
 * \brief CJournal::setMsgReadCount
 * \param value значение количества прочитанных сообщений (глобальный счетчик прочитанных сообщений)
 */
void CJournal::setMsgReadCount(int value)
{
    m_msg_read_count = value;
}
/*!
 * \brief CJournal::setMsgSize
 * \param value значение размера сообщения (количество ячеек, которое занимает сообщение)
 */
void CJournal::setMsgSize(int value)
{
    m_msg_size = value;
}
/*!
 * \brief CJournal::setMsgStartPtr
 * \param value значение указателя на сообщение с которого начинается чтение
 */
void CJournal::setMsgStartPtr(int value)
{
    m_msg_start_ptr = value;
}
/*!
 * \brief CJournal::setMsgTotalNum
 * \param value значение количества сообщений доступных для чтения
 */
void CJournal::setMsgTotalNum(int value)
{
    m_msg_total_num = value;

    CHeaderJournal* header = m_widget->header();

    if(header)
        header->setTextDeviceCountMessages(0, m_msg_total_num);
}
/*!
 * \brief CJournal::setPageAddrCur
 * \param value значение адреса текущей страницы
 */
void CJournal::setPageAddrCur(int value)
{
    m_page_addr_cur = value;
}
/*!
 * \brief CJournal::setRequestSize
 * \param value значение размера запроса по умолчанию (содержит количество сообщений в одном запросе)
 */
void CJournal::setRequestSize(int value)
{
    m_request_size = value;
}
/*!
 * \brief CJournal::setState
 * \param state состояние чтения (активно или нет)
 */
void CJournal::setMsgReadState(bool state)
{
    m_is_msg_read_state = state;

    if(state)
        m_is_clear = false;
}
/*!
 * \brief CJournal::setMsgPrintState
 * \param state состояние флага вывода сообщений в таблицу при чтении
 */
void CJournal::setMsgPrintState(bool state)
{
    m_is_msg_print = state;
}
/*!
 * \brief CJournal::widget
 * \param widget указатель на виджет журнала
 */
void CJournal::widget(CJournalWidget *widget)
{
    m_widget = widget;
}