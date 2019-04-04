#ifndef WIDGETMENU_H
    #define WIDGETMENU_H
    //----------------
    #include <QWidget>
    #include <QEvent>
    #include <QWidgetAction>
    #include <QDebug>
    #include <QFocusEvent>
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
                BUTTON_NONE,
                BUTTON_NEW_PROJECT, // кнопка "Новый" - создание нового проекта
                BUTTON_OPEN_PROJECT, // кнопка "Открыть" - открытие проекта
                BUTTON_SAVE_PROJECT, // кнопка "Сохранить" - сохранение проекта в текущий файл
                BUTTON_SAVE_AS_PROJECT, // кнопка "Сохранить как" - сохранение проекта в указанный каталог
                BUTTON_EXPORT_PROJECT, // кнопка-меню "Экспорт" - открывает перечень допустимых пунктов меню для экспорта
                BUTTON_IMPORT_PROJECT, // кнопка-меню "Импорт" - открывает перечень допустимых пунктов меню для импорта
                BUTTON_SETTINGS_PROJECT, // кнопка-меню "Настройки" - открывает выбор настроек
                BUTTON_CLOSE_PROJECT, // кнопка "Закрыть" - установка дефолтных значений проекта и закрытие текущего файла проекта
                BUTTON_EXIT_APP, // кнопка "Выход" - выход из программы
                BUTTON_PROTECTION_EXPORT, // кнопка "Защиты и автоматика (уставки)" - открывает подпункты экспорта
                BUTTON_JOURNAL_EXPORT, // кнопка "Журналы" - открывает подпункты наименования журналов
                BUTTON_SETTINGS_EXPORT, // кнопка "Настройки" - открывает подпункты наименования настроек
                BUTTON_JOURNAL_CRASH_EXPORT, // кнопка "Аварий" - открывает подпункты экспорта журнала аварий
                BUTTON_JOURNAL_EVENT_EXPORT, // кнопка "Событий" - открывает подпункты экспорта журнала событий
                BUTTON_JOURNAL_HALFHOUR_EXPORT, // кнопка "Получасовок" - открывает подпункты экспорта журнала получасовок
                BUTTON_JOURNAL_ISOLATION_EXPORT, // кнопка "Изоляций" - открывает подпункты экспорта журнала изоляций
                BUTTON_SETTINGS_ANALOG_INPUT_EXPORT, // кнопка "Аналоговые входы" - открывает подпункты экспорта настроек аналоговых входов
                BUTTON_SETTINGS_DISCRET_INPUT_EXPORT, // кнопка "Аналоговые входы" - открывает подпункты экспорта настроек дискретных входов
                BUTTON_SETTINGS_RELAY_EXPORT, // кнопка "Аналоговые входы" - открывает подпункты экспорта настроек реле
                BUTTON_SETTINGS_LED_EXPORT, // кнопка "Аналоговые входы" - открывает подпункты экспорта настроек светодиодов
                BUTTON_SETTINGS_PROTECTION_BLOCK_EXPORT, // кнопка "Аналоговые входы" - открывает подпункты экспорта настроек блокировок защит
                BUTTON_PROTECTION_IMPORT, // кнопка "Защиты и автоматика (уставки)" - открывает подпункты импорта
                BUTTON_JOURNAL_IMPORT, // кнопка "Журналы" - открывает подпункты наименования журналов
                BUTTON_SETTINGS_IMPORT, // кнопка "Настройки" - открывает подпункты наименования настроек
                BUTTON_JOURNAL_CRASH_IMPORT, // кнопка "Аварий" - открывает подпункты импорта журнала аварий
                BUTTON_JOURNAL_EVENT_IMPORT, // кнопка "Событий" - открывает подпункты импорта журнала событий
                BUTTON_JOURNAL_HALFHOUR_IMPORT, // кнопка "Получасовок" - открывает подпункты импорта журнала получасовок
                BUTTON_JOURNAL_ISOLATION_IMPORT, // кнопка "Изоляций" - открывает подпункты импорта журнала изоляций
                BUTTON_SETTINGS_ANALOG_INPUT_IMPORT, // кнопка "Аналоговые входы" - открывает подпункты импорта настроек аналоговых входов
                BUTTON_SETTINGS_DISCRET_INPUT_IMPORT, // кнопка "Аналоговые входы" - открывает подпункты импорта настроек дискретных входов
                BUTTON_SETTINGS_RELAY_IMPORT, // кнопка "Аналоговые входы" - открывает подпункты импорта настроек реле
                BUTTON_SETTINGS_LED_IMPORT, // кнопка "Аналоговые входы" - открывает подпункты импорта настроек светодиодов
                BUTTON_SETTINGS_PROTECTION_BLOCK_IMPORT, // кнопка "Аналоговые входы" - открывает подпункты импорта настроек блокировок защит
                BUTTON_EXPORT_TO_DATEBASE, // кнопка экспорта в базу данных
                BUTTON_EXPORT_TO_EXCEL, // кнопка экспорта в эксель
                BUTTON_EXPORT_TO_PDF, // кнопка экспорта в PDF
                BUTTON_IMPORT_FROM_DATEBASE, // кнопка экспорта в базу данных
                BUTTON_IMPORT_FROM_EXCEL // кнопка экспорта в эксель
            };

            struct OperationType
            {
                int type; // export or import
                int operation; // operation type (export|import protection, journal or settings)
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
            void exportProtectionAutomaticToExcel();
            void exportProtectionAutomaticToDatabase();
            void exportJournalToExcel(const QString&);
            void exportJournalToDatabase(const QString&);
            void exportJournalToPDF(const QString&);
            void exportSettingsToExcel(const QString&);
            void exportSettingsToDatabase(const QString&);

            void importProtectionAutomaticFromExcel();
            void importProtectionAutomaticFromDatabase();
            void importJournalFromExcel(const QString&);
            void importJournalFromDatabase(const QString = "");
            void importSettingsFromExcel(const QString&);
            void importSettingsFromDatabase(const QString&);
            void closeProject();
            void settings();
            void addDocument(const QString&);

        protected:
//            bool eventFilter(QObject *watched, QEvent *event);
            void leaveEvent(QEvent* event);
            void showEvent(QShowEvent *event);

        private:
            void createMenuButtonGroup();
            void exportProtection();
            void exportJournalMenu();
            void exportJournal();
            void exportSettings();
            void exportSettingsAnalogInput();
            void exportSettingsMenu();
            void emitExport(ButtonIDType id);
            void importProtection();
            void importJournal();
            void importJournalMenu();
            void importSettings();
            void importSettingsAnalogInput();
            void importSettingsMenu();
            void emitImport(ButtonIDType id);
            void initMenu();

        private:
            Ui::CWidgetMenu* ui;
            OperationType m_operation;
    };
#endif // WIDGETMENU_H
