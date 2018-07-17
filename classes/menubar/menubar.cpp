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

    QMenu* menu = new QMenu(ui->toolButtonMenuIcon);

    menu->addAction(QIcon(":/images/resource/images/new.png"),
                    tr("&Новый"),
                    this,
                    &CMenuBar::newAction,
                    Qt::CTRL + Qt::Key_N);
    menu->addAction(QIcon(":/images/resource/images/open.png"),
                    tr("&Открыть"),
                    this,
                    &CMenuBar::openAction,
                    Qt::CTRL + Qt::Key_O);
    menu->addAction(QIcon(":/images/resource/images/save.png"),
                    tr("&Сохранить"),
                    this,
                    &CMenuBar::saveAction,
                    Qt::CTRL + Qt::Key_S);
    menu->addAction(QIcon(":/images/resource/images/save_as.png"),
                    tr("&Сохранить как..."),
                    this,
                    &CMenuBar::saveAction);
    QMenu* menuExport = new QMenu(tr("&Экспорт..."), menu);
    menu->addMenu(menuExport);
    menuExport->addAction(QIcon(":/images/resource/images/export_pdf.png"),
                    tr("&Экспорт в PDF"),
                    this,
                    &CMenuBar::exportToPDFAction);
    menuExport->addAction(QIcon(":/images/resource/images/export_excel.png"),
                    tr("&Экспорт в Excel"),
                    this,
                    &CMenuBar::exportToExcelAction);
    menu->addSeparator();
    menu->addAction(QIcon(":/images/resource/images/exit.png"),
                    tr("&Закрыть"),
                    this,
                    &CMenuBar::closeWindow,
                    Qt::ALT + Qt::Key_F4);

    ui->toolButtonMenuIcon->setMenu(menu);
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
