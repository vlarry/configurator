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

            struct RangeType
            {
                float min;
                float max;
                float def;
            };

        public:
            CLineEdit(QWidget* parent = nullptr);
            bool isValidateText(const QString& text) const;
            void setValidatorType(ValidatorType type);
            ValidatorType validatorType() const;
            bool isEdit() const;
            const RangeType& range() const;
            void resetIsEdit();
            void resetToDefault();
            void setRange(RangeType &range);
            void setRangeMin(float min);
            void setRangeMax(float max);
            void setRangeDefault(float def);

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
            RangeType     m_range;
    };
#endif // CLINEEDIT_H
