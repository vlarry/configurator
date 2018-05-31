#include "cmonitorpurpose.h"
#include "ui_monitorpurpose.h"
//---------------------------------------------------------------------
CMonitorPurpose::CMonitorPurpose(const QString& text, QWidget* parent):
    QWidget(parent),
    ui(new Ui::CMonitorPurpose)
{
    ui->setupUi(this);

    setWindowTitle(text);
    setWindowFlag(Qt::Window);

    connect(ui->pushButtonRead, &QPushButton::clicked, this, &CMonitorPurpose::readPurpose);
    connect(ui->pushButtonClear, &QPushButton::clicked, this, &CMonitorPurpose::clearTable);
}
//---------------------------------
CMonitorPurpose::~CMonitorPurpose()
{
    delete ui;
}
//-----------------------------------------------------------------------------------------------
void CMonitorPurpose::setHeaders(const QStringList& row_labels, const QStringList& column_labels)
{
    HierarchicalHeaderView* hheader = new HierarchicalHeaderView(Qt::Horizontal, ui->tableViewMonitor);
    HierarchicalHeaderView* vheader = new HierarchicalHeaderView(Qt::Vertical, ui->tableViewMonitor);
    CMatrixPurposeModel*    model   = new CMatrixPurposeModel(row_labels, column_labels);

    hheader->setMaximumHeight(200);

    ui->tableViewMonitor->setItemDelegate(new CTableItemDelegate(CTableItemDelegate::PURPOSE_TYPE));
    ui->tableViewMonitor->setHorizontalHeader(hheader);
    ui->tableViewMonitor->setVerticalHeader(vheader);
    ui->tableViewMonitor->setModel(model);
    ui->tableViewMonitor->resizeColumnsToContents();
    ui->tableViewMonitor->resizeRowsToContents();

    ui->tableViewMonitor->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    ui->tableViewMonitor->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
}
//----------------------------------------
QTableView* CMonitorPurpose::table() const
{
    return ui->tableViewMonitor;
}
//--------------------------------
void CMonitorPurpose::clearTable()
{
    CMatrixPurposeModel* model = static_cast<CMatrixPurposeModel*>(ui->tableViewMonitor->model());

    if(!model)
        return;

    CMatrix& matrix = model->matrixTable();

    for(int i = 0; i < matrix.rowCount(); i++)
    {
        for(int j = 0; j < matrix.columnCount(); j++)
        {
            matrix[i][j].setState(false);
        }
    }

    model->updateData();
}
//--------------------------------------------------
void CMonitorPurpose::closeEvent(QCloseEvent* event)
{
    emit closeWindow();

    QWidget::closeEvent(event);
}