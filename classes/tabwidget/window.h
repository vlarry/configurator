#ifndef WINDOW_H
    #define WINDOW_H
    //----------------
    #include <QWidget>
    #include <QApplication>
    #include <QMouseEvent>
    #include <QVBoxLayout>
    #include <QDebug>
    #include "widgetmimedata.h"
    #include "widgetdrag.h"
    //---------------------------
    class CWindow: public QWidget
    {
        Q_OBJECT

        public:
            explicit CWindow(QWidget* parent = nullptr);

        signals:

        public slots:

        protected:
            void mousePressEvent(QMouseEvent* event);
            void mouseMoveEvent(QMouseEvent* event);

        private:
            void startDrag();

        private:
            QPoint m_drag_pos;
    };
#endif // WINDOW_H
