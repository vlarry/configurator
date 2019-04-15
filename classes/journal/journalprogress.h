#ifndef JOURNALPROGRESS_H
    #define JOURNALPROGRESS_H
    //----------------
    #include <QWidget>
    #include <QPushButton>
    //----------
    namespace Ui
    {
        class JournalProgress;
    }
    //-----------------------------------
    class JournalProgress: public QWidget
    {
        Q_OBJECT

        public:
            explicit JournalProgress(int total, const QString &title, QWidget *parent = nullptr);
            ~JournalProgress();
            void setProgressValue(int value);

        private slots:
            void readCancel();

        signals:
            void cancel(bool);

        private:
            Ui::JournalProgress *ui;
            int m_total;
    };
#endif // JOURNALPROGRESS_H
