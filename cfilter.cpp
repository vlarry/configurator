#include "cfilter.h"
//-----------------
CFilter::CFilter():
    m_range(RangeType({ 0, 0 })),
    m_limit(LimitType({ 0, 0 })),
    m_date(DateType({ QDate::currentDate(), QDate::currentDate() })),
    m_time(QTime::fromString("00:00:00", "HH:mm:ss")),
    m_type(FilterLimitType),
    m_state(false)
{

}
//---------------------------------------------------------------------------------------
CFilter::CFilter(int min, int max, int low, int upper, QDate &from, QDate &to, QTime &t):
    m_range(RangeType({ min, max })),
    m_limit(LimitType({ low, upper })),
    m_date(DateType({ from, to })),
    m_time(t),
    m_type(FilterLimitType),
    m_state(false)
{

}
//--------------------------------------------------------------------------------------------------------
CFilter::CFilter(CFilter::RangeType &range, CFilter::LimitType &limit, CFilter::DateType &date, QTime &t):
    m_range(range),
    m_limit(limit),
    m_date(date),
    m_time(t),
    m_type(FilterLimitType),
    m_state(false)
{

}
//----------------------------------
CFilter::RangeType &CFilter::range()
{
    return m_range;
}
//----------------------------------
CFilter::LimitType &CFilter::limit()
{
    return m_limit;
}
//--------------------------------
CFilter::DateType &CFilter::date()
{
    return m_date;
}
//--------------------------------
int CFilter::rangeMinValue() const
{
    return m_range.min;
}
//--------------------------------
int CFilter::rangeMaxValue() const
{
    return m_range.max;
}
//--------------------------------
int CFilter::limitLowValue() const
{
    return m_limit.low;
}
//----------------------------------
int CFilter::limitUpperValue() const
{
    return m_limit.upper;
}
//------------------------
QDate &CFilter::dateFrom()
{
    return m_date.from;
}
//----------------------
QDate &CFilter::dateTo()
{
    return m_date.to;
}
//--------------------
QTime &CFilter::time()
{
    return m_time;
}
//---------------------------------------
CFilter::FilterType CFilter::type() const
{
    return m_type;
}
//-------------------------
bool CFilter::state() const
{
    return m_state;
}
//-------------------
void CFilter::reset()
{
    m_limit = LimitType({ m_range.min, m_range.max });
    m_date  = DateType({ QDate::currentDate(), QDate::currentDate() });
    m_type  = FilterLimitType;
    m_state = false;
}
//-----------------------------------------------
void CFilter::setRange(CFilter::RangeType &range)
{
    m_range = range;
}
//--------------------------------------
void CFilter::setRange(int min, int max)
{
    m_range = RangeType({ min, max });
}
//-----------------------------------------------
void CFilter::setLimit(CFilter::LimitType &limit)
{
    m_limit = limit;
}
//----------------------------------------
void CFilter::setLimit(int low, int upper)
{
    m_limit = LimitType({ low, upper });
}
//-----------------------------------
void CFilter::setDate(DateType &date)
{
    m_date = date;
}
//-------------------------------------------------------
void CFilter::setDate(const QDate &from, const QDate &to)
{
    m_date = DateType({ from, to });
}
//--------------------------------------
void CFilter::setTime(const QTime &time)
{
    m_time = time;
}
//---------------------------------------------
void CFilter::setType(CFilter::FilterType type)
{
    m_type = type;
}
//--------------------------------
void CFilter::setState(bool state)
{
    m_state = state;
}
//----------------------------
CFilter::operator bool() const
{
    return m_state;
}
