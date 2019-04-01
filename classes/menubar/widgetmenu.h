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
                BUTTON_NEW_PROJECT, // кнопка "Новый" - создание нового проекта
                BUTTON_OPEN_PROJECT, // кнопка "Открыть" - открытие проекта
                BUTTON_SAVE_PROJECT, // кнопка "Сохранить" - сохранение проекта в текущий файл
                BUTTON_SAVE_AS_PROJECT, // кнопка "Сохранить как" - сохранение проекта в указанный каталог
                BUTTON_EXPORT_PROJECT, // кнопка-меню "Экспорт" - открывает перечень допустимых пунктов меню для экспорта
                BUTTON_IMPORT_PROJECT, // кнопка-меню "Импорт" - открывает перечень допустимых пунктов меню для импорта
                BUTTON_SETTINGS_PROJECT, // кнопка-меню "Настройки" - открывает выбор настроек
                BUTTON_CLOSE_PROJECT, // кнопка "Закрыть" - установка дефолтных значений проекта и закрытие текущего файла проекта
                BUTTON_EXIT_APP // кнопка "Выход" - выход из программы
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

        protected:
            bool eventFilter(QObject *watched, QEvent *event);

        private:
            void createMenuButtonGroup();
            void exportProtection();
            void exportJournalMenu();
            void exportJournal();

        private:
            Ui::CWidgetMenu* ui;
    };
#endif // WIDGETMENU_H
