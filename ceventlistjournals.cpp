#include "ceventlistjournals.h"
#include "ui_eventlistjournals.h"
//----------------------------------------------------------------------------------------------------------
CEventListJournals::CEventListJournals(const QString& journal_name, QVector<cell_t>& list, QWidget* parent):
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

    setWindowTitle(tr("Список журналов %1").arg(journal_name));

    connect(ui->listwgtListEventJournals, &QListWidget::itemDoubleClicked, this, &CEventListJournals::slotDoubleClicked);

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
//-----------------------------------------------------
QString CEventListJournals::currentSerialNumber() const
{
    return m_cells[ui->listwgtListEventJournals->currentRow()].sn;
}
//---------------------------------------------------------------
void CEventListJournals::slotDoubleClicked(QListWidgetItem* item)
{
    ui->listwgtListEventJournals->setCurrentItem(item);

    accept();
}
