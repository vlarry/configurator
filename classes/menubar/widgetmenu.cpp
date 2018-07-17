#include "widgetmenu.h"
#include "ui_widgetmenu.h"
//----------------------------------------
CWidgetMenu::CWidgetMenu(QWidget* parent):
    QWidgetAction(parent),
    ui(new Ui::CWidgetMenu)
{
    ui->setupUi(this);
}
//-------------------------
CWidgetMenu::~CWidgetMenu()
{
    delete ui;
}
