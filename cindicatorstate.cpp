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
//-----------------------------------------------------------------------------------
void CIndicatorState::setLists(const QStringList& led_list, relay_list_t& relay_list)
{
    int count = 0;

    ui->listWidgetLed->setIconSize(QSize(32, 32));
    ui->listWidgetRelay->setIconSize(QSize(32, 32));

    for(const QString& led: led_list)
    {
        CIndicatorCell* cell = new CIndicatorCell;

        cell->setText(QString("%1").arg(++count));
        cell->setIcon(QPixmap(":/images/resource/images/led_off.png"));

        QListWidgetItem* item = new QListWidgetItem(ui->listWidgetLed);
        item->setToolTip(led);
        item->setSizeHint(cell->sizeHint());

        ui->listWidgetLed->setItemWidget(item, cell);
    }

    count = 0;

    for(relay_t& relay: relay_list)
    {
        CIndicatorCell* cell = new CIndicatorCell;

        cell->setText(QString("%1").arg(++count));

        if(relay.type == RELAY_TYPE_ONE)
            cell->setIcon(QPixmap(":/images/resource/images/relay_type1_off.png"));
        else if(relay.type == RELAY_TYPE_TWO)
            cell->setIcon(QPixmap(":/images/resource/images/relay_type2_off.png"));

        QListWidgetItem* item = new QListWidgetItem(ui->listWidgetRelay);

        item->setToolTip(relay.description);
        item->setData(Qt::UserRole, relay.type);
        item->setSizeHint(cell->sizeHint());

        ui->listWidgetRelay->setItemWidget(item, cell);
    }

    m_timer.start(1000);
}
//---------------------------------
void CIndicatorState::changeState()
{
    if(m_state)
    {
        for(int i = 0; i < ui->listWidgetLed->count(); i++)
        {
            QListWidgetItem* item = ui->listWidgetLed->item(i);

            if(item)
            {
                CIndicatorCell*  cell = static_cast<CIndicatorCell*>(ui->listWidgetLed->itemWidget(item));

                if(cell)
                {
                    cell->setIcon(QPixmap(":/images/resource/images/led_off.png"));
                }
            }
        }

        for(int i = 0; i < ui->listWidgetRelay->count(); i++)
        {
            QListWidgetItem* item = ui->listWidgetRelay->item(i);

            if(item)
            {
                CIndicatorCell* cell = static_cast<CIndicatorCell*>(ui->listWidgetRelay->itemWidget(item));

                if(cell)
                {
                    if(RelayType(item->data(Qt::UserRole).toInt()) == RELAY_TYPE_ONE)
                        cell->setIcon(QPixmap(":/images/resource/images/relay_type1_off.png"));
                    else if(RelayType(item->data(Qt::UserRole).toInt()) == RELAY_TYPE_TWO)
                        cell->setIcon(QPixmap(":/images/resource/images/relay_type2_off.png"));
                }
            }
        }
    }
    else
    {
        for(int i = 0; i < ui->listWidgetLed->count(); i++)
        {
            QListWidgetItem* item = ui->listWidgetLed->item(i);

            if(item)
            {
                CIndicatorCell*  cell = static_cast<CIndicatorCell*>(ui->listWidgetLed->itemWidget(item));

                if(cell)
                {
                    cell->setIcon(QPixmap(":/images/resource/images/led_on.png"));
                }
            }
        }

        for(int i = 0; i < ui->listWidgetRelay->count(); i++)
        {
            QListWidgetItem* item = ui->listWidgetRelay->item(i);

            if(item)
            {
                CIndicatorCell* cell = static_cast<CIndicatorCell*>(ui->listWidgetRelay->itemWidget(item));

                if(cell)
                {
                    if(RelayType(item->data(Qt::UserRole).toInt()) == RELAY_TYPE_ONE)
                        cell->setIcon(QPixmap(":/images/resource/images/relay_type1_on.png"));
                    else if(RelayType(item->data(Qt::UserRole).toInt()) == RELAY_TYPE_TWO)
                        cell->setIcon(QPixmap(":/images/resource/images/relay_type2_on.png"));
                }
            }
        }
    }

    m_state = !m_state;
}
//--------------------------------------------------
void CIndicatorState::closeEvent(QCloseEvent* event)
{
    emit closeWindowIndicator(Qt::Unchecked);

    QWidget::closeEvent(event);
}
