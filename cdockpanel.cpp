#include "cdockpanel.h"
#include "ui_dockpanel.h"
//--------------------------------------
CDockPanel::CDockPanel(QWidget* parent):
    QWidget(parent),
    ui(new Ui::CDockPanel)
{
    ui->setupUi(this);
}
//-----------------------
CDockPanel::~CDockPanel()
{
    delete ui;
}
