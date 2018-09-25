#ifndef CVARIBALELIST_H
    #define CVARIBALELIST_H
    //----------------
    #include <QWidget>
    #include <QTableWidget>
    #include <QHeaderView>
    #include <QResizeEvent>
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
    };
#endif // CVARIBALELIST_H
