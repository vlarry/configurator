#include "cjournal.h"
//-----------------------------
bool CJournal::m_state = false;
//-------------------
CJournal::CJournal():
    m_shift_prt(0),
    m_size_msg(0),
    m_size_request(0),
    m_count_msg(0),
    m_count_limit(0),
    m_count_total(0),
    m_address_shift(0),
    m_address_number(0),
    m_buffer(data_t(0)),
    m_widget(nullptr)
{

}
//----------------------------------------------------------------------------------------------------------------------
CJournal::CJournal(int shift_ptr, int size_msg, int size_request, int addr_shift, int addr_num, CJournalWidget* widget):
    m_shift_prt(shift_ptr),
    m_size_msg(size_msg),
    m_size_request(size_request),
    m_count_msg(0),
    m_count_limit(0),
    m_count_total(0),
    m_address_shift(addr_shift),
    m_address_number(addr_num),
    m_buffer(data_t(0)),
    m_widget(widget)
{

}
//-------------------
CJournal::~CJournal()
{
    m_widget = nullptr;
}
//----------------------
int CJournal::shiftPrt()
{
    return m_shift_prt;
}
//---------------------
int CJournal::sizeMsg()
{
    return m_size_msg;
}
//-------------------------
int CJournal::sizeRequest()
{
    return m_size_request;
}
//----------------------------------------------------
CModBusDataUnit::vlist_t &CJournal::CJournal::buffer()
{
    return m_buffer;
}
//--------------------------------
CJournalWidget *CJournal::widget()
{
    return m_widget;
}
//-------------------------------------------------------------
void CJournal::appendData(const CModBusDataUnit::vlist_t &data)
{
    m_buffer += data;
}
//---------------------------------------
void CJournal::setShiftPtr(int shift_ptr)
{
    m_shift_prt = shift_ptr;
}
