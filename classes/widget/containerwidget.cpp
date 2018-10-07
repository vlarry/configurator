#include "containerwidget.h"
#include "ui_containerwidget.h"
//-------------------------------------------------------------------------------------------------------------------
CContainerWidget::CContainerWidget(const QString& title, QWidget* contentWidget, AnchorType anchor, QWidget* parent):
    QWidget(parent),
    ui(new Ui::CContainerWidget),
    m_superParent(nullptr),
    m_anchor(anchor),
    m_pos(QPoint(-1, -1)),
    m_id(-1),
    m_background_color(QColor())
{
    ui->setupUi(this);
    ui->toolButtonHeader->hide();
    ui->labelHeader->setText(title);
    ui->labelHeader->setAlignment(Qt::AlignCenter);
    ui->labelHeader->setAutoFillBackground(true);
    ui->labelHeader->installEventFilter(this);

    QFont f(ui->labelHeader->font());
    f.setPointSize(12);
    f.setBold(true);
    ui->labelHeader->setFont(f);

    setWidget(contentWidget);
    setAttribute(Qt:: WA_DeleteOnClose);
}
//-----------------------------------
CContainerWidget::~CContainerWidget()
{
    delete ui;
}
//-----------------------------------------------------------
CContainerWidget::AnchorType CContainerWidget::anchor() const
{
    return m_anchor;
}
//---------------------------------
QWidget* CContainerWidget::widget()
{
    return m_contentWidget;
}
//---------------------------------
void CContainerWidget::buttonHide()
{
    ui->toolButtonHeader->hide();
}
//---------------------------------
void CContainerWidget::buttonShow()
{
    ui->toolButtonHeader->show();
}
//----------------------------------------------------
QColor CContainerWidget::backgroundColorHeader() const
{
    return m_background_color;
}
//-------------------------------------------
QString CContainerWidget::headerTitle() const
{
    return ui->labelHeader->text();
}
//------------------------------
int CContainerWidget::id() const
{
    return m_id;
}
//--------------------------------------------
QWidget *CContainerWidget::superParent() const
{
    return m_superParent;
}
//--------------------------------------------
void CContainerWidget::setWidget(QWidget* wgt)
{
    m_contentWidget = wgt;

    QVBoxLayout* vlayout = new QVBoxLayout(ui->widgetContainer);
    vlayout->setContentsMargins(0, 0, 0, 0);
    vlayout->setSpacing(0);
    vlayout->setAlignment(Qt::AlignCenter);
    vlayout->addWidget(m_contentWidget);
    ui->widgetContainer->setLayout(vlayout);
}
//-----------------------------------------------------------------------
void CContainerWidget::setHeaderBackground(const QColor& backgroundColor)
{
    m_background_color = backgroundColor;

    QLinearGradient gradient(0, 0, 0, ui->labelHeader->height());
    gradient.setColorAt(0, backgroundColor);
    gradient.setColorAt(0.5, Qt::gray);
    gradient.setColorAt(1, backgroundColor);

    QPalette pal(ui->labelHeader->palette());
    pal.setBrush(QPalette::Window, QBrush(gradient));
    ui->labelHeader->setPalette(pal);
}
//-------------------------------------------------------------------
void CContainerWidget::setAnchor(CContainerWidget::AnchorType anchor)
{
    m_anchor = anchor;
}
//----------------------------------
void CContainerWidget::setID(int id)
{
    m_id = id;
}
//----------------------------------------------------
void CContainerWidget::setSuperParent(QWidget* parent)
{
    m_superParent = parent;
}
//----------------------------------------------------------------
bool CContainerWidget::eventFilter(QObject* object, QEvent* event)
{
    if(object == ui->labelHeader)
    {
        if(event->type() == QEvent::MouseButtonPress)
        {
            QMouseEvent* me = static_cast<QMouseEvent*>(event);

            if(me->button() == Qt::LeftButton)
                m_pos = me->pos();
        }
        else if(event->type() == QEvent::MouseMove)
        {
            QMouseEvent* me = static_cast<QMouseEvent*>(event);

            if(me->buttons() & Qt::LeftButton)
            {
                if(QApplication::startDragDistance() <= (me->pos() - m_pos).manhattanLength())
                {
                    QWidget* parentWgt = static_cast<QWidget*>(parent());

                    if(!parentWgt)
                        return false;

                    CContainerWidget* copyContainer = new CContainerWidget(headerTitle(), widget(), anchor(), parentWgt);\
                    copyContainer->setSuperParent(superParent());
                    copyContainer->setHeaderBackground(backgroundColorHeader());
                    copyContainer->setGeometry(copyContainer->x(), copyContainer->y(), width(), height());

                    QDrag* drag = new QDrag(this);
                    QMimeData* mimedata = new QMimeData;

                    mimedata->setProperty("CONTAINER", QVariant::fromValue(copyContainer));
                    mimedata->setData("application/widget_container", QByteArray());
                    drag->setMimeData(mimedata);

                    QPixmap pixmap(copyContainer->size());
                    copyContainer->render(&pixmap);
                    drag->setPixmap(pixmap);

                    close();
                    emit removeContainer(m_id);

                    Qt::DropAction result = drag->exec(Qt::MoveAction);

                    if(result == Qt::IgnoreAction)
                    {
                        CContainerWidget* tcontainer = mimedata->property("CONTAINER").value<CContainerWidget*>();

                        if(tcontainer)
                        {
                            QPoint pos = m_superParent->mapFromParent(QCursor::pos());
                            tcontainer->setParent(m_superParent);
                            tcontainer->show();
                            tcontainer->move(pos);
                        }
                    }
                }
            }
        }
        else if(event->type() == QEvent::MouseButtonRelease)
            m_pos = QPoint(-1, -1);
    }

    return false;
}
