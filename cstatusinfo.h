#ifndef CSTATUSINFO_H
    #define CSTATUSINFO_H
    //----------------
    #include <QWidget>
    #include <QCloseEvent>
    #include <QShowEvent>
    #include <QMessageBox>
    #include <QTimer>
    #include "qcell.h"
    //----------
    namespace Ui
    {
        class CStatusInfo;
    }
    //---------------------------------------
    typedef QMap<int, QString> status_list_t;
    //-------------------------------
    class CStatusInfo: public QWidget
    {
        Q_OBJECT

        public:
            explicit CStatusInfo(QWidget* parent = nullptr);
            ~CStatusInfo();
            void setStatusList(status_list_t& list);
            void updateMcpInfo(quint32 value);
            void updateModuleInfo(const QVector<quint16>& info);

        public slots:
            void stateUpdateData(bool state = false);
            void timeoutUpdate();
            void timeoutValueChanged(int new_value);

        protected:
            void closeEvent(QCloseEvent* event);
            void showEvent(QShowEvent* event);

        signals:
            void buttonUpdate();
            void closeWindow(bool = false);

        private:
            Ui::CStatusInfo* ui;
            status_list_t    m_status_list;
            QTimer*          m_timeout_update;
    };
#endif // CSTATUSINFO_H
