#ifndef MENUCOMBOBOX_H
    #define MENUCOMBOBOX_H
    //------------------
    #include <QComboBox>
    #include <QWheelEvent>
    //-----------------------------------
    class CMenuComboBox: public QComboBox
    {
        Q_OBJECT

        public:
            CMenuComboBox(QWidget *parent = nullptr);
            int  defaultIndex() const;
            bool isEdit() const;
            void resetIsEdit();
            void resetToDefault();
            void setDefaultIndex(int index);

        public slots:
            void isCurrentIndexChanged(int);

        signals:
            void editValue();

        protected:
            void wheelEvent(QWheelEvent*);

        private:
            bool m_is_edit; // было изменение текущего индекса
            int  m_def; // номер пункта по умолчанию
    };
#endif // MENUCOMBOBOX_H
