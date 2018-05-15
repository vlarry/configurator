#ifndef CDEBUGINFO_H
    #define CDEBUGINFO_H
    //----------------
    #include <QWidget>
    #include <QDebug>
    //----------
    namespace Ui
    {
        class CDebugInfo;
    }
    //------------------------------
    class CDebugInfo: public QWidget
    {
        Q_OBJECT

        public:
            explicit CDebugInfo(const QString& title, QWidget* parent = nullptr);
            ~CDebugInfo();

            void setHedears(const QStringList& rows, const QStringList& columns);
            void setData(int channel, const QVector<quint16>& data);

        public slots:
            void readInfoCtrl(bool state);
            void timeoutChanged(int newTime);

        signals:
            void readInfo(int, bool = false);
            void closeWindow(bool = false);

        protected:
            void closeEvent(QCloseEvent* event);

        private:
            Ui::CDebugInfo* ui;
    };
#endif // CDEBUGINFO_H
