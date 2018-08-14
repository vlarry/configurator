#ifndef DRAGWIDGET_H
    #define DRAGWIDGET_H
    //----------------
    #include <QWidget>
    #include <QLayout>
    #include <QPaintEvent>
    #include <QPainter>
    #include <QMouseEvent>
    #include <QDebug>
    //----------
    namespace Ui
    {
        class CDragWidget;
    }
    //-------------------------------
    class CDragWidget: public QWidget
    {
        Q_OBJECT

        public:
            explicit CDragWidget(const QString& title, QWidget* parent = nullptr);
            ~CDragWidget();

            void setWidget(QWidget* widget);
            QWidget* widget() const;

        protected:
            void setWindowTitle(const QString& title);
            void paintEvent(QPaintEvent* event);
            bool eventFilter(QObject* obj, QEvent* event);

        private:
            Ui::CDragWidget* ui;
            QWidget*         m_widget;
            QPoint           m_mouse_pos;
    };
#endif // DRAGWIDGET_H
