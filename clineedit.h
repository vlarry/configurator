#ifndef CLINEEDIT_H
    #define CLINEEDIT_H
    //------------------
    #include <QLineEdit>
    #include <QFocusEvent>
    #include <QMouseEvent>
    #include <QKeyEvent>
    #include <QLocale>
    #include <QDoubleValidator>
    #include <QDebug>
    //-------------------------------
    class CLineEdit: public QLineEdit
    {
        Q_OBJECT

        public:
            enum ValidatorType
            {
                INT,
                FLOAT
            };

        public:
            CLineEdit(QWidget* parent = nullptr);
            bool isValidateText(const QString& text) const;
            void setValidatorType(ValidatorType type);
            ValidatorType validatorType() const;
            bool isEdit() const;
            void resetIsEdit();

        protected:
            void mouseReleaseEvent(QMouseEvent* event);
            void keyPressEvent(QKeyEvent* event);
            void focusOutEvent(QFocusEvent* event);

        private slots:
            void validateTextChanged(const QString& text);

        signals:
            void editValue();

        private:
            bool          m_focus;
            ValidatorType m_validator_type;
            bool          m_is_edit; // было редактирование ячейки?
    };
#endif // CLINEEDIT_H
