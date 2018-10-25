#include "dockwidget.h"
#include "ui_dockwidget.h"
//-----------------------------
int CDockWidget::m_idCount = 0;
//----------------------------------------
CDockWidget::CDockWidget(QWidget* parent):
    QWidget(parent),
    ui(new Ui::CDockWidget),
    m_controlItem(nullptr)
{
    ui->setupUi(this);
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
        container->setID(m_idCount++);
        container->setAnchor(CContainerWidget::AnchorType::AnchorDockWidget);
        container->setSide(m_controlItem->dir());
        container->setPosition(ui->verticalLayoutContainer->count());
        container->setHeaderBackground(QColor(190, 190, 190));
        container->show();
        ui->verticalLayoutContainer->addWidget(container);

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
    for(int i = 0; i < ui->verticalLayoutContainer->count(); i++)
    {
        QLayoutItem* item = ui->verticalLayoutContainer->itemAt(i);
        CContainerWidget* tcontainer = static_cast<CContainerWidget*>(item->widget());

        if(tcontainer->id() == id)
            return tcontainer;
    }

    return nullptr;
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
    for(int i = 0; i < ui->verticalLayoutContainer->count(); i++)
    {
        QLayoutItem* item = ui->verticalLayoutContainer->itemAt(i);

        if(item)
        {
            CContainerWidget* tcontainer = static_cast<CContainerWidget*>(item->widget());

            if(tcontainer->id() == id)
            {
                tcontainer->setPosition(-1);
                ui->verticalLayout->removeWidget(tcontainer);
                if(ui->verticalLayoutContainer->isEmpty()) // контейнер пуст
                    emit m_controlItem->clicked();
            }
        }
    }
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
        addContainer(container);
    }
}
