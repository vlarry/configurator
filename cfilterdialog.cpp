#include "cfilterdialog.h"
#include "ui_filterdialog.h"
//-------------------------------------------------------------------
CFilterDialog::CFilterDialog(const CFilter& filter, QWidget* parent):
    QDialog(parent),
    ui(new Ui::CFilterDialog),
    m_btnGroup(nullptr),
    m_filter(filter)
{
    ui->setupUi(this);

    ui->stackwgtFilter->setCurrentIndex(0);

    ui->calendarwgtBegin->setSelectionMode(QCalendarWidget::SingleSelection);
    ui->calendarwgtEnd->setSelectionMode(QCalendarWidget::SingleSelection);

    connect(ui->listWidgetFilterButton, &QListWidget::currentRowChanged, this, &CFilterDialog::filterChanged);
    connect(ui->widgetSliderInterval, &RangeSlider::lowerValueChanged, this, &CFilterDialog::spinBoxIntervalFromChanged);
    connect(ui->widgetSliderInterval, &RangeSlider::upperValueChanged, this, &CFilterDialog::spinBoxIntervalToChanged);
    connect(ui->spinBoxIntervalFrom, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this,
            &CFilterDialog::sliderIntervalFromChanged);
    connect(ui->spinBoxIntervalTo, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this,
            &CFilterDialog::sliderIntervalToChanged);

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
    if(value < ui->widgetSliderInterval->GetUpperValue())
        ui->spinBoxIntervalFrom->setValue(value);
}
//-----------------------------------------------------
void CFilterDialog::spinBoxIntervalToChanged(int value)
{
    if(value > ui->widgetSliderInterval->GetLowerValue())
        ui->spinBoxIntervalTo->setValue(value);
}
//------------------------------------------------------
void CFilterDialog::sliderIntervalFromChanged(int value)
{
    if(value < ui->widgetSliderInterval->GetUpperValue())
        ui->widgetSliderInterval->SetLowerValue(value);
}
//----------------------------------------------------
void CFilterDialog::sliderIntervalToChanged(int value)
{
    if(value > ui->widgetSliderInterval->GetLowerValue())
        ui->widgetSliderInterval->SetUpperValue(value);
}
//----------------------------------------------
void CFilterDialog::showEvent(QShowEvent* event)
{
    QDialog::showEvent(event);

    ui->widgetSliderInterval->SetRange(m_filter.rangeMinValue(), m_filter.rangeMaxValue());
    ui->spinBoxIntervalFrom->setRange(m_filter.rangeMinValue(), m_filter.rangeMaxValue());
    ui->spinBoxIntervalTo->setRange(m_filter.rangeMinValue(), m_filter.rangeMaxValue());
    ui->spinBoxIntervalFrom->setValue(m_filter.limitLowValue());
    ui->spinBoxIntervalTo->setValue(m_filter.limitUpperValue());
    ui->calendarwgtBegin->setSelectedDate(m_filter.dateFrom());
    ui->calendarwgtEnd->setSelectedDate(m_filter.dateTo());

    ui->listWidgetFilterButton->addItems(QStringList() << tr("Интервал") << tr("Дата"));
    ui->listWidgetFilterButton->setCurrentRow(0);

    if(m_filter.limitLowValue() == 0 && m_filter.limitUpperValue() == 0)
    {
        ui->spinBoxIntervalFrom->setDisabled(true);
        ui->spinBoxIntervalTo->setDisabled(true);
        ui->widgetSliderInterval->setDisabled(true);
    }
}
//------------------------------------
const CFilter &CFilterDialog::filter()
{
    m_filter.setDate(ui->calendarwgtBegin->selectedDate(), ui->calendarwgtEnd->selectedDate());
    m_filter.setLimit(ui->widgetSliderInterval->GetLowerValue(), ui->widgetSliderInterval->GetUpperValue());
    m_filter.setTime(ui->timeEditBeginFilter->time());
    m_filter.setType(((ui->stackwgtFilter->currentIndex() == 0)?CFilter::FilterLimitType:CFilter::FilterDateType));
    m_filter.setState(true);

    return m_filter;
}
