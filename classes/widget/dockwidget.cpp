#include "dockwidget.h"
#include "ui_dockwidget.h"
//----------------------------------------
CDockWidget::CDockWidget(QWidget* parent):
    QWidget(parent),
    ui(new Ui::CDockWidget)
{
    ui->setupUi(this);
    ui->gridLayoutContainer->setColumnStretch(0, 1);
    ui->pushButtonItemCtrl->setMinimumWidth(16);
    ui->pushButtonItemCtrl->resize(16, ui->pushButtonItemCtrl->height());
    setAcceptDrops(true);
}
//-------------------------
CDockWidget::~CDockWidget()
{
    delete ui;
}
//---------------------------------------------------------
void CDockWidget::addContainer(CContainerWidget* container)
{
    if(container)
    {
        int row = ui->gridLayoutContainer->count();
        container->setID(row);
        container->show();
        container->setVisible(true);
        ui->gridLayoutContainer->addWidget(container, row, 0);

        connect(container, &CContainerWidget::removeContainer, this, &CDockWidget::removeItem);
    }
}
//----------------------------------------
CDockPanelItemCtrl *CDockWidget::control()
{
    return ui->pushButtonItemCtrl;
}
//----------------------------------------------
CContainerWidget* CDockWidget::container(int id)
{
    CContainerWidget* tcontainer = nullptr;
    QLayoutItem* lItem = ui->gridLayoutContainer->itemAt(id);

    if(lItem)
    {
        tcontainer = static_cast<CContainerWidget*>(lItem->widget());
    }

    return tcontainer;
}
//-----------------------------
void CDockWidget::hideContent()
{
    ui->widgetContainer->hide();
}
//-----------------------------
void CDockWidget::showContent()
{
    ui->widgetContainer->show();
}
//---------------------------------------
bool CDockWidget::isContentHidden() const
{
    return ui->widgetContainer->isHidden();
}
//---------------------------------------------
void CDockWidget::setVisibleContent(bool state)
{
    if(state)
        showContent();
    else
        hideContent();
}
//----------------------------------
void CDockWidget::removeItem(int id)
{
    ui->gridLayoutContainer->removeItem(ui->gridLayoutContainer->takeAt(id));
}
//------------------------------------------------------
void CDockWidget::dragEnterEvent(QDragEnterEvent* event)
{
    if(event->mimeData()->hasFormat("application/widget_container"))
        event->acceptProposedAction();
    else
        event->ignore();
}
//--------------------------------------------
void CDockWidget::dropEvent(QDropEvent* event)
{
    CContainerWidget* container = event->mimeData()->property("CONTAINER").value<CContainerWidget*>();

    if(container)
    {
        event->accept();
        container->setAnchor(CContainerWidget::AnchorType::AnchorDockWidget);
        addContainer(container);
    }
}
