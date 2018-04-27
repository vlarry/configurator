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
        CIndicatorCell* cell = new CIndicatorCell;

        cell->setText(QString("%1").arg(++count));
        cell->setName(led);
        cell->setIcon(QPixmap(":/images/resource/images/led_off.png"));

        QListWidgetItem* item = new QListWidgetItem(ui->listWidgetLed);
        item->setToolTip(led);
        item->setSizeHint(cell->sizeHint());

        ui->listWidgetLed->setItemWidget(item, cell);
    }

    count = 0;

    for(const QString& relay: relay_list)
    {
        CIndicatorCell* cell = new CIndicatorCell;

        cell->setText(QString("%1").arg(++count));
        cell->setName(relay);

        RelayType type = ((count == 2 || count == 3 || count == 5 || count == 6 || count == 7)?RELAY_TYPE_TWO:
                                                                                               RELAY_TYPE_ONE);

        if(type == RELAY_TYPE_ONE)
            cell->setIcon(QPixmap(":/images/resource/images/relay_type1_off.png"));
        else if(type == RELAY_TYPE_TWO)
            cell->setIcon(QPixmap(":/images/resource/images/relay_type2_off.png"));

        QListWidgetItem* item = new QListWidgetItem(ui->listWidgetRelay);

        item->setToolTip(relay);
        item->setData(Qt::UserRole, type);
        item->setSizeHint(cell->sizeHint());

        ui->listWidgetRelay->setItemWidget(item, cell);
    }

    if(ui->listWidgetLed->count() > 0)
        ui->listWidgetLed->item(0)->setSelected(true);

    if(ui->listWidgetRelay->count() > 0)
        ui->listWidgetRelay->item(0)->setSelected(true);

    m_timer.start(1000);
}
//------------------------------------
QStringList CIndicatorState::ledList()
{
    QStringList list;

    for(int i = 0; i < ui->listWidgetLed->count(); i++)
    {
        QListWidgetItem* item = ui->listWidgetLed->item(i);

        if(item)
        {
            CIndicatorCell*  cell = static_cast<CIndicatorCell*>(ui->listWidgetLed->itemWidget(item));

            if(cell)
                list << cell->name();
        }
    }

    return list;
}
//--------------------------------------
QStringList CIndicatorState::relayList()
{
    QStringList list;

    for(int i = 0; i < ui->listWidgetRelay->count(); i++)
    {
        QListWidgetItem* item = ui->listWidgetRelay->item(i);

        if(item)
        {
            CIndicatorCell*  cell = static_cast<CIndicatorCell*>(ui->listWidgetRelay->itemWidget(item));

            if(cell)
                list << cell->name();
        }
    }

    return list;
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
