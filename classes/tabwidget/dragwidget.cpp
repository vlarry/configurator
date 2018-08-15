#include "dragwidget.h"
#include "ui_dragwidget.h"
//--------------------------------------------------------------
CDragWidget::CDragWidget(const QString& title, QWidget* parent):
    QWidget(parent),
    ui(new Ui::CDragWidget),
    m_widget(nullptr)
{
    ui->setupUi(this);

    QFont f = font();
    f.setBold(true);
    setFont(f);
    setWindowTitle(title);

//    this->setWindowFlags(Qt::SubWindow | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);

    ui->labelHeader->installEventFilter(this);
}
//-------------------------
CDragWidget::~CDragWidget()
{
    delete ui;
}
//------------------------------------------
void CDragWidget::setWidget(QWidget* widget)
{
    QLayout* l = layout();
    m_widget = widget;

    if(l)
    {
        l->addWidget(m_widget);
    }
}
//----------------------------------
QWidget* CDragWidget::widget() const
{
    return m_widget;
}
//----------------------------------------------------
void CDragWidget::setWindowTitle(const QString& title)
{
    ui->labelHeader->setText(title);
}
//----------------------------------------------
void CDragWidget::paintEvent(QPaintEvent* event)
{
    QWidget::paintEvent(event);

    QPainter painter(this);
    painter.setPen(Qt::gray);
    painter.drawLine(QPoint(0, 0), QPoint(rect().right(), 0));
    painter.drawLine(QPoint(rect().right(), 0), QPoint(rect().right(), rect().bottom()));
    painter.drawLine(QPoint(rect().right(), rect().bottom()), QPoint(0, rect().bottom()));
    painter.drawLine(QPoint(0, rect().bottom()), QPoint(0, 0));
}
//--------------------------------------------------------
bool CDragWidget::eventFilter(QObject* obj, QEvent* event)
{
    if(obj == ui->labelHeader)
    {
        if(event->type() == QEvent::MouseButtonPress)
        {
            QMouseEvent* me = dynamic_cast<QMouseEvent*>(event);

            m_mouse_pos = me->pos();

            return true;
        }
        else if(event->type() == QEvent::MouseMove)
        {
            QMouseEvent* me = dynamic_cast<QMouseEvent*>(event);
            QPoint delta = me->pos() - m_mouse_pos;
            move(pos() + delta);

            return true;
        }
    }

    return false;
}
