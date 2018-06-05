#ifndef CSTATUSINFO_H
    #define CSTATUSINFO_H
    //----------------
    #include <QWidget>
    #include <QMessageBox>
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

        protected:
            void closeEvent(QCloseEvent* event);

        signals:
            void updateStatusInfo();
            void closeWindow(bool = false);

        private:
            Ui::CStatusInfo* ui;
            status_list_t    m_status_list;
    };
#endif // CSTATUSINFO_H
