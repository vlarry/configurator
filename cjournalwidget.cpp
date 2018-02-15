#include "cjournalwidget.h"
#include "ui_journalwidget.h"
//----------------------------------------------
CJournalWidget::CJournalWidget(QWidget* parent):
    QWidget(parent),
    ui(new Ui::CJournalWidget)
{
    ui->setupUi(this);

    ui->tableWidgetJournal->setShowGrid(true);
    ui->tableWidgetJournal->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidgetJournal->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidgetJournal->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidgetJournal->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);

    connect(ui->widgetJournalHeader, &CHeaderJournal::clickedButtonRead, this, &CJournalWidget::clickedButtonRead);
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
//--------------------------------------
void CJournalWidget::headerClear() const
{
    ui->widgetJournalHeader->clear();
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
//-------------------------------------
void CJournalWidget::tableClear() const
{
    ui->tableWidgetJournal->clearContents();
    ui->tableWidgetJournal->setRowCount(0);
}
