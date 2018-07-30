#ifndef TABWIDGET_H
    #define TABWIDGET_H
    //-------------------
    #include <QTabWidget>
    #include <QEvent>
    #include <QMouseEvent>
    #include <QDropEvent>
    #include <QDragEnterEvent>
    #include <QDragMoveEvent>
    #include <QMimeData>
    #include <QDebug>
    #include "widgetmimedata.h"
    #include "widgetdrag.h"
    //---------------------------------
    class CTabWidget: public QTabWidget
    {
        Q_OBJECT

        public:
            CTabWidget(QWidget* parent = nullptr);

        private slots:
            void tabDoubleClicked(int index);

        protected:
            void dragEnterEvent(QDragEnterEvent* event);
            void dropEvent(QDropEvent* event);

        private:

    };
#endif // TABWIDGET_H
