#include "menubar.h"
#include "ui_menubar.h"
//----------------------------------
CMenuBar::CMenuBar(QWidget* parent):
    QWidget(parent),
    ui(new Ui::CMenuBar),
    m_mouse_pos(QPoint(-1, -1))
{
    ui->setupUi(this);

    connect(ui->toolButtonCloseWindow, &QToolButton::clicked, this, &CMenuBar::closeWindow);
    connect(ui->toolButtonExpandWindow, &QToolButton::clicked, this, &CMenuBar::expandedWindow);
    connect(ui->toolButtonMinimizeWindow, &QToolButton::clicked, this, &CMenuBar::minimizeWindow);
}
//-------------------
CMenuBar::~CMenuBar()
{
    delete ui;
}
//------------------------------------
QPoint CMenuBar::mousePosition() const
{
    return m_mouse_pos;
}
//------------------------------------------------
void CMenuBar::setWindowTitle(const QString& text)
{
    ui->labelTitleProgram->setText(text);
}
//------------------------------------------------
void CMenuBar::mousePressEvent(QMouseEvent* event)
{
    m_mouse_pos = event->pos();
}
//-----------------------------------------------
void CMenuBar::mouseMoveEvent(QMouseEvent* event)
{
    emit menubarMouseUpdatePosition(event->pos());
}
//--------------------------------------------------
void CMenuBar::mouseReleaseEvent(QMouseEvent* event)
{
    Q_UNUSED(event);
    m_mouse_pos = QPoint(-1, -1);
}
//------------------------------------------------------
void CMenuBar::mouseDoubleClickEvent(QMouseEvent* event)
{
    if(event->buttons() == Qt::LeftButton)
    {
        emit expandedWindow();
    }
}
