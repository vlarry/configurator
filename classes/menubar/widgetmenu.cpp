#include "widgetmenu.h"
#include "ui_widgetmenu.h"
//----------------------------------------
CWidgetMenu::CWidgetMenu(QWidget* parent):
    QWidget(parent),
    ui(new Ui::CWidgetMenu),
    m_operation(OperationType({ BUTTON_NONE, BUTTON_NONE })),
    m_isButtonActive(false),
    m_button_menu_list(QVector<CToolButton*>())
{
    ui->setupUi(this);
    initMenu();

    connect(ui->listWidgetOpenDocument, &QListWidget::itemDoubleClicked, this, &CWidgetMenu::itemDoubleClicked);
}
//-------------------------
CWidgetMenu::~CWidgetMenu()
{
    delete ui;
}
//---------------------------------------------------
void CWidgetMenu::addOpenDocument(const QString& doc)
{
    if(ui->listWidgetOpenDocument->count() != 0)
    {
        QList<QListWidgetItem*> items = ui->listWidgetOpenDocument->findItems(doc, Qt::MatchCaseSensitive);
        if(!items.isEmpty())
            return;
    }

    if(ui->listWidgetOpenDocument->isHidden())
        ui->listWidgetOpenDocument->show();

    ui->listWidgetOpenDocument->addItem(doc);
}
//-------------------------------------
void CWidgetMenu::activateMenuButtons()
{
    ui->toolButtonSaveProject->setEnabled(true);
    ui->toolButtonSaveAsProject->setEnabled(true);
    ui->toolButtonExportProject->setEnabled(true);
    ui->toolButtonImportProject->setEnabled(true);
    ui->toolButtonSettings->setEnabled(true);
    m_isButtonActive = true;
}
//--------------------------------------
void CWidgetMenu::deactivateMenuButton()
{
    ui->toolButtonSaveProject->setDisabled(true);
    ui->toolButtonSaveAsProject->setDisabled(true);
    ui->toolButtonExportProject->setDisabled(true);
    ui->toolButtonImportProject->setDisabled(true);
    ui->toolButtonSettings->setDisabled(true);
    m_isButtonActive = false;
}
//--------------------------------------------------------
void CWidgetMenu::itemDoubleClicked(QListWidgetItem *item)
{
    if(item)
        openExistsProject(item->text());
}
//-----------------------------------
void CWidgetMenu::updateButtonGroup()
{
    for(CToolButton *button: m_button_menu_list)
    {
        if(button)
            button->update();
    }
}
//--------------------------------------------
void CWidgetMenu::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);
}
//-----------------------------------------
void CWidgetMenu::leaveEvent(QEvent *event)
{
    QWidget::leaveEvent(event);
    ui->stackedWidgetMenuLevel1->setCurrentIndex(0);
    m_operation = OperationType({ BUTTON_NONE, BUTTON_NONE });
}
//----------------------------------
void CWidgetMenu::exportProtection()
{
    ui->frameSubmenu1->hide();
    ui->frameSubmenu2->hide();
    ui->frameSubmenu3->show();
    ui->toolButtonExptortToPDF->hide();
    ui->toolButtonExptortToExcel->show();
    ui->toolButtonExptortToDB->show();
}
//-----------------------------------
void CWidgetMenu::exportJournalMenu()
{
    if(ui->frameSubmenu3->isHidden())
        ui->frameSubmenu3->show();

    if(ui->toolButtonExptortToDB->isHidden())
        ui->toolButtonExptortToDB->show();

    if(ui->toolButtonExptortToPDF->isHidden())
        ui->toolButtonExptortToPDF->show();

    ui->toolButtonExptortToExcel->hide();
}
//-------------------------------
void CWidgetMenu::exportJournal()
{
    ui->frameSubmenu2->hide();
    ui->frameSubmenu3->hide();
    ui->frameSubmenu1->show();
}
//--------------------------------
void CWidgetMenu::exportSettings()
{
    ui->frameSubmenu1->hide();
    ui->frameSubmenu3->hide();
    ui->frameSubmenu2->show();
}
//-------------------------------------------
void CWidgetMenu::exportSettingsAnalogInput()
{
    if(ui->frameSubmenu3->isHidden())
        ui->frameSubmenu3->show();

    if(ui->toolButtonExptortToDB->isHidden())
        ui->toolButtonExptortToDB->show();

    if(ui->toolButtonExptortToExcel->isHidden())
        ui->toolButtonExptortToExcel->show();

    ui->toolButtonExptortToPDF->hide();
}
//------------------------------------
void CWidgetMenu::exportSettingsMenu()
{
    if(ui->frameSubmenu3->isHidden())
        ui->frameSubmenu3->show();

    if(ui->toolButtonExptortToDB->isHidden())
        ui->toolButtonExptortToDB->show();

    ui->toolButtonExptortToPDF->hide();
    ui->toolButtonExptortToExcel->hide();
}
//--------------------------------------------------------
void CWidgetMenu::emitExport(CWidgetMenu::ButtonIDType id)
{
    if(m_operation.operation == BUTTON_PROTECTION_EXPORT)
    {
        if(id == BUTTON_EXPORT_TO_EXCEL)
            emit exportProtectionAutomaticToExcel();
        else if(id == BUTTON_EXPORT_TO_DATEBASE)
            emit exportProtectionAutomaticToDatabase();
    }
    else if(m_operation.type == BUTTON_JOURNAL_EXPORT)
    {
        QString journal_type;

        if(m_operation.operation == BUTTON_JOURNAL_CRASH_EXPORT)
        {
            journal_type = "CRASH";
        }
        else if(m_operation.operation == BUTTON_JOURNAL_EVENT_EXPORT)
        {
            journal_type = "EVENT";
        }
        else if(m_operation.operation == BUTTON_JOURNAL_HALFHOUR_EXPORT)
        {
            journal_type = "HALFHOUR";
        }
        else if(m_operation.operation == BUTTON_JOURNAL_ISOLATION_EXPORT)
        {
            journal_type = "ISOLATION";
        }

        if(journal_type.isEmpty())
            return;

        if(id == BUTTON_EXPORT_TO_EXCEL)
        {
            emit exportJournalToExcel(journal_type);
        }
        else if(id == BUTTON_EXPORT_TO_DATEBASE)
        {
            emit exportJournalToDatabase(journal_type);
        }
        else if(id == BUTTON_EXPORT_TO_PDF)
        {
            emit exportJournalToPDF(journal_type);
        }
    }
    else if(m_operation.type == BUTTON_SETTINGS_EXPORT)
    {
        QString type;

        if(m_operation.operation == BUTTON_SETTINGS_ANALOG_INPUT_EXPORT)
        {
            type = "ANALOG";
        }
        else if(m_operation.operation == BUTTON_SETTINGS_DISCRET_INPUT_EXPORT)
        {
            type = "INPUT";
        }
        else if(m_operation.operation == BUTTON_SETTINGS_RELAY_EXPORT)
        {
            type = "RELAY";
        }
        else if(m_operation.operation == BUTTON_SETTINGS_LED_EXPORT)
        {
            type = "LED";
        }
        else if(m_operation.operation == BUTTON_SETTINGS_PROTECTION_BLOCK_EXPORT)
        {
            type = "PROTECTION";
        }

        if(type.isEmpty())
            return;

        if(id == BUTTON_EXPORT_TO_EXCEL)
        {
            emit exportSettingsToExcel(type);
        }
        else if(id == BUTTON_EXPORT_TO_DATEBASE)
        {
            emit exportSettingsToDatabase(type);
        }
    }

    m_operation = OperationType({ BUTTON_NONE, BUTTON_NONE });
}
//----------------------------------
void CWidgetMenu::importProtection()
{
    ui->frameSubmenuImport1->hide();
    ui->frameSubmenuImport2->hide();
    ui->frameSubmenuImport3->show();
    ui->toolButtonImportFromExcel->show();
    ui->toolButtonImportFromDB->show();
}
//-------------------------------
void CWidgetMenu::importJournal()
{
    ui->frameSubmenuImport2->hide();
    ui->frameSubmenuImport3->hide();
    ui->frameSubmenuImport1->show();
}
//-----------------------------------
void CWidgetMenu::importJournalMenu()
{
    if(ui->frameSubmenuImport3->isHidden())
        ui->frameSubmenuImport3->show();

    if(ui->toolButtonImportFromDB->isHidden())
        ui->toolButtonImportFromDB->show();

    ui->toolButtonImportFromExcel->hide();
}
//--------------------------------
void CWidgetMenu::importSettings()
{
    ui->frameSubmenuImport1->hide();
    ui->frameSubmenuImport3->hide();
    ui->frameSubmenuImport2->show();
}
//-------------------------------------------
void CWidgetMenu::importSettingsAnalogInput()
{
    if(ui->frameSubmenuImport3->isHidden())
        ui->frameSubmenuImport3->show();

    if(ui->toolButtonImportFromDB->isHidden())
        ui->toolButtonImportFromDB->show();

    if(ui->toolButtonImportFromExcel->isHidden())
        ui->toolButtonImportFromExcel->show();
}
//------------------------------------
void CWidgetMenu::importSettingsMenu()
{
    if(ui->frameSubmenuImport3->isHidden())
        ui->frameSubmenuImport3->show();

    if(ui->toolButtonImportFromDB->isHidden())
        ui->toolButtonImportFromDB->show();

    ui->toolButtonImportFromExcel->hide();
}
//--------------------------------------------------------
void CWidgetMenu::emitImport(CWidgetMenu::ButtonIDType id)
{
    if(m_operation.operation == BUTTON_PROTECTION_IMPORT)
    {
        if(id == BUTTON_IMPORT_FROM_EXCEL)
            emit importProtectionAutomaticFromExcel();
        else if(id == BUTTON_IMPORT_FROM_DATEBASE)
            emit importProtectionAutomaticFromDatabase();
    }
    else if(m_operation.type == BUTTON_JOURNAL_IMPORT)
    {
        QString journal_type;

        if(m_operation.operation == BUTTON_JOURNAL_CRASH_IMPORT)
        {
            journal_type = "CRASH";
        }
        else if(m_operation.operation == BUTTON_JOURNAL_EVENT_IMPORT)
        {
            journal_type = "EVENT";
        }
        else if(m_operation.operation == BUTTON_JOURNAL_HALFHOUR_IMPORT)
        {
            journal_type = "HALFHOUR";
        }
        else if(m_operation.operation == BUTTON_JOURNAL_ISOLATION_IMPORT)
        {
            journal_type = "ISOLATION";
        }

        if(journal_type.isEmpty())
            return;

        if(id == BUTTON_IMPORT_FROM_EXCEL)
        {
            emit importJournalFromExcel(journal_type);
        }
        else if(id == BUTTON_IMPORT_FROM_DATEBASE)
        {
            emit importJournalFromDatabase(journal_type);
        }
    }
    else if(m_operation.type == BUTTON_SETTINGS_IMPORT)
    {
        QString type;

        if(m_operation.operation == BUTTON_SETTINGS_ANALOG_INPUT_IMPORT)
        {
            type = "ANALOG";
        }
        else if(m_operation.operation == BUTTON_SETTINGS_DISCRET_INPUT_IMPORT)
        {
            type = "INPUT";
        }
        else if(m_operation.operation == BUTTON_SETTINGS_RELAY_IMPORT)
        {
            type = "RELAY";
        }
        else if(m_operation.operation == BUTTON_SETTINGS_LED_IMPORT)
        {
            type = "LED";
        }
        else if(m_operation.operation == BUTTON_SETTINGS_PROTECTION_BLOCK_IMPORT)
        {
            type = "PROTECTION";
        }

        if(type.isEmpty())
            return;

        if(id == BUTTON_IMPORT_FROM_EXCEL)
        {
            emit importSettingsFromExcel(type);
        }
        else if(id == BUTTON_IMPORT_FROM_DATEBASE)
        {
            emit importSettingsFromDatabase(type);
        }
    }

    m_operation = OperationType({ BUTTON_NONE, BUTTON_NONE });
}
//--------------------------
void CWidgetMenu::initMenu()
{
    // Кнопки основного меню
    ui->toolButtonNewProject->setID(BUTTON_NEW_PROJECT);
    ui->toolButtonOpenProject->setID(BUTTON_OPEN_PROJECT);
    ui->toolButtonSaveProject->setID(BUTTON_SAVE_PROJECT);
    ui->toolButtonSaveAsProject->setID(BUTTON_SAVE_AS_PROJECT);
    ui->toolButtonExportProject->setID(BUTTON_EXPORT_PROJECT);
    ui->toolButtonImportProject->setID(BUTTON_IMPORT_PROJECT);
    ui->toolButtonSettings->setID(BUTTON_SETTINGS_PROJECT);
    ui->toolButtonCloseProject->setID(BUTTON_CLOSE_PROJECT);
    ui->toolButtonSettingDebug->setID(BUTTON_SETTING_DEBUG);
    ui->toolButtonExit->setID(BUTTON_EXIT_APP);

    QAction *actionNewProject = new QAction(QIcon(":/images/resource/images/new.png"), tr("Новый"), this);
    QAction *actionOpenProject = new QAction(QIcon(":/images/resource/images/open.png"), tr("Открыть"), this);
    QAction *actionSaveProject = new QAction(QIcon(":/images/resource/images/save.png"), tr("Сохранить"), this);
    QAction *actionSaveAsProject = new QAction(QIcon(":/images/resource/images/save_as.png"), tr("Сохранить как..."), this);
    QAction *actionExportProject = new QAction(QIcon(":/images/resource/images/export.png"), tr("Экспорт"), this);
    QAction *actionImportProject = new QAction(QIcon(":/images/resource/images/import.png"), tr("Импорт"), this);
    QAction *actionSettinsProject = new QAction(QIcon(":/images/resource/images/settings.png"), tr("Настройки"), this);
    QAction *actionClosedProject = new QAction(QIcon(":/images/resource/images/close_button.png"), tr("Закрыть"), this);
    QAction *actionExit = new QAction(QIcon(":/images/resource/images/exit.png"), tr("Выход"), this);

    ui->toolButtonNewProject->setDefaultAction(actionNewProject);
    ui->toolButtonOpenProject->setDefaultAction(actionOpenProject);
    ui->toolButtonSaveProject->setDefaultAction(actionSaveProject);
    ui->toolButtonSaveAsProject->setDefaultAction(actionSaveAsProject);
    ui->toolButtonExportProject->setDefaultAction(actionExportProject);
    ui->toolButtonImportProject->setDefaultAction(actionImportProject);
    ui->toolButtonSettings->setDefaultAction(actionSettinsProject);
    ui->toolButtonCloseProject->setDefaultAction(actionClosedProject);
    ui->toolButtonExit->setDefaultAction(actionExit);

    connect(ui->toolButtonNewProject, &CToolButton::clicked, this, &CWidgetMenu::clicked);
    connect(ui->toolButtonOpenProject, &CToolButton::clicked, this, &CWidgetMenu::clicked);
    connect(ui->toolButtonSaveProject, &CToolButton::clicked, this, &CWidgetMenu::clicked);
    connect(ui->toolButtonSaveAsProject, &CToolButton::clicked, this, &CWidgetMenu::clicked);
    connect(ui->toolButtonCloseProject, &CToolButton::clicked, this, &CWidgetMenu::clicked);
    connect(ui->toolButtonSettingDebug, &CToolButton::clicked, this, &CWidgetMenu::clicked);
    connect(ui->toolButtonExit, &CToolButton::clicked, this, &CWidgetMenu::clicked);

    connect(ui->toolButtonNewProject, &CToolButton::hovered, this, &CWidgetMenu::hoverChanged);
    connect(ui->toolButtonOpenProject, &CToolButton::hovered, this, &CWidgetMenu::hoverChanged);
    connect(ui->toolButtonSaveProject, &CToolButton::hovered, this, &CWidgetMenu::hoverChanged);
    connect(ui->toolButtonSaveAsProject, &CToolButton::hovered, this, &CWidgetMenu::hoverChanged);
    connect(ui->toolButtonCloseProject, &CToolButton::hovered, this, &CWidgetMenu::hoverChanged);
    connect(ui->toolButtonExportProject, &CToolButton::hovered, this, &CWidgetMenu::hoverChanged);
    connect(ui->toolButtonImportProject, &CToolButton::hovered, this, &CWidgetMenu::hoverChanged);
    connect(ui->toolButtonSettings, &CToolButton::hovered, this, &CWidgetMenu::hoverChanged);

    ui->toolButtonExportProject->setCheckable(true);
    ui->toolButtonImportProject->setCheckable(true);
    ui->toolButtonSettings->setCheckable(true);

    ui->toolButtonExportProject->setButtonMenu(true, 1);
    ui->toolButtonImportProject->setButtonMenu(true, 1);
    ui->toolButtonSettings->setButtonMenu(true, 1);

    // Меню Экспорта
    ui->toolButtonProtectionExport->setButtonMenu(true, 2);
    ui->toolButtonJournalExport->setButtonMenu(true, 2);
    ui->toolButtonSettingsExport->setButtonMenu(true, 2);
    ui->toolButtonJournalCrashExport->setButtonMenu(true, 3);
    ui->toolButtonJournalEventExport->setButtonMenu(true, 3);
    ui->toolButtonJournalHalfhourExport->setButtonMenu(true, 3);
    ui->toolButtonJournalIsolationExport->setButtonMenu(true, 3);
    ui->toolButtonSettingsAnalogInputExport->setButtonMenu(true, 3);
    ui->toolButtonSettingsDiscretInputExport->setButtonMenu(true, 3);
    ui->toolButtonSettingsRelayExport->setButtonMenu(true, 3);
    ui->toolButtonSettingsLedExport->setButtonMenu(true, 3);
    ui->toolButtonSettingsProtectionBlockExport->setButtonMenu(true, 3);
    ui->toolButtonProtectionExport->setID(BUTTON_PROTECTION_EXPORT);
    ui->toolButtonJournalExport->setID(BUTTON_JOURNAL_EXPORT);
    ui->toolButtonSettingsExport->setID(BUTTON_SETTINGS_EXPORT);
    ui->toolButtonJournalCrashExport->setID(BUTTON_JOURNAL_CRASH_EXPORT);
    ui->toolButtonJournalEventExport->setID(BUTTON_JOURNAL_EVENT_EXPORT);
    ui->toolButtonJournalHalfhourExport->setID(BUTTON_JOURNAL_HALFHOUR_EXPORT);
    ui->toolButtonJournalIsolationExport->setID(BUTTON_JOURNAL_ISOLATION_EXPORT);
    ui->toolButtonSettingsAnalogInputExport->setID(BUTTON_SETTINGS_ANALOG_INPUT_EXPORT);
    ui->toolButtonSettingsDiscretInputExport->setID(BUTTON_SETTINGS_DISCRET_INPUT_EXPORT);
    ui->toolButtonSettingsRelayExport->setID(BUTTON_SETTINGS_RELAY_EXPORT);
    ui->toolButtonSettingsLedExport->setID(BUTTON_SETTINGS_LED_EXPORT);
    ui->toolButtonSettingsProtectionBlockExport->setID(BUTTON_SETTINGS_PROTECTION_BLOCK_EXPORT);

    m_button_menu_list << ui->toolButtonExportProject << ui->toolButtonImportProject << ui->toolButtonSettings <<
                          ui->toolButtonProtectionExport << ui->toolButtonJournalExport << ui->toolButtonSettingsExport <<
                          ui->toolButtonJournalCrashExport << ui->toolButtonJournalEventExport << ui->toolButtonJournalHalfhourExport <<
                          ui->toolButtonJournalIsolationExport << ui->toolButtonSettingsAnalogInputExport <<
                          ui->toolButtonSettingsDiscretInputExport << ui->toolButtonSettingsRelayExport << ui->toolButtonSettingsLedExport <<
                          ui->toolButtonSettingsProtectionBlockExport << ui->toolButtonProtectionImport << ui->toolButtonJournalImport <<
                          ui->toolButtonSettingsImport << ui->toolButtonJournalCrashImport << ui->toolButtonJournalEventImport <<
                          ui->toolButtonJournalHalfhourImport << ui->toolButtonJournalIsolationImport <<
                          ui->toolButtonSettingsAnalogInputImport << ui->toolButtonSettingsDiscretInputImport <<
                          ui->toolButtonSettingsRelayImport << ui->toolButtonSettingsLedImport <<
                          ui->toolButtonSettingsProtectionBlockImport;

    for(CToolButton *button: m_button_menu_list)
    {
        if(button)
        {
            connect(button, &CToolButton::updateButtons, this, &CWidgetMenu::updateButtonGroup);
        }
    }

    ui->frameSubmenu1->hide();
    ui->frameSubmenu2->hide();
    ui->frameSubmenu3->hide();

    QAction *actionProtectionExportToExcel = new QAction(QIcon(":/images/resource/images/export_excel.png"), tr("Экспорт в Excel"), this);
    QAction *actionProtectionExportToPDF = new QAction(QIcon(":/images/resource/images/export_pdf.png"), tr("Экспорт в PDF"), this);
    QAction *actionProtectionExportToDB = new QAction(QIcon(":/images/resource/images/db_export.png"), tr("Экспорт в БД"), this);

    ui->toolButtonExptortToExcel->setDefaultAction(actionProtectionExportToExcel);
    ui->toolButtonExptortToPDF->setDefaultAction(actionProtectionExportToPDF);
    ui->toolButtonExptortToDB->setDefaultAction(actionProtectionExportToDB);

    ui->toolButtonExptortToExcel->setID(BUTTON_EXPORT_TO_EXCEL);
    ui->toolButtonExptortToPDF->setID(BUTTON_EXPORT_TO_PDF);
    ui->toolButtonExptortToDB->setID(BUTTON_EXPORT_TO_DATEBASE);

    connect(ui->toolButtonExptortToExcel, &CToolButton::clicked, this, &CWidgetMenu::clicked);
    connect(ui->toolButtonExptortToPDF, &CToolButton::clicked, this, &CWidgetMenu::clicked);
    connect(ui->toolButtonExptortToDB, &CToolButton::clicked, this, &CWidgetMenu::clicked);

    connect(ui->toolButtonProtectionExport, &CToolButton::hovered, this, &CWidgetMenu::hoverChanged);
    connect(ui->toolButtonJournalExport, &CToolButton::hovered, this, &CWidgetMenu::hoverChanged);
    connect(ui->toolButtonSettingsExport, &CToolButton::hovered, this, &CWidgetMenu::hoverChanged);
    connect(ui->toolButtonJournalCrashExport, &CToolButton::hovered, this, &CWidgetMenu::hoverChanged);
    connect(ui->toolButtonJournalEventExport, &CToolButton::hovered, this, &CWidgetMenu::hoverChanged);
    connect(ui->toolButtonJournalHalfhourExport, &CToolButton::hovered, this, &CWidgetMenu::hoverChanged);
    connect(ui->toolButtonJournalIsolationExport, &CToolButton::hovered, this, &CWidgetMenu::hoverChanged);

    connect(ui->toolButtonSettingsAnalogInputExport, &CToolButton::hovered, this, &CWidgetMenu::hoverChanged);
    connect(ui->toolButtonSettingsDiscretInputExport, &CToolButton::hovered, this, &CWidgetMenu::hoverChanged);
    connect(ui->toolButtonSettingsRelayExport, &CToolButton::hovered, this, &CWidgetMenu::hoverChanged);
    connect(ui->toolButtonSettingsLedExport, &CToolButton::hovered, this, &CWidgetMenu::hoverChanged);
    connect(ui->toolButtonSettingsProtectionBlockExport, &CToolButton::hovered, this, &CWidgetMenu::hoverChanged);

    // Меню Импорта
    ui->toolButtonProtectionImport->setButtonMenu(true, 2);
    ui->toolButtonJournalImport->setButtonMenu(true, 2);
    ui->toolButtonSettingsImport->setButtonMenu(true, 2);
    ui->toolButtonJournalCrashImport->setButtonMenu(true, 3);
    ui->toolButtonJournalEventImport->setButtonMenu(true, 3);
    ui->toolButtonJournalHalfhourImport->setButtonMenu(true, 3);
    ui->toolButtonJournalIsolationImport->setButtonMenu(true, 3);
    ui->toolButtonSettingsAnalogInputImport->setButtonMenu(true, 3);
    ui->toolButtonSettingsDiscretInputImport->setButtonMenu(true, 3);
    ui->toolButtonSettingsRelayImport->setButtonMenu(true, 3);
    ui->toolButtonSettingsLedImport->setButtonMenu(true, 3);
    ui->toolButtonSettingsProtectionBlockImport->setButtonMenu(true, 3);
    ui->toolButtonProtectionImport->setID(BUTTON_PROTECTION_IMPORT);
    ui->toolButtonJournalImport->setID(BUTTON_JOURNAL_IMPORT);
    ui->toolButtonSettingsImport->setID(BUTTON_SETTINGS_IMPORT);
    ui->toolButtonJournalCrashImport->setID(BUTTON_JOURNAL_CRASH_IMPORT);
    ui->toolButtonJournalEventImport->setID(BUTTON_JOURNAL_EVENT_IMPORT);
    ui->toolButtonJournalHalfhourImport->setID(BUTTON_JOURNAL_HALFHOUR_IMPORT);
    ui->toolButtonJournalIsolationImport->setID(BUTTON_JOURNAL_ISOLATION_IMPORT);
    ui->toolButtonSettingsAnalogInputImport->setID(BUTTON_SETTINGS_ANALOG_INPUT_IMPORT);
    ui->toolButtonSettingsDiscretInputImport->setID(BUTTON_SETTINGS_DISCRET_INPUT_IMPORT);
    ui->toolButtonSettingsRelayImport->setID(BUTTON_SETTINGS_RELAY_IMPORT);
    ui->toolButtonSettingsLedImport->setID(BUTTON_SETTINGS_LED_IMPORT);
    ui->toolButtonSettingsProtectionBlockImport->setID(BUTTON_SETTINGS_PROTECTION_BLOCK_IMPORT);

    ui->frameSubmenuImport1->hide();
    ui->frameSubmenuImport2->hide();
    ui->frameSubmenuImport3->hide();

    QAction *actionProtectionImportFromExcel = new QAction(QIcon(":/images/resource/images/import_excel.png"), tr("Импорт из Excel"), this);
    QAction *actionProtectionImportFromDB = new QAction(QIcon(":/images/resource/images/db_import.png"), tr("Импорт из БД"), this);

    ui->toolButtonImportFromExcel->setDefaultAction(actionProtectionImportFromExcel);
    ui->toolButtonImportFromDB->setDefaultAction(actionProtectionImportFromDB);

    ui->toolButtonImportFromExcel->setID(BUTTON_IMPORT_FROM_EXCEL);
    ui->toolButtonImportFromDB->setID(BUTTON_IMPORT_FROM_DATEBASE);

    connect(ui->toolButtonImportFromExcel, &CToolButton::clicked, this, &CWidgetMenu::clicked);
    connect(ui->toolButtonImportFromDB, &CToolButton::clicked, this, &CWidgetMenu::clicked);

    connect(ui->toolButtonProtectionImport, &CToolButton::hovered, this, &CWidgetMenu::hoverChanged);
    connect(ui->toolButtonJournalImport, &CToolButton::hovered, this, &CWidgetMenu::hoverChanged);
    connect(ui->toolButtonSettingsImport, &CToolButton::hovered, this, &CWidgetMenu::hoverChanged);
    connect(ui->toolButtonJournalCrashImport, &CToolButton::hovered, this, &CWidgetMenu::hoverChanged);
    connect(ui->toolButtonJournalEventImport, &CToolButton::hovered, this, &CWidgetMenu::hoverChanged);
    connect(ui->toolButtonJournalHalfhourImport, &CToolButton::hovered, this, &CWidgetMenu::hoverChanged);
    connect(ui->toolButtonJournalIsolationImport, &CToolButton::hovered, this, &CWidgetMenu::hoverChanged);

    connect(ui->toolButtonSettingsAnalogInputImport, &CToolButton::hovered, this, &CWidgetMenu::hoverChanged);
    connect(ui->toolButtonSettingsDiscretInputImport, &CToolButton::hovered, this, &CWidgetMenu::hoverChanged);
    connect(ui->toolButtonSettingsRelayImport, &CToolButton::hovered, this, &CWidgetMenu::hoverChanged);
    connect(ui->toolButtonSettingsLedImport, &CToolButton::hovered, this, &CWidgetMenu::hoverChanged);
    connect(ui->toolButtonSettingsProtectionBlockImport, &CToolButton::hovered, this, &CWidgetMenu::hoverChanged);

    ui->stackedWidgetMenuLevel1->setCurrentIndex(0);

    ui->toolButtonSaveProject->setEnabled(false);
    ui->toolButtonSaveAsProject->setEnabled(false);
    ui->toolButtonExportProject->setEnabled(false);
    ui->toolButtonImportProject->setEnabled(false);
    ui->toolButtonSettings->setEnabled(true);
}
//----------------------------------
void CWidgetMenu::backlightControl()
{

}
//-------------------------
void CWidgetMenu::clicked()
{
    CToolButton* button = qobject_cast<CToolButton*>(sender());

    if(!button)
        return;

    ButtonIDType button_id = static_cast<ButtonIDType>(button->id());

    switch(button_id)
    {
        case BUTTON_NEW_PROJECT:
            emit newProject();
        break;

        case BUTTON_OPEN_PROJECT:
            emit openProject();
        break;

        case BUTTON_SAVE_PROJECT:
            emit saveProject();
        break;

        case BUTTON_SAVE_AS_PROJECT:
            emit saveAsProject();
        break;

        case BUTTON_CLOSE_PROJECT:
            emit closeProject();
        break;

        case BUTTON_EXIT_APP:
            emit closeWindow();
        break;

        case BUTTON_SETTINGS_PROJECT:
            emit settings();
        break;

        case BUTTON_EXPORT_TO_EXCEL:
        case BUTTON_EXPORT_TO_DATEBASE:
        case BUTTON_EXPORT_TO_PDF:
            emitExport(button_id);
        break;

        case BUTTON_IMPORT_FROM_EXCEL:
        case BUTTON_IMPORT_FROM_DATEBASE:
            emitImport(button_id);
        break;

        case BUTTON_SETTING_DEBUG:
            emit settings();
        break;

        default: qDebug() << QString("ID кнопки не определено: %1").arg(button_id); break;
    }
}
//------------------------------------
void CWidgetMenu::hoverChanged(int id)
{
    if(!m_isButtonActive)
        return;

    switch (id)
    {
        case BUTTON_EXPORT_PROJECT:
            ui->toolButtonExportProject->setChecked(false);
            ui->toolButtonImportProject->setChecked(false);
            ui->toolButtonSettings->setChecked(false);
            ui->stackedWidgetMenuLevel1->setCurrentIndex(1);
            ui->frameSubmenu1->hide();
            ui->frameSubmenu2->hide();
            ui->frameSubmenu3->hide();
            ui->toolButtonExportProject->setChecked(true);
            m_operation.type = id;
        break;

        case BUTTON_PROTECTION_EXPORT:
            exportProtection();
            m_operation.operation = id;
        break;

        case BUTTON_JOURNAL_EXPORT:
            exportJournal();
            m_operation.type = id;
        break;

        case BUTTON_JOURNAL_CRASH_EXPORT:
        case BUTTON_JOURNAL_EVENT_EXPORT:
        case BUTTON_JOURNAL_HALFHOUR_EXPORT:
        case BUTTON_JOURNAL_ISOLATION_EXPORT:
            exportJournalMenu();
            m_operation.operation = id;
        break;

        case BUTTON_SETTINGS_EXPORT:
            exportSettings();
            m_operation.type = id;
        break;

        case BUTTON_SETTINGS_ANALOG_INPUT_EXPORT:
            exportSettingsAnalogInput();
            m_operation.operation = id;
        break;

        case BUTTON_SETTINGS_DISCRET_INPUT_EXPORT:
        case BUTTON_SETTINGS_RELAY_EXPORT:
        case BUTTON_SETTINGS_LED_EXPORT:
        case BUTTON_SETTINGS_PROTECTION_BLOCK_EXPORT:
            exportSettingsMenu();
            m_operation.operation = id;
        break;

        case BUTTON_IMPORT_PROJECT:
            ui->toolButtonExportProject->setChecked(false);
            ui->toolButtonImportProject->setChecked(false);
            ui->toolButtonSettings->setChecked(false);
            ui->stackedWidgetMenuLevel1->setCurrentIndex(2);
            ui->frameSubmenuImport1->hide();
            ui->frameSubmenuImport2->hide();
            ui->frameSubmenuImport3->hide();
            ui->toolButtonImportProject->setChecked(true);
            m_operation.type = id;
        break;

        case BUTTON_PROTECTION_IMPORT:
            importProtection();
            m_operation.operation = id;
        break;

        case BUTTON_JOURNAL_IMPORT:
            importJournal();
            m_operation.type = id;
        break;

        case BUTTON_JOURNAL_CRASH_IMPORT:
        case BUTTON_JOURNAL_EVENT_IMPORT:
        case BUTTON_JOURNAL_HALFHOUR_IMPORT:
        case BUTTON_JOURNAL_ISOLATION_IMPORT:
            importJournalMenu();
            m_operation.operation = id;
        break;

        case BUTTON_SETTINGS_IMPORT:
            importSettings();
            m_operation.type = id;
        break;

        case BUTTON_SETTINGS_ANALOG_INPUT_IMPORT:
            importSettingsAnalogInput();
            m_operation.operation = id;
        break;

        case BUTTON_SETTINGS_DISCRET_INPUT_IMPORT:
        case BUTTON_SETTINGS_RELAY_IMPORT:
        case BUTTON_SETTINGS_LED_IMPORT:
        case BUTTON_SETTINGS_PROTECTION_BLOCK_IMPORT:
            importSettingsMenu();
            m_operation.operation = id;
        break;

        case BUTTON_SETTINGS_PROJECT:
            ui->toolButtonExportProject->setChecked(false);
            ui->toolButtonImportProject->setChecked(false);
            ui->toolButtonSettings->setChecked(false);
            ui->stackedWidgetMenuLevel1->setCurrentIndex(3);
            ui->toolButtonSettings->setChecked(true);
        break;

        default:
            ui->stackedWidgetMenuLevel1->setCurrentIndex(0);
            ui->toolButtonExportProject->setChecked(false);
            ui->toolButtonImportProject->setChecked(false);
            ui->toolButtonSettings->setChecked(false);
        break;
    }
}
