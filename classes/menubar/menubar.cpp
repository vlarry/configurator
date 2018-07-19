#include "menubar.h"
#include "ui_menubar.h"
//----------------------------------
CMenuBar::CMenuBar(QWidget* parent):
    QWidget(parent),
    ui(new Ui::CMenuBar),
    m_mouse_pos(QPoint(-1, -1))
{
    ui->setupUi(this);

    QMenu*         menu         = new QMenu;
    QWidgetAction* widgetAction = new QWidgetAction(this);

    m_widgetMenu = new CWidgetMenu(menu);

    widgetAction->setDefaultWidget(m_widgetMenu);
    menu->addAction(widgetAction);

    ui->toolButtonMenuIcon->setMenu(menu);

    QMenu* otherButtonMenu = new QMenu(this);

    QAction* actionMinimizeTabMenu = otherButtonMenu->addAction(tr("Минимизировать ленту"));

    actionMinimizeTabMenu->setCheckable(true);

    ui->toolButtonMinimizeTabMenu->setMenu(otherButtonMenu);

    ui->toolButtonMinimizeTabMenu->hide(); // скрытие кнопки, т.к. не удалось пока реализовать функционал

    connect(ui->toolButtonCloseWindow, &QToolButton::clicked, this, &CMenuBar::closeWindow);
    connect(ui->toolButtonExpandWindow, &QToolButton::clicked, this, &CMenuBar::expandedWindow);
    connect(ui->toolButtonMinimizeWindow, &QToolButton::clicked, this, &CMenuBar::minimizeWindow);
    connect(actionMinimizeTabMenu, &QAction::triggered, this, &CMenuBar::minimizeMenu);
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
//---------------------------------------
CWidgetMenu* CMenuBar::widgetMenu() const
{
    return m_widgetMenu;
}
//--------------------------------
void CMenuBar::exportToPDFAction()
{
    qInfo() << tr("Пункт меню: Экспорт проекта в PDF");
}
//----------------------------------
void CMenuBar::exportToExcelAction()
{
    qInfo() << tr("Пункт меню: Экспорт проекта в Excel");
}
//------------------------
void CMenuBar::newAction()
{
    qInfo() << tr("Пункт меню: Новый проект");
}
//-------------------
void CMenuBar::openAction()
{
    qInfo() << tr("Пункт меню: Открыть проект");
}
//-------------------------
void CMenuBar::saveAction()
{
    qInfo() << tr("Пункт меню: Сохранить проект");
}
//---------------------------
void CMenuBar::saveAsAction()
{
    qInfo() << tr("Пункт меню: Сохранить проект как...");
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
