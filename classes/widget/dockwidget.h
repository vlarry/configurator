#ifndef DOCKWIDGET_H
    #define DOCKWIDGET_H
    //----------------
    #include <QWidget>
    #include <QDebug>
    #include "cdockpanelitemctrl.h"
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
            QWidget* widget(const QString& type, bool isRemove = false);
            CDockPanelItemCtrl* control();
            void hideContent();
            void showContent();
            bool isContentHidden() const;
            void setVisibleContent(bool state);

        private:
            Ui::CDockWidget* ui;
    };
#endif // DOCKWIDGET_H
