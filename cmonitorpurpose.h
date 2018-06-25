#ifndef CMONITORPURPOSE_H
    #define CMONITORPURPOSE_H
    //----------------
    #include <QWidget>
    #include <QShowEvent>
    #include <QCloseEvent>
    #include <QTableView>
    #include <QTimer>
    #include "cmatrixpurposemodel.h"
    //----------
    namespace Ui
    {
        class CMonitorPurpose;
    }
    //-----------------------------------
    class CMonitorPurpose: public QWidget
    {
        Q_OBJECT

        public:
            explicit CMonitorPurpose(const QString& text, QWidget* parent = nullptr);
            ~CMonitorPurpose();

            void        setHeaders(const QStringList& row_labels, const QStringList& column_labels);
            QTableView* table() const;

        public slots:
            void stateUpdateData(bool state = false);//
            void timeoutUpdate();//
            void timeoutValueChanged(int new_value);//

        private slots:
            void clearTable();

        protected:
            void closeEvent(QCloseEvent* event);
            void showEvent(QShowEvent* event);//

        signals:
            void closeWindow(bool = false);
            void buttonUpdate();

        private:
            Ui::CMonitorPurpose* ui;
            QTimer*              m_timeout_update;
    };
#endif // CMONITORPURPOSE_H
