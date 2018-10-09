#include "tabwidget.h"
//--------------------------------------
CTabWidget::CTabWidget(QWidget* parent):
    QTabWidget(parent)
{
    setMouseTracking(true);
    setAcceptDrops(true);

    this->tabBar()->setAcceptDrops(true);

    connect(this, &CTabWidget::tabBarDoubleClicked, this, &CTabWidget::tabDoubleClicked);
}
//------------------------------------------
void CTabWidget::tabDoubleClicked(int index)
{
    QWidget* tabWidget = widget(index);
    QString  tabTitle  = tabText(index);
    QLayout* l         = tabWidget->layout();
    QWidget* newWidget = new QWidget(this);

    newWidget->setLayout(l);
    newWidget->setWindowTitle(tabTitle);
    newWidget->setWindowFlag(Qt::Window);
    newWidget->show();

    removeTab(index);
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

        addTab(container->widget(), container->headerTitle());
    }
}
