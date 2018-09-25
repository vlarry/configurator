#include "cvaribalelist.h"
//--------------------------------------------
CVaribaleList::CVaribaleList(QWidget* parent):
    QTableWidget(parent)
{
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);

    setColumnCount(6);
    setHorizontalHeaderLabels(QStringList() << tr("Индекс") << tr("Название") << tr("Первичное") << tr("")
                                            << tr("Вторичное") << tr("Ед. Измерения"));
    horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    horizontalHeader()->setStretchLastSection(true);
}
//--------------------------------------------------
void CVaribaleList::resizeEvent(QResizeEvent* event)
{
    QTableWidget::resizeEvent(event);

    emit resizeSize();
}
