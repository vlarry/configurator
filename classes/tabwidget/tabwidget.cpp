#include "tabwidget.h"
//--------------------------------------
CTabWidget::CTabWidget(QWidget* parent):
    QTabWidget(parent),
    m_superParent(nullptr)
{
    setMouseTracking(true);
    setAcceptDrops(true);

    this->tabBar()->setAcceptDrops(true);

    connect(this, &CTabWidget::tabBarDoubleClicked, this, &CTabWidget::tabDoubleClicked);
}
//----------------------------------------------
void CTabWidget::setSuperParent(QWidget* parent)
{
    m_superParent = parent;
}
//------------------------------------------
void CTabWidget::tabDoubleClicked(int index)
{
    CContainerWidget* container = static_cast<CContainerWidget*>(widget(index));

    if(!container)
        return;

    container->setAnchor(CContainerWidget::AnchorType::AnchorFree);
    container->setSuperParent(m_superParent);
    container->setParent(m_superParent);
    container->setHeaderBackground(QColor(190, 190, 190));
    container->headerShow();

    QPoint pos = container->pos();

    if(m_superParent)
    {
        int cx = (m_superParent->width() - m_superParent->x())/2;
        int cy = (m_superParent->height() - m_superParent->y())/2;
        int x  = cx - container->width()/2;
        int y  = cy - container->height()/2;

        pos = QPoint(x, y);
    }

    container->move(pos);
    container->show();
}
//-----------------------------------------------------
void CTabWidget::dragEnterEvent(QDragEnterEvent* event)
{
    if(event->mimeData()->hasFormat("application/widget_container"))
        event->acceptProposedAction();
    else
        event->ignore();
}
//-------------------------------------------
void CTabWidget::dropEvent(QDropEvent* event)
{
    CContainerWidget* container = event->mimeData()->property("CONTAINER").value<CContainerWidget*>();

    if(container)
    {
        event->accept();
        container->setAnchor(CContainerWidget::AnchorType::AnchorDockWidget);
        container->headerHide();
        addTab(container, container->headerTitle());
    }
}
