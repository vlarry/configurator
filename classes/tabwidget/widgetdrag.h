#ifndef WIDGETDRAG_H
    #define WIDGETDRAG_H
    //----------------
    #include <QWidget>
    #include <QDrag>
    #include <widgetmimedata.h>
    //-----------------------------
    class CWidgetDrag: public QDrag
    {
        public:
            CWidgetDrag(QWidget* wgtDragSource = nullptr);
            void setWidget(QWidget* wgt);
    };
#endif // WIDGETDRAG_H
