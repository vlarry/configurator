#ifndef TOOLBUTTON_H
    #define TOOLBUTTON_H
    //--------------------
    #include <QToolButton>
    #include <QEvent>
    //-----------------------------------
    class CToolButton: public QToolButton
    {
        Q_OBJECT

        public:
            CToolButton(QWidget* parent = nullptr);
            void setID(int id);
            int  id() const;

        protected:
            void enterEvent(QEvent* event);
            void leaveEvent(QEvent* event);

        signals:
            void hovered(int id = -1);
            void leaved(int id);

        private:
            int m_id;
    };
#endif // TOOLBUTTON_H
