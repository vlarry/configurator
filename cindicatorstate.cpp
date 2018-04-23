#include "cindicatorstate.h"
#include "ui_indicatorstate.h"
//------------------------------------------------
CIndicatorState::CIndicatorState(QWidget* parent):
    QWidget(parent),
    ui(new Ui::CIndicatorState),
    m_state(false)
{
    ui->setupUi(this);

    setWindowTitle(tr("Состояния индикаторов"));
    setWindowFlag(Qt::Window);

    connect(&m_timer, &QTimer::timeout, this, &CIndicatorState::changeState);
}
//---------------------------------
CIndicatorState::~CIndicatorState()
{
    delete ui;
}
//----------------------------------------------------------------------------------------
void CIndicatorState::setLists(const QStringList& led_list, const QStringList& relay_list)
{
    int count = 0;

    ui->listWidgetLed->setIconSize(QSize(32, 32));
    ui->listWidgetRelay->setIconSize(QSize(32, 32));

    for(const QString& led: led_list)
    {
        QListWidgetItem* item = new QListWidgetItem(QString("%1").arg(++count));

        item->setToolTip(led);
        item->setIcon(QPixmap(":/images/resource/images/led_off.png"));
        item->setTextAlignment(Qt::AlignRight);

        ui->listWidgetLed->addItem(item);
    }

    count = 0;

    for(const QString& relay: relay_list)
    {
        QListWidgetItem* item = new QListWidgetItem(QString("%1").arg(++count));

        item->setToolTip(relay);

        if(count == 3 || count == 4 || count == 5 || count ==6)
            item->setIcon(QPixmap(":/images/resource/images/relay_type2_off.png"));
        else
            item->setIcon(QPixmap(":/images/resource/images/relay_type1_off.png"));

        item->setTextAlignment(Qt::AlignRight);

        ui->listWidgetRelay->addItem(item);
    }

    m_timer.start(1000);
}
//---------------------------------
void CIndicatorState::changeState()
{
    if(m_state)
    {
        ui->listWidgetLed->item(0)->setIcon(QPixmap(":/images/resource/images/led_off.png"));
        ui->listWidgetRelay->item(0)->setIcon(QPixmap(":/images/resource/images/relay_type1_off.png"));
        ui->listWidgetRelay->item(4)->setIcon(QPixmap(":/images/resource/images/relay_type2_off.png"));
    }
    else
    {
        ui->listWidgetLed->item(0)->setIcon(QPixmap(":/images/resource/images/led_on.png"));
        ui->listWidgetRelay->item(0)->setIcon(QPixmap(":/images/resource/images/relay_type1_on.png"));
        ui->listWidgetRelay->item(4)->setIcon(QPixmap(":/images/resource/images/relay_type2_on.png"));
    }

    m_state = !m_state;
}
//--------------------------------------------------
void CIndicatorState::closeEvent(QCloseEvent* event)
{
    emit closeWindowIndicator(Qt::Unchecked);

    QWidget::closeEvent(event);
}
