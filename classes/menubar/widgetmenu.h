#ifndef WIDGETMENU_H
    #define WIDGETMENU_H
    //----------------
    #include <QWidgetAction>
    //----------
    namespace Ui
    {
        class CWidgetMenu;
    }
    //-------------------------------------
    class CWidgetMenu: public QWidgetAction
    {
        Q_OBJECT

        public:
            explicit CWidgetMenu(QWidget* parent = nullptr);
            ~CWidgetMenu();

        private:
            Ui::CWidgetMenu* ui;
    };
#endif // WIDGETMENU_H
