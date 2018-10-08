#ifndef CDOCKPANELITEMCTRL_H
    #define CDOCKPANELITEMCTRL_H
    //--------------------
    #include <QPushButton>
    #include <QWidget>
    #include <QPaintEvent>
    #include <QShowEvent>
    #include <QPainter>
    #include <QTimer>
    #include <QDebug>
    //------------------------------------------
    class CDockPanelItemCtrl: public QPushButton
    {
        public:
            enum DirType
            {
                DirNone,
                DirLeft,
                DirRight,
                DirTop,
                DirBottom
            };

            enum StateType
            {
                Close,
                Open
            };

        public:
            CDockPanelItemCtrl(QWidget* parent = nullptr);
            CDockPanelItemCtrl(const QString& text, QWidget* parent = nullptr);

            DirType  dir() const;
            StateType state() const;
            QString   text() const;
            void      setDir(DirType dir);
            void      setState(StateType state);
            void      setText(const QString& text);

        public slots:
            void timeoutRepaint();

        protected:
            void paintEvent(QPaintEvent* event);
            void showEvent(QShowEvent* event);

        private:
            QString   m_text;
            DirType   m_dir;
            StateType m_state;
            QTimer*   m_timer_repaint;
    };
#endif // CDOCKPANELITEMCTRL_H
