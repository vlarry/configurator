#include "menubar.h"
#include "ui_menubar.h"
//----------------------------------
CMenuBar::CMenuBar(QWidget* parent):
    QWidget(parent),
    ui(new Ui::CMenuBar)
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
