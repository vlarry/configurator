#include "dockwidget.h"
#include "ui_dockwidget.h"
//----------------------------------------
CDockWidget::CDockWidget(QWidget* parent):
    QWidget(parent),
    ui(new Ui::CDockWidget),
    m_pos(-1, -1)
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

        connect(container, &CContainerWidget::containerClicked, this, &CDockWidget::pressItem);
        connect(container, &CContainerWidget::containerMoved, this, &CDockWidget::moveItem);
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
//---------------------------------------------
void CDockWidget::pressItem(QMouseEvent* event)
{
    if(event->button() == Qt::LeftButton)
    {
        m_pos = event->pos();
    }
}
//----------------------------------------------------
void CDockWidget::moveItem(QMouseEvent* event, int id)
{
    if((event->buttons() & Qt::LeftButton) && (QApplication::startDragDistance() <= (event->pos() - m_pos).manhattanLength()))
    {
        CContainerWidget* currentContainer = container(id);

        if(!currentContainer)
            return;

        CContainerWidget* copyContainer = new CContainerWidget(currentContainer->headerTitle(), currentContainer->widget(),
                                                               currentContainer->anchor(), this);
        copyContainer->setHeaderBackground(currentContainer->backgroundColorHeader());

        QDrag* drag = new QDrag(this);
        QMimeData* mimedata = new QMimeData;

        mimedata->setProperty("CONTAINER", QVariant::fromValue(copyContainer));
        mimedata->setData("application/widget_container", QByteArray());
        drag->setMimeData(mimedata);

        QPixmap pixmap(currentContainer->size());
        currentContainer->render(&pixmap);
        drag->setPixmap(pixmap);

        currentContainer->close();
        ui->gridLayoutContainer->removeItem(ui->gridLayoutContainer->takeAt(id));

        Qt::DropAction result = drag->exec(Qt::MoveAction);

        if(result != Qt::MoveAction)
        {
            CContainerWidget* tcontainer = mimedata->property("CONTAINER").value<CContainerWidget*>();

            if(tcontainer)
            {
                addContainer(tcontainer);
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
        addContainer(container);
    }
}
