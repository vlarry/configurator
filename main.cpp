#include "configuratorwindow.h"
#include <QApplication>
#include <QMessageLogContext>
//-----------------------------------------------------------------------------------
void logOutput(QtMsgType type, const QMessageLogContext& context, const QString& msg)
{
    QByteArray localMsg = msg.toLocal8Bit();

    QString message;
    QString end_line = "\n";

#ifdef Q_OS_WIN
    end_line = "\r\n";
#endif

    switch (type)
    {
        case QtDebugMsg:
            message = QObject::tr("Отладка: %1 (%2:%3, %4).").arg(msg).arg(context.file).arg(context.line).
                                                              arg(context.function);
        break;

        case QtInfoMsg:
            message = QObject::tr("Сообщение: %1.").arg(msg);
        break;

        case QtWarningMsg:
            message = QObject::tr("Предупреждение: %1 (%2:%3, %4).").arg(msg).arg(context.file).
                                                                     arg(context.line).arg(context.function);
        break;

        case QtCriticalMsg:
            message = QObject::tr("Критическая ошибка: %1 (%2:%3, %4).").arg(msg).arg(context.file).
                                                                         arg(context.line).arg(context.function);
        break;

        case QtFatalMsg:
            message = QObject::tr("Фатальная ошибка: %1 (%2:%3, %4).").arg(msg).arg(context.file).
                                                                       arg(context.line).arg(context.function);
        break;
    }

    QFile file("log.txt");

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
//------------------------------
int main(int argc, char* argv[])
{
    QCoreApplication::setOrganizationName(ORGANIZATION_NAME);
    QCoreApplication::setOrganizationDomain(ORGANIZATION_DOMAIN);

    qInstallMessageHandler(logOutput);

    QApplication a(argc, argv);

    ConfiguratorWindow w;
    w.show();
    
    return a.exec();
}
