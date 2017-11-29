#include "cversionsoftware.h"
#include "ui_versionform.h"
//--------------------------------------------------
CVersionSoftware::CVersionSoftware(QWidget *parent):
    QWidget(parent),
    ui(new Ui::CVersionSoftware)
{
    ui->setupUi(this);
    setWindowFlag(Qt::Window);

    ui->plainTextEdit->clear();

    QPalette p = ui->plainTextEdit->palette();

    p.setColor(QPalette::Base, QColor(Qt::black));
    p.setColor(QPalette::Text, QColor(Qt::gray));

    ui->plainTextEdit->setPalette(p);

    this->setWindowTitle(tr("История версий"));
}
//-----------------------------------
CVersionSoftware::~CVersionSoftware()
{
    delete ui;
}
//----------------------------------------------------------------
void CVersionSoftware::setText(const QMap<QString, QString>& data)
{
    for(int i = 0; i < data.keys().count(); i++)
    {
        QString key = data.keys().at(i);
        QString str = key + tr(":\n") + data.value(key) + tr("\n");

        ui->plainTextEdit->appendPlainText(str);
    }
}
