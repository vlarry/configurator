#include "toolbutton.h"
//----------------------------------------
CToolButton::CToolButton(QWidget* parent):
    QToolButton(parent),
    m_id(-1)
{

}
//-----------------------------
void CToolButton::setID(int id)
{
    m_id = id;
}
//-------------------------
int CToolButton::id() const
{
    return m_id;
}
//-----------------------------------------
void CToolButton::enterEvent(QEvent* event)
{
    QToolButton::enterEvent(event);

    emit hovered(m_id);
}
