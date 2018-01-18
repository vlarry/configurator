#include "ccalendarwidget.h"
#include "ui_calendarwidget.h"
//------------------------------------------------
CCalendarWidget::CCalendarWidget(QWidget* parent):
    QWidget(parent),
    ui(new Ui::CCalendarWidget)
{
    ui->setupUi(this);

    QDate date_beg = QDate::currentDate();
    QDate date_end = date_beg;

    date_beg.setDate(date_beg.year(), date_beg.month(), 1);

    ui->calendarwgtBegin->setSelectedDate(date_beg);
    ui->calendarwgtEnd->setSelectedDate(date_end);

    connect(ui->calendarwgtBegin, &QCalendarWidget::selectionChanged, this, &CCalendarWidget::dateChanged);
    connect(ui->calendarwgtEnd, &QCalendarWidget::selectionChanged, this, &CCalendarWidget::dateChanged);
}
//---------------------------------
CCalendarWidget::~CCalendarWidget()
{
    delete ui;
}
//--------------------------------------
QDate CCalendarWidget::dateBegin() const
{
    return ui->calendarwgtBegin->selectedDate();
}
//------------------------------------
QDate CCalendarWidget::dateEnd() const
{
    return ui->calendarwgtEnd->selectedDate();
}
//------------------------------------------------------------------------------
void CCalendarWidget::setDateRange(const QDate& date_beg, const QDate& date_end)
{
    ui->calendarwgtBegin->setSelectedDate(date_beg);
    ui->calendarwgtEnd->setSelectedDate(date_end);
}
