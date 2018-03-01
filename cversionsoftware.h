#ifndef CVERSIONSOFTWARE_H
    #define CVERSIONSOFTWARE_H
    //----------------
    #include <QWidget>
    #include <QTextCharFormat>
    //----------
    namespace Ui
    {
        class CVersionSoftware;
    }
    //------------------------------------
    class CVersionSoftware: public QWidget
    {
        Q_OBJECT

        public:
            explicit CVersionSoftware(QWidget* parent = nullptr);
            ~CVersionSoftware();

            void setText(const QVector<QPair<QString, QString> >& data);

        private:
            Ui::CVersionSoftware* ui;
    };

#endif // CVERSIONSOFTWARE_H
