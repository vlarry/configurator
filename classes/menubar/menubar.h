#ifndef MENUBAR_H
    #define MENUBAR_H
    //----------------
    #include <QWidget>
    #include <QMouseEvent>
    #include <QPoint>
    #include <QDebug>
    //----------
    namespace Ui
    {
        class CMenuBar;
    }
    //----------------------------
    class CMenuBar: public QWidget
    {
        Q_OBJECT

        public:
            explicit CMenuBar(QWidget* parent = nullptr);
            ~CMenuBar();
            QPoint mousePosition() const;

        signals:
            void closeWindow();
            void expandedWindow();
            void minimizeWindow();
            void menubarMouseUpdatePosition(QPoint);

        protected:
            void mousePressEvent(QMouseEvent* event);
            void mouseMoveEvent(QMouseEvent* event);
            void mouseReleaseEvent(QMouseEvent* event);
            void mouseDoubleClickEvent(QMouseEvent* event);

        private:
            Ui::CMenuBar* ui;
            QPoint        m_mouse_pos;
    };
#endif // MENUBAR_H
