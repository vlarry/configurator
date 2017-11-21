#include "cterminal.h"
#include "ui_cterminal.h"
//------------------------------------
CTerminal::CTerminal(QWidget *parent):
    QWidget(parent),
    ui(new Ui::CTerminal)
{
    ui->setupUi(this);
    
    QPalette p = ui->pteConsole->palette();
    
    p.setColor(QPalette::Base, QColor(Qt::black));
    p.setColor(QPalette::Text, QColor(Qt::green));
    
    ui->pteConsole->setPalette(p);
    
    setWindowTitle(tr("Терминал"));
    setWindowFlag(Qt::Window);
}
//---------------------
CTerminal::~CTerminal()
{
    delete ui;
}
//----------------------------------------------------------
void CTerminal::appendData(QByteArray& data, bool isRequest)
{
    QString str = (isRequest)?tr("Запрос:\n"):tr("Ответ:\n");
    
    for(QString s: data)
    {
        str += s.toLocal8Bit().toHex() + " ";
    }
    
    str += tr("   (") + QString::number(data.count()) + tr(" байт)\n");
    
    ui->pteConsole->appendPlainText(str);
}
//--------------------------------------------
void CTerminal::closeEvent(QCloseEvent* event)
{
    emit closeTerminal(Qt::Unchecked);
}
