#include "dockwidget.h"
#include "ui_dockwidget.h"
//-----------------------------
int CDockWidget::m_idCount = 0;
//----------------------------------------
CDockWidget::CDockWidget(QWidget* parent):
    QFrame(parent),
    ui(new Ui::CDockWidget),
    m_controlItem(nullptr),
    m_splitter(nullptr)
{
    ui->setupUi(this);
    ui->pushButtonItemCtrlBottom->hide();
    ui->pushButtonItemCtrlLeft->hide();
    ui->pushButtonItemCtrlRight->hide();
    ui->pushButtonItemCtrlTop->hide();

    m_splitter = new QSplitter(Qt::Vertical, ui->widgetContainer);
    m_splitter->setChildrenCollapsible(true);
    m_splitter->setObjectName("DockSplitter");
    m_splitter->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->verticalLayoutContainer->addWidget(m_splitter);

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

        int container_pos = m_splitter->count();
        container->setPosition(container_pos);

        container->setHeaderBackground(QColor(190, 190, 190));
        container->show();
        m_splitter->addWidget(container);
        m_splitter->setStretchFactor(container_pos, 1);
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
    for(int i = 0; i < m_splitter->count(); i++)
    {
        CContainerWidget* tcontainer = static_cast<CContainerWidget*>(m_splitter->widget(i));

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
        QWidget* wgt = m_splitter->widget(i);

        if(wgt)
        {
            CContainerWidget* tcontainer = static_cast<CContainerWidget*>(wgt);

            if(tcontainer->id() == id)
            {
                tcontainer->setPosition(-1);
                tcontainer->setSide(CDockPanelItemCtrl::DirNone);
                delete tcontainer;

                if(m_splitter->count() == 0) // контейнер пуст
                    emit m_controlItem->clicked();
            }
        }
    }
}
//------------------------------------------------------
void CDockWidget::dragEnterEvent(QDragEnterEvent* event)
{
    if(event->mimeData()->hasFormat("application/widget_container"))
    {
        if(event->source() == this)
        {
            event->setDropAction(Qt::MoveAction);
            event->accept();
            setStyleSheet("CDockWidget { border: none; }");
        }
        else
        {
            event->acceptProposedAction();
            setStyleSheet("CDockWidget { border: 5px solid lightgreen; }");
        }
    }
    else
    {
        event->ignore();
        setStyleSheet("CDockWidget { border: none; }");
    }
}
//----------------------------------------------------
void CDockWidget::dragMoveEvent(QDragMoveEvent *event)
{
    if(event->mimeData()->hasFormat("application/widget_container"))
    {
        if(event->source() == this)
        {
            event->setDropAction(Qt::MoveAction);
            event->accept();
            setStyleSheet("CDockWidget { border: none; }");
        }
        else
        {
            event->acceptProposedAction();
            setStyleSheet("CDockWidget { border: 5px solid lightgreen; }");
        }
    }
    else
    {
        event->ignore();
        setStyleSheet("CDockWidget { border: none; }");
    }
}
//------------------------------------------------------
void CDockWidget::dragLeaveEvent(QDragLeaveEvent *event)
{
    Q_UNUSED(event);
    setStyleSheet("CDockWidget { border: none; }");
}
//--------------------------------------------
void CDockWidget::dropEvent(QDropEvent* event)
{
    CContainerWidget* container = event->mimeData()->property("CONTAINER").value<CContainerWidget*>();
    setStyleSheet("CDockWidget { border: none; }");

    if(container)
    {
        event->accept();
        addContainer(container);
    }
}
