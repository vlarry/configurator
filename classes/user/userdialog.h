#ifndef USERDIALOG_H
    #define USERDIALOG_H
    //----------------
    #include <QDialog>
    //----------
    namespace Ui
    {
        class CUserDialog;
    }
    //-------------------------------
    class CUserDialog: public QDialog
    {
        Q_OBJECT

        public:
            struct user_t
            {
                QString login;
                QString password;
            };

        public:
            explicit CUserDialog(const QStringList& users, QWidget* parent = nullptr);
            ~CUserDialog();

            user_t user() const;
            void setMessage(const QString& text);

        private:
            Ui::CUserDialog* ui;
    };
#endif // USERDIALOG_H
