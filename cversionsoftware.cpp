#include "cversionsoftware.h"
#include "ui_versionform.h"
//--------------------------------------------------
CVersionSoftware::CVersionSoftware(QWidget *parent):
    QWidget(parent),
    ui(new Ui::CVersionSoftware)
{
    ui->setupUi(this);
    setWindowFlag(Qt::Window);

    ui->textEdit->clear();
    QFont font = ui->textEdit->font();
    font.setPointSize(14);
    ui->textEdit->setFont(font);

    QPalette p = ui->textEdit->palette();

    p.setColor(QPalette::Base, QColor(Qt::black));
    p.setColor(QPalette::Text, QColor(Qt::gray));

    ui->textEdit->setPalette(p);

    this->setWindowTitle(tr("История версий"));
}
//-----------------------------------
CVersionSoftware::~CVersionSoftware()
{
    delete ui;
}
//---------------------------------------------------------------------------
void CVersionSoftware::setText(const QVector<QPair<QString, QString> >& data)
{
    for(int i = 0; i < data.count(); i++)
    {
        QString key = data[i].first;
        QString str = key + tr(":\n") + data[i].second + tr("\n");

        if(i == data.count() - 1)
            ui->textEdit->setTextColor(Qt::green);
        else
            ui->textEdit->setTextColor(Qt::gray);

        ui->textEdit->append(str);
    }
}
