#include "tabwidget.h"
//--------------------------------------
CTabWidget::CTabWidget(QWidget* parent):
    QTabWidget(parent)
{
    setMouseTracking(true);

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
