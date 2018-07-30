#include "window.h"
//--------------------------------
CWindow::CWindow(QWidget *parent):
    QWidget(parent)
{

}
//-----------------------
void CWindow::startDrag()
{
    CWidgetDrag* drag = new CWidgetDrag(this);
    drag->setWidget(this);
    drag->exec(Qt::CopyAction);
}
//-----------------------------------------------
void CWindow::mousePressEvent(QMouseEvent* event)
{
    if(event->button() == Qt::LeftButton)
        m_drag_pos = event->pos();

    QWidget::mousePressEvent(event);
}
//----------------------------------------------
void CWindow::mouseMoveEvent(QMouseEvent* event)
{
    if(event->button() == Qt::LeftButton)
    {
        int distance = (event->pos() - m_drag_pos).manhattanLength();

        if(distance > QApplication::startDragDistance())
            startDrag();
    }

    QWidget::mouseMoveEvent(event);
}
