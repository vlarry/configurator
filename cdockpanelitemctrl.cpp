#include "cdockpanelitemctrl.h"
//------------------------------------------------------
CDockPanelItemCtrl::CDockPanelItemCtrl(QWidget* parent):
    QPushButton(parent),
    m_text(""),
    m_side(Left),
    m_state(Open)
{

}
//---------------------------------------------------------------------------
CDockPanelItemCtrl::CDockPanelItemCtrl(const QString& text, QWidget* parent):
    QPushButton(parent),
    m_text(text),
    m_side(Left),
    m_state(Open)
{

}
//-----------------------------------------------------------
CDockPanelItemCtrl::SideType CDockPanelItemCtrl::side() const
{
    return m_side;
}
//-------------------------------------------------------------
CDockPanelItemCtrl::StateType CDockPanelItemCtrl::state() const
{
    return m_state;
}
//--------------------------------------
QString CDockPanelItemCtrl::text() const
{
    return m_text;
}
//-----------------------------------------------------------------
void CDockPanelItemCtrl::setSide(CDockPanelItemCtrl::SideType side)
{
    m_side = side;
}
//--------------------------------------------------------------------
void CDockPanelItemCtrl::setState(CDockPanelItemCtrl::StateType state)
{
    m_state = state;
}
//---------------------------------------------------
void CDockPanelItemCtrl::setText(const QString& text)
{
    m_text = text;
}
//-----------------------------
void CDockPanelItemCtrl::show()
{
    updateGeometry();
}
//-----------------------------------------------------
void CDockPanelItemCtrl::paintEvent(QPaintEvent* event)
{
    QPushButton::paintEvent(event);

    if(m_text.isEmpty())
        return;

    QPainter painter(this);
    QRect    but_rect(event->rect());
    QFont    f(painter.font());

    f.setPointSize(8);

    painter.setFont(f);
    painter.save();

    painter.translate(but_rect.left(), but_rect.bottom());
    painter.rotate(-90);

    int pos_x = but_rect.height() - painter.fontMetrics().width(m_text) - 5;
    int pos_y = but_rect.width()/2 - painter.fontMetrics().height()/2;
    int w     = but_rect.height();
    int h     = painter.fontMetrics().width(m_text)*1.2f;

    QRect r(pos_x, pos_y, w, h);
    painter.drawText(r, m_text);

    painter.restore();

    int cx = but_rect.width()/2;
    int cy = but_rect.height()/2;

    QPolygon polygon_left, polygon_right, polygon;

    polygon_left << QPoint(cx - 4, cy) << QPoint(cx + 4, cy - 6) << QPoint(cx + 4, cy + 6);
    polygon_right << QPoint(cx - 4, cy - 6) << QPoint(cx + 4, cy) << QPoint(cx - 4, cy + 6);

    if(m_state == Close)
    {
        if(m_side == Left)
            polygon = polygon_left;
        else if(m_side == Right)
            polygon = polygon_right;
    }
    else if(m_state == Open)
    {
        if(m_side == Left)
            polygon = polygon_right;
        else if(m_side == Right)
            polygon = polygon_left;
    }

    painter.setBrush(Qt::green);
    painter.drawPolygon(polygon);
}

