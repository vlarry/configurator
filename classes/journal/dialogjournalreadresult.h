#ifndef DIALOGJOURNALREADRESULT_H
    #define DIALOGJOURNALREADRESULT_H
    //----------------
    #include <QDialog>
    //----------
    namespace Ui
    {
        class DialogJournalReadResult;
    }
    //-------------------------------------------
    class DialogJournalReadResult: public QDialog
    {
        Q_OBJECT

        public:
            explicit DialogJournalReadResult(const QString &title, QWidget *parent = nullptr);
            ~DialogJournalReadResult();
            void setValues(int total, int read, float time);

        private:
            Ui::DialogJournalReadResult *ui;
    };
#endif // DIALOGJOURNALREADRESULT_H
