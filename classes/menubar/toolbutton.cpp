#include "toolbutton.h"
//------------------------------------------------------------
QVector<int> CToolButton::m_button_level = QVector<int>(3, 0);
//----------------------------------------
CToolButton::CToolButton(QWidget* parent):
    QToolButton(parent),
    m_id(-1),
    m_menu({ false, false, 0 })
{

}
//-----------------------------
void CToolButton::setID(int id)
{
    m_id = id;
}
//----------------------------------------------------
void CToolButton::setButtonMenu(bool state, int level)
{
    m_menu = { state, false, level };
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

    switch(m_menu.level)
    {
        case 1:
            m_button_level = { m_id, 0, 0 };
        break;

        case 2:
            m_button_level = { m_button_level[0], m_id, 0 };
        break;

        case 3:
            m_button_level[2] = m_id;
        break;

        default:
            if(this->objectName().toUpper() != QString("toolButtonExptortToExcel").toUpper() &&
               this->objectName().toUpper() != QString("toolButtonExptortToPDF").toUpper() &&
               this->objectName().toUpper() != QString("toolButtonExptortToDB").toUpper() &&
               this->objectName().toUpper() != QString("toolButtonImportFromExcel").toUpper() &&
               this->objectName().toUpper() != QString("toolButtonImportFromDB").toUpper())
            {
                m_button_level = { 0, 0, 0 };
            }
        break;
    }

    emit updateButtons();
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

    if(m_menu.is_menu)
    {
        QPainter painter(this);
        painter.setPen(Qt::black);
        painter.setBrush(Qt::green);

        QRect r = event->rect();
        int start_x = r.width() - 12;
        int start_y = r.height()/2 + 4;
        QPolygon poly_sign_menu = QPolygon() << QPoint(start_x, start_y) << QPoint(start_x, start_y - 8) << QPoint(start_x + 4, start_y - 4);
        painter.drawPolygon(poly_sign_menu);

        if(m_menu.level > 0)
        {
            if(m_button_level[m_menu.level - 1] == m_id)
                painter.fillRect(r, QBrush(QColor(0, 255, 0, 64)));
        }
    }
}
