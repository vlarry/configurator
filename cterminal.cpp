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
//------------------------------------------
void CTerminal::appendData(QByteArray& data)
{
    Q_UNUSED(data);
}
//--------------------------------------------
void CTerminal::closeEvent(QCloseEvent* event)
{
    QWidget::closeEvent(event);
    
    emit close(Qt::Unchecked);
}
