#ifndef TABWIDGET_H
    #define TABWIDGET_H
    //-------------------
    #include <QTabWidget>
    #include <QEvent>
    #include <QMouseEvent>
    #include <QDropEvent>
    #include <QDragEnterEvent>
    #include <QDebug>
    //---------------------------------
    class CTabWidget: public QTabWidget
    {
        public:
            CTabWidget(QWidget* parent = nullptr);

        private slots:
            void tabDoubleClicked(int index);

        protected:

        private:

    };
#endif // TABWIDGET_H
