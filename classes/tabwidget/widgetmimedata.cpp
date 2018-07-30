#include "widgetmimedata.h"
//---------------------------------
CWidgetMimeData::CWidgetMimeData():
    QMimeData(),
    m_wgt(nullptr)
{

}
//---------------------------------
CWidgetMimeData::~CWidgetMimeData()
{

}
//---------------------------------
QString CWidgetMimeData::mimeType()
{
    return "application/widget";
}
//-------------------------------------------
void CWidgetMimeData::setWidget(QWidget* wgt)
{
    m_wgt = wgt;
    setData(mimeType(), QByteArray());
}
//--------------------------------------
QWidget *CWidgetMimeData::widget() const
{
    return m_wgt;
}
