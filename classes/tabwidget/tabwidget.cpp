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
    connect(this, &CTabWidget::removeContainer, this, &CTabWidget::tabRemove);
}
//--------------------------------------------------------
void CTabWidget::addContainer(CContainerWidget* container)
{
    container->setAnchor(CContainerWidget::AnchorType::AnchorDockWidget);
    container->setSide(CDockPanelItemCtrl::DirBottom);

    int index = addTab(container, container->headerTitle());

    if(index != -1)
        setCurrentIndex(index);
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
    container->setSide(CDockPanelItemCtrl::DirNone);
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
//----------------------------------------------
void CTabWidget::tabRemove(const QString &title)
{
    if(!title.isEmpty())
    {
        for(int i = 0; i < count(); i++)
        {
            QString text = tabBar()->tabText(i);

            if(title.toUpper() == text.toUpper())
            {
                removeTab(i);
                break;
            }
        }
    }
}
//-----------------------------------------------------
void CTabWidget::dragEnterEvent(QDragEnterEvent* event)
{
    if(event->mimeData()->hasFormat("application/widget_container"))
    {
        if(event->source() == this)
        {
            event->setDropAction(Qt::MoveAction);
            event->accept();
            setStyleSheet("border: none;");
        }
        else
        {
            event->acceptProposedAction();
            setStyleSheet("border: 5px solid lightgreen;");
        }
    }
    else
    {
        event->ignore();
        setStyleSheet("border: none;");
    }
}
//---------------------------------------------------
void CTabWidget::dragMoveEvent(QDragMoveEvent *event)
{
    if(event->mimeData()->hasFormat("application/widget_container"))
    {
        if(event->source() == this)
        {
            event->setDropAction(Qt::MoveAction);
            event->accept();
            setStyleSheet("border: none;");
        }
        else
        {
            event->acceptProposedAction();
            setStyleSheet("border: 5px solid lightgreen;");
        }
    }
    else
    {
        event->ignore();
        setStyleSheet("border: none;");
    }
}
//-----------------------------------------------------
void CTabWidget::dragLeaveEvent(QDragLeaveEvent *event)
{
    Q_UNUSED(event);
    setStyleSheet("border: none;");
}
//-------------------------------------------
void CTabWidget::dropEvent(QDropEvent* event)
{
    CContainerWidget* container = event->mimeData()->property("CONTAINER").value<CContainerWidget*>();
    setStyleSheet("border: none;");

    if(container)
    {
        event->accept();
        addContainer(container);
    }
}
