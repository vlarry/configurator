#ifndef CSTATUSBAR_H
    #define CSTATUSBAR_H
    //----------------
    #include <QWidget>
    #include <QTimer>
    #include <QLabel>
    //----------
    namespace Ui
    {
        class CStatusBar;
    }
    //------------------------------
    class CStatusBar: public QWidget
    {
        Q_OBJECT

        public:
            explicit CStatusBar(QWidget* parent = nullptr);
            ~CStatusBar();

            void    addWidget(QWidget* widget);
            void    connectStateChanged(bool state);
            void    clearSerialNumber();
            void    clearStatusMessage();
            bool    isState() const;
            bool    serialNumberIsEmpty() const;
            QString serialNumberText();
            void    setSerialNumber(const QString& sn_text);
            void    setStatusMessage(const QString& message, int timeout = 0);

        public slots:
            void timeoutStatusMessage();

        private:
            Ui::CStatusBar* ui;
            QTimer*         m_timerStatusMessage;
    };
#endif // CSTATUSBAR_H
