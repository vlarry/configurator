#ifndef INTERVALDIALOG_H
    #define INTERVALDIALOG_H
    //----------------
    #include <QDialog>
    //----------
    namespace Ui
    {
        class CIntervalDialog;
    }
    //-----------------------------------
    class CIntervalDialog: public QDialog
    {
        Q_OBJECT

        public:
            explicit CIntervalDialog(const QString& title, QWidget* parent = nullptr);
            ~CIntervalDialog();
            int interval() const;
            bool isRepeatChecked() const;

        signals:
            void repeat(bool);

        private:
            Ui::CIntervalDialog* ui;
    };
#endif // INTERVALDIALOG_H
