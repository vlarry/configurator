#include "cindicatorcell.h"
#include "ui_cindicatorcell.h"
//----------------------------------------------
CIndicatorCell::CIndicatorCell(QWidget* parent):
    QWidget(parent),
    ui(new Ui::CIndicatorCell)
{
    ui->setupUi(this);
}
//-------------------------------
CIndicatorCell::~CIndicatorCell()
{
    delete ui;
}
//-----------------------------------------------
void CIndicatorCell::setText(const QString& text)
{
    ui->labelText->setText(text);
}
//-----------------------------------------------
void CIndicatorCell::setIcon(const QPixmap& icon)
{
//    ui->labelIcon->setFixedSize(icon.size());
    ui->labelIcon->setPixmap(icon);
}
