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
            enum mode_t
            {
                NORMAL_MODE,
                EDIT_MODE
            };

            struct user_t
            {
                QString login;
                QString password;
            };

        public:
            explicit CUserDialog(const QStringList& users, QWidget* parent = nullptr);
            ~CUserDialog();

            user_t user() const;
            user_t currentUser() const;
            void setPasswordMode(mode_t mode);

        private:
            Ui::CUserDialog* ui;
    };
#endif // USERDIALOG_H
