#ifndef CDOCKPANELITEMCTRL_H
    #define CDOCKPANELITEMCTRL_H
    //--------------------
    #include <QPushButton>
    #include <QWidget>
    #include <QPaintEvent>
    #include <QPainter>
    #include <QDebug>
    //------------------------------------------
    class CDockPanelItemCtrl: public QPushButton
    {
        public:
            enum SideType
            {
                Left,
                Right
            };

            enum StateType
            {
                Close,
                Open
            };

        public:
            CDockPanelItemCtrl(QWidget* parent = nullptr);
            CDockPanelItemCtrl(const QString& text, QWidget* parent = nullptr);

            SideType  side() const;
            StateType state() const;
            QString   text() const;
            void      setSide(SideType side);
            void      setState(StateType state);
            void      setText(const QString& text);

        public slots:
            void show();

        protected:
            void paintEvent(QPaintEvent* event);

        private:
            QString   m_text;
            SideType  m_side;
            StateType m_state;
    };
#endif // CDOCKPANELITEMCTRL_H
