#include "purposetableview.h"
//----------------------------------------------------
CPurposeTableView::CPurposeTableView(QWidget* parent):
    QTableView(parent)
{
    setMouseTracking(true);
}
//--------------------------------------------------------
void CPurposeTableView::mouseMoveEvent(QMouseEvent* event)
{
    QModelIndex index = indexAt(event->pos());
    selectionModel()->clear();

    int rows    = model()->rowCount();
    int columns = model()->columnCount();

    for(int i = 0; i < rows; i++)
    {
        for(int j = 0; j < columns; j++)
        {
            if(i == index.row() || j == index.column())
                selectionModel()->select(model()->index(i, j), QItemSelectionModel::Select);
        }
    }
}
