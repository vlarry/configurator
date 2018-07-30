#include "widgetdrag.h"
//-----------------------------------------------
CWidgetDrag::CWidgetDrag(QWidget* wgtDragSource):
    QDrag(wgtDragSource)
{

}
//---------------------------------------
void CWidgetDrag::setWidget(QWidget* wgt)
{
    CWidgetMimeData* md = new CWidgetMimeData;
    md->setWidget(wgt);
    setMimeData(md);
}
