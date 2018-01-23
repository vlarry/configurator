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

            void connectStateChanged(bool state);
            void clearStatusMessage();
            void setSerialNumber(const QString& sn_text);
            void setStatusMessage(const QString& message, int timeout = 0);

        public slots:
            void timeoutStatusMessage();

        private:
            Ui::CStatusBar* ui;
            QTimer*         m_timer;
    };
#endif // CSTATUSBAR_H
