#ifndef TABWIDGET_H
    #define TABWIDGET_H
    //-------------------
    #include <QTabWidget>
    #include <QEvent>
    #include <QMouseEvent>
    #include <QTabBar>
    #include <QDropEvent>
    #include <QDragEnterEvent>
    #include <QDragMoveEvent>
    #include <QDebug>
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
