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
                IMPORTPROJECT,
                IMPORTFROMEXCELPROJECT,
                SETTINGS,
                SETTINGDEBUG,
                CLOSEPROJECT,
                EXITAPPLICATION
            };

        public:
            explicit CWidgetMenu(QWidget* parent = nullptr);
            ~CWidgetMenu();

        public slots:
            void clicked();
            void hoverChanged(int id);
            void addOpenDocument(const QString& doc);
            void activateMenuButtons();

        signals:
            void activateMenu();
            void closeWindow();
            void newProject();
            void openProject();
            void saveProject();
            void saveAsProject();
            void exportToPDFProject();
            void exportToExcelProject();
            void exportJournalToDataBase();
            void importFromExcelProject();
            void importJournalFromDataBase();
            void closeProject();
            void settings();
            void addDocument(const QString&);

        private:
            void createMenuButtonGroup();

        private:
            Ui::CWidgetMenu* ui;
    };
#endif // WIDGETMENU_H
