#include "widgetmenu.h"
#include "ui_widgetmenu.h"
//----------------------------------------
CWidgetMenu::CWidgetMenu(QWidget* parent):
    QWidget(parent),
    ui(new Ui::CWidgetMenu)
{
    ui->setupUi(this);

    ui->toolButtonNewProject->setProperty("ID", NEWPROJECT);
    ui->toolButtonOpenProject->setProperty("ID", OPENPROJECT);
    ui->toolButtonSaveProject->setProperty("ID", SAVEPROJECT);
    ui->toolButtonSaveAsProject->setProperty("ID", SAVEASPROJECT);
    ui->toolButtonExportProject->setProperty("ID", EXPORTPROJECT);
    ui->toolButtonExportToPDF->setProperty("ID", EXPORTTOPDFPROJECT);
    ui->toolButtonExportToExcel->setProperty("ID", EXPORTTOEXCELPROJECT);
    ui->toolButtonCloseProject->setProperty("ID", CLOSEPROJECT);
    ui->toolButtonExit->setProperty("ID", EXITAPPLICATION);

    ui->toolButtonNewProject->installEventFilter(this);
    ui->toolButtonOpenProject->installEventFilter(this);
    ui->toolButtonSaveProject->installEventFilter(this);
    ui->toolButtonSaveAsProject->installEventFilter(this);
    ui->toolButtonExportProject->installEventFilter(this);
    ui->toolButtonCloseProject->installEventFilter(this);

    ui->toolButtonNewProject->setShortcut(QKeySequence("CTRL+N"));
    ui->toolButtonOpenProject->setShortcut(QKeySequence("CTRL+O"));
    ui->toolButtonSaveProject->setShortcut(QKeySequence("CTRL+S"));

    QMenu* menu = new QMenu(this);

    ui->toolButtonExportProject->setMenu(menu);
    ui->toolButtonExportProject->setCheckable(false);

    connect(ui->toolButtonExit, &QToolButton::clicked, this, &CWidgetMenu::closeWindow);
    connect(ui->toolButtonNewProject, &QToolButton::clicked, this, &CWidgetMenu::clicked);
    connect(ui->toolButtonOpenProject, &QToolButton::clicked, this, &CWidgetMenu::clicked);
    connect(ui->toolButtonSaveProject, &QToolButton::clicked, this, &CWidgetMenu::clicked);
    connect(ui->toolButtonSaveAsProject, &QToolButton::clicked, this, &CWidgetMenu::clicked);
    connect(ui->toolButtonExportToPDF, &QToolButton::clicked, this, &CWidgetMenu::clicked);
    connect(ui->toolButtonExportToExcel, &QToolButton::clicked, this, &CWidgetMenu::clicked);
    connect(ui->toolButtonCloseProject, &QToolButton::clicked, this, &CWidgetMenu::clicked);
}
//-------------------------
CWidgetMenu::~CWidgetMenu()
{
    delete ui;
}
//--------------------------------------------------------
bool CWidgetMenu::eventFilter(QObject* obj, QEvent* event)
{
    if(event->type() == QEvent::HoverEnter)
    {
        QToolButton* button = qobject_cast<QToolButton*>(obj);

        if(button)
        {
            QVariant property_button = button->property("ID");

            if(property_button.isValid())
            {
                ButtonIDType button_id = static_cast<ButtonIDType>(property_button.toInt());

                if(button_id != EXPORTPROJECT)
                    ui->stackedWidgetMenuFunction->setCurrentIndex(0);
                else
                    ui->stackedWidgetMenuFunction->setCurrentIndex(1);

                return true;
            }
        }

        return false;
    }

    return QWidget::eventFilter(obj, event);
}
//-------------------------
void CWidgetMenu::clicked()
{
    QToolButton* button = qobject_cast<QToolButton*>(sender());

    if(!button)
        return;

    ButtonIDType button_id = static_cast<ButtonIDType>(button->property("ID").toInt());

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

        case EXPORTTOPDFPROJECT:
            emit exportToPDFProject();
        break;

        case EXPORTTOEXCELPROJECT:
            emit exportToExcelProject();
        break;

        case CLOSEPROJECT:
            emit closeProject();
        break;

        default: break;
    }
}
