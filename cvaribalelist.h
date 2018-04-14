#ifndef CVARIBALELIST_H
    #define CVARIBALELIST_H
    //----------------
    #include <QWidget>
    #include <QListWidget>
    #include <QResizeEvent>
    //-------------------------------------
    class CVaribaleList: public QListWidget
    {
        Q_OBJECT

        public:
            CVaribaleList(QWidget* parent = nullptr);
            void resizeEvent(QResizeEvent* event);

        signals:
            void resizeSize();
    };
#endif // CVARIBALELIST_H
