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

        struct menu_t
        {
            bool is_menu;
            bool is_active;
            int  level;
        };

        public:
            CToolButton(QWidget* parent = nullptr);
            int  id() const;
            void setID(int id);
            void setButtonMenu(bool state, int level);

        protected:
            void enterEvent(QEvent* event);
            void leaveEvent(QEvent* event);
            void paintEvent(QPaintEvent *event);

        signals:
            void hovered(int id = -1);
            void leaved(int id);
            void updateButtons();

        private:
            int m_id;
            menu_t m_menu;
            static QVector<int> m_button_level;
    };
#endif // TOOLBUTTON_H
