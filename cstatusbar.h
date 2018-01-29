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

            void    connectStateChanged(bool state);
            void    clearSerialNumber();
            void    clearStatusMessage();
            bool    isState() const;
            bool    serialNumberIsEmpty() const;
            QString serialNumberText();
            void    setProgressbarTitle(const QString& title);
            void    setSerialNumber(const QString& sn_text);
            void    setStatusMessage(const QString& message, int timeout = 0);

        public slots:
            void timeoutStatusMessage();
            void timeoutProgressbar();
            void startProgressbar();
            void stopProgressbar();

        private:
            Ui::CStatusBar* ui;
            QTimer*         m_timerStatusMessage;
            QTimer*         m_timerProgressbar;
    };
#endif // CSTATUSBAR_H
