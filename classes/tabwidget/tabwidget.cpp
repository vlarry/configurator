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
    Q_UNUSED(event);
}
//-------------------------------------------
void CTabWidget::dropEvent(QDropEvent* event)
{
    Q_UNUSED(event);
}
