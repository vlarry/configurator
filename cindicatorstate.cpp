#include "cindicatorstate.h"
#include "ui_indicatorstate.h"
//------------------------------------------------
CIndicatorState::CIndicatorState(QWidget* parent):
    QWidget(parent),
    ui(new Ui::CIndicatorState)
{
    ui->setupUi(this);

    setWindowTitle(tr("Состояния выходов"));
    setWindowFlag(Qt::Window);
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
}
//-----------------------------------------------------------------
void CIndicatorState::setOutputStates(const QVector<quint16>& data)
{
    if(data.isEmpty() || data.count() == 1 || ui->listWidgetLed->count() == 0 || ui->listWidgetRelay->count() == 0)
        return;

    for(int i = 0; i < ui->listWidgetRelay->count(); i++)
    {
        QListWidgetItem* item = ui->listWidgetRelay->item(i);

        if(item)
        {
            CIndicatorCell* cell = qobject_cast<CIndicatorCell*>(ui->listWidgetRelay->itemWidget(item));

            if(cell)
            {
                RelayType type  = RelayType(item->data(Qt::UserRole).toInt());
                bool      state = (data[0]&(1 << i));

                if(state)
                {
                    if(type == RELAY_TYPE_ONE)
                        cell->setIcon(QPixmap(":/images/resource/images/relay_type1_on.png"));
                    else if(type == RELAY_TYPE_TWO)
                        cell->setIcon(QPixmap(":/images/resource/images/relay_type2_on.png"));
                }
                else
                {
                    if(type == RELAY_TYPE_ONE)
                        cell->setIcon(QPixmap(":/images/resource/images/relay_type1_off.png"));
                    else if(type == RELAY_TYPE_TWO)
                        cell->setIcon(QPixmap(":/images/resource/images/relay_type2_off.png"));
                }
            }
        }
    }

    quint16 led_bits = ((data[0]&0xF000) >> 12) | (((data[1])&0x000F) << 4);

    for(int i = 0; i < ui->listWidgetLed->count(); i++)
    {
        QListWidgetItem* item = ui->listWidgetLed->item(i);

        if(item)
        {
            CIndicatorCell* cell = static_cast<CIndicatorCell*>(ui->listWidgetLed->itemWidget(item));

            if(cell)
            {
                bool    state = (led_bits&(1 << i));
                QString icon = "";

                if(state)
                    icon = ":/images/resource/images/led_on.png";
                else
                    icon = ":/images/resource/images/led_off.png";

                if(!icon.isEmpty())
                    cell->setIcon(QPixmap(icon));
            }
        }
    }
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
//--------------------------------------------------
void CIndicatorState::closeEvent(QCloseEvent* event)
{
    emit closeWindow();

    QWidget::closeEvent(event);
}
