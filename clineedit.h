#ifndef CLINEEDIT_H
    #define CLINEEDIT_H
    //------------------
    #include <QLineEdit>
    #include <QMouseEvent>
    #include <QDebug>
    //-------------------------------
    class CLineEdit: public QLineEdit
    {
        Q_OBJECT

        public:
            CLineEdit(QWidget* parent = nullptr);

        protected:
            void mouseReleaseEvent(QMouseEvent* event);
    };
#endif // CLINEEDIT_H
