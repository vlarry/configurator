#include "containerwidget.h"
#include "ui_containerwidget.h"
//-------------------------------------------------------------------------------------------------------------------
CContainerWidget::CContainerWidget(const QString& title, QWidget* contentWidget, AnchorType anchor, QWidget* parent):
    QWidget(parent),
    ui(new Ui::CContainerWidget),
    m_superParent(nullptr),
    m_contentWidget(nullptr),
    m_anchor(anchor),
    m_pos(QPoint(-1, -1)),
    m_size_pos(QPoint(-1, -1)),
    m_id(-1),
    m_background_color(QColor())
{
    ui->setupUi(this);

    setMouseTracking(true);
    setAutoFillBackground(true);
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    ui->toolButtonHeaderFunction->hide();
    ui->toolButtonHeaderClose->hide();
    ui->labelHeader->setText(title);
    ui->labelHeader->setAlignment(Qt::AlignCenter);
    ui->labelHeader->setAutoFillBackground(true);
    ui->labelHeader->installEventFilter(this);
    ui->toolButtonGrip->installEventFilter(this);
    ui->toolButtonGrip->hide();

    QFont f(ui->labelHeader->font());
    f.setPointSize(10);
    f.setBold(true);
    ui->labelHeader->setFont(f);

    setAnchor(m_anchor);
    setWidget(contentWidget);
    resize(800, 600);
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
//-----------------------------------------
void CContainerWidget::functionButtonHide()
{
    ui->toolButtonHeaderFunction->hide();
}
//-----------------------------------------
void CContainerWidget::functionButtonShow()
{
    ui->toolButtonHeaderFunction->show();
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
//------------------------------------------------
bool CContainerWidget::buttonFunctionState() const
{
    return !ui->toolButtonHeaderFunction->isHidden();
}
//---------------------------------------------------
QToolButton* CContainerWidget::buttonFunction() const
{
    return ui->toolButtonHeaderFunction;
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
    m_contentWidget->setMouseTracking(true);

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

    QLinearGradient gradientButton(0, 0, 0, ui->toolButtonHeaderFunction->height());
    gradient.setColorAt(0, backgroundColor);
    gradient.setColorAt(0.5, Qt::gray);
    gradient.setColorAt(1, backgroundColor);

    QPalette palBut(ui->toolButtonHeaderFunction->palette());
    palBut.setBrush(QPalette::Button, QBrush(gradientButton));
    ui->toolButtonHeaderFunction->setPalette(palBut);
}
//-------------------------------------------------------------------
void CContainerWidget::setAnchor(CContainerWidget::AnchorType anchor)
{
    m_anchor = anchor;

    if(m_anchor == AnchorType::AnchorFree)
    {
        ui->toolButtonHeaderClose->show();
        ui->toolButtonGrip->show();
        connect(ui->toolButtonHeaderClose, &QToolButton::clicked, this, &CContainerWidget::close);
    }
    else
    {
        disconnect(ui->toolButtonHeaderClose, &QToolButton::clicked, this, &CContainerWidget::close);
        ui->toolButtonHeaderClose->hide();
        ui->toolButtonGrip->hide();
    }
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
//-------------------------------------------------------
void CContainerWidget::setButtonFunctionState(bool state)
{
    if(state)
    {
        buttonFunctionStateChanged(false);
        ui->toolButtonHeaderFunction->show();
        connect(ui->toolButtonHeaderFunction, &QToolButton::clicked, this, &CContainerWidget::buttonFunctionStateChanged);
    }
    else
    {
        ui->toolButtonHeaderFunction->hide();
    }
}
//-----------------------------------------------------------
void CContainerWidget::buttonFunctionStateChanged(bool state)
{
    if(state)
    {
        ui->toolButtonHeaderFunction->setIcon(QIcon(":/images/resource/images/branch_open.png"));
        ui->toolButtonHeaderFunction->setToolTip(tr("Свернуть меню"));
    }
    else
    {
        ui->toolButtonHeaderFunction->setIcon(QIcon(":/images/resource/images/branch_close.png"));
        ui->toolButtonHeaderFunction->setToolTip(tr("Развернуть меню"));
    }
}
//----------------------------------------
void CContainerWidget::expandedContainer()
{
    Qt::WindowStates states = windowState();

    if(!(states & Qt::WindowMaximized))
        showMaximized();
    else
        showNormal();
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
                    QDrag* drag = new QDrag(this);
                    QMimeData* mimedata = new QMimeData;

                    mimedata->setProperty("CONTAINER", QVariant::fromValue(this));
                    mimedata->setData("application/widget_container", QByteArray());
                    drag->setMimeData(mimedata);

                    QPixmap pixmap(size());
                    render(&pixmap);
                    drag->setPixmap(pixmap);

                    close();

                    if(m_anchor == AnchorType::AnchorDockWidget)
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
                            tcontainer->setAnchor(AnchorType::AnchorFree);
                        }
                    }
                }
            }
        }
        else if(event->type() == QEvent::MouseButtonRelease)
            m_pos = QPoint(-1, -1);
    }
    else if(object == ui->toolButtonGrip)
    {
        if(event->type() == QEvent::MouseButtonPress)
        {
            QMouseEvent* me = static_cast<QMouseEvent*>(event);

            if(me->button() == Qt::LeftButton)
            {
                qDebug() << "press";
                m_size_pos = me->pos();
            }
        }
        else if(event->type() == QEvent::MouseMove)
        {
            QMouseEvent* me = static_cast<QMouseEvent*>(event);

            if(me->buttons() & Qt::LeftButton)
            {
                QPoint offset = me->pos() - m_size_pos;
                qDebug() << "move";
                resize(geometry().width() + offset.x(), geometry().height() + offset.y());
            }
        }
        else if(event->type() == QEvent::MouseButtonRelease)
        {
            QMouseEvent* me = static_cast<QMouseEvent*>(event);

            if(me->buttons() & Qt::LeftButton)
            {
                qDebug() << "release";
                m_size_pos = QPoint(-1, -1);
            }
        }
    }

    return false;
}
