#include "cfilterdialog.h"
#include "ui_filterdialog.h"
//--------------------------------------------------------------------------
CFilterDialog::CFilterDialog(const FilterValueType& value, QWidget* parent):
    QDialog(parent),
    ui(new Ui::CFilterDialog),
    m_btnGroup(nullptr)
{
    initFilter();
    setValue(value);
}
//--------------------------------------------
CFilterDialog::CFilterDialog(QWidget* parent):
    QDialog(parent),
    ui(new Ui::CFilterDialog),
    m_btnGroup(nullptr)
{
    initFilter();
}
//-----------------------------
CFilterDialog::~CFilterDialog()
{
    delete ui;
}
//---------------------------------------------------
CFilterDialog::FilterValueType CFilterDialog::value()
{
    return FilterValueType({ FilterType(ui->stackwgtFilter->currentIndex()), m_intervalMax, ui->spinboxIntervalBegin->value(),
                             ui->spinboxIntervalCount->value(), ui->calendarwgtBegin->selectedDate(),
                                                                ui->calendarwgtEnd->selectedDate() });
}
//--------------------------------------------------------
void CFilterDialog::setValue(const FilterValueType& value)
{
    m_intervalMax = value.intervalMax;

    ui->spinboxIntervalBegin->setValue(value.intervalBegin);
    ui->spinboxIntervalCount->setValue(value.intervalCount);
    ui->spinboxIntervalCount->setMaximum(value.intervalMax - value.intervalBegin);
    ui->calendarwgtBegin->setSelectedDate(value.dateBegin);
    ui->calendarwgtEnd->setSelectedDate(value.dateEnd);

    ui->stackwgtFilter->setCurrentIndex(int(value.type));
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
//------------------------------
void CFilterDialog::initFilter()
{
    ui->setupUi(this);

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
//----------------------------------------------
void CFilterDialog::showEvent(QShowEvent* event)
{
    QDialog::showEvent(event);
}
