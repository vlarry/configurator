#include "cfilter.h"
//-----------------
CFilter::CFilter():
    m_interval({ 0, 0, 0 }),
    m_date({ QDate::currentDate(), QDate::currentDate() }),
    m_state(OFF),
    m_type(INTERVAL)
{

}
//-------------------------------------------------------------------------------
CFilter::CFilter(const FilterIntervalType& interval, const FilterDateType& date):
    m_interval(interval),
    m_date(date),
    m_state(OFF),
    m_type(INTERVAL)
{

}
//--------------------------------------------------
const CFilter::FilterDateType& CFilter::date() const
{
    return m_date;
}
//----------------------------------------------------------
const CFilter::FilterIntervalType& CFilter::interval() const
{
    return m_interval;
}
//-------------------------
bool CFilter::state() const
{
    return m_state;
}
//---------------------------------------
CFilter::FilterType CFilter::type() const
{
    return m_type;
}
//-----------------------------------------------
void CFilter::setDate(const FilterDateType& date)
{
    m_date = date;
}
//-----------------------------------------------------------
void CFilter::setInterval(const FilterIntervalType& interval)
{
    m_interval = interval;
}
//--------------------------------
void CFilter::setState(bool state)
{
    m_state = state;
}
//---------------------------------------------
void CFilter::setType(CFilter::FilterType type)
{
    m_type = type;
}
