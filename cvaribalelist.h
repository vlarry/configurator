#ifndef CVARIBALELIST_H
    #define CVARIBALELIST_H
    //----------------
    #include <QWidget>
    #include <QTableWidget>
    #include <QHeaderView>
    #include <QResizeEvent>
    #include <QShowEvent>
    #include <QDebug>
    //--------------------------------------
    class CVaribaleList: public QTableWidget
    {
        Q_OBJECT

        public:
            CVaribaleList(QWidget* parent = nullptr);
            void resizeEvent(QResizeEvent* event);

        signals:
            void resizeSize();

        protected:
            void showEvent(QShowEvent* event);
    };
#endif // CVARIBALELIST_H
