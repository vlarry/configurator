#include "dockwidget.h"
#include "ui_dockwidget.h"
//----------------------------------------
CDockWidget::CDockWidget(QWidget* parent):
    QWidget(parent),
    ui(new Ui::CDockWidget)
{
    ui->setupUi(this);
    ui->gridLayoutContainer->setColumnStretch(0, 1);
}
//-------------------------
CDockWidget::~CDockWidget()
{
    delete ui;
}
//---------------------------------------
void CDockWidget::addWidget(QWidget* wgt)
{
    if(wgt)
    {
        ui->gridLayoutContainer->addWidget(wgt, ui->gridLayoutContainer->rowCount(), 0);
    }
}
//-----------------------------------------------
QWidget* CDockWidget::widget(const QString& type)
{
    QWidget* wgt = nullptr;

    if(!type.isEmpty() && !ui->gridLayoutContainer->isEmpty())
    {
        for(int row = 0; row < ui->gridLayoutContainer->rowCount(); row++)
        {
            QLayoutItem* item = ui->gridLayoutContainer->itemAt(row);

            if(item)
            {
                QWidget* twgt = item->widget();

                if(twgt && twgt->isWidgetType())
                {
                    if(twgt->property("TYPE").toString().toUpper() == type.toUpper())
                    {
                        wgt = twgt;
                        ui->gridLayoutContainer->removeItem(item);
                        break;
                    }
                }
            }
        }
    }

    return wgt;
}
