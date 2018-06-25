#include "cmonitorpurpose.h"
#include "ui_monitorpurpose.h"
//---------------------------------------------------------------------
CMonitorPurpose::CMonitorPurpose(const QString& text, QWidget* parent):
    QWidget(parent),
    ui(new Ui::CMonitorPurpose),
    m_timeout_update(nullptr)
{
    ui->setupUi(this);

    setWindowTitle(text);
    setWindowFlag(Qt::Window);

    m_timeout_update = new QTimer(this);

    connect(ui->pushButtonRead, &QPushButton::clicked, this, &CMonitorPurpose::buttonUpdate);
    connect(ui->pushButtonClear, &QPushButton::clicked, this, &CMonitorPurpose::clearTable);
    connect(ui->checkBoxUpdateData, &QCheckBox::clicked, this, &CMonitorPurpose::stateUpdateData);
    connect(ui->spinBoxUpdateData, SIGNAL(valueChanged(int)), this, SLOT(timeoutValueChanged(int)));
    connect(m_timeout_update, &QTimer::timeout, this, &CMonitorPurpose::timeoutUpdate);
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
//-----------------------------------------------
void CMonitorPurpose::stateUpdateData(bool state)
{
    if(state)
        m_timeout_update->start(ui->spinBoxUpdateData->value());
    else
        m_timeout_update->stop();
}
//-----------------------------------
void CMonitorPurpose::timeoutUpdate()
{
    emit buttonUpdate();
}
//------------------------------------------------------
void CMonitorPurpose::timeoutValueChanged(int new_value)
{
    if(ui->checkBoxUpdateData->isChecked())
    {
        m_timeout_update->stop();
        m_timeout_update->start(new_value);
    }
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
            matrix[i][j].setState(CColumn::INACTIVE);
        }
    }

    model->updateData();
}
//--------------------------------------------------
void CMonitorPurpose::closeEvent(QCloseEvent* event)
{
    QWidget::closeEvent(event);
    m_timeout_update->stop();
    emit closeWindow();
}
//------------------------------------------------
void CMonitorPurpose::showEvent(QShowEvent* event)
{
    QWidget::showEvent(event);

    if(ui->checkBoxUpdateData->isChecked())
        m_timeout_update->start(ui->spinBoxUpdateData->value());
}
