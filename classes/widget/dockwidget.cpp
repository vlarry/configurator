#include "dockwidget.h"
#include "ui_dockwidget.h"
//----------------------------------------
CDockWidget::CDockWidget(QWidget* parent):
    QWidget(parent),
    ui(new Ui::CDockWidget),
    m_controlItem(nullptr)
{
    ui->setupUi(this);
    ui->gridLayoutContainer->setColumnStretch(0, 1);

    ui->pushButtonItemCtrlBottom->hide();
    ui->pushButtonItemCtrlLeft->hide();
    ui->pushButtonItemCtrlRight->hide();
    ui->pushButtonItemCtrlTop->hide();

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
    return m_controlItem;
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
//------------------------------------------------------------------
void CDockWidget::setControlItemDir(CDockPanelItemCtrl::DirType dir)
{
    m_controlItem = nullptr;

    ui->pushButtonItemCtrlBottom->hide();
    ui->pushButtonItemCtrlLeft->hide();
    ui->pushButtonItemCtrlRight->hide();
    ui->pushButtonItemCtrlTop->hide();

    switch(dir)
    {
        case CDockPanelItemCtrl::DirNone:
            m_controlItem = nullptr;
        break;

        case CDockPanelItemCtrl::DirLeft:
            m_controlItem = ui->pushButtonItemCtrlLeft;
        break;

        case CDockPanelItemCtrl::DirRight:
            m_controlItem = ui->pushButtonItemCtrlRight;
        break;

        case CDockPanelItemCtrl::DirTop:
            m_controlItem = ui->pushButtonItemCtrlTop;
        break;

        case CDockPanelItemCtrl::DirBottom:
            m_controlItem = ui->pushButtonItemCtrlBottom;
        break;

        default:
            m_controlItem = nullptr;
        break;
    }

    if(m_controlItem)
    {
        m_controlItem->setDir(dir);
        m_controlItem->show();
    }
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
