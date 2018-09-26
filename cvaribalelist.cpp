#include "cvaribalelist.h"
//--------------------------------------------
CVaribaleList::CVaribaleList(QWidget* parent):
    QTableWidget(parent)
{
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
    setColumnCount(6);
}
//--------------------------------------------------
void CVaribaleList::resizeEvent(QResizeEvent* event)
{
    QTableWidget::resizeEvent(event);

    emit resizeSize();
}
//----------------------------------------------
void CVaribaleList::showEvent(QShowEvent* event)
{
    QTableWidget::showEvent(event);
//    horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
//    horizontalHeader()->setStretchLastSection(true);
    horizontalHeader()->hide();
    verticalHeader()->hide();
}
