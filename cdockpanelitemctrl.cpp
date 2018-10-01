#include "cdockpanelitemctrl.h"
//------------------------------------------------------
CDockPanelItemCtrl::CDockPanelItemCtrl(QWidget* parent):
    QPushButton(parent),
    m_text(""),
    m_dir(Left),
    m_state(Open)
{

}
//---------------------------------------------------------------------------
CDockPanelItemCtrl::CDockPanelItemCtrl(const QString& text, QWidget* parent):
    QPushButton(parent),
    m_text(text),
    m_dir(Left),
    m_state(Open)
{

}
//---------------------------------------------------------
CDockPanelItemCtrl::DirType CDockPanelItemCtrl::dir() const
{
    return m_dir;
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
//--------------------------------------------------------------
void CDockPanelItemCtrl::setDir(CDockPanelItemCtrl::DirType dir)
{
    m_dir = dir;
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
//---------------------------------------
void CDockPanelItemCtrl::timeoutRepaint()
{
    m_timer_repaint->stop();
    updateGeometry();
}
//---------------------------------------------------
void CDockPanelItemCtrl::showEvent(QShowEvent* event)
{
    QPushButton::showEvent(event);

    m_timer_repaint = new QTimer(this);
    connect(m_timer_repaint, &QTimer::timeout, this, &CDockPanelItemCtrl::timeoutRepaint);
    m_timer_repaint->start(50);
}
//-----------------------------------------------------
void CDockPanelItemCtrl::paintEvent(QPaintEvent* event)
{
    QPushButton::paintEvent(event);

    QPainter painter(this);
    QRect    but_rect(event->rect());
    QFont    f(painter.font());

    f.setPointSize(8);

    painter.setFont(f);

    QLinearGradient gradient(0, 0, but_rect.width(), 0);
    gradient.setColorAt(0, QColor(190, 190, 190));
    gradient.setColorAt(0.5, Qt::gray);
    gradient.setColorAt(1, QColor(190, 190, 190));

    painter.fillRect(but_rect, QBrush(gradient));

    if((m_dir == Left || m_dir == Right) && !m_text.isEmpty())
    {
        painter.save();

        painter.translate(but_rect.left(), but_rect.bottom());
        painter.rotate(-90);

        QRect r(but_rect.height() - painter.fontMetrics().width(m_text) - 5,
                but_rect.width()/2 - painter.fontMetrics().height()/2,
                but_rect.height(), static_cast<int>(painter.fontMetrics().width(m_text)*1.2f));
        painter.drawText(r, m_text);

        painter.restore();
    }
    else if(m_text.isEmpty())
    {
        QRect r = but_rect;
        r.setX(r.x() + 5);
        painter.drawText(r, Qt::AlignLeft | Qt::AlignVCenter, m_text);
    }

    int cx = but_rect.width()/2;
    int cy = but_rect.height()/2;

    QPolygon polygon;
    QPolygon polygon_open, polygon_close;

    switch(m_dir)
    {
        case Left:
            polygon_open << QPoint(cx - 4, cy) << QPoint(cx + 4, cy - 6) << QPoint(cx + 4, cy + 6);
            polygon_close << QPoint(cx - 4, cy - 6) << QPoint(cx + 4, cy) << QPoint(cx - 4, cy + 6);
        break;

        case Right:
            polygon_close << QPoint(cx - 4, cy) << QPoint(cx + 4, cy - 6) << QPoint(cx + 4, cy + 6);
            polygon_open << QPoint(cx - 4, cy - 6) << QPoint(cx + 4, cy) << QPoint(cx - 4, cy + 6);
        break;

        case Top:
            polygon_open << QPoint(cx - 6, cy + 4) << QPoint(cx + 6, cy + 4) << QPoint(cx, cy - 4);
            polygon_close << QPoint(cx - 6, cy - 4) << QPoint(cx + 6, cy - 4) << QPoint(cx, cy + 4);
        break;

        case Bottom:
            polygon_close << QPoint(cx - 6, cy + 4) << QPoint(cx + 6, cy + 4) << QPoint(cx, cy - 4);
            polygon_open << QPoint(cx - 6, cy - 4) << QPoint(cx + 6, cy - 4) << QPoint(cx, cy + 4);
        break;
    }

    if(m_state == Close)
    {
        polygon = polygon_close;
    }
    else if(m_state == Open)
    {
        polygon = polygon_open;
    }

    painter.setBrush(Qt::lightGray);
    painter.drawPolygon(polygon);
}

