#include "cfilterdialog.h"
#include "ui_filterdialog.h"
//-------------------------------------------------------------------
CFilterDialog::CFilterDialog(const CFilter& filter, QWidget* parent):
    QDialog(parent),
    ui(new Ui::CFilterDialog),
    m_btnGroup(nullptr),
    m_intervalMax(filter.interval().max)
{
    ui->setupUi(this);

    ui->widgetSliderInterval->SetRange(filter.interval().begin, filter.interval().end);
    ui->spinBoxIntervalFrom->setRange(filter.interval().begin, filter.interval().end - 1);
    ui->spinBoxIntervalTo->setRange(filter.interval().begin + 1, filter.interval().end);
    ui->spinBoxIntervalFrom->setValue(filter.interval().begin);
    ui->spinBoxIntervalTo->setValue(filter.interval().end);
    ui->calendarwgtBegin->setSelectedDate(filter.date().begin);
    ui->calendarwgtEnd->setSelectedDate(filter.date().end);

    ui->listWidgetFilterButton->addItems(QStringList() << tr("Интервал") << tr("Дата"));
    ui->listWidgetFilterButton->setCurrentRow(0);

    if(filter.interval().begin == 0 && filter.interval().end == 0)
    {
        ui->spinBoxIntervalFrom->setDisabled(true);
        ui->spinBoxIntervalTo->setDisabled(true);
        ui->widgetSliderInterval->setDisabled(true);
    }

    ui->stackwgtFilter->setCurrentIndex(0);

    ui->calendarwgtBegin->setSelectionMode(QCalendarWidget::SingleSelection);
    ui->calendarwgtEnd->setSelectionMode(QCalendarWidget::SingleSelection);

    connect(ui->listWidgetFilterButton, &QListWidget::currentRowChanged, this, &CFilterDialog::filterChanged);
    connect(ui->widgetSliderInterval, &RangeSlider::lowerValueChanged, this, &CFilterDialog::spinBoxIntervalFromChanged);
    connect(ui->widgetSliderInterval, &RangeSlider::upperValueChanged, this, &CFilterDialog::spinBoxIntervalToChanged);
    connect(ui->spinBoxIntervalFrom, &QSpinBox::editingFinished, this, &CFilterDialog::sliderIntervalFromChanged);
    connect(ui->spinBoxIntervalTo, &QSpinBox::editingFinished, this, &CFilterDialog::sliderIntervalToChanged);

    QObjectList objList = children();

    for(QObject* obj: objList)
    {
        if(QString(obj->metaObject()->className()).toUpper() == "QDIALOGBUTTONBOX")
        {
            QDialogButtonBox* buttons = qobject_cast<QDialogButtonBox*>(obj);

            if(buttons)
            {
                QPushButton* butOk     = qobject_cast<QPushButton*>(buttons->button(QDialogButtonBox::Yes));
                QPushButton* butCancel = qobject_cast<QPushButton*>(buttons->button(QDialogButtonBox::Cancel));

                if(butOk)
                    butOk->setText(tr("Ок"));

                if(butCancel)
                    butCancel->setText(tr("Отмена"));
            }
        }
    }
}
//------------------------------------------
void CFilterDialog::filterChanged(int index)
{
    if(index < 0 || index >= ui->stackwgtFilter->count())
        return;

    ui->stackwgtFilter->setCurrentIndex(index);
}
//-------------------------------------------------------
void CFilterDialog::spinBoxIntervalFromChanged(int value)
{
    if(value >= ui->widgetSliderInterval->GetUpperValue())
        ui->widgetSliderInterval->setLowerValue(--value);

    ui->spinBoxIntervalFrom->setValue(value);
}
//-----------------------------------------------------
void CFilterDialog::spinBoxIntervalToChanged(int value)
{
    if(value <= ui->widgetSliderInterval->GetLowerValue())
        ui->widgetSliderInterval->setUpperValue(++value);

    ui->spinBoxIntervalTo->setValue(value);
}
//---------------------------------------------
void CFilterDialog::sliderIntervalFromChanged()
{
    ui->widgetSliderInterval->setLowerValue(ui->spinBoxIntervalFrom->value());
}
//-------------------------------------------
void CFilterDialog::sliderIntervalToChanged()
{
    int value = ui->spinBoxIntervalTo->value();
    if(ui->spinBoxIntervalTo->value() <= ui->spinBoxIntervalFrom->value())
        value = ui->spinBoxIntervalFrom->value() + 1;

    ui->widgetSliderInterval->setUpperValue(value);
}
//----------------------------------------------
void CFilterDialog::showEvent(QShowEvent* event)
{
    QDialog::showEvent(event);
}
//-----------------------------------
const CFilter CFilterDialog::filter()
{
    CFilter::FilterDateType     date;
    CFilter::FilterIntervalType interval;

    date.begin = ui->calendarwgtBegin->selectedDate();
    date.end   = ui->calendarwgtEnd->selectedDate();

    interval.begin = ui->spinBoxIntervalFrom->value();
    interval.end   = ui->spinBoxIntervalTo->value();

    CFilter cfilter(interval, date, ui->timeEditBeginFilter->time());

    cfilter.setType(((ui->stackwgtFilter->currentIndex() == 0)?CFilter::INTERVAL:CFilter::DATE));
    cfilter.setState(true);

    return cfilter;
}
//---------------------------------------------------------
const CFilter::FilterIntervalType CFilterDialog::interval()
{
    CFilter::FilterIntervalType i;

    i.begin = ui->spinBoxIntervalFrom->value();
    i.end   = ui->spinBoxIntervalTo->value();

    return i;
}
//-------------------------------------------------
const CFilter::FilterDateType CFilterDialog::date()
{
    CFilter::FilterDateType d;

    d.begin = ui->calendarwgtBegin->selectedDate();
    d.end   = ui->calendarwgtEnd->selectedDate();

    return d;
}
//-------------------------------
const QTime CFilterDialog::time()
{
    return ui->timeEditBeginFilter->time();
}
