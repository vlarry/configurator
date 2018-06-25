#ifndef CLINEEDIT_H
    #define CLINEEDIT_H
    //------------------
    #include <QLineEdit>
    #include <QFocusEvent>
    #include <QMouseEvent>
    #include <QKeyEvent>
    #include <QLocale>
    #include <QDebug>
    //-------------------------------
    class CLineEdit: public QLineEdit
    {
        Q_OBJECT

        public:
            CLineEdit(QWidget* parent = nullptr);

        protected:
            void mouseReleaseEvent(QMouseEvent* event);
            void keyPressEvent(QKeyEvent* event);
            void focusOutEvent(QFocusEvent* event);

        private:
            bool m_focus;
    };
#endif // CLINEEDIT_H
