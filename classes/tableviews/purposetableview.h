#ifndef PURPOSETABLEVIEW_H
    #define PURPOSETABLEVIEW_H
    //-------------------
    #include <QTableView>
    #include <QMouseEvent>
    //----------------------------------------
    class CPurposeTableView: public QTableView
    {
        public:
            CPurposeTableView(QWidget* parent = nullptr);

        protected:
            void mouseMoveEvent(QMouseEvent *event);
    };
#endif // PURPOSETABLEVIEW_H
