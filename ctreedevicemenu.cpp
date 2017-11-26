#include "ctreedevicemenu.h"
//------------------------------------------------
CTreeDeviceMenu::CTreeDeviceMenu(QWidget* parent):
    QTreeWidget(parent)
{
    QTreeWidgetItem* itemSettings   = new QTreeWidgetItem(this);
    QTreeWidgetItem* itemJournals   = new QTreeWidgetItem(this);
    QTreeWidgetItem* itemMeasures   = new QTreeWidgetItem(this);
    QTreeWidgetItem* itemMonitoring = new QTreeWidgetItem(this);

    QTreeWidgetItem* itemSetInputAnalogs   = new QTreeWidgetItem(itemSettings);
    QTreeWidgetItem* itemSetProtections    = new QTreeWidgetItem(itemSettings);
    QTreeWidgetItem* itemSetDevConnections = new QTreeWidgetItem(itemSettings);
    QTreeWidgetItem* itemSetAutomation     = new QTreeWidgetItem(itemSettings);

    QTreeWidgetItem* itemJournalCrashs = new QTreeWidgetItem(itemJournals);
    QTreeWidgetItem* itemJournalEvents = new QTreeWidgetItem(itemJournals);
    QTreeWidgetItem* itemJournalOscill = new QTreeWidgetItem(itemJournals);

    QTreeWidgetItem* itemMeasPrimaryValues   = new QTreeWidgetItem(itemMeasures);
    QTreeWidgetItem* itemMeasSecondaryValues = new QTreeWidgetItem(itemMeasures);
    QTreeWidgetItem* itemMeasPowerElectric   = new QTreeWidgetItem(itemMeasures);

    QTreeWidgetItem* itemMonitorInputDiscrets  = new QTreeWidgetItem(itemMonitoring);
    QTreeWidgetItem* itemMonitorOutputDiscrets = new QTreeWidgetItem(itemMonitoring);

    QTreeWidgetItem* itemInAnalogMain        = new QTreeWidgetItem(itemSetInputAnalogs);
    QTreeWidgetItem* itemInAnalogCalibration = new QTreeWidgetItem(itemSetInputAnalogs);

    QTreeWidgetItem* itemProtectCurrentMax  = new QTreeWidgetItem(itemSetProtections);
    QTreeWidgetItem* itemProtectEarthy      = new QTreeWidgetItem(itemSetProtections);
    QTreeWidgetItem* itemProtectPower       = new QTreeWidgetItem(itemSetProtections);
    QTreeWidgetItem* itemProtectMotor       = new QTreeWidgetItem(itemSetProtections);
    QTreeWidgetItem* itemProtectFrequency   = new QTreeWidgetItem(itemSetProtections);
    QTreeWidgetItem* itemProtectExternal    = new QTreeWidgetItem(itemSetProtections);
    QTreeWidgetItem* itemProtectTemperature = new QTreeWidgetItem(itemSetProtections);
    QTreeWidgetItem* itemProtectLevel       = new QTreeWidgetItem(itemSetProtections);

    itemSettings->setText(0, tr("Настройки"));
    itemJournals->setText(0, tr("Журналы"));
    itemMeasures->setText(0, tr("Измерения"));
    itemMonitoring->setText(0, tr("Мониторинг"));

    itemSetInputAnalogs->setText(0, tr("Аналоговые входы"));
    itemSetProtections->setText(0, tr("Защита"));
    itemSetDevConnections->setText(0, tr("Коммутационные аппараты"));
    itemSetAutomation->setText(0, tr("Автоматика"));

    itemJournalCrashs->setText(0, tr("Аварий"));
    itemJournalEvents->setText(0, tr("Событий"));
    itemJournalOscill->setText(0, tr("Осциллограф"));

    itemMeasPrimaryValues->setText(0, tr("Первичные величины"));
    itemMeasSecondaryValues->setText(0, tr("Вторичные величины"));
    itemMeasPowerElectric->setText(0, tr("Электроэнергия"));

    itemMonitorInputDiscrets->setText(0, tr("Дискретные входы"));
    itemMonitorOutputDiscrets->setText(0, tr("Дискретные выходы"));

    itemInAnalogMain->setText(0, tr("Основные"));
    itemInAnalogCalibration->setText(0, tr("Калибровки"));

    itemProtectCurrentMax->setText(0, tr("Максимальные токовые"));
    itemProtectEarthy->setText(0, tr("Земляные"));
    itemProtectPower->setText(0, tr("Защиты по напряжению"));
    itemProtectMotor->setText(0, tr("Защиты двигателя"));
    itemProtectFrequency->setText(0, tr("Защиты по частоте"));
    itemProtectExternal->setText(0, tr("Внешние защиты"));
    itemProtectTemperature->setText(0, tr("Температурные защиты"));
    itemProtectLevel->setText(0, tr("Уровневые защиты"));

    this->addTopLevelItem(itemSettings);
    this->addTopLevelItem(itemJournals);
    this->addTopLevelItem(itemMeasures);
    this->addTopLevelItem(itemMonitoring);

    itemSettings->addChild(itemSetInputAnalogs);
    itemSettings->addChild(itemSetProtections);
    itemSettings->addChild(itemSetDevConnections);
    itemSettings->addChild(itemSetAutomation);

    itemJournals->addChild(itemJournalCrashs);
    itemJournals->addChild(itemJournalEvents);
    itemJournals->addChild(itemJournalOscill);

    itemMeasures->addChild(itemMeasPrimaryValues);
    itemMeasures->addChild(itemMeasSecondaryValues);
    itemMeasures->addChild(itemMeasPowerElectric);

    itemMonitoring->addChild(itemMonitorInputDiscrets);
    itemMonitoring->addChild(itemMonitorOutputDiscrets);

    itemSetInputAnalogs->addChild(itemInAnalogMain);
    itemSetInputAnalogs->addChild(itemInAnalogCalibration);

    itemSetProtections->addChild(itemProtectCurrentMax);
    itemSetProtections->addChild(itemProtectEarthy);
    itemSetProtections->addChild(itemProtectPower);
    itemSetProtections->addChild(itemProtectMotor);
    itemSetProtections->addChild(itemProtectFrequency);
    itemSetProtections->addChild(itemProtectExternal);
    itemSetProtections->addChild(itemProtectTemperature);
    itemSetProtections->addChild(itemProtectLevel);
}
