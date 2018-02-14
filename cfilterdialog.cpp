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

    int count = filter.interval().end - filter.interval().begin;

    ui->spinboxIntervalBegin->setValue(filter.interval().begin);
    ui->spinboxIntervalCount->setValue(((count > 0)?count:0));
    ui->spinboxIntervalCount->setMaximum(((count > 0)?count:0));
    ui->calendarwgtBegin->setSelectedDate(filter.date().begin);
    ui->calendarwgtEnd->setSelectedDate(filter.date().end);

    if(filter.interval().begin == 0 && filter.interval().end == 0)
        ui->groupboxFilterInterval->setDisabled(true);

    m_btnGroup = new QButtonGroup;

    m_btnGroup->addButton(ui->pushbtnFilterInterval, 0);
    m_btnGroup->addButton(ui->pushbtnFilterDate, 1);

    ui->stackwgtFilter->setCurrentIndex(0);

    ui->calendarwgtBegin->setSelectionMode(QCalendarWidget::SingleSelection);
    ui->calendarwgtEnd->setSelectionMode(QCalendarWidget::SingleSelection);

    connect(m_btnGroup, static_cast<void (QButtonGroup::*)(QAbstractButton*)>(&QButtonGroup::buttonClicked), this,
                                                                              &CFilterDialog::filterChanged);
    connect(ui->spinboxIntervalBegin, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this,
                                                                           &CFilterDialog::intervalChanged);
}
//--------------------------------------------------------
void CFilterDialog::filterChanged(QAbstractButton* button)
{
    ui->stackwgtFilter->setCurrentIndex(m_btnGroup->id(button));
}
//--------------------------------------------
void CFilterDialog::intervalChanged(int value)
{
    int count = m_intervalMax - value;

    ui->spinboxIntervalCount->setMaximum(count);
    ui->spinboxIntervalCount->setValue(count);
}
//-----------------------------------
const CFilter CFilterDialog::filter()
{
    CFilter::FilterDateType     date;
    CFilter::FilterIntervalType interval;

    date.begin = ui->calendarwgtBegin->selectedDate();
    date.end   = ui->calendarwgtEnd->selectedDate();

    interval.begin = ui->spinboxIntervalBegin->value();
    interval.end   = ui->spinboxIntervalBegin->value() + ui->spinboxIntervalCount->value();

    CFilter cfilter(interval, date);

    cfilter.setType(((ui->stackwgtFilter->currentIndex() == 0)?CFilter::INTERVAL:CFilter::DATE));
    cfilter.setState(true);

    return cfilter;
}
//---------------------------------------------------------
const CFilter::FilterIntervalType CFilterDialog::interval()
{
    CFilter::FilterIntervalType i;

    i.begin = ui->spinboxIntervalBegin->value();
    i.end   = ui->spinboxIntervalBegin->value() + ui->spinboxIntervalCount->value();

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
