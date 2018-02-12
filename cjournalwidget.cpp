#include "cjournalwidget.h"
#include "ui_journalwidget.h"
//----------------------------------------------
CJournalWidget::CJournalWidget(QWidget* parent):
    QWidget(parent),
    ui(new Ui::CJournalWidget)
{
    ui->setupUi(this);
}
//-------------------------------
CJournalWidget::~CJournalWidget()
{
    delete ui;
}
//--------------------------------------------
CHeaderJournal* CJournalWidget::header() const
{
    return ui->widgetJournalHeader;
}
//-----------------------------------------
QTableWidget* CJournalWidget::table() const
{
    return ui->tableWidgetJournal;
}
//--------------------------------------------------------------
void CJournalWidget::setTableHeaders(const QStringList& headers)
{
    ui->tableWidgetJournal->setColumnCount(headers.count());
    ui->tableWidgetJournal->setHorizontalHeaderLabels(headers);
}
//----------------------------------------------------------------
void CJournalWidget::setTableColumnWidth(const QVector<int>& list)
{
    if(list.isEmpty() || list.count() != ui->tableWidgetJournal->columnCount())
        return;

    for(int i = 0; i < list.count(); i++)
    {
        ui->tableWidgetJournal->setColumnWidth(i, list[i]);
    }
}
//-------------------------------------------------------------
void CJournalWidget::setTableColumnWidth(int column, int width)
{
    if(column < 0 || column >= ui->tableWidgetJournal->columnCount())
        return;

    ui->tableWidgetJournal->setColumnWidth(column, width);
}
