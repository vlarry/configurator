#include "toolbutton.h"
//----------------------------------------
CToolButton::CToolButton(QWidget* parent):
    QToolButton(parent),
    m_id(-1),
    m_is_menu(false)
{

}
//-----------------------------
void CToolButton::setID(int id)
{
    m_id = id;
}
//--------------------------------------------
void CToolButton::setIndicatorMenu(bool state)
{
    m_is_menu = state;
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
//-----------------------------------------
void CToolButton::leaveEvent(QEvent *event)
{
    QToolButton::leaveEvent(event);
    emit leaved(m_id);
}
//----------------------------------------------
void CToolButton::paintEvent(QPaintEvent *event)
{
    QToolButton::paintEvent(event);

    if(m_is_menu)
    {
        QPainter painter(this);
        painter.setPen(Qt::black);
        painter.setBrush(Qt::green);

        QRect r = event->rect();
        int start_x = r.width() - 12;
        int start_y = r.height()/2 + 4;
        QPolygon poly_sign_menu = QPolygon() << QPoint(start_x, start_y) << QPoint(start_x, start_y - 8) << QPoint(start_x + 4, start_y - 4);
        painter.drawPolygon(poly_sign_menu);
    }
}
