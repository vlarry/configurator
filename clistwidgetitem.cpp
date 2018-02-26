#include "clistwidgetitem.h"
#include "ui_listwidgetitem.h"
//------------------------------------------------
CListWidgetItem::CListWidgetItem(QWidget* parent):
    QWidget(parent),
    ui(new Ui::CListWidgetItem)
{
    ui->setupUi(this);
}
//---------------------------------
CListWidgetItem::~CListWidgetItem()
{
    delete ui;
}
//------------------------------------------------------------------------------
void CListWidgetItem::setPropertyData(const QString& name, const QString& value)
{
    ui->labelPropertyName->setText(name);
    ui->labelPropertyValue->setText(value);
}
//--------------------------------------------------------
void CListWidgetItem::setPropertyName(const QString& name)
{
    ui->labelPropertyName->setText(name);
}
//----------------------------------------------------------
void CListWidgetItem::setPropertyValue(const QString& value)
{
    ui->labelPropertyValue->setText(value);
}
