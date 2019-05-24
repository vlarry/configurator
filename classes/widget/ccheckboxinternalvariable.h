#ifndef CCHECKBOXINTERNALVARIABLE_H
    #define CCHECKBOXINTERNALVARIABLE_H
    //------------------
    #include <QCheckBox>
    #include <QPaintEvent>
    #include <QPainter>
    #include <QTimer>
    //-----------------------------------------------
    class CCheckBoxInternalVariable: public QCheckBox
    {
        private:
            bool m_is_changed;

        public:
            CCheckBoxInternalVariable(const QString &text, QWidget *parent = nullptr);
            void setChangedState(bool state);

        public slots:
            void timeoutStateChanged();

        protected:
            void paintEvent(QPaintEvent *event);
    };
#endif // CCHECKBOXINTERNALVARIABLE_H
