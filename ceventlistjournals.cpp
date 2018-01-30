#include "ceventlistjournals.h"
#include "ui_eventlistjournals.h"
//-----------------------------------------------------------------------------
CEventListJournals::CEventListJournals(QVector<cell_t>& list, QWidget* parent):
    QDialog(parent),
    ui(new Ui::CEventListJournals),
    m_cells(list)
{
    if(list.isEmpty())
        return;

    ui->setupUi(this);

    QStringList items;

    for(cell_t cell: list)
    {
        items << cell.name;
    }

    ui->listwgtListEventJournals->addItems(items);
    ui->listwgtListEventJournals->setCurrentRow(0);
}
//---------------------------------------
CEventListJournals::~CEventListJournals()
{
    delete ui;
}
//---------------------------------------
int CEventListJournals::currentId() const
{
    return m_cells[ui->listwgtListEventJournals->currentRow()].id;
}
