#include "cjournal.h"
//--------------------------------------------------------------------------------------------------------------------
CJournal::CJournal(int msg_size, int addr_msg_count, int addr_shift_ptr, int page_addr_start, CJournalWidget *widget):
    m_msg_size(msg_size),
    m_addr_msg_count(addr_msg_count),
    m_addr_shift_ptr(addr_shift_ptr),
    m_page_addr_start(page_addr_start),
    m_msg_on_page(PAGE_SIZE/m_msg_size),
    m_shift_ptr_cur(0),
    m_msg_count(0),
    m_msg_cur(0),
    m_msg_read(0),
    m_filter(CFilter()),
    m_widget(widget),
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
int CJournal::msgCurrent() const
{
    return m_msg_cur;
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

}
/*!
 * \brief CJournal::read
 * \return unit запроса
 *
 * Возвращает запрос на чтение сообщений
 */
CModBusDataUnit CJournal::read()
{

}
/*!
 * \brief CJournal::receiver
 * \param data данные сообщения
 *
 * Приемник сообщений и их вывод в журнал
 */
void CJournal::receiver(CModBusDataUnit::cell_t &data)
{

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

}
