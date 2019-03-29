#include "widgetmenu.h"
#include "ui_widgetmenu.h"
//----------------------------------------
CWidgetMenu::CWidgetMenu(QWidget* parent):
    QWidget(parent),
    ui(new Ui::CWidgetMenu)
{
    ui->setupUi(this);

    ui->toolButtonNewProject->setID(NEWPROJECT);
    ui->toolButtonOpenProject->setID(OPENPROJECT);
    ui->toolButtonSaveProject->setID(SAVEPROJECT);
    ui->toolButtonSaveAsProject->setID(SAVEASPROJECT);
    ui->toolButtonExportProject->setID(EXPORTPROJECT);
    ui->toolButtonExportToPDF->setID(EXPORTTOPDFPROJECT);
    ui->toolButtonExportToExcel->setID(EXPORTTOEXCELPROJECT);
    ui->toolButtonImportProject->setID(IMPORTPROJECT);
    ui->toolButtonImportFromExcel->setID(IMPORTFROMEXCELPROJECT);
    ui->toolButtonCloseProject->setID(CLOSEPROJECT);
    ui->toolButtonExit->setID(EXITAPPLICATION);
    ui->toolButtonSettings->setID(SETTINGS);
    ui->toolButtonSettingDebug->setID(SETTINGDEBUG);

    ui->toolButtonProtectionExport->setID(PROTECTION_EXPORT);
    ui->toolButtonJournalExport->setID(JOURNAL_EXPORT);
    ui->toolButtonSettingsExport->setID(SETTINGS_EXPORT);

    ui->toolButtonNewProject->installEventFilter(this);
    ui->toolButtonOpenProject->installEventFilter(this);
    ui->toolButtonSaveProject->installEventFilter(this);
    ui->toolButtonSaveAsProject->installEventFilter(this);
    ui->toolButtonExportProject->installEventFilter(this);
    ui->toolButtonImportProject->installEventFilter(this);
    ui->toolButtonCloseProject->installEventFilter(this);
    ui->toolButtonSettings->installEventFilter(this);
    ui->toolButtonProtectionExport->installEventFilter(this);
    ui->toolButtonJournalExport->installEventFilter(this);
    ui->toolButtonSettingsExport->installEventFilter(this);

    ui->toolButtonNewProject->setShortcut(QKeySequence("CTRL+N"));
    ui->toolButtonOpenProject->setShortcut(QKeySequence("CTRL+O"));
    ui->toolButtonSaveProject->setShortcut(QKeySequence("CTRL+S"));

    QMenu* menu = new QMenu(this);

    ui->toolButtonExportProject->setMenu(menu);
    ui->toolButtonExportProject->setCheckable(false);

    ui->toolButtonImportProject->setMenu(menu);
    ui->toolButtonImportProject->setCheckable(false);

    ui->toolButtonSettings->setMenu(menu);
    ui->toolButtonSettings->setCheckable(false);

    ui->toolButtonProtectionExport->setMenu(menu);
    ui->toolButtonProtectionExport->setCheckable(false);

    ui->toolButtonJournalExport->setMenu(menu);
    ui->toolButtonJournalExport->setCheckable(false);

    ui->toolButtonSettingsExport->setMenu(menu);
    ui->toolButtonSettingsExport->setCheckable(false);

    ui->listWidgetOpenDocument->setPalette(palette());
    ui->listWidgetOpenDocument->hide();

    ui->stackedWidgetMenuLevel1->setCurrentIndex(0);
    ui->stackedWidgetMenuLevel2->hide();
    ui->stackedWidgetMenuLevel3->hide();

    connect(ui->toolButtonExit, &CToolButton::clicked, this, &CWidgetMenu::closeWindow);
    connect(ui->toolButtonNewProject, &CToolButton::clicked, this, &CWidgetMenu::clicked);
    connect(ui->toolButtonOpenProject, &CToolButton::clicked, this, &CWidgetMenu::clicked);
    connect(ui->toolButtonSaveProject, &CToolButton::clicked, this, &CWidgetMenu::clicked);
    connect(ui->toolButtonSaveAsProject, &CToolButton::clicked, this, &CWidgetMenu::clicked);
    connect(ui->toolButtonExportToPDF, &CToolButton::clicked, this, &CWidgetMenu::clicked);
    connect(ui->toolButtonExportToExcel, &CToolButton::clicked, this, &CWidgetMenu::clicked);
    connect(ui->toolButtonImportFromExcel, &CToolButton::clicked, this, &CWidgetMenu::clicked);
    connect(ui->toolButtonCloseProject, &CToolButton::clicked, this, &CWidgetMenu::clicked);
    connect(ui->toolButtonExportProject, &CToolButton::clicked, this, &CWidgetMenu::clicked);
    connect(ui->toolButtonImportProject, &CToolButton::clicked, this, &CWidgetMenu::clicked);
    connect(ui->toolButtonSettings, &CToolButton::clicked, this, &CWidgetMenu::clicked);
    connect(ui->toolButtonSettingDebug, &CToolButton::clicked, this, &CWidgetMenu::clicked);

    connect(ui->toolButtonProtectionExport, &CToolButton::clicked, this, &CWidgetMenu::clicked);
    connect(ui->toolButtonJournalExport, &CToolButton::clicked, this, &CWidgetMenu::clicked);
    connect(ui->toolButtonSettingsExport, &CToolButton::clicked, this, &CWidgetMenu::clicked);

    connect(ui->toolButtonImportJournalFromDataBase, &CToolButton::clicked, this, &CWidgetMenu::importJournalFromDataBase);
    connect(ui->toolButtonNewProject, &CToolButton::hovered, this, &CWidgetMenu::hoverChanged);
    connect(ui->toolButtonOpenProject, &CToolButton::hovered, this, &CWidgetMenu::hoverChanged);
    connect(ui->toolButtonSaveProject, &CToolButton::hovered, this, &CWidgetMenu::hoverChanged);
    connect(ui->toolButtonSaveAsProject, &CToolButton::hovered, this, &CWidgetMenu::hoverChanged);
    connect(ui->toolButtonExportToPDF, &CToolButton::hovered, this, &CWidgetMenu::hoverChanged);
    connect(ui->toolButtonExportToExcel, &CToolButton::hovered, this, &CWidgetMenu::hoverChanged);
    connect(ui->toolButtonImportFromExcel, &CToolButton::hovered, this, &CWidgetMenu::hoverChanged);
    connect(ui->toolButtonCloseProject, &CToolButton::hovered, this, &CWidgetMenu::hoverChanged);
    connect(ui->toolButtonExportProject, &CToolButton::hovered, this, &CWidgetMenu::hoverChanged);
    connect(ui->toolButtonImportProject, &CToolButton::hovered, this, &CWidgetMenu::hoverChanged);
    connect(ui->toolButtonSettings, &CToolButton::hovered, this, &CWidgetMenu::hoverChanged);

    connect(ui->toolButtonProtectionExport, &CToolButton::clicked, this, &CWidgetMenu::hoverChanged);
    connect(ui->toolButtonJournalExport, &CToolButton::clicked, this, &CWidgetMenu::hoverChanged);
    connect(ui->toolButtonSettingsExport, &CToolButton::clicked, this, &CWidgetMenu::hoverChanged);

    connect(this, &CWidgetMenu::addDocument, this, &CWidgetMenu::addOpenDocument);

    ui->toolButtonSaveProject->setDisabled(true);
    ui->toolButtonSaveAsProject->setDisabled(true);
    ui->toolButtonExportProject->setDisabled(true);
    ui->toolButtonImportProject->setDisabled(true);
    ui->toolButtonSettings->setDisabled(true);
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
//---------------------------------
QSize CWidgetMenu::sizeHint() const
{
    int w = ui->frameMenuMain->sizeHint().width() + ui->stackedWidgetMenuLevel1->sizeHint().width() + 100;

    if(!ui->stackedWidgetMenuLevel2->isHidden())
    {
        w += ui->stackedWidgetMenuLevel2->sizeHint().width();
    }

    if(!ui->stackedWidgetMenuLevel3->isHidden())
    {
        w += ui->stackedWidgetMenuLevel3->sizeHint().width();
    }

    QSize s = QSize(w, 450);
    qDebug() << "sizeHint: " << s;
    return s;
}
//----------------------------------------
QSize CWidgetMenu::minimumSizeHint() const
{
    return sizeHint();
}
//----------------------------------
void CWidgetMenu::exportProtection()
{
    setFixedSize(QSize(700, 450));/*
    ui->stackedWidgetMenuLevel2->hide();
    ui->stackedWidgetMenuLevel3->show();
    ui->stackedWidgetMenuLevel3->setCurrentIndex(0);*/
    updateGeometry();
    qDebug() << "export protection";
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
        case NEWPROJECT:
            emit newProject();
        break;

        case OPENPROJECT:
            emit openProject();
        break;

        case SAVEPROJECT:
            emit saveProject();
        break;

        case SAVEASPROJECT:
            emit saveAsProject();
        break;

        case EXPORTPROJECT:
            ui->stackedWidgetMenuLevel1->setCurrentIndex(1);
        break;

        case IMPORTPROJECT:
            ui->stackedWidgetMenuLevel1->setCurrentIndex(2);
        break;

        case EXPORTTOPDFPROJECT:
            emit exportToPDFProject();
        break;

        case EXPORTTOEXCELPROJECT:
            emit exportToExcelProject();
        break;

        case IMPORTFROMEXCELPROJECT:
            emit importFromExcelProject();
        break;

        case CLOSEPROJECT:
            emit closeProject();
        break;

        case SETTINGS:
            ui->stackedWidgetMenuLevel1->setCurrentIndex(3);
        break;

        case SETTINGDEBUG:
            emit settings();
        break;

        case PROTECTION_EXPORT:
            exportProtection();
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
            ui->stackedWidgetMenuLevel1->setCurrentIndex(0);
        break;

        case EXPORTPROJECT:
            ui->stackedWidgetMenuLevel1->setCurrentIndex(1);
        break;

        case PROTECTION_EXPORT:
            exportProtection();
        break;
    }
}
