#include "cvaribalelist.h"
//--------------------------------------------
CVaribaleList::CVaribaleList(QWidget* parent):
    QListWidget(parent)
{
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
}
//--------------------------------------------------
void CVaribaleList::resizeEvent(QResizeEvent* event)
{
    QListWidget::resizeEvent(event);

    emit resizeSize();
}
