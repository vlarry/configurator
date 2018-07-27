#ifndef WIDGETMENU_H
    #define WIDGETMENU_H
    //----------------
    #include <QWidget>
    #include <QEvent>
    #include <QMenu>
    #include <QDebug>
    //----------
    namespace Ui
    {
        class CWidgetMenu;
    }
    //-------------------------------
    class CWidgetMenu: public QWidget
    {
        Q_OBJECT

        public:
            enum ButtonIDType
            {
                NEWPROJECT,
                OPENPROJECT,
                SAVEPROJECT,
                SAVEASPROJECT,
                EXPORTPROJECT,
                EXPORTTOPDFPROJECT,
                EXPORTTOEXCELPROJECT,
                CLOSEPROJECT,
                EXITAPPLICATION
            };

        public:
            explicit CWidgetMenu(QWidget* parent = nullptr);
            ~CWidgetMenu();

        protected:
            bool eventFilter(QObject* obj, QEvent* event);

        public slots:
            void clicked();
            void addOpenDocument(const QString& doc);

        signals:
            void closeWindow();
            void newProject();
            void openProject();
            void saveProject();
            void saveAsProject();
            void exportToPDFProject();
            void exportToExcelProject();
            void closeProject();
            void addDocument(const QString&);

        private:
            void createMenuButtonGroup();

        private:
            Ui::CWidgetMenu* ui;
    };
#endif // WIDGETMENU_H
