#ifndef TOOLBUTTON_H
    #define TOOLBUTTON_H
    //--------------------
    #include <QToolButton>
    #include <QFrame>
    #include <QVBoxLayout>
    #include <QEvent>
    #include <QDebug>
    #include <QMenu>
    #include <QPainter>
    #include <QPaintEvent>
    //----------
    class CMenu;
    //-----------------------------------
    class CToolButton: public QToolButton
    {
        Q_OBJECT

        public:
            CToolButton(QWidget* parent = nullptr);
            int  id() const;
            void setID(int id);
            void setIndicatorMenu(bool state);

        protected:
            void enterEvent(QEvent* event);
            void leaveEvent(QEvent* event);
            void paintEvent(QPaintEvent *event);

        signals:
            void hovered(int id = -1);
            void leaved(int id);

        private:
            int m_id;
            int m_is_menu;
    };
#endif // TOOLBUTTON_H
