#include "cvaribalelist.h"
//--------------------------------------------
CVaribaleList::CVaribaleList(QWidget* parent):
    QTableWidget(parent)
{
    setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Expanding);
    setColumnCount(6);
}
//--------------------------------------------------
void CVaribaleList::resizeEvent(QResizeEvent* event)
{
    QTableWidget::resizeEvent(event);
}
//----------------------------------------------
void CVaribaleList::showEvent(QShowEvent* event)
{
    QTableWidget::showEvent(event);
    horizontalHeader()->setStretchLastSection(true);
}
