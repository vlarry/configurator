#ifndef WIDGETMIMEDATA_H
    #define WIDGETMIMEDATA_H
    //----------------
    #include <QWidget>
    #include <QMimeData>
    //-------------------------------------
    class CWidgetMimeData: public QMimeData
    {
        public:
            CWidgetMimeData();
            virtual ~CWidgetMimeData();
            static QString mimeType();
            void           setWidget(QWidget* wgt);
            QWidget*       widget() const;

        private:
            QWidget* m_wgt;
    };
#endif // WIDGETMIMEDATA_H
