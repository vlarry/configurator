#include "widgetmenu.h"
#include "ui_widgetmenu.h"
//----------------------------------------
CWidgetMenu::CWidgetMenu(QWidget* parent):
    QWidget(parent),
    ui(new Ui::CWidgetMenu)
{
    ui->setupUi(this);

    ui->toolButtonNewProject->setID(BUTTON_NEW_PROJECT);
    ui->toolButtonOpenProject->ssetID(BUTTON_OPEN_PROJECT);
    ui->toolButtonSaveProject->setID(BUTTON_SAVE_PROJECT);
    ui->toolButtonSaveAsProject->setID(BUTTON_SAVE_AS_PROJECT);
    ui->toolButtonExportProject->setID(BUTTON_EXPORT_PROJECT);
    ui->toolButtonImportProject->setID(BUTTON_IMPORT_PROJECT);
    ui->toolButtonSettings->setID(BUTTON_SETTINGS_PROJECT);
    ui->toolButtonCloseProject->setID(BUTTON_CLOSE_PROJECT);

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

    connect(actionNewProject, &QAction::triggered, this, &CWidgetMenu::clicked);
    connect(actionOpenProject, &QAction::triggered, this, &CWidgetMenu::clicked);
    connect(actionSaveProject, &QAction::triggered, this, &CWidgetMenu::clicked);
    connect(actionSaveAsProject, &QAction::triggered, this, &CWidgetMenu::clicked);
    connect(actionExportProject, &QAction::triggered, this, &CWidgetMenu::clicked);
    connect(actionImportProject, &QAction::triggered, this, &CWidgetMenu::clicked);
    connect(actionSettinsProject, &QAction::triggered, this, &CWidgetMenu::clicked);
    connect(actionClosedProject, &QAction::triggered, this, &CWidgetMenu::clicked);
    connect(actionExit, &QAction::triggered, this, &CWidgetMenu::clicked);
}
//-------------------------
CWidgetMenu::~CWidgetMenu()
{
    delete ui;
}
//---------------------------------------------------
void CWidgetMenu::addOpenDocument(const QString& doc)
{
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
}
//------------------------------------------------------------
bool CWidgetMenu::eventFilter(QObject *watched, QEvent *event)
{
    return false;
}
//----------------------------------
void CWidgetMenu::exportProtection()
{
//    ui->toolButtonExportToDB->show();
//    ui->toolButtonExportToExcel->show();
//    ui->stackedWidgetMenuLevel3->show();
//    ui->frameMenuLevel2->hide();
//    ui->frameMenuLevel3->show();
}
//-----------------------------------
void CWidgetMenu::exportJournalMenu()
{
//    ui->stackedWidgetMenuLevel2->show();
//    ui->frameMenuLevel2->show();
//    ui->stackedWidgetMenuLevel2->setCurrentIndex(0);
}
//-------------------------------
void CWidgetMenu::exportJournal()
{
//    ui->toolButtonExportToDB->show();
//    ui->toolButtonExportToPDF->show();
//    ui->toolButtonExportToExcel->hide();
//    ui->stackedWidgetMenuLevel3->show();
//    ui->frameMenuLevel3->show();
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

        case BUTTON_EXPORT_PROJECT:
            ui->stackedWidgetMenuLevel1->setCurrentIndex(1);
        break;

        case BUTTON_IMPORT_PROJECT:
            ui->stackedWidgetMenuLevel1->setCurrentIndex(2);
        break;

        case BUTTON_CLOSE_PROJECT:
            emit closeProject();
        break;

        case BUTTON_SETTINGS_PROJECT:
            ui->stackedWidgetMenuLevel1->setCurrentIndex(3);
        break;

        case BUTTON_EXIT_APP:
        break;

        default: qDebug() << QString("ID кнопки не определено: %1").arg(button_id); break;
    }
}
//------------------------------------
void CWidgetMenu::hoverChanged(int id)
{
    switch (id)
    {
        case IMPORTPROJECT:
        case EXPORTTOEXCELPROJECT:
        case IMPORTFROMEXCELPROJECT:
        case EXPORTTOPDFPROJECT:
        case SETTINGS:

        break;

        case EXPORTPROJECT:

        break;

        case PROTECTION_EXPORT:
            exportProtection();
        break;

        case JOURNAL_MENU:
            exportJournalMenu();
        break;

        case JOURNAL_CRASH_EXPORT:
        case JOURNAL_EVENT_EXPORT:
        case JOURNAL_HALFHOUR_EXPORT:
        case JOURNAL_ISOLATION_EXPORT:
            exportJournal();
        break;
    }
}
