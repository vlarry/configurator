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
            bool isEdit() const;
            void resetIsEdit();

        public slots:
            void isCurrentIndexChanged(int);

        signals:
            void editValue();

        protected:
            void wheelEvent(QWheelEvent*);

        private:
            bool m_is_edit; // было изменение текущего индекса?
    };
#endif // MENUCOMBOBOX_H
