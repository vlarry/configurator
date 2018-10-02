#include "cterminal.h"
#include "ui_cterminal.h"
//------------------------------------
CTerminal::CTerminal(QWidget* parent):
    QWidget(parent),
    ui(new Ui::CTerminal)
{
    ui->setupUi(this);
    
    QPalette p = ui->pteConsole->palette();
    
    p.setColor(QPalette::Base, QColor(Qt::black));
    p.setColor(QPalette::Text, QColor(Qt::green));

    ui->lineEditDeviceCommand->setPalette(p);
    
    setWindowTitle(tr("Терминал"));
    setWindowFlag(Qt::Window);

    ui->lineEditDeviceCommand->installEventFilter(this);
    ui->lineEditDeviceCommand->setValidator(new QIntValidator(1, 99));

    connect(ui->pushButtonSendDeviceCommand, &QPushButton::clicked, this, &CTerminal::convertDeviceCommand);
    setAcceptDrops(true);

    setMouseTracking(true);

    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnBottomHint);
}
//---------------------
CTerminal::~CTerminal()
{
    delete ui;
}
//--------------------
void CTerminal::show()
{
    QWidget::show();

    if(!ui->lineEditDeviceCommand->text().isEmpty())
        ui->lineEditDeviceCommand->selectAll();
}
//----------------------------------------------------------
void CTerminal::appendData(QByteArray& data, bool isRequest)
{
    ui->pteConsole->appendPlainText(((isRequest)?tr("Запрос: "):tr("Ответ: ")) + data.toHex(' ').toUpper() + QString(5, ' ') +
                                    tr("(%1 байт).\n").arg(QString::number(data.count())));
}
//------------------------------------
void CTerminal::convertDeviceCommand()
{
    if(ui->lineEditDeviceCommand->text().isEmpty())
        return;

    int cmd = ui->lineEditDeviceCommand->text().toInt();

    if(cmd > 0)
        emit sendDeviceCommand(cmd);
}
//--------------------------------------------
void CTerminal::closeEvent(QCloseEvent* event)
{
    emit closeTerminal(Qt::Unchecked);
    
    QWidget::closeEvent(event);
}
//----------------------------------------------------------
bool CTerminal::eventFilter(QObject* watched, QEvent* event)
{
    if(watched == ui->lineEditDeviceCommand)
    {
        if(event->type() == QEvent::KeyPress)
        {
            QKeyEvent* ke = static_cast<QKeyEvent*>(event);

            if(ke)
            {
                if(ke->key() == Qt::Key_Return)
                {
                    ui->lineEditDeviceCommand->selectAll();
                    convertDeviceCommand();

                    return true;
                }
            }
        }
        else if(event->type() == event->MouseButtonPress)
        {
            ui->lineEditDeviceCommand->selectAll();

            return true;
        }
    }

    return false;
}
