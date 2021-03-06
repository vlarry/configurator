#include "configuratorwindow.h"
#include <QApplication>
#include <QMessageLogContext>
#include <QTranslator>
#include <QLibraryInfo>
//-----------------------------------------------------------------------------------
void logOutput(QtMsgType type, const QMessageLogContext& context, const QString& msg)
{
    QString message;
    QString end_line = "\n";

#ifdef Q_OS_WIN
    end_line = "\r\n";
#endif

    switch (type)
    {
        case QtDebugMsg:
            if(QLibraryInfo::isDebugBuild())
                message = QObject::tr("Отладка: %1 (%2:%3, %4).").arg(msg).arg(context.file).arg(context.line).arg(context.function);
        break;

        case QtInfoMsg:
            message = QObject::tr("Сообщение: %1.").arg(msg);
        break;

        case QtWarningMsg:
            message = QObject::tr("Предупреждение: %1 (%2:%3, %4).").arg(msg).arg(context.file).arg(context.line).arg(context.function);
        break;

        case QtCriticalMsg:
            message = QObject::tr("Критическая ошибка: %1 (%2:%3, %4).").arg(msg).arg(context.file).arg(context.line).arg(context.function);
        break;

        case QtFatalMsg:
            message = QObject::tr("Фатальная ошибка: %1 (%2:%3, %4).").arg(msg).arg(context.file).arg(context.line).arg(context.function);
        break;
    }

    QDir dir;

    if(!dir.exists("outputs"))
        dir.mkdir("outputs");

    if(!message.isEmpty())
    {
        QFile file("outputs/log.txt");

        if(!file.open(QFile::WriteOnly | QFile::Append))
        {
            qDebug() << QObject::tr("Невозможно открыть файл логирования для записи");
            file.close();

            return;
        }

        QTextStream out(&file);

        QString dt = QDateTime::currentDateTime().toString("[dd.mm.yyyy - HH:mm:ss.zzz]");

        out <<  QString("%1 -> %2").arg(dt).arg(message) << end_line;

        file.close();
    }
}
//------------------------------
int main(int argc, char* argv[])
{
    QCoreApplication::setOrganizationName(ORGANIZATION_NAME);
    QCoreApplication::setOrganizationDomain(ORGANIZATION_DOMAIN);

    qInstallMessageHandler(logOutput);

    QApplication a(argc, argv);
    QTranslator translator;

    translator.load("qt_Ru"/* + QLocale::system().name()*/, QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    a.installTranslator(&translator);

    QFile styleFile(":/styles/resource/styles/default_style.qss");

    styleFile.open(QFile::ReadOnly);

    QString qss_str = styleFile.readAll();

    qApp->setStyleSheet(qss_str);

    styleFile.close();

    ConfiguratorWindow w;
    w.setWindowIcon(QIcon(QPixmap(":/images/resource/images/configurator.png")));
    w.show();
    
    return a.exec();
}
