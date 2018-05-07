#ifndef CTIMEFILTERDIALOG_H
    #define CTIMEFILTERDIALOG_H
    //----------------
    #include <QDialog>
    //----------
    namespace Ui
    {
        class CTimeFilterDialog;
    }
    //-------------------------------------
    class CTimeFilterDialog: public QDialog
    {
        Q_OBJECT

        public:
            explicit CTimeFilterDialog(QWidget* parent = nullptr);
            ~CTimeFilterDialog();

            QTime time() const;

        private:
            Ui::CTimeFilterDialog* ui;
    };
#endif // CTIMEFILTERDIALOG_H
