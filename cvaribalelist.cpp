#include "cvaribalelist.h"
//--------------------------------------------
CVaribaleList::CVaribaleList(QWidget* parent):
    QListWidget(parent)
{

}
//--------------------------------------------------
void CVaribaleList::resizeEvent(QResizeEvent* event)
{
    QListWidget::resizeEvent(event);

    emit resizeSize();
}
