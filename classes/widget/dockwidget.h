#ifndef DOCKWIDGET_H
    #define DOCKWIDGET_H
    //----------------
    #include <QWidget>
    //----------
    namespace Ui
    {
        class CDockWidget;
    }
    //-------------------------------
    class CDockWidget: public QWidget
    {
        Q_OBJECT

        public:
            enum class WidgetType
            {
                WidgetVariable
            };

        public:
            explicit CDockWidget(QWidget* parent = nullptr);
            ~CDockWidget();
            void addWidget(QWidget* wgt);
            QWidget* widget(const QString& type);

        private:
            Ui::CDockWidget* ui;
    };
#endif // DOCKWIDGET_H
